#ifndef APPLICATION_H
#define APPLICATION_H

#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"

#include <iostream>

//用户登录需要的前置地址
extern char MDFRONT[];
extern char TRADEFRONT[];

//用户登录需要的信息。
extern TThostFtdcBrokerIDType APPID;
extern TThostFtdcUserIDType USERID;
extern TThostFtdcPasswordType PASSWD;

//订阅的行情列表
extern char* PPINSTRUMENTID[];
extern int NUMOFINSTRUMENTID;

class CtpMdSpi;

class application {
 public:
  application();
  // 应用初始化函数
  void Init();
  // 应用运行函数
  void Run();
  // 订阅用户进行登录
  void MdUserLogin(TThostFtdcBrokerIDType appId, TThostFtdcUserIDType userId,
                   TThostFtdcPasswordType passwd);
  // 订阅关心的合约编码
  void SubscribeMarketData(char* instIdList[], int len);
  ///用户登录请求
  void TraderUserLogin(TThostFtdcBrokerIDType appId,
                       TThostFtdcUserIDType userId,
                       TThostFtdcPasswordType passwd);
  ///请求查询投资者结算结果
  void ReqQrySettlementInfo();
  ///投资者结算结果确认
  void ReqSettlementInfoConfirm();
  ///请求查询资金账户
  void ReqQryTradingAccount();
  ///请求查询投资者持仓
  void ReqQryInvestorPosition(TThostFtdcInstrumentIDType instId);
  ///报单录入请求,最后一项开平标志类型,默认的开仓
  void ReqOrderInsert(TThostFtdcInstrumentIDType instId,
                      TThostFtdcDirectionType dir, TThostFtdcPriceType price,
                      TThostFtdcVolumeType vol, TThostFtdcOffsetFlagType type);
  ///报单操作请求,现在主要是撤单
  void AppReqOrderAction(TThostFtdcSequenceNoType orderSeq);
  ///判断当前的仓位是不是还需要锁仓，如果需要的话，同时调用锁仓函数。
  bool CheckToLock(TThostFtdcInstrumentIDType InstrumentID,
                   TThostFtdcPriceType lastPrice);
  // 打印当前所有的订单
  void PrintOrders();
  // 打印当前所有的成交的订单
  void PrintTrades();
  //平掉所有的仓位，方便做实验
  void CloseALlTrades();

  CtpMdSpi* GetMdSpi();
  virtual ~application();

 protected:
 private:
  int requestid_;
  // 保存程序运行时的trader的spi和md的spi。
  CtpTraderSpi* ptraderspi_;
  CtpMdSpi* pmdspi_;

  //保存程序运行时的trader api 和md api
  CThostFtdcTraderApi* ptraderapi_;
  CThostFtdcMdApi* pmdapi_;
};

#endif // APPLICATION_H
