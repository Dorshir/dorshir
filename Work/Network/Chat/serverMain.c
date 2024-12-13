#include "server_net.h"
#include "serverApp.h"
#include "protocol.h"
#include "userManager.h"
#include "groupManager.h"
#include <stdio.h>

int main()
{
   int port = 8888;
   UserManager* userMng = UserManager_Create(100);  
   if (!userMng)
   {
       printf("Failed to create user manager\n");
       return 1;
   }
   GroupManager* groupMng = GroupManager_Create(100);
   if (!groupMng)
   {
       UserManager_Destroy(&userMng);
       printf("Failed to create group manager\n");
       return 1;
   }
   ServerApp* app = ServerApp_Create(userMng, groupMng);
   if (!app)
   {
       GroupManager_Destroy(&groupMng);
       UserManager_Destroy(&userMng);
       printf("Failed to create server app\n");
       return 1;
   }
   ServerNet* server = ServerNetCreate(port, ServerAppHandler, app);
   if (!server)
   {
       ServerApp_Destroy(&app);
       GroupManager_Destroy(&groupMng);
       UserManager_Destroy(&userMng);
       printf("Failed to create server\n");
       return 1;
   }

   printf("Server is running on port %d...\n", port);
   ServerNetRun(server);
   ServerNetDestroy(&server);
   ServerApp_Destroy(&app);
   GroupManager_Destroy(&groupMng);
   UserManager_Destroy(&userMng);
   
   return 0;
}
