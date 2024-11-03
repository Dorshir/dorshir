#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/* StackCreate Tests */
void TestStackCreate_InvalidCapacityAndBlockSize();
void TestStackCreate_NegativeInitialCapacity();
void TestStackCreate_NegativeBlockSize();
void TestStackCreate_ZeroInitialCapacity_NonZeroBlockSize();
void TestStackCreate_NonZeroInitialCapacity_ZeroBlockSize();
void TestStackCreate_ValidParameters();

/* StackDestroy Tests */
void TestStackDestroy_ValidStack();
void TestStackDestroy_DoubleDestroy();
void TestStackDestroy_NullElementDestroy();

/* StackPush Tests */
void TestStackPush_ValidPush();
void TestStackPush_NullStack();
void TestStackPush_NullItem();
void TestStackPush_Overflow();
void TestStackPush_GrowthAllocationFail();
void TestStackPush_GrowthSuccess();

/* StackPop Tests */
void TestStackPop_ValidPop();
void TestStackPop_NullStack();
void TestStackPop_NullPValue();
void TestStackPop_EmptyStack();

/* StackTop Tests */
void TestStackTop_ValidTop();
void TestStackTop_NullStack();
void TestStackTop_NullPValue();
void TestStackTop_EmptyStack();

/* StackSize Tests */
void TestStackSize_ValidSize();
void TestStackSize_NullStack();

/* StackCapacity Tests */
void TestStackCapacity_ValidCapacity();
void TestStackCapacity_NullStack();

/* StackIsEmpty Tests */
void TestStackIsEmpty_NullStack();
void TestStackIsEmpty_ValidScenario();

/* StackPrint Tests */
void TestStackPrint_NullStackOrAction();
void TestStackPrint_ValidRun();
void TestStackPrint_NullContext();

int main()
{
    TestStackCreate_InvalidCapacityAndBlockSize();
    TestStackCreate_NegativeInitialCapacity();
    TestStackCreate_NegativeBlockSize();
    TestStackCreate_ZeroInitialCapacity_NonZeroBlockSize();
    TestStackCreate_NonZeroInitialCapacity_ZeroBlockSize();
    TestStackCreate_ValidParameters();

    TestStackDestroy_ValidStack();
    TestStackDestroy_DoubleDestroy();
    TestStackDestroy_NullElementDestroy();

    TestStackPush_ValidPush();
    TestStackPush_NullStack();
    TestStackPush_NullItem();
    TestStackPush_Overflow();
    TestStackPush_GrowthAllocationFail();
    TestStackPush_GrowthSuccess();

    TestStackPop_ValidPop();
    TestStackPop_NullStack();
    TestStackPop_NullPValue();
    TestStackPop_EmptyStack();

    TestStackTop_ValidTop();
    TestStackTop_NullStack();
    TestStackTop_NullPValue();
    TestStackTop_EmptyStack();

    TestStackSize_ValidSize();
    TestStackSize_NullStack();

    TestStackCapacity_ValidCapacity();
    TestStackCapacity_NullStack();

    TestStackIsEmpty_NullStack();
    TestStackIsEmpty_ValidScenario();

    TestStackPrint_NullStackOrAction();
    TestStackPrint_ValidRun();
    TestStackPrint_NullContext();

    return 0;
}

/* Helper functions for element action */

int PrintElement(void *_element, size_t _index, void *_context)
{
    return 1;
}

int SearchIntegerElem(void *_element, size_t _index, void *_context)
{
    if (_context != NULL)
    {
        return (*((int *)_element) == *((int *)_context)) ? 0 : 1;
    }
    else
    {
        return 1;
    }
}

/* StackCreate Tests */

void TestStackCreate_InvalidCapacityAndBlockSize()
{
    Stack *stack = StackCreate(0, 0);
    if (stack == NULL)
    {
        printf("TestStackCreate_InvalidCapacityAndBlockSize: Pass\n");
    }
    else
    {
        printf("TestStackCreate_InvalidCapacityAndBlockSize: Fail\n");
        StackDestroy(&stack, NULL);
    }
}

void TestStackCreate_NegativeInitialCapacity()
{
    /* -1 -> fail allocation */
    Stack *stack = StackCreate(-1, 0);
    if (stack == NULL)
    {
        printf("TestStackCreate_NegativeInitialCapacity: Pass\n");
    }
    else
    {
        printf("TestStackCreate_NegativeInitialCapacity: Fail\n");
        StackDestroy(&stack, NULL);
    }
}

