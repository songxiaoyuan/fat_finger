//#include "main.h"
#include "config.h"
#include "application/application.h"
//#include "init/init.h"
//#include "createThread/createThread.h"
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]){
  //g_hEvent=CreateEvent(NULL, true, false, NULL);

  //if(argc < 2)  cerr<<"格式: 命令 参数, 输入有误."<<endl;
  //else if(strcmp(argv[1],"--md")==0)    test_md();
  //else if(strcmp(argv[1],"--order")==0) test_order();
  //test_md();
  // test_trader();
  //初始化环境，建立md和trader的API和spi


/*
  int i;
  initApiAndSpi();
    cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] userLogin        -- 登录"<<endl;
    cerr<<" [2] userSubscribe    -- 用户订阅信息,此时会自动创建处理数据线程"<<endl;
     cerr<<"[3] printOrders      -- 查询现在所有订单的状态"<<endl;
     cerr<<"[4] printTrades      -- 查询现在所有成交订单的状态"<<endl;
    cerr<<" [5]  Exit            -- 退出"<<endl;
    cerr<<"----------------------------------------------"<<endl;
  while(1){
    cin>>i;
    switch(i){
      case 1:{
              //用户登录信息，包括md登录和trader登录
              userLogin();
              break;
            }
      case 2:{
              // 用户订阅信息，此时线程会根据传回来的数据处理是不是乌龙指，
              // 如果发现是乌龙指会自动下单，同时在乌龙指结束的时候自动锁仓。
              userSubscribe();
              break;
            }
      case 3:{
              // 程序退出，或者可以做别的操作。
              printOrders();
              break;
            }
          case 4:{
              // 程序退出，或者可以做别的操作。
              printTrades();
              break;
            }
    case 5:{
              // 程序退出，或者可以做别的操作。
              exit(0);
              break;
            }
    }
  }
  */
  application* app = new application();
  APPLICATION =app;
  app->Init();
  app->Run();

  return 0;
}
