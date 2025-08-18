#include "user.h"
#include <string.h> /* strlen, strncpy */
#include <stdlib.h> /* malloc, free */

struct User
{
    char *m_userName;
    char *m_password;
    LoginStatus m_isLoggedIn;
};

User *User_Create(char *_username, char *_password)
{
    if (_username == NULL || _password == NULL)
    {
        return NULL;
    }

    User *newUser = malloc(sizeof(User));
    if (newUser == NULL)
    {
        return NULL;
    }

    size_t userNameLen = strlen(_username);
    size_t passwordLen = strlen(_password);
    if (userNameLen == 0 || passwordLen == 0)
    {
        return NULL;
    }

    newUser->m_userName = malloc(sizeof(char) * (userNameLen + 1));
    if (newUser->m_userName == NULL)
    {
        free(newUser);
        return NULL;
    }

    newUser->m_password = malloc(sizeof(char) * (passwordLen + 1));
    if (newUser->m_password == NULL)
    {
        free(newUser->m_userName);
        free(newUser);
        return NULL;
    }

    strncpy(newUser->m_userName, _username, userNameLen);
    strncpy(newUser->m_password, _password, passwordLen);
    newUser->m_userName[userNameLen] = '\0';
    newUser->m_password[passwordLen] = '\0';
    newUser->m_isLoggedIn = LOGGED_OUT; 

    return newUser;
}

void User_Destroy(User **_user)
{
    if (_user == NULL || *_user == NULL)
    {
        return;
    }

    free((*_user)->m_userName);
    free((*_user)->m_password);
    free(*_user);

    *_user = NULL;
}

int User_PasswordCheck(User *_first, User *_second)
{
    return (strcmp(_first->m_password, _second->m_password) == 0);
}

void User_SetLoginStatus(User* _user, LoginStatus _status)
{
    if (_user != NULL)
    {
        _user->m_isLoggedIn = _status;
    }
}

LoginStatus User_GetLoginStatus(User* _user)
{
    if (_user == NULL)
    {
        return 0;
    }
    return _user->m_isLoggedIn;
}

