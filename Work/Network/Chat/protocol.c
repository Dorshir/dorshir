#include "protocol.h"
#include "limits.h"
#include <stdio.h>  /* memcpy*/
#include <stdint.h> /* SIZE_MAX */
#include <string.h> /* strncpy, strlen */
#include <stdlib.h> /* size_t */

#define USER_NAME_PASS_REQ_SIZE_TL 4
#define USER_NAME_PASS_RES_SIZE_TL 3
#define USER_NAME_STATUS_SIZE 1

#define GROUP_REQ_SIZE_TL 4
#define GROUP_RES_SIZE_TL 3
#define GROUP_STATUS_SIZE 1

#define GET_GROUPS_REQ_SIZE_TL 3
#define GET_GROUPS_RES_SIZE_TL 4

#define TAG_POS 0
#define TOTAL_LENGTH_POS 1
#define FIRST_FIELD_LEN_POS 2
#define FIRST_FIELD_POS 3

static size_t PackUserNamePassReq(ProtocolTag _tag, UserNamePass *_userNamePass, char *_packed);
static ProtocolResult UnPackUserNamePassReq(char *_packed, size_t _length, UserNamePass *_userNamePass);
static size_t PackUserNamePassRes(ProtocolTag _tag, char _status, char *_buffer);
static ProtocolResult UnPackUserNamePassRes(char *_buffer, char *_status);
static size_t PackGroupReq(ProtocolTag _tag, GroupRequest *_groupReq, char *_packed);
static ProtocolResult UnPackGroupReq(char *_packed, size_t _length, GroupRequest *_groupReq);
static size_t PackGroupResWithAddress(ProtocolTag _tag, char _status, const char *_address, char *_buffer);
static size_t PackGetGroupsReqWrapper(ProtocolTag _tag, char *_userName, char *_packed);
static ProtocolResult UnPackGetGroupsReqWrapper(ProtocolTag _tag, char *_packed, size_t _length, char *_userName);
static size_t PackGetGroupsResWrapper(ProtocolTag _tag, char _status, char *_groupsList, char *_packed);
static ProtocolResult UnPackCreateGroupResWrapper(ProtocolTag _tag, char *_packed, size_t _length, char *_status, char *_groupsList);
static ProtocolResult UnPackGroupResWithAddress(char *_buffer, size_t _length, char *_status, char **_address, ProtocolTag _expectedTag);
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

/* Create Group Request */

size_t PackCreateGroupReq(GroupRequest *_groupReq, char *_packed)
{
    if (_groupReq == NULL || _packed == NULL)
    {
        return 0;
    }
    return PackGroupReq(PROTOCOL_CREATE_GROUP_REQ, _groupReq, _packed);
}

ProtocolResult UnPackCreateGroupReq(char *_packed, size_t _length, GroupRequest *_groupReq)
{
    if (_groupReq == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_packed[TAG_POS] != PROTOCOL_CREATE_GROUP_REQ)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }
    return UnPackGroupReq(_packed, _length, _groupReq);
}

/* Create Group Response */

size_t PackCreateGroupResWithAddress(char _status, const char *_address, char *_buffer)
{
    return PackGroupResWithAddress(PROTOCOL_CREATE_GROUP_RES, _status, _address, _buffer);
}

ProtocolResult UnPackCreateGroupResWithAddress(char *_buffer, size_t _length, CreateGroupResponse *_response)
{
    return UnPackGroupResWithAddress(_buffer, _length, &_response->m_status, &_response->m_address, PROTOCOL_CREATE_GROUP_RES);
}

/* Get Groups Request */

size_t PackGetGroupsReq(char *_userName, char *_packed)
{
    return PackGetGroupsReqWrapper(PROTOCOL_GET_GROUPS_REQ, _userName, _packed);
}

ProtocolResult UnPackGetGroupsReq(char *_packed, size_t _length, char *_userName)
{
    return UnPackGetGroupsReqWrapper(PROTOCOL_GET_GROUPS_REQ, _packed, _length, _userName);
}

/* Get Groups Response */

size_t PackGetGroupsRes(char _status, char *_groupsList, char *_packed)
{
    return PackGetGroupsResWrapper(PROTOCOL_GET_GROUPS_RES, _status, _groupsList, _packed);
}

ProtocolResult UnPackGetGroupsRes(char *_packed, size_t _length, char *_status, char *_groupsList)
{
    return UnPackCreateGroupResWrapper(PROTOCOL_GET_GROUPS_RES, _packed, _length, _status, _groupsList);
}

/* Join Group Request*/

size_t PackJoinGroupReq(GroupRequest *_groupReq, char *_packed)
{
    if (_groupReq == NULL || _packed == NULL)
    {
        return 0;
    }
    return PackGroupReq(PROTOCOL_JOIN_GROUP_REQ, _groupReq, _packed);
}

ProtocolResult UnPackJoinGroupReq(char *_packed, size_t _length, GroupRequest *_groupReq)
{
    if (_groupReq == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_packed[TAG_POS] != PROTOCOL_JOIN_GROUP_REQ)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }
    return UnPackGroupReq(_packed, _length, _groupReq);
}

