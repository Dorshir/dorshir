#include "executer.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include <stdio.h>  /* NULL */
#include <time.h>   /* */
#include <string.h> /* strcpy */
#include <stdint.h> /* SIZE_MAX */

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

    newPe->m_name[length] = '\0';
    strcpy(newPe->m_name, _name);

    newPe->m_pauseFlag = FALSE;

    if (VectorCreate(5, 3) != VECTOR_SUCCESS)
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
    Task *newTask = Task_Create(_taskFunction, _context, _period_ms, 0); /* should not be 0, s hould be period + current */
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

    Heap *minHeap = HeapBuild(_executor->m_tasks, 1); /* should place a comperator based on the clockid at the executor and the t2e of each task */
    if (minHeap == NULL)
    {
        return executeCycles;
    }

    while (_executor->m_pauseFlag == FALSE)
    {
        Task *extractedTask = HeapExtract(minHeap);
        if (extractedTask == NULL)
        {
            return executeCycles;
        }

        if (Task_Execute(extractedTask) == 0)
        {
            /* calc new t2e for this task based on currenttime + period */
            if (HeapInsert(minHeap, extractedTask) != HEAP_SUCCESS)
            {
                free(minHeap);
                return SIZE_MAX;
            }
        }

        ++executeCycles;
    }
    return executeCycles;
}

size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor)
{
    if (_executor == NULL)
    {
        return SIZE_MAX;
    }
    _executor->m_pauseFlag = TRUE;
    return VectorSize(_executor->m_tasks);
}

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor)
{
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