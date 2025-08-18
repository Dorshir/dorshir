#include <stdlib.h>
#include "queue.h"

struct Queue {
    void** m_que;
    size_t m_size;
    size_t m_head;
    size_t m_tail;
    size_t m_nItems;
};

/* Create a new queue with specified size */
Queue* QueueCreate(size_t _size) {
    Queue* queue;
    if (_size == 0) {
        return NULL;
    }
    queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }
    queue->m_que = (void**)malloc(_size * sizeof(void*));
    if (!queue->m_que) {
        free(queue);
        return NULL;
    }
    queue->m_size = _size;
    queue->m_head = 0;
    queue->m_tail = 0;
    queue->m_nItems = 0;
    return queue;
}

/* Deallocate a previously created queue */
void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy) {
    size_t i;
    if (!_queue || !*_queue) {
        return;
    }
    if (_itemDestroy) {
        for (i = 0; i < (*_queue)->m_nItems; ++i) 
        {
            _itemDestroy((*_queue)->m_que[(*_queue)->m_head]);
            (*_queue)->m_head = ((*_queue)->m_head + 1) % (*_queue)->m_size;
        }
    }
    free((*_queue)->m_que);
    free(*_queue);
    *_queue = NULL;
}

/* Insert an item into the queue */
QueueResult QueueInsert(Queue* _queue, void* _item) {
    if (!_queue) {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if (!_item) {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    if (_queue->m_nItems == _queue->m_size) {
        return QUEUE_OVERFLOW_ERROR;
    }
    _queue->m_que[_queue->m_tail] = _item;
    _queue->m_tail = (_queue->m_tail + 1) % _queue->m_size;
    _queue->m_nItems++;
    return QUEUE_SUCCESS;
}

/* Remove the first item from the queue */
QueueResult QueueRemove(Queue* _queue, void** _item) {
    if (!_queue) {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if (!_item) {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    if (_queue->m_nItems == 0) {
        return QUEUE_DATA_NOT_FOUND_ERROR;
    }
    *_item = _queue->m_que[_queue->m_head];
    _queue->m_head = (_queue->m_head + 1) % _queue->m_size;
    _queue->m_nItems--;
    return QUEUE_SUCCESS;
}

/* Check if the queue is empty */
int QueueIsEmpty(Queue* _queue) {
    if (!_queue) {
        return 1; 
    }
    return (_queue->m_nItems == 0);
}

/* Apply a function to each element in the queue */
size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context) {
    size_t count = 0,i;
    if (!_queue || !_action) {
        return 0;
    }
    for (i = 0; i < _queue->m_nItems; ++i) {
        if (_action(_queue->m_que[(_queue->m_head + i) % _queue->m_size], _context) == 0)
        {
            break;
        }
        count++;
    }
    return count;
}

