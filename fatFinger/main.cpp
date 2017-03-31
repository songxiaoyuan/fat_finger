#include "main.h"
#include <iostream>

using namespace std;

void test_md(void)
{
  //初始化UserApi
  CThostFtdcMdApi* pMdApi=CThostFtdcMdApi::CreateFtdcMdApi();
  CtpMdSpi* pMdSpi=new CtpMdSpi(pMdApi); //创建回调处理类对象MdSpi
  pMdApi->RegisterSpi(pMdSpi);			 // 回调对象注入接口类
  pMdApi->RegisterFront(mdFront);		     // 注册行情前置地址
  pMdApi->Init();      //接口线程启动, 开始工作
  ShowMdCommand(pMdSpi,true);
  pMdApi->Join();      //等待接口线程退出
  //pUserApi->Release(); //接口对象释放
  cout<<"extis!!!"<<endl;
}

void test_trader(void)
{
  //初始化UserApi
  CThostFtdcTraderApi* pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
  CtpTraderSpi* pTraderSpi = new CtpTraderSpi(pTraderApi);
  pTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderSpi);			// 注册事件类
  pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);					// 注册公有流
  pTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // 注册私有流
  pTraderApi->RegisterFront(tradeFront);							// 注册交易前置地址
  pTraderApi->Init();

  //初始化UserApi
  /*
  CThostFtdcMdApi* pMdApi=CThostFtdcMdApi::CreateFtdcMdApi();
  CtpMdSpi* pMdSpi=new CtpMdSpi(pMdApi); //创建回调处理类对象MdSpi
  pMdApi->RegisterSpi(pMdSpi);       // 回调对象注入接口类
  pMdApi->RegisterFront(mdFront);        // 注册行情前置地址
  pMdApi->Init();      //接口线程启动, 开始工作
  */

  int cmd;
  cin>>cmd;
  ShowTraderCommand(pTraderSpi,true);
  //ShowMdCommand(pMdSpi,true,cmd);
  pTraderApi->Join();
  //pMdApi->Join();      //等待接口线程退出
  cout<<"the trader isrequestId exitst"<<endl;
  //pUserApi->Release();
}
int main(int argc, const char* argv[]){
  //g_hEvent=CreateEvent(NULL, true, false, NULL);

  //if(argc < 2)  cerr<<"格式: 命令 参数, 输入有误."<<endl;
  //else if(strcmp(argv[1],"--md")==0)    test_md();
  //else if(strcmp(argv[1],"--order")==0) test_order();
  //test_md();
  test_trader();
  return 0;
}
