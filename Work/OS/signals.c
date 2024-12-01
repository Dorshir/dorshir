#include <signal.h> /* signal */
#include <stdio.h>  /* printf, fflush */
#include <unistd.h> /* sleep */


void func()
{
    static int count = 0;
    if (count == 1)
    {
        signal(SIGINT, SIG_DFL);
    }
    ++count;
}

int main()
{
    signal(SIGINT, func);
    while (1)
    {
        printf("*");
        fflush(stdout);
        sleep(1);
    }

    return 0;
}