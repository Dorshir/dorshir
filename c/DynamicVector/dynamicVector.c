#include "dynamicVector.h"
#include <stdlib.h> /* Dynamic Memory */

#define IS_VECTOR_FULL(vector) ((vector)->m_nItems == (vector)->m_size)
#define NUM_OF_EMPTY_SPACES(vector) ((vector)->m_size - (vector)->m_nItems)
#define TWO_CHUNKS(vector) ((vector)->m_blockSize * 2)
#define NEW_SIZE(vector) ((vector)->m_size - (vector)->m_blockSize)
#define SHOULD_SHRINK(vector) ((NUM_OF_EMPTY_SPACES(vector) == TWO_CHUNKS(vector)) && (NEW_SIZE(vector) >= (vector)->m_originalSize))

struct Vector
{
    void **m_items;
    size_t m_originalSize; /* original allocated space for items */
    size_t m_size;         /* actual allocated space for items*/
    size_t m_nItems;       /* actual number of items */
    size_t m_blockSize;    /* the chunk size to be allocated when no space*/
};

VectorResult IsValidInputRemove(Vector *_vector, void **_pValue);
VectorResult IsValidInputGetSet(const Vector *_vector, size_t _index, void **_pValue);
void ShrinkVector(Vector *_vector);
VectorResult ReallocItems(Vector *_vector);

/* Main Functions */

VectorResult VectorGet(const Vector *_vector, size_t _index, void **_pValue)
{
    VectorResult result = IsValidInputGetSet(_vector, _index, _pValue);
    if (result == VECTOR_SUCCESS)
    {
        *_pValue = _vector->m_items[_index];
    }
    return result;
}

VectorResult VectorSet(Vector *_vector, size_t _index, void *_value)
{
    VectorResult result = IsValidInputGetSet(_vector, _index, _value);
    if (result == VECTOR_SUCCESS)
    {
        _vector->m_items[_index] = _value;
    }
    return result;
}

size_t VectorSize(const Vector *_vector)
{
    if (_vector == NULL)
    {
        return 0;
    }
    return _vector->m_nItems;
}

size_t VectorCapacity(const Vector *_vector)
{
    if (_vector == NULL)
    {
        return 0;
    }
    return _vector->m_size;
}

size_t VectorForEach(const Vector *_vector, VectorElementAction _action, void *_context)
{
    size_t index;
    if (_vector == NULL || _action == NULL)
    {
        return 0;
    }
    for (index = 0; index < _vector->m_nItems; index++)
    {
        void *element = _vector->m_items[index];
        if (_action(element, index, _context) == 0)
        {
            break;
        }
    }
    return index;
}

VectorResult VectorRemove(Vector *_vector, void **_pValue)
{
    VectorResult result = IsValidInputRemove(_vector, _pValue);
    if (result != VECTOR_SUCCESS)
    {
        return result;
    }

    *_pValue = _vector->m_items[_vector->m_nItems - 1];
    --(_vector->m_nItems);

    if (SHOULD_SHRINK(_vector))
    {
        ShrinkVector(_vector);
    }
    return result;
}

VectorResult VectorAppend(Vector *_vector, void *_item)
{
    void **temp;
    if (_vector == NULL)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    else if (_item == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (IS_VECTOR_FULL(_vector))
    {
        if (_vector->m_blockSize == 0)
        {
            return VECTOR_OVERFLOW;
        }

        if (ReallocItems(_vector) != VECTOR_SUCCESS)
        {
            return VECTOR_ALLOCATION_ERROR;
        }
    }

    _vector->m_items[_vector->m_nItems++] = _item;

    return VECTOR_SUCCESS;
}

Vector *VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
    Vector *newVector;

    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }

    newVector = (Vector *)malloc(sizeof(Vector));
    if (newVector == NULL)
    {
        return NULL;
    }

    newVector->m_blockSize = _blockSize;
    newVector->m_nItems = 0;
    newVector->m_size = _initialCapacity;
    newVector->m_originalSize = _initialCapacity;

    newVector->m_items = malloc(_initialCapacity * sizeof(void *));
    if (newVector->m_items == NULL)
    {
        free(newVector);
        return NULL;
    }
    return newVector;
}

void VectorDestroy(Vector **_vector, void (*_elementDestroy)(void *_item))
{
    size_t index;
    if (_vector == NULL || (*_vector) == NULL)
    {

        return;
    }

    if (_elementDestroy != NULL)
    {
        for (index = 0; index < (*_vector)->m_nItems; index++)
        {
            _elementDestroy(((*_vector)->m_items)[index]);
        }
    }

    free((*_vector)->m_items);
    free(*_vector);
    *_vector = NULL;
}

/* Help Functions */

VectorResult ReallocItems(Vector *_vector)
{
    void **temp = realloc(_vector->m_items, (_vector->m_size + _vector->m_blockSize) * sizeof(void *));
    if (temp == NULL)
    {
        return VECTOR_ALLOCATION_ERROR;
    }

    _vector->m_items = temp;
    _vector->m_size += _vector->m_blockSize;
    return VECTOR_SUCCESS;
}

VectorResult IsValidInputRemove(Vector *_vector, void **_pValue)
{
    VectorResult result = VECTOR_SUCCESS;
    if (_vector == NULL)
    {
        result = VECTOR_UNITIALIZED_ERROR;
    }
    else if (_pValue == NULL)
    {
        return NULL_PTR_ERROR;
    }
    else if (_vector->m_nItems == 0)
    {
        result = VECTOR_UNDERFLOW;
    }
    return result;
}

void ShrinkVector(Vector *_vector)
{
    void **temp = realloc(_vector->m_items, (_vector->m_size - _vector->m_blockSize) * sizeof(void *));
    if (temp == NULL)
    {
        return;
    }
    _vector->m_items = temp;
    _vector->m_size -= _vector->m_blockSize;
}

VectorResult IsValidInputGetSet(const Vector *_vector, size_t _index, void **_pValue)
{
    VectorResult result = VECTOR_SUCCESS;
    if (_vector == NULL)
    {
        result = VECTOR_UNITIALIZED_ERROR;
    }
    else if (_pValue == NULL)
    {
        return NULL_PTR_ERROR;
    }
    else if (_index >= _vector->m_nItems)
    {
        result = VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    return result;
}

size_t GetBlockSize(Vector *_vector)
{
    return _vector->m_blockSize;
}
size_t GetSize(Vector *_vector)
{
    return _vector->m_size;
}
size_t GetNItems(Vector *_vector)
{
    return _vector->m_nItems;
}
size_t GetOriginalSize(Vector *_vector)
{
    return _vector->m_originalSize;
}
void **GetItems(Vector *_vector)
{
    return _vector->m_items;
}