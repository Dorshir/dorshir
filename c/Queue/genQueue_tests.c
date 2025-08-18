#include "genQueue.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper function */
static int DumbAction(void *element, void *context);
static int SumAction(void *element, void *context);

/* QueueCreate Tests */
static void TestQueueCreateValidSize();
static void TestQueueCreateZeroSize();

/* QueueDestroy Tests */
static void TestQueueDestroyInitializedQueue();
static void TestQueueDestroyUninitializedQueue();
static void TestQueueDestroyDoubleDestroy();
static void TestQueueDestroyNullElementDestroy();

/* QueueInsert Tests */
static void TestQueueInsertValid();
static void TestQueueInsertNullQueue();
static void TestQueueInsertNullItem();
static void TestQueueInsertOverflow();

/* QueueRemove Tests */
static void TestQueueRemoveValidRemove();
static void TestQueueRemoveNullQueue();
static void TestQueueRemoveNullPValue();
static void TestQueueRemoveEmptyQueue();

/* QueueIsEmpty Tests */
static void TestQueueIsEmptyNullQueue();
static void TestQueueIsEmptyActuallyEmpty();
static void TestQueueIsEmptyActuallyNotEmpty();

/* QueueForEach Tests */
static void TestQueueForEachNullQueueOrAction();
static void TestQueueForEachValidRun();
static void TestQueueForEachNullContext();

int main()
{
    /* QueueCreate Tests */
    TestQueueCreateValidSize();
    TestQueueCreateZeroSize();

    /* QueueDestroy Tests */
    TestQueueDestroyInitializedQueue();
    TestQueueDestroyUninitializedQueue();
    TestQueueDestroyDoubleDestroy();
    TestQueueDestroyNullElementDestroy();

    /* QueueInsert Tests */
    TestQueueInsertValid();
    TestQueueInsertNullQueue();
    TestQueueInsertNullItem();
    TestQueueInsertOverflow();

    /* QueueRemove Tests */
    TestQueueRemoveValidRemove();
    TestQueueRemoveNullQueue();
    TestQueueRemoveNullPValue();
    TestQueueRemoveEmptyQueue();

    /* QueueIsEmpty Tests */
    TestQueueIsEmptyNullQueue();
    TestQueueIsEmptyActuallyEmpty();
    TestQueueIsEmptyActuallyNotEmpty();

    /* QueueForEach Tests */
    TestQueueForEachNullQueueOrAction();
    TestQueueForEachValidRun();
    TestQueueForEachNullContext();

    return 0;
}

/* QueueCreate Tests */

static void TestQueueCreateValidSize()
{
    Queue *queue = QueueCreate(5);
    if (queue != NULL)
    {
        printf("TestQueueCreateValidSize: PASS\n");
        QueueDestroy(&queue, NULL);
    }
    else
    {
        printf("TestQueueCreateValidSize: FAIL\n");
    }
}

static void TestQueueCreateZeroSize()
{
    Queue *queue = QueueCreate(0);
    if (queue == NULL)
    {
        printf("TestQueueCreateZeroSize: PASS\n");
    }
    else
    {
        printf("TestQueueCreateZeroSize: FAIL\n");
        QueueDestroy(&queue, NULL);
    }
}

/* QueueDestroy Tests */

static void TestQueueDestroyInitializedQueue()
{
    Queue *queue = QueueCreate(5);
    QueueDestroy(&queue, free);
    if (queue == NULL)
    {
        printf("TestQueueDestroyInitializedQueue: PASS\n");
    }
    else
    {
        printf("TestQueueDestroyInitializedQueue: FAIL\n");
        QueueDestroy(&queue, NULL);
    }
}

static void TestQueueDestroyUninitializedQueue()
{
    Queue *queue = NULL;
    QueueDestroy(&queue, NULL);
    if (queue == NULL)
    {
        printf("TestQueueDestroyUninitializedQueue: PASS\n");
    }
    else
    {
        printf("TestQueueDestroyUninitializedQueue: FAIL\n");
        QueueDestroy(&queue, NULL);
    }
}

static void TestQueueDestroyDoubleDestroy()
{
    Queue *queue = QueueCreate(5);
    QueueDestroy(&queue, NULL);
    QueueDestroy(&queue, NULL);
    if (queue == NULL)
    {
        printf("TestQueueDestroyDoubleDestroy: PASS\n");
    }
    else
    {
        printf("TestQueueDestroyDoubleDestroy: FAIL\n");
        QueueDestroy(&queue, NULL);
    }
}

static void TestQueueDestroyNullElementDestroy()
{
    int item = 42;
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &item);
    QueueDestroy(&queue, NULL);
    if (queue == NULL)
    {
        printf("TestQueueDestroyNullElementDestroy: PASS\n");
    }
    else
    {
        printf("TestQueueDestroyNullElementDestroy: FAIL\n");
        QueueDestroy(&queue, NULL);
    }
}

/* QueueInsert Tests */

