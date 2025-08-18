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

    char msg[MSG_SIZE] = "Hello from client";

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);

    int sentBytes = sendto(sock, msg, strlen(msg) + 1, 0, (struct sockaddr *)&sin, sizeof(sin));
    if (sentBytes < 0)
    {
        perror("sendto failed");
        close(sock);
        return -2;
    }


    close(sock);
    return 0;
}