#ifndef BASICFUN_H
#define BASICFUN_H

#include <fcntl.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

void basicPrint(string error);

void convert(char *inbuf, int inlen, char *outbuf, int outlen);
string ConvertGb18030ToUtf8(char message[]);

char MapDirection(char src, bool toOrig);
char MapOffset(char src, bool toOrig);
#endif  // BASICFUN_H
