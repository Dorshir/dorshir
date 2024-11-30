#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#define _POSIX_C_SOURCE 199309L

#include <time.h>   /* clockid_t */
#include <stddef.h> /* size_t */

typedef enum PeriodicExecutorResult
{
    PE_SUCCESS,          
    PE_ALLOCATION_ERROR,   
    PE_UNINITIALIZED_ERROR,
    PE_PERIOD_ERROR         
} PeriodicExecutorResult;

typedef struct PeriodicExecutor PeriodicExecutor;

/**
 * @brief Creates a new Periodic Executor.
 *
 * @param[in] _name - Name of the executor, must not be NULL.
 * @param[in] _clk_id - Clock ID to be used for timing (e.g., CLOCK_REALTIME).
 *
 * @return Pointer to the newly created Periodic Executor.
 * @retval NULL on failure (e.g., invalid parameters, allocation failure).
 */
PeriodicExecutor *PeriodicExecutor_Create(const char *_name, clockid_t _clk_id);

/**
 * @brief Adds a task to the executor.
 *
 * @param[in] _executor - Executor to add the task to, must not be NULL.
 * @param[in] _taskFunction - Function to be executed as the task, must not be NULL.
 * @param[in] _context - Context to be passed to the task function.
 * @param[in] _period_ms - Period of the task in milliseconds, must be greater than 0.
 *
 * @return int - Result of the operation.
 * @retval 1 on success.
 * @retval 0 on failure (e.g., invalid parameters, allocation failure).
 */
int PeriodicExecutor_Add(PeriodicExecutor *_executor, int (*_taskFunction)(void *), void *_context, size_t _period_ms);

/**
 * @brief Runs the executor, executing scheduled tasks.
 *
 * The executor will run tasks based on their scheduled times and will continue
 * until all tasks are completed or the executor is paused.
 *
 * @param[in] _executor - Executor to run, must not be NULL.
 *
 * @return size_t - Number of tasks executed.
 * @retval SIZE_MAX on failure (e.g., invalid executor).
 */
size_t PeriodicExecutor_Run(PeriodicExecutor *_executor);

/**
 * @brief Pauses the executor, halting execution of tasks.
 *
 * Execution can be resumed by calling PeriodicExecutor_Run again.
 *
 * @param[in] _executor - Executor to pause, must not be NULL.
 *
 * @return size_t - Number of tasks remaining in the executor.
 * @retval SIZE_MAX on failure (e.g., invalid executor).
 */
size_t PeriodicExecutor_Pause(PeriodicExecutor *_executor);

/**
 * @brief Destroys the executor and frees associated resources.
 *
 * @param[in] _executor - Executor to destroy.
 *
 * @warning If _executor is NULL or already destroyed, the function does nothing.
 */
void PeriodicExecutor_Destroy(PeriodicExecutor *_executor);

#endif /* __EXECUTOR_H__ */
