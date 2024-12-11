#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <stddef.h> /* size_t */

typedef struct ClientNet ClientNet;


ClientNet *ClientNet_Create(char *_serverAddr, int _port);

int ClientNet_Send(ClientNet *_clientNet, char *_message, size_t _length);

int ClientNet_Receive(ClientNet *_clientNet, char *_buffer, size_t _bufferSize);

void ClientNet_Destroy(ClientNet ** _clientnet);


#endif /* __CLIENTNET_H__ */
