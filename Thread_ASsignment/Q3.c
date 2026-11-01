#include <stdio.h>
#include <pthread.h>

#define SIZE 10

// Thread function
void *Sort(void *arg)
{
    int *arr = (int *)arg;
    int i, j, min, temp;

    // Selection sort
    for(i = 0; i < SIZE - 1; i++)
    {
        min = i;

        for(j = i + 1; j < SIZE; j++)
        {
            if(arr[j] < arr[min])
            {
                min = j;
            }
        }

        // Swap elements
        temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }

    return NULL;
}

int main(void)
{
    pthread_t th;
    int arr[SIZE];

    // Accept 10 integers
    printf("Enter 10 numbers:\n");

    for(int i = 0; i < SIZE; i++)
    {
        scanf("%d", &arr[i]);
    }

    // Create thread and pass array
    pthread_create(&th, NULL, Sort, (void *)arr);

    // Wait for sorting to complete
    pthread_join(th, NULL);

    // Print sorted array
    printf("Sorted array:\n");

    for(int i = 0; i < SIZE; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}
