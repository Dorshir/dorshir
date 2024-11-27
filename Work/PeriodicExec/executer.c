#include "executer.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include <stdio.h>
#include <time.h>

PeriodicExecutor *PeriodicExecutor_Create(const char *_name, clockid_t _clk_id)
{
    if (_name == NULL)
    {
        return NULL;
    }
}

int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms)
{

}

size_t PeriodicExecutor_Run(PeriodicExecutor *_executor)
{
    return 1;
}

size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor)
{
    return 1;
}

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor)
{
}
