#ifndef __UI_H__
#define __UI_H__

#include "limits.h"

void IdentificationUI(char* _username,char* _password);
void StatusUI(int _tag, int _status);
void FailMsgUI(int _tag);
void WorngChoice();
char mainUI();
int ClientUI();
void ClientInsertGroupName(char* _groupName);
void WorngGroupName();
void WorngGroupNameToJoin();
void ClientGroupCreateErrUI();
void EnterGroupNameToJoin(char* _group);


#endif /*__UI_H__*/