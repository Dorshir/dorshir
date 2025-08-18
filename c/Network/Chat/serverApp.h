
#ifndef SERVER_APP_H
#define SERVER_APP_H

#include "protocol.h"
#include "userManager.h"
#include "groupManager.h"

typedef struct ServerApp ServerApp;

ServerApp *ServerApp_Create(UserManager *_userMng, GroupManager* _groupMng);
void ServerApp_Destroy(ServerApp** _app);
void ServerAppHandler(int _clientSocket, ProtocolTag _tag, char* _buffer, size_t _length, void* _context);

#endif
