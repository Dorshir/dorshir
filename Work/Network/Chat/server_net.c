#include "server_net.h"
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>
#include "limits.h"
#define MAX_CLIENTS 100


struct ServerNet
{
    int m_serverSocket;
    ClientHandler m_handler;
    void *m_context;
    int m_clientSockets[MAX_CLIENTS];
};

static volatile int g_keepRunning = 1;
static int clientCounter = 1;  
static int clientMap[MAX_CLIENTS] = {0}; 

/* Static Function Prototypes */
static int SetNonBlocking(int socket);
static void HandleClientMessage(ServerNet *server, int clientSocket);
static void CloseClientSocket(ServerNet *server, int clientIndex);
static void AddClientSocket(ServerNet *server, int clientSocket);
static void RemoveClientSocket(ServerNet *server, int clientSocket);
static void SignalHandler(int signum);

ServerNet* ServerNetCreate(int port, ClientHandler handler, void *context)
{
    if (!handler)
    {
        return NULL;
    }

    ServerNet *server = malloc(sizeof(ServerNet));
    if (!server)
    {
        return NULL;
    }

    server->m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->m_serverSocket < 0)
    {
        free(server);
        return NULL;
    }

    if (!SetNonBlocking(server->m_serverSocket))
    {
        close(server->m_serverSocket);
        free(server);
        return NULL;
    }

    struct sockaddr_in serverAddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };

    if (bind(server->m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        close(server->m_serverSocket);
        free(server);
        return NULL;
    }

    if (listen(server->m_serverSocket, MAX_CLIENTS) < 0)
    {
        close(server->m_serverSocket);
        free(server);
        return NULL;
    }

    int optval = 1;
    if (setsockopt(server->m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt");
        close(server->m_serverSocket);
        free(server);
        return NULL;
    }
    server->m_handler = handler;
    server->m_context = context;
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        server->m_clientSockets[i] = -1; 
    }

    printf("Server created and listening on port %d\n", port);

    return server;
}

void ServerNetRun(ServerNet *server)
{
    if (!server)
    {
        return;
    }
    struct sigaction sa = {0};
    sa.sa_handler = SignalHandler;
    sigaction(SIGINT, &sa, NULL);
    printf("Server running...\n");
    fd_set readFds;
    int maxSd;
    while (g_keepRunning)
    {
        FD_ZERO(&readFds);
        FD_SET(server->m_serverSocket, &readFds);
        maxSd = server->m_serverSocket;

        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if (server->m_clientSockets[i] >= 0)
            {
                FD_SET(server->m_clientSockets[i], &readFds);
                if (server->m_clientSockets[i] > maxSd)
                {
                    maxSd = server->m_clientSockets[i];
                }
            }
        }
        int activity = select(maxSd + 1, &readFds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR)
        {
            perror("Select error");
            break;
        }
        if (activity == 0)
        {
            continue;
        }

        if (FD_ISSET(server->m_serverSocket, &readFds))
        {
            int newSocket = accept(server->m_serverSocket, NULL, NULL);
            if (newSocket < 0)
            {
                continue;
            }
            AddClientSocket(server, newSocket);
        }
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            int clientSocket = server->m_clientSockets[i];
            if (clientSocket >= 0 && FD_ISSET(clientSocket, &readFds))
            {
                HandleClientMessage(server, clientSocket);
            }
        }
    }
}

int ServerSendMessage(int clientSocket, ProtocolTag tag, const char *payload, size_t payloadSize)
{
    if (clientSocket < 0 || payload == NULL || payloadSize == 0)
    {
        return -1;
    }

    ssize_t bytesSent = send(clientSocket, payload, payloadSize, 0);
    if (bytesSent < 0)
    {
        perror("ServerSendMessage: Failed to send");
        return -1;
    }

    printf("Sent %zd bytes to client\n", bytesSent);
    return bytesSent;
}


void ServerNetDestroy(ServerNet **server)
{
    if (!server || !*server)
    {
        return;
    }

    close((*server)->m_serverSocket);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if ((*server)->m_clientSockets[i] >= 0)
        {
            close((*server)->m_clientSockets[i]);
        }
    }

    free(*server);
    *server = NULL;

    printf("\nReceived SIGINT (Ctrl+C). Shutting down server...\n");
}

/* Static Helper Functions */

static int SetNonBlocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0)
    {
        perror("SetNonBlocking: Failed to get flags");
        return 0;
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("SetNonBlocking: Failed to set non-blocking mode");
        return 0;
    }

    return 1;
}

static void HandleClientMessage(ServerNet *server, int clientSocket)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead <= 0)
    {
        if (bytesRead < 0)
        {
            perror("HandleClientMessage: Failed to read from client");
        }
        int clientNum = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) 
        {
            if (server->m_clientSockets[i] == clientSocket) 
            {
                clientNum = clientMap[i];
                break;
            }
        }
        printf("Client #%d disconnected (FD: %d)\n", clientNum, clientSocket);
        RemoveClientSocket(server, clientSocket);
        return;
    }
    buffer[bytesRead] = '\0';
    ProtocolTag tag = GetTag(buffer);
    if (tag == PROTOCOL_INVALID_TAG)
    {
        printf("Invalid protocol tag received.\n");
        return;
    }

    if (server->m_handler)
    {
        server->m_handler(clientSocket, tag, buffer, (size_t)bytesRead, server->m_context);
    }
}

static void CloseClientSocket(ServerNet *server, int clientIndex)
{
    if (server->m_clientSockets[clientIndex] >= 0)
    {
        close(server->m_clientSockets[clientIndex]);
        server->m_clientSockets[clientIndex] = -1;
    }
}

static void AddClientSocket(ServerNet *server, int clientSocket)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (server->m_clientSockets[i] == -1)
        {
            server->m_clientSockets[i] = clientSocket;
            clientMap[i] = clientCounter;
            printf("New client #%d connected (FD: %d)\n", clientCounter++, clientSocket);
            return;
        }
    }
    printf("Too many clients connected. Closing new connection: %d\n", clientSocket);
    close(clientSocket);
}

static void RemoveClientSocket(ServerNet *server, int clientSocket)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (server->m_clientSockets[i] == clientSocket)
        {
            CloseClientSocket(server, i);
            break;
        }
    }
}

static void SignalHandler(int signum) 
{
    if (signum == SIGINT) 
    {
        g_keepRunning = 0;
    }
}
