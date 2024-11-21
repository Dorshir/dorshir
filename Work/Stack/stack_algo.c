#include "stack.h"
#include "stack_algo.h"
#include <stdlib.h> /* size_t */
#include <stdio.h>  /* printf */
#include <ctype.h>  /* isdigit */

#define INITIALIZE_STACK_SIZE 20
#define INITIALIZE_STACK_BLOCKSIZE 6
#define IS_PARENTHESES(char) (char == '(' || char == ')' || char == '[' || char == ']' || char == '{' || char == '}')
#define IS_OPEN_PARENTHESES(char) (char == '(' || char == '[' || char == '{')
#define IS_OPERAND(char) (char == '+' || char == '-' || char == '/' || char == '*')
#define MAX_BITS_IN_INT 32

/***************************** Help Functions Decelerations *****************************/

static Bool IsBalancedCurrentParentheses(char _current, char _poppedElement);
static Bool UpdateValidationStack(Stack *_validationStack, char *_current);
static Bool ValidateExpression(char *_exp, Stack *_validationStack);
static Status CheckInput(char *_infixExp, char *_postfixExp);
static void PopCurrentScopeElements(Stack *_stack, char *_postfixExp, size_t *_postIndex);
static Bool IsLowerPriorityOperand(char _current, char _popped);
static void BuildNextSubExp(Stack *_stack, char *_postfixExp, size_t *_postIndex, char *_current);
static void BuildPostfixExp(char *_infixExp, char *_postfixExp, Stack *_stack);
static void BuildPostfixExp(char *_infixExp, char *_postfixExp, Stack *_stack);
static void EvalExp(char *_exp, int *_result);
static void EvalSubExp(Stack *_stack, char _op);
static int GetOperatorPriority(char _op);
static void GetNextNumberOneDigit(char *_exp, size_t *index, int *_result);
static void GetNextNumber(char *_exp, size_t *index, int *_result);

/***************************** Main Functions *****************************/

