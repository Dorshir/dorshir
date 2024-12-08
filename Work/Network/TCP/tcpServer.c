#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> /* sprintf, printf */
#include <unistd.h>
#include <stdlib.h>       /* malloc, free */
#include <fcntl.h>        /* fcntl */
#include <errno.h>        /* errno */
#include <string.h>       /* strlen */
#include <sys/resource.h> /* getrlimit */

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
static void CloseConnectedClient(int _sockFd, int *_fdCount);

int InitServer(int *_sockFd, List **_activeClients, int *_fdCount, int *_fdCountTreshold)
{
    struct sockaddr_in serverAddr = {0};
    int optval = 1;
    struct rlimit limit;

    if (_sockFd == NULL || _activeClients == NULL || _fdCount == NULL || _fdCountTreshold == NULL)
    {
        return -1;
    }

    if (getrlimit(RLIMIT_NOFILE, &limit) == 0)
    {
        *_fdCountTreshold = limit.rlim_cur - 3;
    }
    else
    {
        return -2;
    }

    *_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (*_sockFd < 0)
    {
        perror("socket failed");
        return -3;
    }

    if (setsockopt(*_sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt failed");
        close(*_sockFd);
        return -4;
    }

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(*_sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind failed");
        close(*_sockFd);
        return -5;
    }

    if (listen(*_sockFd, CONN_WAIT_QUEUE_SIZE) < 0)
    {
        perror("listen failed");
        close(*_sockFd);
        return -6;
    }

    if (SetSockNonBlocking(_sockFd) < 0)
    {
        close(*_sockFd);
        return -7;
    }

    *_activeClients = ListCreate();
    if (*_activeClients == NULL)
    {
        close(*_sockFd);
        return -8;
    }

    *_fdCount = 4;

    return 1;
}

void CheckForNewClient(int _sockFd, List *_activeClients, int *_fdCount, int _fdCountTreshold)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLength = sizeof(clientAddr);
    int flags, *connFd, tempFd;

    if (_activeClients == NULL || _fdCount == NULL)
    {
        return;
    }

    tempFd = accept(_sockFd, (struct sockaddr *)&clientAddr, &addrLength);
    if (tempFd < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept failed");
        }
        return;
    }

    if (*_fdCount == _fdCountTreshold)
    {
        printf("Reached fd count treshold. Closing the new connection.\n");
        close(tempFd);
        return;
    }

    ++(*_fdCount);

    connFd = malloc(sizeof(int));
    if (connFd == NULL)
    {
        CloseConnectedClient(tempFd, _fdCount);
        return;
    }

    *connFd = tempFd;

    if (SetSockNonBlocking(connFd) < 0)
    {
        CloseConnectedClient(*connFd, _fdCount);
        return;
    }

    if (ListPushTail(_activeClients, connFd) == NULL)
    {
        CloseConnectedClient(*connFd, _fdCount);
        free(connFd);
    }
}

void CheckForEvents(List *_activeClients, int *_fdCount)
{
    if (_activeClients == NULL || _fdCount == NULL)
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
            CloseConnectedClient(*fd, _fdCount);
            free(fd);
        }
    }
}

void DestroyServer(int _sockFd, List **_activeClients)
{
    if (_activeClients == NULL || *_activeClients == NULL)
    {
        return;
    }
    ListDestroy(_activeClients, DestroyClient);
    close(_sockFd);
}

int main()
{
    int sockFd, status, fdCount = 0, fdCountTreshold;
    List *activeClients;

    status = InitServer(&sockFd, &activeClients, &fdCount, &fdCountTreshold);
    if (status < 0)
    {
        return status;
    }

    while (TRUE)
    {
        CheckForNewClient(sockFd, activeClients, &fdCount, fdCountTreshold);
        CheckForEvents(activeClients, &fdCount);
    }

    DestroyServer(sockFd, &activeClients);

    return 0;
}

static void CloseConnectedClient(int _sockFd, int *_fdCount)
{
    close(_sockFd);
    --(*_fdCount);
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

    return 1;
}

static int RecFromClient(void *_fd, void *_dummy)
{
    char buffer[BUFFER_SIZE];
    int clientFd = *((int *)(_fd));

    int readBytes = recv(clientFd, buffer, BUFFER_SIZE, 0);
    if (readBytes == 0)
    {
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
        return 0;
    }
    else if (sentBytes < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("send failed");
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