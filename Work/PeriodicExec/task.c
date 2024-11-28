#define _POSIX_C_SOURCE 199309L
#include "task.h"
#include "calctime.h"

#include <time.h>
#include <stdio.h>  /* NULL */
#include <limits.h> /* INT_MAX */
#include <stdlib.h> /* malloc, free */

struct Task
{
    TaskFunc m_func;
    void *m_context;
    size_t m_period;
    size_t m_t2e;
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
    newTask->m_period = _period_ms;
    newTask->m_clk_id = _clk_id;
    newTask->m_t2e = 0;

    return newTask;
}

int Task_Execute(Task *_task)
{
    if (_task == NULL)
    {
        return INT_MAX;
    }

    size_t currentTime = GetCurrentTime_ms(_task->m_t2e);
    while (currentTime < _task->m_t2e)
    {
        currentTime = GetCurrentTime_ms(_task->m_clk_id);
        /* busy wait (maybe condition variable is better) */
    }
    return _task->m_func(_task->m_context);
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

int SetTime2Exec(void *_element, size_t _index, void *_context)
{
    if (_element == NULL)
    {
        return 0;
    }
    Task* task = (Task*)_element; 
    task->m_t2e = GetCurrentTime_ms(task->m_clk_id) + task->m_period;

    return 1;
}

void Task_Destroy(Task **_task)
{
    if (_task == NULL || *_task == NULL)
    {
        return;
    }
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
