#ifndef __GROUP_H__
#define __GROUP_H__

#include <stddef.h>

typedef struct Group Group;

typedef enum Group_Result
{
    GROUP_SUCCESS,
    GROUP_UNINITIALIZED_ERROR,
    GROUP_EMPTY
} GroupResult;

/**
 * @brief Create a new group.
 *
 * Allocates and initializes a new group with the specified name and address.
 *
 * @param[in] _groupName The name of the group. Must not be NULL or empty.
 * @param[in] _address The address associated with the group. Must not be NULL or empty.
 * @return Pointer to the newly created Group on success, or NULL on failure.
 */
Group *Group_Create(char *_groupName, char *_address);

/**
 * @brief Increment the number of members in the group.
 *
 * Adds one to the group's member count.
 *
 * @param[in] _group Pointer to the Group. Must not be NULL.
 * @return GroupResult indicating success or type of failure.
 * @retval GROUP_SUCCESS Increment succeeded.
 * @retval GROUP_UNINITIALIZED_ERROR If _group is NULL.
 */
GroupResult Group_IncrementMembers(Group *_group);

/**
 * @brief Decrement the number of members in the group.
 *
 * Subtracts one from the group's member count.
 * Returns `GROUP_EMPTY` if the count reaches zero.
 *
 * @param[in] _group Pointer to the Group. Must not be NULL.
 * @return GroupResult indicating success, error, or if the group is empty.
 * @retval GROUP_SUCCESS Decrement succeeded.
 * @retval GROUP_EMPTY Group member count reached zero.
 * @retval GROUP_UNINITIALIZED_ERROR If _group is NULL.
 */
GroupResult Group_DecrementMembers(Group *_group);

/**
 * @brief Destroy a group.
 *
 * Frees all resources associated with the group and sets the group pointer to NULL.
 *
 * @param[in,out] _group A pointer to a pointer to the Group to be destroyed.
 */
void Group_Destroy(Group **_group);

/**
 * @brief Get the address associated with the group.
 *
 * @param[in] _group Pointer to the Group. Must not be NULL.
 * @return The address string of the group, or NULL if _group is NULL.
 */
char *Group_GetAddress(Group *_group);

/**
 * @brief Get the name of the group.
 *
 * @param[in] _group Pointer to the Group. Must not be NULL.
 * @return The name of the group, or NULL if _group is NULL.
 */
const char *Group_GetName(Group *_group);

/**
 * @brief Get the current member count of the group.
 *
 * @param[in] _group Pointer to the Group. Must not be NULL.
 * @return The number of members in the group, or 0 if _group is NULL.
 */
size_t Group_GetMemberCount(Group *_group);

#endif /* __GROUP_H__ */
