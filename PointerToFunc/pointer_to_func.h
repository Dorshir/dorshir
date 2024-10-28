#ifndef __POINTER_TO_FUNC__
#define __POINTER_TO_FUNC__

#include <stdlib.h>

/**
 * @brief   Decleration of a new type - Compare function - between two integers.
 *
 * @param   a  First integer to compare with.
 * @param   b  Second integer to compare with.
 * 
 * @return  Value to decide if a > b, by the specific criterion inside this function.
 */
typedef int (*cmpFunc)(int a, int b);


/**
 * @brief   Sorts an integer array by a given compare function.
 *
 * @param   arr         Array of integers.
 * @param   length      Length of the array.
 * @param   comparator  Pointer to compare function. This function expect two integers and returns an int the decides if a > b but its own criterion.
 * 
 * @return  NULL_PTR_ERROR (-1) if given arr is a null ptr.
 *          OK (0) if the sort applied successfully.
 */
int Sort(int *arr, size_t length, cmpFunc comparator);

#endif