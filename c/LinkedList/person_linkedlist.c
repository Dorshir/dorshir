#include "person_linkedlist.h"
#include <stdlib.h> /* NULL */
#include <stdio.h>  /* printf */

/**************************** Help Functions Declerations ****************************/

static void PrintPerson(Person *_p);

/**************************** Main Functions ****************************/

Person *ListInsertHead(Person *_head, Person *_p)
{
    if (_p == NULL)
    {
        return NULL;
    }

    _p->m_next = _head;
    return _p;
}

Person *ListRemoveHead(Person *_head, Person **_item)
{
    Person *temp;
    if (_item == NULL)
    {
        return NULL;
    }
    else if (_head == NULL)
    {
        *_item = NULL;
        return _head;
    }
    *_item = _head;
    temp = _head->m_next;
    (*_item)->m_next = NULL;

    return temp;
}

Person *ListInsertByKey(Person *_head, int _key, Person *_p)
{
    Person *prev, *current;
    if (_p == NULL)
    {
        return _head;
    }
    /* Insert at head */
    if (_head == NULL || _head->m_id > _key)
    {
        _p->m_next = _head;
        return _p;
    }
    /* Duplicate key, no insertion */
    if (_head->m_id == _key)
    {
        return _head;
    }

    prev = _head, current = _head->m_next;
    while (current != NULL)
    {
        /* Duplicate key. no insertion */
        if (current->m_id == _key)
        {
            return _head;
        }
        /*  Found the spot (prev). insert */
        else if (current->m_id > _key)
        {
            _p->m_next = current;
            prev->m_next = _p;
            return _head;
        }

        current = current->m_next;
        prev = prev->m_next;
    }
    /* Reached the end of the list. insert */
    prev->m_next = _p;
    _p->m_next = NULL;
    return _head;
}

Person *ListInsertByKeyRec(Person *_head, int _key, Person *_p)
{
    if (_p == NULL)
    {
        return _head;
    }
    /* Empty list or current key > new key or end of the list, INSERT */
    else if (_head == NULL || _head->m_id > _key)
    {
        _p->m_next = _head;
        return _p;
    }
    /* Duplicate, do not insert */
    else if (_head->m_id == _key)
    {
        return _head;
    }
    else
    {
        _head->m_next = ListInsertByKeyRec(_head->m_next, _key, _p);
        return _head;
    }
}

Person *ListRemoveByKey(Person *_head, int _key, Person **_p)
{
    Person *prev, *current;
    if (_p == NULL || _head == NULL)
    {
        return _head;
    }
    /* Head has the desired key. Remove */
    if (_head->m_id == _key)
    {
        *_p = _head;
        _head = _head->m_next;
        (*_p)->m_next = NULL;
        return _head;
    }

    prev = _head, current = _head->m_next;
    while (current != NULL)
    {
        /* Found has the desired key. Remove */
        if (current->m_id == _key)
        {
            *_p = current;
            prev->m_next = current->m_next;
            (*_p)->m_next = NULL;
            return _head;
        }
        current = current->m_next;
        prev = prev->m_next;
    }
    *_p = NULL;
    return _head;
}

Person *ListRemoveByKeyRec(Person *_head, int _key, Person **_p)
{
    if (_p == NULL)
    {
        return _head;
    }
    /* Empty list or key is greater */
    else if (_head == NULL || _head->m_id > _key)
    {
        return NULL;
    }
    /* Found key. remove */
    else if (_head->m_id == _key)
    {
        *_p = _head;
        return _head->m_next;
    }
    else
    {
        _head->m_next = ListRemoveByKeyRec(_head->m_next, _key, _p);
        return _head;
    }
}

void PrintList(Person *_head)
{
    if (_head == NULL)
    {
        return;
    }

    while (_head != NULL)
    {
        PrintPerson(_head);
        _head = _head->m_next;
    }
}

/**************************** Help Functions ****************************/

static void PrintPerson(Person *_p)
{
    printf("Name: %s\nID: %d\nAge: %d\n\n", _p->m_name, _p->m_id, _p->m_age);
}