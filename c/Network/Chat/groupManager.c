#include "HashMap.h"
#include "queue.h"
#include "groupManager.h"
#include "group.h"
#include <stdlib.h> /* malloc, free, NULL */
#include <string.h> /* strcmp, strlen, strncpy */
#include <stdio.h>  /* snprintf */

#define BUFFER_SIZE 4096
#define MAX_NUM_OF_GROUPS 100
#define ADDR_SIZE 16
#define INITIAL_TAKEN_ADDR 0

#define BASE_ADDR_MC "224.0.0."

struct GroupManager
{
    HashMap *m_groups;
    Queue *m_mcAdresses;
    size_t m_numOfTakenAddr;
};

/* Helpers Declerations */

static GroupManagerResult InputCheckCreateGroup(GroupManager *_manager, char *_groupName);
static void DestroyGroupWrapper(void *_value);
static size_t HashFuncGroupName(void *_groupName);
static int GroupNameComperator(void *_first, void *_second);
static int GroupListBuilder(const void *_key, void *_value, void *_context);
static GroupManagerResult InitialMulticastAddresses(GroupManager *_manager);

/* API Functions */

GroupManager *GroupManager_Create()
{
    GroupManager *newGroupManager = malloc(sizeof(GroupManager));
    if (newGroupManager == NULL)
    {
        return NULL;
    }

    newGroupManager->m_groups = HashMap_Create(MAX_NUM_OF_GROUPS, HashFuncGroupName, GroupNameComperator);
    if (newGroupManager->m_groups == NULL)
    {
        free(newGroupManager);
        return NULL;
    }

    newGroupManager->m_mcAdresses = QueueCreate(MAX_NUM_OF_GROUPS);
    if (newGroupManager->m_mcAdresses == NULL)
    {
        HashMap_Destroy(&newGroupManager->m_groups, NULL, NULL);
        free(newGroupManager);
        return NULL;
    }

    if (InitialMulticastAddresses(newGroupManager) != GROUP_MANAGER_SUCCESS)
    {
        GroupManager_Destroy(&newGroupManager);
    }

    newGroupManager->m_numOfTakenAddr = INITIAL_TAKEN_ADDR;

    return newGroupManager;
}

