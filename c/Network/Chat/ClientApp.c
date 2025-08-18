#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h> // strncpy
#include <stdlib.h> // malloc() , free()
#include <unistd.h> 
#include <sys/types.h>
#include "ClientApp.h"
#include "clientnet.h"
#include "limits.h"
#include "protocol.h"
#include <signal.h>
#include "UI.h"
#include "vector.h"
#include <mqueue.h>


#define QUEUE1 "/queue1"
#define QUEUE2 "/queue2"
#define MAX_MSG_COUNT 10

#define MAX_MSG_SIZE 4096
#define PASS 1
#define FAIL -1
#define AGAIN 0
#define PORT 8888
#define IP_ADDRS "127.0.0.1"
#define REGISTER 1
#define LOGIN 2 
#define EXIT 3 

#define CREATE_GROUP 1
#define JOIN_GROUP 2
#define LEAVE_GROUP 3
#define LOG_OUT 4


struct ClientApp
{
    ClientNet* _clientNet;
};

typedef struct GroupChat
{
    char m_groupName[MAX_GROUPNAME_LEN];
    pid_t m_pids[2];

}GroupChat;


static ClientAppResult ReqAndResCreateGroup(ClientApp* _app ,char* _userName ,char* _groupName, char* _addressBuffer);

static ClientAppResult Register(ClientApp* _app ,UserNamePass* _userId,char* _msg , char* _recMsg);

static GroupRequest* CreateGroupRequest(char* _userName ,char* _groupName);

static ClientAppResult LogIn(ClientApp* _app ,UserNamePass* _userId,char* _msg, char* _recMsg);

static int ReqAndConRegisterFromServer(ClientApp* _app, UserNamePass* _userId, char* _msg, char* _recMsg);

static int ReqAndConLogInFromServer(ClientApp* _app, UserNamePass* _userId, char* _msg, char* _recMsg);

static void FreeGroupRequest(GroupRequest* _group);

static ClientAppResult JoinGroup(ClientApp* _app, char* _groupName , char* _userName);

static ClientAppResult ReqJoinGroup(ClientApp* _app ,char* _userName ,char* _groupName, char* _addressBuffer);

static ClientAppResult CreateGroup(ClientApp* _app, char* _groupName ,char* _userName);

static void JoinChat(char* _chatIp, GroupChat* _groupChat);

static void GetPid(GroupChat* _chat);

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
    
    Vector* groupList = VectorCreate(5,5); //define
    if(groupList == NULL)
    {
        free(app);
        return NULL;
    }
    ClientNet* clientNet = ClientNet_Create(IP_ADDRS,PORT);
    if(clientNet == NULL)
    {
        free(app);
        free(groupList);
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
    ClientAppResult result;
    int status = FAIL;
    char newGroupName[MAX_GROUPNAME_LEN];
    char existGroup[MAX_GROUPNAME_LEN];


    char choice;
    //maybe function.
    while(status != PASS && status != EXIT)
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
                //function that check username and password.
                // while(IdentificationCheck(username,password) == FAIL)
                // {
                //     WorngIdentificationUI();
                //     IdentificationUI(username,password);
                // }
                userId->m_userName = username;
                userId->m_password = password;
                result = Register(_app,userId,msg,recMsg);
                if(result == CLIENTAPP_SUCSSES)
                {
                    if(*recMsg == 1)
                    {
                        StatusUI(REGISTER,PASS);
                        break;
                    }
                }
                FailMsgUI(REGISTER); // UI FUNCTION
                break;

            case LOGIN:
                IdentificationUI(username,password);
                //function that check username and password.
                // while(IdentificationCheck(username,password) == FAIL)
                // {
                //     WorngIdentificationUI();
                //     IdentificationUI(username,password);
                //
                //
                // }
                userId->m_userName = username;
                userId->m_password = password;
                result = LogIn(_app,userId,msg,recMsg);
                if(result == CLIENTAPP_SUCSSES)
                {
                    if(*recMsg == 1)
                    {
                        StatusUI(LOGIN,PASS);
                        status = PASS;
                        break;
                    }
                }
                FailMsgUI(LOGIN);
                break;
            
            case EXIT: 
                status = EXIT;
                break;
            
            default:
                WorngChoice();
                break;
        }
    }
    if (status == PASS)
    {
        status = 0;
        while(status != EXIT)
        {
            choice = '0';
            choice = ClientUI();
            
            switch (choice)
            {
            case CREATE_GROUP:
                
                // while(FixGroupName(groupName) != PASS)
                // {
                //     if(status == FAIL)
                //     {
                //         WorngGroupName();
                //     }
                    ClientInsertGroupName(newGroupName);
                // }
                CreateGroup(_app,newGroupName,username);
                break;
            
            case JOIN_GROUP:
                
                EnterGroupNameToJoin(existGroup);
                ClientAppResult status = JoinGroup(_app,existGroup,username);
                if(status != CLIENTAPP_SUCSSES)
                {
                    WorngGroupNameToJoin();
                }
                break;
            
            case LEAVE_GROUP:
                /* code */
                break;
            
            case LOG_OUT:
                // leave all groups.
                status = EXIT;
                break;
            default:
                WorngChoice();
                break;
            }
        }
    }

    return PASS;
}

