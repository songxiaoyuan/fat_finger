#pragma once
#ifndef CREATETHREAD_H
#define CREATETHREAD_H
#include "api/trade/public/ThostFtdcMdApi.h"
#include "api/trade/public/ThostFtdcTraderApi.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

//这个函数是用来创建处理接受到交易数据的线程。
//传入的参数是一个条件变量，用来等待相应的合约代码的数据到来，从而处发线程
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
int TEST=13;
void *createThread(void *);

#endif // CREATETHREAD_H
