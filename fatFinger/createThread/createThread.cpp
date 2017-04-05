#include "createThread.h"

extern CThostFtdcMdApi* pMdApi;
extern CThostFtdcTraderApi* pTraderApi;
extern TThostFtdcVolumeType MAXCOUNT;
extern int THRESHOLD;
extern TThostFtdcBrokerIDType appId;		// 应用单元
extern TThostFtdcUserIDType userId;		// 投资者代码
void *createThread(void *cond)
{
   TThostFtdcPriceType BidPrice = 0;
   TThostFtdcPriceType AskPrice = 0;
   TThostFtdcInstrumentIDType InstrumentID=NULL;
   while(1)
    {
        //pthread_cond_wait(&cond,&mutex);/*等待*/
        pthread_cond_wait((pthread_cond_t*)cond,&MUTEX);/*等待*/
        cout<<"this is thread a"<<endl;
        CThostFtdcDepthMarketDataField *pCurrentDepthMarketData = pMdApi->getDPMarketDataField()
        if (InstrumentID ==NULL){
        	  pthread_mutex_lock(&MUTEX);
              InstrumentID = pCurrentDepthMarketData->InstrumentID;
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
  			  	pTraderApi->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Sell,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else if (CurrentAskPrice <= (AskPrice*(1 - THRESHOLD/100))){
  			  	//说明卖一价跌停，所以赶紧开多,，就是赶紧买
  			  	pTraderApi->ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,CurrentBidPrice,MAXCOUNT);
  			  }
  			  else{
  			  	//说明现在已经不是乌龙指了，根据自己的判断，已经在阈值之下。
  			  	BidPrice = CurrentBidPrice;
  			  	AskPrice = CurrentAskPrice;
  			  }
        }
        
    }

}