void DestroyApp(ClientApp** _app)
{
    if (_app == NULL || *_app == NULL)
    {
        return;
        return;
    }
    ClientNet_Destroy(&(*_app)->_clientNet);
    free((*_app));
    *_app = NULL;
}

// AUXILIARY FUNCTIONS :


static ClientAppResult CreateGroup(ClientApp* _app, char* _groupName ,char* _userName)
{
    //ask for list
    //print the list ui function
    GroupChat* groupChat = (GroupChat*)malloc(sizeof(GroupChat));
    if (groupChat == NULL)
    {
        //function from UI print status.
        return CLIENTAPP_ALLOCATION_FAILED; 
    }
    char addressBuffer[MAX_ADDR_SIZE];
    ClientAppResult status = ReqAndResCreateGroup(_app,_userName,_groupName,addressBuffer);
    if(status != CLIENTAPP_SUCSSES)
    {
        // function from UI print status.
        return status;
    }
    size_t groupNameLen = strlen(_groupName);
    memcpy(groupChat->m_groupName,_groupName,groupNameLen);
    groupChat->m_groupName[groupNameLen] = '\0';
    JoinChat(addressBuffer,groupChat);
    // insert to data base.
    printf("chat name : %s\npid1 = %d\npid2 = %d\n",groupChat->m_groupName,groupChat->m_pids[0],groupChat->m_pids[1]);
    return CLIENTAPP_SUCSSES;
    //else print error (UI FUNCTION).
}

static ClientAppResult ReqAndResCreateGroup(ClientApp* _app ,char* _userName ,char* _groupName, char* _addressBuffer) // fix if!!
{
    GroupRequest* groupReq = CreateGroupRequest(_userName,_groupName);
    if(groupReq == NULL)
    {
        return CLIENTAPP_ALLOCATION_FAILED;
    }
    char msg[MAX_MSG_SIZE];
    char reqBuffer[MAX_MSG_SIZE];
    char resBuffer[MAX_MSG_SIZE];

    
    size_t msgSize = PackCreateGroupReq(groupReq,reqBuffer);
    if (msgSize > 0)
    {
        int msgLen = ClientNet_Send(_app->_clientNet, reqBuffer, msgSize);
        if(msgLen > 0)
        {
            // printf("Sent registration request, waiting for response...\n");  // make it function of UI
            msgLen = ClientNet_Receive(_app->_clientNet, resBuffer, MAX_MSG_SIZE);
            if(msgLen > 0)
            {             
                CreateGroupResponse* response = malloc(sizeof(CreateGroupResponse));
                if(response == NULL) // can be function
                {
                    FreeGroupRequest(groupReq);
                    return CLIENTAPP_ALLOCATION_FAILED;
                }
                response->m_address = malloc(sizeof(char)*(MAX_ADDR_SIZE +1));
                if(response->m_address == NULL)
                {
                    FreeGroupRequest(groupReq);
                    free(response);
                    return CLIENTAPP_ALLOCATION_FAILED;
                }
                
                ProtocolResult status = UnPackCreateGroupResWithAddress(resBuffer,msgLen,response);
                if (status == PROTOCOL_SUCCESS)
                {
                    if(response->m_status == PASS)
                    {
                        size_t addrLen = strlen(response->m_address);
                        memcpy(_addressBuffer,response->m_address,addrLen);
                        _addressBuffer[addrLen]='\0';
                    }
                    FreeGroupRequest(groupReq);
                    free(response->m_address);
                    free(response);
                    return CLIENTAPP_SUCSSES;
                }
                else
                {
                    printf("Failed to unpack server response\n");
                    
                }
                free(response->m_address);
                free(response);
                FreeGroupRequest(groupReq);
                return CLIENTAPP_COMMUNICATION_ERROR;
            }
            else
            {
                printf("Failed to receive server response\n");
                FreeGroupRequest(groupReq);
                return CLIENTAPP_COMMUNICATION_ERROR;
            }
        }
        else
        {
            printf("Failed to send registration request\n");
        }
        
    }
    FreeGroupRequest(groupReq);
    return CLIENTAPP_COMMUNICATION_ERROR;  
}

