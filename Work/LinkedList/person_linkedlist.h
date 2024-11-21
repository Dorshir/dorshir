#ifndef __PERSON_LINKEDLIST_H__
#define __PERSON_LINKEDLIST_H__


typedef struct Person
{
    int m_id; /* Primary Key */
    char m_name[128];
    int m_age;
    struct Person *m_next;
} Person;

/**
 * @brief Insert a new person at the head of the list.
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[in] _p - Pointer to the person to insert.
 *
 * @return Pointer to the new head of the list (should be _p if insertion accomplished).
 */
Person *ListInsertHead(Person *_head, Person *_p);

/**
 * @brief Remove the head person from the list.
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[out] _item - Pointer to store the removed person.
 *
 * @return Pointer to the new head of the list.
 */
Person *ListRemoveHead(Person *_head, Person **_item);

/**
 * @brief Insert a person into the list, by key (non recursive). No duplicate key allowed.
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[in] _key - The key (Person ID) to insert by.
 * @param[in] _p - Pointer to the person to insert.
 *
 * @return Pointer to the head of the list.
 *
 */
Person *ListInsertByKey(Person *_head, int _key, Person *_p);

/**
 * @brief Insert a person into the list, by key (recursive). No duplicate key allowed.
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[in] _key - The key (Person ID) to insert by.
 * @param[in] _p - Pointer to the person to insert.
 *
 * @return Pointer to the head of the list.
 */
Person *ListInsertByKeyRec(Person *_head, int _key, Person *_p);

/**
 * @brief Remove a person from the list by key (non recursive).
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[in] _key - The key (Person ID) of the person to remove.
 * @param[out] _p - Pointer to store the removed person.
 *
 * @return Pointer to the head of the list.
 */
Person *ListRemoveByKey(Person *_head, int _key, Person **_p);

/**
 * @brief Remove a person from the list by key (recursive).
 *
 * @param[in] _head - Pointer to the head of the list.
 * @param[in] _key - The key (Person ID) of the person to remove.
 * @param[out] _p - Pointer to store the removed person.
 *
 * @return Pointer to the head of the list.
 */
Person *ListRemoveByKeyRec(Person *_head, int _key, Person **_p);

/**
 * @brief Print the entire person list.
 *
 * @param[in] _head - Pointer to the head of the list.
 */
void PrintList(Person *_head);

#endif /* #ifndef __PERSON_LINKEDLIST_H__ */