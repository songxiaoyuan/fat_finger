#include "createThread.h"

// 创建线程时，线程的运行函数。
//传入的参数是一个条件变量，用来等待相应的合约代码的数据到来，从而处发线程
void *createThreadFun(void *cond){
   TThostFtdcPriceType BidPrice = 0;
   TThostFtdcPriceType AskPrice = 0;
   TThostFtdcInstrumentIDType InstrumentID="0";
   while(1){
        //pthread_cond_wait(&cond,&mutex);/*等待*/
        pthread_cond_wait((pthread_cond_t*)cond,&MUTEX);/*等待*/
        cout<<"this is thread a"<<endl;
        CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = PMDSPI->getDPMarketDataField();
        if (!strcmp(InstrumentID,"0")){
            //说明这是第一次调用此函数，初始化函数的基本信息。
        	    pthread_mutex_lock(&MUTEX);
        	    strcpy(InstrumentID, pCurrentDepthMarketData->InstrumentID);
              //InstrumentID = pCurrentDepthMarketData->InstrumentID;
              BidPrice = pCurrentDepthMarketData->BidPrice1;
              AskPrice = pCurrentDepthMarketData->AskPrice1;
  			      pthread_mutex_unlock(&MUTEX);
        }
        else{
        	  pthread_mutex_lock(&MUTEX);
              TThostFtdcPriceType CurrentBidPrice = pCurrentDepthMarketData->BidPrice1;
              TThostFtdcPriceType CurrentAskPrice = pCurrentDepthMarketData->AskPrice1;
  			  pthread_mutex_unlock(&MUTEX);
  			  if (CurrentBidPrice >=(BidPrice*(1+THRESHOLD/100))){
  			  	//说明买一价已经涨停，所以赶紧开空,就是赶紧卖
  			  	/* code */
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else if (CurrentAskPrice <= (AskPrice*(1 - THRESHOLD/100))){
  			  	//说明卖一价跌停，所以赶紧开多,，就是赶紧买
  			  	PTRADERSPI->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else{
  			  	//说明现在已经不是乌龙指了，根据自己的判断，已经在阈值之下。
  			  	BidPrice = CurrentBidPrice;
  			  	AskPrice = CurrentAskPrice;
  			  }
        }
    }
}

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
