#include "task.h"
#include <stdio.h>  /* NULL */
#include <limits.h> /* INT_MAX */

struct Task
{
    TaskFunc m_func;
    void *m_context;
    size_t m_period;
    size_t m_t2e;
};

typedef enum Task_Result
{
    TASK_SUCCESS,
    TASK_ALLOCATION_ERROR,
    TASK_UNINITIALIZE_ERROR
} TaskResult;

static TaskResult CreateInputCheck(TaskFunc _taskFunc, void *_context, size_t _period_ms);

/* **************** * API Functions * **************** */

Task *Task_Create(TaskFunc _taskFunc, void *_context, size_t _period_ms, size_t _t2e_ms)
{
    TaskResult res = CreateInputCheck(_taskFunc, _context, _period_ms);
    if (res != TASK_SUCCESS)
    {
        return NULL;
    }

    Task *newTask = malloc(sizeof(Task));
    if (newTask == NULL)
    {
        return TASK_ALLOCATION_ERROR;
    }

    newTask->m_func = _taskFunc;
    newTask->m_context = _context;
    newTask->m_period = _period_ms;
    newTask->m_t2e = 1; /* need to be period + current time( should depend on the clock id.. )*/

    return newTask;
}

int Task_Execute(Task *_task)
{
    if (_task == NULL)
    {
        return INT_MAX;
    }
    return _task->m_func(_task->m_context);
}

Task *Task_Destroy(Task **_task)
{
    if (_task == NULL || *_task == NULL)
    {
        return NULL;
    }
    free(*_task);
    *_task = NULL;
}

/* **************** * Static Functions * **************** */

static TaskResult CreateInputCheck(TaskFunc _taskFunc, void *_context, size_t _period_ms)
{
    if (_taskFunc == NULL || _context == NULL || _period_ms == 0)
    {
        return TASK_UNINITIALIZE_ERROR;
    }
    return TASK_SUCCESS;
}