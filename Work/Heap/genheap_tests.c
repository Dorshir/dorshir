#include "genheap.h"
#include "genvec.h"
#include <stdio.h>
#include <stdlib.h>

Vector *GetVec(Heap *_heap);
size_t GetHeapSize(Heap *_heap);
LessThanComparator GetComparator(Heap *_heap);

static int IntComparator(const void *_left, const void *_right);
static int *CreateInt(int _value);
static int PrintItem(const void *_elem, void *_context);

/* HeapBuild Tests */
static void TestHeapBuildValid();
static void TestHeapBuildInvalidLessFunc();

/* HeapDestroy Tests */
static void TestHeapDestroyValid();
static void TestHeapDestroyNullHeap();
static void TestHeapDestroyDoubleDestroy();

/* HeapInsert Tests */
static void TestHeapInsertValid();
static void TestHeapInsertNullHeap();
static void TestHeapInsertNullElement();
static void TestHeapInsertIntoEmptyHeap();

/* HeapPeek Tests */
static void TestHeapPeekValid();
static void TestHeapPeekNullHeap();
static void TestHeapPeekEmptyHeap();

/* HeapExtract Tests */
static void TestHeapExtractValid();
static void TestHeapExtractNullHeap();
static void TestHeapExtractEmptyHeap();

/* HeapSize Tests */
static void TestHeapSizeValid();
static void TestHeapSizeNullHeap();

/* HeapForEach Tests */
static void TestHeapForEachValid();
static void TestHeapForEachNullHeap();
static void TestHeapForEachNullActionFunction();

int main()
{
    /* HeapBuild Tests */
    TestHeapBuildValid();
    TestHeapBuildInvalidLessFunc();

    /* HeapDestroy Tests */
    TestHeapDestroyValid();
    TestHeapDestroyNullHeap();
    TestHeapDestroyDoubleDestroy();

    /* HeapInsert Tests */
    TestHeapInsertValid();
    TestHeapInsertNullHeap();
    TestHeapInsertNullElement();
    TestHeapInsertIntoEmptyHeap();

    /* HeapPeek Tests */
    TestHeapPeekValid();
    TestHeapPeekNullHeap();
    TestHeapPeekEmptyHeap();

    /* HeapExtract Tests */
    TestHeapExtractValid();
    TestHeapExtractNullHeap();
    TestHeapExtractEmptyHeap();

    /* HeapSize Tests */
    TestHeapSizeValid();
    TestHeapSizeNullHeap();

    /* HeapForEach Tests */
    TestHeapForEachValid();
    TestHeapForEachNullHeap();
    TestHeapForEachNullActionFunction();

    return 0;
}

static int IntComparator(const void *_left, const void *_right)
{
    if (_left == NULL || _right == NULL)
    {
        return 0;
    }
    return *(int *)_left < *(int *)_right;
}

static int *CreateInt(int _value)
{
    int *ptr = malloc(sizeof(int));
    if (ptr == NULL)
    {
        return NULL;
    }
    *ptr = _value;
    return ptr;
}

static int PrintItem(const void *_elem, void *_context)
{
    if (_elem == NULL)
    {
        return 0;
    }
    return 1;
}

