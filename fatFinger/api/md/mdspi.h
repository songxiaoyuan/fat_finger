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
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

public:
	void ReqUserLogin(TThostFtdcBrokerIDType	appId,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd);
	void SubscribeMarketData(char* instIdList[],int len);
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	//�������õ�ǰ���ܵ����ݣ�Ȼ�󹩴��������߳�ȥ������
	void setDPMarketDataField(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//������ȡ�õ����ض��ĺ�Լ��������ݣ�Ȼ�󹩴��������߳�ȥ����
	CThostFtdcDepthMarketDataField* getDPMarketDataField(TThostFtdcInstrumentIDType InstrumentID);

private:
  CThostFtdcMdApi* pUserApi;
  unordered_map<string,CThostFtdcDepthMarketDataField*> mapPCurrentDepthMarketData;
  //CThostFtdcDepthMarketDataField *pCurrentDepthMarketData;
};

#endif
