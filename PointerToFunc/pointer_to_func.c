#include "pointer_to_func.h"
#include <stdio.h>

#define NULL_PTR_ERROR -1
#define OK 0

static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int Sort(int *arr, size_t length, cmpFunc comparator)
{
    int i, j, flag;

    if (arr == NULL)
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
