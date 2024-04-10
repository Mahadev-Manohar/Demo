#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int fd[2];
    pid_t pid;

    if(pipe(fd)==-1)
    {
        printf("pipe failed");
    }
    pid=fork();
    if(pid<0)
    {
        printf("fork failed");
    }
    else if(pid==0)
    {
        close(fd[0]);
        int x=5;
        write(fd[1],&x,sizeof(x));
        close(fd[1]);
        wait(NULL);


    }
    else{
            close(fd[1]);
            int y;
            read(fd[1],&y,sizeof(y));
            close(fd[0]);
            printf("the fata from %d",y);



    }



}
