#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    int ret, status;
    pid_t reaped_pid;
    
    printf("program started !!!\n\n");

    // 1. Loop to create all 5 child processes first
    for (int i = 1; i <= 5; i++)
    {
        ret = fork();
        
        if (ret < 0) {
            perror("fork failed");
            exit(1);
        }
        else if (ret == 0) // Child process
        {
            // Each child sleeps for a different multiple of 5 seconds
            // Child 1: 5s | Child 2: 10s | Child 3: 15s | Child 4: 20s | Child 5: 25s
            sleep(i * 5); 
            
            printf("child count = %d | pid = %d\n", i, getpid());
            exit(0); 
        }
    }

    // 2. Parent process block at the very end
    // The parent waits until ALL children finish before cleaning them up
    printf("Parent is waiting for all children to complete...\n\n");
    
    for (int i = 1; i <= 5; i++)
    {
        reaped_pid = wait(&status);
        printf("cleaned up child pid = %d | exit status = %d\n", reaped_pid, WEXITSTATUS(status));
    }

    printf("\nprogram finished !!!\n");
    return 0;
}

