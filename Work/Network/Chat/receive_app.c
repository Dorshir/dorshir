#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mqueue.h>
#define QUEUE "/queue2"
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
    struct ip_mreq mreq;
    char buf[1024];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow multiple sockets to use the same port number
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Bind to the multicast port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(multicast_ip);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    mreq.imr_multiaddr.s_addr = inet_addr(multicast_ip);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0) {
        perror("setsockopt(IP_ADD_MEMBERSHIP)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for messages on %s:%d...\n", multicast_ip, port);

    // Receive loop
    while (1) {
        ssize_t n = recv(sockfd, buf, sizeof(buf)-1, 0);
        if (n < 0) {
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
