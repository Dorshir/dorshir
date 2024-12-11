#include "protocol.h"
#include "limits.h"
#include <stdio.h>  /* memcpy*/
#include <stdint.h> /* SIZE_MAX */
#include <string.h> /* strncpy, strlen */
#include <stdlib.h> /* size_t */

#define REG_REQ_SIZE_TL 4
#define REQ_RES_SIZE_TL 3

#define TAG_POS 0
#define TOTAL_LENGTH_POS 1
#define FIRST_FIELD_LEN_POS 2
#define FIRST_FIELD_POS 3

#define REG_STATUS_SIZE 1

/* Registration Request */

size_t PackRegReq(UserNamePass *_userNamePass, char *_packed)
{
    if (_userNamePass == NULL || _packed == NULL)
    {
        return 0;
    }

    unsigned char tag = PROTOCOL_REGISTRATION_REQ;
    size_t userNameLen = strlen(_userNamePass->m_userName);
    size_t passwordLen = strlen(_userNamePass->m_password);
    size_t length = REG_REQ_SIZE_TL + userNameLen + passwordLen;

    if (length >= MAX_MSG_SIZE)
    {
        return 0;
    }

    _packed[TAG_POS] = (char)tag;
    _packed[TOTAL_LENGTH_POS] = (char)length;
    _packed[FIRST_FIELD_LEN_POS] = (char)userNameLen;
    memcpy(&_packed[FIRST_FIELD_POS], _userNamePass->m_userName, userNameLen);
    _packed[FIRST_FIELD_POS + userNameLen] = (char)passwordLen;
    memcpy(&_packed[userNameLen + REG_REQ_SIZE_TL], _userNamePass->m_password, passwordLen);

    return length;
}

ProtocolResult UnPackRegReq(char *_packed, size_t _length, UserNamePass *_userNamePass)
{
    if (_userNamePass == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_length < REG_REQ_SIZE_TL)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char userNameLen = (unsigned char)_packed[FIRST_FIELD_LEN_POS];

    if (REG_REQ_SIZE_TL + userNameLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char passwordLen = (unsigned char)_packed[FIRST_FIELD_POS + userNameLen];

    if (REG_REQ_SIZE_TL + userNameLen + passwordLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    memcpy(_userNamePass->m_userName, &_packed[FIRST_FIELD_POS], userNameLen);

    memcpy(_userNamePass->m_password, &_packed[FIRST_FIELD_POS + userNameLen + 1], passwordLen);

    return PROTOCOL_SUCCESS;
}

/* Registration Response */

size_t PackRegRes(char status, char *buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }  
    buffer[TAG_POS] = PROTOCOL_REGISTRATION_RES; 
    buffer[TOTAL_LENGTH_POS] = REQ_RES_SIZE_TL + 1;
    buffer[FIRST_FIELD_LEN_POS] = 1;
    buffer[FIRST_FIELD_POS] = status;
    
    return REQ_RES_SIZE_TL + 1;
}

ProtocolResult UnPackRegRes(char* buffer, size_t length, char* status)
{
    if (status == NULL || length < REQ_RES_SIZE_TL + 1 || buffer == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }  
    if (buffer[TAG_POS] != PROTOCOL_REGISTRATION_RES)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }

   
    *status = buffer[FIRST_FIELD_POS];
    return PROTOCOL_SUCCESS;
}

/*****************************************************************************************************************/

ProtocolTag GetTag(char *_buffer)
{
    if (_buffer == NULL)
    {
        return PROTOCOL_INVALID_TAG;
    }

    ProtocolTag tag = _buffer[TAG_POS];

    if (tag < PROTOCOL_REGISTRATION_REQ || tag > PROTOCOL_LOGIN)
    {
        return PROTOCOL_INVALID_TAG;
    }

    return tag;
}
