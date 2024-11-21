#include "dynamicVector.h"
#include <stdio.h>
#include <stdlib.h>
size_t GetBlockSize(Vector *_vector);
size_t GetSize(Vector *_vector);
size_t GetNItems(Vector *_vector);
size_t GetOriginalSize(Vector *_vector);
void **GetItems(Vector *_vector);

/* Action function for VectorForEach - Print Integer */
int PrintIntegers(void *_element, size_t _index, void *_context)
{
    printf("%d ", *(int *)_element);
    return 1;
}

/* Action function for VectorForEach - Search Integer */
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

/******************************** VectorCreate ********************************/

/* Test function for VectorCreate with valid parameters */
void TestVectorCreate_ValidParams()
{
    Vector *vector = VectorCreate(10, 5);
    if (vector != NULL && GetBlockSize(vector) == 5 && GetSize(vector) == 10 && GetOriginalSize(vector) == 10)
    {
        printf("Test VectorCreate_ValidParams: pass\n");
    }
    else
    {
        printf("Test VectorCreate_ValidParams: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with invalid parameters */
void TestVectorCreate_InvalidAlloc()
{
    Vector *vector = VectorCreate(-1, 0);
    if (vector == NULL)
    {
        printf("Test VectorCreate_InvalidAlloc: pass\n");
    }
    else
    {
        printf("Test VectorCreate_InvalidAlloc: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with zero initial capacity BUT valid block size */
void TestVectorCreate_ZeroInitialCapacityValidBlockSize()
{
    Vector *vector = VectorCreate(0, 4);
    if (vector != NULL && GetNItems(vector) == 0 && GetBlockSize(vector) == 4 && GetSize(vector) == 0 && GetOriginalSize(vector) == 0)
    {
        printf("Test VectorCreate_ZeroInitialCapacityValidBlockSize: pass\n");
    }
    else
    {
        printf("Test VectorCreate_ZeroInitialCapacityValidBlockSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with valid initial capacity BUT zero block size */
void TestVectorCreate_ValidInitialCapacityZeroBlockSize()
{
    Vector *vector = VectorCreate(4, 0);
    if (vector != NULL && GetNItems(vector) == 0 && GetSize(vector) == 4 && GetBlockSize(vector) == 0 && GetOriginalSize(vector) == 4)
    {
        printf("Test VectorCreate_ValidInitialCapacityZeroBlockSize: pass\n");
    }
    else
    {
        printf("Test VectorCreate_ValidInitialCapacityZeroBlockSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with zero initial capacity and block size */
void TestVectorCreate_ZeroInitialCapacityZeroBlockSize()
{
    Vector *vector = VectorCreate(0, 0);
    if (vector == NULL)
    {
        printf("Test VectorCreate_ZeroInitialCapacityZeroBlockSize: pass\n");
    }
    else
    {
        printf("Test VectorCreate_ZeroInitialCapacityZeroBlockSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with < 0 initial capacity and valid block size */
void TestVectorCreate_LessThanZeroInitialCapacityValidBlockSize()
{
    Vector *vector = VectorCreate(-5, 0);
    if (vector == NULL)
    {
        printf("Test VectorCreate_LessThanZeroInitialCapacityValidBlockSize: pass\n");
    }
    else
    {
        printf("Test VectorCreate_LessThanZeroInitialCapacityValidBlockSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with valid initial capacity and < 0 block size */
void TestVectorCreate_ValidInitialCapacityLessThanZeroBlockSize()
{
    Vector *vector = VectorCreate(0, -5);
    if (vector != NULL)
    {
        printf("Test VectorCreate_ValidInitialCapacityLessThanZeroBlockSize: pass\n");
    }
    else
    {
        printf("Test VectorCreate_ValidInitialCapacityLessThanZeroBlockSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/******************************** VectorDestroy ********************************/

/* Test function for VectorDesroy with valid vector pointer */
void TestVectorDestroy_ValidVectorPointer()
{
    Vector *vector = VectorCreate(2, 2);
    if (vector != NULL)
    {
        VectorDestroy(&vector, NULL);
        if (vector == NULL)
        {
            printf("Test VectorDestroy_ValidVectorPointer: pass\n");
        }
        else
        {
            printf("Test VectorDestroy_ValidVectorPointer: fail\n");
        }
    }
    else
    {
        printf("Test VectorDestroy_ValidVectorPointer: fail - Create Failed\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorDesroy with double destory calls */
void TestVectorDestroy_DoubleDestroySameVector()
{
    Vector *vector = VectorCreate(2, 2);
    if (vector != NULL)
    {
        VectorDestroy(&vector, free);
        if (vector == NULL)
        {
            VectorDestroy(&vector, free);
            if (vector == NULL)
            {
                printf("Test VectorDestroy_DoubleDestroySameVector: pass\n");
            }
            else
            {
                printf("Test VectorDestroy_DoubleDestroySameVector: fail\n");
            }
        }
        else
        {
            printf("Test VectorDestroy_DoubleDestroySameVector: fail - First Destroy Failed\n");
        }
    }
    else
    {
        printf("Test TestVectorDestroy_DoubleDestroySameVector: fail - Create Failed\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorDesroy with _elementDestroy as NULL */
void TestVectorDestroy_DestroyVectorWith_elementDestroyAsNULL()
{
    Vector *vector = VectorCreate(2, 2);
    if (vector != NULL)
    {
        VectorDestroy(&vector, NULL);
        if (vector == NULL)
        {
            printf("Test VectorDestroy_DestroyVectorWith_elementDestroyAsNULL: pass\n");
        }
        else
        {
            printf("Test VectorDestroy_DestroyVectorWith_elementDestroyAsNULL: fail\n");
        }
    }
    else
    {
        printf("Test VectorDestroy_DestroyVectorWith_elementDestroyAsNULL: fail - Create Failed\n");
    }
    VectorDestroy(&vector, NULL);
}

/******************************** VectorAppend ********************************/

/* Test function for VectorAppend with a valid item */
void TestVectorAppend_ValidItem()
{
    Vector *vector = VectorCreate(2, 2);
    int item = 42;
    VectorResult result = VectorAppend(vector, &item);
    if ((result == VECTOR_SUCCESS) && (*((int *)GetItems(vector)[0]) == item))
    {
        printf("Test VectorAppend_ValidItem: pass\n");
    }
    else
    {
        printf("Test VectorAppend_ValidItem: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorAppend with a NULL vector */
void TestVectorAppend_NullVector()
{
    int item = 42;
    if (VectorAppend(NULL, &item) == VECTOR_UNITIALIZED_ERROR)
    {
        printf("Test VectorAppend_NullVector: pass\n");
    }
    else
    {
        printf("Test VectorAppend_NullVector: fail\n");
    }
}

/* Test function for VectorAppend with a NULL item */
void TestVectorAppend_NullItem()
{
    Vector *vector = VectorCreate(2, 2);
    if (VectorAppend(vector, NULL) == NULL_PTR_ERROR)
    {
        printf("Test VectorAppend_NullItem: pass\n");
    }
    else
    {
        printf("Test VectorAppend_NullItem: fail\n");
    }
}

/* Test function for VectorAppend with overflow */
void TestVectorAppend_Overflow()
{
    Vector *vector = VectorCreate(2, 0);
    int item = 42;
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);

    if (VectorAppend(vector, &item) == VECTOR_OVERFLOW && GetNItems(vector) == 2 && GetSize(vector) == 2)
    {
        printf("Test VectorAppend_Overflow: pass\n");
    }
    else
    {
        printf("Test VectorAppend_Overflow: fail\n");
    }
}

/* Test function for VectorAppend with need to growth but allocation fail */
void TestVectorAppend_GrowthAllocationFail()
{
    int item = 42;
    int *retrievedItem;
    Vector *vector = VectorCreate(4, -5);
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);

    if (VectorAppend(vector, &item) == VECTOR_ALLOCATION_ERROR && GetNItems(vector) == 4 && GetSize(vector) == 4)
    {
        printf("Test VectorAppend_GrowthAllocationFail: pass\n");
    }
    else
    {
        printf("Test VectorAppend_GrowthAllocationFail: fail\n");
    }
}

/* Test function for VectorAppend with growth */
void TestVectorAppend_Growth()
{
    int item = 42;
    int *retrievedItem;
    VectorResult result;
    Vector *vector = VectorCreate(2, 2);
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);
    VectorAppend(vector, &item);

    result = VectorGet(vector, 2, (void **)&retrievedItem);
    if (result == VECTOR_SUCCESS && GetNItems(vector) == 3 && GetSize(vector) == GetOriginalSize(vector) + GetBlockSize(vector))
    {
        printf("Test VectorAppend_Growth: pass\n");
    }
    else
    {
        printf("Test VectorAppend_Growth: fail\n");
    }
}

/******************************** VectorRemove ********************************/

/* Test function for VectorRemove with valid conditions */
void TestVectorRemove_Valid()
{
    int *removedItem;
    int item = 42;
    size_t prevNItems;
    VectorResult result;
    Vector *vector = VectorCreate(2, 2);

    VectorAppend(vector, &item);
    prevNItems = GetNItems(vector);
    result = VectorRemove(vector, (void **)&removedItem);
    if (result == VECTOR_SUCCESS && *removedItem == item && GetNItems(vector) == prevNItems - 1)
    {
        printf("Test VectorRemove_Valid: pass\n");
    }
    else
    {
        printf("Test VectorRemove_Valid: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorRemove with null pointer as input vector */
void TestVectorRemove_NullVector()
{
    int *removedItem;
    VectorRemove(NULL, (void **)&removedItem);
    if (VectorRemove(NULL, (void **)&removedItem) == VECTOR_UNITIALIZED_ERROR)
    {
        printf("Test VectorRemove_NullVector: pass\n");
    }
    else
    {
        printf("Test VectorRemove_NullVector: fail\n");
    }
}

/* Test function for VectorRemove with null as a pointer for the retreive value */
void TestVectorRemove_NullRetr()
{
    Vector *vector = VectorCreate(2, 2);

    if (VectorRemove(vector, NULL) == NULL_PTR_ERROR)
    {
        printf("Test VectorRemove_NullRetr: pass\n");
    }
    else
    {
        printf("Test VectorRemove_NullRetr: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorRemove under underflow condition */
void TestVectorRemove_Underflow()
{
    int *removedItem;
    Vector *vector = VectorCreate(2, 2);
    if (VectorRemove(vector, (void **)&removedItem) == VECTOR_UNDERFLOW)
    {
        printf("Test VectorRemove_Underflow: pass\n");
    }
    else
    {
        printf("Test VectorRemove_Underflow: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorRemove with shrink */
void TestVectorRemove_Shrink()
{
    int *removedItem;
    int index;
    int item = 42;
    VectorResult result;
    Vector *vector = VectorCreate(2, 2);

    for (index = 0; index < 10; index++)
    {
        VectorAppend(vector, &item);
    }

    if (GetSize(vector) == 10)
    {
        for (index = 0; index < 3; index++)
        {
            VectorRemove(vector, (void **)&removedItem);
        }

        if (GetNItems(vector) == 7 && GetSize(vector) == 10)
        {
            result = VectorRemove(vector, (void **)&removedItem);
            if (result == VECTOR_SUCCESS && GetNItems(vector) == 6 && GetSize(vector) == 8)
            {

                printf("Test VectorRemove_Shrink: pass\n");
            }
            else
            {
                printf("Test VectorRemove_Shrink: fail\n");
            }
        }
        else
        {
            printf("Test VectorRemove_Shrink: fail\n");
        }
    }
    else
    {
        printf("Test VectorRemove_Shrink: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorRemove with no need to shrink due to under init size */
void TestVectorRemove_ShouldNotShrinkUnderOriginalSize()
{
    int *removedItem;
    int index;
    int item = 42;
    size_t initSize = 8;
    VectorResult result;
    Vector *vector = VectorCreate(initSize, 2);

    for (index = 0; index < initSize; index++)
    {
        VectorAppend(vector, &item);
    }

    for (index = 0; index < 6; index++)
    {
        VectorRemove(vector, (void **)&removedItem);
    }

    if (GetNItems(vector) == 2 && GetSize(vector) == initSize)
    {
        printf("Test VectorRemove_ShouldNotShrinkUnderOriginalSize: pass\n");
    }
    else
    {
        printf("Test VectorRemove_ShouldNotShrinkUnderOriginalSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorRemove with no need to shrink due to less than 2 chunks free */
void TestVectorRemove_ShouldNotShrinkLessThan2ChunksFree()
{
    int *removedItem;
    int index;
    int item = 42;
    size_t initBockSize = 2;
    size_t initSize = 8;
    VectorResult result;
    Vector *vector = VectorCreate(initSize, initBockSize);

    for (index = 0; index < initSize; index++)
    {
        VectorAppend(vector, &item);
    }

    for (index = 0; index < initBockSize * 2 - 1; index++)
    {
        VectorRemove(vector, (void **)&removedItem);
    }

    if (GetSize(vector) == initSize)
    {
        printf("Test VectorRemove_ShouldNotShrinkLessThan2ChunksFree: pass\n");
    }
    else
    {
        printf("Test VectorRemove_ShouldNotShrinkLessThan2ChunksFree: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/******************************** VectorGet ********************************/

/* Test function for VectorGet with a valid index */
void TestVectorGet_ValidIndex()
{
    int *retrievedItem;
    int item = 42;
    Vector *vector = VectorCreate(2, 2);
    VectorAppend(vector, &item);

    if (VectorGet(vector, 0, (void **)&retrievedItem) == VECTOR_SUCCESS && *retrievedItem == item)
    {
        printf("Test VectorGet_ValidIndex: pass\n");
    }
    else
    {
        printf("Test VectorGet_ValidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorGet with an invalid index */
void TestVectorGet_InvalidIndex()
{
    int *retrievedItem;
    int item = 42;
    Vector *vector = VectorCreate(2, 2);
    VectorAppend(vector, &item);

    if (VectorGet(vector, 1, (void **)&retrievedItem) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
    {
        printf("Test VectorGet_InvalidIndex: pass\n");
    }
    else
    {
        printf("Test VectorGet_InvalidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorGet with a null pointer as a retriever */
void TestVectorGet_NullRetriever()
{
    Vector *vector = VectorCreate(2, 2);
    int item = 42;
    VectorAppend(vector, &item);

    if (VectorGet(vector, 0, NULL) == NULL_PTR_ERROR)
    {
        printf("Test VectorGet_InvalidIndex: pass\n");
    }
    else
    {
        printf("Test VectorGet_InvalidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/******************************** VectorSet ********************************/

/* Test function for VectorSet with a valid index */
void TestVectorSet_ValidIndex()
{
    Vector *vector = VectorCreate(2, 2);
    int item1 = 42;
    int item2 = 24;
    int *retrievedItem;
    VectorAppend(vector, &item1);
    if (VectorSet(vector, 0, &item2) == VECTOR_SUCCESS)
    {
        VectorGet(vector, 0, (void **)&retrievedItem);
        if (*retrievedItem == item2)
        {
            printf("Test VectorSet_ValidIndex: pass\n");
        }
        else
        {
            printf("Test VectorSet_ValidIndex: fail\n");
        }
    }
    else
    {
        printf("Test VectorSet_ValidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorSet with a valid index BUT null -pValue */
void TestVectorSet_ValidIndexButInvalidSetPointer()
{
    Vector *vector = VectorCreate(2, 2);
    int item1 = 42;
    int *retrievedItem;
    VectorAppend(vector, &item1);
    if (VectorSet(vector, 0, NULL) == NULL_PTR_ERROR)
    {
        printf("Test VectorSet_ValidIndexButInvalidSetPointer: pass\n");
    }
    else
    {
        printf("Test VectorSet_ValidIndexButInvalidSetPointer: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorSet with an invalid index */
void TestVectorSet_InvalidIndex()
{
    Vector *vector = VectorCreate(2, 2);
    int item = 42;
    if (VectorSet(vector, 0, &item) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
    {
        printf("Test VectorSet_InvalidIndex: pass\n");
    }
    else
    {
        printf("Test VectorSet_InvalidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorSet with uninitialized vector */
void TestVectorSet_UniniVector()
{
    int item2 = 54;
    int *retrievedItem;
    if (VectorSet(NULL, 0, &item2) == VECTOR_UNITIALIZED_ERROR)
    {
        printf("Test VectorSet_UniniVector: pass\n");
    }
    else
    {
        printf("Test VectorSet_UniniVector: fail\n");
    }
}

/******************************** VectorSize ********************************/

/* Test function for VectorSize with valid parameters */
void TestVectorSize_ValidParam()
{
    Vector *vector = VectorCreate(2, 2);
    int item = 42;
    int *removedItem;
    VectorAppend(vector, &item);

    if (VectorSize(vector) == 1)
    {
        VectorRemove(vector, (void **)&removedItem);
        if (VectorSize(vector) == 0)
        {
            VectorRemove(vector, (void **)&removedItem);
            if (VectorSize(vector) == 0)
            {
                printf("Test VectorSize_ValidParam: pass\n");
            }
        }
    }
    else
    {
        printf("Test VectorSize_ValidParam: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorSize with invalid parameters */
void TestVectorSize_InvalidParam()
{
    if (VectorSize(NULL) == 0)
    {
        printf("Test VectorSize_ValidParam: pass\n");
    }
    else
    {
        printf("Test VectorSize_ValidParam: fail\n");
    }
}

/******************************** VectorCapacity ********************************/

/* Test function for VectorCapacity with valid parameters */
void TestVectorCapacity_ValidParams()
{
    size_t index;
    int *retrievedItem;
    int item = 55;
    Vector *vector = VectorCreate(2, 2);
    if (VectorCapacity(vector) == 2)
    {
        VectorAppend(vector, &item);
        VectorAppend(vector, &item);
        VectorAppend(vector, &item);
        if (VectorCapacity(vector) == 4)
        {
            for (index = 0; index < 7; index++)
            {
                VectorAppend(vector, &item);
            }
            if (GetSize(vector) == 10)
            {
                for (index = 0; index < 5; index++)
                {
                    VectorRemove(vector, (void **)&retrievedItem);
                }
                if (GetSize(vector) == 8)
                {
                    printf("Test VectorCapacity_ValidParams: pass\n");
                }
                else
                {
                    printf("Test VectorCapacity_ValidParams: fail\n");
                }
            }
        }
        else
        {
            printf("Test VectorCapacity_ValidParams: fail - append\n");
        }
    }
    else
    {
        printf("Test VectorCapacity_ValidParams: fail - create\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCapacity with invalid parameters */
void TestVectorCapacity_InvalidParams()
{
    if (VectorCapacity(NULL) == 0)
    {
        printf("Test VectorCapacity_InvalidParams: pass\n");
    }
    else
    {
        printf("Test VectorCapacity_InvalidParams: fail\n");
    }
}

/******************************** VectorForEach ********************************/

/* Test function for VectorForEach with _action that should break the loop */
void TestVectorForEach_ActionWithBreak()
{
    size_t index;
    int *retrievedItem;
    Vector *vector = VectorCreate(2, 2);
    int item1 = 1, item2 = 2, item3 = 3;
    VectorAppend(vector, &item1);
    VectorAppend(vector, &item2);
    VectorAppend(vector, &item3);

    index = VectorForEach(vector, SearchIntegerElem, &item2);
    VectorGet(vector, index, (void **)&retrievedItem);

    if (index == 1 && *retrievedItem == item2)
    {
        printf("Test VectorForEach_ActionWithBreak: pass\n");
    }
    else
    {
        printf("Test VectorForEach_ActionWithBreak: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorForEach with _action that should NOT break the loop */
void TestVectorForEach_ActionWithNoBreak()
{
    size_t index;
    Vector *vector = VectorCreate(2, 2);
    int item1 = 1, item2 = 2, item3 = 3;
    VectorAppend(vector, &item1);
    VectorAppend(vector, &item2);

    index = VectorForEach(vector, SearchIntegerElem, &item3);

    if (index == GetNItems(vector))
    {
        printf("Test VectorForEach_ActionWithNoBreak: pass\n");
    }
    else
    {
        printf("Test VectorForEach_ActionWithNoBreak: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorForEach with valid _action and _context as null */
void TestVectorForEach_ActionWithUninitContext()
{
    size_t index;
    Vector *vector = VectorCreate(2, 2);
    int item1 = 1, item2 = 2;
    VectorAppend(vector, &item1);
    VectorAppend(vector, &item2);

    index = VectorForEach(vector, SearchIntegerElem, NULL);

    if (index == GetNItems(vector))
    {
        printf("Test VectorForEach_ActionWithUninitContext: pass\n");
    }
    else
    {
        printf("Test VectorForEach_ActionWithUninitContext: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorForEach with invalid params */
void TestVectorForEach_InvalidParams()
{
    size_t index1, index2;
    Vector *vector = VectorCreate(2, 2);
    int item1 = 1, item2 = 2;
    VectorAppend(vector, &item1);
    VectorAppend(vector, &item2);

    index1 = VectorForEach(vector, NULL, NULL);
    index2 = VectorForEach(NULL, SearchIntegerElem, NULL);

    if (index1 == 0 && index2 == 0)
    {
        printf("Test VectorForEach_InvalidParams: pass\n");
    }
    else
    {
        printf("Test VectorForEach_InvalidParams: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Main function to run all tests */
int main()
{
    TestVectorCreate_ValidParams();
    TestVectorCreate_InvalidAlloc();
    TestVectorCreate_ZeroInitialCapacityZeroBlockSize();
    TestVectorCreate_ZeroInitialCapacityValidBlockSize();
    TestVectorCreate_ValidInitialCapacityZeroBlockSize();
    TestVectorCreate_LessThanZeroInitialCapacityValidBlockSize();
    TestVectorCreate_ValidInitialCapacityLessThanZeroBlockSize();
    TestVectorDestroy_ValidVectorPointer();
    TestVectorDestroy_DoubleDestroySameVector();
    TestVectorDestroy_DestroyVectorWith_elementDestroyAsNULL();
    TestVectorAppend_ValidItem();
    TestVectorAppend_NullVector();
    TestVectorAppend_NullItem();
    TestVectorAppend_Overflow();
    TestVectorAppend_GrowthAllocationFail();
    TestVectorAppend_Growth();
    TestVectorRemove_Valid();
    TestVectorRemove_NullVector();
    TestVectorRemove_NullRetr();
    TestVectorRemove_Underflow();
    TestVectorRemove_Shrink();
    TestVectorRemove_ShouldNotShrinkUnderOriginalSize();
    TestVectorRemove_ShouldNotShrinkLessThan2ChunksFree();
    TestVectorGet_ValidIndex();
    TestVectorGet_InvalidIndex();
    TestVectorGet_NullRetriever();
    TestVectorSet_ValidIndex();
    TestVectorSet_ValidIndexButInvalidSetPointer();
    TestVectorSet_InvalidIndex();
    TestVectorSet_UniniVector();
    TestVectorSize_ValidParam();
    TestVectorSize_InvalidParam();
    TestVectorCapacity_ValidParams();
    TestVectorCapacity_InvalidParams();
    TestVectorForEach_ActionWithBreak();
    TestVectorForEach_ActionWithNoBreak();
    TestVectorForEach_ActionWithUninitContext();
    TestVectorForEach_InvalidParams();
    return 0;
}
