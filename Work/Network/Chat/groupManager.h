#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

typedef struct GroupManager GroupManager;

typedef enum GroupManager_Result
{
    GROUP_MANAGER_SUCCESS,
    GROUP_MANAGER_UNINITIALIZED_ERROR,
    GROUP_MANAGER_MC_ADDRESSES_OVERFLOW,
    GROUP_MANAGER_GROUP_ALREADY_EXISTS,
    GROUP_MANAGER_ALLOCATION_FAILED,
    GROUP_MANAGER_ADDRESS_RECEIVE_FAILED,
    GROUP_MANAGER_GROUP_NOT_FOUND,
    GROUP_MANAGER_INC_MEMBERS_ERROR,
    GROUP_MANAGER_GROUP_CREATE_FAILED,
    GROUP_MANAGER_INVALID_GROUP_NAME,
    GROUP_MANAGER_ADDRESS_ADD_FAILED
} GroupManagerResult;

GroupManager* GroupManager_Create();

GroupManagerResult GroupManager_CreateGroup(GroupManager *_manager, char *_groupName);

GroupManagerResult GroupManager_JoinGroup(GroupManager *_manager, char *_groupName);

GroupManagerResult GroupManager_LeaveGroup(GroupManager *_manager, char *_groupName);

void GroupManager_Destroy(GroupManager ** _groupManager);



#endif /* __GROUP_MANAGER_H__ */
