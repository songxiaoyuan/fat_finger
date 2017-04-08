#ifndef createThread_H
#define createThread_H
#include "api/md/mdspi.h"
#include "api/trader/traderspi.h"
#include "application/application.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <unordered_map>

class CtpMdSpi;
class application;

// 遇到乌龙指的时候，每一个tick交易量
extern TThostFtdcVolumeType MAXCOUNT;

// 判断是不是乌龙指的阈值
extern double THRESHOLD;
// 判断是不是回归正常的阈值
extern double NORMALTHRESHOLD;

//互斥量，用来异步的访问当前传过来的数据。因为可能会存在多个线程同时处理一个指针的时候。
extern pthread_mutex_t MUTEX;

extern application* APPLICATION;


class createThread
{
    public:
        createThread(pthread_cond_t *cond,CThostFtdcDepthMarketDataField *pDepthMarketData);
        //线程启动函数
         int start();
         //静态函数，线程函数
         static void* threadFunc(void *arg);
         virtual ~createThread();
    protected:
    private:
       //线程启动的时候的一些条件变量，包括卖一价，买一价，最新价，以及对应的合约编码
        TThostFtdcPriceType BidPrice;
        TThostFtdcPriceType AskPrice;
        TThostFtdcPriceType LastPrice;
        TThostFtdcInstrumentIDType InstrumentID;
        bool FindFatFinger = false;
        pthread_cond_t *cond;
        CThostFtdcDepthMarketDataField *pInitDepthMarketData;
        pthread_t pid;
        //线程执行函数
        int threadRun();
};

#endif // createThread_H
