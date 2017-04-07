#include "createThread.h"

// 创建线程时，线程的运行函数。
//传入的参数是一个结构体，结构体包括一个条件变量，用来等待相应的合约代码的数据到来，从而处发线程
//结构体中还摆阔接受到的数据的地址，用来初始化线程信息。
/*
void *createThreadFun(void *arg){

   struct threadArgument *threadArg = (struct threadArgument*)arg;
   pthread_cond_t cond = threadArg->cond;
   CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = threadArg->pDepthMarketData;
   TThostFtdcPriceType BidPrice = 0;
   TThostFtdcPriceType AskPrice = 0;
   TThostFtdcPriceType LastPrice = 0;
   TThostFtdcInstrumentIDType InstrumentID="0";
   bool FindFatFinger = true;
   cout<<"the thread is called"<<endl;
   while(1){
        //pthread_cond_wait(&cond,&mutex);
        pthread_mutex_lock(&MUTEX);
        pthread_cond_wait((pthread_cond_t*)&cond,&MUTEX);
        //线程被唤醒，读取数据，因为已经lock了，所以读的时候可以保证准确性，但是也应该快速的读取。尽快释放lock。
        CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = PMDSPI->getDPMarketDataField(InstrumentID);
        TThostFtdcInstrumentIDType CurrentInstrumentID="0";
        strcpy(CurrentInstrumentID, pCurrentDepthMarketData->InstrumentID);
        //TThostFtdcInstrumentIDType CurrentInstrumentID=pCurrentDepthMarketData->InstrumentID;
        TThostFtdcPriceType CurrentBidPrice = pCurrentDepthMarketData->BidPrice1;
        TThostFtdcPriceType CurrentAskPrice = pCurrentDepthMarketData->AskPrice1;
        TThostFtdcPriceType CurrentLastPrice = pCurrentDepthMarketData->LastPrice;
        //cout<<"----------------"<<pCurrentDepthMarketData->InstrumentID<<endl;
        //cout<<"the current bid price is "<< pCurrentDepthMarketData->BidPrice1<<endl;
        //cout<<"the pre bid price is "<<pCurrentDepthMarketData->AskPrice1<<endl;
       // cout<<"++++++++++++++++++++++++++++++++"<<pCurrentDepthMarketData->InstrumentID<<endl;
        pthread_mutex_unlock(&MUTEX);
        if (strcmp(InstrumentID,CurrentInstrumentID)){
            //说明这是第一次调用此函数，初始化函数的基本信息。
              strcpy(InstrumentID, CurrentInstrumentID);
              BidPrice = CurrentBidPrice;
              AskPrice = CurrentAskPrice;
              LastPrice= CurrentLastPrice;
        }
        else{
              //通过接受到的数据，判断是不是乌龙指。
  			  if (CurrentBidPrice >=(BidPrice*(1+THRESHOLD/100))){
  			  	//说明买一价已经涨停，所以赶紧开空,就是赶紧卖

  			  	FindFatFinger = true;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	cout<<"the current bid price is "<<CurrentBidPrice<<endl;
  			  	cout<<"the pre bid price is "<<BidPrice<<endl;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else if (CurrentAskPrice <= (AskPrice*(1 - THRESHOLD/100))){
  			    //说明卖一价已经跌停了，所以赶紧开多，就是赶紧买。
  			    FindFatFinger = true;
                cout<<"----------------"<<InstrumentID<<endl;
  			  	cout<<"the current ask price is "<<CurrentAskPrice<<endl;
  			  	cout<<"the pre ask price is "<<AskPrice<<endl;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	//说明卖一价跌停，所以赶紧开多,，就是赶紧买
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,CurrentBidPrice,MAXCOUNT);
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
  			  	    bool isLocked = PTRADERSPI->CheckToLock(InstrumentID);
  			  	    if(isLocked){
  			  	      //已经锁仓完毕，此次乌龙指处理结束。
  			  	      FindFatFinger = true;
  			  	    }
  			  	}
  			  	//说明处于乌龙指和现在的正常之间
                else{
                  //暂时不做任何处理
                }
  			  }
        }
        pthread_t tmp = pthread_self();
        cout<<"the thread "<<InstrumentID<<" is called  once "<<tmp<<endl;
    }
}
*/

