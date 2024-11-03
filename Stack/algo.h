#ifndef __ALGO_H__
#define __ALGO_H__


typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

typedef enum Status
{
    VALID_INPUT,
    NULL_PTR_ERROR,
    UNBALANCED_EXPRESSION,
    MEMORY_ALLOCATION_ERROR
} Status;


Status InfixToPostfix(char *infixExp, char *postfixExp);


Bool IsBalancedParentheses(char *exp);



#endif /* #ifndef __ALGO_H__ */
