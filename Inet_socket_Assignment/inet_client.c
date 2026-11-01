#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int sfd;
    int num[2], sum;

    struct sockaddr_in server;

    // 1. Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect to server
    connect(sfd, (struct sockaddr *)&server, sizeof(server));

    // 4. Accept two numbers from user
    printf("Enter two numbers: ");
    scanf("%d %d", &num[0], &num[1]);

    // 5. Send numbers to server
    send(sfd, num, sizeof(num), 0);

    // 6. Receive sum from server
    recv(sfd, &sum, sizeof(sum), 0);

    // 7. Display result
    printf("Sum = %d\n", sum);

    // 8. Close socket
    close(sfd);

    return 0;
}
