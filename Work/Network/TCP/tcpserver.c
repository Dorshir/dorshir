#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "new_gen_dlist.h"

#define SERVER_PORT 5588
#define BUFFER_SIZE 4096
#define CONN_WAIT_QUEUE_SIZE 32
#define TRUE 1

static int RecFromClient(void *_fd, void *_dummy);
static void DestroyClient(void *_fd);

void InitServer(int *_sockFd, List **_activeClients)
{
    struct sockaddr_in serverAddr = {0};
    int optval = 1;

    if (_sockFd == NULL)
    {
        return;
    }

    *_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (*_sockFd < 0)
    {
        perror("socket failed");
        return;
    }

    if (setsockopt(*_sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt failed");
        close(*_sockFd);
        return;
    }

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(*_sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind failed");
        close(*_sockFd);
        return;
    }

    if (listen(*_sockFd, CONN_WAIT_QUEUE_SIZE) < 0)
    {
        perror("listen failed");
        close(*_sockFd);
        return;
    }

    *_activeClients = ListCreate();
    if (*_activeClients == NULL)
    {
        close(*_sockFd);
        return;
    }
}

void CheckForNewClient(int _sockFd, List *_activeClients)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLength = sizeof(clientAddr);
    int *connFd = malloc(sizeof(int));
    if (connFd == NULL)
    {
        return;
    }

    *connFd = accept(_sockFd, (struct sockaddr *)&clientAddr, &addrLength);
    if (*connFd < 0)
    {
        perror("accept failed");
        free(connFd);
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

    ListItr itr = ListItrForEach(begin, end, RecFromClient, NULL);
    if (itr != end)
    {
        int *fd = ListItrRemove(itr);
        free(fd);
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
    int sockFd;
    List *activeClients;

    InitServer(&sockFd, &activeClients);

    while (TRUE)
    {
        CheckForNewClient(sockFd, activeClients);
        CheckForEvents(activeClients);
    }

    DestroyServer(sockFd, &activeClients);

    return 0;
}

static int RecFromClient(void *_fd, void *_dummy)
{
    char buffer[BUFFER_SIZE];
    int clientFd = *((int *)(_fd));


    int readBytes = recv(clientFd, buffer, BUFFER_SIZE, 0);
    if (readBytes <= 0)
    {
        perror("recv failed");
        close(clientFd);
        return 0;
    }
    else
    {
        printf("Received message: %s\n", buffer);
        return 1;
    }
}

static void DestroyClient(void *_fd)
{
    int clientFd = *((int *)(_fd));
    close(clientFd);
    free(_fd);
}