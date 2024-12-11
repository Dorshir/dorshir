#include <stdio.h>
#include <string.h>
#define REGISTER 1
#define LOGIN 2 
#define EXIT 3
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
    memcpy(_password,password,passwordLen);
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

char mainUI()
{
    // ODB = Omri Dor Barak.
    char choice;
    printf("wellcome to ODB chat!\n press 1 to register\npress 2 to log in\npress 3 to exit\n");
    printf("please enter your choice : ");
    scanf("%s",&choice);
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

    
}