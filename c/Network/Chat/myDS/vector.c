#include <stdlib.h>
#include "vector.h"

struct Vector {
    void** m_items;
    size_t m_originalSize;
    size_t m_size;
    size_t m_nItems;
    size_t m_blockSize;
};

/* Creates a new vector with given initial capacity and block size for resizing */
Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
    Vector* vector;   
    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }   
    vector = (Vector*)malloc(sizeof(Vector));
    if (!vector)
    {
        return NULL;
    }    
    vector->m_items = (void**)malloc(_initialCapacity * sizeof(void*));
    if (!vector->m_items) 
    {
        free(vector);
        return NULL;
    }    
    vector->m_originalSize = _initialCapacity;
    vector->m_size = _initialCapacity;
    vector->m_nItems = 0;
    vector->m_blockSize = _blockSize;   
    return vector;
}

/* Frees the vector and optionally destroys all elements using provided function */
void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
    size_t i;
    
    if (!_vector || !*_vector || !_elementDestroy) {
        return;
    }
    
    if (_elementDestroy)
     {
        for (i = 0; i < (*_vector)->m_nItems; ++i)
         {
            _elementDestroy((*_vector)->m_items[i]);
         }
    }   
    free((*_vector)->m_items);
    free(*_vector);
    *_vector = NULL;
}

/* Increases vector capacity by block size when full */
static VectorResult VectorGrow(Vector* _vector)
{
    void** temp;
    size_t newSize;      
    newSize = _vector->m_size + _vector->m_blockSize;
    temp = (void**)realloc(_vector->m_items, newSize * sizeof(void*));
    if (!temp) 
    {
        return VECTOR_ALLOCATION_ERROR;
    }    
    _vector->m_items = temp;
    _vector->m_size = newSize;
    return VECTOR_SUCCESS;
}

/* Decreases vector capacity by block size when too much unused space */
static VectorResult VectorShrink(Vector* _vector)
{
    void** temp;
    size_t newSize;    
    if (_vector->m_blockSize == 0 || 
        _vector->m_size <= _vector->m_originalSize) {
        return VECTOR_SUCCESS;
    }   
    newSize = _vector->m_size - _vector->m_blockSize;
    temp = (void**)realloc(_vector->m_items, newSize * sizeof(void*));   
    _vector->m_items = temp;
    _vector->m_size = newSize;
    return VECTOR_SUCCESS;
}

/* Adds new element to end of vector, growing if necessary */
VectorResult VectorAppend(Vector* _vector, void* _item)
{
    if (!_vector || !_item) 
    {
        return VECTOR_UNITIALIZED_ERROR;
    }  
    if (_vector->m_nItems == _vector->m_size) 
    {
        if (_vector->m_blockSize == 0)
        {
            return VECTOR_OVERFLOW_ERROR;
        }
        if (VectorGrow(_vector) != VECTOR_SUCCESS)
        {   
            return VECTOR_ALLOCATION_ERROR;
        }
    }    
    _vector->m_items[_vector->m_nItems++] = _item;
    return VECTOR_SUCCESS;
}

/* Removes and returns last element from vector, shrinking if necessary */
VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
    if (!_vector || !_pValue )
    {
        return VECTOR_UNITIALIZED_ERROR;
    }    
    if (_vector->m_nItems == 0)
    {
        return VECTOR_UNDERFLOW_ERROR;
    }   
    *_pValue = _vector->m_items[--_vector->m_nItems];
    
    if (_vector->m_size - _vector->m_nItems  >=  2 * _vector->m_blockSize)
    {
        VectorShrink(_vector);
    }   
    return VECTOR_SUCCESS;
}

/* Retrieves element at specified index without removing it */
VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue)
{
    if (!_vector || !_pValue )
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    
    if (_index >= _vector->m_nItems)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    *_pValue = _vector->m_items[_index];
    return VECTOR_SUCCESS;
}

/* Updates element at specified index with new value */
VectorResult VectorSet(Vector* _vector, size_t _index, void* _value)
{
    if (!_vector || !_value)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }  
    if (_index >= _vector->m_nItems) 
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    } 
    _vector->m_items[_index] = _value;
    return VECTOR_SUCCESS;
}

/* Returns current number of elements in vector */
size_t VectorSize(const Vector* _vector)
{    
    return (_vector) ? _vector->m_nItems : 0;
}

/* Returns current total capacity of vector */
size_t VectorCapacity(const Vector* _vector)
{
    return (_vector) ? _vector->m_size : 0;
}

/* Performs given action on each element until completion or action returns 0 */
size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
    size_t i;   
    if (!_vector || !_action) 
    {
        return 0;
    }    
    for (i = 0; i < _vector->m_nItems; ++i)
     {
        if (_action(_vector->m_items[i], i, _context) == 0)
        {
            break;
        }
    }   
    return i;
}
/* Gets the actual allocated size of the vector */
size_t VectorGetActualSize(const Vector* _vector)
{
    return (_vector) ? _vector->m_size : 0;
}

/* Gets the original size of the vector */
size_t VectorGetOriginalSize(const Vector* _vector)
{
    return (_vector) ? _vector->m_originalSize : 0;
}

/* Gets the block size of the vector */
size_t VectorGetBlockSize(const Vector* _vector)
{
    return (_vector) ? _vector->m_blockSize : 0;
}
