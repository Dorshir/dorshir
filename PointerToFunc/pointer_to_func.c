#include "pointer_to_func.h"
#include <stdlib.h> /* size_t, malloc */
#include <string.h> /* memcpy */

static void Swap(int *a, int *b);
static void GenericSwap(void *a, void *b, void *temp, size_t elemSize);

/* Main Functions */

int Sort(int *arr, size_t length, cmpFunc comparator)
{
    int i, j, flag;

    if (arr == NULL || comparator == NULL)
    {
        return NULL_PTR_ERROR;
    }

    for (i = 0; i < length - 1; i++)
    {
        flag = 0;
        for (j = 0; j < length - i - 1; j++)
        {
            if (comparator(arr[j], arr[j + 1]))
            {
                flag = 1;
                Swap(&arr[j], &arr[j + 1]);
            }
        }
        if (flag == 0)
        {
            break;
        }
    }
    return OK;
}

int GenericSort(void *arr, size_t length, size_t elemSize, cmpFuncGeneric comparator)
{
    size_t i, j;
    int currPos;
    int flag;
    char *ptr;
    void *temp;

    if (arr == NULL || comparator == NULL)
    {
        return NULL_PTR_ERROR;
    }

    ptr = arr;
    temp = malloc(elemSize);
    if (temp == NULL)
    {
        return ALLOCATION_FAILED;
    }

    for (i = 0; i < length - 1; i++)
    {
        flag = 0;
        for (j = 0; j < length - i - 1; j = j + 1)
        {
            currPos = j * elemSize;
            if (comparator(&ptr[currPos], &ptr[currPos + elemSize]))
            {
                flag = 1;
                GenericSwap(&ptr[currPos], &ptr[currPos + elemSize], temp, elemSize);
            }
        }
        if (flag == 0)
        {
            break;
        }
    }
    free(temp);
    return OK;
}

/* Static Functions */

static void GenericSwap(void *a, void *b, void *temp, size_t elemSize)
{
    memcpy(temp, a, elemSize);
    memcpy(a, b, elemSize);
    memcpy(b, temp, elemSize);
}

static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