void *createThreadFun(void *arg){

   struct threadArgument *threadArg = (struct threadArgument*)arg;
   pthread_cond_t *cond = threadArg->cond;
   CThostFtdcDepthMarketDataField *pInitDepthMarketData = threadArg->pDepthMarketData;
   TThostFtdcPriceType BidPrice = pInitDepthMarketData->BidPrice1;
   TThostFtdcPriceType AskPrice = pInitDepthMarketData->AskPrice1;
   TThostFtdcPriceType LastPrice = pInitDepthMarketData->LastPrice;
   TThostFtdcInstrumentIDType InstrumentID="0";
   strcpy(InstrumentID, pInitDepthMarketData->InstrumentID);
   bool FindFatFinger = true;
   //cout<<"the thread is called"<<endl;
   while(1){
        //pthread_cond_wait(&cond,&mutex);/*等待*/
        pthread_mutex_lock(&MUTEX);
        pthread_cond_wait((pthread_cond_t*)cond,&MUTEX);/*等待*/
        //线程被唤醒，读取数据，因为已经lock了，所以读的时候可以保证准确性，但是也应该快速的读取。尽快释放lock。
        CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = PMDSPI->getDPMarketDataField(InstrumentID);
        if(pCurrentDepthMarketData ==NULL){
          continue;
        }
        //TThostFtdcInstrumentIDType CurrentInstrumentID=pCurrentDepthMarketData->InstrumentID;
        TThostFtdcPriceType CurrentBidPrice = pCurrentDepthMarketData->BidPrice1;
        TThostFtdcPriceType CurrentAskPrice = pCurrentDepthMarketData->AskPrice1;
        TThostFtdcPriceType CurrentLastPrice = pCurrentDepthMarketData->LastPrice;
        //cout<<"----------------"<<pCurrentDepthMarketData->InstrumentID<<endl;
        //cout<<"the current bid price is "<< pCurrentDepthMarketData->BidPrice1<<endl;
        //cout<<"the pre bid price is "<<pCurrentDepthMarketData->AskPrice1<<endl;
       // cout<<"++++++++++++++++++++++++++++++++"<<pCurrentDepthMarketData->InstrumentID<<endl;
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
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else if (CurrentAskPrice <= (AskPrice*(1 - THRESHOLD/100))){
  			    //说明卖一价已经跌停了，所以赶紧开多，就是赶紧买。
  			    FindFatFinger = true;
                cout<<"----------------"<<InstrumentID<<endl;
  			  	cout<<"the current ask price is "<<CurrentAskPrice<<endl;
  			  	cout<<"the pre ask price is "<<AskPrice<<endl;
  			  	cout<<"----------------"<<InstrumentID<<endl;
  			  	//说明卖一价跌停，所以赶紧开多,，就是赶紧买
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,CurrentBidPrice,MAXCOUNT);
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
  			  	    bool isLocked = PTRADERSPI->CheckToLock(InstrumentID);
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
        pthread_t tmp = pthread_self();
        cout<<"the thread "<<InstrumentID<<" is called  once "<<tmp<<endl;
    }
}

/*
void createThreadSet(){
  //根据订阅的合约编码的数量，创建线程ID
  pthread_t  ThreadIDs[NUMOFINSTRUMENTID];

	// 首先根据订阅的合约的编码创造相应的条件变量。
  // 同时创建相应的处理数据的线程。
  for (int i = 0; i < NUMOFINSTRUMENTID; ++i){
      string InstrumentID = PPINSTRUMENTID[i];
      PTHREADCONDS[InstrumentID] = PTHREAD_COND_INITIALIZER;
      pthread_create(&ThreadIDs[i],NULL,createThreadFun,(void *)&PTHREADCONDS[InstrumentID]);
  }
}
*/
