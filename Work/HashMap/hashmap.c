#include "hashmap.h"
#include "new_gen_dlist.h"
#include <math.h>   /* sqrt */
#include <stdlib.h> /* malloc, calloc, free */

#define CURR_LIST(map, index) (map->m_data[index])
#define FIND_KEY(map, index, key, findContext) (ListItrForEach(ListItrBegin(CURR_LIST(map, index)), ListItrEnd(CURR_LIST(map, index)), FindKeyFunction, findContext))
#define LIST_END_ITR(map, index) (ListItrEnd(CURR_LIST(map, index)))
#define PACKED_KEY(element) (((KeyValuePack *)element)->m_key)
#define PACKED_VALUE(element) (((KeyValuePack *)element)->m_value)
#define DESTROY_LIST_PACKS(map, index, dFuncs) (ListItrForEach(ListItrBegin(CURR_LIST(map, index)), ListItrEnd(CURR_LIST(map, index)), DestroyPack, dFuncs))
#define LIST_FOR_EACH(map, index, action, context) (ListItrForEach(ListItrBegin(CURR_LIST(map, index)), ListItrEnd(CURR_LIST(map, index)), action, context))
#define HASHED_INDEX(map, key) (map->m_hashFunction((void *)key) % map->m_capacity)
#define IS_DUPLICATED_KEY(map, index, key,findContext) (FIND_KEY(map, index, key) != LIST_END_ITR(map, index))
#define UNINITIALIZED_LIST(map, index) (CURR_LIST(map, index) == NULL)

struct HashMap
{
    List **m_data;
    size_t (*m_hashFunction)(void *_data);
    int (*m_equalityFunction)(void *_firstData, void *_secondData);
    size_t m_hashSize;
    size_t m_capacity;
    size_t m_numOfItems;
};

typedef struct KeyValuePack
{
    const void *m_key;
    void *m_value;
} KeyValuePack;

typedef struct DestroyContext
{
    void (*valDestroy)(void *_value);
    void (*keyDestroy)(void *_key);
} DestroyContext;

typedef struct FindContext
{
    const void *m_key;
    int (*m_equalityFunction)(void *_firstData, void *_secondData);
} FindContext;

static int FindKeyFunction(void *_element, void *_context);
static void RemoveElement(ListItr itr, void **_pKey, void **_pValue);
static int DestroyPack(void *_packedElem, void *packedFuncs);
static int IsPrime(int num);
static int FindNextPrime(int num);
static Map_Result Pack(KeyValuePack **packed, const void *_key, const void *_value);

/* API FUNCTIONS */

HashMap *HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
    HashMap *newMap;
    if (_capacity == 0 || _hashFunc == NULL || _keysEqualFunc == NULL)
    {
        return NULL;
    }

    newMap = malloc(sizeof(HashMap));
    if (newMap == NULL)
    {
        return NULL;
    }

    newMap->m_capacity = FindNextPrime(_capacity);
    newMap->m_data = calloc(newMap->m_capacity, sizeof(List *));
    if (newMap->m_data == NULL)
    {
        free(newMap);
        return NULL;
    }
    newMap->m_equalityFunction = _keysEqualFunc;
    newMap->m_hashFunction = _hashFunc;
    newMap->m_hashSize = _capacity;
    newMap->m_numOfItems = 0;

    return newMap;
}

void HashMap_Destroy(HashMap **_map, void (*_keyDestroy)(void *_key), void (*_valDestroy)(void *_value))
{
    size_t index;
    DestroyContext dFuncs;
    if (_map == NULL || *_map == NULL)
    {
        return;
    }
    dFuncs.valDestroy = _valDestroy;
    dFuncs.keyDestroy = _keyDestroy;

    
    for (index = 0; index < (*_map)->m_capacity; index++)
    {
        if (CURR_LIST((*_map),index) != NULL)
        {
            ListItr begin = ListItrBegin(CURR_LIST((*_map),index));
            ListItr end = ListItrEnd(CURR_LIST((*_map),index));

            while (!ListItrEquals(begin, end))
            {
                KeyValuePack *pack = (KeyValuePack *)ListItrGet(begin);
                ListItr next = ListItrNext(begin);

                DestroyPack(pack, &dFuncs);

                ListItrRemove(begin);

                begin = next;
            }

            ListDestroy(&(CURR_LIST((*_map),index)), NULL);
        }
    }

    free((*_map)->m_data);
    free(*_map);
    *_map = NULL;
}

Map_Result HashMap_Insert(HashMap *_map, const void *_key, const void *_value)
{
    size_t index;
    KeyValuePack *packed;
    ListItr foundItr;

    if (_map == NULL || _key == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }

    index = HASHED_INDEX(_map,_key);

    if (CURR_LIST(_map,index) == NULL)
    {
        CURR_LIST(_map,index) = ListCreate();
        if (CURR_LIST(_map,index) == NULL)
        {
            return MAP_ALLOCATION_ERROR;
        }
    }
    else
    {
        FindContext findContext = { _key, _map->m_equalityFunction };

        foundItr = ListItrForEach(
            ListItrBegin(CURR_LIST(_map,index)),
            ListItrEnd(CURR_LIST(_map,index)),
            FindKeyFunction,
            &findContext
        );

        if (!ListItrEquals(foundItr, ListItrEnd(CURR_LIST(_map,index))))
        {
            return MAP_KEY_DUPLICATE_ERROR;
        }
    }

    if (Pack(&packed, _key, _value) != MAP_SUCCESS)
    {
        return MAP_ALLOCATION_ERROR;
    }

    if (ListPushHead(CURR_LIST(_map,index), packed) == NULL)
    {
        free(packed);
        return MAP_ALLOCATION_ERROR;
    }

    ++(_map->m_numOfItems);
    return MAP_SUCCESS;
}

