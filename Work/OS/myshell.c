#include <unistd.h>   /* fork */
#include <sys/wait.h> /* waitpid */
#include <stdio.h>    /* printf */
#include <string.h>   /* strtok, strcmp */
#define TRUE 1
#define MAX_COMMAND_LENGTH 50
#define MAX_NUM_OF_PARAMS 10

typedef enum Status
{
    INVALID_NUM_OF_PARAMS,
    READ_COMMAND_FAILED,
    SUCCESS
} Status;

static void TypePrompt();
static int ReadCommand(char *_command, char **_parameters);
static void PrintExitStatus(Status status);

int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *parameters[MAX_NUM_OF_PARAMS];
    int status;
    Status readStatus;

    while (TRUE)
    {
        TypePrompt();
        readStatus = ReadCommand(command, parameters);
        if (readStatus != SUCCESS)
        {
            PrintExitStatus(readStatus);
            continue;
        }
        else if (parameters[0] == NULL)
        {
            continue;
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }

        if (fork() != 0)
        {
            /* Parent Code */
            waitpid(-1, &status, 0);
        }
        else
        {
            /* Child code */
            execvp(parameters[0], parameters);
            printf("Failed execute command: %s\n", command);
            break;
        }
    }
    return 0;
}

static void TypePrompt()
{
    printf("MyShell: ");
}

static int ReadCommand(char *_command, char **_parameters)
{
    size_t index = 0;
    char *nextTok;
    char *delim = " \n";

    if (fgets(_command, MAX_COMMAND_LENGTH, stdin) == NULL)
    {
        return READ_COMMAND_FAILED;
    }
    nextTok = strtok(_command, delim);
    while (nextTok != NULL && index < MAX_NUM_OF_PARAMS)
    {
        _parameters[index++] = nextTok;
        nextTok = strtok(NULL, delim);
    }
    _parameters[index] = NULL;
    if (nextTok != NULL)
    {
        return INVALID_NUM_OF_PARAMS;
    }
    return SUCCESS;
}

static void PrintExitStatus(Status status)
{
    printf("Read command failed.");
    if (status == INVALID_NUM_OF_PARAMS)
    {
        printf("Invalid number of parameters. Allowed <= %d.\n", MAX_NUM_OF_PARAMS);
    }
    printf("fgets failed.\n");
}
