#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

#define N 100

pthread_mutex_t lock;
sem_t empty;
sem_t full;

int g_i = 0;

void InsertItem(int item)
{
}

int ProduceItem()
{
}

int RemoveItem()
{
}

void ConsumeItem(int item)
{
}

void *producer(void *_dumb)
{
    int item;
    while (TRUE)
    {
        item = ProduceItem();
        sem_wait(&empty);
        pthread_mutex_lock(&lock);
        InsertItem(item);
        printf("g_i: %d\n", ++g_i);
        pthread_mutex_unlock(&lock);
        sem_post(&full);
    }
}

void *consumer(void *_dumb)
{
    int item;
    while (TRUE)
    {
        sem_wait(&full);
        pthread_mutex_lock(&lock);
        item = RemoveItem();
        printf("g_i: %d\n", --g_i);
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        ConsumeItem(item);
    }
}

int main()
{
    pthread_t prod1;
    pthread_t prod2;
    pthread_t cons1;
    pthread_t cons2;
    pthread_mutexattr_t attr;
    // pthread_mutexattr_init(&attr);
    // pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    // pthread_mutex_init(&lock, &attr);
    pthread_mutex_init(&lock, NULL);

    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    pthread_create(&prod1, NULL, producer, NULL);
    // pthread_create(&prod2, NULL, producer, NULL);
    // pthread_create(&cons1, NULL, consumer, NULL);
    pthread_create(&cons2, NULL, consumer, NULL);

    pthread_join(prod1, NULL);
    // pthread_join(prod2, NULL);
    // pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    return 0;
}