/* Join Group Response */

size_t PackJoinGroupResWithAddress(char _status, const char *_address, char *_buffer)
{
    return PackGroupResWithAddress(PROTOCOL_JOIN_GROUP_RES, _status, _address, _buffer);
}

ProtocolResult UnPackJoinGroupResWithAddress(char *_buffer, size_t _length, JoinGroupResponse *_response)
{
    return UnPackGroupResWithAddress(_buffer, _length, &_response->m_status, &_response->m_address, PROTOCOL_JOIN_GROUP_RES);
}

/* Leave Group Request */

size_t PackLeaveGroupReq(GroupRequest *_groupReq, char *_packed)
{
    if (_groupReq == NULL || _packed == NULL)
    {
        return 0;
    }
    return PackGroupReq(PROTOCOL_LEAVE_GROUP_REQ, _groupReq, _packed);
}

ProtocolResult UnPackLeaveGroupReq(char *_packed, size_t _length, GroupRequest *_groupReq)
{
    if (_groupReq == NULL || _length == 0 || _packed == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_packed[TAG_POS] != PROTOCOL_LEAVE_GROUP_REQ)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }
    return UnPackGroupReq(_packed, _length, _groupReq);
}

/* Leave Group Response */

size_t PackLeaveGroupRes(char _status, char *_buffer)
{
    if (_buffer == NULL)
    {
        return 0;
    }
    return PackUserNamePassRes(PROTOCOL_LEAVE_GROUP_RES, _status, _buffer);
}

