#include "init.h"

using namespace std;

void initApiAndSpi(){
	//初始化traderApi
	CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CtpTraderSpi* pTraderSpi = new CtpTraderSpi(pTraderApi);
	pTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderSpi);			// 注册事件类
	pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);					// 注册公有流
	pTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // 注册私有流
	pTraderApi->RegisterFront(TRADEFRONT);	// 注册交易前置地址
	pTraderApi->Init();
	PTRADERSPI = pTraderSpi;
	PTRADERAPI = pTraderApi;

	//初始化MDApi
	CThostFtdcMdApi* pMdApi=CThostFtdcMdApi::CreateFtdcMdApi();
	CtpMdSpi* pMdSpi=new CtpMdSpi(pMdApi); //创建回调处理类对象MdSpi
	pMdApi->RegisterSpi(pMdSpi);       // 回调对象注入接口类
	pMdApi->RegisterFront(MDFRONT);        // 注册行情前置地址
	pMdApi->Init();      //接口线程启动, 开始工作
	PMDSPI = pMdSpi;
	PMDAPI = pMdApi;
}

void userLogin(){
	//MD用户开始登录和订阅信息。
	cout<<"登录MD用户"<<endl;
	PMDSPI->ReqUserLogin(APPID,USERID,PASSWD);

	//Trader用户开始登录，并且进行结算单确认。
	cout<<"trader用户开始登录"<<endl;
	PTRADERSPI->ReqUserLogin(APPID,USERID,PASSWD);
	cout<<"开始结算单请求并且确认"<<endl;
	PTRADERSPI->ReqQrySettlementInfo();
	PTRADERSPI->ReqSettlementInfoConfirm();
}

void userSubscribe(){
	// 开始订阅信息，程序接受数据，并且处理数据。
	cout<<"MD用户开始订阅合约"<<endl;
	PMDSPI->SubscribeMarketData(PPINSTRUMENTID,NUMOFINSTRUMENTID);
}

void printOrders(){
	//用户打印自己的所有的订单
	PTRADERSPI->PrintOrders();
}

void printTrades(){
	// 用户打印自己的所有的成交订单
	PTRADERSPI->PrintTrades();
}
