#include "protocol.h"
#include "limits.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    UserNamePass userInput = {"testUser", "Pass123"};
    UserNamePass *unpackedData = malloc(sizeof(UserNamePass));
    if (unpackedData == NULL)
    {
        return -1;
    }

    unpackedData->m_password = malloc(sizeof(char) * 8);
    unpackedData->m_userName = malloc(sizeof(char) * 8);

    char buffer[MAX_MSG_SIZE];
    char packedLength;
    ProtocolResult unpackResult;

    printf("Original Data:\n");
    printf("Username: %s\n", userInput.m_userName);
    printf("Password: %s\n", userInput.m_password);

    // Pack the data
    packedLength = PackRegReq(&userInput, buffer);
    if (packedLength == 0)
    {
        printf("Error: Packing failed.\n");
        return 1;
    }

    for (size_t i = 0; i < packedLength; i++)
    {
        printf("%2X ", (int)buffer[i]);
    }

    printf("\nPacked Data Length: %d\n", packedLength);

    // Unpack the data
    unpackResult = UnPackRegReq(buffer, packedLength, unpackedData);
    if (unpackResult != PROTOCOL_SUCCESS)
    {
        printf("Error: Unpacking failed with code %d.\n", unpackResult);
        return 1;
    }

    printf("\nUnpacked Data:\n");
    printf("Username: %s\n", unpackedData->m_userName);
    printf("Password: %s\n", unpackedData->m_password);

    // Compare results
    if (strcmp(userInput.m_userName, unpackedData->m_userName) == 0 &&
        strcmp(userInput.m_password, unpackedData->m_password) == 0)
    {
        printf("\nTest passed: Data matches.\n");
    }
    else
    {
        printf("\nTest failed: Data mismatch.\n");
    }

    char inputStatus, outputStatus = 1;

    char buff[MAX_MSG_SIZE];

    packedLength = PackRegRes(outputStatus, buff);

    if (packedLength == 3)
    {
        printf("Correct length.\n");
    }

    if (UnPackRegRes(buff, packedLength, &inputStatus) != PROTOCOL_SUCCESS)
    {
        printf("Bad unpacking.\n");
    }

    printf("res status: %d\n", (int)inputStatus);

    return 0;
}
