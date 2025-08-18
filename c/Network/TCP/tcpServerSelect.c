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
#include <sys/select.h>

#include "new_gen_dlist.h"

#define SERVER_PORT 5588
#define BUFFER_SIZE 4096
#define CONN_WAIT_QUEUE_SIZE 1000
#define TRUE 1
#define FAIL -1
#define MSG_SIZE 50
#define MAX_NUM_OF_FDS 1024
#define FD_TRESHOLD 1000

static int RecvFromClient(int _clientFd);
static void DestroyClient(void *_fd);
static int SendToClient(int _clientFd);
static void CloseConnectedClient(int _sockFd, int *_fdCount, fd_set *_masterFds);
static void SetMaxIOFDs(int _maxFDs);

int InitServer(int *_sockFd, List **_activeClients, int *_fdCount, fd_set *_masterFds)
{
    struct sockaddr_in serverAddr = {0};
    int optval = 1;

    if (_sockFd == NULL || _activeClients == NULL || _fdCount == NULL || _masterFds == NULL)
    {
        return -1;
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

    *_activeClients = ListCreate();
    if (*_activeClients == NULL)
    {
        close(*_sockFd);
        return -7;
    }

    FD_ZERO(_masterFds);
    FD_SET(*_sockFd, _masterFds);

    *_fdCount = 4;

    return 1;
}

void CheckForNewClient(int _sockFd, List *_activeClients, int *_fdCount, fd_set *_masterFds)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLength = sizeof(clientAddr);
    int *connFd, tempFd;

    if (_activeClients == NULL || _fdCount == NULL)
    {
        return;
    }

    tempFd = accept(_sockFd, (struct sockaddr *)&clientAddr, &addrLength);
    if (tempFd < 0)
    {
        perror("accept failed");
        return;
    }

    if (*_fdCount == FD_TRESHOLD)
    {
        close(tempFd);
        return;
    }
    ++(*_fdCount);

    connFd = malloc(sizeof(int));
    if (connFd == NULL)
    {
        CloseConnectedClient(tempFd, _fdCount, _masterFds);
        return;
    }

    *connFd = tempFd;

    if (ListPushTail(_activeClients, connFd) == NULL)
    {
        CloseConnectedClient(*connFd, _fdCount, _masterFds);
        free(connFd);
    }

    FD_SET(*connFd, _masterFds);
}

void CheckForTransferEvents(List *_activeClients, int *_fdCount, fd_set *_masterFds, fd_set *_tempFds)
{
    if (_activeClients == NULL || _fdCount == NULL || _masterFds == NULL || _tempFds == NULL)
    {
        return;
    }

    ListItr current = ListItrBegin(_activeClients);
    ListItr end = ListItrEnd(_activeClients);
    ListItr nextItr;

    while (current != end)
    {
        int *fdPtr = (int *)ListItrGet(current);

        nextItr = ListItrNext(current);

        if (FD_ISSET(*fdPtr, _tempFds))
        {
            if (RecvFromClient(*fdPtr) == 0)
            {
                int *removedFd = (int *)ListItrRemove(current);
                CloseConnectedClient(*removedFd, _fdCount, _masterFds);
                free(removedFd);
            }
            else
            {
                if (SendToClient(*fdPtr) == 0)
                {
                    int *removedFd = (int *)ListItrRemove(current);
                    CloseConnectedClient(*removedFd, _fdCount, _masterFds);
                    free(removedFd);
                }
            }

            break;
        }

        current = nextItr;
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
    int sockFd, status, activity, listenFdFlag, fdCount = 0;
    List *activeClients;
    fd_set masterFds, tempFds;

    SetMaxIOFDs(MAX_NUM_OF_FDS);

    status = InitServer(&sockFd, &activeClients, &fdCount, &masterFds);
    if (status < 0)
    {
        return status;
    }

    while (TRUE)
    {
        listenFdFlag = 0;
        tempFds = masterFds;
        activity = select(MAX_NUM_OF_FDS, &tempFds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select");
            continue;
        }

        if (FD_ISSET(sockFd, &tempFds))
        {
            CheckForNewClient(sockFd, activeClients, &fdCount, &masterFds);
            listenFdFlag = 1;
        }

        if (activity == 1 && listenFdFlag == 1)
        {
            continue;
        }
        CheckForTransferEvents(activeClients, &fdCount, &masterFds, &tempFds);
    }

    DestroyServer(sockFd, &activeClients);

    return 0;
}

static void CloseConnectedClient(int _sockFd, int *_fdCount, fd_set *_masterFds)
{
    close(_sockFd);
    --(*_fdCount);
    FD_CLR(_sockFd, _masterFds);
}

static int RecvFromClient(int _clientFd)
{
    char buffer[BUFFER_SIZE];

    int readBytes = recv(_clientFd, buffer, BUFFER_SIZE, 0);
    if (readBytes <= 0)
    {
        return 0;
    }
    printf("Received message: %s\n", buffer);

    return 1;
}

static int SendToClient(int _clientFd)
{
    char msg[MSG_SIZE] = "Hello from server!";

    int sentBytes = send(_clientFd, msg, strlen(msg) + 1, 0);
    if (sentBytes <= 0)
    {
        return 0;
    }

    return 1;
}

static void DestroyClient(void *_fd)
{
    int clientFd = *((int *)(_fd));
    close(clientFd);
    free(_fd);
}

static void SetMaxIOFDs(int _maxFDs)
{
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) != 0)
    {
        perror("Failed to get RLIMIT_NOFILE");
        return;
    }

    limit.rlim_cur = _maxFDs;

    if (setrlimit(RLIMIT_NOFILE, &limit) != 0)
    {
        perror("Failed to set RLIMIT_NOFILE");
        return;
    }
}