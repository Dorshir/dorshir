#ifndef SERVER_NET_H
#define SERVER_NET_H



#include <stddef.h> 
#include "protocol.h" 

typedef void (*ClientHandler)(int clientSocket, ProtocolTag tag, char *buffer, size_t length, void *context);
typedef struct ServerNet ServerNet;
ServerNet* ServerNetCreate(int port, ClientHandler handler, void *context);
void ServerNetRun(ServerNet *server);
int ServerSendMessage(int clientSocket, ProtocolTag tag, const char *payload, size_t payloadSize);
void ServerNetDestroy(ServerNet **server);

#endif // SERVER_NET_H

