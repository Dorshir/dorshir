#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

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
    PROTOCOL_CREATE_GROUP_REQ,   
    PROTOCOL_CREATE_GROUP_RES,   
    PROTOCOL_JOIN_GROUP_REQ,     
    PROTOCOL_JOIN_GROUP_RES,
    PROTOCOL_GET_GROUPS_REQ,
    PROTOCOL_GET_GROUPS_RES, 
    PROTOCOL_LEAVE_GROUP_REQ,    
    PROTOCOL_LEAVE_GROUP_RES

} ProtocolTag;

typedef struct UserNamePass
{
    char *m_userName;
    char *m_password;
} UserNamePass;

typedef struct GroupRequest
{
    char* m_userName;
    char* m_groupName;
} GroupRequest;

typedef struct CreateGroupResponse
{
    char m_status;
    char* m_address;
} CreateGroupResponse;

typedef struct JoinGroupResponse
{
    char m_status;
    char* m_address;
} JoinGroupResponse;

ProtocolTag GetTag(char *_buffer);


/* Registration */

size_t PackRegReq(UserNamePass *_userNamePass, char *_packed);

ProtocolResult UnPackRegReq(char *_buffer, size_t _length, UserNamePass *_userNamePass);

size_t PackRegRes(char _status, char *_buffer);

ProtocolResult UnPackRegRes(char *_buffer, size_t _length, char *_status);


/* Login */

size_t PackLoginReq(UserNamePass *_userNamePass, char *_packed);

ProtocolResult UnPackLoginReq(char *_packed, size_t _length, UserNamePass *_userNamePass);

size_t PackLoginRes(char _status, char *_buffer);

ProtocolResult UnPackLoginRes(char *_buffer, size_t _length, char *_status);


/* Create Group */

size_t PackCreateGroupReq(GroupRequest* _groupReq, char* _packed);

ProtocolResult UnPackCreateGroupReq(char* _packed, size_t _length, GroupRequest* _groupReq);

size_t PackCreateGroupResWithAddress(char status, const char* address, char* buffer);

ProtocolResult UnPackCreateGroupResWithAddress(char* buffer, size_t length, CreateGroupResponse* response);


/* Join Group */

size_t PackJoinGroupReq(GroupRequest* _groupReq, char* _packed);

ProtocolResult UnPackJoinGroupReq(char* _packed, size_t _length, GroupRequest* _groupReq);

size_t PackJoinGroupResWithAddress(char status, const char* address, char* buffer);

ProtocolResult UnPackJoinGroupResWithAddress(char* buffer, size_t length, JoinGroupResponse* response);


/* Get Groups */

size_t PackGetGroupsReq(char *_userName, char *_packed);

ProtocolResult UnPackGetGroupsReq(char *_packed, size_t _length, char *_userName);

size_t PackGetGroupsRes(char _status, char *_groupsList, char *_packed);

ProtocolResult UnPackGetGroupsRes(char *_packed, size_t _length, char *_status, char *_groupsList);


/* Leave Group */

size_t PackLeaveGroupReq(GroupRequest* _groupReq, char* _packed);

ProtocolResult UnPackLeaveGroupReq(char* _packed, size_t _length, GroupRequest* _groupReq);

size_t PackLeaveGroupRes(char _status, char* _buffer);

ProtocolResult UnPackLeaveGroupRes(char* _buffer, size_t _length, char* _status);

#endif /* __PROTOCOL_H__ */
