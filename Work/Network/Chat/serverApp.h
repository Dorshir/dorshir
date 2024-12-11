
#ifndef SERVER_APP_H
#define SERVER_APP_H

#include "protocol.h"
#include "userManager.h"

typedef struct ServerApp ServerApp;

ServerApp* ServerApp_Create(UserManager* userMng);
void ServerApp_Destroy(ServerApp** _app);
void ServerAppHandler(int clientSocket, ProtocolTag tag, char* buffer, size_t length, void* context);

#endif
