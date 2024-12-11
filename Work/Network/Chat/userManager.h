#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__

#include <stddef.h> /* size_t */

typedef struct UserManager UserManager;

typedef enum UserManager_Result
{
    USER_MANAGER_SUCCESS,
    USER_MANAGER_UNINITIALIZED_ERROR,
} UserManagerResult;



#endif /* __USER_MANAGER_H__ */
