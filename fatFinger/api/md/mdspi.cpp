#include "mdspi.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;
extern int requestId;

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

void CtpMdSpi::ReqUserLogin(TThostFtdcBrokerIDType	appId,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);
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
  cout<<"this is one"<<endl;
  TThostFtdcInstrumentIDType tmp ="au1706";
  TThostFtdcInstrumentIDType tmp1 ="au1706";
  if(strcmp(pDepthMarketData->InstrumentID,tmp)==0){
    cout<<"this is test"<<endl;
      cerr<<" 行情 | 合约:"<<pDepthMarketData->InstrumentID
    <<" 现价:"<<pDepthMarketData->LastPrice
    <<" 最高价:" << pDepthMarketData->HighestPrice
    <<" 最低价:" << pDepthMarketData->LowestPrice
    <<" 卖一价:" << pDepthMarketData->AskPrice1
    <<" 卖一量:" << pDepthMarketData->AskVolume1
    <<" 买一价:" << pDepthMarketData->BidPrice1
    <<" 买一量:" << pDepthMarketData->BidVolume1
    <<" 持仓量:"<< pDepthMarketData->OpenInterest <<endl;
  }
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
   cout<<"this is two"<<endl;
}

bool CtpMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    cerr<<" 响应 | "<<pRspInfo->ErrorMsg<<endl;
  }
  return ret;
}
