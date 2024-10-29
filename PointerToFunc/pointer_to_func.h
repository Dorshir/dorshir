#ifndef __POINTER_TO_FUNC__
#define __POINTER_TO_FUNC__
#include <stdlib.h>

#define NULL_PTR_ERROR -1
#define OK 0


/**
 * @brief   Decleration of a new type - Compare function - between two integers.
 *
 * @param   a  First integer to compare with.
 * @param   b  Second integer to compare with.
 * 
 * @return  Value to decide to swap -a- with -b- , by the specific criterion inside this function.
 */
typedef int (*cmpFunc)(int a, int b);


/**
 * @brief   Generic compare function for two elements.
 *
 * @param   a  First element to compare with.
 * @param   b  Second element to compare with.
 * 
 * @return  Value to decide if to swap -a- with -b- , by the specific criterion inside this function.
 */
typedef int (*cmpFuncGeneric)(void* a, void* b);


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


/**
 * @brief   Sorts a generic array by a given element size and a compare function.
 *
 * @param   arr         Array of elements.
 * @param   length      Length of the array.
 * @param   comparator  Pointer to compare function.
 * 
 * @return  NULL_PTR_ERROR (-1) if given arr is a null ptr.
 *          OK (0) if the sort applied successfully.
 */
int GenericSort(void *arr, size_t length, size_t elemSize, cmpFuncGeneric comparator);


#endif