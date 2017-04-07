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

extern TThostFtdcBrokerIDType APPID;		// 应用单元
extern TThostFtdcUserIDType USERID;		// 投资者代码

class CtpTraderSpi : public CThostFtdcTraderSpi
{
public:
   CtpTraderSpi(CThostFtdcTraderApi* api):pUserApi(api){};

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

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
	///用户登录请求
	void ReqUserLogin(TThostFtdcBrokerIDType	appId,
	        TThostFtdcUserIDType	userId,	TThostFtdcPasswordType	passwd);
    ///请求查询投资者结算结果
	void ReqQrySettlementInfo();
	///投资者结算结果确认
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument(TThostFtdcInstrumentIDType instId);
	///请求查询资金账户
	void ReqQryTradingAccount();
	///请求查询投资者持仓
	void ReqQryInvestorPosition(TThostFtdcInstrumentIDType instId);
	///报单录入请求
   void ReqOrderInsert(TThostFtdcInstrumentIDType instId,
        TThostFtdcDirectionType dir,TThostFtdcPriceType price,   TThostFtdcVolumeType vol);
        ///报单直接以市场价成交
   void ReqOrderInsertTake(TThostFtdcInstrumentIDType instId,
    TThostFtdcDirectionType dir, TThostFtdcVolumeType vol);
	///报单操作请求
	void ReqOrderAction(TThostFtdcSequenceNoType orderSeq);

	/// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	///判断当前的仓位是不是还需要锁仓，如果需要的话，同时调用锁仓函数。
	bool CheckToLock(TThostFtdcInstrumentIDType InstrumentID, TThostFtdcPriceType lastPrice);

  void PrintOrders();
  void PrintTrades();

private:
  CThostFtdcTraderApi* pUserApi;
  // 会话参数
  int	 frontId;	//前置编号
  int	 sessionId;	//会话编号
  char orderRef[13];

  unordered_map<string,vector<CThostFtdcOrderField*>> orderMapVector;
  unordered_map<string,vector<CThostFtdcTradeField*>> tradeMapVector;

  vector<CThostFtdcOrderField*> orderList;
  vector<CThostFtdcTradeField*> tradeList;

};

#endif
