#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    FILE* file = fopen("data.txt","w");


    //修改刷写模式
    setvbuf(file,NULL,_IONBF,0);

    //文件写入是全缓冲，等待缓冲区满或手动刷写
    fprintf(file,"hello");

    //手动刷写
    fflush(file);

    char *args[] = {"/usr/bin/ping","-c","10","www.baidu.com",NULL};
    char *envs[] = {NULL};

    //execve()一个新的可执行程序完全替换当前进程的映像（代码、数据、堆栈、环境变量等），当前进程的 PID 不变，但原来的程序彻底消失了。
    //第一个参数，要执行程序的完整路径
    //第二个参数传递给新程序的命令行参数数组
    //传给新程序的环境变量数组
    execve(args[0],args,envs);

    perror("execve");

    return 0;
}
