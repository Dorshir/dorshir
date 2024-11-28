#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "executor.h"
#include "task.h"
#include "calctime.h"
#include "genvec.h"


Vector *GetTasks(PeriodicExecutor *_executor);

clockid_t GetClockID(PeriodicExecutor *_executor);

char *GetName(PeriodicExecutor *_executor);

int GetPauseFlag(PeriodicExecutor *_executor);

size_t GetMagicNum(PeriodicExecutor *_executor);

int PrintA(void *_context)
{
    static int count = 0;
    printf("IN PRINTA\n");
    ++count;
    if(count == 5){
        return 0;
    }
    return 1;
}
int PrintB(void *_context)
{
    printf("IN PRINTB\n");

    return 1;
}

int main()
{

    PeriodicExecutor *pe = PeriodicExecutor_Create("pe1", CLOCK_REALTIME);

    PeriodicExecutor_Add(pe, PrintA, NULL, 2000);
    PeriodicExecutor_Add(pe, PrintB, NULL, 3000);

    PeriodicExecutor_Run(pe);

    return 0;
}