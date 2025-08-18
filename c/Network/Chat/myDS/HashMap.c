#include <stdlib.h>
#include "newGenDlist.h"
#include "HashMap.h"

#define MINIMUM_SIZE 5

typedef struct KeyValuePair 
{
    const void* m_key;
    const void* m_value;
} KeyValuePair;

struct HashMap {
    List** m_data;
    HashFunction m_hashFunction;
    EqualityFunction m_equalityFunction;
    size_t m_hashSize;    /* original size given by the client */
    size_t m_capacity;    /* real hash size after finding prime */
    size_t m_numOfItems;  /* number of items currently in the table */
    size_t m_numOfLists;  /* number of initialized lists */
};

typedef struct PairDestroyFuncs {
    void (*m_keyDestroy)(void* _key);
    void (*m_valDestroy)(void* _value);
} PairDestroyFuncs;

/* Function declarations for static functions */
static int IsPrime(size_t _num);
static size_t NextPrime(size_t _num);
static KeyValuePair* CreatePair(const void* _key, const void* _value);
static void DestroyPairWithContext(void* _pair, void* _context);
static void DestroyPairFunc(void* _pair);


HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
    HashMap* hashMap;
    List** temp;
    size_t hashSize;    
    if (!_hashFunc || !_keysEqualFunc || _capacity < MINIMUM_SIZE)
    {
        return NULL;
    }
    hashSize = IsPrime(_capacity) ? _capacity : NextPrime(_capacity);   
    hashMap = (HashMap*)malloc(sizeof(HashMap));
    if (!hashMap)
    {
        return NULL;
    }
    temp = (List**)calloc(hashSize, sizeof(List*));
    if (!temp)
    {
        free(hashMap);
        return NULL;
    }
    hashMap->m_data = temp;
    hashMap->m_hashFunction = _hashFunc;
    hashMap->m_equalityFunction = _keysEqualFunc;
    hashMap->m_hashSize = _capacity;
    hashMap->m_capacity = hashSize;
    hashMap->m_numOfItems = 0;
    hashMap->m_numOfLists = 0;   
    return hashMap;
}

void HashMap_Destroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
    size_t i;
    List** lists;
    PairDestroyFuncs destroyFuncs;
    if (!_map || !*_map)
    {
        return;
    }  
    lists = (*_map)->m_data;
    if (_keyDestroy || _valDestroy)
    {
        destroyFuncs.m_keyDestroy = _keyDestroy;
        destroyFuncs.m_valDestroy = _valDestroy;    
        for (i = 0; i < (*_map)->m_capacity; i++)
        {
            if (lists[i] != NULL)
            {
                ListItr begin = ListItrBegin(lists[i]);
                ListItr end = ListItrEnd(lists[i]);
                ListItr current = begin;           
                while (!ListItrEquals(current, end))
                {
                    KeyValuePair* pair = (KeyValuePair*)ListItrGet(current);
                    DestroyPairWithContext(pair, &destroyFuncs);
                    current = ListItrNext(current);
                }
                ListDestroy(&lists[i], NULL);
            }
        }
    }
    else
    {
        for (i = 0; i < (*_map)->m_capacity; i++)
        {
            if (lists[i] != NULL)
            {
                ListDestroy(&lists[i], DestroyPairFunc);
            }
        }
    }
    free((*_map)->m_data);
    free(*_map);
    *_map = NULL;
}

Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
    size_t hashIndex;
    KeyValuePair* pair;
    void* existingValue;
    if (!_map)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (!_key)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if (HashMap_Find(_map, _key, &existingValue) == MAP_SUCCESS)
    {
        return MAP_KEY_DUPLICATE_ERROR;
    }
    hashIndex = _map->m_hashFunction((void*)_key) % _map->m_capacity;
    if (!_map->m_data[hashIndex])
    {
        _map->m_data[hashIndex] = ListCreate();
        if (!_map->m_data[hashIndex])
        {
            return MAP_ALLOCATION_ERROR;
        }
        _map->m_numOfLists++;
    }
    pair = CreatePair(_key, _value);
    if (!pair)
    {
        if (ListSize(_map->m_data[hashIndex]) == 0)
        {
            ListDestroy(&_map->m_data[hashIndex], NULL);
            _map->m_data[hashIndex] = NULL;
            _map->m_numOfLists--;
        }
        return MAP_ALLOCATION_ERROR;
    }
    if (!ListPushTail(_map->m_data[hashIndex], pair))
    {
        free(pair);
        if (ListSize(_map->m_data[hashIndex]) == 0)
        {
            ListDestroy(&_map->m_data[hashIndex], NULL);
            _map->m_data[hashIndex] = NULL;
            _map->m_numOfLists--;
        }
        return MAP_ALLOCATION_ERROR;
    }
    _map->m_numOfItems++;
    return MAP_SUCCESS;
}

