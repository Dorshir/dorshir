#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mqueue.h>
#define QUEUE "/queue1"
#define MAX_MSG_COUNT 10
#define MAX_MSG_SIZE 4096

static void SendPid(pid_t _pid);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Multicast IP> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();
    
    SendPid(pid);
    
    const char *multicast_ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in addr;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set TTL for multicast messages (optional)
    unsigned char ttl = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt(IP_MULTICAST_TTL)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, multicast_ip, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Type messages to send. Press CTRL+C to quit.\n");

    char buf[1024];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        size_t len = strlen(buf);
        if (buf[len-1] == '\n') {
            buf[len-1] = '\0'; // remove newline
        }

        if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("sendto");
        }
    }

    close(sockfd);
    return 0;
}

static void SendPid(pid_t _pid)
{
    mqd_t mq;
    char bufferPid[MAX_MSG_SIZE];
    struct mq_attr attr;
    
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_COUNT;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE, O_RDWR | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1) 
    {
        perror("Failed to create/open queue1");
        exit(1);
    }

        sprintf(bufferPid,"%d",_pid);
        mq_send(mq, bufferPid, strlen(bufferPid) + 1, 0);
}