GroupManagerResult GroupManager_CreateGroup(GroupManager *_manager, char *_groupName)
{
    Group *dummy, *newGroup;
    char *address, *groupNameKey;
    size_t groupNameLen;

    GroupManagerResult status = InputCheckCreateGroup(_manager, _groupName);
    if (status != GROUP_MANAGER_SUCCESS)
    {
        return status;
    }

    groupNameLen = strlen(_groupName);
    if (groupNameLen == 0)
    {
        return GROUP_MANAGER_INVALID_GROUP_NAME;
    }

    if (HashMap_Find(_manager->m_groups, _groupName, (void **)&dummy) == MAP_SUCCESS)
    {
        return GROUP_MANAGER_GROUP_ALREADY_EXISTS;
    }

    if (QueueRemove(_manager->m_mcAdresses, (void **)&address) != QUEUE_SUCCESS)
    {
        return GROUP_MANAGER_ADDRESS_RECEIVE_FAILED;
    }

    groupNameKey = malloc(sizeof(char) * (groupNameLen + 1));
    if (groupNameKey == NULL)
    {
        return GROUP_MANAGER_ALLOCATION_FAILED;
    }

    strncpy(groupNameKey, _groupName, groupNameLen);
    groupNameKey[groupNameLen] = '\0';

    newGroup = Group_Create(_groupName, address);
    if (newGroup == NULL)
    {
        free(groupNameKey);
        QueueInsert(_manager->m_mcAdresses, address);
        return GROUP_MANAGER_ALLOCATION_FAILED;
    }

    if (HashMap_Insert(_manager->m_groups, groupNameKey, newGroup) != MAP_SUCCESS)
    {
        free(groupNameKey);
        QueueInsert(_manager->m_mcAdresses, address);
        return GROUP_MANAGER_GROUP_CREATE_FAILED;
    }

    ++(_manager->m_numOfTakenAddr);

    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroupManager_JoinGroup(GroupManager *_manager, char *_groupName)
{
    Group *foundGroup;
    if (_manager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED_ERROR;
    }

    if (HashMap_Find(_manager->m_groups, _groupName, (void **)&foundGroup) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_GROUP_NOT_FOUND;
    }

    if (Group_IncrementMembers(foundGroup) != GROUP_SUCCESS)
    {
        return GROUP_MANAGER_INC_MEMBERS_ERROR;
    }

    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroupManager_LeaveGroup(GroupManager *_manager, char *_groupName)
{
    Group *foundGroup;
    Group *removedGroup;
    char *removedGroupName;
    GroupResult res;
    if (_manager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED_ERROR;
    }

    if (HashMap_Find(_manager->m_groups, _groupName, (void **)&foundGroup) != MAP_SUCCESS)
    {
        return GROUP_MANAGER_GROUP_NOT_FOUND;
    }

    res = Group_DecrementMembers(foundGroup);
    if (res == GROUP_EMPTY)
    {
        if (HashMap_Remove(_manager->m_groups, _groupName, (void **)&removedGroupName, (void **)&removedGroup) != MAP_SUCCESS)
        {
            return GROUP_MANAGER_GROUP_NOT_FOUND;
        }

        char *addr = Group_GetAddress(removedGroup);
        Group_Destroy(&removedGroup);
        free(removedGroupName);

        if (QueueInsert(_manager->m_mcAdresses, addr) != QUEUE_SUCCESS)
        {
            return GROUP_MANAGER_ADDRESS_ADD_FAILED;
        }
    }

    else if (res == GROUP_UNINITIALIZED_ERROR)
    {
        return GROUP_MANAGER_UNINITIALIZED_ERROR;
    }

    return GROUP_MANAGER_SUCCESS;
}

void GroupManager_Destroy(GroupManager **_groupManager)
{
    if (_groupManager == NULL || *_groupManager == NULL)
    {
        return;
    }
    if ((*_groupManager)->m_groups != NULL)
    {
        HashMap_Destroy(&(*_groupManager)->m_groups, free, DestroyGroupWrapper);
    }

    if ((*_groupManager)->m_mcAdresses != NULL)
    {
        QueueDestroy(&(*_groupManager)->m_mcAdresses, free);
    }
    free(*_groupManager);
    *_groupManager = NULL;
}

char *GroupManager_GetGroupAddress(GroupManager *_manager, const char *_groupName)
{
    Group *foundGroup;
    if (_manager == NULL || _groupName == NULL)
    {
        return NULL;
    }
    if (HashMap_Find(_manager->m_groups, _groupName, (void **)&foundGroup) != MAP_SUCCESS)
    {
        return NULL;
    }
    return Group_GetAddress(foundGroup);
}

GroupManagerResult GroupManager_GetActiveGroups(GroupManager *_manager, char *_buffer, size_t _maxSize)
{
    if (_manager == NULL || _buffer == NULL || _maxSize == 0)
    {
        return GROUP_MANAGER_UNINITIALIZED_ERROR;
    }
    _buffer[0] = '\0';

    if (HashMap_Size(_manager->m_groups) == 0)
    {
        return GROUP_MANAGER_SUCCESS;
    }
    if (HashMap_ForEach(_manager->m_groups, GroupListBuilder, _buffer) == 0)
    {
        return GROUP_MANAGER_GROUP_LIST_ERROR;
    }
    return GROUP_MANAGER_SUCCESS;
}

/* Static Functions */

static GroupManagerResult InputCheckCreateGroup(GroupManager *_manager, char *_groupName)
{
    if (_manager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED_ERROR;
    }
    if (_manager->m_numOfTakenAddr == MAX_NUM_OF_GROUPS)
    {
        return GROUP_MANAGER_MC_ADDRESSES_OVERFLOW;
    }
    return GROUP_MANAGER_SUCCESS;
}

static void DestroyGroupWrapper(void *_value)
{
    Group *group = (Group *)_value;
    Group_Destroy(&group);
}

static size_t HashFuncGroupName(void *_groupName)
{
    char *groupName = (char *)_groupName;
    size_t hash = 0;
    for (; *groupName; groupName++)
    {
        hash = (hash * 31) + *groupName;
    }
    return hash;
}

static int GroupNameComperator(void *_first, void *_second)
{
    char *firstName = (char *)_first;
    char *secondName = (char *)_second;
    return (strcmp(firstName, secondName) == 0);
}

static int GroupListBuilder(const void *_key, void *_value, void *_context)
{
    char *buffer = (char *)_context;
    Group *group = (Group *)_value;
    const char *groupName = (const char *)_key;
    size_t numMembers = Group_GetMemberCount(group);
    size_t currentLen = strlen(buffer);
    snprintf(buffer + currentLen, BUFFER_SIZE - currentLen,
             "%s (%zu members)\n", groupName, numMembers);

    return 1;
}

static GroupManagerResult InitialMulticastAddresses(GroupManager *_manager)
{
    char address[ADDR_SIZE], *addrCopy;
    for (size_t index = 1; index <= MAX_NUM_OF_GROUPS; ++index)
    {
        snprintf(address, sizeof(address), "%s%u", BASE_ADDR_MC, (unsigned int)index);
        addrCopy = strdup(address);
        if (addrCopy == NULL || QueueInsert(_manager->m_mcAdresses, addrCopy) != QUEUE_SUCCESS)
        {
            return GROUP_MANAGER_INIT_MC_ADDR_FAILED;
        }
    }
    return GROUP_MANAGER_SUCCESS;
}