#include <limits.h>
#include <stdlib.h>
#include <math.h>
#define NULL_PTR_ERROR -1
#define NO_RESULT -2

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
