#ifndef BASICFUN_H
#define BASICFUN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iconv.h>
using namespace std;

void basicPrint(string error);

void convert(char *inbuf,int inlen,char *outbuf,int outlen);
string ConvertGb18030ToUtf8(char message[]);
#endif // BASICFUN_H