Status InfixToPostfix(char *_infixExp, char *_postfixExp)
{

    Stack *stack;
    Status resStatus = CheckInput(_infixExp, _postfixExp);
    if (resStatus != VALID_INPUT)
    {
        return resStatus;
    }

    stack = StackCreate(INITIALIZE_STACK_SIZE, INITIALIZE_STACK_BLOCKSIZE);
    if (stack == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    BuildPostfixExp(_infixExp, _postfixExp, stack);

    StackDestroy(&stack, NULL);
}

Bool IsBalancedParentheses(char *_exp)
{
    Bool res = TRUE;
    Stack *stack;
    if (_exp == NULL)
    {
        return TRUE;
    }

    stack = StackCreate(INITIALIZE_STACK_SIZE, INITIALIZE_STACK_BLOCKSIZE);
    if (stack == NULL)
    {
        return FALSE;
    }

    if (ValidateExpression(_exp, stack) == FALSE)
    {
        res = FALSE;
    }

    if (!StackIsEmpty(stack))
    {
        res = FALSE;
    }

    StackDestroy(&stack, NULL);
    return res;
}

int EvaluatePostfixExpression(char *_exp)
{
    int result = 0;
    if (_exp == NULL)
    {
        return result;
    }
    EvalExp(_exp, &result);

    return result;
}

/***************************** Help Functions *****************************/

static Bool IsBalancedCurrentParentheses(char _current, char _poppedElement)
{
    Bool res = TRUE;
    switch (_current)
    {
    case ')':
        if (_poppedElement != '(')
        {
            res = FALSE;
        }
        break;
    case ']':
        if (_poppedElement != '[')
        {
            res = FALSE;
        }
        break;
    case '}':
        if (_poppedElement != '{')
        {
            res = FALSE;
        }
        break;
    default:
        break;
    }
    return res;
}

static Bool UpdateValidationStack(Stack *_validationStack, char *_current)
{
    Bool res = TRUE;
    char *poppedElement;
    if (IS_OPEN_PARENTHESES(*_current))
    {
        StackPush(_validationStack, _current);
    }
    else if (StackPop(_validationStack, (void **)&poppedElement) == STACK_SUCCESS)
    {
        if (!IsBalancedCurrentParentheses(*_current, *poppedElement))
        {
            res = FALSE;
        }
    }
    return res;
}

static Bool ValidateExpression(char *_exp, Stack *_validationStack)
{
    Bool res = TRUE;
    size_t index = 0;

    while (_exp[index] != '\0')
    {
        if (IS_PARENTHESES(_exp[index]))
        {
            if (!UpdateValidationStack(_validationStack, &_exp[index]))
            {
                res = FALSE;
                break;
            }
        }
        ++index;
    }
    return res;
}

static Status CheckInput(char *_infixExp, char *_postfixExp)
{
    if (_infixExp == NULL || _postfixExp == NULL)
    {
        return NULL_PTR_ERROR;
    }
    else if (!IsBalancedParentheses(_infixExp))
    {
        return UNBALANCED_EXPRESSION;
    }
    else
    {
        return VALID_INPUT;
    }
}

static void PopCurrentScopeElements(Stack *_stack, char *_postfixExp, size_t *_postIndex)
{
    char *popped;
    while (!StackIsEmpty(_stack))
    {
        StackPop(_stack, (void **)&popped);
        if (IS_OPEN_PARENTHESES(*popped))
        {
            break;
        }
        _postfixExp[(*_postIndex)++] = *popped;
    }
}

static int GetOperatorPriority(char _op)
{
    switch (_op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

static Bool IsLowerPriorityOperand(char _current, char _stackTop)
{
    int currentPriority = GetOperatorPriority(_current);
    int stackTopPriority = GetOperatorPriority(_stackTop);

    return currentPriority <= stackTopPriority;
}

static void BuildNextSubExp(Stack *_stack, char *_postfixExp, size_t *_postIndex, char *_current)
{
    char *popped;
    while (!StackIsEmpty(_stack))
    {
        StackTop(_stack, (void **)&popped);
        if (IS_OPEN_PARENTHESES(*popped))
        {
            break;
        }
        else if (IS_OPERAND(*popped))
        {
            if (IsLowerPriorityOperand(*_current, *popped))
            {
                StackPop(_stack, (void **)&popped);
                _postfixExp[(*_postIndex)++] = *popped;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    StackPush(_stack, _current);
}

static void BuildPostfixExp(char *_infixExp, char *_postfixExp, Stack *_stack)
{
    char *popped;
    size_t inIndex = 0, postIndex = 0;
    char *current = &_infixExp[inIndex];

    while (*current != '\0')
    {
        if (IS_PARENTHESES(*current))
        {
            if (IS_OPEN_PARENTHESES(*current))
            {
                StackPush(_stack, current);
            }
            else
            {
                PopCurrentScopeElements(_stack, _postfixExp, &postIndex);
            }
        }
        else if (IS_OPERAND(*current))
        {
            BuildNextSubExp(_stack, _postfixExp, &postIndex, current);
        }
        else
        {
            _postfixExp[postIndex++] = *current;
        }
        current = &_infixExp[++inIndex];
    }

    while (!StackIsEmpty(_stack))
    {
        StackPop(_stack, (void **)&popped);
        _postfixExp[postIndex++] = *popped;
    }

    _postfixExp[postIndex] = '\0';
}

static void EvalSubExp(Stack *_stack, char _op)
{
    int *pFirst, *pSecond, *pResult;

    StackPop(_stack, (void **)&pSecond);
    StackPop(_stack, (void **)&pFirst);

    pResult = (int *)malloc(sizeof(int));

    switch (_op)
    {
    case '+':
        *pResult = *pFirst + *pSecond;
        break;
    case '-':
        *pResult = *pFirst - *pSecond;
        break;
    case '/':
        if (*pSecond == 0)
        {
            free(pResult);
            free(pFirst);
            free(pSecond);
            return;
        }
        *pResult = *pFirst / *pSecond;
        break;
    case '*':
        *pResult = *pFirst * *pSecond;
        break;
    default:
        free(pResult);
        free(pFirst);
        free(pSecond);
        return;
    }

    StackPush(_stack, pResult);
    free(pFirst);
    free(pSecond);
}

static void GetNextNumber(char *_exp, size_t *index, int *_result)
{
    size_t j = 0;
    char numAsStr[MAX_BITS_IN_INT] = {0};
    while (_exp[*index] != '\0' && isdigit(_exp[*index]))
    {
        numAsStr[j++] = _exp[*index];
        ++(*index);
    }
    numAsStr[j++] = '\0';
    *_result = atoi(numAsStr);
}

static void GetNextNumberOneDigit(char *_exp, size_t *_index, int *_result)
{
    char nums[2];
    nums[0] = _exp[*_index];
    nums[1] = '\0';
    *_result = atoi(nums);
    ++(*_index);
}

static void EvalExp(char *_exp, int *_result)
{
    size_t index = 0;
    int number;
    int *pNumber, *pFinalResult;
    Stack *stack = StackCreate(INITIALIZE_STACK_SIZE, INITIALIZE_STACK_BLOCKSIZE);
    if (stack == NULL)
    {
        return;
    }

    while (_exp[index] != '\0')
    {
        if (IS_OPERAND(_exp[index]))
        {
            if (StackSize(stack) < 2)
            { /* Should be > 2 numbers in stack when reaching an operand */
                *_result = INT_MAX;
                StackDestroy(&stack, free);
                return;
            }
            EvalSubExp(stack, _exp[index]);
            ++index;
        }
        else if (isdigit(_exp[index]))
        {
            pNumber = (int *)malloc(sizeof(int));
            GetNextNumberOneDigit(_exp, &index, pNumber);
            StackPush(stack, pNumber);
        }
        else
        {
            /* Invalid expression */
            *_result = INT_MAX;
            StackDestroy(&stack, free);
            return;
        }
    }

    StackPop(stack, (void **)&pFinalResult);
    *_result = *pFinalResult;
    free(pFinalResult);
    StackDestroy(&stack, free);
}
