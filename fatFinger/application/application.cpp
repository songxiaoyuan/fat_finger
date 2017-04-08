#include "application.h"

application::application(){
    //ctor
}

void application::Init(){
  //初始化traderApi
  CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
  CtpTraderSpi* pTraderSpi = new CtpTraderSpi(pTraderApi);
  pTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderSpi);      // 注册事件类
  pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);         // 注册公有流
  pTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);        // 注册私有流
  pTraderApi->RegisterFront(TRADEFRONT);  // 注册交易前置地址
  pTraderApi->Init();
  ptraderspi_ = pTraderSpi;
  ptraderapi_ = pTraderApi;

  //初始化MDApi
  CThostFtdcMdApi* pMdApi=CThostFtdcMdApi::CreateFtdcMdApi();
  CtpMdSpi* pMdSpi=new CtpMdSpi(pMdApi); //创建回调处理类对象MdSpi
  pMdApi->RegisterSpi(pMdSpi);       // 回调对象注入接口类
  pMdApi->RegisterFront(MDFRONT);        // 注册行情前置地址
  pMdApi->Init();      //接口线程启动, 开始工作
  pmdspi_ = pMdSpi;
  pmdapi_ = pMdApi;
}

void application::Run(){
  int i;
    cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] userLogin        --  Md和Trader登录"<<endl;
    cerr<<" [2] userSubscribe    -- 用户订阅信息,此时会自动创建处理数据线程"<<endl;
     cerr<<"[3] printOrders      -- 查询现在所有订单的状态"<<endl;
     cerr<<"[4] printTrades      -- 查询现在所有成交订单的状态"<<endl;
    cerr<<"[５] CloseALlTrades      -- 平掉所有的仓位"<<endl;
    cerr<<"[６] ReqOrderAction      -- 根据报单编码撤单"<<endl;
    cerr<<" [７]  Exit            -- 退出"<<endl;
    cerr<<"----------------------------------------------"<<endl;
  while(1){
    cin>>i;
    switch(i){
      case 1:{
              //用户登录信息，包括md登录和trader登录
              //MD用户开始登录和订阅信息。
              cout<<"登录MD用户"<<endl;
              MdUserLogin(APPID,USERID,PASSWD);

              //Trader用户开始登录，并且进行结算单确认。
              cout<<"trader用户开始登录"<<endl;
              TraderUserLogin(APPID,USERID,PASSWD);
              cout<<"开始结算单请求并且确认"<<endl;
              //ReqQrySettlementInfo();
              ReqSettlementInfoConfirm();
              break;
            }
      case 2:{
              // 用户订阅信息，此时线程会根据传回来的数据处理是不是乌龙指，
              // 如果发现是乌龙指会自动下单，同时在乌龙指结束的时候自动锁仓。
             SubscribeMarketData(PPINSTRUMENTID,NUMOFINSTRUMENTID);
              break;
            }
      case 3:{
              // 打印现在所有的订单。
              PrintOrders();
              break;
            }
      case 4:{
              // 打印现在所有的已经成交的订单。
              PrintTrades();
              break;
            }
    case 5:{
              // 平掉所有的仓位。
              CloseALlTrades();
              break;
            }
    case 6:{
              // 平掉所有的仓位。
              cout<<"请输入订单编码　"<<endl;
              TThostFtdcSequenceNoType orderSeq;
              cin>>orderSeq;
              ReqOrderAction(orderSeq);
              break;
            }
      case 7:{
              // 程序退出，或者可以做别的操作。
              exit(0);
              break;
            }
    }
  }
  pmdapi_->Join();
  ptraderapi_->Join();
}

void application::MdUserLogin(TThostFtdcBrokerIDType  appId,
                TThostFtdcUserIDType  userId, TThostFtdcPasswordType  passwd){
      CThostFtdcReqUserLoginField req;
      memset(&req, 0, sizeof(req));
      strcpy(req.BrokerID, APPID);
      strcpy(req.UserID, userId);
      strcpy(req.Password, passwd);
      int ret = pmdapi_->ReqUserLogin(&req, ++requestid_);
           cerr<<" 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl;
}

