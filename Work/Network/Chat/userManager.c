#include "userManager.h"
#include "HashMap.h"
#include "user.h"
#include "limits.h"
#include <stdlib.h> /* malloc ,free, NULL */
#include <string.h> /* strcmp */

#define TRUE 1

struct UserManager
{
    HashMap *m_users;
};

/* Helpers Declerations */

static size_t HashUserName(void *_username);
static int CompareUserNames(void *_first, void *_second);
static void DestroyUserValue(void *_value);

/* API Functions */

UserManager *UserManager_Create(size_t _capacity)
{
    UserManager *manager = (UserManager *)malloc(sizeof(UserManager));
    if (!manager)
    {
        return NULL;
    }
    manager->m_users = HashMap_Create(_capacity, HashUserName, CompareUserNames);
    if (!manager->m_users)
    {
        free(manager);
        return NULL;
    }
    return manager;
}

void UserManager_Destroy(UserManager **_userManager)
{
    if (!_userManager || !*_userManager)
    {
        return;
    }
    HashMap_Destroy(&(*_userManager)->m_users, free, DestroyUserValue);
    free(*_userManager);
    *_userManager = NULL;
}

UserManagerResult UserManager_RegisterUser(UserManager *_manager, char *_userName, char *_password)
{
    User *existingUser;
    char *userNameKey;

    if (!_manager || !_userName || !_password)
    {
        return USER_MANAGER_UNINITIALIZED_ERROR;
    }

    User *newUser = User_Create(_userName, _password);
    if (!newUser)
    {
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Find(_manager->m_users, _userName, (void **)&existingUser) == MAP_SUCCESS)
    {
        return USER_MANAGER_USER_ALREADY_EXISTS;
    }

    userNameKey = strdup(_userName);
    if (!userNameKey)
    {
        User_Destroy(&newUser);
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Insert(_manager->m_users, userNameKey, newUser) != MAP_SUCCESS)
    {
        free(userNameKey);
        User_Destroy(&newUser);
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManager_LoginUser(UserManager *_manager, char *_userName, char *_password)
{
    User *foundUser;

    if (!_manager || !_userName || !_password)
    {
        return USER_MANAGER_UNINITIALIZED_ERROR;
    }

    User *cand = User_Create(_userName, _password);
    if (cand == NULL)
    {
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Find(_manager->m_users, _userName, (void **)&foundUser) != MAP_SUCCESS)
    {
        User_Destroy(&cand);
        return USER_MANAGER_USER_NOT_FOUND;
    }

    if (User_PasswordCheck(cand, foundUser) != TRUE)
    {
        User_Destroy(&cand);
        return USER_MANAGER_WRONG_PASSWORD;
    }

    User_SetLoginStatus(foundUser, LOGGED_IN);

    User_Destroy(&cand);
    return USER_MANAGER_SUCCESS;
}

int UserManager_IsUserLoggedIn(UserManager *_manager, const char *_userName)
{
    User *user;
    if (_manager == NULL || _userName == NULL)
    {
        return 0;
    }

    if (HashMap_Find(_manager->m_users, (void *)_userName, (void **)&user) != MAP_SUCCESS)
    {
        return 0;
    }
    return (User_GetLoginStatus(user) == LOGGED_IN);
}

/* Helpers */

static size_t HashUserName(void *_username)
{
    char *username = (char *)_username;
    size_t hash = 0;
    for (; *username; username++)
    {
        hash = (hash * 31) + *username;
    }
    return hash;
}

static int CompareUserNames(void *_first, void *_second)
{
    return strcmp((char *)_first, (char *)_second) == 0;
}

static void DestroyUserValue(void *_value)
{
    User *user = (User *)_value;
    User_Destroy(&user);
}
