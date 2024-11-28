#ifndef __TASK_H__
#define __TASK_H__

#define _POSIX_C_SOURCE 199309L

#include <time.h>

#include <stddef.h>  /* size_t */



typedef struct Task Task;
typedef int (*TaskFunc)(void *_context);

Task *Task_Create(TaskFunc _taskFunc, void *_context, size_t _period_ms, clockid_t _clk_id);

void Task_Destroy(Task **_task);

int Task_Execute(Task *_task);

int TaskComparator(const void *_a, const void *_b);

int SetTime2Exec(void *_element, size_t _index, void *_context);

#endif /* __TASK_H__ */