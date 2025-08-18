#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h> /* for size_t */

typedef enum {
    QUEUE_SUCCESS,
    QUEUE_UNINITIALIZED_ERROR,
    QUEUE_OVERFLOW_ERROR,
    QUEUE_DATA_NOT_FOUND_ERROR,
    QUEUE_DATA_UNINITIALIZED_ERROR
} QueueResult;

typedef struct Queue Queue;

typedef void (*DestroyItem)(void* _element);
typedef int (*ActionFunction)(void* _element, void* _context);

/**
 * @brief Create a new queue with a specified size
 * @param[in] _size - The size of the queue
 * @return Queue pointer - on success, NULL on failure
 */
Queue* QueueCreate(size_t _size);

/**
 * @brief Deallocate a previously created queue
 * @param[in] _queue - The queue to deallocate
 * @param[in] _itemDestroy - Optional destroy function for each item
 */
void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy);

/**
 * @brief Add a new item to the queue
 * @param[in] _queue - The queue to add the item to
 * @param[in] _item - The item to add to the queue
 * @return QueueResult - Result code of the operation
 * @retval QUEUE_SUCCESS - On success
 * @retval QUEUE_UNINITIALIZED_ERROR - If _queue is NULL
 * @retval QUEUE_DATA_UNINITIALIZED_ERROR - If _item is NULL
 * @retval QUEUE_OVERFLOW_ERROR - If the queue is full
 */
QueueResult QueueInsert(Queue* _queue, void* _item);

/**
 * @brief Remove the first item in the queue
 * @param[in] _queue - The queue to remove the item from
 * @param[out] _item - Pointer to store the removed item
 * @return QueueResult - Result code of the operation
 * @retval QUEUE_SUCCESS - On success
 * @retval QUEUE_UNINITIALIZED_ERROR - If _queue is NULL
 * @retval QUEUE_DATA_UNINITIALIZED_ERROR - If _item is NULL
 * @retval QUEUE_DATA_NOT_FOUND_ERROR - If the queue is empty
 */
QueueResult QueueRemove(Queue* _queue, void** _item);

/**
 * @brief Check if a given queue is empty
 * @param[in] _queue - The queue to check
 * @return int - 1 if the queue is empty, 0 otherwise
 */
int QueueIsEmpty(Queue* _queue);

/**
 * @brief Apply a function to each element in the queue
 * @param[in] _queue - The queue
 * @param[in] _action - The function to apply to each element
 * @param[in] _context - Context to pass to the action function
 * @return size_t - The number of elements processed before stopping
 *
 * @details If action returns 0, the iteration stops and the function returns the number of elements processed.
 */
size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context);

#endif /* __QUEUE_H__ */

