#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mqueue.h>

#define MAX_MSG_COUNT 10
#define MAX_MSG_SIZE 4096
#define BUFFER_SIZE 1024

static void SendPid(char *_mqName);

int main(int argc, char *argv[])
{
    const char *multicast_ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    char buf[BUFFER_SIZE];


    SendPid(argv[3]);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow multiple sockets to use the same port number
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Bind to the multicast port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(multicast_ip);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    mreq.imr_multiaddr.s_addr = inet_addr(multicast_ip);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt(IP_ADD_MEMBERSHIP)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for messages on %s:%d...\n", multicast_ip, port);

    // Receive loop
    while (1)
    {
        ssize_t n = recv(sockfd, buf, sizeof(buf) - 1, 0);
        if (n < 0)
        {
            perror("recv");
            break;
        }
        buf[n] = '\0';
        printf("Received: %s\n", buf);
        fflush(stdout);
    }

    // Cleanup
    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    close(sockfd);
    return 0;
}

static void SendPid(char *_mqName)
{
    mqd_t mq;
    char bufferPid[MAX_MSG_SIZE];
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_COUNT;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    pid_t pid = getpid();

    mq = mq_open(_mqName, O_RDWR | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1)
    {
        perror("Failed to create/open queue1");
        exit(1);
    }

    sprintf(bufferPid, "%d", pid);
    mq_send(mq, bufferPid, strlen(bufferPid) + 1, 0);
}
