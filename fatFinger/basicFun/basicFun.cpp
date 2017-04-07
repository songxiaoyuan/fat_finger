#include "basicFun.h"

void basicPrint(char error[]){
    //cout<<"this basicPrint is called"<<endl;
    char *filePath = "error.txt";
    FILE* file_fd = fopen(filePath,"a");
    //error = "test";
    char *huiche = "\n";
    int writeLen = fwrite(error,1,strlen(error),file_fd);
    int writeLen1 = fwrite(huiche,1,1,file_fd);
    fclose(file_fd);
}




// 代码转换操作类
class CodeConverter {
private:
iconv_t cd;
public:
// 构造
CodeConverter(const char *from_charset,const char *to_charset) {
cd = iconv_open(to_charset,from_charset);
}

// 析构
~CodeConverter() {
iconv_close(cd);
}

// 转换输出
int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
char **pin = &inbuf;
char **pout = &outbuf;

memset(outbuf,0,outlen);
return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
}
};

/* this is ok!!
//将输出信息从gb18030转换成ｕｆｔ８,方便在Ｌｉｎｕｘ终端显示。
string ConvertGb18030ToUtf8(char message[]){
 int OUTLEN = 255;
  //char *in_utf8 = message;
 char *in_gb2312 = message;
 char outbuf[OUTLEN];

 CodeConverter cc2 = CodeConverter("gb2312","utf-8");
 cc2.convert(in_gb2312,strlen(in_gb2312),outbuf,OUTLEN);

return outbuf;
}
*/
/*
//将输出信息从gb18030转换成ｕｆｔ８,方便在Ｌｉｎｕｘ终端显示。
string ConvertGb18030ToUtf8(char message[]){
 int OUTLEN = 255;
  //char *in_utf8 = message;
 char *in_gb2312 = message;
 char outbuf[OUTLEN];

 CodeConverter cc2 = CodeConverter("gb2312","utf-8");
 cc2.convert(in_gb2312,strlen(in_gb2312),outbuf,OUTLEN);

return outbuf;
}
*/


string ConvertGb18030ToUtf8(char message[]){
 int OUTLEN = 255;
  //char *in_utf8 = message;
 char *in_gb2312 = message;
 char outbuf[OUTLEN];
 convert(message,strlen(message),outbuf,OUTLEN);
 string tmp = outbuf;
 return tmp;
}



//将输出信息从gb18030转换成ｕｆｔ８,方便在Ｌｉｎｕｘ终端显示。
void convert(char *inbuf,int inlen,char *outbuf,int outlen){
 iconv_t cd = iconv_open("utf-8","gb18030");
 char **pin = &inbuf;
 char **pout = &outbuf;

  memset(outbuf,0,outlen);
  iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
 iconv_close(cd);
}

