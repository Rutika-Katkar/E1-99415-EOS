#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(void)
{
//create a pipe

int arr[2];
int fd[2];
//int fd1[2];//another pipe
int ret=pipe(arr);
if(ret == -1)
{
perror("pipe() is failed");
_exit(-1);
}


int n1=10;
int n2=20;
int arr1[2];
int sum=0;
int pid=fork();

if(pid == 0)
{
//child process
close(fd[0]);
arr1[0]=n1;
arr1[1]=n2;
write (arr[1],arr1,sizeof(arr1));
printf("[%d]child:sent %d and %d to parent\n",getpid(), arr1[0],arr1[1]);
close(fd[1]);
_exit(0);
}
else
{
//parent process//write
wait(NULL);
close(fd[1]);//close write end
read(arr[0],arr1,sizeof(arr1));
printf("[%d]Parent : Received numbers %d and %d \n",getpid(),arr1[0],arr1[1]);
sum=arr1[0]+arr1[1];
//printf("%d=%d+%d",sum,n1,n2);
printf("[%d]Parent:sum=%d+%d=%d\n",getpid(),arr1[0],arr1[1],sum);
close(fd[0]);
//wait(NULL);
}
printf("program finished\n");
return 0;
}
