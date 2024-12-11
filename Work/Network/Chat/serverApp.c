#include "serverApp.h"
#include "server_net.h"
#include "protocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "userManager.h"

#define MAX_USERNAME_SIZE 256
#define MAX_PASSWORD_SIZE 256

struct ServerApp
{
    UserManager *m_userManager;
};

// helper functions declelertion:
static void SendErrorResponse(int clientSocket);
static void HandleRegistrationRequest(ServerApp *app, int clientSocket, const char *buffer, size_t length);
static void HandleLoginRequest(ServerApp *app, int clientSocket, const char *buffer, size_t length);

// Main functions

ServerApp *ServerApp_Create(UserManager *userMng)
{
    if (userMng == NULL)
    {
        return NULL;
    }
    ServerApp *app = (ServerApp *)malloc(sizeof(ServerApp));
    if (!app)
    {
        return NULL;
    }
    app->m_userManager = userMng;
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
void ServerAppHandler(int clientSocket, ProtocolTag tag, char *buffer, size_t length, void *context)
{
    ServerApp *app = (ServerApp *)context;
    if (!app)
    {
        printf("ServerAppHandler: NULL context\n");
        SendErrorResponse(clientSocket);
        return;
    }
    switch (tag)
    {
    case PROTOCOL_REGISTRATION_REQ:
        HandleRegistrationRequest(app, clientSocket, buffer, length);
        break;
    case PROTOCOL_LOGIN_REQ:
        HandleLoginRequest(app, clientSocket, buffer, length);
        break;
    default:
        printf("Unknown protocol tag received: %d\n", tag);
        SendErrorResponse(clientSocket);
        break;
    }
}

// helper functions:

static void SendErrorResponse(int clientSocket)
{
    char responseBuffer[BUFFER_SIZE];
    size_t responseSize = PackRegRes(0, responseBuffer);

    if (responseSize > 0)
    {
        if (ServerSendMessage(clientSocket, PROTOCOL_REGISTRATION_RES, responseBuffer, responseSize) < 0)
        {
            printf("Failed to send error response to client\n");
        }
    }
}

static void HandleRegistrationRequest(ServerApp *app, int clientSocket, const char *buffer, size_t length)
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
        SendErrorResponse(clientSocket);
        return;
    }
    ProtocolResult result = UnPackRegReq((char *)buffer, length, &userData);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid registration request.\n");
        free(userData.m_userName);
        free(userData.m_password);
        SendErrorResponse(clientSocket);
        return;
    }
    printf("Registration request: Username: %s\n", userData.m_userName);
    managerResult = UserManager_RegisterUser(app->m_userManager, userData.m_userName, userData.m_password);
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
        if (ServerSendMessage(clientSocket, PROTOCOL_REGISTRATION_RES, responseBuffer, responseSize) < 0)
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

static void HandleLoginRequest(ServerApp *app, int clientSocket, const char *buffer, size_t length)
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
        SendErrorResponse(clientSocket);
        return;
    }
    ProtocolResult result = UnPackLoginReq((char *)buffer, length, &userData);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid login request.\n");
        free(userData.m_userName);
        free(userData.m_password);
        SendErrorResponse(clientSocket);
        return;
    }
    printf("Login request: Username: %s\n", userData.m_userName);
    managerResult = UserManager_LoginUser(app->m_userManager, userData.m_userName, userData.m_password);
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
        if (ServerSendMessage(clientSocket, PROTOCOL_LOGIN_RES, responseBuffer, responseSize) < 0)
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
