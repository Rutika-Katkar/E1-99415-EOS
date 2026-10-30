#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int fd1,fd2;
int num[2];
int sum;
int main(void)
{


mkfifo("/home/sunbeam/client_to_server",0666);
mkfifo("/home/sunbeam/server_to_client",0666);

printf("program started..\n");
 fd1=("/home/sunbeam/client_to_server",O_RDONLY);

//receive two numbers from client
read(fd1,num,sizeof(num));

printf("Received two numbers= %d, %d ",num[0],num[1]);
sum=num[0]+num[1];
close(fd1);

fd2=("/home/sunbeam/server_to_client",O_WRONLY);

//send sum to client 
write(fd2,&sum,sizeof(sum));
close(fd2);
printf("server sent sum=%d\n",sum);
return 0;
}
