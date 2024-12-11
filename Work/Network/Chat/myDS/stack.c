#include <stdlib.h>
#include "vector.h"
#include "stack.h"

struct Stack
{
    Vector* m_vector;
};

Stack* StackCreate(size_t _initialCapacity, size_t _blockSize)
{
    Stack* stack; 
    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }   
    stack = (Stack*)malloc(sizeof(Stack));
    if (!stack)
    {
        return NULL;
    }   
    stack->m_vector = VectorCreate(_initialCapacity, _blockSize);
    if (!stack->m_vector)
    {
        free(stack);
        return NULL;
    }   
    return stack;
}

void StackDestroy(Stack** _stack, void (*_elementDestroy)(void* _item))
{
    if (!_stack || !*_stack)
    {
        return;
    }   
    VectorDestroy(&((*_stack)->m_vector), _elementDestroy);
    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack* _stack, void* _item)
{
    VectorResult result;   
    if (!_stack || !_item)
    {
        return STACK_UNITIALIZED_ERROR;
    }    
    result = VectorAppend(_stack->m_vector, _item);
    switch (result)
    {
        case VECTOR_SUCCESS:
            return STACK_SUCCESS;
        case VECTOR_ALLOCATION_ERROR:
            return STACK_ALLOCATION_ERROR;
        case VECTOR_OVERFLOW_ERROR:
            return STACK_OVERFLOW_ERROR;
        default:
            return STACK_UNITIALIZED_ERROR;
    }
}

StackResult StackPop(Stack* _stack, void** _pValue)
{
    VectorResult result;
    
    if (!_stack || !_pValue)
    {
        return STACK_UNITIALIZED_ERROR;
    }
    
    if (!_stack->m_vector || VectorSize(_stack->m_vector) == 0)  
    {
        return STACK_UNDERFLOW_ERROR;
    }
    
    result = VectorRemove(_stack->m_vector, _pValue);
    return (result == VECTOR_SUCCESS) ? STACK_SUCCESS : STACK_UNITIALIZED_ERROR;
}

StackResult StackTop(Stack* _stack, void** _pValue)
{
    VectorResult result;
    size_t size;    
    if (!_stack || !_pValue)
    {
        return STACK_UNITIALIZED_ERROR;
    }
    
    if (!_stack->m_vector || VectorSize(_stack->m_vector) == 0)  
    {
        return STACK_IS_EMPTY;
    }   
    size = VectorSize(_stack->m_vector);
    result = VectorGet(_stack->m_vector, size - 1, _pValue);
    return (result == VECTOR_SUCCESS) ? STACK_SUCCESS : STACK_UNITIALIZED_ERROR;
}

int StackIsEmpty(Stack* _stack)
{
    if (!_stack) {
        return 1;  
    }
    return (VectorSize(_stack->m_vector) == 0); 
}     
size_t StackSize(const Stack* _stack)
{
    return (_stack) ? VectorSize(_stack->m_vector) : 0;
}

size_t StackCapacity(const Stack* _stack)
{
    return (_stack) ? VectorCapacity(_stack->m_vector) : 0;
}

size_t StackPrint(const Stack* _stack, StackElementAction _action, void* _context)
{
    if (!_stack || !_action)
    {
        return 0;
    }   
    return VectorForEach(_stack->m_vector, (VectorElementAction)_action, _context);
}
