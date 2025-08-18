#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_PORT 5588
#define BUFFER_SIZE 4096

int main()
{

    char buffer[BUFFER_SIZE];

    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("bind failed");
        close(sock);
        return -2;
    }

    struct sockaddr_in rec;
    int recLength = sizeof(rec);
    int readBytes = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&rec, &recLength);
    if (readBytes < 0)
    {
        perror("recvfrom failed");
        close(sock);
        return -3;
    }

    buffer[readBytes] = '\0';

    printf("Received message: %s\n", buffer);

    close(sock);
    return 0;
}