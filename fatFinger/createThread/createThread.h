#pragma once
#ifndef CREATETHREAD_H
#define CREATETHREAD_H
#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <unordered_map>
using namespace std;

// 当前订阅的合约编码的列表。
extern char *PPINSTRUMENTID[];

//当前订阅的合约编码的数量。
extern int NUMOFINSTRUMENTID;

// 用来保存特定的合约编号对应的条件变量，当有订阅的合约的数据到来时，可以选择的去触发相应的条件变量，从而触发相应的
// 线程去处理数据。
extern unordered_map<string,pthread_cond_t> PTHREADCONDS;

//程序运行时的trader的spi和md的spi。
extern CtpTraderSpi* PTRADERSPI;
extern CtpMdSpi* PMDSPI;

// 遇到乌龙指的时候，每一个tick交易量
extern TThostFtdcVolumeType MAXCOUNT;

// 判断是不是乌龙指的阈值
extern double THRESHOLD;

//互斥量，用来异步的访问当前传过来的数据。因为可能会存在多个线程同时处理一个指针的时候。
extern pthread_mutex_t MUTEX;

//创建线程时，线程的运行函数。
//@param 输入变量cond：pthread_cond_t* 条件变量的地址。
//当md接受函数接受到数据的时候，会触发相应的条件变量，线程开始处理数据。
void *createThreadFun(void *cond);

// 创建线程集合，负责等待条件变量，如果接受到了数据，触发条件变量，处理数据。
void createThreadSet();

#endif // CREATETHREAD_H
