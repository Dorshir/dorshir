#include "new_gen_dlist.h"
#include <stdlib.h> /* Dynamic Memory */

#define PREV_ITR(itr) ((Node *)_itr)->m_prev
#define IS_BEGIN(itr) (PREV_ITR(itr) == (PREV_ITR(itr)->m_prev))

typedef struct Node Node;

struct Node
{
    void *m_data;
    Node *m_next;
    Node *m_prev;
};

struct List
{
    Node m_head;
    Node m_tail;
};

static ListItr InsertByRefPoint(Node *_refNode, Node *_newNode)
{
    _newNode->m_prev = _refNode->m_prev;
    _newNode->m_next = _refNode;
    _refNode->m_prev->m_next = _newNode;
    _refNode->m_prev = _newNode;
    return _newNode;
}

List *ListCreate(void)
{
    List *newList;
    newList = malloc(sizeof(List));
    if (newList == NULL)
    {
        return NULL;
    }

    newList->m_head.m_data = NULL;
    newList->m_head.m_prev = &newList->m_head;
    newList->m_head.m_next = &newList->m_tail;

    newList->m_tail.m_data = NULL;
    newList->m_tail.m_prev = &newList->m_head;
    newList->m_tail.m_next = &newList->m_tail;

    return newList;
}

void ListDestroy(List **_pList, void (*_elementDestroy)(void *_item))
{
    Node *curr, *temp;
    if (*_pList == NULL || _pList == NULL)
    {
        return;
    }

    curr = (*_pList)->m_head.m_next;
    while (curr != &(*_pList)->m_tail)
    {
        if (_elementDestroy != NULL)
        {
            _elementDestroy(curr->m_data);
        }
        temp = curr;
        curr = curr->m_next;
        free(temp);
    }
    free(*_pList);
    *_pList = NULL;
}

ListItr ListPushHead(List *_list, void *_item)
{
    Node *_newNode;
    if (_item == NULL || _list == NULL)
    {
        return NULL;
    }
    _newNode = malloc(sizeof(Node));
    if (_newNode == NULL)
    {
        return NULL;
    }
    _newNode->m_data = _item;

    return InsertByRefPoint(_list->m_head.m_next, _newNode);
}

ListItr ListPushTail(List *_list, void *_item)
{
    Node *_newNode;
    if (_item == NULL || _list == NULL)
    {
        return NULL;
    }
    _newNode = malloc(sizeof(Node));
    if (_newNode == NULL)
    {
        return NULL;
    }
    _newNode->m_data = _item;

    return InsertByRefPoint(&_list->m_tail, _newNode);
}

void *ListPopHead(List *_list)
{
    void *popped;
    if (_list == NULL || _list->m_head.m_next == &_list->m_tail)
    {
        popped = NULL;
    }
    else
    {
        popped = _list->m_head.m_next->m_data;
        _list->m_head.m_next->m_prev = &_list->m_head;
        _list->m_head.m_next = _list->m_head.m_next->m_next;
    }
    return popped;
}

void *ListPopTail(List *_list)
{
    void *popped;
    if (_list == NULL || _list->m_tail.m_prev == &_list->m_head)
    {
        popped = NULL;
    }
    else
    {
        popped = _list->m_tail.m_prev->m_data;
        _list->m_tail.m_prev->m_prev->m_next = &_list->m_tail;
    }
    return popped;
}

ListItr ListItrBegin(const List *_list)
{
    if (_list == NULL)
    {
        return NULL;
    }
    return _list->m_head.m_next;
}

ListItr ListItrEnd(const List *_list)
{
    if (_list == NULL)
    {
        return NULL;
    }
    return (void*)&_list->m_tail;
}

int ListItrEquals(const ListItr _a, const ListItr _b)
{
    return _a == _b;
}

ListItr ListItrNext(ListItr _itr)
{
    if (_itr == NULL)
    {
        return NULL;
    }
    return ((Node *)_itr)->m_next;
}

ListItr ListItrPrev(ListItr _itr)
{
    if (_itr == NULL)
    {
        return NULL;
    }
    if (IS_BEGIN(_itr))
    {
        return _itr;
    }
    return PREV_ITR(_itr);
}

void *ListItrGet(ListItr _itr)
{
    void *ret;
    if (_itr == NULL || ((Node *)_itr)->m_next == _itr)
    {
        ret = NULL;
    }
    else
    {
        ret = ((Node *)_itr)->m_data;
    }
    return ret;
}

void *ListItrSet(ListItr _itr, void *_element)
{
    void *ret;
    if (_itr == NULL || _element == NULL || ((Node *)_itr)->m_next == _itr)
    {
        ret = NULL;
    }
    else
    {
        ret = ((Node *)_itr)->m_data;
        ((Node *)_itr)->m_data = _element;
    }
    return ret;
}

ListItr ListItrInsertBefore(ListItr _itr, void *_element)
{
    Node *newNode;
    if (_itr == NULL || _element == NULL)
    {
        return NULL;
    }

    newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return NULL;
    }
    newNode->m_data = _element;
    return InsertByRefPoint((Node *)_itr, newNode);
}

void *ListItrRemove(ListItr _itr)
{
    void *popped;
    if (_itr == NULL || ((Node *)_itr)->m_next == _itr)
    {
        popped = NULL;
    }
    else
    {
        popped = ((Node *)_itr)->m_data;
        ((Node *)_itr)->m_prev->m_next = ((Node *)_itr)->m_next;
    }
    return popped;
}

size_t ListSize(const List *_list)
{
    Node *curr;
    size_t counter = 0;
    if (_list == NULL || _list->m_tail.m_prev == &_list->m_head)
    {
        return 0;
    }
    curr = _list->m_head.m_next;
    while (curr != &_list->m_tail)
    {
        ++counter;
        curr = curr->m_next;
    }
    return counter;
}

size_t ListIsEmpty(List *_list)
{
    return (_list == NULL || _list->m_tail.m_prev == &_list->m_head);
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void *_context)
{
    Node *curr;
    if (_begin == NULL || _end == NULL || _action == NULL)
    {
        return NULL;
    }
    curr = _begin;
    while (curr != _end)
    {
        if (_action(curr->m_data, _context) == 0)
        {
            break;
        }
        curr = curr->m_next;
    }
    return curr;
}

size_t GetSizeOfStruct()
{
    return sizeof(List);
}