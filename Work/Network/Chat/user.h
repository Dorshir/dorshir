#ifndef __USER_H__
#define __USER_H__


typedef struct User User;

typedef enum User_Result
{
    USER_SUCCESS,
    USER_UNINITIALIZED_ERROR,
} UserResult;


User* UserCreate(char* _username, char* _password);

void UserDelete(User ** _user);




#endif /* __USER_H__ */