Map_Result HashMap_Find(const HashMap* _map, const void* _searchKey, void** _pValue)
{
    size_t hashIndex;
    ListItr begin, end, found;
    KeyValuePair* pair;
    KeyValuePair searchPair;
    if (!_map || !_pValue)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (!_searchKey)
    {
        return MAP_KEY_NULL_ERROR;
    }
    hashIndex = _map->m_hashFunction((void*)_searchKey) % _map->m_capacity;
    if (!_map->m_data[hashIndex])
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }  
    searchPair.m_key = _searchKey;
    begin = ListItrBegin(_map->m_data[hashIndex]);
    end = ListItrEnd(_map->m_data[hashIndex]);  
    while (!ListItrEquals(begin, end))
    {
        pair = (KeyValuePair*)ListItrGet(begin);
        if (_map->m_equalityFunction((void*)pair->m_key, (void*)_searchKey))
        {
            *_pValue = (void*)pair->m_value;
            return MAP_SUCCESS;
        }
        begin = ListItrNext(begin);
    }
    return MAP_KEY_NOT_FOUND_ERROR;
}

Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
    size_t hashIndex;
    ListItr begin, end, current;
    KeyValuePair* pair;
    if (!_map || !_pKey || !_pValue)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (!_searchKey)
    {
        return MAP_KEY_NULL_ERROR;
    }
    hashIndex = _map->m_hashFunction((void*)_searchKey) % _map->m_capacity;
    
    if (!_map->m_data[hashIndex])
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    begin = ListItrBegin(_map->m_data[hashIndex]);
    end = ListItrEnd(_map->m_data[hashIndex]);
    current = begin;
    while (!ListItrEquals(current, end))
    {
        pair = (KeyValuePair*)ListItrGet(current);
        if (_map->m_equalityFunction((void*)pair->m_key, (void*)_searchKey))
        {
            *_pKey = (void*)pair->m_key;
            *_pValue = (void*)pair->m_value;
            ListItrRemove(current);
            free(pair);
            if (ListSize(_map->m_data[hashIndex]) == 0)
            {
                ListDestroy(&_map->m_data[hashIndex], NULL);
                _map->m_data[hashIndex] = NULL;
                _map->m_numOfLists--;
            }
            _map->m_numOfItems--;
            return MAP_SUCCESS;
        }
        current = ListItrNext(current);
    }
    return MAP_KEY_NOT_FOUND_ERROR;
}

size_t HashMap_Size(const HashMap* _map)
{
    if (!_map)
    {
        return 0;
    }
    return _map->m_numOfItems;
}

size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
    size_t i, count = 0;
    ListItr begin, end, current;
    KeyValuePair* pair;   
    if (!_map || !_action)
    {
        return 0;
    }  
    for (i = 0; i < _map->m_capacity; i++)
    {
        if (_map->m_data[i])
        {
            begin = ListItrBegin(_map->m_data[i]);
            end = ListItrEnd(_map->m_data[i]);
            current = begin;       
            while (!ListItrEquals(current, end))
            {
                pair = (KeyValuePair*)ListItrGet(current);
                if (!_action(pair->m_key, (void*)pair->m_value, _context))
                {
                    return count;
                }
                count++;
                current = ListItrNext(current);
            }
        }
    }
    return count;
}

/* Helper functions  */

static int IsPrime(size_t _num)
{
    size_t i;   
    if (_num < 2)
    {
        return 0;
    }   
    for (i = 2; i * i <= _num; i++)
    {
        if (_num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

static size_t NextPrime(size_t _num)
{
    while (!IsPrime(_num))
    {
        _num++;
    }
    return _num;
}

static KeyValuePair* CreatePair(const void* _key, const void* _value)
{
    KeyValuePair* pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    if (pair)
    {
        pair->m_key = _key;
        pair->m_value = _value;
    }
    return pair;
}

static void DestroyPairFunc(void* _pair)
{
    if (_pair)
    {
        free(_pair);
    }
}

static void DestroyPairWithContext(void* _pair, void* _context)
{
    KeyValuePair* pair = (KeyValuePair*)_pair;
    PairDestroyFuncs* funcs = (PairDestroyFuncs*)_context;
    
    if (funcs->m_keyDestroy)
    {
        funcs->m_keyDestroy((void*)pair->m_key);
    }
    if (funcs->m_valDestroy)
    {
        funcs->m_valDestroy((void*)pair->m_value);
    }
    free(pair);
}


















