#define _POSIX_SOURCE

#include <signal.h>    /* signal, kill */
#include <stdio.h>     /* printf */
#include <unistd.h>    /* sleep, fork */
#include <sys/types.h> /* pid_t */

#define SEC_TO_SLEEP_FATHER 10
#define SEC_TO_SLEEP_CHILD 1
#define TRUE 1

int main()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        while (TRUE)
        {
            printf("IN CHILD\n");
            sleep(SEC_TO_SLEEP_CHILD);
        }
    }
    else
    {
        sleep(SEC_TO_SLEEP_FATHER);
        kill(pid, SIGINT);
    }

    return 0;
}