#include "stack.h"
#include "algo.h"
#include <stdlib.h> /* size_t */
#include <stdio.h>  /* printf */

#define INITIALIZE_STACK_SIZE 20
#define INITIALIZE_STACK_BLOCKSIZE 6
#define IS_PARENTHESES(char) (char == '(' || char == ')' || char == '[' || char == ']' || char == '{' || char == '}')
#define IS_OPEN_PARENTHESES(char) (char == '(' || char == '[' || char == '{')
#define IS_OPERAND(char) (char == '+' || char == '-' || char == '/' || char == '*')

/***************************** Help Functions Decelerations *****************************/

static Bool IsBalancedCurrentParentheses(char current, char poppedElement);
static Bool UpdateValidationStack(Stack *validationStack, char *current);
static Bool ValidateExpression(char *exp, Stack *validationStack);
static Status CheckInput(char *infixExp, char *postfixExp);
static void PopCurrentScopeElements(Stack *stack, char *postfixExp, size_t *postIndex);
static Bool IsLowerPriorityOperand(char current, char popped);
static void BuildNextSubExp(Stack *stack, char *postfixExp, size_t *postIndex, char *current);
static void BuildPostfixExp(char *infixExp, char *postfixExp, Stack *stack);
static void BuildPostfixExp(char *infixExp, char *postfixExp, Stack *stack);

/***************************** Main Functions *****************************/

Status InfixToPostfix(char *infixExp, char *postfixExp)
{

    Stack *stack;
    Status resStatus = CheckInput(infixExp, postfixExp);
    if (resStatus != VALID_INPUT)
    {
        return resStatus;
    }

    stack = StackCreate(INITIALIZE_STACK_SIZE, INITIALIZE_STACK_BLOCKSIZE);
    if (stack == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    BuildPostfixExp(infixExp, postfixExp, stack);

    StackDestroy(&stack, NULL);
}

Bool IsBalancedParentheses(char *exp)
{
    Bool res = TRUE;
    Stack *stack = StackCreate(INITIALIZE_STACK_SIZE, INITIALIZE_STACK_BLOCKSIZE);
    if (stack == NULL)
    {
        return FALSE;
    }

    if (ValidateExpression(exp, stack) == FALSE)
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

/***************************** Help Functions *****************************/

static Bool IsBalancedCurrentParentheses(char current, char poppedElement)
{
    Bool res = TRUE;
    switch (current)
    {
    case ')':
        if (poppedElement != '(')
        {
            res = FALSE;
        }
        break;
    case ']':
        if (poppedElement != '[')
        {
            res = FALSE;
        }
        break;
    case '}':
        if (poppedElement != '{')
        {
            res = FALSE;
        }
        break;
    default:
        break;
    }
    return res;
}

static Bool UpdateValidationStack(Stack *validationStack, char *current)
{
    Bool res = TRUE;
    char *poppedElement;
    if (IS_OPEN_PARENTHESES(*current))
    {
        StackPush(validationStack, current);
    }
    else if (StackPop(validationStack, (void **)&poppedElement) == STACK_SUCCESS)
    {
        if (!IsBalancedCurrentParentheses(*current, *poppedElement))
        {
            res = FALSE;
        }
    }
    return res;
}

static Bool ValidateExpression(char *exp, Stack *validationStack)
{
    Bool res = TRUE;
    size_t index = 0;

    while (exp[index] != '\0')
    {
        if (IS_PARENTHESES(exp[index]))
        {
            if (!UpdateValidationStack(validationStack, &exp[index]))
            {
                res = FALSE;
                break;
            }
        }
        ++index;
    }
    return res;
}

static Status CheckInput(char *infixExp, char *postfixExp)
{
    if (infixExp == NULL || postfixExp == NULL)
    {
        return NULL_PTR_ERROR;
    }
    else if (!IsBalancedParentheses(infixExp))
    {
        return UNBALANCED_EXPRESSION;
    }
    else
    {
        return VALID_INPUT;
    }
}

static void PopCurrentScopeElements(Stack *stack, char *postfixExp, size_t *postIndex)
{
    char *popped;
    while (!StackIsEmpty(stack))
    {
        StackPop(stack, (void **)&popped);
        if (IS_OPEN_PARENTHESES(*popped))
        {
            break;
        }
        postfixExp[(*postIndex)++] = *popped;
    }
}

static int GetOperatorPrecedence(char op)
{
    switch (op)
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

static Bool IsLowerPriorityOperand(char current, char stackTop)
{
    int currentPrecedence = GetOperatorPrecedence(current);
    int stackTopPrecedence = GetOperatorPrecedence(stackTop);

    return currentPrecedence <= stackTopPrecedence;
}

static void BuildNextSubExp(Stack *stack, char *postfixExp, size_t *postIndex, char *current)
{
    char *popped;
    while (!StackIsEmpty(stack))
    {
        StackTop(stack, (void **)&popped);
        if (IS_OPEN_PARENTHESES(*popped))
        {
            break;
        }
        else if (IS_OPERAND(*popped))
        {
            if (IsLowerPriorityOperand(*current, *popped))
            {
                StackPop(stack, (void **)&popped);
                postfixExp[(*postIndex)++] = *popped;
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
    StackPush(stack, current);
}

static void BuildPostfixExp(char *infixExp, char *postfixExp, Stack *stack)
{
    char *popped;
    size_t inIndex = 0, postIndex = 0;
    char *current = &infixExp[inIndex];

    while (*current != '\0')
    {
        if (IS_PARENTHESES(*current))
        {
            if (IS_OPEN_PARENTHESES(*current))
            {
                StackPush(stack, current);
            }
            else
            {
                PopCurrentScopeElements(stack, postfixExp, &postIndex);
            }
        }
        else if (IS_OPERAND(*current))
        {
            BuildNextSubExp(stack, postfixExp, &postIndex, current);
        }
        else
        {
            postfixExp[postIndex++] = *current;
        }
        current = &infixExp[++inIndex];
    }

    while (!StackIsEmpty(stack))
    {
        StackPop(stack, (void **)&popped);
        postfixExp[postIndex++] = *popped;
    }

    postfixExp[postIndex] = '\0';
}
