#include "serverApp.h"
#include "server_net.h"
#include "protocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "userManager.h"
#include "groupManager.h"
#include "limits.h"

#define MAX_USERNAME_SIZE 256
#define MAX_PASSWORD_SIZE 256
#define MAX_GROUP_NAME_SIZE 256

struct ServerApp
{
    UserManager *m_userManager;
    GroupManager* m_groupManager;
};

// helper functions declelertion

static void SendErrorResponse(int _clientSocket);
static void HandleRegistrationRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);
static void HandleLoginRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);
static void HandleCreateGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);
static void HandleGetGroupsRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);
static void HandleJoinGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);
static void HandleLeaveGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length);

// Main functions

ServerApp *ServerApp_Create(UserManager *_userMng, GroupManager* _groupMng)
{
    if (_userMng == NULL)
    {
        return NULL;
    }
    ServerApp *app = (ServerApp *)malloc(sizeof(ServerApp));
    if (!app)
    {
        return NULL;
    }
    app->m_userManager = _userMng;
    app->m_groupManager = _groupMng;
    return app;
}

void ServerApp_Destroy(ServerApp **_app)
{
    if (!_app || !*_app)
    {
        return;
    }
    free(*_app);
    *_app = NULL;
}

void ServerAppHandler(int _clientSocket, ProtocolTag _tag, char* _buffer, size_t _length, void* _context)
{
    ServerApp* app = (ServerApp*)_context;
    if (!app)
    {
        printf("ServerAppHandler: NULL context\n");
        SendErrorResponse(_clientSocket);
        return;
    }
    switch (_tag)
    {
        case PROTOCOL_REGISTRATION_REQ:
            HandleRegistrationRequest(app, _clientSocket, _buffer, _length);
            break;
            
        case PROTOCOL_LOGIN_REQ:
            HandleLoginRequest(app, _clientSocket, _buffer, _length);
            break;

        case PROTOCOL_CREATE_GROUP_REQ:  
            HandleCreateGroupRequest(app, _clientSocket, _buffer, _length);
            break;
            
        case PROTOCOL_GET_GROUPS_REQ:
            HandleGetGroupsRequest(app, _clientSocket, _buffer, _length);
            break;     

        case PROTOCOL_JOIN_GROUP_REQ:
            HandleJoinGroupRequest(app, _clientSocket, _buffer, _length);
            break;

        case PROTOCOL_LEAVE_GROUP_REQ:
            HandleLeaveGroupRequest(app, _clientSocket, _buffer, _length);
            break;
            
        default:
            printf("Unknown protocol tag received: %d\n", _tag);
            SendErrorResponse(_clientSocket);
            break;
    }
}

// helper functions:

static void SendErrorResponse(int _clientSocket)
{
   char responseBuffer[BUFFER_SIZE];
   size_t responseSize = PackRegRes(0, responseBuffer);

   if (responseSize > 0)
   {
       if (ServerSendMessage(_clientSocket, PROTOCOL_REGISTRATION_RES, responseBuffer, responseSize) < 0)
       {
           printf("Failed to send error response to client\n");
       }
   }
}

static void HandleRegistrationRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
   UserNamePass userData;
   char responseBuffer[BUFFER_SIZE];
   char status;
   size_t responseSize;
   UserManagerResult managerResult;
   userData.m_userName = malloc(MAX_USERNAME_SIZE);
   userData.m_password = malloc(MAX_PASSWORD_SIZE);
   if (!userData.m_userName || !userData.m_password)
   {
       printf("Memory allocation failed.\n");
       free(userData.m_userName);
       free(userData.m_password);
       SendErrorResponse(_clientSocket);
       return;
   }
   ProtocolResult result = UnPackRegReq((char*)_buffer, _length, &userData);
   if (result != PROTOCOL_SUCCESS)
   {
       printf("Invalid registration request.\n");
       free(userData.m_userName);
       free(userData.m_password);
       SendErrorResponse(_clientSocket);
       return;
   }
   printf("Registration request: Username: %s\n", userData.m_userName);
   managerResult = UserManager_RegisterUser(_app->m_userManager, userData.m_userName, userData.m_password);
   switch (managerResult)
   {
   case USER_MANAGER_SUCCESS:
       printf("User registered successfully\n");
       status = 1;
       break;
   case USER_MANAGER_USER_ALREADY_EXISTS:
       printf("Registration failed: user already exists\n");
       status = 0;
       break;
   case USER_MANAGER_ALLOCATION_ERROR:
       printf("Registration failed: server memory error\n");
       status = 0;
       break;
   default:
       printf("Registration failed: unknown error\n");
       status = 0;
       break;
   }
   responseSize = PackRegRes(status, responseBuffer);
   if (responseSize > 0)
   {
       if (ServerSendMessage(_clientSocket, PROTOCOL_REGISTRATION_RES, responseBuffer, responseSize) < 0)
       {
           printf("Failed to send response to client\n");
       }
       else
       {
           printf("Response sent successfully to client\n");
       }
   }
   else
   {
       printf("Failed to pack response message\n");
   }
   free(userData.m_userName);
   free(userData.m_password);
}

