#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

#include <stddef.h> /* size_t */

typedef struct GroupManager GroupManager;

typedef enum GroupManager_Result
{
    GROUP_MANAGER_SUCCESS,
    GROUP_MANAGER_UNINITIALIZED_ERROR,
} GroupManagerResult;

GroupManager* GroupManagerCreate();

void GroupManagerDelete(GroupManager ** _groupManager);



#endif /* __GROUP_MANAGER_H__ */
