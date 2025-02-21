#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int pipeleft[2]){
    int p;
    read(pipeleft[0],&p,sizeof(p));
    if(p==-1)
        exit(0);
    printf("prime %d\n",p);
    int piperight[2];
    pipe(piperight);
    if(fork()==0){
        close(piperight[1]); //子进程不需要写
        close(pipeleft[0]); //子进程不需要父进程的输入管道
        prime(piperight);
    }else{
        close(piperight[0]); //父进程不需要读
        int buf;
        while(read(pipeleft[0],&buf,sizeof(buf))&&buf!=-1){
            if(buf%p!=0){ //筛除
                write(piperight[1],&buf,sizeof(buf));
            }
        }
        buf=-1;
        write(piperight[1],&buf,sizeof(buf));
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int pipe1[2];
    pipe(pipe1);
    if(fork()==0){ //子进程
        close(pipe1[1]);
        prime(pipe1);
        wait(0);
    }else{
        close(pipe1[0]);
        int i;
        for(i=2;i<=35;i++){
            write(pipe1[1],&i,sizeof(i));
        }
        i=-1;
        write(pipe1[1],&i,sizeof(i));
        wait(0);
    }
    exit(0);
}