static void HandleLoginRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
   UserNamePass userData;
   char responseBuffer[BUFFER_SIZE];
   char status;
   size_t responseSize;
   UserManagerResult managerResult;
   userData.m_userName = malloc(MAX_USERNAME_SIZE);
   userData.m_password = malloc(MAX_PASSWORD_SIZE);
   if (!userData.m_userName || !userData.m_password)
   {
       printf("Memory allocation failed.\n");
       free(userData.m_userName);
       free(userData.m_password);
       SendErrorResponse(_clientSocket);
       return;
   }
   ProtocolResult result = UnPackLoginReq((char*)_buffer, _length, &userData);
   if (result != PROTOCOL_SUCCESS)
   {
       printf("Invalid login request.\n");
       free(userData.m_userName);
       free(userData.m_password);
       SendErrorResponse(_clientSocket);
       return;
   }
   printf("Login request: Username: %s\n", userData.m_userName);
   managerResult = UserManager_LoginUser(_app->m_userManager, userData.m_userName, userData.m_password);
   switch (managerResult)
   {
   case USER_MANAGER_SUCCESS:
       printf("User logged in successfully\n");
       status = 1;
       break;
   case USER_MANAGER_USER_NOT_FOUND:
       printf("Login failed: user not found\n");
       status = 0;
       break;
   case USER_MANAGER_WRONG_PASSWORD:
       printf("Login failed: wrong password\n");
       status = 0;
       break;
   default:
       printf("Login failed: unknown error\n");
       status = 0;
       break;
   }
   responseSize = PackLoginRes(status, responseBuffer);
   if (responseSize > 0)
   {
       if (ServerSendMessage(_clientSocket, PROTOCOL_LOGIN_RES, responseBuffer, responseSize) < 0)
       {
           printf("Failed to send response to client\n");
       }
       else
       {
           printf("Response sent successfully to client\n");
       }
   }
   else
   {
       printf("Failed to pack response message\n");
   }
   free(userData.m_userName);
   free(userData.m_password);
}

