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

  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  virtual void OnFrontConnected();

  ///登录请求响应
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///投资者结算结果确认响应
  virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询合约响应
  virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询资金账户响应
  virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///请求查询投资者持仓响应
  virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单录入请求响应
  virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///报单操作请求响应
  virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///错误应答
  virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  virtual void OnFrontDisconnected(int nReason);

  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  virtual void OnHeartBeatWarning(int nTimeLapse);

  ///报单通知
  virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

  ///成交通知
  virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

public:
  // 返回当前所有的订单
  unordered_map<string,vector<CThostFtdcOrderField*>> ReturnAllOrder();
  // 返回当前所有的成交的订单
  unordered_map<string,vector<CThostFtdcTradeField*>> ReturnAllTrade();
    /// 是否收到成功的响应
  bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);


private:
  // 会话参数
  int  frontId; //前置编号
  int  sessionId; //会话编号
  char orderRef[13];

  unordered_map<string,vector<CThostFtdcOrderField*>> orderMapVector;
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector;
};

#endif