void TestStackCreate_NegativeBlockSize()
{
    Stack *stack = StackCreate(5, (size_t)-1);
    if (stack != NULL)
    {
        printf("TestStackCreate_NegativeBlockSize: Pass\n");
        StackDestroy(&stack, NULL);
    }
    else
    {
        printf("TestStackCreate_NegativeBlockSize: Fail\n");
    }
}

void TestStackCreate_ZeroInitialCapacity_NonZeroBlockSize()
{
    Stack *stack = StackCreate(0, 5);
    if (stack != NULL)
    {
        printf("TestStackCreate_ZeroInitialCapacity_NonZeroBlockSize: Pass\n");
        StackDestroy(&stack, NULL);
    }
    else
    {
        printf("TestStackCreate_ZeroInitialCapacity_NonZeroBlockSize: Fail\n");
    }
}

void TestStackCreate_NonZeroInitialCapacity_ZeroBlockSize()
{
    Stack *stack = StackCreate(5, 0);
    if (stack != NULL)
    {
        printf("TestStackCreate_NonZeroInitialCapacity_ZeroBlockSize: Pass\n");
        StackDestroy(&stack, NULL);
    }
    else
    {
        printf("TestStackCreate_NonZeroInitialCapacity_ZeroBlockSize: Fail\n");
    }
}

void TestStackCreate_ValidParameters()
{
    Stack *stack = StackCreate(5, 5);
    if (stack != NULL)
    {
        printf("TestStackCreate_ValidParameters: Pass\n");
        StackDestroy(&stack, NULL);
    }
    else
    {
        printf("TestStackCreate_ValidParameters: Fail\n");
    }
}

/* StackDestroy Tests */

void TestStackDestroy_ValidStack()
{
    Stack *stack = StackCreate(5, 5);
    StackDestroy(&stack, free);
    if (stack == NULL)
    {
        printf("TestStackDestroy_ValidStack: Pass\n");
    }
    else
    {
        printf("TestStackDestroy_ValidStack: Fail\n");
    }
}

void TestStackDestroy_DoubleDestroy()
{
    Stack *stack = StackCreate(5, 5);
    StackDestroy(&stack, free);
    StackDestroy(&stack, free);
    if (stack == NULL)
    {
        printf("TestStackDestroy_DoubleDestroy: Pass\n");
    }
    else
    {
        printf("TestStackDestroy_DoubleDestroy: Fail\n");
    }
}

void TestStackDestroy_NullElementDestroy()
{
    Stack *stack = StackCreate(5, 5);
    StackDestroy(&stack, NULL);
    if (stack == NULL)
    {
        printf("TestStackDestroy_NullElementDestroy: Pass\n");
    }
    else
    {
        printf("TestStackDestroy_NullElementDestroy: Fail\n");
    }
}

/* StackPush Tests */

