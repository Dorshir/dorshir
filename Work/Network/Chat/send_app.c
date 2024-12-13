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

    SendPid(argv[3]);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, multicast_ip, &addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Type messages to send. Press CTRL+C to quit.\n");

    char buf[BUFFER_SIZE];
    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        size_t len = strlen(buf);
        if (buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0'; // remove newline
        }

        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("sendto");
        }
    }

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
