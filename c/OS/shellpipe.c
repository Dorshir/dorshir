#define _XOPEN_SOURCE 700

#include <stdio.h>  /* printf */
#include <string.h> /* strcat */
#include <stdlib.h> /* size_t */

#define COMMAND_SIZE 50
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;

    char buffer[BUFFER_SIZE];
    char command1[COMMAND_SIZE] = "";
    char command2[COMMAND_SIZE] = "";
    char space[2] = " ";

    if (argc < 3)
    {
        printf("Invalid number of arguments\n");
        return -2;
    }

    size_t index = 1;
    strncat(command1, argv[index++], COMMAND_SIZE - strlen(command1) - 1);
    strncat(command1, space, COMMAND_SIZE - strlen(command1) - 1);

    while (index < argc && argv[index][0] == '-')
    {
        strncat(command1, argv[index++], COMMAND_SIZE - strlen(command1) - 1);
        strncat(command1, space, COMMAND_SIZE - strlen(command1) - 1);
    }

    if (index >= argc)
    {
        printf("Invalid number of arguments\n");
        return -2;
    }

    strncat(command2, argv[index++], COMMAND_SIZE - strlen(command2) - 1);
    strncat(command2, space, COMMAND_SIZE - strlen(command2) - 1);

    while (index < argc && argv[index][0] == '-')
    {
        strncat(command2, argv[index++], COMMAND_SIZE - strlen(command2) - 1);
        strncat(command2, space, COMMAND_SIZE - strlen(command2) - 1);
    }

    fp1 = popen(command1, "r");
    fp2 = popen(command2, "w");

    while (fgets(buffer, BUFFER_SIZE - 1, fp1) != NULL)
    {
        fputs(buffer, fp2);
    }

    if (fp1 == NULL || fp2 == NULL)
    {
        printf("ERROR in open pipe\n");
        return -1;
    }

    pclose(fp1);
    pclose(fp2);

    return 0;
}
