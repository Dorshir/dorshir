#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define NULL_PTR_ERROR -1
#define INVALID_INPUT_ERROR -2

/**
 * @brief   Finds the median of an integer array.
 *
 * @param   arr     Array of integers.
 * @param   length  Length of the array.
 * @return  Median value of the array. Returns -1 if the array pointer is NULL and -2 if the median cannot be found.
 */
int findMedian(const int arr[], int length);


/**
 * @brief   Merge two sorted arrays into one array.
 * 
 * @param arr1  Sorted array 1 of integers.
 * @param length1   Length of arr1.
 * @param arr2  Sorted array 2 of integers.
 * @param length2   Length of arr2.
 * @param result_arr    Array which will be returned as the marged array.
 * @return  Sorted array which is a merged array of the two given arrays. Returns -1 if one of the arrays pointer is NULL and -2 for any other error.
 **/
int merge(const int arr1[], int length1, const int arr2[], int length2, int result_arr[]);


/**
 * @brief   Reverse a given string in-place.
 * 
 * @param _str  The string to be reversed. Cannot be a NULL pointer.
 * @return  0 if successful, or -1 if the string pointer is NULL.
 */
int ReverseStr(char* _str);


/**
 * @brief   Check if a given string is a palindrome.
 * 
 * @param _str  The string to check. Cannot be a NULL pointer.
 * @return  1 if is palindrome, 0 not, and -1 if the input string pointer is NULL.
 */
int IsPalindrome(char* _str);


/**
 * @brief   Convert a string to an integer.
 * 
 * @param _str  The string to be converted. This must be a null-terminated string containing numeric characters.
 * @param _num  Pointer to an integer where the converted value will be stored.
 * @return  0 if successful, or -1 if the input string or the integer pointer is NULL, or -2 if the input is invalid.
 */
int MyAToI(char* _str, int* _num);


/**
 * @brief   Convert an integer to a string.
 * 
 * @param _num      The integer to be converted.
 * @param _buffer   Buffer where the resulting null-terminated string will be stored. The buffer must be large enough to hold the result.
 * @return  0 if successful, or -1 if the input buffer pointer is NULL.
 */
int MyIToA(int _num, char* _buffer);

