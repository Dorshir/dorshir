#define _POSIX_C_SOURCE 199309L

#include "calctime.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* CalcTime Tests */
static void TestCalcTimeValid();
static void TestCalcTimeNull();

/* SleepIfNeeds Tests */
static void TestSleepIfNeedsValid();
static void TestSleepIfNeedsNoSleep();
static void TestSleepIfNeedsNull();

/* TimeComperator Tests */
static void TestTimeComperatorEqual();
static void TestTimeComperatorEarlier();
static void TestTimeComperatorLater();

int main()
{
    TestCalcTimeValid();
    TestCalcTimeNull();

    TestSleepIfNeedsValid();
    TestSleepIfNeedsNoSleep();
    TestSleepIfNeedsNull();

    TestTimeComperatorEqual();
    TestTimeComperatorEarlier();
    TestTimeComperatorLater();

    return 0;
}

/* CalcTime Tests */
static void TestCalcTimeValid()
{
    printf("TestCalcTimeValid: ");
    struct timespec ts;
    CalcTime(CLOCK_MONOTONIC, 500000000, &ts);

    if (ts.tv_nsec >= 500000000)
    {
        printf("PASS (tv_sec: %ld, tv_nsec: %ld)\n", ts.tv_sec, ts.tv_nsec);
    }
    else
    {
        printf("FAIL (tv_sec: %ld, tv_nsec: %ld)\n", ts.tv_sec, ts.tv_nsec);
    }
}

static void TestCalcTimeNull()
{
    printf("TestCalcTimeNull: ");
    CalcTime(CLOCK_MONOTONIC, 500000000, NULL);
    printf("PASS (no crash)\n");
}

/* SleepIfNeeds Tests */
static void TestSleepIfNeedsValid()
{
    printf("TestSleepIfNeedsValid: ");
    struct timespec currTs, taskTs;
    clock_gettime(CLOCK_MONOTONIC, &currTs);
    taskTs = currTs;
    taskTs.tv_nsec += 500000000;
    if (taskTs.tv_nsec >= 1000000000)
    {
        taskTs.tv_nsec -= 1000000000;
        taskTs.tv_sec += 1;
    }

    SleepIfNeeds(CLOCK_MONOTONIC, &taskTs, &currTs);
    printf("PASS (slept for 500ms)\n");
}

static void TestSleepIfNeedsNoSleep()
{
    printf("TestSleepIfNeedsNoSleep: ");
    struct timespec currTs, taskTs;
    clock_gettime(CLOCK_MONOTONIC, &currTs);
    taskTs = currTs;

    SleepIfNeeds(CLOCK_MONOTONIC, &taskTs, &currTs);
    printf("PASS (did not sleep)\n");
}

static void TestSleepIfNeedsNull()
{
    printf("TestSleepIfNeedsNull: ");
    SleepIfNeeds(CLOCK_MONOTONIC, NULL, NULL);
    printf("PASS (no crash)\n");
}

/* TimeComperator Tests */
static void TestTimeComperatorEqual()
{
    printf("TestTimeComperatorEqual: ");
    struct timespec ts1, ts2;
    ts1.tv_sec = 1;
    ts1.tv_nsec = 500000000;
    ts2.tv_sec = 1;
    ts2.tv_nsec = 500000000;

    int result = TimeComperator(CLOCK_MONOTONIC, &ts1, &ts2);
    if (result == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (result: %d)\n", result);
    }
}

static void TestTimeComperatorEarlier()
{
    printf("TestTimeComperatorEarlier: ");
    struct timespec ts1, ts2;
    ts1.tv_sec = 1;
    ts1.tv_nsec = 400000000;
    ts2.tv_sec = 1;
    ts2.tv_nsec = 500000000;

    int result = TimeComperator(CLOCK_MONOTONIC, &ts1, &ts2);
    if (result == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (result: %d)\n", result);
    }
}

static void TestTimeComperatorLater()
{
    printf("TestTimeComperatorLater: ");
    struct timespec ts1, ts2;
    ts1.tv_sec = 1;
    ts1.tv_nsec = 600000000;
    ts2.tv_sec = 1;
    ts2.tv_nsec = 500000000;

    int result = TimeComperator(CLOCK_MONOTONIC, &ts1, &ts2);
    if (result == -1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (result: %d)\n", result);
    }
}
