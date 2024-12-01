#include <stdio.h>     /* printf*/
#include <unistd.h>    /* sleep */
#include <string.h>    /* strncpy */
#include <sys/types.h> /* pid_t */
#include <wait.h>      /* wait */

#define PIPE_SIZE 2
#define BUFFER_SIZE 200
#define PING_PONG_SIZE 5

#define IN(p) (p[0])
#define OUT(p) (p[1])

int main()
{
    int p1[PIPE_SIZE], p2[PIPE_SIZE];
    char buffer[BUFFER_SIZE];
    pid_t pid;

    char helloChild[] = "Hello Child";
    char helloFather[] = "Hello father";
    char end[] = "END";

    pipe(p1);
    pipe(p2);

    pid = fork();
    if (pid > 0)
    {
        close(IN(p1));
        close(OUT(p2));
        size_t count = 0;
        while (count < PING_PONG_SIZE)
        {
            write(OUT(p1), helloChild, sizeof(helloChild));
            read(IN(p2), buffer, sizeof(buffer));
            ++count;
            sleep(1);
        }

        write(OUT(p1), end, sizeof(end));
        read(IN(p2), buffer, sizeof(buffer));

        printf("%s\n", buffer);

        wait(NULL);
        close(OUT(p1));
        close(IN(p2));
    }
    else
    {
        close(OUT(p1));
        close(IN(p2));

        while (strncmp(buffer, end, BUFFER_SIZE) != 0)
        {
            write(OUT(p2), helloFather, sizeof(helloFather));
            read(IN(p1), buffer, sizeof(buffer));
        }

        write(OUT(p2), end, sizeof(end));

        close(OUT(p1));
        close(IN(p2));
    }

    return 0;
}