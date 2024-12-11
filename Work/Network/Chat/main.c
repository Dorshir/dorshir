#include "server_net.h"
#include "serverApp.h"
 #include <stdio.h>
int main()
{
    int port = 8888;
    ServerNet *server = ServerNetCreate(port, ServerAppHandler, NULL);
    if (!server)
    {
        printf("Failed to create server\n");
        return 1;
    }

    ServerNetRun(server);
    ServerNetDestroy(&server);

    return 0;
}

