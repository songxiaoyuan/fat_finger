#ifndef ORDER_TRADERSPI_H_
#define ORDER_TRADERSPI_H_
#pragma once

#include "api/trade/public/ThostFtdcTraderApi.h"
#include "basicFun/basicFun.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

extern TThostFtdcBrokerIDType APPID;		// Ӧ�õ�Ԫ
extern TThostFtdcUserIDType USERID;		// Ͷ���ߴ���

class CtpTraderSpi : public CThostFtdcTraderSpi
{
public:
   CtpTraderSpi(CThostFtdcTraderApi* api):pUserApi(api){};

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

public:
	///�û���¼����
	void ReqUserLogin(TThostFtdcBrokerIDType	appId,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd);
    ///�����ѯͶ���߽�����
	void ReqQrySettlementInfo();
	///Ͷ���߽�����ȷ��
	void ReqSettlementInfoConfirm();
	///�����ѯ��Լ
	void ReqQryInstrument(TThostFtdcInstrumentIDType instId);
	///�����ѯ�ʽ��˻�
	void ReqQryTradingAccount();
	///�����ѯͶ���ֲ߳�
	void ReqQryInvestorPosition(TThostFtdcInstrumentIDType instId);
	///����¼������
   void ReqOrderInsert(TThostFtdcInstrumentIDType instId,
        TThostFtdcDirectionType dir,TThostFtdcPriceType price,   TThostFtdcVolumeType vol);
        ///����ֱ�����г��۳ɽ�
   void ReqOrderInsertTake(TThostFtdcInstrumentIDType instId,
    TThostFtdcDirectionType dir, TThostFtdcVolumeType vol);
	///������������
	void ReqOrderAction(TThostFtdcSequenceNoType orderSeq);

	/// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	///�жϵ�ǰ�Ĳ�λ�ǲ��ǻ���Ҫ���֣������Ҫ�Ļ���ͬʱ�������ֺ�����
	bool CheckToLock(TThostFtdcInstrumentIDType InstrumentID, TThostFtdcPriceType lastPrice);

  void PrintOrders();
  void PrintTrades();

private:
  CThostFtdcTraderApi* pUserApi;
  // �Ự����
  int	 frontId;	//ǰ�ñ��
  int	 sessionId;	//�Ự���
  char orderRef[13];

  unordered_map<string,vector<CThostFtdcOrderField*>> orderMapVector;
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector;

  vector<CThostFtdcOrderField*> orderList;
  vector<CThostFtdcTradeField*> tradeList;

};

#endif
