#include <stdio.h>
#include <string.h>
#define REGISTER 1
#define LOGIN 2 
#define EXIT 3
#include "limits.h"


void IdentificationUI(char* _username,char* _password);
void StatusUI(int _tag, int _status);
void FailMsgUI(int _tag);
char mainUI();