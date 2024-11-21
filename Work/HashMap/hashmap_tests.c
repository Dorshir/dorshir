#include "new_gen_dlist.h"
#include "hashmap.h"
#include <stdio.h>  /* printf */
#include <stdlib.h> /* dynamic memory */

static int *CreateInt(int _value);
static int IntComparator(void *_left, void *_right);

List **GetData(HashMap *_map);
HashFunction GetHashFunc(HashMap *_map);
EqualityFunction GetEqualityFunc(HashMap *_map);
size_t GetMapSize(HashMap *_map);
size_t GetMapCap(HashMap *_map);
size_t GetNItems(HashMap *_map);

size_t intHash(void *key)
{
    // Cast the void* to a pointer to uint32_t (32-bit unsigned integer)
    size_t x = *(int *)key;

    // Apply a bit-mixing hash function
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

/* HashMap_Create Tests */
static void TestHashMap_CreateValid();
static void TestHashMap_CreateNullFuncs();
static void TestHashMap_CreateZeroCapacity();
// static void TestHashMap_CreateAllocationFail();

/* HashMap_Destroy Tests */
static void TestHashMap_DestroyValid();
static void TestHashMap_DestroyNullMap();
static void TestHashMap_DestroyDoubleDestroy();

/* HashMap_Insert Tests */
static void TestHashMap_InsertFirstListInsert();
static void TestHashMap_InsertNullKeyOrMap();
static void TestHashMap_InsertNullValue();
// static void TestHashMap_InsertDuplicateKey();
// static void TestHashMap_InsertDeeperAfterHashing();

/* HashMap_Remove Tests */
static void TestHashMap_RemoveKeyExistsFirstItem();
static void TestHashMap_RemoveKeyExistsDeeperInList();
static void TestHashMap_RemoveKeyNotExists();
static void TestHashMap_RemoveFromEmptyMap();
static void TestHashMap_RemoveNullMap();
static void TestHashMap_RemoveNullKey();
static void TestHashMap_RemoveNullPointers();

/* HashMap_Find Tests */
static void TestHashMap_FindKeyExists();
static void TestHashMap_FindKeyNotExists();
static void TestHashMap_FindInEmptyMap();
static void TestHashMap_FindNullMap();
static void TestHashMap_FindNullKey();
static void TestHashMap_FindNullValuePointer();

/* HashMap_Size Tests */
static void TestHashMap_SizeEmptyMap();
static void TestHashMap_SizeAfterInsertions();
static void TestHashMap_SizeAfterRemovals();
static void TestHashMap_SizeNullMap();

int main()
{
    TestHashMap_CreateValid();
    TestHashMap_CreateNullFuncs();
    TestHashMap_CreateZeroCapacity();
    TestHashMap_DestroyValid();
    TestHashMap_DestroyNullMap();
    TestHashMap_DestroyDoubleDestroy();
    TestHashMap_InsertFirstListInsert();
    TestHashMap_InsertNullKeyOrMap();
    TestHashMap_InsertNullValue();
    TestHashMap_SizeEmptyMap();
    TestHashMap_SizeAfterInsertions();
    TestHashMap_SizeAfterRemovals();
    TestHashMap_SizeNullMap();
    TestHashMap_RemoveKeyExistsFirstItem();
    TestHashMap_RemoveKeyExistsDeeperInList();
    TestHashMap_RemoveKeyNotExists();
    TestHashMap_RemoveFromEmptyMap();
    TestHashMap_RemoveNullMap();
    TestHashMap_RemoveNullKey();
    TestHashMap_RemoveNullPointers();
    TestHashMap_FindKeyExists();
    TestHashMap_FindKeyNotExists();
    TestHashMap_FindInEmptyMap();
    TestHashMap_FindNullMap();
    TestHashMap_FindNullKey();
    TestHashMap_FindNullValuePointer();

    return 0;
}

static int *CreateInt(int _value)
{
    int *ptr = malloc(sizeof(int));
    if (ptr == NULL)
    {
        return NULL;
    }
    *ptr = _value;
    return ptr;
}

static int IntComparator(void *_left, void *_right)
{
    if (_left == NULL || _right == NULL)
    {
        return 0;
    }
    return *(int *)_left == *(int *)_right;
}

/* HashMap_Create Tests */

void TestHashMap_CreateValid()
{
    printf("TestHashMap_CreateValid: ");
    HashMap *map;
    map = HashMap_Create(4, intHash, IntComparator);
    if (map != NULL && GetMapSize(map) == 4 && GetMapCap(map) == 5 && GetNItems(map) == 0)
    {
        printf("PASS\n");
        free(map);
    }
    else
    {
        printf("FAIL\n");
    }
}

void TestHashMap_CreateNullFuncs()
{
    printf("TestHashMap_CreateNullFuncs: ");
    HashMap *map;
    map = HashMap_Create(4, NULL, IntComparator);
    if (map == NULL)
    {
        map = HashMap_Create(4, intHash, NULL);
        if (map == NULL)
        {
            printf("PASS\n");
        }
        else
        {
            printf("FAIL\n");
            free(map);
        }
    }
    else
    {
        free(map);
        printf("FAIL\n");
    }
}

void TestHashMap_CreateZeroCapacity()
{
    printf("TestHashMap_CreateZeroCapacity: ");
    HashMap *map;
    map = HashMap_Create(0, intHash, IntComparator);
    if (map == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        free(map);
        printf("FAIL\n");
    }
}

/* HashMap_Destroy Tests */

void TestHashMap_DestroyValid()
{
    printf("TestHashMap_DestroyValid: ");
    HashMap *map;
    int *val1 = CreateInt(10);
    int *key1 = CreateInt(1);
    int *val2 = CreateInt(20);
    int *key2 = CreateInt(2);
    int *val3 = CreateInt(30);
    int *key3 = CreateInt(3);
    map = HashMap_Create(5, intHash, IntComparator);

    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);
    HashMap_Insert(map, key3, val3);

    HashMap_Destroy(&map, free, free);
    if (map == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        free(map);
        printf("FAIL\n");
    }
}

