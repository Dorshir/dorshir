#include <stdio.h>
#include <string.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define NULL_PTR_ERROR -1
#define OK 1
#define INVALID_INPUT_ERROR -2


/**
 * @brief   Finds the first occurrence of the substring str2 in the string str1.
 *
 * @param   str1    The string which we are looking for the substring str2 in it.
 * @param   str2    The string that we are looking for in str2.
 * @return  Returns a pointer to the beginning of the located substring in str1 if exist.
 *          Returns NULL if the substring is not found.
 */
void *MyStrStr(char *str1, char *str2);

/**
 * @brief   Converts a decimal number provided as a string to its binary representation and prints it.
 *
 * @param   str     The string containing the decimal number to convert.
 *
 * @return  Returns OK (0) on success.
 *          Returns NULL_PTR_ERROR (-1) if the input string pointer is NULL.
 *          Returns INVALID_INPUT_ERROR (-2) if the input is invalid (e.g., empty string or non-numeric).
 */
int StringDecToBinary(char* str);

/**
 * @brief   Removes all characters from the string s1 that match any character in the string s2.
 *
 * @param   s1  The string to be modified. Characters found in s2 will be removed from this string.
 * @param   s2  The string containing characters to be removed from s1.
 *
 * @return  Returns OK (0) on success.
 *          Returns NULL_PTR_ERROR (-1) if either s1 or s2 is NULL.
 */
int Squeeze(char* s1, const char* s2);