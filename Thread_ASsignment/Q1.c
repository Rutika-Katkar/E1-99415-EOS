#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

sem_t sem_parent, sem_child;

int main(void)
{
    // Initialize semaphores
    sem_init(&sem_parent, 0, 1);
    sem_init(&sem_child, 0, 0);

    int ret = fork();

    if (ret == 0)
    {
        // Child process
        while (1)
        {
            sem_wait(&sem_child);

            printf("Infotech\n");
            fflush(stdout);

            sem_post(&sem_parent);
        }
    }
    else
    {
        // Parent process
        while (1)
        {
            sem_wait(&sem_parent);

            printf("Sunbeam\n");
            fflush(stdout);

            sem_post(&sem_child);
        }

        wait(NULL);
    }

    sem_destroy(&sem_parent);
    sem_destroy(&sem_child);

    return 0;
}
