#include "api/trader/traderspi.h"

void CtpTraderSpi::OnFrontConnected() {
  cerr << " 连接交易前置.trader..成功" << endl;
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                  CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
  if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin) {
    // 保存会话参数
    frontId = pRspUserLogin->FrontID;
    sessionId = pRspUserLogin->SessionID;
    int nextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
    sprintf(orderRef, "%d", ++nextOrderRef);
    cerr << " 响应 | 登录成功...当前交易日:" << pRspUserLogin->TradingDay
         << endl;
  }
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(
    CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  if (!IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm) {
    cerr << " 响应 | 结算单..." << pSettlementInfoConfirm->InvestorID << "...<"
         << pSettlementInfoConfirm->ConfirmDate << " "
         << pSettlementInfoConfirm->ConfirmTime << ">...确认" << endl;
  } else {
    cout << "the selletment info confirm is wrong" << endl;
  }
}

void CtpTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
                                      CThostFtdcRspInfoField *pRspInfo,
                                      int nRequestID, bool bIsLast) {
  cout << "this OnRspQryInstrument is called" << endl;
  if (!IsErrorRspInfo(pRspInfo) && pInstrument) {
    cerr << " 响应 | 合约:" << pInstrument->InstrumentID
         << " 交割月:" << pInstrument->DeliveryMonth
         << " 多头保证金率:" << pInstrument->LongMarginRatio
         << " 空头保证金率:" << pInstrument->ShortMarginRatio << endl;
  }
}

void CtpTraderSpi::OnRspQryTradingAccount(
    CThostFtdcTradingAccountField *pTradingAccount,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  cout << "this OnRspQryTradingAccount is called" << endl;
  if (!IsErrorRspInfo(pRspInfo) && pTradingAccount) {
    cerr << " 响应 | 权益:" << pTradingAccount->Balance
         << " 可用:" << pTradingAccount->Available
         << " 保证金:" << pTradingAccount->CurrMargin
         << " 平仓盈亏:" << pTradingAccount->CloseProfit << " 持仓盈亏"
         << pTradingAccount->PositionProfit
         << " 手续费:" << pTradingAccount->Commission
         << " 冻结保证金:" << pTradingAccount->FrozenMargin
         //<<" 冻结手续费:"<<pTradingAccount->FrozenCommission
         << endl;
  }
}

void CtpTraderSpi::OnRspQryInvestorPosition(
    CThostFtdcInvestorPositionField *pInvestorPosition,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  cout << "this OnRspQryInvestorPosition is called" << endl;
  if (!IsErrorRspInfo(pRspInfo) && pInvestorPosition) {
    cerr << " 响应 | 合约:" << pInvestorPosition->InstrumentID << " 方向:"
         << MapDirection(pInvestorPosition->PosiDirection - 2, false)
         << " 总持仓:" << pInvestorPosition->Position
         << " 昨仓:" << pInvestorPosition->YdPosition
         << " 今仓:" << pInvestorPosition->TodayPosition
         << " 持仓盈亏:" << pInvestorPosition->PositionProfit
         << " 保证金:" << pInvestorPosition->UseMargin << endl;
  }
}

void CtpTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
                                    CThostFtdcRspInfoField *pRspInfo,
                                    int nRequestID, bool bIsLast) {
  cout << "this OnRspOrderInsert is called" << endl;
  if (!IsErrorRspInfo(pRspInfo) && pInputOrder) {
    cerr << "响应 | 报单提交成功...报单引用:" << pInputOrder->OrderRef << endl;
  }
}

void CtpTraderSpi::OnRspOrderAction(
    CThostFtdcInputOrderActionField *pInputOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  cout << "this OnRspOrderAction is called" << endl;
  if (!IsErrorRspInfo(pRspInfo) && pInputOrderAction) {
    cerr << " 响应 | 撤单成功..."
         << "交易所:" << pInputOrderAction->ExchangeID
         << " 报单编号:" << pInputOrderAction->OrderSysID << endl;
  }
}

unordered_map<string, vector<CThostFtdcOrderField *>>
CtpTraderSpi::ReturnAllOrder() {
  return orderMapVector;
}

unordered_map<string, vector<CThostFtdcTradeField *>>
CtpTraderSpi::ReturnAllTrade() {
  return tradeMapVector;
}

///报单回报
void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  CThostFtdcOrderField *order = new CThostFtdcOrderField();
  memcpy(order, pOrder, sizeof(CThostFtdcOrderField));
  bool founded = false;
  string instrumentID = order->InstrumentID;
  pthread_t tmp = pthread_self();
 TThostFtdcErrorMsgType error ="0";
      strcpy(error,pOrder->StatusMsg);
    cerr << " 回报 | 报单已提交...序号:" << order->BrokerOrderSeq
       << " the thread is " << tmp << endl;
   cerr<<" 回报 | 报单的状态是...序号:"<<ConvertGb18030ToUtf8(error)<<endl;
  if (orderMapVector.find(instrumentID) != orderMapVector.end()) {
    //说明此前有相应的合约编码的报单
    for (unsigned int i = 0; i < orderMapVector[instrumentID].size(); i++) {
      if (orderMapVector[instrumentID][i]->BrokerOrderSeq ==
          order->BrokerOrderSeq) {
        CThostFtdcOrderField *tmp = orderMapVector[instrumentID][i];
        orderMapVector[instrumentID][i] = order;
        delete tmp;
        founded = true;
        break;
      }
    }
    if (!founded) {
      orderMapVector[instrumentID].push_back(order);
    }
  } else {
    //第一次发现此合约编码类型的合约
    orderMapVector[instrumentID].push_back(order);
  }

  // basicPrint(order->StatusMsg);
}

///成交通知
void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  CThostFtdcTradeField *trade = new CThostFtdcTradeField();
  memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));
  bool founded = false;
  string instrumentID = trade->InstrumentID;
  if (tradeMapVector.find(instrumentID) != tradeMapVector.end()) {
    //说明此前有相应的合约编码的合约已经成交
    for (unsigned int i = 0; i < tradeMapVector[instrumentID].size(); i++) {
      if (tradeMapVector[instrumentID][i]->BrokerOrderSeq ==
          trade->BrokerOrderSeq) {
        CThostFtdcTradeField *tmp = tradeMapVector[instrumentID][i];
        tradeMapVector[instrumentID][i] = trade;
        delete tmp;
        founded = true;
        break;
      }
    }
    if (!founded) {
      tradeMapVector[instrumentID].push_back(trade);
    }
  } else {
    //第一次发现此成交合约编码类型的合约
    tradeMapVector[instrumentID].push_back(trade);
  }
  cerr << " 回报 | 报单已成交...成交编号:" << trade->TradeID << endl;
}

void CtpTraderSpi::OnFrontDisconnected(int nReason) {
  cerr << " 响应 | 连接中断..."
       << " reason=" << nReason << endl;
}

void CtpTraderSpi::OnHeartBeatWarning(int nTimeLapse) {
  cerr << " 响应 | 心跳超时警告..."
       << " TimerLapse = " << nTimeLapse << endl;
}

void CtpTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                              bool bIsLast) {
  IsErrorRspInfo(pRspInfo);
}

bool CtpTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  // 如果ErrorID != 0, 说明收到了错误的响应
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    //basicPrint(pRspInfo->ErrorMsg);
    TThostFtdcErrorMsgType error ="0";
    strcpy(error,pRspInfo->ErrorMsg);
    cerr<<" 响应 错误信息 | "<<ConvertGb18030ToUtf8(error)<<endl;
  }
  return ret;
}
