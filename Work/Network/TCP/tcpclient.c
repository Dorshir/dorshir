#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 5588
#define SERVER_ADDR "127.0.0.1"

#define MSG_SIZE 50

int main()
{
    struct sockaddr_in serverAddr = {0};
    char msg[MSG_SIZE] = "Hello from client";

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        perror("socket failed");
        return -1;
    }

    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect failed");
        close(sockFd);
        return -2;
    }

    int sentBytes = send(sockFd, msg, strlen(msg) + 1, 0);
    if (sentBytes < 0)
    {
        perror("sendto failed");
        close(sockFd);
        return -3;
    }

    close(sockFd);
    return 0;
}