#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#pragma once

#include "api/trade/public/ThostFtdcMdApi.h"
#include "api/trade/public/ThostFtdcTraderApi.h"
//请求编号
int requestId=0;
TThostFtdcVolumeType MAXCOUNT = 1;
int THRESHOLD=10;


// 前置地址 7*24
//char mdFront[]   ="tcp://180.168.146.187:10031";
//char tradeFront[]="tcp://180.168.146.187:10030";

char mdFront[]   ="tcp://180.168.146.187:10010";
char tradeFront[]="tcp://180.168.146.187:10000";

//所采用的账号和密码
TThostFtdcBrokerIDType   appId= "9999";
TThostFtdcUserIDType	 userId = "068674";
TThostFtdcPasswordType   passwd = "19930208";

//订阅的行情列表
char *ppInstrumentID[] = {"au1712","au1706"};
int NumOfInstrumentID = 2;

#endif // CONFIG_H_INCLUDED
