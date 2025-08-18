#include <stdio.h>
#include <string.h>
#define REGISTER 1
#define LOGIN 2 
#define EXIT 3
#define JOIN_GROUP 2
#define CREATE_GROUP 1
#define LEAVE_GROUP 3
#define LOG_OUT 4
#include "limits.h"


void IdentificationUI(char* _username,char* _password)
{
    char username[2 * MAX_USERNAME_LEN];
    char password[2 * MAX_PASSWORD_LEN];
    int check = 0;
    while(1)
    {
        printf("enter username : ");
        scanf("%s",username);
        //check = usernameRules(username);
        // if(check == 1)
        // {
            break;
        // }
        // printf("iligal Username, choose username again\n");
    }
    while(1)
    {
        printf("enter password : ");
        scanf("%s",password);
        //check = passwordRules(username);
        // if(check == PASS)
        // {
            break;
        // }
        // printf("iligal password, choose password again \n");
        
    }
    int usernameLen = strlen(username);
    int passwordLen = strlen(password);
    memcpy(_username,username,usernameLen);
    _username[usernameLen] = '\0';
    memcpy(_password,password,passwordLen);
    _password[passwordLen] = '\0';
}

void StatusUI(int _tag, int _status)
{
    if(_tag == REGISTER)
    {
        if (_status == FAIL)
        {
            printf("User already exists.\n");
        }
        printf("User successfully created.\n");
    }
    if(_tag == LOGIN)
    {
        if (_status == FAIL)
        {
            printf("Worng username/password, please try again.\n");
        }
        printf("Login success.\n");
    }
}

void FailMsgUI(int _tag)
{
    if(_tag == REGISTER)
    {
        printf("Registration failed\n");
    }
    if(_tag == LOGIN)
    {
        printf("Login failed.\n");
    }
    
}
void WorngChoice()
{
    printf("Worng choice, try again.\n");
}

int mainUI()
{
    // ODB = Omri Dor Barak.
    char choice;
    printf("wellcome to ODB chat!\npress 1 to register\npress 2 to log in\npress 3 to exit\n");
    printf("please enter your choice : ");
    scanf(" %c", &choice);
    if(choice == '1')
    {
        return REGISTER;    
    }
    if(choice == '2')
    {
        return LOGIN;    
    }
    if(choice == '3')
    {
        return EXIT;    
    }
    return -1;
}
int ClientUI() // AfterLoginUI
{
    // ODB = Omri Dor Barak.
    char choice;
    printf("Hello user!\npress 1 to create group\npress 2 to join group\npress 3 to leave group\npress 4 to log out\n");
    printf("please enter your choice : ");
    scanf(" %c", &choice);
    if(choice == '1')
    {
        return CREATE_GROUP;    
    }
    if(choice == '2')
    {
        return JOIN_GROUP;    
    }
    if(choice == '3')
    {
        return LEAVE_GROUP;    
    }
    if (choice == '4')
    {
        return LOG_OUT;
    }
    return -1;
}

void ClientInsertGroupName(char* _groupName)
{
    char groupName[2 * MAX_GROUPNAME_LEN];
    printf("enter group name (15 characters max) : ");
    scanf("%s",groupName);
    int groupNameLen = strlen(groupName);
    memcpy(_groupName,groupName,groupNameLen);
    _groupName[groupNameLen] = '\0';
}

void EnterGroupNameToJoin(char* _group)
{
    char groupName[MAX_GROUPNAME_LEN];
    printf("Enter group name to join : ");
    scanf("%s",groupName);
    int groupNameLen = strlen(groupName);
    memcpy(_group,groupName,groupNameLen);
    _group[groupNameLen] = '\0';
}


void WorngGroupName()
{
    printf("worng group name,maximum 15 characters without space.\n");
}

void WorngGroupNameToJoin()
{
    printf("Worng group name, please try again.\n");
}

void ClientGroupCreateErrUI()
{
    printf("Group create failed.\n");
}