#ifndef __TASK_H__
#define __TASK_H__

#define _POSIX_C_SOURCE 199309L

#include <time.h>   /* clockid_t */
#include <stddef.h> /* size_t */

typedef struct Task Task;

/**
 * @typedef TaskFunc
 * 
 * @brief Function pointer type for task functions.
 *
 * @param[in] _context - The context provided to the task function.
 *
 * @return int - Return 0 to indicate the task should be rescheduled,
 * non-zero to indicate the task is complete and should not be rescheduled.
 */
typedef int (*TaskFunc)(void *_context);

/**
 * @brief Creates a new Task.
 *
 * @param[in] _taskFunc - Function to be executed as the task, must not be NULL.
 * @param[in] _context - Context to be passed to the task function.
 * @param[in] _period_ms - Period of the task in milliseconds, must be greater than 0.
 * @param[in] _clk_id - Clock ID to be used for timing (e.g., CLOCK_REALTIME).
 *
 * @return Pointer to the newly created Task.
 * @retval NULL on failure (e.g., invalid parameters, allocation failure).
 */
Task *Task_Create(TaskFunc _taskFunc, void *_context, size_t _period_ms, clockid_t _clk_id);

/**
 * @brief Destroys the Task and frees associated resources.
 *
 * @param[in,out] _task - Pointer to the Task to be destroyed.
 *
 * @warning After destruction, *_task will be set to NULL.
 * @warning If _task or *_task is NULL, the function does nothing.
 */
void Task_Destroy(Task **_task);

/**
 * @brief Executes the task function.
 *
 * @param[in] _task - The Task to execute, must not be NULL.
 *
 * @return int - Return value from the task function.
 * @retval 0 if the task should be rescheduled.
 * @retval Non-zero if the task is complete and should not be rescheduled.
 * @retval INT_MAX if _task is NULL.
 */
int Task_Execute(Task *_task);

/**
 * @brief Comparator function for tasks.
 *
 * Compares the next execution times of two tasks.
 *
 * @param[in] _a - Pointer to the first Task.
 * @param[in] _b - Pointer to the second Task.
 *
 * @return int - Comparison result.
 * @retval Negative if _a should come before _b.
 * @retval Zero if they are equal.
 * @retval Positive if _a should come after _b.
 */
int TaskComparator(const void *_a, const void *_b);

/**
 * @brief Sets the next execution time for the task.
 *
 * @param[in] _element - The Task to update, must not be NULL.
 * @param[in] _index - Index of the Task (unused).
 * @param[in] _context - Context (unused).
 *
 * @return int - Always returns 1.
 */
int SetTime2Exec(void *_element, size_t _index, void *_context);

#endif /* __TASK_H__ */
