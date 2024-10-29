#include "pointer_to_func.h"
#include <stdio.h>
#include <string.h>

static void Swap(int *a, int *b);
static void GenericSwap(void *a, void *b,void* temp, size_t elemSize);

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
    char i, j;
    int flag;
    char *ptr = arr;
    void * temp = malloc(elemSize);

    if (arr == NULL || comparator == NULL)
    {
        return NULL_PTR_ERROR;
    }

    for (i = 0; i < length - 1; i++)
    {
        flag = 0;
        for (j = 0; j < length * elemSize - i - elemSize; j = j + elemSize)
        {
            if (comparator(&ptr[j], &ptr[j + elemSize]))
            {
                flag = 1;
                GenericSwap(&ptr[j], &ptr[j + elemSize],temp, elemSize);
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

static void GenericSwap(void *a, void *b,void* temp, size_t elemSize)
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