static void HandleCreateGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
    GroupRequest groupData;
    char responseBuffer[BUFFER_SIZE];
    char status = 0; 
    size_t responseSize;
    GroupManagerResult managerResult;
    groupData.m_userName = malloc(MAX_USERNAME_SIZE);
    groupData.m_groupName = malloc(MAX_GROUP_NAME_SIZE);
    if (!groupData.m_userName || !groupData.m_groupName)
    {
        printf("Memory allocation failed.\n");
        free(groupData.m_userName);
        free(groupData.m_groupName);
        SendErrorResponse(_clientSocket);
        return;
    }
    ProtocolResult result = UnPackCreateGroupReq((char*)_buffer, _length, &groupData);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid create group request.\n");
        free(groupData.m_userName);
        free(groupData.m_groupName);
        SendErrorResponse(_clientSocket);
        return;
    }    
    printf("Create group request: User: %s, Group: %s\n", 
           groupData.m_userName, groupData.m_groupName);    
    if (UserManager_IsUserLoggedIn(_app->m_userManager, groupData.m_userName))
    {
        managerResult = GroupManager_CreateGroup(_app->m_groupManager, groupData.m_groupName);      
        if (managerResult == GROUP_MANAGER_SUCCESS)
        {
            printf("Group created successfully\n");
            char* groupAddress = GroupManager_GetGroupAddress(_app->m_groupManager, groupData.m_groupName);
            if (groupAddress != NULL)
            {
                printf("Group IP address: %s\n", groupAddress);
                responseSize = PackCreateGroupResWithAddress(1, groupAddress, responseBuffer);
                status = 1;
                
                managerResult = GroupManager_JoinGroup(_app->m_groupManager, groupData.m_groupName);
                if (managerResult != GROUP_MANAGER_SUCCESS)
                {
                    printf("Warning: Creator failed to join group\n");
                }
            }
            else
            {
                printf("Failed to get group address\n");
                responseSize = PackCreateGroupResWithAddress(0, NULL, responseBuffer);
            }
        }
        else
        {
            switch(managerResult)
            {
                case GROUP_MANAGER_GROUP_ALREADY_EXISTS:
                    printf("Create failed: group name already exists\n");
                    break;

                case GROUP_MANAGER_ALLOCATION_FAILED:
                    printf("Create failed: server memory error\n");
                    break;

                case GROUP_MANAGER_INVALID_GROUP_NAME:
                    printf("Create failed: invalid group name\n");
                    break;

                case GROUP_MANAGER_MC_ADDRESSES_OVERFLOW:
                    printf("Create failed: no available multicast addresses\n");
                    break;

                case GROUP_MANAGER_ADDRESS_RECEIVE_FAILED:
                    printf("Create failed: failed to get multicast address\n");
                    break;

                default:
                    printf("Create failed: unknown error\n");
                    break;
            }
            responseSize = PackCreateGroupResWithAddress(0, NULL, responseBuffer);
        }
    }
    else
    {
        printf("Create group failed: user not logged in\n");
        responseSize = PackCreateGroupResWithAddress(0, NULL, responseBuffer);
    }  
    if (responseSize > 0)
    {
        if (ServerSendMessage(_clientSocket, PROTOCOL_CREATE_GROUP_RES, 
                            responseBuffer, responseSize) < 0)
        {
            printf("Failed to send response to client\n");
        }
        else
        {
            printf("Response sent successfully to client\n");
        }
    }
    free(groupData.m_userName);
    free(groupData.m_groupName);
}

static void HandleGetGroupsRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
    char userName[MAX_USERNAME_SIZE];
    char responseBuffer[BUFFER_SIZE];
    char groupsListBuffer[BUFFER_SIZE];
    size_t responseSize;
    
    ProtocolResult result = UnPackGetGroupsReq((char*)_buffer, _length, userName);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid get groups request.\n");
        SendErrorResponse(_clientSocket);
        return;
    }
    
    printf("Get groups request from user: %s\n", userName);
    
    if (UserManager_IsUserLoggedIn(_app->m_userManager, userName))
    {
        GroupManagerResult gmResult = GroupManager_GetActiveGroups(_app->m_groupManager, groupsListBuffer, BUFFER_SIZE);
        if (gmResult == GROUP_MANAGER_SUCCESS)
        {
            printf("Groups list generated successfully\n");
            responseSize = PackGetGroupsRes(1, groupsListBuffer, responseBuffer);
        }
        else
        {
            printf("Failed to generate groups list\n");
            responseSize = PackGetGroupsRes(0, NULL, responseBuffer);
        }
    }
    else
    {
        printf("Get groups failed: user not logged in\n");
        responseSize = PackGetGroupsRes(0, NULL, responseBuffer);
    }
    
    if (responseSize > 0)
    {
        if (ServerSendMessage(_clientSocket, PROTOCOL_GET_GROUPS_RES, 
                            responseBuffer, responseSize) < 0)
        {
            printf("Failed to send response to client\n");
        }
        else
        {
            printf("Response sent successfully to client\n");
        }
    }
}

