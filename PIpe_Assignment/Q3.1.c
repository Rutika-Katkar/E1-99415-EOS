#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    int fd[2];
    int pid1, pid2;

    printf("Program started !!!\n");

    // Create a pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Create first child
    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    if (pid1 == 0)
    {
        // Child 1: Execute who

        close(fd[0]);  // Close read end

        dup2(fd[1], STDOUT_FILENO); // Redirect output to pipe

        close(fd[1]);

        execlp("who", "who", (char *)NULL);

        perror("execlp who");
        _exit(1);
    }

    // Create second child
    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0);
        return 1;
    }

    if (pid2 == 0)
    {
        // Child 2: Execute wc

        close(fd[1]);  // Close write end

        dup2(fd[0], STDIN_FILENO); // Redirect input from pipe

        close(fd[0]);

        execlp("wc", "wc", (char *)NULL);

        perror("execlp wc");
        _exit(1);
    }

    // Parent does not use the pipe
    close(fd[0]);
    close(fd[1]);

    // Wait for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Program finished !!!\n");

    return 0;
}
