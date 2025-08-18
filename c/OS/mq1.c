#include <stdio.h>    /* printf */
#include <string.h>   /* strcpy, strlen */
#include <fcntl.h>    /* popen, pclose */
#include <sys/stat.h> /* S_IRWXU */
#include <mqueue.h>   /* mq_attr, mqd_t, mq_open, mq_close, mq_unlink, mq_send, mq_receive */

#define QUEUE1_NAME "/queue1"
#define QUEUE2_NAME "/queue2"
#define MAX_MSG_SIZE 100
#define END_SIZE 5
#define PING_PONG_SIZE 5
#define MAX_MESSAGES 10

int main()
{
    mqd_t messageQueue1Id;
    mqd_t messageQueue2Id;
    struct mq_attr attr;

    char messageText[MAX_MSG_SIZE];
    char endMessage[END_SIZE] = "end";
    int status, count = 0;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    messageQueue1Id = mq_open(QUEUE1_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, &attr);
    if (messageQueue1Id == -1)
    {
        perror("Unable to create queue1");
        return 2;
    }

    messageQueue2Id = mq_open(QUEUE2_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, &attr);
    if (messageQueue2Id == -1)
    {
        perror("Unable to create queue2");
        mq_close(messageQueue1Id);
        mq_unlink(QUEUE1_NAME);
        return 2;
    }

    while (count < PING_PONG_SIZE)
    {
        strcpy(messageText, "Message from pro1");
        status = mq_send(messageQueue1Id, messageText, strlen(messageText) + 1, 0);
        if (status == -1)
        {
            perror("Unable to send message");
            mq_close(messageQueue1Id);
            mq_close(messageQueue2Id);
            mq_unlink(QUEUE1_NAME);
            mq_unlink(QUEUE2_NAME);
            return 2;
        }

        status = mq_receive(messageQueue2Id, messageText, MAX_MSG_SIZE, NULL);
        if (status == -1)
        {
            perror("Unable to receive message");
            mq_close(messageQueue1Id);
            mq_close(messageQueue2Id);
            mq_unlink(QUEUE1_NAME);
            mq_unlink(QUEUE2_NAME);
            return 2;
        }

        printf("%s\n", messageText);

        ++count;
    }

    status = mq_send(messageQueue1Id, endMessage, strlen(endMessage) + 1, 0);
    if (status == -1)
    {
        perror("Unable to send end message");
        mq_close(messageQueue1Id);
        mq_close(messageQueue2Id);
        mq_unlink(QUEUE1_NAME);
        mq_unlink(QUEUE2_NAME);
        return 2;
    }

    status = mq_receive(messageQueue2Id, messageText, MAX_MSG_SIZE, NULL);
    if (status == -1)
    {
        perror("Unable to receive end message");
        mq_close(messageQueue1Id);
        mq_close(messageQueue2Id);
        mq_unlink(QUEUE1_NAME);
        mq_unlink(QUEUE2_NAME);
        return 2;
    }

    printf("%s\n", messageText);

    mq_close(messageQueue1Id);
    mq_close(messageQueue2Id);
    mq_unlink(QUEUE1_NAME);
    mq_unlink(QUEUE2_NAME);

    return 0;
}
