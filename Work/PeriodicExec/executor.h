#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <stddef.h>    /* size_t */

typedef enum PeriodicExecutorResult
{
    PE_SUCCESS,
    PE_ALLOCATION_ERROR,
    PE_UNINITIALIZED_ERROR,
    PE_PERIOD_ERROR
} PeriodicExecutorResult;

typedef struct PeriodicExecutor PeriodicExecutor;

PeriodicExecutor *PeriodicExecutor_Create(const char *_name, clockid_t _clk_id);

int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms);

size_t PeriodicExecutor_Run(PeriodicExecutor *_executor);

size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor);

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor);

#endif /* __EXECUTOR_H__ */