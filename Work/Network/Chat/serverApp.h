#ifndef SERVER_APP_H
#define SERVER_APP_H

#include "protocol.h"

void ServerAppHandler(int _clientSocket, ProtocolTag _tag, char *_buffer, size_t _length, void *_context);

#endif // SERVER_APP_H

