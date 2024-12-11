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

UserManager *UserManager_Create(size_t _capacity);
void UserManager_Destroy(UserManager **_userManager);
UserManagerResult UserManager_RegisterUser(UserManager *manager, char *_userName, char *_password);
UserManagerResult UserManager_LoginUser(UserManager *manager, char *_userName, char *_password);

#endif