Map_Result HashMap_Remove(HashMap *_map, const void *_searchKey, void **_pKey, void **_pValue)
{
    size_t index;
    ListItr itr;
    FindContext findContext;

    if (_map == NULL || _searchKey == NULL || _pKey == NULL || _pValue == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }

    index = HASHED_INDEX(_map,_searchKey);

    if (CURR_LIST(_map,index) == NULL)
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }

    findContext.m_key = _searchKey;
    findContext.m_equalityFunction = _map->m_equalityFunction;

    itr = ListItrForEach(
        ListItrBegin(CURR_LIST(_map,index)),
        ListItrEnd(CURR_LIST(_map,index)),
        FindKeyFunction,
        &findContext
    );

    if (!ListItrEquals(itr, ListItrEnd(CURR_LIST(_map,index))))
    {
        RemoveElement(itr, _pKey, _pValue);
        --(_map->m_numOfItems);
        return MAP_SUCCESS;
    }

    return MAP_KEY_NOT_FOUND_ERROR;
}

Map_Result HashMap_Find(const HashMap *_map, const void *_key, void **_pValue)
{
    size_t index;
    ListItr itr;
    FindContext findContext;

    if (_map == NULL || _key == NULL || _pValue == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }

    index = HASHED_INDEX(_map,_key);

    if (CURR_LIST(_map,index) == NULL)
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }

    findContext.m_key = _key;
    findContext.m_equalityFunction = _map->m_equalityFunction;

    itr = ListItrForEach(
        ListItrBegin(CURR_LIST(_map,index)),
        ListItrEnd(CURR_LIST(_map,index)),
        FindKeyFunction,
        &findContext
    );

    if (!ListItrEquals(itr, ListItrEnd(CURR_LIST(_map,index))))
    {
        KeyValuePack *pack = (KeyValuePack *)ListItrGet(itr);
        *_pValue = pack->m_value;
        return MAP_SUCCESS;
    }

    return MAP_KEY_NOT_FOUND_ERROR;
}

size_t HashMap_Size(const HashMap *_map)
{
    if (_map == NULL)
    {
        return 0;
    }
    return _map->m_numOfItems;
}

size_t HashMap_ForEach(const HashMap *_map, KeyValueActionFunction _action, void *_context)
{
    size_t index, actionCount = 0;

    if (_map == NULL || _action == NULL)
    {
        return 0;
    }

    for (index = 0; index < _map->m_capacity; index++)
    {
        if (_map->m_data[index] != NULL)
        {
            ListItr begin = ListItrBegin(_map->m_data[index]);
            ListItr end = ListItrEnd(_map->m_data[index]);

            while (!ListItrEquals(begin, end))
            {
                KeyValuePack *pack = (KeyValuePack *)ListItrGet(begin);

                if (!_action(pack->m_key, pack->m_value, _context))
                {
                    return actionCount;
                }

                begin = ListItrNext(begin);
                ++actionCount;
            }
        }
    }

    return actionCount;
}

/* HELPER FUNCTIONS */

static void RemoveElement(ListItr _itr, void **_pKey, void **_pValue)
{
    KeyValuePack *pack = ListItrGet(_itr);
    *_pKey = (void *)pack->m_key;
    *_pValue = (void *)pack->m_value;
    ListItrRemove(_itr);
    free(pack);
}

static int DestroyPack(void *_packedElem, void *_context)
{
    DestroyContext *dFuncs = (DestroyContext *)_context;
    KeyValuePack *pack = (KeyValuePack *)_packedElem;

    if (dFuncs->keyDestroy != NULL && pack->m_key != NULL)
    {
        dFuncs->keyDestroy((void *)pack->m_key);
    }

    if (dFuncs->valDestroy != NULL && pack->m_value != NULL)
    {
        dFuncs->valDestroy(pack->m_value);
    }

    free(pack);
    return 1; // Continue iteration
}

static int IsPrime(int _num)
{
    int i;
    int bound;
    if (_num < 2)
    {
        return 0;
    }
    if (_num == 2)
    {
        return 1;
    }

    bound = sqrt(_num);

    for (i = 2; i <= bound; i++)
    {
        if (_num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

static int FindNextPrime(int _num)
{
    while (!IsPrime(_num))
    {
        ++_num;
    }
    return _num;
}

static Map_Result Pack(KeyValuePack **_packed, const void *_key, const void *_value)
{
    *_packed = malloc(sizeof(KeyValuePack));

    if (*_packed == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }

    (*_packed)->m_key = (void *)_key;
    (*_packed)->m_value = (void *)_value;
    return MAP_SUCCESS;
}

static int FindKeyFunction(void *_element, void *_context)
{
    KeyValuePack *packed = (KeyValuePack *)_element;
    FindContext *context = (FindContext *)_context;

    if (context->m_equalityFunction((void *)packed->m_key, (void *)context->m_key))
    {
        return 0;
    }
    return 1;
}

/* GETTERS */

List **GetData(HashMap *_map)
{
    return _map->m_data;
}
HashFunction GetHashFunc(HashMap *_map)
{
    return _map->m_hashFunction;
}
EqualityFunction GetEqualityFunc(HashMap *_map)
{
    return _map->m_equalityFunction;
}
size_t GetMapSize(HashMap *_map)
{
    return _map->m_hashSize;
}
size_t GetMapCap(HashMap *_map)
{
    return _map->m_capacity;
}
size_t GetNItemsHash(HashMap *_map)
{
    return _map->m_numOfItems;
}
