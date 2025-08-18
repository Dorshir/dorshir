#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

#include <stddef.h> /* size_t */

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
    GROUP_MANAGER_ADDRESS_ADD_FAILED,
    GROUP_MANAGER_GROUP_LIST_ERROR,
    GROUP_MANAGER_INIT_MC_ADDR_FAILED
} GroupManagerResult;

/**
 * @brief Create a new GroupManager instance.
 *
 * Allocates and initializes a new GroupManager to manage groups and multicast addresses.
 *
 * @return Pointer to the newly created GroupManager on success, or NULL on failure.
 */
GroupManager *GroupManager_Create();

/**
 * @brief Create a new group.
 *
 * Creates a group with the specified name and assigns it a multicast address.
 *
 * @param[in] _manager Pointer to the GroupManager. Must not be NULL.
 * @param[in] _groupName Name of the group to create. Must not be NULL or empty.
 * @return GroupManagerResult indicating success or type of failure.
 */
GroupManagerResult GroupManager_CreateGroup(GroupManager *_manager, char *_groupName);

/**
 * @brief Join an existing group.
 *
 * Increments the member count of the specified group.
 *
 * @param[in] _manager Pointer to the GroupManager. Must not be NULL.
 * @param[in] _groupName Name of the group to join. Must not be NULL or empty.
 * @return GroupManagerResult indicating success or type of failure.
 */
GroupManagerResult GroupManager_JoinGroup(GroupManager *_manager, char *_groupName);

/**
 * @brief Leave a group.
 *
 * Decrements the member count of the specified group.
 * If the group becomes empty, it is destroyed, and its multicast address is recycled.
 *
 * @param[in] _manager Pointer to the GroupManager. Must not be NULL.
 * @param[in] _groupName Name of the group to leave. Must not be NULL or empty.
 * @return GroupManagerResult indicating success or type of failure.
 */
GroupManagerResult GroupManager_LeaveGroup(GroupManager *_manager, char *_groupName);

/**
 * @brief Destroy the GroupManager instance.
 *
 * Frees all resources associated with the GroupManager, including groups and multicast addresses.
 *
 * @param[in,out] _groupManager A pointer to a pointer to the GroupManager.
 * Upon successful destruction, *_groupManager will be set to NULL.
 */
void GroupManager_Destroy(GroupManager **_groupManager);

/**
 * @brief Get the multicast address of a group.
 *
 * Retrieves the multicast address assigned to the specified group.
 *
 * @param[in] _manager Pointer to the GroupManager. Must not be NULL.
 * @param[in] _groupName Name of the group. Must not be NULL or empty.
 * @return Pointer to the multicast address string, or NULL on failure.
 */
char *GroupManager_GetGroupAddress(GroupManager *_manager, const char *_groupName);

/**
 * @brief Get a list of active groups.
 *
 * Retrieves a formatted list of active groups and their member counts.
 *
 * @param[in] _manager Pointer to the GroupManager. Must not be NULL.
 * @param[out] _buffer Buffer to store the list. Must not be NULL.
 * @param[in] _maxSize Maximum size of the buffer.
 * @return GroupManagerResult indicating success or type of failure.
 */
GroupManagerResult GroupManager_GetActiveGroups(GroupManager *_manager, char *_buffer, size_t _maxSize);

#endif /* __GROUP_MANAGER_H__ */
