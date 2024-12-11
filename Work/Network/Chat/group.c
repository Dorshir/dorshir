#include "group.h"
#include <stdlib.h> /* malloc, free, NULL */
#include <string.h> /* strlen, strncpy */

#define INITIAL_GROUP_SIZE 1

struct Group
{
    char *m_name;
    char *m_address;
    size_t m_numOfMembers;
};

char *GroupGetAddress(Group *_group)
{
    if (_group == NULL)
    {
        return NULL;
    }
    return _group->m_address;
}

Group *GroupCreate(char *_groupName, char *_address)
{
    if (_groupName == NULL || _address == NULL)
    {
        return NULL;
    }

    size_t groupNameLen = strlen(_groupName);
    size_t addressLen = str(_address);
    if (groupNameLen == 0 || addressLen == 0)
    {
        return NULL;
    }

    Group *newGroup = malloc(sizeof(Group));
    if (newGroup == NULL)
    {
        return NULL;
    }

    newGroup->m_name = malloc(sizeof(char) * (groupNameLen + 1));
    if (newGroup->m_name == NULL)
    {
        free(newGroup);
        return NULL;
    }

    newGroup->m_address = malloc(sizeof(char) * (addressLen + 1));
    if (newGroup->m_name == NULL)
    {
        free(newGroup->m_name);
        free(newGroup);
        return NULL;
    }

    strncpy(newGroup->m_name, _groupName, groupNameLen);
    strncpy(newGroup->m_address, _address, addressLen);

    newGroup->m_name[groupNameLen] = '\0';
    newGroup->m_address[addressLen] = '\0';

    newGroup->m_numOfMembers = INITIAL_GROUP_SIZE;

    return newGroup;
}

GroupResult GroupIncrementMembers(Group *_group)
{
    if (_group == NULL)
    {
        return GROUP_UNINITIALIZED_ERROR;
    }
    ++_group->m_numOfMembers;
    return GROUP_SUCCESS;
}

GroupResult GroupDecrementMembers(Group *_group)
{
    if (_group == NULL)
    {
        return GROUP_UNINITIALIZED_ERROR;
    }
    --_group->m_numOfMembers;

    if (_group->m_numOfMembers == 0)
    {
        return GROUP_EMPTY;
    }

    return GROUP_SUCCESS;
}

void GroupDestroy(Group **_group)
{
    if (_group == NULL || *_group == NULL)
    {
        return;
    }
    free((*_group)->m_name);
    free(*_group);
    *_group = NULL;
}
