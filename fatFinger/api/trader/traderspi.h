#ifndef ORDER_TRADERSPI_H_
#define ORDER_TRADERSPI_H_
#pragma once

#include "api/trade/public/ThostFtdcTraderApi.h"
#include "basicfun/basicfun.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

class CtpTraderSpi : public CThostFtdcTraderSpi
{
public:
   CtpTraderSpi(){};

  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
  virtual void OnFrontConnected();

  ///��¼������Ӧ
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

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
  // ���ص�ǰ���еĶ���
  unordered_map<string,vector<CThostFtdcOrderField*>> ReturnAllOrder();
  // ���ص�ǰ���еĳɽ��Ķ���
  unordered_map<string,vector<CThostFtdcTradeField*>> ReturnAllTrade();
    /// �Ƿ��յ��ɹ�����Ӧ
  bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);


private:
  // �Ự����
  int  frontId; //ǰ�ñ��
  int  sessionId; //�Ự���
  char orderRef[13];

  unordered_map<string,vector<CThostFtdcOrderField*>> orderMapVector;
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector;
};

#endif
