#include "dynamicVector.h"
#include <stdio.h>
#include <stdlib.h>

struct Vector
{
    void **m_items;
    size_t m_originalSize; /* original allocated space for items */
    size_t m_size;         /* actual allocated space for items*/
    size_t m_nItems;       /* actual number of items */
    size_t m_blockSize;    /* the chunk size to be allocated when no space*/
};

/* Action function for VectorForEach - Print Integer */
int PrintIntegers(void *_element, size_t _index, void *_context)
{
    printf("%d ", *(int *)_element);
    return 1;
}

/* Action function for VectorForEach - Search Integer */
int SearchIntegerElem(void *_element, size_t _index, void *_context)
{
    return (*((int *)_element) == *((int *)_context)) ? 0 : 1;
}

/* Test function for VectorCreate with valid parameters */
void TestVectorCreate_ValidParams()
{
    Vector *vector = VectorCreate(10, 5);
    if (vector != NULL && vector->m_blockSize == 5 && vector->m_size == 10 && vector->m_originalSize == 10)
    {
        printf("TestVectorCreate_ValidParams: pass\n");
    }
    else
    {
        printf("TestVectorCreate_ValidParams: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCreate with zero initial capacity and block size */
void TestVectorCreate_ZeroInitialCapacityZeroBlockSize()
{
    Vector *vector = VectorCreate(0, 0);
    if (vector == NULL)
    {
        printf("TestVectorCreate_ZeroInitialCapacityZeroBlockSize: pass\n");
    }
    else
    {
        printf("TestVectorCreate_ZeroInitialCapacityZeroBlockSize: fail\n");
        VectorDestroy(&vector, NULL);
    }
}

/* Test function for VectorAppend with a valid item */
void TestVectorAppend_ValidItem()
{
    Vector *vector = VectorCreate(2, 2);
    int item = 42;
    VectorResult result = VectorAppend(vector, &item);
    if ((result == VECTOR_SUCCESS) && (*((int *)vector->m_items[0]) == item))
    {
        printf("TestVectorAppend_ValidItem: pass\n");
    }
    else
    {
        printf("TestVectorAppend_ValidItem: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorAppend with a NULL vector */
void TestVectorAppend_NullVector()
{
    int item = 42;
    if (VectorAppend(NULL, &item) == VECTOR_UNITIALIZED_ERROR)
    {
        printf("TestVectorAppend_NullVector: pass\n");
    }
    else
    {
        printf("TestVectorAppend_NullVector: fail\n");
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

    result = VectorGet(vector, 3, (void **)&retrievedItem);
    if (result == VECTOR_SUCCESS && vector->m_nItems == 3 && vector->m_size == vector->m_originalSize + vector->m_blockSize)
    {
        printf("Test VectorAppend_Growth: pass\n");
    }
    else
    {
        printf("Test VectorAppend_Growth: fail\n");
    }
}

/* Test function for VectorRemove with valid conditions */
void TestVectorRemove_Valid()
{
    int *removedItem;
    int item = 42;
    size_t prevNItems;
    VectorResult result;
    Vector *vector = VectorCreate(2, 2);

    VectorAppend(vector, &item);
    prevNItems = vector->m_nItems;
    result = VectorRemove(vector, (void **)&removedItem);
    if (result == VECTOR_SUCCESS && removedItem == &item && vector->m_nItems == prevNItems - 1)
    {
        printf("Test VectorRemove_Valid: pass\n");
    }
    else
    {
        printf("Test VectorRemove_Valid: fail\n");
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

    if (VectorRemove(vector, (void **)&removedItem) == VECTOR_SUCCESS && vector->m_size == 10)
    {
        for (index = 0; index < 4; index++)
        {
            VectorRemove(vector, (void **)&removedItem);
        }

        if (vector->m_nItems == vector->m_blockSize * 2 && vector->m_size == 10)
        {
            result = VectorRemove(vector, (void **)&removedItem);
            if (result == VECTOR_SUCCESS && vector->m_nItems == 5 && vector->m_size == 6)
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

/* Test function for VectorGet with a valid index */
void TestVectorGet_ValidIndex()
{
    int *retrievedItem;
    int item = 42;
    Vector *vector = VectorCreate(2, 2);
    VectorAppend(vector, &item);

    if (VectorGet(vector, 0, (void **)&retrievedItem) == VECTOR_SUCCESS && retrievedItem == &item)
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

    if (VectorGet(vector, 0, NULL) == VECTOR_UNITIALIZED_ERROR)
    {
        printf("TestVectorGet_InvalidIndex: pass\n");
    }
    else
    {
        printf("TestVectorGet_InvalidIndex: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

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
        if (retrievedItem == &item2)
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

/* Test function for VectorSize */
void TestVectorSize()
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
                printf("Test VectorSize: pass\n");
            }
        }
    }
    else
    {
        printf("TestVectorSize: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorCapacity */
void TestVectorCapacity()
{
    Vector *vector = VectorCreate(2, 2);
    if (VectorCapacity(vector) == 2)
    {
        printf("Test VectorCapacity: pass\n");
    }
    else
    {
        printf("Test VectorCapacity: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Test function for VectorForEach */
void TestVectorForEach()
{
    size_t index;
    int* retrievedItem;
    Vector *vector = VectorCreate(2, 2);
    int item1 = 1, item2 = 2, item3 = 3;
    VectorAppend(vector, &item1);
    VectorAppend(vector, &item2);
    VectorAppend(vector, &item3);

    index = VectorForEach(vector, SearchIntegerElem, &item2);
    VectorGet(vector, index, (void **)&retrievedItem);
    
    if (index == 1 && *retrievedItem == item2)
    {
        printf("Test VectorForEach: pass\n");
    }
    else
    {
        printf("Test VectorForEach: fail\n");
    }
    VectorDestroy(&vector, NULL);
}

/* Main function to run all tests */
int main()
{
    TestVectorCreate_ValidParams();
    TestVectorCreate_ZeroInitialCapacityZeroBlockSize();
    TestVectorAppend_ValidItem();
    TestVectorAppend_NullVector();
    TestVectorRemove_Valid();
    TestVectorRemove_Underflow();
    TestVectorGet_ValidIndex();
    TestVectorGet_InvalidIndex();
    TestVectorSet_ValidIndex();
    TestVectorSet_InvalidIndex();
    TestVectorSize();
    TestVectorCapacity();
    TestVectorForEach();
    return 0;
}
