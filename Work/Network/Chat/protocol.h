#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stddef.h> /* size_t */

typedef struct Protocol Protocol;

typedef enum Protocol_Result
{
    PROTOCOL_SUCCESS = 0,
    PROTOCOL_UNINITIALIZED_ERROR,
    PROTOCOL_ALLOCATION_ERROR
} ProtocolResult;

typedef enum Protocol_Tag
{
    PROTOCOL_REGISTRATION = 1,
    PROTOCOL_LOGIN
} ProtocolTag;

size_t Pack(char *_buffer, size_t _length);

size_t UnPack(char *_packed, size_t _length, char *_buffer);

#endif /* __PROTOCOL_H__ */
