#include "createthread.h"

createThread::createThread(pthread_cond_t *cond,CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    //初始化对象的两个参数，包括条件变量和第一次接受到的数据。
    this->cond = cond;
    this->pInitDepthMarketData = pDepthMarketData;

    //根据第一次接受到的数据，初始化卖一价，买一价和最新价等信息。
    BidPrice = pInitDepthMarketData->BidPrice1;
    AskPrice = pInitDepthMarketData->AskPrice1;
    LastPrice = pInitDepthMarketData->LastPrice;
    strcpy(InstrumentID, pInitDepthMarketData->InstrumentID);
}

//线程构造函数　
void* createThread::threadFunc(void *arg){
  //传入的是对象自身的一个指针
  createThread *obj = (createThread*)arg;
  cout<<"the thread of "<<obj->InstrumentID<<" is started"<<endl;
  obj->threadRun();
}

int createThread::threadRun(){
  pthread_t tmp = pthread_self();
  //cout<<"the thread "<<InstrumentID<<" is called  " <<tmp<<endl;
  //cout<<"the adderss of cond is "<<&cond<<endl;
     while(1){
        //pthread_cond_wait(&cond,&mutex);/*等待*/
        pthread_mutex_lock(&MUTEX);
        pthread_cond_wait((pthread_cond_t*)cond,&MUTEX);/*等待*/
        //线程被唤醒，读取数据，因为已经lock了，所以读的时候可以保证准确性，但是也应该快速的读取。尽快释放lock。
        CtpMdSpi* pmdspi = APPLICATION->GetMdSpi();
        CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = pmdspi->getDPMarketDataField(InstrumentID);
        if(pCurrentDepthMarketData ==NULL){
          continue;
        }
        TThostFtdcPriceType CurrentBidPrice = pCurrentDepthMarketData->BidPrice1;
        TThostFtdcPriceType CurrentAskPrice = pCurrentDepthMarketData->AskPrice1;
        TThostFtdcPriceType CurrentLastPrice = pCurrentDepthMarketData->LastPrice;
        pthread_mutex_unlock(&MUTEX);
              //通过接受到的数据，判断是不是乌龙指。
  			  if (CurrentBidPrice >=(BidPrice*(1+THRESHOLD/100))){
  			  	//说明买一价已经涨停，所以赶紧开空,就是赶紧卖
  			  	/* code */
  			  	FindFatFinger = true;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	cout<<"the current bid price is "<<CurrentBidPrice<<endl;
  			  	cout<<"the pre bid price is "<<BidPrice<<endl;
  			  	cout<<"----------------"<<InstrumentID<<endl;
                APPLICATION->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT,THOST_FTDC_OF_Open);
  			  }
  			  else if (CurrentAskPrice <= (AskPrice*(1 - THRESHOLD/100))){
  			    //说明卖一价已经跌停了，所以赶紧开多，就是赶紧买。
  			    FindFatFinger = true;
                cout<<"----------------"<<InstrumentID<<endl;
  			  	cout<<"the current ask price is "<<CurrentAskPrice<<endl;
  			  	cout<<"the pre ask price is "<<AskPrice<<endl;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	//说明卖一价跌停，所以赶紧开多,，就是赶紧买
  			  	APPLICATION->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT,THOST_FTDC_OF_Open);
  			  }
  			  else{
  			      //说明以前还不是乌龙指
  			      if (!FindFatFinger){
                    BidPrice = CurrentBidPrice;
  			  	    AskPrice = CurrentAskPrice;
  			  	    LastPrice = CurrentLastPrice;
  			      }
                //说明发生了乌龙指，但是现在已经不是乌龙指了，根据自己的判断，已经在阈值之下。
  			  	//首先判断是不是已经回归正常了，如果回归正常，那么开始进行锁仓操作。
                else if(CurrentLastPrice <=(LastPrice*(1+NORMALTHRESHOLD/100))&& CurrentLastPrice>=(LastPrice*(1-NORMALTHRESHOLD/100))){
                    BidPrice = CurrentBidPrice;
  			  	    AskPrice = CurrentAskPrice;
  			  	    LastPrice = CurrentLastPrice;
  			  	    //TODO::此处还是要注意一个锁仓的概念，现在还没有锁仓。
  			  	    //只有锁仓之后，这个发现乌龙指的bool才会false，说明此次乌龙指的处理已经结束。
  			  	    bool isLocked = APPLICATION->CheckToLock(InstrumentID,LastPrice);
  			  	    if(isLocked){
  			  	      //已经锁仓完毕，此次乌龙指处理结束。
  			  	      FindFatFinger = true;
  			  	    }
  			  	    else{
  			  	      cout<<"we need to lock"<<endl;
  			  	    }
  			  	}
  			  	//说明处于乌龙指和现在的正常之间
                else{
                  //暂时不做任何处理
                }
  			  }

       //cout<<"the thread "<<InstrumentID<<" is called  once "<<tmp<<endl;
    }
}

 //类中创建线程的函数，如果启动成功返回０　否则返回-1
int createThread::start(){
  cout<<"start to create thread of "<<InstrumentID<<endl;
   int ret = pthread_create(&pid,NULL,threadFunc,(void *)this);
   if (ret==0){
   	 return 1;
   }
   else{
   	return 0;
   }
}

createThread::~createThread()
{
    //dtor
}