void application::SubscribeMarketData(char* instIdList[],int len){
  int ret=pmdapi_->SubscribeMarketData(instIdList, len);
  cerr<<" 请求 | 发送行情订阅... "<<((ret == 0) ? "成功" : "失败")<< endl;
}

void application::TraderUserLogin(TThostFtdcBrokerIDType  appId,
                  TThostFtdcUserIDType  userId, TThostFtdcPasswordType  passwd){
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, appId);
    strcpy(req.UserID, userId);
    strcpy(req.Password, passwd);
    int ret = ptraderapi_->ReqUserLogin(&req, ++requestid_);
    cerr<<" 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl;
}

void application::ReqQrySettlementInfo(){
  CThostFtdcQrySettlementInfoField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, APPID);
  strcpy(req.InvestorID, USERID);
  int ret = ptraderapi_->ReqQrySettlementInfo(&req, ++requestid_);
  cerr<<" 请求 | 发送结算结果查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void application::ReqSettlementInfoConfirm(){
  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, APPID);
  strcpy(req.InvestorID, USERID);
  int ret = ptraderapi_->ReqSettlementInfoConfirm(&req, ++requestid_);
  cerr<<" 请求 | 发送结算单确认..."<<((ret == 0)?"成功":"失败")<<endl;
}

void application::ReqQryTradingAccount(){
  CThostFtdcQryTradingAccountField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, APPID);
  strcpy(req.InvestorID, USERID);
  int ret = ptraderapi_->ReqQryTradingAccount(&req, ++requestid_);
  cerr<<" 请求 | 发送资金查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void application::ReqQryInvestorPosition(TThostFtdcInstrumentIDType instId){
  CThostFtdcQryInvestorPositionField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, APPID);
  strcpy(req.InvestorID, USERID);
  strcpy(req.InstrumentID, instId);
  int ret = ptraderapi_->ReqQryInvestorPosition(&req, ++requestid_);
  cerr<<" 请求 | 发送持仓查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void application::ReqOrderInsert(TThostFtdcInstrumentIDType instId,
                TThostFtdcDirectionType dir,TThostFtdcPriceType price,TThostFtdcVolumeType vol,TThostFtdcOffsetFlagType type){
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, APPID);  //应用单元代码
    strcpy(req.InvestorID, USERID); //投资者代码
    strcpy(req.InstrumentID, instId); //合约代码
    strcpy(req.OrderRef, "");  //报单引用

    req.LimitPrice = price; //价格
    if(0==req.LimitPrice){
      req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//价格类型=市价
      req.TimeCondition = THOST_FTDC_TC_IOC;//有效期类型:立即完成，否则撤销
    }else{
      req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价
      req.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
    }
    req.Direction = MapDirection(dir,true);  //买卖方向
    //req.CombOffsetFlag[0] = MapOffset(kpp[0],true); //组合开平标志:开仓
    req.CombOffsetFlag[0] = type;
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;   //组合投机套保标志
    req.VolumeTotalOriginal = vol;  ///数量
    req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
    req.MinVolume = 1;  //最小成交量:1
    req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即

    //TThostFtdcPriceType StopPrice;  //止损价
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;  //强平原因:非强平
    req.IsAutoSuspend = 0;  //自动挂起标志:否
    req.UserForceClose = 0;   //用户强评标志:否

    int ret = ptraderapi_->ReqOrderInsert(&req, ++requestid_);
    cerr<<" 请求 | 发送报单..."<<((ret == 0)?"成功":"失败")<< endl;
}

