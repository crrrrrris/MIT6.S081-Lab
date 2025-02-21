#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pipe1[2],pipe2[2];
    char buf[1];
    pipe(pipe1); //父->子
    pipe(pipe2); //子->父
    if(fork()!=0){
        write(pipe1[1],"a",1);
        read(pipe2[0],buf,1);
        printf("%d: received pong\n",getpid());
        wait(0); //等价于wait(NULL); 阻塞父进程，直到某个子进程退出。回收子进程的资源，防止其变成僵尸进程。
    }else{
        read(pipe1[0],buf,1);
        printf("%d: received ping\n",getpid());
        write(pipe2[1],buf,1);
    }
    exit(0);
}