#include "dynamicArray.h"

void PrintArray(DynamicArray* dptr) {
    int i;
    for(i = 0; i < dptr->NumOfElements; ++i) {
        printf("%d,", dptr->dArray[i]);
    }
    printf("\n");
}

DynamicArray *DynamicArrayCreate(int size, int blockSize)
{
    DynamicArray *pDynamicArray;
    pDynamicArray = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (pDynamicArray == NULL)
    {
        return NULL;
    }

    pDynamicArray->dArrayBlockSize = blockSize;
    pDynamicArray->NumOfElements = 0;
    pDynamicArray->dArraySize = size;

    pDynamicArray->dArray = (int *)malloc(size * sizeof(int));
    if (pDynamicArray->dArray == NULL)
    {
        free(pDynamicArray);
        return NULL;
    }

    return pDynamicArray;
}

void DynamicArrayDestroy(DynamicArray *DynamincArrayPtr)
{
    if (DynamincArrayPtr == NULL || DynamincArrayPtr->dArray == NULL)
    {
        return;
    }

    free(DynamincArrayPtr->dArray);
    free(DynamincArrayPtr);
}

int DynamicArrayInsert(DynamicArray *DynamincArrayPtr, int data)
{
    int *temp;
    if (DynamincArrayPtr == NULL || DynamincArrayPtr->dArray == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (DynamincArrayPtr->NumOfElements == DynamincArrayPtr->dArraySize)
    {
        if (DynamincArrayPtr->dArrayBlockSize == 0)
        {
            return OVERFLOW;
        }

        temp = (int *)realloc(DynamincArrayPtr->dArray, DynamincArrayPtr->dArrayBlockSize * sizeof(int));
        if (temp == NULL)
        {
            return REALLOC_FAILED;
        }
        DynamincArrayPtr->dArray = temp;

        DynamincArrayPtr->dArray[DynamincArrayPtr->NumOfElements] = data;
        DynamincArrayPtr->NumOfElements++;
        DynamincArrayPtr->dArraySize += DynamincArrayPtr->dArrayBlockSize;
    }
    else
    {
        DynamincArrayPtr->dArray[DynamincArrayPtr->NumOfElements] = data;
        DynamincArrayPtr->NumOfElements++;
        return OK;
    }
}

int BinarySearch(int array[], int x, int length)
{
    int low = 0;
    int high = length - 1;
    int mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;

        if (x == array[mid])
            return mid;

        if (x > array[mid])
            low = mid + 1;

        else
            high = mid - 1;
    }

    return -1;
}

int DynamicArrayDelete(DynamicArray *DynamincArrayPtr, int *data)
{
    if (DynamincArrayPtr == NULL || DynamincArrayPtr->dArray == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (DynamincArrayPtr->NumOfElements == 0)
    {
        return UNDERFLOW;
    }
    *data = DynamincArrayPtr->dArray[DynamincArrayPtr->NumOfElements - 1];
    DynamincArrayPtr->NumOfElements--;

    return OK;
}
