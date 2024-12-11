#include "userManager.h"
#include "HashMap.h"
#include "user.h"
#include "limits.h"
#include <stdlib.h>
#include <string.h>

#define TRUE 1

struct UserManager
{
    HashMap *m_users;
};

// helper functions declarations:
static size_t HashUsername(void *_username);
static int CompareUsernames(void *first, void *second);
static void DestroyUserKey(void *_key);
static void DestroyUserValue(void *_value);

// Main functions
UserManager *UserManager_Create(size_t _capacity)
{
    UserManager *manager = (UserManager *)malloc(sizeof(UserManager));
    if (!manager)
    {
        return NULL;
    }
    manager->m_users = HashMap_Create(_capacity, HashUsername, CompareUsernames);
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
    HashMap_Destroy(&(*_userManager)->m_users, DestroyUserKey, DestroyUserValue);
    free(*_userManager);
    *_userManager = NULL;
}

UserManagerResult UserManager_RegisterUser(UserManager *manager, char *_userName, char *_password)
{
    User *existingUser;
    char *username;

    if (!manager || !_userName || !_password)
    {
        return USER_MANAGER_UNINITIALIZED_ERROR;
    }

    User *newUser = UserCreate(_userName, _password);
    if (!newUser)
    {
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Find(manager->m_users, _userName, (void **)&existingUser) == MAP_SUCCESS)
    {
        return USER_MANAGER_USER_ALREADY_EXISTS;
    }

    username = strdup(_userName);
    if (!username)
    {
        UserDelete(&newUser);
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Insert(manager->m_users, username, newUser) != MAP_SUCCESS)
    {
        free(username);
        UserDelete(&newUser);
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    return USER_MANAGER_SUCCESS;
}

UserManagerResult UserManager_LoginUser(UserManager *manager, char *_userName, char *_password)
{
    User *foundUser;

    if (!manager || !_userName || !_password)
    {
        return USER_MANAGER_UNINITIALIZED_ERROR;
    }

    User *cand = UserCreate(_userName, _password);
    if (cand == NULL)
    {
        return USER_MANAGER_ALLOCATION_ERROR;
    }

    if (HashMap_Find(manager->m_users, _userName, (void **)&foundUser) != MAP_SUCCESS)
    {
        UserDelete(&cand);
        return USER_MANAGER_USER_NOT_FOUND;
    }

    if (PasswordCheck(cand, foundUser) != TRUE)
    {
        UserDelete(&cand);
        return USER_MANAGER_WRONG_PASSWORD;
    }

    UserDelete(&cand);
    return USER_MANAGER_SUCCESS;
}

// helper functions

static size_t HashUsername(void *_username)
{
    char *username = (char *)_username;
    size_t hash = 0;
    for (; *username; username++)
    {
        hash = (hash * 31) + *username;
    }
    return hash;
}

static int CompareUsernames(void *first, void *second)
{
    return strcmp((char *)first, (char *)second) == 0;
}

static void DestroyUserKey(void *_key)
{
    free(_key);
}

static void DestroyUserValue(void *_value)
{
    User *user = (User *)_value;
    UserDelete(&user);
}
