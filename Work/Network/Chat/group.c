#include "group.h"
#include <stdlib.h> /* malloc, free, NULL */
#include <string.h> /* strlen, strncpy */

#define INITIAL_GROUP_SIZE 1

struct Group
{
    char *m_name;
    size_t m_size;
};

GroupCreate(char *_groupName)
{
    if (_groupName == NULL)
    {
        return NULL;
    }

    size_t groupNameLen = strlen(_groupName);
    if (groupNameLen == 0)
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

    strncpy(newGroup->m_name, _groupName, groupNameLen);

    newGroup->m_name[groupNameLen] = '\0';

    newGroup->m_size = INITIAL_GROUP_SIZE;

    return newGroup;
}

void GroupDelete(Group **_group)
{
    if (_group == NULL || *_group == NULL)
    {
        return;
    }
    free((*_group)->m_name);
    free(*_group);
    *_group = NULL;
}
