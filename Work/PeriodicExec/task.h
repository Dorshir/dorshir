#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>  /* size_t */



typedef Task Task;
typedef int (*TaskFunc)(void *_context);

Task *Task_Create(TaskFunc _taskFunc, void *_context, size_t _period_ms, size_t _t2e_ms);

Task *Task_Destroy(Task **_task);

int Task_Execute(Task *_task);

#endif /* __TASK_H__ */