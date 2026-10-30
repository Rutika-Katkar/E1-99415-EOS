#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    int pid1, pid2;

    printf("Program started !!!\n");
    fflush(stdout);

    // Create first child to execute who
    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid1 == 0)
    {
        // Child 1: Execute who
        execlp("who", "who", (char *)NULL);

        // Executes only if execlp fails
        perror("execlp who");
        _exit(1);
    }

    // Create second child to execute wc
    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        waitpid(pid1, NULL, 0);
        return 1;
    }
    else if (pid2 == 0)
    {
        // Child 2: Execute wc
        execlp("wc", "wc", (char *)NULL);

        // Executes only if execlp fails
        perror("execlp wc");
        _exit(1);
    }

    // Parent waits for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Program finished !!!\n");

    return 0;
}
