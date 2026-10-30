#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
    int fd1, fd2;
    int num[2];
    int sum;

    // Read two numbers from user
    printf("Enter first number: ");
    fflush(stdout);
    scanf("%d", &num[0]);

    printf("Enter second number: ");
    fflush(stdout);
    scanf("%d", &num[1]);

    // Open first FIFO in write mode
    fd1 = open("/home/sunbeam/client_to_server", O_WRONLY);

    if (fd1 == -1)
    {
        perror("Error opening client_to_server");
        return 1;
    }

    // Send both numbers together to server
    if (write(fd1, num, sizeof(num)) != sizeof(num))
    {
        perror("Error sending numbers");
        close(fd1);
        return 1;
    }

    close(fd1);

    // Open second FIFO in read mode
    fd2 = open("/home/sunbeam/server_to_client", O_RDONLY);

    if (fd2 == -1)
    {
        perror("Error opening server_to_client");
        return 1;
    }

    // Receive sum from server
    if (read(fd2, &sum, sizeof(sum)) != sizeof(sum))
    {
        printf("Error: Did not receive the sum correctly.\n");
        close(fd2);
        return 1;
    }

    close(fd2);

    // Display result
    printf("Sum received from server = %d\n", sum);

    return 0;
}