void TestHashMap_DestroyNullMap()
{
    printf("TestHashMap_DestroyNullMap: ");
    HashMap *map = NULL;

    HashMap_Destroy(NULL, free, free);
    HashMap_Destroy(&map, free, free);

    printf("PASS\n");
}

void TestHashMap_DestroyDoubleDestroy()
{
    printf("TestHashMap_DestroyDoubleDestroy: ");
    HashMap *map;
    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Destroy(&map, free, free);
    HashMap_Destroy(&map, free, free);
    if (map == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        free(map);
        printf("FAIL\n");
    }
}

/* HashMap_Insert Tests */
void TestHashMap_InsertFirstListInsert()
{
    printf("TestHashMap_InsertFirstListInsert: ");
    HashMap *map;
    void *resVal; 
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    HashMap_Find(map, key1, &resVal);

    if (resVal == val1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_InsertNullKeyOrMap()
{
    printf("TestHashMap_InsertNullKeyOrMap: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);

    if (HashMap_Insert(NULL, key1, val1) == MAP_UNINITIALIZED_ERROR)
    {
        map = HashMap_Create(5, intHash, IntComparator);
        if (HashMap_Insert(map, NULL, val1) == MAP_UNINITIALIZED_ERROR && GetNItems(map) == 0)
        {
            printf("PASS\n");
        }
        else
        {
            printf("FAIL\n");
        }
        HashMap_Destroy(&map, free, free);
    }
    else
    {
        printf("FAIL\n");
    }
    free(key1);
    free(val1);
}

void TestHashMap_InsertNullValue()
{
    printf("TestHashMap_InsertNullValue: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    map = HashMap_Create(5, intHash, IntComparator);

    if (HashMap_Insert(map, key1, NULL) == MAP_SUCCESS && GetNItems(map) == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    HashMap_Destroy(&map, free, NULL); 
}

/* Test for inserting a duplicate key */
void TestHashMap_InsertDuplicateKey()
{
    printf("TestHashMap_InsertDuplicateKey: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *val2 = CreateInt(20);

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Insert(map, key1, val2) == MAP_KEY_DUPLICATE_ERROR)
    {
        printf("PASS\n");
        free(val2); 
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

/* Test for inserting into a bucket with collision */
void TestHashMap_InsertDeeperAfterHashing()
{
    printf("TestHashMap_InsertDeeperAfterHashing: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(6);
    int *val2 = CreateInt(20);

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    void *resVal1, *resVal2;
    if (HashMap_Find(map, key1, &resVal1) == MAP_SUCCESS && resVal1 == val1 &&
        HashMap_Find(map, key2, &resVal2) == MAP_SUCCESS && resVal2 == val2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

/* HashMap_Remove Tests */
void TestHashMap_RemoveKeyExistsFirstItem()
{
    printf("TestHashMap_RemoveKeyExistsFirstItem: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    int *val2 = CreateInt(20);
    void *removedKey, *removedValue;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    if (HashMap_Remove(map, key1, &removedKey, &removedValue) == MAP_SUCCESS &&
        removedKey == key1 && removedValue == val1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(removedKey);
    free(removedValue);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_RemoveKeyExistsDeeperInList()
{
    printf("TestHashMap_RemoveKeyExistsDeeperInList: ");
    HashMap *map;
    int *key1 = CreateInt(1); 
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(6); 
    int *val2 = CreateInt(20);
    void *removedKey, *removedValue;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    if (HashMap_Remove(map, key2, &removedKey, &removedValue) == MAP_SUCCESS &&
        removedKey == key2 && removedValue == val2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(removedKey);
    free(removedValue);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_RemoveKeyNotExists()
{
    printf("TestHashMap_RemoveKeyNotExists: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    void *removedKey, *removedValue;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Remove(map, key2, &removedKey, &removedValue) == MAP_KEY_NOT_FOUND_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key2);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_RemoveFromEmptyMap()
{
    printf("TestHashMap_RemoveFromEmptyMap: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    void *removedKey, *removedValue;

    map = HashMap_Create(5, intHash, IntComparator);

    if (HashMap_Remove(map, key1, &removedKey, &removedValue) == MAP_KEY_NOT_FOUND_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key1);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_RemoveNullMap()
{
    printf("TestHashMap_RemoveNullMap: ");
    int *key1 = CreateInt(1);
    void *removedKey, *removedValue;

    if (HashMap_Remove(NULL, key1, &removedKey, &removedValue) == MAP_UNINITIALIZED_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key1);
}

void TestHashMap_RemoveNullKey()
{
    printf("TestHashMap_RemoveNullKey: ");
    HashMap *map;
    void *removedKey, *removedValue;

    map = HashMap_Create(5, intHash, IntComparator);

    if (HashMap_Remove(map, NULL, &removedKey, &removedValue) == MAP_UNINITIALIZED_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}
void TestHashMap_RemoveNullPointers()
{
    printf("TestHashMap_RemoveNullPointers: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    void *dummy;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Remove(map, key1, NULL, &dummy) == MAP_UNINITIALIZED_ERROR)
    {
        if (HashMap_Remove(map, key1, &dummy, NULL) == MAP_UNINITIALIZED_ERROR)
        {
            printf("PASS\n");
        }
        else
        {
            printf("FAIL\n");
        }
    }
    else
    {
        printf("FAIL ");
    }
    HashMap_Destroy(&map, free, free);
}

/* HashMap_Find Tests */

void TestHashMap_FindKeyExists()
{
    printf("TestHashMap_FindKeyExists: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    void *foundValue;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Find(map, key1, &foundValue) == MAP_SUCCESS && foundValue == val1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

void TestHashMap_FindKeyNotExists()
{
    printf("TestHashMap_FindKeyNotExists: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    void *foundValue;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Find(map, key2, &foundValue) == MAP_KEY_NOT_FOUND_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key2);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_FindInEmptyMap()
{
    printf("TestHashMap_FindInEmptyMap: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    void *foundValue;

    map = HashMap_Create(5, intHash, IntComparator);

    if (HashMap_Find(map, key1, &foundValue) == MAP_KEY_NOT_FOUND_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key1);
    HashMap_Destroy(&map, free, free);
}

void TestHashMap_FindNullMap()
{
    printf("TestHashMap_FindNullMap: ");
    int *key1 = CreateInt(1);
    void *foundValue;

    if (HashMap_Find(NULL, key1, &foundValue) == MAP_UNINITIALIZED_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    free(key1);
}

void TestHashMap_FindNullKey()
{
    printf("TestHashMap_FindNullKey: ");
    HashMap *map;
    void *foundValue;

    map = HashMap_Create(5, intHash, IntComparator);

    if (HashMap_Find(map, NULL, &foundValue) == MAP_UNINITIALIZED_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

void TestHashMap_FindNullValuePointer()
{
    printf("TestHashMap_FindNullValuePointer: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);

    if (HashMap_Find(map, key1, NULL) == MAP_UNINITIALIZED_ERROR)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

/* HashMap_Size Tests */

void TestHashMap_SizeEmptyMap()
{
    printf("TestHashMap_SizeEmptyMap: ");
    HashMap *map;
    size_t size;

    map = HashMap_Create(5, intHash, IntComparator);
    size = HashMap_Size(map);

    if (size == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

void TestHashMap_SizeAfterInsertions()
{
    printf("TestHashMap_SizeAfterInsertions: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    int *val2 = CreateInt(20);
    size_t size;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    size = HashMap_Size(map);

    if (size == 2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

void TestHashMap_SizeAfterRemovals()
{
    printf("TestHashMap_SizeAfterRemovals: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    int *val2 = CreateInt(20);
    void *removedKey, *removedValue;
    size_t size;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    HashMap_Remove(map, key1, &removedKey, &removedValue);
    free(removedKey);
    free(removedValue);

    size = HashMap_Size(map);

    if (size == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}

void TestHashMap_SizeNullMap()
{
    printf("TestHashMap_SizeNullMap: ");
    size_t size;

    size = HashMap_Size(NULL);

    if (size == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* Optional HashMap_ForEach Test */
static int IncrementCounter(const void *_key, void *_value, void *_context)
{
    int *counter = (int *)_context;
    (*counter)++;
    return 1;
}

void TestHashMap_ForEach()
{
    printf("TestHashMap_ForEach: ");
    HashMap *map;
    int *key1 = CreateInt(1);
    int *val1 = CreateInt(10);
    int *key2 = CreateInt(2);
    int *val2 = CreateInt(20);
    int counter = 0;

    map = HashMap_Create(5, intHash, IntComparator);
    HashMap_Insert(map, key1, val1);
    HashMap_Insert(map, key2, val2);

    size_t iterated = HashMap_ForEach(map, IncrementCounter, &counter);

    if (iterated == 2 && counter == 2)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    HashMap_Destroy(&map, free, free);
}
