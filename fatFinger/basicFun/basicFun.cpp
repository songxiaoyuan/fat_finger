#include "basicFun.h"


void basicPrint(char error[]){
    cout<<"this basicPrint is called"<<endl;
    char *filePath = "error.txt";
    FILE* file_fd = fopen(filePath,"a");
    //error = "test";
    char *huiche = "\n";
    int writeLen = fwrite(error,1,strlen(error),file_fd);
    int writeLen1 = fwrite(huiche,1,1,file_fd);
    fclose(file_fd);
}
