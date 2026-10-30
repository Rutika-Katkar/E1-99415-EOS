#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MSQ_KEY 0x1234

// Message structure
struct msgbuf
{
    long mtype;
    int num1;
    int num2;
    int sum;
};

int main(void)
{
    int msgid;
    int ret, status;
    struct msgbuf msg;

    printf("Program started !!!\n");

    // Create a message queue
    msgid = msgget(MSQ_KEY, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    // Create child process
    ret = fork();

    if (ret == -1)
    {
        perror("fork");
        msgctl(msgid, IPC_RMID, NULL);
        return 1;
    }

    if (ret == 0)
    {
        // Child process

        // Enter two numbers
        printf("Child: Enter first number: ");
        fflush(stdout);
        scanf("%d", &msg.num1);

        printf("Child: Enter second number: ");
        fflush(stdout);
        scanf("%d", &msg.num2);

        // Send two numbers to parent
        msg.mtype = 1;

        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        // Receive sum from parent
        msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 2, 0);

        // Print result
        printf("Child: Sum received from parent = %d\n", msg.sum);

        printf("Child process exiting...\n");
        exit(0);
    }
    else
    {
        // Parent process

        // Receive two numbers from child
        msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);

        printf("Parent: Received numbers = %d, %d\n",
               msg.num1, msg.num2);

        // Calculate sum
        msg.sum = msg.num1 + msg.num2;

        printf("Parent: Sum = %d\n", msg.sum);

        // Send sum back to child using the same queue
        msg.mtype = 2;

        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        // Wait for child to finish
        wait(&status);

        printf("Parent: Child completed.\n");

        // Delete message queue
        msgctl(msgid, IPC_RMID, NULL);

        printf("Parent process exiting...\n");
    }

    return 0;
}
