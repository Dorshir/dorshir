#include "executor.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include <stdlib.h> /* free, malloc */
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
    size_t m_magic;
};

static PeriodicExecutorResult AddInputCheck(PeriodicExecutor *_executor, int (*_taskFunction)(void *), size_t _period_ms);

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

    newPe->m_magic = SIZE_MAX;

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

    newPe->m_tasks = VectorCreate(5, 3);

    if (newPe->m_tasks == NULL)
    {
        free(newPe->m_name);
        free(newPe);
        return NULL;
    }

    return newPe;
}

int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms)
{
    PeriodicExecutorResult res = AddInputCheck(_executor, _taskFunction, _period_ms);
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

    VectorForEach(_executor->m_tasks, SetTime2Exec, NULL);

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

        if (Task_Execute(extractedTask) == 0)
        {
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

    _executor->m_pauseFlag = TRUE;
    return VectorSize(_executor->m_tasks);
}

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor)
{
    if (_executor == NULL || _executor->m_magic == 0)
    {
        return;
    }

    VectorDestroy(&_executor->m_tasks, free);
    free(_executor->m_name);
    _executor->m_magic = 0;
    free(_executor);
}

/* **************** * Static Functions * **************** */

static PeriodicExecutorResult AddInputCheck(PeriodicExecutor *_executor, int (*_taskFunction)(void *), size_t _period_ms)
{
    if (_executor == NULL || _taskFunction == NULL)
    {
        return PE_UNINITIALIZED_ERROR;
    }
    else if (_period_ms == 0)
    {
        return PE_PERIOD_ERROR;
    }
    return PE_SUCCESS;
}

/* GETTERS */

Vector *GetTasks(PeriodicExecutor *_executor)
{
    return _executor->m_tasks;
}

clockid_t GetClockID(PeriodicExecutor *_executor)
{
    return _executor->m_clk_id;
}

char *GetName(PeriodicExecutor *_executor)
{
    return _executor->m_name;
}

int GetPauseFlag(PeriodicExecutor *_executor)
{
    return _executor->m_pauseFlag;
}

size_t GetMagicNum(PeriodicExecutor *_executor)
{
    return _executor->m_magic;
}
