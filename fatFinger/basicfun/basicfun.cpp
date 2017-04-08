#include "basicfun.h"

void basicPrint(string error) {
  // cout<<"this basicPrint is called"<<endl;
  char *filePath = "error.txt";
  FILE *file_fd = fopen(filePath, "a");
  // error = "test";
  char *huiche = "\n";
  const char *data = error.data();
  int writeLen = fwrite(data, 1, strlen(data), file_fd);
  int writeLen1 = fwrite(huiche, 1, 1, file_fd);
  fclose(file_fd);
}

string ConvertGb18030ToUtf8(char message[]) {
  int OUTLEN = 255;
  // char *in_utf8 = message;
  char *in_gb2312 = message;
  char outbuf[OUTLEN];
  convert(message, strlen(message), outbuf, OUTLEN);
  string tmp = outbuf;
  return tmp;
}

//将输出信息从gb18030转换成ｕｆｔ８,方便在Ｌｉｎｕｘ终端显示。
void convert(char *inbuf, int inlen, char *outbuf, int outlen) {
  iconv_t cd = iconv_open("utf-8", "gb18030");
  char **pin = &inbuf;
  char **pout = &outbuf;

  memset(outbuf, 0, outlen);
  iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen);
  iconv_close(cd);
}

char MapDirection(char src, bool toOrig = true) {
  if (toOrig) {
    if ('b' == src || 'B' == src) {
      src = '0';
    } else if ('s' == src || 'S' == src) {
      src = '1';
    }
  } else {
    if ('0' == src) {
      src = 'B';
    } else if ('1' == src) {
      src = 'S';
    }
  }
  return src;
}

char MapOffset(char src, bool toOrig = true) {
  if (toOrig) {
    if ('o' == src || 'O' == src) {
      src = '0';
    } else if ('c' == src || 'C' == src) {
      src = '1';
    } else if ('j' == src || 'J' == src) {
      src = '3';
    }
  } else {
    if ('0' == src) {
      src = 'O';
    } else if ('1' == src) {
      src = 'C';
    } else if ('3' == src) {
      src = 'J';
    }
  }
  return src;
}

