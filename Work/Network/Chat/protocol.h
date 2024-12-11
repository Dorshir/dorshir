#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stddef.h> /* size_t */
#include <stddef.h> /* size_t */

typedef struct Protocol Protocol;

typedef enum Protocol_Result
{
    PROTOCOL_SUCCESS,
    PROTOCOL_INVALID_BUFFER_LENGTH,
    PROTOCOL_UNINITIALIZED_ERROR,
    PROTOCOL_UNPACK_FAILURE

} ProtocolResult;

typedef enum Protocol_Tag
{
    PROTOCOL_INVALID_TAG = -1,
    PROTOCOL_REGISTRATION_REQ,
    PROTOCOL_REGISTRATION_RES,
    PROTOCOL_LOGIN_REQ,
    PROTOCOL_LOGIN_RES,

} ProtocolTag;

typedef struct UserNamePass
{
    char *m_userName;
    char *m_password;
} UserNamePass;

ProtocolTag GetTag(char *_buffer);

/* Registration */

size_t PackRegReq(UserNamePass *_userNamePass, char *_buffer);

ProtocolResult UnPackRegReq(char *_buffer, size_t _length, UserNamePass *_userNamePass);

size_t PackRegRes(char _status, char *_buffer);

ProtocolResult UnPackRegRes(char *_buffer, size_t _length, char *_status);

/* Login */

size_t PackLoginReq(UserNamePass *_userNamePass, char *_packed);

ProtocolResult UnPackLoginReq(char *_packed, size_t _length, UserNamePass *_userNamePass);

size_t PackLoginRes(char _status, char *_buffer);

ProtocolResult UnPackLoginRes(char *_buffer, size_t _length, char *_status);


#endif /* __PROTOCOL_H__ */
