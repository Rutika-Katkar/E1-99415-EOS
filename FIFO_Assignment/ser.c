#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
int main(void)
{
int fd1,fd2;
int num[2];
int sum;

//Enter first number
printf("Enter first number:");
fflush(stdout);
scanf("%d",&num[0]);

//Enter second number
printf("Enter second number:");
fflush(stdout);
scanf("%d",&num[1]);

//fd1 in write mode
fd1=("home/sunbeam/client_to_server",O_WRONLY);
if(fd1==-1)
{
perror("Error opening client to server");
return 1;
}

if(write(fd1,num,sizeof(num)) != sizeof(num))
{
perror("error sending numbers");
close(fd1);
return 1;
}
close(fd1);

//open second fifo in read mode
fd2=("/home/sunbeam/server_to_client",O_RDONLY);
if(fd2==-1)
{
perror("Error opening server to client ");
return 1;
}

if(read(fd2,&sum,sizeof(sum))!=sizeof(sum))
{
perror("Error receiving sum");
close(fd2);
return 1;
}
close(fd2);

printf("received sum from server=%d\n",sum);
return 0;
}


