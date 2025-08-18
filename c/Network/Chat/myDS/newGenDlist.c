#include <stdlib.h>
#include "newGenDlist.h"

struct Node {
    void* m_data;
    struct Node* m_next;
    struct Node* m_prev;
};

struct List {
    struct Node m_head;
    struct Node m_tail;
};

List* ListCreate(void)
{
    List* list = (List*)malloc(sizeof(List));
    if (!list)
    {
        return NULL;
    }
    list->m_head.m_data = NULL;
    list->m_tail.m_data = NULL;
    list->m_head.m_next = &list->m_tail;
    list->m_head.m_prev = &list->m_head;
    list->m_tail.m_next = &list->m_tail;
    list->m_tail.m_prev = &list->m_head;
    
    return list;
}

void ListDestroy(List** _pList, void (*_elementDestroy)(void* _item))
{
    struct Node* current;
    struct Node* next;  
    if (!_pList || !*_pList)
    {
        return;
    }    
    current = (*_pList)->m_head.m_next;
    while (current != &(*_pList)->m_tail)
    {
        next = current->m_next;
        if (_elementDestroy)
        {
            _elementDestroy(current->m_data);
        }
        free(current);
        current = next;
    }    
    free(*_pList);
    *_pList = NULL;
}

ListItr ListPushHead(List* _list, void* _item)
{
    struct Node* newNode;   
    if (!_list || !_item) 
    {
        return NULL;
    }   
    newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode)
    {
        return NULL;
    }    
    newNode->m_data = _item;
    newNode->m_prev = &_list->m_head;
    newNode->m_next = _list->m_head.m_next;
    _list->m_head.m_next->m_prev = newNode;
    _list->m_head.m_next = newNode;   
    return newNode;
}    
ListItr ListPushTail(List* _list, void* _item)
{
    struct Node* newNode;    
    if (!_list || !_item)
    {
        return NULL;
    }
   
    newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode)
    {
        return NULL;
    }   
    newNode->m_data = _item;
    newNode->m_next = &_list->m_tail;
    newNode->m_prev = _list->m_tail.m_prev;
    _list->m_tail.m_prev->m_next = newNode;
    _list->m_tail.m_prev = newNode;    
    return newNode;
}    
void* ListPopHead(List* _list)
{
    void* data;
    struct Node* nodeToRemove;    
    if (!_list || _list->m_head.m_next == &_list->m_tail)
    {
        return NULL;
    }   
    nodeToRemove = _list->m_head.m_next;
    data = nodeToRemove->m_data;
    _list->m_head.m_next = nodeToRemove->m_next;
    nodeToRemove->m_next->m_prev = &_list->m_head;   
    free(nodeToRemove);
    return data;
}    

void* ListPopTail(List* _list)
{
    void* data;
    struct Node* nodeToRemove;    
    if (!_list || _list->m_tail.m_prev == &_list->m_head)
    {
        return NULL;
    }    
    nodeToRemove = _list->m_tail.m_prev;
    data = nodeToRemove->m_data;
    nodeToRemove->m_prev->m_next = &_list->m_tail;
    _list->m_tail.m_prev = nodeToRemove->m_prev;    
    free(nodeToRemove);
    return data;
}

ListItr ListItrBegin(const List* _list)
{
    if (!_list)
    {
        return NULL;
    }
    return _list->m_head.m_next;
}

ListItr ListItrEnd(const List* _list)
{
    if (!_list)
    {
        return NULL;
    }
    return (ListItr)&_list->m_tail;
}

int ListItrEquals(const ListItr _a, const ListItr _b)
{
    return _a == _b;
}

ListItr ListItrNext(ListItr _itr)
{
    struct Node* node = (struct Node*)_itr;
    if (!node || node == node->m_next)  
    {
        return _itr;
    }
    return node->m_next;
}

ListItr ListItrPrev(ListItr _itr)
{
    struct Node* node = (struct Node*)_itr;
    if (!node || node->m_prev->m_prev == node->m_prev)  
    {
        return _itr;
    }
    return node->m_prev;
}

void* ListItrGet(ListItr _itr)
{
    struct Node* node = (struct Node*)_itr;
    if (!node || node == node->m_next || node == node->m_prev)  
    {
        return NULL;
    }
    return node->m_data;
}

void* ListItrSet(ListItr _itr, void* _element)
{
    void* oldData;
    struct Node* node = (struct Node*)_itr;
    
    if (!node || node == node->m_next || node == node->m_prev)  
    {
        return NULL;
    }   
    oldData = node->m_data;
    node->m_data = _element;
    return oldData;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
    struct Node* curr = (struct Node*)_itr;
    struct Node* newNode;   
    if (!curr)
    {
        return NULL;
    }    
    newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode)
    {
        return NULL;
    }    
    newNode->m_data = _element;
    newNode->m_prev = curr->m_prev;
    newNode->m_next = curr;
    curr->m_prev->m_next = newNode;
    curr->m_prev = newNode;   
    return newNode;
}

void* ListItrRemove(ListItr _itr)
{
    void* data;
    struct Node* node = (struct Node*)_itr;   
    if (!node || node == node->m_next || node == node->m_prev) 
    {
        return NULL;
    }   
    data = node->m_data;
    node->m_next->m_prev = node->m_prev;
    node->m_prev->m_next = node->m_next;    
    free(node);
    return data;
}
size_t ListSize(const List* _list)
{
    ListItr begin, end;
    size_t count = 0;   
    if (!_list)
    {
        return 0;
    }    
    begin = ListItrBegin(_list);
    end = ListItrEnd(_list);   
    while (!ListItrEquals(begin, end))
    {
        count++;
        begin = ListItrNext(begin);
    }    
    return count;
}
size_t ListIsEmpty(List* _list)
{
    if (!_list)
    {
        return 1;
    }
    return ListSize(_list) == 0;
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
    ListItr curr = _begin;    
    if (!_begin || !_end || !_action)
    {
        return NULL;
    }
    
    while (!ListItrEquals(curr, _end))
    {
        if (!_action(((struct Node*)curr)->m_data, _context))
        {
            break;
        }
        curr = ListItrNext(curr);
    }
    
    return curr;
}
