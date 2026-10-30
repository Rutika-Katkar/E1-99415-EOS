#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    // Create two pipes
    int arr[2][2];
    int ret;

    ret = pipe(arr[0]);  // Pipe 1: Child to Parent

    if (ret == -1)
    {
        perror("pipe 1 failed");
        return 1;
    }

    ret = pipe(arr[1]);  // Pipe 2: Parent to Child

    if (ret == -1)
    {
        perror("pipe 2 failed");
        close(arr[0][0]);
        close(arr[0][1]);
        return 1;
    }

    int arr1[2];
    int sum;
    int result;

    printf("Program started !!!\n");

    int pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process

        close(arr[0][0]);  // Close pipe 1 read end
        close(arr[1][1]);  // Close pipe 2 write end

        // Store two numbers
        arr1[0] = 10;
        arr1[1] = 20;

        // Send numbers to parent
        write(arr[0][1], arr1, sizeof(arr1));

        printf("[%d] Child: Sent %d and %d to parent\n",
               getpid(), arr1[0], arr1[1]);

        close(arr[0][1]);

        // Receive sum from parent
        read(arr[1][0], &result, sizeof(result));

        printf("[%d] Child: Received sum = %d\n",
               getpid(), result);

        close(arr[1][0]);

        _exit(0);
    }
    else
    {
        // Parent process

        close(arr[0][1]);  // Close pipe 1 write end
        close(arr[1][0]);  // Close pipe 2 read end

        // Receive two numbers from child
        read(arr[0][0], arr1, sizeof(arr1));

        printf("[%d] Parent: Received %d and %d\n",
               getpid(), arr1[0], arr1[1]);

        // Calculate sum
        sum = arr1[0] + arr1[1];

        printf("[%d] Parent: Sum = %d\n", getpid(), sum);

        close(arr[0][0]);

        // Send sum to child
        write(arr[1][1], &sum, sizeof(sum));

        close(arr[1][1]);

        // Wait for child to finish
        wait(NULL);
    }

    printf("Program finished !!!\n");

    return 0;
}
