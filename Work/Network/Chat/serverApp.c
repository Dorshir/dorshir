#include "serverApp.h"
#include "server_net.h"
#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERNAME_SIZE 256
#define MAX_PASSWORD_SIZE 256

static void HandleRegistrationRequest(int _clientSocket, const char *_buffer, size_t _length);
static void SendErrorResponse(int _clientSocket);

void ServerAppHandler(int _clientSocket, ProtocolTag _tag, char *_buffer, size_t _length, void *_context)
{
    switch (_tag)
    {
        case PROTOCOL_REGISTRATION_REQ:
            HandleRegistrationRequest(_clientSocket, _buffer, _length);
            break;

        default:
            printf("Unknown protocol tag received.\n");
            SendErrorResponse(_clientSocket);
            break;
    }
}

static void HandleRegistrationRequest(int clientSocket, const char *buffer, size_t length)
{
    UserNamePass userData;
    char responseBuffer[BUFFER_SIZE];
    size_t responseSize;

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

    ProtocolResult result = UnPackRegReq((char*)buffer, length, &userData);
    if (result != PROTOCOL_SUCCESS)
    {
        printf("Invalid registration request.\n");
        free(userData.m_userName);
        free(userData.m_password);
        SendErrorResponse(clientSocket);
        return;
    }

    printf("Received registration request: Username: %s, Password: %s\n", 
           userData.m_userName, userData.m_password);

    
    responseSize = PackRegRes(1, responseBuffer);
    
   
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