ProtocolResult UnPackLeaveGroupRes(char *_buffer, size_t _length, char *_status)
{
    if (_status == NULL || _length < GROUP_RES_SIZE_TL + GROUP_STATUS_SIZE || _buffer == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_buffer[TAG_POS] != PROTOCOL_LEAVE_GROUP_RES)
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
    switch(tag)
    {
        case PROTOCOL_REGISTRATION_REQ:
        case PROTOCOL_REGISTRATION_RES:
        case PROTOCOL_LOGIN_REQ:
        case PROTOCOL_LOGIN_RES:
        case PROTOCOL_CREATE_GROUP_REQ:
        case PROTOCOL_CREATE_GROUP_RES:
        case PROTOCOL_JOIN_GROUP_REQ:
        case PROTOCOL_JOIN_GROUP_RES:
        case PROTOCOL_GET_GROUPS_REQ:
        case PROTOCOL_GET_GROUPS_RES:
        case PROTOCOL_LEAVE_GROUP_REQ:
        case PROTOCOL_LEAVE_GROUP_RES:
            return tag;
        default:
            return PROTOCOL_INVALID_TAG;
    }
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
    _buffer[TAG_POS] = _tag;
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

static size_t PackGroupReq(ProtocolTag _tag, GroupRequest *_groupReq, char *_packed)
{
    size_t userNameLen = strlen(_groupReq->m_userName);
    size_t groupNameLen = strlen(_groupReq->m_groupName);
    size_t length = GROUP_REQ_SIZE_TL + userNameLen + groupNameLen;

    if (length >= MAX_MSG_SIZE)
    {
        return 0;
    }

    _packed[TAG_POS] = (char)_tag;
    _packed[TOTAL_LENGTH_POS] = (char)length;
    _packed[FIRST_FIELD_LEN_POS] = (char)userNameLen;
    memcpy(&_packed[FIRST_FIELD_POS], _groupReq->m_userName, userNameLen);
    _packed[FIRST_FIELD_POS + userNameLen] = (char)groupNameLen;
    memcpy(&_packed[userNameLen + GROUP_REQ_SIZE_TL], _groupReq->m_groupName, groupNameLen);

    return length;
}

static ProtocolResult UnPackGroupReq(char *_packed, size_t _length, GroupRequest *_groupReq)
{
    if (_length < GROUP_REQ_SIZE_TL)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char userNameLen = (unsigned char)_packed[FIRST_FIELD_LEN_POS];
    if (GROUP_REQ_SIZE_TL + userNameLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char groupNameLen = (unsigned char)_packed[FIRST_FIELD_POS + userNameLen];
    if (GROUP_REQ_SIZE_TL + userNameLen + groupNameLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    memcpy(_groupReq->m_userName, &_packed[FIRST_FIELD_POS], userNameLen);
    memcpy(_groupReq->m_groupName, &_packed[FIRST_FIELD_POS + userNameLen + 1], groupNameLen);

    _groupReq->m_userName[userNameLen] = '\0';
    _groupReq->m_groupName[groupNameLen] = '\0';

    return PROTOCOL_SUCCESS;
}

static size_t PackGroupResWithAddress(ProtocolTag _tag, char _status, const char *_address, char *_buffer)
{
    size_t addrLen;
    if (_buffer == NULL || (_status == 1 && _address == NULL))
    {
        return 0;
    }
    _buffer[TAG_POS] = _tag;
    if (_status == 0)
    {
        _buffer[TOTAL_LENGTH_POS] = GROUP_RES_SIZE_TL;
        _buffer[FIRST_FIELD_LEN_POS] = GROUP_STATUS_SIZE;
        _buffer[FIRST_FIELD_POS] = _status;
        return GROUP_RES_SIZE_TL;
    }
    addrLen = strlen(_address);
    _buffer[TOTAL_LENGTH_POS] = GROUP_RES_SIZE_TL + addrLen;
    _buffer[FIRST_FIELD_LEN_POS] = GROUP_STATUS_SIZE;
    _buffer[FIRST_FIELD_POS] = _status;
    _buffer[FIRST_FIELD_POS + 1] = (char)addrLen;
    memcpy(&_buffer[FIRST_FIELD_POS + 2], _address, addrLen);
    return GROUP_RES_SIZE_TL + addrLen + 2;
}

static ProtocolResult UnPackGroupResWithAddress(char *_buffer, size_t _length, char *_status, char **_address, ProtocolTag _expectedTag)
{
    unsigned char addrLen;

    if (_buffer == NULL || _length < GROUP_RES_SIZE_TL || _status == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    if (_buffer[TAG_POS] != _expectedTag)
    {
        return PROTOCOL_UNPACK_FAILURE;
    }
    *_status = _buffer[FIRST_FIELD_POS];

    if (*_status == 0)
    {
        *_address = NULL;
        return PROTOCOL_SUCCESS;
    }
    addrLen = (unsigned char)_buffer[FIRST_FIELD_POS + 1];
    if (_length < GROUP_RES_SIZE_TL + addrLen)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    *_address = (char *)malloc(addrLen + 1);
    if (*_address == NULL)
    {
        return PROTOCOL_UNINITIALIZED_ERROR;
    }
    memcpy(*_address, &_buffer[FIRST_FIELD_POS + 2], addrLen);
    (*_address)[addrLen] = '\0';

    return PROTOCOL_SUCCESS;
}

static size_t PackGetGroupsReqWrapper(ProtocolTag _tag, char *_userName, char *_packed)
{
    size_t userNameLen = strlen(_userName);

    size_t length = GET_GROUPS_REQ_SIZE_TL + userNameLen;

    if (length >= MAX_MSG_SIZE)
    {
        return 0;
    }

    _packed[TAG_POS] = (char)_tag;
    _packed[TOTAL_LENGTH_POS] = (char)length;
    _packed[FIRST_FIELD_LEN_POS] = (char)userNameLen;
    memcpy(&_packed[FIRST_FIELD_POS], _userName, userNameLen);

    return length;
}

static ProtocolResult UnPackGetGroupsReqWrapper(ProtocolTag _tag, char *_packed, size_t _length, char *_userName)
{
    if (_length < GET_GROUPS_REQ_SIZE_TL)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }
    unsigned char userNameLen = (unsigned char)_packed[FIRST_FIELD_LEN_POS];
    if (GET_GROUPS_REQ_SIZE_TL + userNameLen > _length) 
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }
    memcpy(_userName, &_packed[FIRST_FIELD_POS], userNameLen);
    _userName[userNameLen] = '\0';

    return PROTOCOL_SUCCESS;
}

static size_t PackGetGroupsResWrapper(ProtocolTag _tag, char _status, char *_groupsList, char *_packed)
{
    size_t groupsListLen = (_groupsList != NULL) ? strlen(_groupsList) : 0; 
    size_t length = GET_GROUPS_RES_SIZE_TL + groupsListLen;

    if (length >= MAX_MSG_SIZE)
    {
        return 0;
    }
    _packed[TAG_POS] = (char)_tag;
    _packed[TOTAL_LENGTH_POS] = (char)length;
    _packed[FIRST_FIELD_LEN_POS] = GROUP_STATUS_SIZE;
    _packed[FIRST_FIELD_POS] = _status;
    
    if (_status == 1 && _groupsList != NULL)  
    {
        _packed[FIRST_FIELD_POS + GROUP_STATUS_SIZE] = (char)groupsListLen;
        memcpy(&_packed[GROUP_STATUS_SIZE + GET_GROUPS_RES_SIZE_TL], _groupsList, groupsListLen);
    }

    return length;
}

static ProtocolResult UnPackCreateGroupResWrapper(ProtocolTag _tag, char *_packed, size_t _length, char *_status, char *_groupsList)
{
    if (_length < GET_GROUPS_RES_SIZE_TL)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    unsigned char groupsListLen = (unsigned char)_packed[FIRST_FIELD_POS + GROUP_STATUS_SIZE];

    if (GET_GROUPS_RES_SIZE_TL + groupsListLen > _length)
    {
        return PROTOCOL_INVALID_BUFFER_LENGTH;
    }

    *_status = _packed[FIRST_FIELD_POS];

    memcpy(_groupsList, &_packed[FIRST_FIELD_POS + GROUP_STATUS_SIZE + 1], groupsListLen);

    _groupsList[groupsListLen] = '\0';

    return PROTOCOL_SUCCESS;
}
