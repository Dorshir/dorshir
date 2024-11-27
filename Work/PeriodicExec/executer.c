#include "executer.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include <stdlib.h> /* free, malloc */
#include <stdio.h>  /* NULL */
#include <time.h>   /* */
#include <string.h> /* strcpy */
#include <stdint.h> /* SIZE_MAX */

#define SMALL_PERIOD 0.00001

typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

struct PeriodicExecutor
{
    Vector *m_tasks;
    clockid_t m_clk_id;
    char *m_name;
    Bool m_pauseFlag;
};

static PeriodicExecutorResult AddInputCheck(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms);
int TaskComparator(const void *_a, const void *_b);

/* **************** * API Functions * **************** */

PeriodicExecutor *PeriodicExecutor_Create(const char *_name, clockid_t _clk_id)
{
    if (_name == NULL)
    {
        return NULL;
    }

    PeriodicExecutor *newPe = malloc(sizeof(PeriodicExecutor));
    if (newPe == NULL)
    {
        return NULL;
    }

    newPe->m_clk_id = _clk_id;

    size_t length = strlen(_name);

    newPe->m_name = malloc((length + 1) * sizeof(char));
    if (newPe->m_name == NULL)
    {
        free(newPe);
        return NULL;
    }

    newPe->m_name[length] = '\0'; /* null-termination for strcpy */
    strcpy(newPe->m_name, _name);

    newPe->m_pauseFlag = FALSE;

    if (VectorCreate(5, 3) == NULL)
    {
        free(newPe->m_name);
        free(newPe);
        return NULL;
    }

    return newPe;
}

int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms)
{
    PeriodicExecutorResult res = AddInputCheck(_executor, _taskFunction, _context, _period_ms);
    if (res != PE_SUCCESS)
    {
        return FALSE;
    }

    Task *newTask = Task_Create(_taskFunction, _context, _period_ms, _executor->m_clk_id);
    if (newTask == NULL)
    {
        return FALSE;
    }

    if (VectorAppend(_executor->m_tasks, newTask) != VECTOR_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

size_t PeriodicExecutor_Run(PeriodicExecutor *_executor)
{
    size_t executeCycles = 0;
    Heap *minHeap = HeapBuild(_executor->m_tasks, TaskComparator);
    if (minHeap == NULL)
    {
        return executeCycles;
    }

    while (_executor->m_pauseFlag == FALSE)
    {
        Task *extractedTask = HeapExtract(minHeap);
        if (extractedTask == NULL)
        {
            break;
        }

        size_t currentTime = GetCurrentTime_ms(_executor->m_clk_id);
        while (currentTime < extractedTask->m_t2e)
        {
            currentTime = GetCurrentTime_ms(_executor->m_clk_id);
            /* busy wait (maybe condition variable is better) */
        }

        if (Task_Execute(extractedTask) == 0)
        {
            currentTime = GetCurrentTime_ms(_executor->m_clk_id);
            extractedTask->m_t2e = currentTime + extractedTask->m_period;
            if (HeapInsert(minHeap, extractedTask) != HEAP_SUCCESS)
            {
                Task_Destroy(&extractedTask);
                HeapDestroy(&minHeap);
                return SIZE_MAX;
            }
        }
        else
        {
            Task_Destroy(&extractedTask);
        }
        ++executeCycles;
    }

    HeapDestroy(&minHeap);
    return executeCycles;
}

size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor)
{
    if (_executor == NULL)
    {
        return SIZE_MAX;
    }

    if (PeriodicExecutor_Add(_executor, Pause, _executor, SMALL_PERIOD) == FALSE)
    {
        return SIZE_MAX;
    }

    return VectorSize(_executor->m_tasks);
}

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor)
{
    if (_executor == NULL)
    {
        return;
    }
    VectorDestroy(_executor->m_tasks, free);
    free(_executor->m_name);
    free(_executor);
}

/* **************** * Static Functions * **************** */

static PeriodicExecutorResult AddInputCheck(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms)
{
    if (_executor == NULL || _taskFunction == NULL || _context == NULL)
    {
        return PE_UNINITIALIZED_ERROR;
    }
    else if (_period_ms == 0)
    {
        return PE_PERIOD_ERROR;
    }
    return PE_SUCCESS;
}

int TaskComparator(const void *_a, const void *_b)
{
    if (_a == NULL || _b == NULL)
    {
        return 0;
    }

    const Task *taskA = *(const Task **)_a;
    const Task *taskB = *(const Task **)_b;

    if (taskA->m_t2e < taskB->m_t2e)
    {
        return 1;
    }
    else if (taskA->m_t2e > taskB->m_t2e)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void Pause(PeriodicExecutor *_executor)
{
    _executor->m_pauseFlag = TRUE;
}