void TestStackPush_ValidPush()
{
    void *topItem = NULL;
    int item = 10;
    Stack *stack = StackCreate(5, 5);
    StackResult res = StackPush(stack, &item);
    if (res == STACK_SUCCESS)
    {
        StackTop(stack, &topItem);
        if (topItem == &item && StackSize(stack) == 1)
        {
            printf("TestStackPush_ValidPush: Pass\n");
        }
        else
        {
            printf("TestStackPush_ValidPush: Fail\n");
        }
    }
    else
    {
        printf("TestStackPush_ValidPush: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPush_NullStack()
{
    int item = 10;
    StackResult res = StackPush(NULL, &item);
    if (res == STACK_UNITIALIZED_ERROR)
    {
        printf("TestStackPush_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackPush_NullStack: Fail\n");
    }
}

void TestStackPush_NullItem()
{
    Stack *stack = StackCreate(5, 5);
    StackResult res = StackPush(stack, NULL);
    if (res == STACK_NULL_PTR_ERROR)
    {
        printf("TestStackPush_NullItem: Pass\n");
    }
    else
    {
        printf("TestStackPush_NullItem: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPush_Overflow()
{
    int item = 10;
    StackResult res;
    Stack *stack = StackCreate(2, 0);
    StackPush(stack, &item);
    StackPush(stack, &item);

    res = StackPush(stack, &item);
    if (res == STACK_OVERFLOW)
    {
        printf("TestStackPush_Overflow: Pass\n");
    }
    else
    {
        printf("TestStackPush_Overflow: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPush_GrowthAllocationFail()
{
    int item = 10;
    StackResult res;
    Stack *stack = StackCreate(2, -3);
    StackPush(stack, &item);
    StackPush(stack, &item);

    res = StackPush(stack, &item);
    if (res == STACK_ALLOCATION_ERROR)
    {
        printf("TestStackPush_GrowthAllocationFail: Pass\n");
    }
    else
    {
        printf("TestStackPush_GrowthAllocationFail: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPush_GrowthSuccess()
{
    int item = 10;
    StackResult res;
    Stack *stack = StackCreate(2, 2);

    StackPush(stack, &item);
    StackPush(stack, &item);

    res = StackPush(stack, &item);
    if (res == STACK_SUCCESS && StackSize(stack) == 3 && StackCapacity(stack) == 4)
    {
        printf("TestStackPush_GrowthSuccess: Pass\n");
    }
    else
    {
        printf("TestStackPush_GrowthSuccess: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

/* StackPop Tests */

void TestStackPop_ValidPop()
{
    StackResult res;
    void *poppedItem = NULL;
    int item1 = 10, item2 = 20;
    Stack *stack = StackCreate(5, 5);

    StackPush(stack, &item1);
    StackPush(stack, &item2);

    res = StackPop(stack, &poppedItem);
    if (res == STACK_SUCCESS && poppedItem == &item2 && StackSize(stack) == 1)
    {
        printf("TestStackPop_ValidPop: Pass\n");
    }
    else
    {
        printf("TestStackPop_ValidPop: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPop_NullStack()
{
    void *poppedItem = NULL;
    StackResult res = StackPop(NULL, &poppedItem);
    if (res == STACK_UNITIALIZED_ERROR)
    {
        printf("TestStackPop_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackPop_NullStack: Fail\n");
    }
}

void TestStackPop_NullPValue()
{
    Stack *stack = StackCreate(5, 5);
    StackResult res = StackPop(stack, NULL);
    if (res == STACK_NULL_PTR_ERROR)
    {
        printf("TestStackPop_NullPValue: Pass\n");
    }
    else
    {
        printf("TestStackPop_NullPValue: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPop_EmptyStack()
{
    void *poppedItem = NULL;
    Stack *stack = StackCreate(5, 5);

    StackResult res = StackPop(stack, &poppedItem);
    if (res == STACK_IS_EMPTY)
    {
        printf("TestStackPop_EmptyStack: Pass\n");
    }
    else
    {
        printf("TestStackPop_EmptyStack: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

/* StackTop Tests */

void TestStackTop_ValidTop()
{
    StackResult res;
    void *topItem = NULL;
    int item1 = 10;
    Stack *stack = StackCreate(5, 5);
    StackPush(stack, &item1);

    res = StackTop(stack, &topItem);
    if (res == STACK_SUCCESS && topItem == &item1 && StackSize(stack) == 1)
    {
        printf("TestStackTop_ValidTop: Pass\n");
    }
    else
    {
        printf("TestStackTop_ValidTop: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackTop_NullStack()
{
    void *topItem = NULL;
    StackResult res = StackTop(NULL, &topItem);
    if (res == STACK_UNITIALIZED_ERROR)
    {
        printf("TestStackTop_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackTop_NullStack: Fail\n");
    }
}

void TestStackTop_NullPValue()
{
    Stack *stack = StackCreate(5, 5);
    StackResult res = StackTop(stack, NULL);
    if (res == STACK_NULL_PTR_ERROR)
    {
        printf("TestStackTop_NullPValue: Pass\n");
    }
    else
    {
        printf("TestStackTop_NullPValue: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackTop_EmptyStack()
{
    void *topItem = NULL;
    Stack *stack = StackCreate(5, 5);

    StackResult res = StackTop(stack, &topItem);
    if (res == STACK_IS_EMPTY)
    {
        printf("TestStackTop_EmptyStack: Pass\n");
    }
    else
    {
        printf("TestStackTop_EmptyStack: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

/* StackSize Tests */

void TestStackSize_ValidSize()
{
    Stack *stack = StackCreate(5, 5);
    int *ret;
    int item1 = 10, item2 = 20;
    StackPush(stack, &item1);
    StackPush(stack, &item2);
    if (StackSize(stack) == 2)
    {
        StackPop(stack, (void **)&ret);
        if (StackSize(stack) == 1)
        {
            printf("TestStackSize_ValidSize: Pass\n");
        }
        else
        {
            printf("TestStackSize_ValidSize: Fail\n");
        }
    }
    else
    {
        printf("TestStackSize_ValidSize: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackSize_NullStack()
{
    if (StackSize(NULL) == 0)
    {
        printf("TestStackSize_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackSize_NullStack: Fail\n");
    }
}

/* StackCapacity Tests */

void TestStackCapacity_ValidCapacity()
{
    Stack *stack = StackCreate(5, 5);
    if (StackCapacity(stack) == 5)
    {
        printf("TestStackCapacity_ValidCapacity: Pass\n");
    }
    else
    {
        printf("TestStackCapacity_ValidCapacity: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackCapacity_NullStack()
{
    if (StackCapacity(NULL) == 0)
    {
        printf("TestStackCapacity_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackCapacity_NullStack: Fail\n");
    }
}

/* StackIsEmpty Tests */

void TestStackIsEmpty_NullStack()
{
    if (StackIsEmpty(NULL) == 1)
    {
        printf("TestStackIsEmpty_NullStack: Pass\n");
    }
    else
    {
        printf("TestStackIsEmpty_NullStack: Fail\n");
    }
}

void TestStackIsEmpty_ValidScenario()
{
    int item = 10;
    Stack *stack = StackCreate(5, 5);

    if (StackIsEmpty(stack) == 1)
    {
        StackPush(stack, &item);
        if (StackIsEmpty(stack) == 0)
        {
            printf("TestStackIsEmpty_ValidScenario: Pass\n");
        }
        else
        {
            printf("TestStackIsEmpty_ValidScenario: Fail\n");
        }
    }
    else
    {
        printf("TestStackIsEmpty_ValidScenario: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

/* StackPrint Tests */

void TestStackPrint_NullStackOrAction()
{
    Stack *stack = StackCreate(5, 5);
    size_t result = StackPrint(NULL, PrintElement, NULL);
    if (result == 0)
    {
        result = StackPrint(stack, NULL, NULL);
        if (result == 0)
        {
            printf("TestStackPrint_NullStackOrAction: Pass\n");
        }
        else
        {
            printf("TestStackPrint_NullStackOrAction: Fail\n");
        }
    }
    else
    {
        printf("TestStackPrint_NullStackOrAction: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPrint_ValidRun()
{
    size_t index;
    int item1 = 10, item2 = 20;
    Stack *stack = StackCreate(5, 5);

    StackPush(stack, &item1);
    StackPush(stack, &item2);
    index = StackPrint(stack, PrintElement, NULL);
    if (index == StackSize(stack))
    {
        printf("TestStackPrint_ValidRun: Pass\n");
    }
    else
    {
        printf("TestStackPrint_ValidRun: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPrint_NullContext()
{
    size_t index;
    int item1 = 10;
    Stack *stack = StackCreate(5, 5);

    StackPush(stack, &item1);
    StackPush(stack, &item1);
    StackPush(stack, &item1);

    index = StackPrint(stack, PrintElement, NULL);
    if (index == StackSize(stack))
    {
        printf("TestStackPrint_NullContext: Pass\n");
    }
    else
    {
        printf("TestStackPrint_NullContext: Fail\n");
    }
    StackDestroy(&stack, NULL);
}

void TestStackPrint_BreakTrigger()
{
    size_t index;
    int item1 = 1, item2 = 2, item3 = 3;
    Stack *stack = StackCreate(5, 5);

    StackPush(stack, &item1);
    StackPush(stack, &item2);
    StackPush(stack, &item3);

    index = StackPrint(stack, SearchIntegerElem, &item2);
    if (index == StackSize(stack) - 1)
    {
        printf("TestStackPrint_BreakTrigger: Pass\n");
    }
    else
    {
        printf("TestStackPrint_BreakTrigger: Fail\n");
    }
    StackDestroy(&stack, NULL);
}
