#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    int fd1, fd2;
    int num[2];
    int sum;

    // Create two FIFOs
    mkfifo("/home/sunbeam/client_to_server", 0666);
    mkfifo("/home/sunbeam/server_to_client", 0666);

    printf("Server started...\n");

    // Open first FIFO in read mode
    fd1 = open("/home/sunbeam/client_to_server", O_RDONLY);

    // Receive two numbers from client
    read(fd1, num, sizeof(num));

    printf("Received numbers = %d, %d\n", num[0], num[1]);

    // Calculate sum
    sum = num[0] + num[1];

    close(fd1);

    // Open second FIFO in write mode
    fd2 = open("/home/sunbeam/server_to_client", O_WRONLY);

    // Send sum to client
    write(fd2, &sum, sizeof(sum));

    close(fd2);

    printf("Server sent sum = %d\n", sum);

    return 0;
}
