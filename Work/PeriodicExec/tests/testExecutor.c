#define _POSIX_C_SOURCE 199309L

#include "executor.h"
#include "dynamicVector.h"
#include "genheap.h"
#include "task.h"
#include "calctime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

/* Helper Functions */
static int SampleTaskFunction(void *context);
static int InvalidTaskFunction(void *context);

/* Test Function Prototypes */
/* Create Tests */
static void TestPeriodicExecutor_CreateValid();
static void TestPeriodicExecutor_CreateNullName();

/* Add Tests */
static void TestPeriodicExecutor_AddValid();
static void TestPeriodicExecutor_AddNullExecutor();
static void TestPeriodicExecutor_AddNullTaskFunction();
static void TestPeriodicExecutor_AddNullContext();
static void TestPeriodicExecutor_AddZeroPeriod();

/* Run Tests */
static void TestPeriodicExecutor_RunWithTasks();
static void TestPeriodicExecutor_RunWithNoTasks();
static void TestPeriodicExecutor_RunPause();

/* Pause Tests */
static void TestPeriodicExecutor_PauseValid();
static void TestPeriodicExecutor_PauseNullExecutor();

/* Destroy Tests */
static void TestPeriodicExecutor_DestroyValid();
static void TestPeriodicExecutor_DestroyNullExecutor();
static void TestPeriodicExecutor_DestroyDoubleDestroy();

/* Main Function */
int main()
{
    /* Create Tests */
    TestPeriodicExecutor_CreateValid();
    TestPeriodicExecutor_CreateNullName();

    /* Add Tests */
    TestPeriodicExecutor_AddValid();
    TestPeriodicExecutor_AddNullExecutor();
    TestPeriodicExecutor_AddNullTaskFunction();
    TestPeriodicExecutor_AddNullContext();
    TestPeriodicExecutor_AddZeroPeriod();

    /* Run Tests */
    TestPeriodicExecutor_RunWithTasks();
    TestPeriodicExecutor_RunWithNoTasks();
    TestPeriodicExecutor_RunPause();

    /* Pause Tests */
    TestPeriodicExecutor_PauseValid();
    TestPeriodicExecutor_PauseNullExecutor();

    /* Destroy Tests */
    TestPeriodicExecutor_DestroyValid();
    TestPeriodicExecutor_DestroyNullExecutor();
    TestPeriodicExecutor_DestroyDoubleDestroy();

    return 0;
}

/* Helper Function Implementations */
static int SampleTaskFunction(void *context)
{
    char *message = (char *)context;
    printf("Executing Task: %s\n", message);
    return 0; // Return 0 to indicate rescheduling
}

static int InvalidTaskFunction(void *context)
{
    return -1; // Return non-zero to indicate task completion
}

/* Test Function Implementations */

