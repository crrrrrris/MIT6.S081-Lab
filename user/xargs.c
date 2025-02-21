#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    sleep(10);
    int xargc=0; //用于存储传递给 exec() 的参数个数
    char *xargv[MAXARG]; //用于存储传递给 exec() 的命令行参数
    char buf[512]; //，用于存储从标准输入读取的数据
    read(0,buf,512); //0 表示标准输入,即左边命令的输出
    char *p=buf;
    for(int i=1;i<argc;i++){ //命令行传入的参数存储到 xargv 中
        xargv[xargc++]=argv[i];
    }
    for(int i=0;i<512;i++){
        if(buf[i]=='\n'){ 
            if(fork()==0){ //子进程
                buf[i]=0; //ascii码0
                xargv[xargc++]=p; // 将当前行的指针p加入xargv
                xargv[xargc]=0;
                xargc++;
                exec(argv[1],xargv);
                exit(0);
            }else{
                p=&buf[i+1];//更新 p 为指向下一行的起始位置
                wait(0);
            }
        }
    }
    wait(0);
    exit(0);
}