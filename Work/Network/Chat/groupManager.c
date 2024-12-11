#include "groupManager.h"
#include <stdlib.h>

struct GroupManager
{

};

GroupManager* GroupManagerCreate()
{

    GroupManager* newGroupManager = malloc(sizeof(GroupManager));
    if(newGroupManager == NULL)
    {
        return NULL;
    }


    return newGroupManager;
}


void GroupManagerDelete(GroupManager ** _groupManager)
{
    if(_groupManager == NULL || *_groupManager == NULL){
        return;
    }
    free(* _groupManager);
    * _groupManager = NULL;
}