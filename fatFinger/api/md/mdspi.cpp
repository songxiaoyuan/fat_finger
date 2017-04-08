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
  string InstrumentID = pDepthMarketData->InstrumentID;
  mapPCurrentDepthMarketData[InstrumentID]=pDepthMarketData;
  //pCurrentDepthMarketData = pDepthMarketData;
  pthread_mutex_unlock(&MUTEX);
}

CThostFtdcDepthMarketDataField* CtpMdSpi::getDPMarketDataField(TThostFtdcInstrumentIDType InstrumentID){
  string tmpID = InstrumentID;
  if(mapPCurrentDepthMarketData.find(InstrumentID) != mapPCurrentDepthMarketData.end()){
    return mapPCurrentDepthMarketData[InstrumentID];
  }
  else{
    return NULL;
  }
  //return pCurrentDepthMarketData;
}

void CtpMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin)
	{
    cerr<<" 响应 | 登录成功...当前交易日:"
      <<pRspUserLogin->TradingDay<<endl;
	}
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
  //将接受到的数据保存在对象里面，方便其他线程以后的读取。
  setDPMarketDataField(pDepthMarketData);
  string InstrumentID = pDepthMarketData->InstrumentID;

  cout<<InstrumentID<<endl;

  if(PTHREADCONDS.find(InstrumentID) != PTHREADCONDS.end()){

    //根据获取到的数据的合约编码，发送信号，去唤醒相应的线程去处理数据。
    pthread_cond_signal(&PTHREADCONDS[InstrumentID]);
  }
  else{
    //说明处理此合约编码的数据还没有建立，需要建立线程
    PTHREADCONDS[InstrumentID] = PTHREAD_COND_INITIALIZER;
    createThread *ct = new createThread(&PTHREADCONDS[InstrumentID],pDepthMarketData);
    //cout<<"the address of ct is "<<ct<<endl;
    int start = ct->start();
    //cout<<"starting the thread"<<endl;
    if (!start){
      cout<<"the thread is not installed"<<endl;
      //创建线程没有成功，下次重新创建。
      unordered_map<string,pthread_cond_t>::iterator iter;
      iter = PTHREADCONDS.find(InstrumentID);
      if(iter !=  PTHREADCONDS.end()){
        PTHREADCONDS.erase(iter);
      }
    }
  }
}

bool CtpMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    cerr<<" 响应 | "<<pRspInfo->ErrorMsg<<endl;
  }
  return ret;
}
