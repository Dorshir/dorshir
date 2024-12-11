#include "protocol.h"
#include "limits.h"
#include <stdio.h>  /* memcpy*/
#include <stdint.h> /* SIZE_MAX */
#include <string.h> /* strncpy, strlen */
#include <stdlib.h> /* size_t */

#define USER_NAME_PASS_REQ_SIZE_TL 4
#define USER_NAME_PASS_RES_SIZE_TL 3
#define USER_NAME_STATUS_SIZE 1

#define TAG_POS 0
#define TOTAL_LENGTH_POS 1
#define FIRST_FIELD_LEN_POS 2
#define FIRST_FIELD_POS 3

static size_t PackUserNamePassReq(ProtocolTag _tag, UserNamePass *_userNamePass, char *_packed);
static ProtocolResult UnPackUserNamePassReq(char *_packed, size_t _length, UserNamePass *_userNamePass);
static size_t PackUserNamePassRes(ProtocolTag _tag, char _status, char *_buffer);
static ProtocolResult UnPackUserNamePassRes(char *_buffer, char *_status);

/* Registration Request */

size_t PackRegReq(UserNamePass *_userNamePass, char *_packed)
{
    if (_userNamePass == NULL || _packed == NULL)
    {
        return 0;
    }

    unsigned char tag = PROTOCOL_REGISTRATION_REQ;

    return PackUserNamePassReq(tag, _userNamePass, _packed);
}

ProtocolResult UnPackRegReq(char *_packed, size_t _length, UserNamePass *_userNamePass)
{
    if (_userNamePass == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }

    return UnPackUserNamePassReq(_packed, _length, _userNamePass);
}

/* Registration Response */

size_t PackRegRes(char _status, char *_buffer)
{
    if (_buffer == NULL)
    {
        return 0;
    }
    return PackUserNamePassRes(PROTOCOL_REGISTRATION_RES, _status, _buffer);
}

ProtocolResult UnPackRegRes(char *_buffer, size_t _length, char *_status)
{
    if (_status == NULL || _length < USER_NAME_PASS_RES_SIZE_TL + USER_NAME_STATUS_SIZE || _buffer == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_buffer[TAG_POS] != PROTOCOL_REGISTRATION_RES)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }

    return UnPackUserNamePassRes(_buffer, _status);
}

/* Login Request */

size_t PackLoginReq(UserNamePass *_userNamePass, char *_packed)
{
    if (_userNamePass == NULL || _packed == NULL)
    {
        return 0;
    }

    unsigned char tag = PROTOCOL_LOGIN_REQ;

    return PackUserNamePassReq(tag, _userNamePass, _packed);
}

ProtocolResult UnPackLoginReq(char *_packed, size_t _length, UserNamePass *_userNamePass)
{
    if (_userNamePass == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }

    return UnPackUserNamePassReq(_packed, _length, _userNamePass);
}

/* Login Response */

size_t PackLoginRes(char _status, char *_buffer)
{
    if (_buffer == NULL)
    {
        return 0;
    }
    return PackUserNamePassRes(PROTOCOL_LOGIN_RES, _status, _buffer);
}

ProtocolResult UnPackLoginRes(char *_buffer, size_t _length, char *_status)
{
    if (_status == NULL || _length < USER_NAME_PASS_RES_SIZE_TL + USER_NAME_STATUS_SIZE || _buffer == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_buffer[TAG_POS] != PROTOCOL_LOGIN_RES)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }

    return UnPackUserNamePassRes(_buffer, _status);
}

/*****************************************************************************************************************/

ProtocolTag GetTag(char *_buffer)
{
    if (_buffer == NULL)
    {
        return PROTOCOL_INVALID_TAG;
    }

    ProtocolTag tag = _buffer[TAG_POS];

    if (tag < PROTOCOL_REGISTRATION_REQ || tag > PROTOCOL_LOGIN_RES)
    {
        return PROTOCOL_INVALID_TAG;
    }

    return tag;
}

/* Static Functions */

static size_t PackUserNamePassReq(ProtocolTag _tag, UserNamePass *_userNamePass, char *_packed)
{
    size_t userNameLen = strlen(_userNamePass->m_userName);
    size_t passwordLen = strlen(_userNamePass->m_password);
    size_t length = USER_NAME_PASS_REQ_SIZE_TL + userNameLen + passwordLen;

    if (length >= MAX_MSG_SIZE)
    {
        return 0;
    }

    _packed[TAG_POS] = (char)_tag;
    _packed[TOTAL_LENGTH_POS] = (char)length;
    _packed[FIRST_FIELD_LEN_POS] = (char)userNameLen;
    memcpy(&_packed[FIRST_FIELD_POS], _userNamePass->m_userName, userNameLen);
    _packed[FIRST_FIELD_POS + userNameLen] = (char)passwordLen;
    memcpy(&_packed[userNameLen + USER_NAME_PASS_REQ_SIZE_TL], _userNamePass->m_password, passwordLen);

    return length;
}

static ProtocolResult UnPackUserNamePassReq(char *_packed, size_t _length, UserNamePass *_userNamePass)
{
    if (_length < USER_NAME_PASS_REQ_SIZE_TL)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char userNameLen = (unsigned char)_packed[FIRST_FIELD_LEN_POS];

    if (USER_NAME_PASS_REQ_SIZE_TL + userNameLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char passwordLen = (unsigned char)_packed[FIRST_FIELD_POS + userNameLen];

    if (USER_NAME_PASS_REQ_SIZE_TL + userNameLen + passwordLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    memcpy(_userNamePass->m_userName, &_packed[FIRST_FIELD_POS], userNameLen);

    memcpy(_userNamePass->m_password, &_packed[FIRST_FIELD_POS + userNameLen + 1], passwordLen);

    _userNamePass->m_userName[userNameLen] = '\0';
    _userNamePass->m_password[passwordLen] = '\0';

    return PROTOCOL_SUCCESS;
}

static size_t PackUserNamePassRes(ProtocolTag _tag, char _status, char *_buffer)
{
    _buffer[TAG_POS] = PROTOCOL_REGISTRATION_RES;
    _buffer[TOTAL_LENGTH_POS] = USER_NAME_PASS_RES_SIZE_TL + USER_NAME_STATUS_SIZE;
    _buffer[FIRST_FIELD_LEN_POS] = USER_NAME_STATUS_SIZE;
    _buffer[FIRST_FIELD_POS] = _status;

    return USER_NAME_PASS_RES_SIZE_TL + USER_NAME_STATUS_SIZE;
}

static ProtocolResult UnPackUserNamePassRes(char *_buffer, char *_status)
{
    *_status = _buffer[FIRST_FIELD_POS];
    return PROTOCOL_SUCCESS;
}