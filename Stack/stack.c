#include "stack.h"
#include "dynamicVector.h"
#include <stdlib.h> /* size_t, dynamic memory */

struct Stack
{
    Vector *m_vector;
};

static StackResult IsValidInputPopTop(Stack *_stack, void **_pValue);
static StackResult StatusConvert(VectorResult vectorStatus);

/************************* Main Functions *************************/

Stack *StackCreate(size_t _initialCapacity, size_t _blockSize)
{
    Stack *newStack;
    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }

    newStack = (Stack *)malloc(sizeof(Stack));
    if (newStack == NULL)
    {
        return NULL;
    }

    newStack->m_vector = VectorCreate(_initialCapacity, _blockSize);
    if (newStack->m_vector == NULL)
    {
        free(newStack);
        return NULL;
    }

    return newStack;
}

void StackDestroy(Stack **_stack, void (*_elementDestroy)(void *_item))
{
    if (_stack == NULL || (*_stack) == NULL)
    {
        return;
    }
    
    VectorDestroy(&(*_stack)->m_vector, _elementDestroy);
    
    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack *_stack, void *_item)
{
    if (_stack == NULL)
    {
        return STACK_UNITIALIZED_ERROR;
    }
    else if (_item == NULL)
    {
        return STACK_NULL_PTR_ERROR;
    }
    return StatusConvert(VectorAppend(_stack->m_vector, _item));
}

StackResult StackPop(Stack *_stack, void **_pValue)
{
    StackResult result = IsValidInputPopTop(_stack, _pValue);
    if (result != STACK_SUCCESS)
    {
        return result;
    }
    return StatusConvert(VectorRemove(_stack->m_vector, _pValue));
}

StackResult StackTop(Stack *_stack, void **_pValue)
{
    StackResult result = IsValidInputPopTop(_stack, _pValue);
    if (result != STACK_SUCCESS)
    {
        return result;
    }
    return StatusConvert(VectorGet(_stack->m_vector, VectorSize(_stack->m_vector) - 1, _pValue));
}

size_t StackSize(const Stack *_stack)
{
    if (_stack == NULL)
    {
        return 0;
    }
    return VectorSize(_stack->m_vector);
}

size_t StackCapacity(const Stack *_stack)
{
    if (_stack == NULL)
    {
        return 0;
    }
    return VectorCapacity(_stack->m_vector);
}

int StackIsEmpty(Stack *_stack)
{
    if (_stack == NULL)
    {
        return 1;
    }
    return VectorSize(_stack->m_vector) == 0;
}

size_t StackPrint(const Stack *_stack, StackElementAction _action, void *_context)
{
    if (_stack == NULL || _action == NULL)
    {
        return 0;
    }
    return VectorForEach(_stack->m_vector,_action,_context);
}

/************************* Help Functions *************************/

static StackResult IsValidInputPopTop(Stack *_stack, void **_pValue)
{
    StackResult result = STACK_SUCCESS;
    if (_stack == NULL)
    {
        result = STACK_UNITIALIZED_ERROR;
    }
    else if (_pValue == NULL)
    {
        return STACK_NULL_PTR_ERROR;
    }
    else if (VectorSize(_stack->m_vector) == 0)
    {
        result = STACK_IS_EMPTY;
    }
    return result;
}

static StackResult StatusConvert(VectorResult vectorStatus)
{
    StackResult stackStatus;
    switch (vectorStatus)
    {
    case VECTOR_SUCCESS:
        stackStatus = STACK_SUCCESS;
        break;
    case VECTOR_ALLOCATION_ERROR:
        stackStatus = STACK_ALLOCATION_ERROR;
        break;
    case NULL_PTR_ERROR:
        stackStatus = STACK_NULL_PTR_ERROR;
        break;
    case VECTOR_OVERFLOW:
        stackStatus = STACK_OVERFLOW;
        break;
    default:
        break;
    }
    return stackStatus;
}
