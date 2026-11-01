#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/sem.h>

#define SIZE        5
#define SHM_KEY     0x1234
#define SEM_KEY     0x4321

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
};


typedef struct circular_buffer
{
    int arr[SIZE];
    int front, rear;
    int count;
}cirbuf_t;

void cirbuf_init(cirbuf_t *cirbuf)
{
    cirbuf->count = 0;
    cirbuf->rear = cirbuf->front = -1;
}

void cirbuf_push(int value, cirbuf_t *cirbuf)
{
    cirbuf->rear = (cirbuf->rear + 1) % SIZE;
    cirbuf->arr[cirbuf->rear] = value;
    cirbuf->count++;
}

int cirbuf_pop(cirbuf_t *cirbuf)
{
    int value = cirbuf->arr[(cirbuf->front + 1) % SIZE];
    cirbuf->front = (cirbuf->front + 1) % SIZE;
    cirbuf->count--;
    return value;
}

int shmid;
int semid;
cirbuf_t *ptr;

void sigint_handler(int signo)
{
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    _exit(0);
}

void sigchld_handler(int signo)
{
    int status;
    wait(&status);
}

int main(void)
{
    struct sembuf ops[2];

    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchld_handler);

    semid = semget(SEM_KEY, 3, IPC_CREAT | 0600);

    union semun sun;
    unsigned short arr[3] = {5, 1, 0};
    sun.array = arr;
    semctl(semid, 0, SETALL, sun);

    // 0th index -  empty semaphore
    // 1st index -  binary semaphore
    // 2nd index - filled semaphore    

    shmid = shmget(SHM_KEY, sizeof(cirbuf_t), IPC_CREAT | 0600);

    ptr = (cirbuf_t *)shmat(shmid, NULL, 0);

    cirbuf_init(ptr);

    int pid = fork();
    if(pid == 0)
    {
        // child process - consumer
        while(1)
        {
            ops[0].sem_num = 1;
            ops[0].sem_op = -1;
            ops[0].sem_flg = 0;
            ops[1].sem_num = 2;
            ops[1].sem_op = -1;
            ops[1].sem_flg = 0;
            semop(semid, ops, 2);

            int value = cirbuf_pop(ptr);
            printf("[%d] consumer [%d] value = %d\n", getpid(), ptr->count, value);
            sleep(1);

            ops[0].sem_num = 1;
            ops[0].sem_op = +1;
            ops[0].sem_flg = 0;
            ops[1].sem_num = 0;
            ops[1].sem_op = +1;
            ops[1].sem_flg = 0;
            semop(semid, ops, 2);
        }
    }
    else
    {
        // parent process - producer
        while(1)
        {
            ops[0].sem_num = 1;
            ops[0].sem_op = -1;
            ops[0].sem_flg = 0;
            ops[1].sem_num = 0;
            ops[1].sem_op = -1;
            ops[1].sem_flg = 0;
            semop(semid, ops, 2);

            int value = rand() % 100;
            cirbuf_push(value, ptr);
            printf("[%d] producer [%d] value = %d\n", getpid(), ptr->count, value);
            sleep(1);

            ops[0].sem_num = 1;
            ops[0].sem_op = +1;
            ops[0].sem_flg = 0;
            ops[1].sem_num = 2;
            ops[1].sem_op = +1;
            ops[1].sem_flg = 0;
            semop(semid, ops, 2);
        }

    }
    
    return 0;
}


























