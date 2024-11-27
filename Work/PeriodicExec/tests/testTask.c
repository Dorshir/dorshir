#define _POSIX_C_SOURCE 199309L

#include "task.h"
#include "calctime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0

/* Helper Functions */
static int ValidTaskFunction(void *context);
static int InvalidTaskFunction(void *context);

/* Test Function Prototypes */
/* Create Tests */
static void TestTask_CreateValid();
static void TestTask_CreateNullFunction();
static void TestTask_CreateNullContext();
static void TestTask_CreateZeroPeriod();

/* Execute Tests */
static void TestTask_ExecuteValid();
static void TestTask_ExecuteNullTask();

/* Destroy Tests */
static void TestTask_DestroyValid();
static void TestTask_DestroyNullTask();
static void TestTask_DestroyDoubleDestroy();

/* Main Function */
int main()
{
    /* Create Tests */
    TestTask_CreateValid();
    TestTask_CreateNullFunction();
    TestTask_CreateNullContext();
    TestTask_CreateZeroPeriod();

    /* Execute Tests */
    TestTask_ExecuteValid();
    TestTask_ExecuteNullTask();

    /* Destroy Tests */
    TestTask_DestroyValid();
    TestTask_DestroyNullTask();
    TestTask_DestroyDoubleDestroy();

    return 0;
}

/* Helper Function Implementations */
static int ValidTaskFunction(void *context)
{
    char *message = (char *)context;
    printf("Executing Valid Task: %s\n", message);
    return 0; // Return 0 to indicate rescheduling
}

static int InvalidTaskFunction(void *context)
{
    printf("Executing Invalid Task.\n");
    return -1; // Return non-zero to indicate task completion
}

/* Test Function Implementations */

/* Create Tests */
static void TestTask_CreateValid()
{
    printf("TestTask_CreateValid: ");
    char *msg = "Task1";
    Task *task = Task_Create(ValidTaskFunction, msg, 1000, CLOCK_MONOTONIC);
    if (task != NULL && task->m_func == ValidTaskFunction && strcmp((char *)task->m_context, msg) == 0 && task->m_period == 1000)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    Task_Destroy(&task);
}

static void TestTask_CreateNullFunction()
{
    printf("TestTask_CreateNullFunction: ");
    char *msg = "Task2";
    Task *task = Task_Create(NULL, msg, 1000, CLOCK_MONOTONIC);
    if (task == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        Task_Destroy(&task);
    }
}

static void TestTask_CreateNullContext()
{
    printf("TestTask_CreateNullContext: ");
    Task *task = Task_Create(ValidTaskFunction, NULL, 1000, CLOCK_MONOTONIC);
    if (task == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        Task_Destroy(&task);
    }
}

static void TestTask_CreateZeroPeriod()
{
    printf("TestTask_CreateZeroPeriod: ");
    char *msg = "Task3";
    Task *task = Task_Create(ValidTaskFunction, msg, 0, CLOCK_MONOTONIC);
    if (task == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        Task_Destroy(&task);
    }
}

/* Execute Tests */
static void TestTask_ExecuteValid()
{
    printf("TestTask_ExecuteValid: ");
    char *msg = "Task4";
    Task *task = Task_Create(ValidTaskFunction, msg, 1000, CLOCK_MONOTONIC);
    if (Task_Execute(task) == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    Task_Destroy(&task);
}

static void TestTask_ExecuteNullTask()
{
    printf("TestTask_ExecuteNullTask: ");
    int result = Task_Execute(NULL);
    if (result == INT_MAX)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL (Result: %d)\n", result);
    }
}

/* Destroy Tests */
static void TestTask_DestroyValid()
{
    printf("TestTask_DestroyValid: ");
    char *msg = "Task5";
    Task *task = Task_Create(ValidTaskFunction, msg, 1000, CLOCK_MONOTONIC);
    Task_Destroy(&task);
    if (task == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestTask_DestroyNullTask()
{
    printf("TestTask_DestroyNullTask: ");
    Task_Destroy(NULL);
    printf("PASS\n");
}

static void TestTask_DestroyDoubleDestroy()
{
    printf("TestTask_DestroyDoubleDestroy: ");
    char *msg = "Task6";
    Task *task = Task_Create(ValidTaskFunction, msg, 1000, CLOCK_MONOTONIC);
    Task_Destroy(&task);
    Task_Destroy(&task); // Second destroy should safely do nothing
    if (task == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}
