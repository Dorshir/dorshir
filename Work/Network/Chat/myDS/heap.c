#include <stdlib.h>
#include "heap.h"
#include "vector.h"

#define HEAP_PARENT(i) (((i) - 1) / 2)
#define HEAP_LEFT(i) (2 * (i) + 1)
#define HEAP_RIGHT(i) (2 * (i) + 2)

#define VECTOR_GET(vec, index, out) VectorGet((vec), (index), (out))
#define VECTOR_SET(vec, index, value) VectorSet((vec), (index), (value))
#define VECTOR_SIZE(vec) VectorSize((vec))

struct Heap {
    Vector* m_vec;
    size_t m_heapSize;
    LessThanComparator _pfLess;
};

/* Function declarations for static functions */
static void SwapElements(Vector* _vec, size_t _i, size_t _j);
static int DefaultComparator(const void* _left, const void* _right);
static void HeapifyDown(Heap* _heap, size_t _index);
static void HeapifyUp(Heap* _heap, size_t _index);

/* Main functions */

Heap* HeapBuild(Vector* _vector, LessThanComparator _pfLess) 
{
    Heap* newHeap;
    size_t i;
    if (!_vector) 
    {  
        return NULL;
    }    
    newHeap = (Heap*)malloc(sizeof(Heap));
    if (!newHeap)
     {
        return NULL;
    }
    newHeap->m_vec = _vector;
    newHeap->m_heapSize = VECTOR_SIZE(_vector);
    newHeap->_pfLess = _pfLess ? _pfLess : DefaultComparator;
    for (i = (newHeap->m_heapSize / 2); i > 0; --i) 
    {
        HeapifyDown(newHeap, i - 1);/* --- */
    }
    return newHeap;
}

Vector* HeapDestroy(Heap** _heap) 
{
    Vector* vector;
    if (!_heap || !*_heap) 
    {
        return NULL;
    }
    vector = (*_heap)->m_vec;
    free(*_heap);
    *_heap = NULL;
    return vector;
}

HeapResultCode HeapInsert(Heap* _heap, void* _element) 
{
    size_t index;
    if (!_heap || !_element)
    {
        return HEAP_NOT_INITIALIZED;
    }
    if (VectorAppend(_heap->m_vec, _element) != VECTOR_SUCCESS) 
    {
        return HEAP_REALLOCATION_FAILED;
    }
    index = _heap->m_heapSize++;
    HeapifyUp(_heap, index);
    return HEAP_SUCCESS;
}

const void* HeapPeek(const Heap* _heap) 
{
    void* root;
    if (!_heap || _heap->m_heapSize == 0) 
    {
        return NULL;
    }    
    VECTOR_GET(_heap->m_vec, 0, &root);
    return root;
}

void* HeapExtract(Heap* _heap) {
    const void* peekedItem;
    void* root;
    void* lastItem;
    size_t lastIndex;
    
    if (!_heap || _heap->m_heapSize == 0) {
        return NULL;
    }
    if (VectorGet(_heap->m_vec, 0, &root) != VECTOR_SUCCESS) {
        return NULL;
    }
    if (_heap->m_heapSize == 1) {
        _heap->m_heapSize--;
        VectorRemove(_heap->m_vec, &lastItem);
        return root;
    }
    lastIndex = _heap->m_heapSize - 1;
    if (VectorGet(_heap->m_vec, lastIndex, &lastItem) != VECTOR_SUCCESS) 
    {
        return NULL;
    }
    if (VectorSet(_heap->m_vec, 0, lastItem) != VECTOR_SUCCESS) {
        return NULL;
    }
    VectorRemove(_heap->m_vec, &lastItem);
    _heap->m_heapSize--;
    HeapifyDown(_heap, 0);
    return root;
}

size_t HeapSize(const Heap* _heap) {
    return (_heap) ? _heap->m_heapSize : 0;
}

size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context) 
{
    size_t i;
    void* element;
    if (!_heap || !_act)
    {
        return 0;
    }
    for (i = 0; i < _heap->m_heapSize; ++i) 
    {
        VECTOR_GET(_heap->m_vec, i, &element);
        if (_act(element, _context) == 0) 
        {
            break;
        }
    }
    return i;
}

/* Static helper function implementations */

static void SwapElements(Vector* _vec, size_t _i, size_t _j) 
{
    void* temp;
    void* elementJ;
    VECTOR_GET(_vec, _i, &temp);
    VECTOR_GET(_vec, _j, &elementJ);
    VECTOR_SET(_vec, _i, elementJ);
    VECTOR_SET(_vec, _j, temp);
}

static int DefaultComparator(const void* _left, const void* _right) 
{
    return (_left < _right) ? -1 : (_left > _right);
}

static void HeapifyDown(Heap* _heap, size_t _index) 
{
   size_t left, right, largest;
   void *element, *leftElem, *rightElem, *largestElem;
   if (!_heap || _index >= _heap->m_heapSize) 
   {
       return;
   }
   if (VectorGet(_heap->m_vec, _index, &element) != VECTOR_SUCCESS)
   {
       return;
   }

   while (1) 
   {
       left = HEAP_LEFT(_index);
       right = HEAP_RIGHT(_index);
       largest = _index;
       if (left < _heap->m_heapSize) 
       {
           if (VectorGet(_heap->m_vec, left, &leftElem) == VECTOR_SUCCESS && 
               VectorGet(_heap->m_vec, largest, &largestElem) == VECTOR_SUCCESS)
           {
               if (_heap->_pfLess(largestElem, leftElem) < 0) 
               {
                   largest = left;
               }
           }
       }

       if (right < _heap->m_heapSize) 
       {
           if (VectorGet(_heap->m_vec, right, &rightElem) == VECTOR_SUCCESS && 
               VectorGet(_heap->m_vec, largest, &largestElem) == VECTOR_SUCCESS)
           {
               if (_heap->_pfLess(largestElem, rightElem) < 0) 
               {
                   largest = right;
               }
           }
       }
       if (largest == _index) 
       {
           break;
       }
       SwapElements(_heap->m_vec, _index, largest);
       _index = largest;
   }
}

static void HeapifyUp(Heap* _heap, size_t _index) 
{
    void* element;
    void* parentElem;
    size_t parentIndex;
    VECTOR_GET(_heap->m_vec, _index, &element);
    while (_index > 0) {
        parentIndex = HEAP_PARENT(_index);
        VECTOR_GET(_heap->m_vec, parentIndex, &parentElem);

        if (_heap->_pfLess(element, parentElem) >= 0) 
        {
            break;
        }
        SwapElements(_heap->m_vec, _index, parentIndex);
        _index = parentIndex;
    }
}

