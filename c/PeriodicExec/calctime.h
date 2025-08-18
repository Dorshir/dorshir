#ifndef __CALCTIME_H__
#define __CALCTIME_H__

#include <time.h>   /* clockid_t, timespec */
#include <stddef.h> /* size_t */

/**
 * @brief Calculates the next execution time for a task.
 *
 * @param[in] _clk_id - Clock ID to be used for timing (e.g., CLOCK_REALTIME).
 * @param[in] _period - Period in nanoseconds to add to the current time.
 * @param[out] _ts - Pointer to a timespec structure to store the calculated time, must not be NULL.
 *
 * @warning _ts must be a valid pointer.
 */
void CalcTime(clockid_t _clk_id, size_t _period, struct timespec *_ts);

/**
 * @brief Sleeps until the specified task execution time if needed.
 *
 * @param[in] _clk_id - Clock ID to be used for timing.
 * @param[in] _taskTs - Scheduled execution time of the task, must not be NULL.
 * @param[in] _currTs - Current time, must not be NULL.
 *
 * @warning _taskTs and _currTs must be valid pointers.
 */
void SleepIfNeeds(clockid_t _clk_id, struct timespec *_taskTs, struct timespec *_currTs);

/**
 * @brief Compares two timespec structures.
 *
 * @param[in] _clk_id - Clock ID (unused in this function).
 * @param[in] _ts1 - First timespec structure, must not be NULL.
 * @param[in] _ts2 - Second timespec structure, must not be NULL.
 *
 * @return int - Comparison result.
 * @retval 1 if _ts1 < _ts2 (i.e., _ts1 is earlier than _ts2).
 * @retval -1 if _ts1 > _ts2 (i.e., _ts1 is later than _ts2).
 * @retval 0 if _ts1 == _ts2.
 *
 * @warning _ts1 and _ts2 must be valid pointers.
 */
int TimeComperator(clockid_t _clk_id, struct timespec *_ts1, struct timespec *_ts2);

#endif /* __CALCTIME_H__ */
