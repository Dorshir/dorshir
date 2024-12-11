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
    PROTOCOL_REGISTRATION_REQ = 0,  
    PROTOCOL_REGISTRATION_RES = 1,
    PROTOCOL_LOGIN = 2,
} ProtocolTag;

typedef struct UserNamePass
{
    char *m_userName;
    char *m_password;
} UserNamePass;

size_t PackRegReq(UserNamePass *_userNamePass, char *_buffer);

ProtocolResult UnPackRegReq(char *_buffer, size_t _length, UserNamePass *_userNamePass);

size_t PackRegRes(char _status, char *_buffer);

ProtocolResult UnPackRegRes(char *_buffer, size_t _length, char *_status);

ProtocolTag GetTag(char *_buffer);

#endif /* __PROTOCOL_H__ */
