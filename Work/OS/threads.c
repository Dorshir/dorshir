#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define NUM_OF_THREADS 10

size_t g_v;

void* PrintFunc(void *tid)
{
    size_t l_v;
    for (l_v = 0; l_v < 10000; l_v++)
    {
        printf("ThreadID: %d\nLocal variable: %ld\nGlobal variable: %ld\n", *((int*)tid), l_v, g_v );
        ++g_v;
    }
}

int main()
{
    size_t index;
    pthread_t threads[NUM_OF_THREADS];
    int indexes[NUM_OF_THREADS];
    for (index = 0; index < NUM_OF_THREADS; index++)
    {
        indexes[index] = index+1;
        pthread_create(&threads[index],NULL, PrintFunc, &indexes[index]);
    }

    for (index = 0; index < NUM_OF_THREADS; index++)
    {
        pthread_join(threads[index], NULL);
    }
    return 0;
}