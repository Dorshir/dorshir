#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_PORT 5588
#define SERVER_ADDR "127.0.0.1"

#define BUFFER_SIZE 4096
#define MSG_SIZE 50
#define TOTAL_NUM_OF_CLIENTS 1000
#define TRUE 1

#define CONNECT_PERC 15
#define DISCONNECT_PERC 5
#define TRANS_PERC 15

#define DISCONNECTED(clientFd) (clientFd == -1)
#define INDEX_INC(index) (index = (index + 1) % TOTAL_NUM_OF_CLIENTS)

static void TransferData(int *_clients, size_t _index)
{
    char buffer[BUFFER_SIZE] = {0};
    char msg[MSG_SIZE];
    sprintf(msg, "Hello from client: %d\n", _clients[_index]);

    int sentBytes = send(_clients[_index], msg, strlen(msg) + 1, 0);
    if (sentBytes < 0)
    {
        perror("send failed");
        close(_clients[_index]);
        _clients[_index] = -1;
    }

    int readBytes = recv(_clients[_index], buffer, BUFFER_SIZE, 0);
    if (readBytes == 0)
    {
        perror("recv failed");
        close(_clients[_index]);
        _clients[_index] = -1;
    }
    else if (readBytes < 0)
    {
        perror("recv failed");
        close(_clients[_index]);
        _clients[_index] = -1;
    }
    else
    {
        printf("Received message: %s\n", buffer);
    }
}

static void DisconnectClient(int *_clients, size_t _index)
{
    close(_clients[_index]);
    _clients[_index] = -1;
}

static void ConnectClient(int *_clients, size_t _index)
{
    struct sockaddr_in serverAddr = {0};

    _clients[_index] = socket(AF_INET, SOCK_STREAM, 0);
    if (_clients[_index] < 0)
    {
        perror("socket failed");
        return;
    }

    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(_clients[_index], (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect failed");
        close(_clients[_index]);
        _clients[_index] = -1;
    }
}

int main()
{
    int clients[TOTAL_NUM_OF_CLIENTS];
    char buffer[BUFFER_SIZE];
    int sockFd, perc, currClient;
    size_t index = 0;

    for (size_t i = 0; i < TOTAL_NUM_OF_CLIENTS; ++i)
    {
        clients[i] = -1;
    }

    while (TRUE)
    {
        perc = rand() % 100 + 1;
        currClient = clients[index];

        if (DISCONNECTED(currClient))
        {
            if (perc <= CONNECT_PERC)
            {
                ConnectClient(clients, index);
            }
        }
        else
        {
            if (perc <= DISCONNECT_PERC)
            {
                DisconnectClient(clients, index);
                continue;
            }
            if (perc <= TRANS_PERC)
            {
                TransferData(clients, index);
            }
        }
        INDEX_INC(index);
        }

    return 0;
}