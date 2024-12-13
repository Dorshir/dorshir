#ifndef __USER_H__
#define __USER_H__

typedef struct User User;

typedef enum Login_Status
{
    LOGGED_OUT, 
    LOGGED_IN   
} LoginStatus;

/**
 * @brief Create a new user.
 *
 * Allocates and initializes a new user with the specified username and password.
 *
 * @param[in] _username The username string. Must not be NULL or empty.
 * @param[in] _password The password string. Must not be NULL or empty.
 * @return Pointer to the newly created User on success, or NULL on failure.
 */
User *User_Create(char *_username, char *_password);

/**
 * @brief Destroy a user.
 *
 * Frees all resources associated with a user. Sets the user pointer to NULL.
 *
 * @param[in,out] _user A pointer to a pointer to the User to be destroyed.
 */
void User_Destroy(User **_user);

/**
 * @brief Check if two users' passwords match.
 *
 * Compares the passwords of two users to verify if they match.
 *
 * @param[in] _first Pointer to the first User. Must not be NULL.
 * @param[in] _second Pointer to the second User. Must not be NULL.
 * @return Non-zero (true) if the passwords match, zero (false) otherwise.
 */
int User_PasswordCheck(User *_first, User *_second);

/**
 * @brief Set the login status of a user.
 *
 * Updates the login status of the specified user.
 *
 * @param[in] _user Pointer to the User. Must not be NULL.
 * @param[in] _status The new login status to set.
 */
void User_SetLoginStatus(User *_user, LoginStatus _status);

/**
 * @brief Get the current login status of a user.
 *
 * Retrieves the current login status of the specified user.
 *
 * @param[in] _user Pointer to the User. Must not be NULL.
 * @return The login status of the user.
 */
LoginStatus User_GetLoginStatus(User *_user);

#endif /* __USER_H__ */
