#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 500

#include "calctime.h"
#include <time.h>   /* clockid_t, clock_gettime, timespec */
#include <unistd.h> /* usleep */

#define MILLIARD 1000000000
#define MILLION 1000000
#define TOTAL_TIME(ts) (ts->tv_sec * MILLIARD + ts->tv_nsec)

void CalcTime(clockid_t _clk_id, size_t _period, struct timespec *_ts)
{
    if (_ts == NULL)
    {
        return;
    }
    clock_gettime(_clk_id, _ts);
    _ts->tv_nsec += _period;
}

void SleepIfNeeds(clockid_t _clk_id, struct timespec *_taskTs, struct timespec *_currTs)
{
    if (_taskTs == NULL || _currTs == NULL)
    {
        return;
    }

    if (TimeComperator(_clk_id, _currTs, _taskTs) == 1)
    {
        long secDiff = _taskTs->tv_sec - _currTs->tv_sec;
        long nanoDiff = _taskTs->tv_nsec - _currTs->tv_nsec;

        if (nanoDiff < 0)
        {
            nanoDiff += MILLIARD;
            secDiff -= 1;
        }

        long totalMicro = secDiff * MILLION + nanoDiff / 1000;

        if (totalMicro > 0)
        {
            usleep(totalMicro);
        }
    }
}

int TimeComperator(clockid_t _clk_id, struct timespec *_ts1, struct timespec *_ts2)
{
    if (TOTAL_TIME(_ts1) < TOTAL_TIME(_ts2))
    {
        return 1;
    }
    else if (TOTAL_TIME(_ts1) > TOTAL_TIME(_ts2))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}