#include "genheap.h"
#include <stdlib.h>

#define LAST_INSERTED_INDEX(heap) (heap->m_heapSize - 1)
#define GET_LAST_INSERTED_ITEM(heap, lastInserted) (VectorGet(heap->m_vec, LAST_INSERTED_INDEX(heap), &lastInserted))
#define FATHER(index) ((index - 1) / 2)
#define LEFT_CHILD(index) (2 * (index) + 1)
#define RIGHT_CHILD(index) (2 * (index) + 2)
#define HAS_RIGHT_CHILD(index, heap) (RIGHT_CHILD(index) < heap->m_heapSize)
#define HAS_LEFT_CHILD(index, heap) (LEFT_CHILD(index) < heap->m_heapSize)
#define RIGHT 1
#define LEFT 2

struct Heap
{
    Vector *m_vec;
    size_t m_heapSize;
    LessThanComparator _pfLess;
};

static void SwapData(Heap *_heap, size_t _index1, size_t _index2);
static void BubbleUp(Heap *_heap);
static void FixAsHeap(Heap *_heap);
static int FindYoungestFather(Heap *_heap);
static void Heapify(Heap *_heap, size_t _index);
static int GetNextPosition(void *_leftData, void *_rightData, LessThanComparator _pfLess, void **_comparedData);

/****************** API Functions ******************/

Heap *HeapBuild(Vector *_vector, LessThanComparator _pfLess)
{
    Heap *newHeap;
    if (_vector == NULL || _pfLess == NULL)
    {
        return NULL;
    }
    newHeap = malloc(sizeof(Heap));
    if (newHeap == NULL)
    {
        return NULL;
    }
    newHeap->m_heapSize = VectorSize(_vector);
    newHeap->m_vec = _vector;
    newHeap->_pfLess = _pfLess;

    FixAsHeap(newHeap);

    return newHeap;
}

Vector *HeapDestroy(Heap **_heap)
{
    Vector *retVec;
    if (_heap == NULL || *_heap == NULL)
    {
        return NULL;
    }

    retVec = (*_heap)->m_vec;
    free(*_heap);
    *_heap = NULL;

    return retVec;
}

HeapResultCode HeapInsert(Heap *_heap, void *_element)
{
    if (_heap == NULL)
    {
        return HEAP_NOT_INITIALIZED;
    }
    else if (_element == NULL)
    {
        return DATA_NOT_INITIALIZED;
    }

    if (VectorAppend(_heap->m_vec, _element) != VECTOR_SUCCESS)
    {
        return HEAP_REALLOCATION_FAILED;
    }
    ++(_heap->m_heapSize);

    BubbleUp(_heap);

    return HEAP_SUCCESS;
}

const void *HeapPeek(const Heap *_heap)
{
    void *_pValue;

    if (_heap == NULL)
    {
        return NULL;
    }
    if (VectorGet(_heap->m_vec, 0, &_pValue) == VECTOR_SUCCESS)
    {
        return _pValue;
    }

    return NULL;
}

void *HeapExtract(Heap *_heap)
{
    void *extracted;
    if (_heap == NULL || _heap->m_heapSize == 0)
    {
        return NULL;
    }

    SwapData(_heap, 0, LAST_INSERTED_INDEX(_heap));
    VectorRemove(_heap->m_vec, &extracted);
    --(_heap->m_heapSize);

    Heapify(_heap, 0);

    return extracted;
}

size_t HeapSize(const Heap *_heap)
{
    if (_heap == NULL)
    {
        return 0;
    }
    return _heap->m_heapSize;
}

size_t HeapForEach(const Heap *_heap, ActionFunction _act, void *_context)
{
    size_t index;
    void *currData;
    if (_heap == NULL || _act == NULL)
    {
        return 0;
    }
    for (index = 0; index < _heap->m_heapSize; index++)
    {
        VectorGet(_heap->m_vec, index, &currData);
        if (_act(currData, _context) == 0)
        {
            break;
        }
    }
    return index;
}

/****************** Help Functions ******************/

static void SwapData(Heap *_heap, size_t _index1, size_t _index2)
{
    void *temp1, *temp2;
    VectorGet(_heap->m_vec, _index1, &temp1);
    VectorGet(_heap->m_vec, _index2, &temp2);
    VectorSet(_heap->m_vec, _index1, temp2);
    VectorSet(_heap->m_vec, _index2, temp1);
}

static void BubbleUp(Heap *_heap)
{
    void *currData, *fatherData;
    size_t curr = LAST_INSERTED_INDEX(_heap);
    if (curr == 0)
    {
        return;
    }
    VectorGet(_heap->m_vec, curr, &currData);
    while (curr > 0)
    {
        VectorGet(_heap->m_vec, FATHER(curr), &fatherData);
        if (!_heap->_pfLess(currData, fatherData))
        {
            break;
        }
        SwapData(_heap, curr, FATHER(curr));
        curr = FATHER(curr);
        // currData = fatherData;
    }
}

static int FindYoungestFather(Heap *_heap)
{
    if (_heap->m_heapSize <= 1)
    {
        return 0;
    }
    return FATHER(LAST_INSERTED_INDEX(_heap));
}

static void Heapify(Heap *_heap, size_t _currIndex)
{
    void *leftData = NULL, *rightData = NULL, *currData, *comparedData;
    int direction;
    size_t nextIndex;
    VectorGet(_heap->m_vec, _currIndex, &currData);

    if (HAS_RIGHT_CHILD(_currIndex, _heap))
    {
        VectorGet(_heap->m_vec, RIGHT_CHILD(_currIndex), &rightData);
    }
    if (HAS_LEFT_CHILD(_currIndex, _heap))
    {
        VectorGet(_heap->m_vec, LEFT_CHILD(_currIndex), &leftData);
    }
    else
    {
        /* LEAF */
        return;
    }
    direction = GetNextPosition(leftData, rightData, _heap->_pfLess, &comparedData);
    nextIndex = (direction == LEFT) ? LEFT_CHILD(_currIndex) : RIGHT_CHILD(_currIndex);
    if (_heap->_pfLess(comparedData, currData))
    {
        SwapData(_heap, _currIndex, nextIndex);
        Heapify(_heap, nextIndex);
    }
}

static int GetNextPosition(void *_leftData, void *_rightData, LessThanComparator _pfLess, void **_comparedData)
{
    if (_rightData != NULL && _pfLess(_rightData, _leftData))
    {
        *_comparedData = _rightData;
        return RIGHT;
    }
    *_comparedData = _leftData;
    return LEFT;
}

static void FixAsHeap(Heap *_heap)
{
    /* check the change to size_t instead of int, if not working, consider run to >0 and handle 0 after while */
    int index = FindYoungestFather(_heap);

    while (index >= 0)
    {
        Heapify(_heap, index);
        index--;
    }
}

/****************** Getters ******************/

Vector *GetVec(Heap *_heap)
{
    return _heap->m_vec;
}
size_t GetHeapSize(Heap *_heap)
{
    return _heap->m_heapSize;
}
LessThanComparator GetComparator(Heap *_heap)
{
    return _heap->_pfLess;
}