static void TestQueueInsertValid()
{
    int item = 42;
    Queue *queue = QueueCreate(5);

    QueueResult result = QueueInsert(queue, &item);
    if (result == QUEUE_SUCCESS)
    {
        printf("TestQueueInsertValidPush: PASS\n");
    }
    else
    {
        printf("TestQueueInsertValidPush: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueInsertNullQueue()
{
    int item = 42;
    QueueResult result = QueueInsert(NULL, &item);
    if (result == QUEUE_UNINITIALIZED_ERROR)
    {
        printf("TestQueueInsertNullQueue: PASS\n");
    }
    else
    {
        printf("TestQueueInsertNullQueue: FAIL\n");
    }
}

static void TestQueueInsertNullItem()
{
    Queue *queue = QueueCreate(5);
    QueueResult result = QueueInsert(queue, NULL);
    if (result == QUEUE_DATA_UNINITIALIZED_ERROR)
    {
        printf("TestQueueInsertNullItem: PASS\n");
    }
    else
    {
        printf("TestQueueInsertNullItem: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueInsertOverflow()
{
    QueueResult result;
    Queue *queue = QueueCreate(2);
    int items[3] = {1, 2, 3};
    QueueInsert(queue, &items[0]);
    QueueInsert(queue, &items[1]);

    result = QueueInsert(queue, &items[2]); /* Should overflow */
    if (result == QUEUE_OVERFLOW_ERROR)
    {
        printf("TestQueueInsertOverflow: PASS\n");
    }
    else
    {
        printf("TestQueueInsertOverflow: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

/* QueueRemove Tests */

static void TestQueueRemoveValidRemove()
{
    QueueResult result;
    int item = 42;
    int *removedItem = NULL;
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &item);

    result = QueueRemove(queue, (void **)&removedItem);
    if (result == QUEUE_SUCCESS && removedItem == &item)
    {
        printf("TestQueueRemoveValidRemove: PASS\n");
    }
    else
    {
        printf("TestQueueRemoveValidRemove: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueRemoveNullQueue()
{
    int *removedItem = NULL;
    QueueResult result = QueueRemove(NULL, (void **)&removedItem);
    if (result == QUEUE_UNINITIALIZED_ERROR)
    {
        printf("TestQueueRemoveNullQueue: PASS\n");
    }
    else
    {
        printf("TestQueueRemoveNullQueue: FAIL\n");
    }
}

static void TestQueueRemoveNullPValue()
{
    QueueResult result;
    int item = 42;
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &item);

    result = QueueRemove(queue, NULL);
    if (result == QUEUE_DATA_UNINITIALIZED_ERROR)
    {
        printf("TestQueueRemoveNullPValue: PASS\n");
    }
    else
    {
        printf("TestQueueRemoveNullPValue: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueRemoveEmptyQueue()
{
    int *removedItem = NULL;
    Queue *queue = QueueCreate(5);

    QueueResult result = QueueRemove(queue, (void **)&removedItem);
    if (result == QUEUE_DATA_NOT_FOUND_ERROR)
    {
        printf("TestQueueRemoveEmptyQueue: PASS\n");
    }
    else
    {
        printf("TestQueueRemoveEmptyQueue: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

/* QueueIsEmpty Tests */

static void TestQueueIsEmptyNullQueue()
{
    size_t result = QueueIsEmpty(NULL);
    if (result == QUEUE_UNINITIALIZED_ERROR)
    {
        printf("TestQueueIsEmptyNullQueue: PASS\n");
    }
    else
    {
        printf("TestQueueIsEmptyNullQueue: FAIL\n");
    }
}

static void TestQueueIsEmptyActuallyEmpty()
{
    Queue *queue = QueueCreate(5);
    size_t result = QueueIsEmpty(queue);
    if (result == 1)
    {
        printf("TestQueueIsEmptyActuallyEmpty: PASS\n");
    }
    else
    {
        printf("TestQueueIsEmptyActuallyEmpty: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueIsEmptyActuallyNotEmpty()
{
    size_t result;
    int item = 42;
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &item);

    result = QueueIsEmpty(queue);
    if (result == 0)
    {
        printf("TestQueueIsEmptyActuallyNotEmpty: PASS\n");
    }
    else
    {
        printf("TestQueueIsEmptyActuallyNotEmpty: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

/* QueueForEach Tests */

static void TestQueueForEachNullQueueOrAction()
{
    Queue *queue;
    size_t result = QueueForEach(NULL, DumbAction, NULL);
    if (result == QUEUE_UNINITIALIZED_ERROR)
    {
        printf("TestQueueForEachNullQueueOrAction (NULL queue): PASS\n");
    }
    else
    {
        printf("TestQueueForEachNullQueueOrAction (NULL queue): FAIL\n");
    }

    queue = QueueCreate(5);
    result = QueueForEach(queue, NULL, NULL);
    if (result == QUEUE_DATA_UNINITIALIZED_ERROR)
    {
        printf("TestQueueForEachNullQueueOrAction (NULL action): PASS\n");
    }
    else
    {
        printf("TestQueueForEachNullQueueOrAction (NULL action): FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueForEachValidRun()
{
    size_t result;
    int sum = 0;
    int items[] = {1, 2, 3};
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &items[0]);
    QueueInsert(queue, &items[1]);
    QueueInsert(queue, &items[2]);

    result = QueueForEach(queue, SumAction, &sum);
    if (result == 3 && sum == 6)
    {
        printf("TestQueueForEachValidRun: PASS\n");
    }
    else
    {
        printf("TestQueueForEachValidRun: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

static void TestQueueForEachNullContext()
{
    size_t result;
    int items[] = {1, 2};
    Queue *queue = QueueCreate(5);
    QueueInsert(queue, &items[0]);
    QueueInsert(queue, &items[1]);
    result = QueueForEach(queue, DumbAction, NULL);
    if (result == 2)
    {
        printf("TestQueueForEachNullContext: PASS\n");
    }
    else
    {
        printf("TestQueueForEachNullContext: FAIL\n");
    }
    QueueDestroy(&queue, NULL);
}

/* Helper functions */

static int DumbAction(void *element, void *context)
{
    return 1;
}

static int SumAction(void *element, void *context)
{
    int *sum = (int *)context;
    *sum += *(int *)element;
    return 1;
}