static GroupRequest* CreateGroupRequest(char* _userName ,char* _groupName)
{
    size_t usernameLen = strlen(_userName);
    size_t groupnameLen = strlen(_groupName);
    GroupRequest* group = malloc(sizeof(GroupRequest)) ;
    if(group == NULL)
    {
        return NULL;
    }
    group->m_userName = malloc(sizeof(char)*(usernameLen + 1));
    if(group->m_userName == NULL)
    {
        free(group);
        return NULL;
    }
    group->m_groupName = malloc(sizeof(char)*(groupnameLen + 1));
    if(group->m_groupName == NULL)
    {
        free(group->m_userName);
        free(group);
        return NULL;
    }
    memcpy(group->m_userName,_userName,usernameLen);
    (group->m_userName)[usernameLen] = '\0';
    memcpy(group->m_groupName,_groupName,groupnameLen);
    (group->m_groupName)[groupnameLen] = '\0';

    return group; 
}

static void FreeGroupRequest(GroupRequest* _group)
{
    free(_group->m_userName);
    free(_group->m_groupName);
    free(_group);
}

static ClientAppResult JoinGroup(ClientApp* _app, char* _groupName , char* _userName)
{
    char addressBuffer[MAX_ADDR_SIZE];
    GroupChat* groupChat = (GroupChat*)malloc(sizeof(GroupChat));
    if (groupChat == NULL)
    {
        return CLIENTAPP_ALLOCATION_FAILED; 
    }  
    ClientAppResult status = ReqJoinGroup(_app,_userName,_groupName,addressBuffer);
    if(status != CLIENTAPP_SUCSSES)
    {
        free(groupChat);
        return status;
    }
    size_t groupNameLen = strlen(_groupName);
    memcpy(groupChat->m_groupName,_groupName,groupNameLen);
    groupChat->m_groupName[groupNameLen] = '\0';
    JoinChat(addressBuffer, groupChat);
    return status;
}

