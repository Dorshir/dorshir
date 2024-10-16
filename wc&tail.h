#ifndef __wctail_h__
#define __wctail_h__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#define INITIAL_AVG_LINE_SIZE 80
#define MAX_LINE_LENGTH 1024


void Wc(char *fileName);

void Tail(const char *fileName, int n);


#endif