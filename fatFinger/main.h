#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "config.h"
#include "basicFun/basicFun.h"
#include "createThread/createThread.h"
#include "api/trade/public/ThostFtdcMdApi.h"
#include "api/trade/public/ThostFtdcTraderApi.h"
#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

extern TThostFtdcBrokerIDType   APPID;
extern TThostFtdcUserIDType	 userId;
extern TThostFtdcPasswordType   passwd;
extern char *ppInstrumentID[];
extern int NumOfInstrumentID;
extern int TEST;
//extern pthread_mutex_t MUTEX;

void ShowTraderCommand(CtpTraderSpi* p, bool print=false){
  cout<<TEST<<endl;
  if(print){
    cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] ReqUserLogin              -- 登录"<<endl;
    cerr<<" [2] ReqSettlementInfoConfirm  -- 结算单确认"<<endl;
    cerr<<" [3] ReqQryInstrument          -- 查询合约"<<endl;
    cerr<<" [4] ReqQryTradingAccount      -- 查询资金"<<endl;
    cerr<<" [5] ReqQryInvestorPosition    -- 查询持仓"<<endl;
    cerr<<" [6] ReqOrderInsert            -- 报单"<<endl;
    cerr<<" [7] ReqOrderAction            -- 撤单"<<endl;
    cerr<<" [8] PrintOrders               -- 显示报单"<<endl;
    cerr<<" [9] PrintOrders               -- 显示成交"<<endl;
    cerr<<" [0] Exit                      -- 退出"<<endl;
    cerr<<" [0] Exit                      -- 退出"<<endl;
    cerr<<"----------------------------------------------"<<endl;
  }

  //TThostFtdcInstrumentIDType    instId;
  //TThostFtdcDirectionType       dir;
  //TThostFtdcCombOffsetFlagType  kpp;
  //TThostFtdcPriceType           price;
  TThostFtdcVolumeType          vol;
  TThostFtdcSequenceNoType      orderSeq;

  int cmd;
  cin>>cmd;
  switch(cmd){
    case 1: {
              cerr<<" 应用单元 > ";
              cerr<<" 投资者代码 > ";
              cerr<<" 交易密码 > ";
              p->ReqUserLogin(APPID,userId,passwd);
              cout<<"dingyue"<<endl;
              p->ReqQrySettlementInfo();
              p->ReqSettlementInfoConfirm();
              break;
            }
    case 2: {
                p->ReqQrySettlementInfo();
                p->ReqSettlementInfoConfirm(); break;
            }
    case 3: {
              cerr<<" 合约1 > ";
              TThostFtdcInstrumentIDType instId = "au1706";
              p->ReqQryInstrument(instId); break;
            }
    case 4: p->ReqQryTradingAccount(); break;
    case 5: {
              cerr<<" 合约2 > ";
              //cin>>instId;
              TThostFtdcInstrumentIDType instId = "au1706";
              p->ReqQryInvestorPosition(instId); break;
            }
    case 6: {
              cerr<<" 合约3 > ";
              //cin>>instId;
              TThostFtdcInstrumentIDType instId = "au1706";
              cerr<<" 方向 > ";
              //cin>>dir;
              TThostFtdcDirectionType dir = '0';
              cerr<<" 开平 > ";
              //cin>>kpp;
              TThostFtdcCombOffsetFlagType kpp = "0";
              cerr<<" 价格 > ";
              //cin>>price;
              TThostFtdcPriceType price = 5600;
              cerr<<" 数量 > ";
              //cin>>vol;
              TThostFtdcVolumeType vol =1;
              p->ReqOrderInsert(instId,dir,price,vol); break;
            }
    case 7: {
              cerr<<" 序号 > "; cin>>orderSeq;
              p->ReqOrderAction(orderSeq);break;
            }
    case 8: p->PrintOrders();break;
    case 9: p->PrintTrades();break;
    case 0: exit(0);
  }
  cout<<"trader out"<<endl;
  //ShowTraderCommand(p);
}


void ShowMdCommand(CtpMdSpi* p, bool print=false,int cmd=1){
  if(print){
    cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] ReqUserLogin              -- 登录并订阅行情"<<endl;
    cerr<<" [0] Exit                      -- 退出"<<endl;
    cerr<<"----------------------------------------------"<<endl;
  }
  /*
  cout<<"登录"<<endl;
  p->ReqUserLogin(APPID,userId,passwd);
  cout<<"合约"<<endl;
   p->SubscribeMarketData(ppInstrumentID,NumOfInstrumentID);
   */

  //int cmd;
  cin>>cmd;
  switch(cmd){
    case 1: {
              cout<<"登录"<<endl;
              p->ReqUserLogin(APPID,userId,passwd);
              cout<<"合约2"<<endl;
              p->SubscribeMarketData(ppInstrumentID,NumOfInstrumentID);
              string str;
              cin>>str;
              cout<<str<<endl;
              break;
            }
    case 0: exit(0);
  }
  cout<<"out"<<endl;
  //ShowMdCommand(p);
}

#endif // MAIN_H_INCLUDED
