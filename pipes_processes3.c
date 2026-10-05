#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipe1[2];
    int pipe2[2];

    pid_t pid1;
    pid_t pid2;

    /* Check that the user entered a grep argument */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <grep argument>\n", argv[0]);
        exit(1);
    }

    /* Create the two pipes */
    if (pipe(pipe1) == -1)
    {
        perror("pipe1");
        exit(1);
    }

    if (pipe(pipe2) == -1)
    {
        perror("pipe2");
        exit(1);
    }

    /* Create P2 (Child process - grep) */
    pid1 = fork();

    if (pid1 < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0)
    {
        /* P2: Create P3 (Child's child - sort) */
        pid2 = fork();

        if (pid2 < 0)
        {
            perror("fork");
            exit(1);
        }

        if (pid2 == 0)
        {
            /* P3: Receive input from pipe2 */
            dup2(pipe2[0], STDIN_FILENO);

            /* Close unused pipe ends */
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            /* Execute sort */
            execlp("sort", "sort", (char *)NULL);

            perror("sort");
            exit(1);
        }
        else
        {
            /* P2: Receive input from pipe1 */
            dup2(pipe1[0], STDIN_FILENO);

            /* Send output to pipe2 */
            dup2(pipe2[1], STDOUT_FILENO);

            /* Close unused pipe ends */
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            /* Execute grep using the command-line argument */
            execlp("grep", "grep", argv[1], (char *)NULL);

            perror("grep");
            exit(1);
        }
    }
    else
    {
        /* P1: Parent process - sends cat output to pipe1 */

        /* Redirect standard output to pipe1 */
        dup2(pipe1[1], STDOUT_FILENO);

        /* Close unused pipe ends */
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        /* Execute cat scores */
        execlp("cat", "cat", "scores", (char *)NULL);

        perror("cat");
        exit(1);
    }

    return 0;
}