void application::CloseALlTrades(){
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector = ptraderspi_->ReturnAllTrade();
  unordered_map<string,vector<CThostFtdcTradeField*>>::iterator iter;
  for(iter = tradeMapVector.begin();iter!=tradeMapVector.end();iter++){
   vector<CThostFtdcTradeField*> tradeList = iter->second;
       for(unsigned int i=0; i<tradeList.size(); i++){
            CThostFtdcTradeField* pTrade = tradeList[i];
            //调用自己的开仓平仓　函数，最后一项传入的参数是强平。
            TThostFtdcDirectionType dir;
            if(pTrade->Direction == THOST_FTDC_D_Buy){
              dir = THOST_FTDC_D_Sell;
            }
            else{
              dir =THOST_FTDC_D_Buy;
            }
            ReqOrderInsert(pTrade->InstrumentID,dir,pTrade->Price,pTrade->Volume,THOST_FTDC_OF_Close);
        }
  }
}

void application::ReqOrderAction(TThostFtdcSequenceNoType orderSeq){
    bool found=false;
    unsigned int i=0;
    unordered_map<string,vector<CThostFtdcOrderField*>>  orderMapVector = ptraderspi_->ReturnAllOrder();
    unordered_map<string,vector<CThostFtdcOrderField*>>::iterator iter;
    for(iter=orderMapVector.begin();iter!=orderMapVector.end();iter++){
      vector<CThostFtdcOrderField*> orderList = iter->second;
      for(i=0;i<orderList.size();i++){
          if(orderList[i]->BrokerOrderSeq == orderSeq){
          found = true;
          break;
         }
      }
      if(found){
            CThostFtdcOrderField* tmporder = orderList[i];
            cout<<tmporder->OrderStatus<<endl;
            if(tmporder->OrderStatus ==THOST_FTDC_OST_AllTraded || tmporder->OrderStatus ==THOST_FTDC_OST_Canceled){
              cout<<"请求　｜　报单已经成交或者撤单 "<<endl;
              return;
            }
            CThostFtdcInputOrderActionField req;
            memset(&req, 0, sizeof(req));
            strcpy(req.BrokerID, APPID);   //经纪公司代码
            strcpy(req.InvestorID, USERID); //投资者代码
            //strcpy(req.OrderRef, pOrderRef); //报单引用
            //req.FrontID = frontId;           //前置编号
            //req.SessionID = sessionId;       //会话编号
            strcpy(req.ExchangeID, orderList[i]->ExchangeID);
            strcpy(req.OrderSysID, orderList[i]->OrderSysID);
            req.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志

            int ret = ptraderapi_->ReqOrderAction(&req, ++requestid_);
            cerr<< " 请求 | 发送撤单..." <<((ret == 0)?"成功":"失败") << endl;
         }
    }
    if(!found){cerr<<" 请求 | 报单不存在."<<endl; return;}


}

bool application::CheckToLock(TThostFtdcInstrumentIDType InstrumentID, TThostFtdcPriceType lastPrice){
    string tmpId = InstrumentID;
    unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector = ptraderspi_->ReturnAllTrade();
    unordered_map<string,vector<CThostFtdcOrderField*>>  orderMapVector = ptraderspi_->ReturnAllOrder();
    vector<CThostFtdcTradeField*> tradeVector = tradeMapVector[tmpId];
    vector<CThostFtdcOrderField*> orderVector = orderMapVector[tmpId];
    int sell = 0;
    int buy = 0;
    //检查合约编码对应的成交合约的买卖数量。
    for(int i=0;i<tradeVector.size();i++){
      if(tradeVector[i]->Direction ==THOST_FTDC_D_Buy && tradeVector[i]->OffsetFlag ==THOST_FTDC_OF_Open){
        buy = buy+tradeVector[i]->Volume;
      }
      else if(tradeVector[i]->Direction ==THOST_FTDC_D_Sell && tradeVector[i]->OffsetFlag ==THOST_FTDC_OF_Open){
        sell = sell+tradeVector[i]->Volume;
      }
    }
    //检查已经下单的合约的买卖数量，这里的报单的只是算没有成交的，和没有撤单的。
    for(int i=0;i<orderVector.size();i++){
      CThostFtdcOrderField* tmp = orderVector[i];
      if(tmp->OrderStatus != THOST_FTDC_OST_AllTraded && tmp->OrderStatus != THOST_FTDC_OST_Canceled){
          if(tmp->Direction== THOST_FTDC_D_Buy){
            buy = buy+tmp->VolumeTotalOriginal;
          }
          else if(tmp->Direction== THOST_FTDC_D_Sell){
            sell = sell + tmp->VolumeTotalOriginal;
          }
       }
     }
    if (sell ==buy){
      //表示已经锁仓了，已经不需要锁仓了。
      return true;
    }
    else if (sell>buy){
      //表示卖的比买的多，所以还是需要锁仓的，需要在买差值。
      //锁仓采用的方法是按照市价，立即成交，否则撤单的方式
      cout<<"调用锁仓函数"<<endl;
      TThostFtdcVolumeType num = sell-buy;
      ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,lastPrice,num,THOST_FTDC_OF_Open);
      return false;
    }
    else{
      //表示买的比卖的多，所以还是需要锁仓的，需要卖差值。
      //锁仓采用的方法是按照市价，立即成交，否则撤单的方式
      cout<<"调用锁仓函数"<<endl;
      TThostFtdcVolumeType num =buy -sell;
      ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,lastPrice,num,THOST_FTDC_OF_Open);
      return false;
    }
}

