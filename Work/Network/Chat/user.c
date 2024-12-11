#include "user.h"
#include <string.h> /* strlen, strncpy */
#include <stdlib.h> /* malloc, free */

struct User
{
    char *m_userName;
    char *m_password;
};

User *UserCreate(char *_username, char *_password)
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

    return newUser;
}

void UserDelete(User **_user)
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

UserResult PasswordCheck(User *_first, User *_second)
{
    return (strcmp(_first->m_password, _second->m_password) == 0);
}

int UserNameComperator(void *_first, void *_second)
{
    User *firstUser = (User *)_first;
    User *secondUser = (User *)_second;

    return (strcmp(firstUser->m_userName, secondUser->m_userName) == 0);
}
