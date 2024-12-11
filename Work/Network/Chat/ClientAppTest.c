#include <stdio.h>
#include "ClientApp.h"


void CreateClientUsernameNULL()
{
    unsigned int port = 1111;
    Client* client = NULL;
    client = CreateClient(NULL,"PASSWORD","127.0.0.1",port);
    if(client == NULL)
    {
        printf("CreateClientUsernameNULL PASS\n");
        return;
    }
    else
    {
        printf("CreateClientUsernameNULL FAIL\n");
        DestroyClient(&client);    
    }
}
void CreateClientPasswordNULL()
{
    unsigned int port = 1111;
    Client* client = NULL;
    client = CreateClient("USERNAME",NULL,"127.0.0.1",port);
    if(client == NULL)
    {
        printf("CreateClientPasswordNULL PASS\n");
        return;
    }
    else
    {
        printf("CreateClientPasswordNULL FAIL\n");
        DestroyClient(&client);    
    }
    
}
void CreateClientIpServerNULL()
{
    unsigned int port = 1111;
    Client* client = NULL;
    client = CreateClient("USERNAME","PASSWORD",NULL,port);
    if(client == NULL)
    {
        printf("CreateClientIpServerNULL PASS\n");
        return;
    }
    else
    {
        printf("CreateClientIpServerNULL FAIL\n");
        DestroyClient(&client);    
    }

}
void CreateClientPortNULL()
{
    unsigned int port = 0;
    Client* client = NULL;
    client = CreateClient("USERNAME","PASSWORD","NULL",port);
    if(client == NULL)
    {
        printf("CreateClientIpServerNULL PASS\n");
        return;
    }
    else
    {
        printf("CreateClientIpServerNULL FAIL\n");
        DestroyClient(&client);    
    }    
}

int main()
{
    CreateClientUsernameNULL();
    CreateClientPasswordNULL();
    CreateClientIpServerNULL();
    CreateClientPortNULL();
    return 0;
}