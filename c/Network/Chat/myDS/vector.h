#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>  /* size_t */

typedef struct Vector Vector;
typedef int (*VectorElementAction)(void* _element, size_t _index, void* _context);

typedef enum Vector_Result {
    VECTOR_SUCCESS,
    VECTOR_UNITIALIZED_ERROR,            /**< Uninitialized vector error */
    VECTOR_ALLOCATION_ERROR,             /**< Memory allocation error */
    VECTOR_UNDERFLOW_ERROR,              /**< Empty vector error */
    VECTOR_OVERFLOW_ERROR,               /**< Fixed size vector overflow error */
    VECTOR_INDEX_OUT_OF_BOUNDS_ERROR     /**< Index exceeds vector size */
} VectorResult;

/**  
 * @brief Create a new dynamic vector with specified initial capacity and block size
 * @param[in] _initialCapacity - Initial number of elements that can be stored
 * @param[in] _blockSize - Number of elements to add when vector needs to grow
 * @return Vector* - On success / NULL on failure
 * @warning if _blockSize is 0, vector will be fixed size
 * @warning if both _initialCapacity and _blockSize are 0, returns NULL
 */
Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize);

/**  
 * @brief Deallocate a vector and optionally destroy its elements
 * @param[in] _vector - Pointer to vector pointer to be destroyed
 * @param[in] _elementDestroy - Optional function to destroy each element (can be NULL)
 */
void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item));

/**  
 * @brief Add an element to the end of the vector
 * @param[in] _vector - Vector to append to
 * @param[in] _item - Element to append
 * @return VECTOR_SUCCESS on success
 * @return VECTOR_UNITIALIZED_ERROR if _vector is NULL
 * @return VECTOR_ALLOCATION_ERROR on memory allocation failure
 * @return VECTOR_OVERFLOW_ERROR if vector is fixed size and full
 */
VectorResult VectorAppend(Vector* _vector, void* _item);

/**  
 * @brief Remove the last element from the vector
 * @param[in] _vector - Vector to remove from
 * @param[out] _pValue - Pointer to store removed element
 * @return VECTOR_SUCCESS on success
 * @return VECTOR_UNITIALIZED_ERROR if _vector or _pValue is NULL
 * @return VECTOR_UNDERFLOW_ERROR if vector is empty
 */
VectorResult VectorRemove(Vector* _vector, void** _pValue);

/**  
 * @brief Get element at specified index
 * @param[in] _vector - Vector to get from
 * @param[in] _index - Index of element to get (0-based)
 * @param[out] _pValue - Pointer to store retrieved element
 * @return VECTOR_SUCCESS on success
 * @return VECTOR_UNITIALIZED_ERROR if _vector or _pValue is NULL
 * @return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR if _index >= vector size
 */
VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue);

/**  
 * @brief Set element at specified index
 * @param[in] _vector - Vector to set in
 * @param[in] _index - Index of element to set (0-based)
 * @param[in] _value - New value to set
 * @return VECTOR_SUCCESS on success
 * @return VECTOR_UNITIALIZED_ERROR if _vector is NULL
 * @return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR if _index >= vector size
 */
VectorResult VectorSet(Vector* _vector, size_t _index, void* _value);

/**  
 * @brief Get number of elements in vector
 * @param[in] _vector - Vector to check
 * @return Number of elements on success, 0 if vector is NULL
 */
size_t VectorSize(const Vector* _vector);

/**  
 * @brief Get current capacity of vector
 * @param[in] _vector - Vector to check
 * @return Total capacity on success, 0 if vector is NULL
 */
size_t VectorCapacity(const Vector* _vector);

/**  
 * @brief Iterate over vector elements with action function
 * @param[in] _vector - Vector to iterate
 * @param[in] _action - Function to call for each element
 * @param[in] _context - Context passed to _action
 * @return Number of elements processed before stopping
 * @note Iteration stops if _action returns 0
 */
size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context);

#endif /* __VECTOR_H__ */
