#define _POSIX_SOURCE

#include <signal.h> /* signal, sigaction */
#include <stdio.h>  /* printf, fflush */
#include <unistd.h> /* sleep */
#include <string.h> /* strncpy */

#define MAX_SIZE_ACTION 30

void func1()
{
    static int count = 0;
    if (count == 1)
    {
        signal(SIGINT, SIG_DFL);
    }
    else
    {
        signal(SIGINT, func1);
    }
    ++count;
}

void func2()
{
    static int count = 0;
    if (count == 1)
    {
        struct sigaction sig = {0};
        sig.sa_handler = SIG_DFL;
        sigaction(SIGINT, &sig, NULL);
    }
    else
    {
        signal(SIGINT, func2);
    }
    ++count;
}

void signalPros()
{
    signal(SIGINT, func1);
    while (1)
    {
        printf("*");
        fflush(stdout);
        sleep(1);
    }
}

void sigactionPros()
{
    struct sigaction sig = {0};
    sig.sa_handler = func2;

    sigaction(SIGINT, &sig, NULL);

    while (1)
    {
        printf("*");
        fflush(stdout);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid number of arguments.\n");
        return 0;
    }

    char action[MAX_SIZE_ACTION];
    strncpy(action, argv[1], MAX_SIZE_ACTION - 1);
    action[MAX_SIZE_ACTION - 1] = '\0';

    if (strcmp(action, "signal") == 0)
    {
        signalPros();
    }
    else if (strcmp(action, "sigaction") == 0)
    {
        sigactionPros();
    }
    else
    {
        printf("Invalid action name.\n");
    }

    return 0;
}