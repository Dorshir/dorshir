#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__

typedef struct ClientApp ClientApp;

typedef enum ClientApp_Results 
{
	CLIENTAPP_LUCK_OF_DATA,
    CLIENTAPP_ALLOCATION_FAILED,
    CLIENTAPP_SUCSSES,
    CLIENTAPP_COMMUNICATION_ERROR,
    CLIENTAPP_SEND_FAILED,
    CLIENTAPP_RECV_FAILED,
    
}ClientAppResult;

/** 
 * @brief Create a new client with given username , password , ipServer and port.
 * @param[in] _username - Client username for login, maximum 15 charecters.
 * @param[in] _password - Client password for login, maximum 15 charecters.
 * @param[in] _ipServer - server ip number to communication.
 * @param[in] _port - port numebr to communication form the server. 
 * @return newly created client or null on failure.
 */
ClientApp* CreateApp ();


/* ADD WHEN WE FINISH THE FUNCTION */
int RunApp(ClientApp* _app);

/**
 * @brief destroy client and set *_client to null
 * @param[in] _client : client to be destroyed 
 * @details destroy client and close client's socket.
 */
void DestroyApp(ClientApp** _app);

#endif /*__CLIENTAPP_H__*/