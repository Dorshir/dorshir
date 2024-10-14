#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define NULL_PTR_ERROR -1
#define UNDERFLOW -2
#define REALLOC_FAILED -3
#define OVERFLOW -4

typedef struct DynamicArray
{
    int *dArray;
    size_t dArraySize;
    int dArrayBlockSize;
    size_t NumOfElements;
} DynamicArray;

/**
 * @brief   Creates a new dynamic array.
 *
 * Allocates and initializes a `DynamicArray` structure with the specified initial size and block size.
 *
 * @param   size        The initial capacity of the dynamic array.
 * @param   blockSize   The block size to use when resizing the array.
 *                      If `blockSize` is 0, the array cannot grow beyond its initial capacity.
 *
 * @return  Returns a pointer to the newly created `DynamicArray` on success.
 *          Returns `NULL` if memory allocation fails.
 */
DynamicArray *DynamicArrayCreate(int size, int blockSize);

/**
 * @brief   Destroys a dynamic array.
 *
 * Frees the memory allocated for the dynamic array's internal array and the `DynamicArray` structure itself.
 *
 * @param   DynamincArrayPtr    Pointer to the `DynamicArray` to destroy.
 *
 * @return  Void.
 */
void DynamicArrayDestroy(DynamicArray *DynaminArrayPtr);

/**
 * @brief   Inserts an element into the dynamic array.
 *
 * Adds a new element to the end of the dynamic array. If the array is full, it attempts to expand it by
 * the block size specified during creation. If the block size is zero and the array is full, insertion
 * fails with an overflow error.
 *
 * @param   DynamincArrayPtr    Pointer to the `DynamicArray` where the data will be inserted.
 * @param   data                The integer data to insert into the array.
 *
 * @return  Returns `OK` (1) on successful insertion.
 *          Returns `NULL_PTR_ERROR` (-1) if `DynamincArrayPtr` is `NULL` or its internal array is `NULL`.
 *          Returns `OVERFLOW` (-4) if the array is full and cannot be expanded (block size is zero).
 *          Returns `REALLOC_FAILED` (-3) if memory reallocation fails when expanding the array.
 */
int DynamicArrayInsert(DynamicArray *DynaminArrayPtr, int data);

/**
 * @brief   Deletes the last element from the dynamic array.
 *
 * Removes the last element from the dynamic array and stores its value in the location pointed to by `data`.
 *
 * @param   DynamincArrayPtr    Pointer to the `DynamicArray` from which to delete the element.
 * @param   data                Pointer to an integer where the deleted data will be stored.
 *
 * @return  Returns `OK` (1) on successful deletion.
 *          Returns `NULL_PTR_ERROR` (-1) if `DynamincArrayPtr`, its internal array, or `data` is `NULL`.
 *          Returns `UNDERFLOW` (-2) if the array is empty.
 */
int DynamicArrayDelete(DynamicArray *DynaminArrayPtr, int *data);