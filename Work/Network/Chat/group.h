#ifndef __GROUP_H__
#define __GROUP_H__

typedef struct Group Group;

typedef enum Group_Result
{
    GROUP_SUCCESS,
    GROUP_UNINITIALIZED_ERROR,
    GROUP_EMPTY
} GroupResult;

Group *GroupCreate(char *_groupName, char *_address);

GroupResult GroupIncrementMembers(Group *_group);

GroupResult GroupDecrementMembers(Group *_group);

void GroupDestroy(Group **_group);

char *GroupGetAddress(Group *_group);

#endif /* __GROUP_H__ */
