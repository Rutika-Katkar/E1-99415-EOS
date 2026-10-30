#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    int ret, status;
    pid_t reaped_pid;
    
    printf("program started !!!\n\n");

    // Loop to create and run 5 child processes ONE BY ONE
    for (int i = 1; i <= 5; i++)
    {
        ret = fork();
        
        if (ret < 0) {
            perror("fork failed");
            exit(1);
        }
        else if (ret == 0) // Child process
        {
            // This child sleeps for 5 seconds by itself
            sleep(5); 
            
            printf("child count = %d  pid = %d\n", i, getpid());
            exit(0); // Child finishes and exits
        }
        else 
        {
            // --- PARENT BLOCK INSIDE THE LOOP ---
            // The parent stops here and waits for the child it JUST created to finish
            reaped_pid = wait(&status);
            printf("cleaned up child pid = %d , exit status = %d\n\n", reaped_pid, WEXITSTATUS(status));
            
            // Only after cleaning up this child will the loop move to the next iteration (i++)
        }
    }

    printf("program finished !!!\n");
    return 0;
}

