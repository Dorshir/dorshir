#include "clientnet.h"
#include <stdlib.h>     /* malloc, free, NULL */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> /* sockaddr_in */
#include <unistd.h>     /* close */
#include <stdio.h>      /* perror */
#include <arpa/inet.h>  /* inet_addr */

struct ClientNet
{
    int m_sockFd;
};

ClientNet *ClientNet_Create(char *_serverAddr, int _port)
{
    ClientNet *newClientNet;
    struct sockaddr_in serverAddr = {0};

    if (_serverAddr == NULL)
    {
        return NULL;
    }

    newClientNet = malloc(sizeof(ClientNet));
    if (newClientNet == NULL)
    {
        return NULL;
    }

    newClientNet->m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (newClientNet->m_sockFd < 0)
    {
        perror("ClientNet_Create: socket");
        free(newClientNet);
        return NULL;
    }

    serverAddr.sin_addr.s_addr = inet_addr(_serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);

    if (connect(newClientNet->m_sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("ClientNet_Create: connect");
        ClientNet_Destroy(&newClientNet);
        return NULL;
    }

    return newClientNet;
}

int ClientNet_Send(ClientNet *_clientNet, char *_message, size_t _length)
{
    int sentBytes;

    if (_clientNet == NULL || _message == NULL || _length == 0)
    {
        return -1;
    }

    sentBytes = send(_clientNet->m_sockFd, _message, _length, 0);
    if (sentBytes <= 0)
    {
        perror("ClientNet_Send: send");
        ClientNet_Destroy(&_clientNet);
        return -2;
    }

    return sentBytes;
}

int ClientNet_Receive(ClientNet *_clientNet, char *_buffer, size_t _bufferSize)
{
    int readBytes;

    if (_clientNet == NULL || _buffer == NULL || _bufferSize == 0)
    {
        return -1;
    }

    readBytes = recv(_clientNet->m_sockFd, _buffer, _bufferSize, 0);
    if (readBytes <= 0)
    {
        perror("ClientNet_Receive: recv");
        ClientNet_Destroy(&_clientNet);
        return -2;
    }

    return readBytes;
}

void ClientNet_Destroy(ClientNet **_clientnet)
{
    if (_clientnet == NULL || *_clientnet == NULL)
    {
        return;
    }

    if ((*_clientnet)->m_sockFd >= 0)
    {
        close((*_clientnet)->m_sockFd);
    }
    free(*_clientnet);
    *_clientnet = NULL;
}



int main()
{

    ClientNet* clientnet = ClientNet_Create("127.0.0.1", 5588);
    return 0;
}