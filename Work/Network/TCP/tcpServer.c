#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "new_gen_dlist.h"

#define SERVER_PORT 5588
#define BUFFER_SIZE 4096
#define CONN_WAIT_QUEUE_SIZE 1000
#define TRUE 1
#define FAIL -1
#define MSG_SIZE 50

static int RecFromClient(void *_fd, void *_dummy);
static void DestroyClient(void *_fd);
static int SendToClient(int _clientFd);
static int SetSockNonBlocking(int *_sockFd);

int InitServer(int *_sockFd, List **_activeClients)
{
    struct sockaddr_in serverAddr = {0};
    int optval = 1;

    if (_sockFd == NULL)
    {
        return -1;
    }

    *_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (*_sockFd < 0)
    {
        perror("socket failed");
        return -2;
    }

    if (setsockopt(*_sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt failed");
        close(*_sockFd);
        return -3;
    }

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(*_sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind failed");
        close(*_sockFd);
        return -4;
    }

    if (listen(*_sockFd, CONN_WAIT_QUEUE_SIZE) < 0)
    {
        perror("listen failed");
        close(*_sockFd);
        return -5;
    }

    if (SetSockNonBlocking(_sockFd) < 0)
    {
        close(*_sockFd);
        return -6;
    }

    *_activeClients = ListCreate();
    if (*_activeClients == NULL)
    {
        close(*_sockFd);
        return -7;
    }

    return 1;
}

void CheckForNewClient(int _sockFd, List *_activeClients)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLength = sizeof(clientAddr);
    int flags, *connFd, tempFd;

    tempFd = accept(_sockFd, (struct sockaddr *)&clientAddr, &addrLength);
    if (tempFd < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept failed");
        }
        return;
    }

    connFd = malloc(sizeof(int));
    if (connFd == NULL)
    {
        close(tempFd);
        return;
    }

    *connFd = tempFd;

    if (SetSockNonBlocking(connFd) < 0)
    {
        close(*connFd);
        return;
    }

    if (ListPushTail(_activeClients, connFd) == NULL)
    {
        free(connFd);
    }
}

void CheckForEvents(List *_activeClients)
{
    if (_activeClients == NULL)
    {
        return;
    }

    ListItr begin = ListItrBegin(_activeClients);
    ListItr end = ListItrEnd(_activeClients);

    ListItr itr = begin;
    ListItr nextItr = ListItrNext(itr);
    while (itr != end && nextItr != end)
    {
        nextItr = ListItrNext(itr);
        itr = ListItrForEach(nextItr, end, RecFromClient, NULL);
        if (itr != end)
        {
            int *fd = ListItrRemove(itr);
            free(fd);
        }
    }
}

void DestroyServer(int _sockFd, List **_activeClients)
{
    if (*_activeClients == NULL)
    {
        return;
    }
    ListDestroy(_activeClients, DestroyClient);
    close(_sockFd);
}

int main()
{
    int sockFd, status;
    List *activeClients;

    status = InitServer(&sockFd, &activeClients);
    if (status < 0)
    {
        return status;
    }

    while (TRUE)
    {
        CheckForNewClient(sockFd, activeClients);
        CheckForEvents(activeClients);
    }

    DestroyServer(sockFd, &activeClients);

    return 0;
}

static int SetSockNonBlocking(int *_sockFd)
{
    int flags;
    if (FAIL == (flags = fcntl(*_sockFd, F_GETFL)))
    {
        perror("error at fcntl. F_GETFL.");
        return -1;
    }
    if (FAIL == fcntl(*_sockFd, F_SETFL, flags | O_NONBLOCK))
    {
        perror("error at fcntl. F_SETFL.");
        return -1;
    }
    if (fcntl(*_sockFd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl failed");
        return -1;
    }

    return 1;
}

static int RecFromClient(void *_fd, void *_dummy)
{
    char buffer[BUFFER_SIZE];
    int clientFd = *((int *)(_fd));

    int readBytes = recv(clientFd, buffer, BUFFER_SIZE, 0);
    if (readBytes == 0)
    {
        close(clientFd);
        return 0;
    }
    else if (readBytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 1;
        }
        else
        {
            close(clientFd);
            return 0;
        }
    }
    else
    {
        printf("Received message: %s\n", buffer);
        return SendToClient(clientFd);
    }
}

static int SendToClient(int _clientFd)
{
    char msg[MSG_SIZE] = "Hello from server!";

    int sentBytes = send(_clientFd, msg, strlen(msg) + 1, 0);
    if (sentBytes == 0)
    {
        perror("send failed");
        close(_clientFd);
        return 0;
    }
    else if (sentBytes < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("send failed");
            close(_clientFd);
            return 0;
        }
    }
    return 1;
}

static void DestroyClient(void *_fd)
{
    int clientFd = *((int *)(_fd));
    close(clientFd);
    free(_fd);
}