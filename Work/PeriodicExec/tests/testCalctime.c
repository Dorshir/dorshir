#define _POSIX_C_SOURCE 199309L

#include "calctime.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>

/* Test Function Prototypes */
/* GetCurrentTime_ms Tests */
static void TestGetCurrentTime_msValid();
static void TestGetCurrentTime_msInvalidClock();

int main()
{
    TestGetCurrentTime_msValid();
    TestGetCurrentTime_msInvalidClock();

    return 0;
}

/* GetCurrentTime_ms Tests */
static void TestGetCurrentTime_msValid()
{
    printf("TestGetCurrentTime_msValid: ");
    size_t time1 = GetCurrentTime_ms(CLOCK_MONOTONIC);

    size_t time2 = GetCurrentTime_ms(CLOCK_MONOTONIC);
    if (time2 > time1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (time1: %zu, time2: %zu)\n", time1, time2);
    }
}

static void TestGetCurrentTime_msInvalidClock()
{
    printf("TestGetCurrentTime_msInvalidClock: ");
    size_t time = GetCurrentTime_ms(-1);
    if (time == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (time: %zu)\n", time);
    }
}
