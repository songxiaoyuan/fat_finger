#ifndef INIT_H
#define INIT_H
#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"

#include <iostream>
//用户登录需要的前置地址
extern char MDFRONT[];
extern char TRADEFRONT[];

//初始化环境的traderspi和mdspi
extern CtpTraderSpi* PTRADERSPI;
extern CtpMdSpi* PMDSPI;

//初始化环境的trader api 和ｍｄ spi
extern CThostFtdcTraderApi* PTRADERAPI;
extern CThostFtdcMdApi* PMDAPI;

//用户登录需要的信息。
extern TThostFtdcBrokerIDType   APPID;
extern TThostFtdcUserIDType	 USERID;
extern TThostFtdcPasswordType   PASSWD;

//订阅的行情列表
extern char *PPINSTRUMENTID[];
extern int NUMOFINSTRUMENTID;


//此文件的作用主要是用来创建程序运行的一些初始化信息。包括创建APi，Mpi实例，用户登录函数，订阅函数等。

//此函数用来创建程序需要的api和spi对象指针，并且通过config.h文件保存
void initApiAndSpi();

//用户登录函数，通过config.h文件中的登录用户名，密码等信息，登录md和trader的服务器
void userLogin();

//用户通过MD开始订阅信息，此时程序变回自动进行，通过md传回来的数据，判断是不是乌龙指，如果是的话，自动下单。
void userSubscribe();

//用户打印现在所有讯在的报单。
void printOrders();


//用户打印现在所有的成交的单子。
void printTrades();



#endif // INIT_H
