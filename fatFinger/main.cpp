//#include "main.h"
#include "config.h"
#include "init/init.h"
#include "createThread/createThread.h"
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
  int i;
  initApiAndSpi();
   cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] userLogin        -- 登录"<<endl;
    cerr<<" [2] createThreadSet  -- 创建线程"<<endl;
    cerr<<" [3] userSubscribe    -- 用户订阅信息"<<endl;
    cerr<<" [4]  Exit            -- 退出"<<endl;
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
              //初始化处理数据的线程，根据订阅编码的列表，订阅多少个创建多少个线程处理数据
              createThreadSet();
              break;
            }
      case 3:{
              // 用户订阅信息，此时线程会根据传回来的数据处理是不是乌龙指，
              // 如果发现是乌龙指会自动下单，同时在乌龙指结束的时候自动锁仓。
              userSubscribe();
              break;
            }
      case 4:{
              // 程序退出，或者可以做别的操作。
              exit(0);
              break;
            }
    }
  }

  return 0;
}
