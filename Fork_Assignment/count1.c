#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    int child_count = 0;
    pid_t pid;

    printf("Parent PID: %d\n", getpid());
    printf("Testing maximum child processes... Please wait.\n\n");

    // Infinite loop used wisely
    while (1) {
        pid = fork();

        if (pid < 0) {
            // Fork failed! We hit the system maximum limit.
            printf("\n[!] Fork failed at child number: %d\n", child_count);
            
            // Print the exact system error reason (e.g., EAGAIN - Resource temporarily unavailable)
            perror("Reason for failure"); 
            break; 
        } 
        else if (pid == 0) {
            // --- CHILD PROCESS ---
            // WISE STEP: Sleep briefly so they exist simultaneously, then exit.
            // Do NOT let the child continue the loop!
            sleep(10); 
            exit(0); 
        } 
        else {
            // --- PARENT PROCESS ---
            child_count++;
            // Print progress cleanly on the same line
            printf("\rSuccessfully created children: %d", child_count);
            fflush(stdout);
        }
    }

    printf("\nProgram finished testing system limits.\n");
    return 0;
}

