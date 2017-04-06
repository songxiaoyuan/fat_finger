#include "mdspi.h"


using namespace std;

void CtpMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
  IsErrorRspInfo(pRspInfo);
}

void CtpMdSpi::OnFrontDisconnected(int nReason)
{
  cerr<<" 响应 | 连接中断..."
    << " reason=" << nReason << endl;
}

void CtpMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
  cerr<<" 响应 | 心跳超时警告..."
    << " TimerLapse = " << nTimeLapse << endl;
}

void CtpMdSpi::OnFrontConnected()
{
	cerr<<" 连接交易前置 md ...成功"<<endl;
}

void CtpMdSpi::setDPMarketDataField(CThostFtdcDepthMarketDataField *pDepthMarketData){
  pthread_mutex_lock(&MUTEX);
  pCurrentDepthMarketData = pDepthMarketData;
  pthread_mutex_unlock(&MUTEX);
}

CThostFtdcDepthMarketDataField* CtpMdSpi::getDPMarketDataField(){
  return pCurrentDepthMarketData;
}

void CtpMdSpi::ReqUserLogin(TThostFtdcBrokerIDType	APPID,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);
	strcpy(req.UserID, userId);
	strcpy(req.Password, passwd);
	int ret = pUserApi->ReqUserLogin(&req, ++requestId);
  cerr<<" 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl;
}

void CtpMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin)
	{
    cerr<<" 响应 | 登录成功...当前交易日:"
      <<pRspUserLogin->TradingDay<<endl;
	}
}

void CtpMdSpi::SubscribeMarketData(char *instIdList[],int len)
{
  int ret=pUserApi->SubscribeMarketData(instIdList, len);
  cerr<<" 请求 | 发送行情订阅... "<<((ret == 0) ? "成功" : "失败")<< endl;
}

void CtpMdSpi::OnRspSubMarketData(
         CThostFtdcSpecificInstrumentField *pSpecificInstrument,
         CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr<<" 响应 |  行情订阅...成功"<<endl;
}

void CtpMdSpi::OnRspUnSubMarketData(
             CThostFtdcSpecificInstrumentField *pSpecificInstrument,
             CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr<<" 响应 |  行情取消订阅...成功"<<endl;
}

void CtpMdSpi::OnRtnDepthMarketData(
             CThostFtdcDepthMarketDataField *pDepthMarketData)
{
  //pthread_mutex_lock(&MUTEX);
  //将接受到的数据保存在对象里面，方便其他线程以后的读取。
  //cout<<"the is called"<<endl;

  setDPMarketDataField(pDepthMarketData);
  string InstrumentID = pDepthMarketData->InstrumentID;

   cout<<InstrumentID<<endl;

  if(PTHREADCONDS.find(InstrumentID) != PTHREADCONDS.end()){

    //根据获取到的数据的合约编码，发送信号，去唤醒相应的线程去处理数据。
    pthread_cond_signal(&PTHREADCONDS[InstrumentID]);
  }
  else{
    //说明处理此合约编码的数据还没有建立，需要建立线程
    cout<<"the thread to caculate "<<InstrumentID<<" is not installed "<<endl;
  }

/*
   TThostFtdcInstrumentIDType tmp ="au1706";
  if(strcmp(pDepthMarketData->InstrumentID,tmp)==0){
     cout<<1<<endl;
   }
   else{
     cout<<2<<endl;
   }
   */
 //pthread_mutex_unlock(&MUTEX);
  /*

  cerr<<" 行情 | 合约:"<<pDepthMarketData->InstrumentID
    <<" 现价:"<<pDepthMarketData->LastPrice
    <<" 最高价:" << pDepthMarketData->HighestPrice
    <<" 最低价:" << pDepthMarketData->LowestPrice
    <<" 卖一价:" << pDepthMarketData->AskPrice1
    <<" 卖一量:" << pDepthMarketData->AskVolume1
    <<" 买一价:" << pDepthMarketData->BidPrice1
    <<" 买一量:" << pDepthMarketData->BidVolume1
    <<" 持仓量:"<< pDepthMarketData->OpenInterest <<endl;
*/
}

bool CtpMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    cerr<<" 响应 | "<<pRspInfo->ErrorMsg<<endl;
  }
  return ret;
}
