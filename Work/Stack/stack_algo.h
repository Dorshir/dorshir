#ifndef __STACK_ALGO_H__
#define __STACK_ALGO_H__

#include <limits.h> /* INT_MAX */

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


/**  
 * @brief  Converts a given infix expression to a postfix expression.
 *
 * @param[in] _infixExp     A string contains the infix expression.
 * @param[in] _postfixExp   A string that will hold the postfix expression.
 * 
 * @returns Status to determine if the convertion accomplished or any error.
 * @retval  VALID_INPUT if convertion accomplished.
 * @retval  NULL_PTR_ERROR if any of the given pointers are null pointers.
 * @retval  UNBALANCED_EXPRESSION if _infixExp is unbalanced parentheses expression.
 * @retval  MEMORY_ALLOCATION_ERROR if dynamic allocating for inside data structure failed.
 */
Status InfixToPostfix(char *_infixExp, char *_postfixExp);


/**  
 * @brief  Check if a given expression is a balanced parentheses expression.
 *
 * @param[in] _exp  A string contains the expression.
 * 
 * @returns True if _exp is a balanced expression or False if it is not.
 * @retval  TRUE if balanced.
 * @retval  FALSE if not balanced.
 */
Bool IsBalancedParentheses(char *_exp);


/**  
 * @brief  Evaluate postfix expression.
 *
 * @param[in] _exp  A string contains the expression.
 * 
 * @returns Evaluated value of the given posfix expression.
 * @retval  Evaluated value if valid expression.
 * @retval  INT_MAX if invalid expression.
 */
int EvaluatePostfixExpression(char* _exp);

#endif /* #ifndef __STACK_ALGO_H__ */