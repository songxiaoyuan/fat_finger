#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#pragma once

#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"
#include <unordered_map>

//此文件的作用是保存程序运行的一些全局变量，包括登录用户名，密码等信息。

// 保存程序运行时的trader的spi和md的spi。
CtpTraderSpi* PTRADERSPI;
CtpMdSpi* PMDSPI;

//保存程序运行时的trader api 和ｍｄ　api
CThostFtdcTraderApi* PTRADERAPI;
CThostFtdcMdApi* PMDAPI;

// 用来保存特定的合约编号对应的条件变量，当有订阅的合约的数据到来时，可以选择的去触发相应的条件变量，从而触发相应的
// 线程去处理数据。
unordered_map<string,pthread_cond_t> PTHREADCONDS;

// 判断对应的合约编码是不是已经下单了
// 其中对应的值如果是'0'，表示还没有下单或者已经锁仓，此时不用处理。
// 如果对应的是'1',表示已经遇到买一价涨停情况，已经开空了。
// 如果对应的是'2',表示已经遇到卖一价跌停的情况，已经开多了。
unordered_map<string,char> TRADERSTATUS;

//请求编号
int requestId=0;

// 遇到乌龙指的时候，每一个tick交易量
TThostFtdcVolumeType MAXCOUNT = 1;

// 判断是不是乌龙指的阈值
double THRESHOLD=10;
// 判断是不是回归正常的阈值
double NORMALTHRESHOLD=0;


// 前置地址 7*24
char MDFRONT[]   ="tcp://180.168.146.187:10031";
char TRADEFRONT[]="tcp://180.168.146.187:10030";

//char MDFRONT[]   ="tcp://180.168.146.187:10010";
//char TRADEFRONT[]="tcp://180.168.146.187:10000";

//所采用的账号和密码
TThostFtdcBrokerIDType   APPID= "9999";
TThostFtdcUserIDType	 USERID = "068674";
TThostFtdcPasswordType   PASSWD = "19930208";

//订阅的行情列表
char *PPINSTRUMENTID[] = {"au1706","au1712"};
int NUMOFINSTRUMENTID = 2;

//互斥量，用来异步的访问当前传过来的数据。因为可能会存在多个线程同时处理一个指针的时候。
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

#endif // CONFIG_H_INCLUDED
