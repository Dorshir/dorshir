#include "dynamicArray.c"
#include <stdio.h>

int main()
{
    size_t size = 2;
    int blockSize = 4;
    DynamicArray *dynamicArrayPtr;
    int data;

    dynamicArrayPtr = DynamicArrayCreate(size, blockSize);
    DynamicArrayInsert(dynamicArrayPtr, 5);
    DynamicArrayInsert(dynamicArrayPtr, 3);
    PrintArray(dynamicArrayPtr);
    printf("Number of elements: %ld\nArray size: %ld\n", dynamicArrayPtr->NumOfElements, dynamicArrayPtr->dArraySize);
    DynamicArrayInsert(dynamicArrayPtr, 10);
    printf("Number of elements: %ld\nArray size: %ld\n", dynamicArrayPtr->NumOfElements, dynamicArrayPtr->dArraySize);

    printf("Delete..\n");
    DynamicArrayDelete(dynamicArrayPtr, &data);
    printf("Number of elements: %ld\nArray size: %ld\n", dynamicArrayPtr->NumOfElements, dynamicArrayPtr->dArraySize);
    printf("Data: %d\n", data);

    DynamicArrayInsert(dynamicArrayPtr, 5);
    DynamicArrayInsert(dynamicArrayPtr, 3);
    DynamicArrayInsert(dynamicArrayPtr, 5);
    DynamicArrayInsert(dynamicArrayPtr, 3);
    printf("Number of elements: %ld\nArray size: %ld\n", dynamicArrayPtr->NumOfElements, dynamicArrayPtr->dArraySize);

    printf("Updating blocksize to 0\n");
    dynamicArrayPtr->dArrayBlockSize = 0;
    printf("Trying to insert another element..\n");
    DynamicArrayInsert(dynamicArrayPtr, 100);
    printf("Number of elements: %ld\nArray size: %ld\n", dynamicArrayPtr->NumOfElements, dynamicArrayPtr->dArraySize);

    DynamicArrayDestroy(dynamicArrayPtr);
    
    return 0;
}