/* Create Tests */
static void TestPeriodicExecutor_CreateValid()
{
    printf("TestPeriodicExecutor_CreateValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor1", CLOCK_MONOTONIC);
    if (executor != NULL && executor->m_tasks != NULL && strcmp(executor->m_name, "Executor1") == 0 && executor->m_pauseFlag == FALSE)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_CreateNullName()
{
    printf("TestPeriodicExecutor_CreateNullName: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create(NULL, CLOCK_MONOTONIC);
    if (executor == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        PeriodicExecutor_Destroy(executor);
    }
}

/* Add Tests */
static void TestPeriodicExecutor_AddValid()
{
    printf("TestPeriodicExecutor_AddValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor2", CLOCK_MONOTONIC);
    char *msg = "Task1";
    int result = PeriodicExecutor_Add(executor, SampleTaskFunction, msg, 1000);
    if (result == TRUE && VectorSize(executor->m_tasks) == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_AddNullExecutor()
{
    printf("TestPeriodicExecutor_AddNullExecutor: ");
    char *msg = "Task2";
    int result = PeriodicExecutor_Add(NULL, SampleTaskFunction, msg, 1000);
    if (result == FALSE)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestPeriodicExecutor_AddNullTaskFunction()
{
    printf("TestPeriodicExecutor_AddNullTaskFunction: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor3", CLOCK_MONOTONIC);
    char *msg = "Task3";
    int result = PeriodicExecutor_Add(executor, NULL, msg, 1000);
    if (result == FALSE && VectorSize(executor->m_tasks) == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_AddNullContext()
{
    printf("TestPeriodicExecutor_AddNullContext: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor4", CLOCK_MONOTONIC);
    int result = PeriodicExecutor_Add(executor, SampleTaskFunction, NULL, 1000);
    if (result == FALSE && VectorSize(executor->m_tasks) == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_AddZeroPeriod()
{
    printf("TestPeriodicExecutor_AddZeroPeriod: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor5", CLOCK_MONOTONIC);
    char *msg = "Task5";
    int result = PeriodicExecutor_Add(executor, SampleTaskFunction, msg, 0);
    if (result == FALSE && VectorSize(executor->m_tasks) == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    PeriodicExecutor_Destroy(executor);
}

/* Run Tests */
static void TestPeriodicExecutor_RunWithTasks()
{
    printf("TestPeriodicExecutor_RunWithTasks: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor6", CLOCK_MONOTONIC);
    char *msg1 = "Task6_1";
    char *msg2 = "Task6_2";
    PeriodicExecutor_Add(executor, SampleTaskFunction, msg1, 500);  // 0.5 seconds
    PeriodicExecutor_Add(executor, SampleTaskFunction, msg2, 1000); // 1 second

    size_t cycles = PeriodicExecutor_Run(executor);
    if (cycles >= 2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Cycles: %zu)\n", cycles);
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_RunWithNoTasks()
{
    printf("TestPeriodicExecutor_RunWithNoTasks: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor7", CLOCK_MONOTONIC);
    size_t cycles = PeriodicExecutor_Run(executor);
    if (cycles == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Cycles: %zu)\n", cycles);
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_RunPause()
{
    printf("TestPeriodicExecutor_RunPause: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor8", CLOCK_MONOTONIC);
    char *msg = "Task8";
    PeriodicExecutor_Add(executor, SampleTaskFunction, msg, 1000); // 1 second

    // Run in a separate process or thread is not feasible in this single-threaded context
    // Therefore, simulate pause by setting the pause flag before running
    executor->m_pauseFlag = TRUE;
    size_t cycles = PeriodicExecutor_Run(executor);
    if (cycles == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Cycles: %zu)\n", cycles);
    }
    PeriodicExecutor_Destroy(executor);
}

/* Pause Tests */
static void TestPeriodicExecutor_PauseValid()
{
    printf("TestPeriodicExecutor_PauseValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor9", CLOCK_MONOTONIC);
    char *msg = "Task9";
    PeriodicExecutor_Add(executor, SampleTaskFunction, msg, 1000); // 1 second

    size_t tasksBeforePause = PeriodicExecutor_Pause(executor);
    if (tasksBeforePause == 1 && executor->m_pauseFlag == TRUE)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Tasks Before Pause: %zu, PauseFlag: %d)\n", tasksBeforePause, executor->m_pauseFlag);
    }
    PeriodicExecutor_Destroy(executor);
}

static void TestPeriodicExecutor_PauseNullExecutor()
{
    printf("TestPeriodicExecutor_PauseNullExecutor: ");
    size_t result = PeriodicExecutor_Pause(NULL);
    if (result == SIZE_MAX)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Result: %zu)\n", result);
    }
}

/* Destroy Tests */
static void TestPeriodicExecutor_DestroyValid()
{
    printf("TestPeriodicExecutor_DestroyValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor10", CLOCK_MONOTONIC);
    PeriodicExecutor_Destroy(executor);
    // Since the executor is freed, we cannot directly check its fields
    // Instead, ensure no crash occurs
    printf("PASS\n");
}

static void TestPeriodicExecutor_DestroyNullExecutor()
{
    printf("TestPeriodicExecutor_DestroyNullExecutor: ");
    PeriodicExecutor_Destroy(NULL);
    printf("PASS\n");
}

static void TestPeriodicExecutor_DestroyDoubleDestroy()
{
    printf("TestPeriodicExecutor_DestroyDoubleDestroy: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor11", CLOCK_MONOTONIC);
    PeriodicExecutor_Destroy(executor);
    // Attempt to destroy again
    PeriodicExecutor_Destroy(executor);
    // If no crash occurs, consider it a pass
    printf("PASS\n");
}
