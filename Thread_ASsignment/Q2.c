#include <stdio.h>
#include <pthread.h>

int start, end;
int sum = 0;

// Thread function
void *Func(void *arg)
{
    for(int i = start; i <= end; i++)
    {
        sum = sum + i;
    }

    return NULL;
}

int main(void)
{
    pthread_t th;

    // Accept range from user
    printf("Enter start number: ");
    scanf("%d", &start);

    printf("Enter end number: ");
    scanf("%d", &end);

    // Create thread
    pthread_create(&th, NULL, Func, NULL);

    // Wait for thread to complete
    pthread_join(th, NULL);

    // Print result in main
    printf("Sum = %d\n", sum);

    return 0;
}