/* HeapBuild Tests */
static void TestHeapBuildValid()
{
    printf("TestHeapBuildValid: ");
    int *item;
    Vector *vec = VectorCreate(10, 5);
    int *val1 = CreateInt(20);
    int *val2 = CreateInt(15);
    int *val3 = CreateInt(30);
    VectorAppend(vec, val1);
    VectorAppend(vec, val2);
    VectorAppend(vec, val3);

    Heap *heap = HeapBuild(vec, IntComparator);
    VectorGet(vec, 0, (void **)&item);
    if (heap != NULL && HeapSize(heap) == 3 && *item == *val2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    VectorDestroy(&vec, free);
    HeapDestroy(&heap);
}

static void TestHeapBuildInvalidLessFunc()
{
    printf("TestHeapBuildInvalidLessFunc: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, NULL);
    if (heap == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        HeapDestroy(&heap);
    }
    VectorDestroy(&vec, free);
}

/* HeapDestroy Tests */
static void TestHeapDestroyValid()
{
    printf("TestHeapDestroyValid: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    HeapDestroy(&heap);
    if (heap == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    VectorDestroy(&vec, free);
}

static void TestHeapDestroyNullHeap()
{
    printf("TestHeapDestroyNullHeap: ");
    HeapDestroy(NULL);
    printf("PASS\n");
}

static void TestHeapDestroyDoubleDestroy()
{
    printf("TestHeapDestroyDoubleDestroy: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    HeapDestroy(&heap);
    HeapDestroy(&heap);
    printf("PASS\n");
    VectorDestroy(&vec, free);
}

/* HeapInsert Tests */
static void TestHeapInsertValid()
{
    printf("TestHeapInsertValid: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    int *val = CreateInt(25);
    HeapResultCode res = HeapInsert(heap, val);
    if (res == HEAP_SUCCESS && HeapSize(heap) == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapInsertNullHeap()
{
    printf("TestHeapInsertNullHeap: ");
    int *val = CreateInt(35);
    HeapResultCode res = HeapInsert(NULL, val);
    if (res == HEAP_NOT_INITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    free(val);
}

static void TestHeapInsertNullElement()
{
    printf("TestHeapInsertNullElement: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    HeapResultCode res = HeapInsert(heap, NULL);
    if (res == DATA_NOT_INITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapInsertIntoEmptyHeap()
{
    printf("TestHeapInsertIntoEmptyHeap: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    int *val = CreateInt(45);
    HeapResultCode res = HeapInsert(heap, val);
    if (res == HEAP_SUCCESS && HeapSize(heap) == 1 && *(int *)HeapPeek(heap) == 45)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

/* HeapPeek Tests */
static void TestHeapPeekValid()
{
    printf("TestHeapPeekValid: ");
    Vector *vec = VectorCreate(10, 5);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(40);
    int *val3 = CreateInt(60);
    VectorAppend(vec, val1);
    VectorAppend(vec, val2);
    VectorAppend(vec, val3);
    Heap *heap = HeapBuild(vec, IntComparator);
    int *peeked = (int *)HeapPeek(heap);
    if (peeked != NULL && *peeked == 40)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapPeekNullHeap()
{
    printf("TestHeapPeekNullHeap: ");
    const void *peeked = HeapPeek(NULL);
    if (peeked == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestHeapPeekEmptyHeap()
{
    printf("TestHeapPeekEmptyHeap: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    const void *peeked = HeapPeek(heap);
    if (peeked == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

/* HeapExtract Tests */
static void TestHeapExtractValid()
{
    printf("TestHeapExtractValid: ");
    Vector *vec = VectorCreate(10, 5);
    int *val1 = CreateInt(70);
    int *val2 = CreateInt(50);
    int *val3 = CreateInt(60);
    VectorAppend(vec, val1);
    VectorAppend(vec, val2);
    VectorAppend(vec, val3);
    Heap *heap = HeapBuild(vec, IntComparator);
    int *extracted = (int *)HeapExtract(heap);
    if (extracted != NULL && *extracted == 50 && HeapSize(heap) == 2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    free(extracted);
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapExtractNullHeap()
{
    printf("TestHeapExtractNullHeap: ");
    void *extracted = HeapExtract(NULL);
    if (extracted == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestHeapExtractEmptyHeap()
{
    printf("TestHeapExtractEmptyHeap: ");
    Vector *vec = VectorCreate(10, 5);
    Heap *heap = HeapBuild(vec, IntComparator);
    void *extracted = HeapExtract(heap);
    if (extracted == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        free(extracted);
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

/* HeapSize Tests */
static void TestHeapSizeValid()
{
    printf("TestHeapSizeValid: ");
    Vector *vec = VectorCreate(10, 5);
    int *val1 = CreateInt(80);
    int *val2 = CreateInt(90);
    VectorAppend(vec, val1);
    VectorAppend(vec, val2);
    Heap *heap = HeapBuild(vec, IntComparator);
    size_t size = HeapSize(heap);
    if (size == 2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapSizeNullHeap()
{
    printf("TestHeapSizeNullHeap: ");
    size_t size = HeapSize(NULL);
    if (size == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* HeapForEach Tests */
static void TestHeapForEachValid()
{
    printf("TestHeapForEachValid: ");
    Vector *vec = VectorCreate(10, 5);
    int *val1 = CreateInt(100);
    int *val2 = CreateInt(110);
    int *val3 = CreateInt(90);
    VectorAppend(vec, val1);
    VectorAppend(vec, val2);
    VectorAppend(vec, val3);
    Heap *heap = HeapBuild(vec, IntComparator);
    size_t count = HeapForEach(heap, PrintItem, NULL);
    if (count == GetHeapSize(heap))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}

static void TestHeapForEachNullHeap()
{
    printf("TestHeapForEachNullHeap: ");
    size_t count = HeapForEach(NULL, PrintItem, NULL);
    if (count == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestHeapForEachNullActionFunction()
{
    printf("TestHeapForEachNullActionFunction: ");
    Vector *vec = VectorCreate(10, 5);
    int *val = CreateInt(120);
    VectorAppend(vec, val);
    Heap *heap = HeapBuild(vec, IntComparator);
    size_t count = HeapForEach(heap, NULL, NULL);
    if (count == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HeapDestroy(&heap);
    VectorDestroy(&vec, free);
}