void application::PrintOrders(){
  unordered_map<string,vector<CThostFtdcOrderField*>> orderMapVector = ptraderspi_->ReturnAllOrder();
  CThostFtdcOrderField* pOrder;
  unordered_map<string,vector<CThostFtdcOrderField*>>::iterator iter;
  for(iter = orderMapVector.begin();iter!=orderMapVector.end();iter++){
    vector<CThostFtdcOrderField*> orderList = iter->second;
    for(unsigned int i=0; i<orderList.size(); i++){
      pOrder = orderList[i];
      TThostFtdcErrorMsgType error ="0";
      strcpy(error,pOrder->StatusMsg);
     cerr<<" 报单 | 合约:"<<pOrder->InstrumentID
      <<" 方向:"<<MapDirection(pOrder->Direction,false)
      <<" 开平:"<<MapOffset(pOrder->CombOffsetFlag[0],false)
      <<" 价格:"<<pOrder->LimitPrice
      <<" 数量:"<<pOrder->VolumeTotalOriginal
      <<" 序号:"<<pOrder->BrokerOrderSeq
      <<" 报单编号:"<<pOrder->OrderSysID
      <<" 报单状态编号:"<<pOrder->OrderStatus
      <<" 状态:"<<ConvertGb18030ToUtf8(error)<<endl;
      //basicPrint(ConvertGb18030ToUtf8(error));
      //<<" 状态:"<<pOrder->OrderStatus<<endl;
      //<<" 状态:"<<pOrder->StatusMsg<<endl;
     // ConvertGb18030ToUtf8(pOrder->StatusMsg);
  }
  }
}

void application::PrintTrades(){
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector = ptraderspi_->ReturnAllTrade();
  CThostFtdcTradeField* pTrade;
  unordered_map<string,vector<CThostFtdcTradeField*>>::iterator iter;
  for(iter = tradeMapVector.begin();iter!=tradeMapVector.end();iter++){
   vector<CThostFtdcTradeField*> tradeList = iter->second;
       for(unsigned int i=0; i<tradeList.size(); i++){
            pTrade = tradeList[i];
            cerr<<" 成交 | 合约:"<< pTrade->InstrumentID
              <<" 方向:"<<MapDirection(pTrade->Direction,false)
              <<" 开平:"<<MapOffset(pTrade->OffsetFlag,false)
              <<" 价格:"<<pTrade->Price
              <<" 数量:"<<pTrade->Volume
              <<" 报单编号:"<<pTrade->OrderSysID
              <<" 成交编号:"<<pTrade->TradeID<<endl;
        }
  }
}

CtpMdSpi* application::GetMdSpi(){
  return pmdspi_;
}

application::~application()
{
    //dtor
}