static ClientAppResult ReqJoinGroup(ClientApp* _app ,char* _userName ,char* _groupName, char* _addressBuffer) // change to enum
{

    GroupRequest* group = CreateGroupRequest(_userName ,_groupName);
    
    if(group == NULL)
    {
        return CLIENTAPP_ALLOCATION_FAILED;
    }
    char reqBuffer[MAX_MSG_SIZE]; 
    char resBuffer[MAX_MSG_SIZE];

    size_t msgSize = PackJoinGroupReq(group,reqBuffer);
    if (msgSize > 0)
    {
       
        int msgLen = ClientNet_Send(_app->_clientNet, reqBuffer, msgSize);
        if(msgLen > 0)
        {
            printf("Sent registration request, waiting for response...\n");  // make it function of UI
            msgLen = ClientNet_Receive(_app->_clientNet, resBuffer, MAX_MSG_SIZE);
            if(msgLen > 0)
            {             
                JoinGroupResponse* response = malloc(sizeof(JoinGroupResponse));
                if(response == NULL)
                {
                    //free to all until now
                    return CLIENTAPP_ALLOCATION_FAILED;
                }
                response->m_address = malloc(sizeof(char)*(MAX_ADDR_SIZE +1));
                if(response->m_address == NULL)
                {
                    //free to all until now
                    free(response);
                    return CLIENTAPP_ALLOCATION_FAILED;
                }
                ProtocolResult status = UnPackJoinGroupResWithAddress(resBuffer,msgLen,response);
                if (status == PROTOCOL_SUCCESS)
                {
                    if(response->m_status == PASS)
                    {
                        size_t addrLen = strlen(response->m_address);
                        memcpy(_addressBuffer,response->m_address,addrLen);
                        _addressBuffer[addrLen]='\0';
                        //free to all.
                        return CLIENTAPP_SUCSSES;
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
    return CLIENTAPP_COMMUNICATION_ERROR;    
}



static void JoinChat(char* _chatIp, GroupChat* _groupChat)
{
    char strRec[MAX_MSG_SIZE];
    char strSend[MAX_MSG_SIZE];
    sprintf(strRec ,"xterm -hold -e './receive_app.out %s %d %s' &",_chatIp,PORT,QUEUE2);
    sprintf(strSend ,"xterm -hold -e './send_app.out %s %d %s' &",_chatIp,PORT,QUEUE1);
    system(strRec);
    system(strSend);
    GetPid(_groupChat);
}

static void GetPid(GroupChat* _chat)
{
    mqd_t mq1,mq2;
    char bufferPid[MAX_MSG_SIZE];
    struct mq_attr attr;
    int index = 0;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_COUNT;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    mq1 = mq_open(QUEUE1, O_RDWR | O_CREAT, 0666, &attr);
    if (mq1 == (mqd_t)-1) 
    {
        perror("Failed to create/open queue1");
        exit(1);
    }
    
    mq2 = mq_open(QUEUE2, O_RDWR | O_CREAT, 0666, &attr);
    if (mq1 == (mqd_t)-1) 
    {
        perror("Failed to create/open queue1");
        exit(1);
    }

    mq_receive(mq1, bufferPid, MAX_MSG_SIZE, NULL);
    printf("received: %s\n", bufferPid);
    int pid1 = atoi(bufferPid);
    printf("%d",pid1);
    _chat->m_pids[0] = pid1;

    mq_receive(mq2, bufferPid, MAX_MSG_SIZE, NULL);
    printf("received: %s\n", bufferPid);
    int pid2 = atoi(bufferPid);
    printf("%d",pid2);
    _chat->m_pids[1] = pid2;

    mq_close(mq1);
    mq_close(mq2);
}

static void KillProcesses(pid_t _pidRecive , pid_t _pidSend)
{
    kill(_pidRecive,SIGKILL);
    kill(_pidSend,SIGKILL);   
}

static ClientAppResult LogIn(ClientApp* _app ,UserNamePass* _userId,char* _msg, char* _recMsg)
{
    if(_userId->m_userName == NULL || _userId->m_password == NULL)
    {
         return CLIENTAPP_LUCK_OF_DATA;
    }
    
    if(ReqAndConLogInFromServer(_app,_userId,_msg,_recMsg) == PASS)
    {
        return CLIENTAPP_SUCSSES;    
    }  
    return CLIENTAPP_COMMUNICATION_ERROR;
}

static ClientAppResult Register(ClientApp* _app ,UserNamePass* _userId, char* _msg, char* _recMsg)
{
    if(_userId->m_userName == NULL || _userId->m_password == NULL)
    {
         return CLIENTAPP_LUCK_OF_DATA;
    }
    
    if(ReqAndConRegisterFromServer(_app,_userId,_msg,_recMsg) == PASS)
    {
        return CLIENTAPP_SUCSSES;    
    }
    return CLIENTAPP_COMMUNICATION_ERROR;
}

static int ReqAndConRegisterFromServer(ClientApp* _app, UserNamePass* _userId, char* _msg, char* _recMsg)
{
    char recBuffer[MAX_MSG_SIZE]; 
    size_t msgSize = PackRegReq(_userId, _msg); 
    if (msgSize > 0)
    {
       
        int msgLen = ClientNet_Send(_app->_clientNet, _msg, msgSize);
        if(msgLen > 0)
        {
            printf("Sent registration request, waiting for response...\n");  // make it function of UI
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

static int ReqAndConLogInFromServer(ClientApp* _app, UserNamePass* _userId, char* _msg, char* _recMsg)
{
    char recBuffer[MAX_MSG_SIZE]; 
    size_t msgSize = PackLoginReq(_userId, _msg);
    if (msgSize > 0)
    {
       
        int msgLen = ClientNet_Send(_app->_clientNet, _msg, msgSize); 
        if(msgLen > 0)
        {
            printf("Sent LogIn request, waiting for response...\n");  // make it function of UI
            msgLen = ClientNet_Receive(_app->_clientNet, recBuffer, MAX_MSG_SIZE);
            if(msgLen > 0)
            {             
                ProtocolResult status = UnPackLoginRes(recBuffer, msgLen, _recMsg);
                if (status == PROTOCOL_SUCCESS)
                {
                    if(*_recMsg == PASS)
                    {
                        return PASS;
                    }
                }
                else
                {
                    printf("Failed to unpack server response\n"); // UI FUNCTION
                }
            }
            else
            {
                printf("Failed to receive server response\n"); // UI FUNCTION
            }
        }
        else
        {
            printf("Failed to send LogIn request\n"); // UI FUNCTION
        }
    }
    return FAIL;    
}
