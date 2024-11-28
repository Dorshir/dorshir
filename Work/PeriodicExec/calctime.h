#ifndef __CALCTIME_H__
#define __CALCTIME_H__

#include <time.h>
#include <sys/types.h>




void CalcTime(clockid_t _clk_id, size_t _period, struct timespec *_ts);

void SleepIfNeeds(clockid_t _clk_id, struct timespec *_taskTs, struct timespec *_currTs);

int TimeComperator(clockid_t _clk_id, struct timespec *_ts1, struct timespec *_ts2);

#endif /* __CALCTIME_H__ */