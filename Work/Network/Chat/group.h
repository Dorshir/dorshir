#ifndef __GROUP_H__
#define __GROUP_H__


typedef struct Group Group;

Group* GroupCreate(char* _groupName);

void GroupDelete(Group ** _group);



#endif /* __GROUP_H__ */
