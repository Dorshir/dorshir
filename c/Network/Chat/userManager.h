#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__

#include <stddef.h> /* size_t */
#include "protocol.h"

typedef struct UserManager UserManager;

typedef enum UserManager_Result
{
    USER_MANAGER_SUCCESS,
    USER_MANAGER_UNINITIALIZED_ERROR,
    USER_MANAGER_ALLOCATION_ERROR,
    USER_MANAGER_USER_ALREADY_EXISTS,
    USER_MANAGER_USER_NOT_FOUND,
    USER_MANAGER_WRONG_PASSWORD
} UserManagerResult;

/**
 * @brief Create a new UserManager instance.
 *
 * Allocates and initializes a new user manager with a specified initial capacity.
 *
 * @param[in] _capacity Initial capacity for the internal user storage.
 * @return Pointer to the newly created UserManager on success, or NULL on failure.
 */
UserManager *UserManager_Create(size_t _capacity);

/**
 * @brief Destroy a UserManager instance.
 *
 * Frees all resources associated with the UserManager.
 *
 * @param[in,out] _userManager A pointer to a pointer to the UserManager.
 * Upon successful destruction, *_userManager will be set to NULL.
 */
void UserManager_Destroy(UserManager **_userManager);

/**
 * @brief Register a new user.
 *
 * Attempts to add a new user with the provided user name and password.
 * If the username already exists, the function will return an error code.
 *
 * @param[in] _manager A pointer to the UserManager.
 * @param[in] _userName The username string. Must not be NULL.
 * @param[in] _password The password string. Must not be NULL.
 * @return UserManagerResult indicating success or type of failure.
 * @retval USER_MANAGER_SUCCESS Successfully added a new user.
 * @retval USER_MANAGER_UNINITIALIZED_ERROR If _manager, _userName, or _password is NULL.
 * @retval USER_MANAGER_ALLOCATION_ERROR If memory allocation failed.
 * @retval USER_MANAGER_USER_ALREADY_EXISTS If the user is already registered.
 */
UserManagerResult UserManager_RegisterUser(UserManager *_manager, char *_userName, char *_password);

/**
 * @brief Login a user.
 *
 * Checks if the given username exists and if the provided password matches the one on record.
 * If successful, the user is marked as logged in.
 *
 * @param[in] _manager A pointer to the UserManager.
 * @param[in] _userName The username string. Must not be NULL.
 * @param[in] _password The password string. Must not be NULL.
 * @return UserManagerResult indicating success or type of failure.
 * @retval USER_MANAGER_SUCCESS Login successful.
 * @retval USER_MANAGER_UNINITIALIZED_ERROR If _manager, _userName, or _password is NULL.
 * @retval USER_MANAGER_ALLOCATION_ERROR If memory allocation failed during processing.
 * @retval USER_MANAGER_USER_NOT_FOUND If the user does not exist.
 * @retval USER_MANAGER_WRONG_PASSWORD If the provided password does not match.
 */
UserManagerResult UserManager_LoginUser(UserManager *_manager, char *_userName, char *_password);

/**
 * @brief Check if a user is currently logged in.
 *
 * @param[in] _manager A pointer to the UserManager.
 * @param[in] _userName The username string. Must not be NULL.
 * @return Non-zero (true) if the user is logged in, zero (false) otherwise.
 */
int UserManager_IsUserLoggedIn(UserManager *_manager, const char *_userName);

#endif /* __USER_MANAGER_H__ */