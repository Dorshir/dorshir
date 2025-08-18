#include "executor.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include <stdlib.h> /* size_t, free, malloc */
#include <time.h>   /* clockid_t */
#include <string.h> /* strcpy */
#include <stdint.h> /* SIZE_MAX */

#define TASKS_SIZE_INIT 5
#define BLOCKSIZE_INIT 3

size_t GetT2E(Task *_task);

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
static void TasksDestroy(void *_task);

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

    newPe->m_tasks = VectorCreate(TASKS_SIZE_INIT, BLOCKSIZE_INIT);
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
        return res;
    }

    Task *newTask = Task_Create(_taskFunction, _context, _period_ms, _executor->m_clk_id);
    if (newTask == NULL)
    {
        return PE_ALLOCATION_ERROR;
    }

    if (VectorAppend(_executor->m_tasks, newTask) != VECTOR_SUCCESS)
    {
        Task_Destroy(&newTask);
        return PE_APPEND_ERROR;
    }
    return PE_SUCCESS;
}

size_t PeriodicExecutor_Run(PeriodicExecutor *_executor)
{
    size_t executeCycles = 0;

    if (VectorForEach(_executor->m_tasks, SetTime2Exec, NULL) != VectorSize(_executor->m_tasks))
    {
        return executeCycles;
    }

    Heap *minHeap = HeapBuild(_executor->m_tasks, TaskComparator);
    if (minHeap == NULL)
    {
        return executeCycles;
    }

    while (_executor->m_pauseFlag == FALSE && HeapSize(minHeap) > 0)
    {
        Task *extractedTask = HeapExtract(minHeap);
        if (extractedTask == NULL) /* No Tasks To Execute */
        {
            break;
        }

        if (Task_Execute(extractedTask) == 0)
        {
            SetTime2Exec(extractedTask, 0, NULL);
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

    VectorDestroy(&_executor->m_tasks, TasksDestroy);
    free(_executor->m_name);
    _executor->m_magic = 0;
    free(_executor);
}

/* **************** * Static Functions * **************** */

static PeriodicExecutorResult AddInputCheck(PeriodicExecutor *_executor, int (*_taskFunction)(void *), size_t _period_ms)
{
    PeriodicExecutorResult res = PE_SUCCESS;
    if (_executor == NULL || _taskFunction == NULL)
    {
        res = PE_UNINITIALIZED_ERROR;
    }
    else if (_period_ms == 0)
    {
        res = PE_PERIOD_ERROR;
    }
    return res;
}

static void TasksDestroy(void *_task)
{
    Task_Destroy(&_task);
}

/* **************** * Getters and Setters Functions * **************** */

Vector *GetTasks(PeriodicExecutor *_executor)
{
    return _executor->m_tasks;
}

clockid_t GetClockIDPE(PeriodicExecutor *_executor)
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

void SetPauseFlag(PeriodicExecutor *_executor)
{
    _executor->m_pauseFlag = !_executor->m_pauseFlag;
}