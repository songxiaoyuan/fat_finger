#ifndef MD_SPI_H_
#define MD_SPI_H_
//#pragma once
#include "api/trade/public/ThostFtdcMdApi.h"
#include "createThread/createThread.h"
#include <unordered_map>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_map>

using namespace std;

extern int requestId;
extern pthread_mutex_t MUTEX;
extern unordered_map<string,pthread_cond_t> PTHREADCONDS;

struct threadArgument{
  pthread_cond_t *cond;
  CThostFtdcDepthMarketDataField *pDepthMarketData;
};

//extern void *createThreadFun(void *cond);
class CtpMdSpi : public CThostFtdcMdSpi
{
public:
  CtpMdSpi(CThostFtdcMdApi* api):pUserApi(api){};
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

public:
	void ReqUserLogin(TThostFtdcBrokerIDType	appId,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd);
	void SubscribeMarketData(char* instIdList[],int len);
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	//用来设置当前接受的数据，然后供处理数据线程去做处理
	void setDPMarketDataField(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//用来获取得到的特定的合约编码的数据，然后供处理数据线程去处理
	CThostFtdcDepthMarketDataField* getDPMarketDataField(TThostFtdcInstrumentIDType InstrumentID);

private:
  CThostFtdcMdApi* pUserApi;
  unordered_map<string,CThostFtdcDepthMarketDataField*> mapPCurrentDepthMarketData;
  //CThostFtdcDepthMarketDataField *pCurrentDepthMarketData;
};

#endif
