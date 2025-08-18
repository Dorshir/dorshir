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

#define FALSE 0
#define TRUE 1

Vector *GetTasks(PeriodicExecutor *_executor);
clockid_t GetClockIDPE(PeriodicExecutor *_executor);
char *GetName(PeriodicExecutor *_executor);
int GetPauseFlag(PeriodicExecutor *_executor);
size_t GetMagicNum(PeriodicExecutor *_executor);
void SetPauseFlag(PeriodicExecutor *_executor);


/* Helper Functions */
static int OneExecTaskFunction(void *context);
static int InvalidTaskFunction(void *context);


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

int main()
{

    TestPeriodicExecutor_CreateValid();
    TestPeriodicExecutor_CreateNullName();

    TestPeriodicExecutor_AddValid();
    TestPeriodicExecutor_AddNullExecutor();
    TestPeriodicExecutor_AddNullTaskFunction();
    TestPeriodicExecutor_AddNullContext();
    TestPeriodicExecutor_AddZeroPeriod();

    TestPeriodicExecutor_RunWithTasks();
    TestPeriodicExecutor_RunWithNoTasks();
    TestPeriodicExecutor_RunPause();

    TestPeriodicExecutor_PauseValid();
    TestPeriodicExecutor_PauseNullExecutor();

    TestPeriodicExecutor_DestroyValid();
    TestPeriodicExecutor_DestroyNullExecutor();
    TestPeriodicExecutor_DestroyDoubleDestroy();

    return 0;
}

/* Helper Functions */
static int InfExecTaskFunction(void *context)
{
    char *message = (char *)context;
    printf("Executing Task: %s\n", message);
    return 0;
}

static int OneExecTaskFunction(void *context)
{
    char *message = (char *)context;
    printf("Executing Task: %s\n", message);
    return 1;
}

/* Test Functions */

/* Create Tests */
static void TestPeriodicExecutor_CreateValid()
{
    printf("TestPeriodicExecutor_CreateValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor1", CLOCK_MONOTONIC);
    if (executor != NULL &&
        GetTasks(executor) != NULL &&
        strcmp(GetName(executor), "Executor1") == 0 &&
        GetPauseFlag(executor) == FALSE)
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
    int result = PeriodicExecutor_Add(executor, OneExecTaskFunction, msg, 1000);
    if (result == TRUE && VectorSize(GetTasks(executor)) == 1)
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
    int result = PeriodicExecutor_Add(NULL, OneExecTaskFunction, msg, 1000);
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
    if (result == FALSE && VectorSize(GetTasks(executor)) == 0)
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
    int result = PeriodicExecutor_Add(executor, OneExecTaskFunction, NULL, 1000);
    if (result == TRUE && VectorSize(GetTasks(executor)) == 1)
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
    int result = PeriodicExecutor_Add(executor, OneExecTaskFunction, msg, 0);
    if (result == FALSE && VectorSize(GetTasks(executor)) == 0)
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
    PeriodicExecutor_Add(executor, OneExecTaskFunction, msg1, 500); 
    PeriodicExecutor_Add(executor, OneExecTaskFunction, msg2, 1000);

    size_t cycles = PeriodicExecutor_Run(executor);
    if (cycles == 2)
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
    PeriodicExecutor_Add(executor, InfExecTaskFunction, msg, 1000);

    SetPauseFlag(executor);
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

static void TestPeriodicExecutor_PauseValid()
{
    printf("TestPeriodicExecutor_PauseValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor9", CLOCK_MONOTONIC);
    char *msg = "Task9";
    PeriodicExecutor_Add(executor, OneExecTaskFunction, msg, 1000);

    size_t tasksBeforePause = PeriodicExecutor_Pause(executor);
    if (tasksBeforePause == 1 && GetPauseFlag(executor) == TRUE)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Tasks Before Pause: %zu, PauseFlag: %d)\n", tasksBeforePause, GetPauseFlag(executor));
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

static void TestPeriodicExecutor_DestroyValid()
{
    printf("TestPeriodicExecutor_DestroyValid: ");
    PeriodicExecutor *executor = PeriodicExecutor_Create("Executor10", CLOCK_MONOTONIC);
    PeriodicExecutor_Destroy(executor);
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
    PeriodicExecutor_Destroy(executor);
    printf("PASS\n");
}


