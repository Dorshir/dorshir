#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <stddef.h> /* size_t */

typedef struct ClientNet ClientNet;

typedef enum ClientNet_Result
{
    CLIENTNET_SUCCESS = 0,
    CLIENTNET_SEND_ERROR,
    CLIENTNET_ALLOCATION_ERROR
} ClientNetResult;


ClientNet *ClientNet_Create(char *_serverAddr, int _port);

int ClientNet_Send(ClientNet *_clientNet, char *_message, size_t _length);

int ClientNet_Receive(ClientNet *_clientNet, char *_buffer, size_t _bufferSize);

void ClientNet_Destroy(ClientNet ** _clientnet);


#endif /* __CLIENTNET_H__ */