static void HandleJoinGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
    GroupRequest groupData;
    char responseBuffer[BUFFER_SIZE];
    char status = 0;  
    size_t responseSize;
    GroupManagerResult managerResult;
    groupData.m_userName = malloc(MAX_USERNAME_SIZE);
    groupData.m_groupName = malloc(MAX_GROUP_NAME_SIZE);
    if (!groupData.m_userName || !groupData.m_groupName)
    {
        printf("Memory allocation failed.\n");
        free(groupData.m_userName);
        free(groupData.m_groupName);
        SendErrorResponse(_clientSocket);
        return;
    }  
    ProtocolResult result = UnPackJoinGroupReq((char*)_buffer, _length, &groupData);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid join group request.\n");
        free(groupData.m_userName);
        free(groupData.m_groupName);
        SendErrorResponse(_clientSocket);
        return;
    }   
    printf("Join group request: User: %s, Group: %s\n", 
           groupData.m_userName, groupData.m_groupName);   
    if (UserManager_IsUserLoggedIn(_app->m_userManager, groupData.m_userName))
    {
        managerResult = GroupManager_JoinGroup(_app->m_groupManager, groupData.m_groupName);      
        switch(managerResult)
        {
            case GROUP_MANAGER_SUCCESS:
                printf("User joined group successfully\n");
                char* groupAddress = GroupManager_GetGroupAddress(_app->m_groupManager, groupData.m_groupName);
                if (groupAddress != NULL)
                {
                    printf("Group IP address: %s\n", groupAddress);
                    responseSize = PackJoinGroupResWithAddress(1, groupAddress, responseBuffer);
                    status = 1;
                }
                else
                {
                    printf("Failed to get group address\n");
                    responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
                }
                break;

            case GROUP_MANAGER_GROUP_NOT_FOUND:
                printf("Join failed: group not found\n");
                responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
                break;

            case GROUP_MANAGER_INC_MEMBERS_ERROR:
                printf("Join failed: error incrementing group members\n");
                responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
                break;

            case GROUP_MANAGER_UNINITIALIZED_ERROR:
                printf("Join failed: uninitialized error\n");
                responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
                break;

            default:
                printf("Join failed: unknown error\n");
                responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
                break;
        }
    }
    else
    {
        printf("Join group failed: user not logged in\n");
        responseSize = PackJoinGroupResWithAddress(0, NULL, responseBuffer);
    }    
    if (responseSize > 0)
    {
        if (ServerSendMessage(_clientSocket, PROTOCOL_JOIN_GROUP_RES, 
                            responseBuffer, responseSize) < 0)
        {
            printf("Failed to send response to client\n");
        }
        else
        {
            printf("Response sent successfully to client\n");
        }
    }
    free(groupData.m_userName);
    free(groupData.m_groupName);
}

static void HandleLeaveGroupRequest(ServerApp* _app, int _clientSocket, const char* _buffer, size_t _length)
{
  GroupRequest groupData;
  char responseBuffer[BUFFER_SIZE];
  char status = 0;  
  size_t responseSize;
  GroupManagerResult managerResult;
  groupData.m_userName = malloc(MAX_USERNAME_SIZE);
  groupData.m_groupName = malloc(MAX_GROUP_NAME_SIZE);
  if (!groupData.m_userName || !groupData.m_groupName)
  {
      printf("Memory allocation failed.\n");
      free(groupData.m_userName);
      free(groupData.m_groupName);
      SendErrorResponse(_clientSocket);
      return;
  } 
  ProtocolResult result = UnPackLeaveGroupReq((char*)_buffer, _length, &groupData);
  if (result != PROTOCOL_SUCCESS)
  {
      printf("Invalid leave group request.\n");
      free(groupData.m_userName);
      free(groupData.m_groupName);
      SendErrorResponse(_clientSocket);
      return;
  }
  printf("Leave group request: User: %s, Group: %s\n", 
         groupData.m_userName, groupData.m_groupName);  
  if (UserManager_IsUserLoggedIn(_app->m_userManager, groupData.m_userName))
  {
      managerResult = GroupManager_LeaveGroup(_app->m_groupManager, groupData.m_groupName);
      switch(managerResult)
      {
          case GROUP_MANAGER_SUCCESS:
              printf("User left group successfully\n");
              status = 1;
              break;

          case GROUP_MANAGER_GROUP_NOT_FOUND:
              printf("Leave failed: group not found\n");
              break;

          case GROUP_MANAGER_UNINITIALIZED_ERROR:
              printf("Leave failed: uninitialized error\n");
              break;

          case GROUP_MANAGER_ADDRESS_ADD_FAILED:
              printf("Leave failed: error returning multicast address\n");
              break;

          default:
              printf("Leave failed: unknown error\n");
              break;
      }
  }
  else
  {
      printf("Leave group failed: user not logged in\n");
  }
  responseSize = PackLeaveGroupRes(status, responseBuffer);
  if (responseSize > 0)
  {
      if (ServerSendMessage(_clientSocket, PROTOCOL_LEAVE_GROUP_RES, 
                          responseBuffer, responseSize) < 0)
      {
          printf("Failed to send response to client\n");
      }
      else
      {
          printf("Response sent successfully to client\n");
      }
  }
  free(groupData.m_userName);
  free(groupData.m_groupName);
}
