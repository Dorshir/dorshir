#include "genQueue.h"
#include <stdlib.h> /* size_t, dynamic memory */
#include "genQueue.h"

struct Queue
{
    void **m_que;
    size_t m_size;
    size_t m_head; /* Index of head in m_que. */
    size_t m_tail; /* Index of tail in m_que. */
    size_t m_nItems;
};

Queue *QueueCreate(size_t _size)
{
    Queue *queue;
    if (_size == 0)
    {
        return NULL;
    }

    queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL)
    {
        return NULL;
    }

    queue->m_head = 0;
    queue->m_tail = 0;
    queue->m_nItems = 0;
    queue->m_size = _size;
    queue->m_que = malloc(_size * sizeof(void *));
    if (queue->m_que == NULL)
    {
        free(queue);
        return NULL;
    }
    return queue;
}

void QueueDestroy(Queue **_queue, DestroyItem _itemDestroy)
{
    size_t index;
    if (_queue == NULL || *_queue == NULL)
    {
        return;
    }

    if (_itemDestroy != NULL)
    {
        for (index = (*_queue)->m_head; index < ((*_queue)->m_nItems); index++)
        {
            _itemDestroy((*_queue)->m_que[index % (*_queue)->m_size]);
        }
    }
    free(*_queue);
    *_queue = NULL;
}

QueueResult QueueInsert(Queue *_queue, void *_item)
{
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    else if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    else if (_queue->m_nItems == _queue->m_size)
    {
        return QUEUE_OVERFLOW_ERROR;
    }

    _queue->m_que[_queue->m_tail++] = _item;
    _queue->m_tail = _queue->m_tail % _queue->m_size;
    ++_queue->m_nItems;
    return QUEUE_SUCCESS;
}

QueueResult QueueRemove(Queue *_queue, void **_item)
{
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    else if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    else if (_queue->m_nItems == 0)
    {
        return QUEUE_DATA_NOT_FOUND_ERROR;
    }

    *_item = _queue->m_que[_queue->m_head];
    ++_queue->m_head;
    --_queue->m_nItems;
    return QUEUE_SUCCESS;
}

size_t QueueIsEmpty(Queue *_queue)
{
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    return _queue->m_nItems == 0;
}

size_t QueueForEach(Queue *_queue, ActionFunction _action, void *_context)
{
    size_t index;
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    else if (_action == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }

    for (index = _queue->m_head; index < _queue->m_nItems; index++)
    {
        void *element = _queue->m_que[index % _queue->m_size];
        if (_action(element, _context) == 0)
        {
            break;
        }
    }
    return index;
}
