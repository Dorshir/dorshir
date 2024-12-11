#include <stdio.h>
#include <string.h> // strncpy
#include <stdlib.h> // malloc() , free()
#include <unistd.h> 
#include "ClientApp.h"
#include "clientnet.h"
#include "limits.h"
#include "protocol.h"
#include "UI.h"

#define MAX_MSG_SIZE 4096
#define PASS 1
#define FAIL -1
#define AGAIN 0
#define PORT 8888
#define IP_ADDRS "127.0.0.1"
#define REGISTER 1
#define SIGN_IN 2 
#define EXIT 3 


struct ClientApp
{
    ClientNet* _clientNet;
};

static ClientAppResult Register(ClientApp* _app ,UserNamePass* _userId,char* _msg , char* _recMsg);

static int ConfirmationFromServer(ClientApp* _app,UserNamePass* _userId, char* _msg,char* _recMsg);

// MAIN FUNCTIONS :

ClientApp* CreateApp ()
{
    ClientApp* app = NULL;
    app = (ClientApp*)malloc(sizeof(ClientApp));
    if(app == NULL)
    {
        return NULL;
        printf("app allocation failed\n");
    }
    char ipAdd[] = IP_ADDRS;
    ClientNet* clientNet = ClientNet_Create(ipAdd,PORT);
    if(clientNet == NULL)
    {
        free(app);
        printf("client net allocation failed\n");
        return NULL;
    }
    app->_clientNet = clientNet;
    printf("CreateApp pass\n");
    return app;
}

int RunApp(ClientApp* _app)
{
    
    char username[MAX_USERNAME_LEN];
    char password[MAX_USERNAME_LEN];
    char msg[MAX_MSG_SIZE];
    char recMsg[MAX_MSG_SIZE];
    int exitFlag = 1;
    ClientAppResult result;
    int status = FAIL;

    char choice;
    //maybe function.
    while(status != PASS || exitFlag != 0)
    {
        UserNamePass* userId = (UserNamePass*)malloc(sizeof(UserNamePass));
        if(userId == NULL)
        {
            printf("userId stract allocation failed\n");
            return FAIL;
        }
        choice = mainUI();
        switch (choice)
        {
            case REGISTER:
                IdentificationUI(username,password); //UI FUNCTION
                userId->m_userName = username;
                userId->m_password = password;
                result = Register(_app,userId,msg,recMsg);
                if(result == CLIENTAPP_SUCSSES)
                {
                    if(*recMsg == 1)
                    {
                        StatusUI(PROTOCOL_REGISTRATION_REQ,status); // UI FUNCTION
                        break;
                    }
                }
                FailMsgUI(PROTOCOL_REGISTRATION_REQ); // UI FUNCTION
                break;

            case SIGN_IN:
                // IdentificationUI(username,password);
                // userId->m_userName = username;
                // userId->m_password = password;
                // result = SignIn(_app,userId,msg);
                // if(result == CLIENTAPP_SUCSSES)
                // {
                //     //function that check the msg and return status pass or failed.
                //     StatusUI(_tag,status); // UI FUNCTION
                //     break;
                // }
                // FailMsgUI(_tag); // UI FUNCTION
                break;
            
            case EXIT: 
                exitFlag = 0;
                return PASS;
                break;
            
            default:
                //WorngChoice();
                exitFlag = 0; //temp
                break;
        }
    }
    return PASS;
}

void DestroyApp(ClientApp** _app)
{
    if (_app == NULL || *_app == NULL)
    {
        return;
    }
    ClientNet_Destroy(&(*_app)->_clientNet);
    free((*_app));
    *_app = NULL;
}

// AUXILIARY FUNCTIONS :

// static ClientAppResult SignIn(ClientApp* _app ,char* _username , char* _password,char* _msg)
// {
//     if(_username == NULL || _password == NULL)
//     {
//          return CLIENTAPP_LUCK_OF_DATA;
//     }
    
//     if(ConfirmationFromServer(_app,_userId,_msg,_recMsg) == FAIL) // TAG WILL BE ENUM second value
//     {
//         return CLIENTAPP_COMMUNICATION_ERROR;    
//     }
//     return CLIENTAPP_SUCSSES;
// }

static ClientAppResult Register(ClientApp* _app ,UserNamePass* _userId, char* _msg, char* _recMsg)
{
    if(_userId->m_userName == NULL || _userId->m_password == NULL)
    {
         return CLIENTAPP_LUCK_OF_DATA;
    }
    
    if(ConfirmationFromServer(_app,_userId,_msg,_recMsg) == PASS)
    {
        printf("Registration successful!\n");  
        return CLIENTAPP_SUCSSES;    
    }
    printf("Registration failed!\n");  
    return CLIENTAPP_COMMUNICATION_ERROR;
}


static int ConfirmationFromServer(ClientApp* _app, UserNamePass* _userId, char* _msg, char* _recMsg)
{
    char recBuffer[MAX_MSG_SIZE]; 
    size_t msgSize = PackRegReq(_userId, _msg); 
    if (msgSize > 0)
    {
       
        int msgLen = ClientNet_Send(_app->_clientNet, _msg, msgSize);
        if(msgLen > 0)
        {
            printf("Sent registration request, waiting for response...\n");  
            msgLen = ClientNet_Receive(_app->_clientNet, recBuffer, MAX_MSG_SIZE);
            if(msgLen > 0)
            {             
                ProtocolResult status = UnPackRegRes(recBuffer, msgLen, _recMsg);
                if (status == PROTOCOL_SUCCESS)
                {
                    if(*_recMsg == PASS)
                    {
                        return PASS;
                    }
                }
                else
                {
                    printf("Failed to unpack server response\n");
                }
            }
            else
            {
                printf("Failed to receive server response\n");
            }
        }
        else
        {
            printf("Failed to send registration request\n");
        }
    }
    return FAIL;    
}
