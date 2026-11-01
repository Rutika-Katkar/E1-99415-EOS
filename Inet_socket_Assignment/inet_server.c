#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
    int sfd, cfd;
    int num[2], sum;

    struct sockaddr_in server;

    // 1. Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket to IP and port
    bind(sfd, (struct sockaddr *)&server, sizeof(server));

    // 4. Listen for client connections
    listen(sfd, 5);

    printf("Server waiting for client...\n");

    // 5. Accept client connection
    cfd = accept(sfd, NULL, NULL);

    // 6. Receive two numbers from client
    recv(cfd, num, sizeof(num), 0);

    printf("Received numbers: %d and %d\n", num[0], num[1]);

    // 7. Calculate sum
    sum = num[0] + num[1];

    // 8. Send sum to client
    send(cfd, &sum, sizeof(sum), 0);

    printf("Sum sent to client: %d\n", sum);

    // 9. Close sockets
    close(cfd);
    close(sfd);

    return 0;
}
