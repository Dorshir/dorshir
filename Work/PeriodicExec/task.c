#define _POSIX_C_SOURCE 199309L
#include "task.h"
#include "calctime.h"

#include <time.h>
#include <stdio.h>  /* NULL */
#include <limits.h> /* INT_MAX */
#include <stdlib.h> /* malloc, free */

#define MILLION 1000000

struct Task
{
    TaskFunc m_func;
    void *m_context;
    size_t m_period;
    struct timespec *m_t2e;
    clockid_t m_clk_id;
};

typedef enum Task_Result
{
    TASK_SUCCESS,
    TASK_ALLOCATION_ERROR,
    TASK_UNINITIALIZE_ERROR
} TaskResult;

static TaskResult CreateInputCheck(TaskFunc _taskFunc, size_t _period_ms);

/* **************** * API Functions * **************** */

Task *Task_Create(TaskFunc _taskFunc, void *_context, size_t _period_ms, clockid_t _clk_id)
{
    TaskResult res = CreateInputCheck(_taskFunc, _period_ms);
    if (res != TASK_SUCCESS)
    {
        return NULL;
    }

    Task *newTask = malloc(sizeof(Task));
    if (newTask == NULL)
    {
        return NULL;
    }

    newTask->m_func = _taskFunc;
    newTask->m_context = _context;
    newTask->m_period = _period_ms * MILLION;
    newTask->m_clk_id = _clk_id;

    newTask->m_t2e = calloc(1, sizeof(struct timespec));
    if (newTask->m_t2e == NULL)
    {
        free(newTask);
        return NULL;
    }

    return newTask;
}

int Task_Execute(Task *_task)
{
    if (_task == NULL)
    {
        return INT_MAX;
    }

    struct timespec curr = {0};

    CalcTime(_task->m_clk_id,0, &curr);

    SleepIfNeeds(_task->m_clk_id, _task->m_t2e, &curr);

    return _task->m_func(_task->m_context);
}

int TaskComparator(const void *_a, const void *_b)
{
    if (_a == NULL || _b == NULL)
    {
        return 0;
    }

    const Task *taskA = (const Task *)_a;
    const Task *taskB = (const Task *)_b;

    int res = TimeComperator(taskA->m_clk_id, taskA->m_t2e, taskB->m_t2e);

    if (res > 0)
    {
        return 1;
    }
    else if (res < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int SetTime2Exec(void *_element, size_t _index, void *_context)
{
    if (_element == NULL)
    {
        return 0;
    }
    Task *task = (Task *)_element;

    CalcTime(task->m_clk_id, task->m_period, task->m_t2e);

    return 1;
}

void Task_Destroy(Task **_task)
{
    if (_task == NULL || *_task == NULL)
    {
        return;
    }
    free((*_task)->m_t2e);
    free(*_task);
    *_task = NULL;
}

/* **************** * Static Functions * **************** */

static TaskResult CreateInputCheck(TaskFunc _taskFunc, size_t _period_ms)
{
    if (_taskFunc == NULL || _period_ms == 0)
    {
        return TASK_UNINITIALIZE_ERROR;
    }
    return TASK_SUCCESS;
}

struct timespec *GetT2E(Task *_task)
{
    return _task->m_t2e;
}
