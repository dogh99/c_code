#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    //使用fork创建子进程
    printf("muamuamuamua%d\n",getpid());

    //fork()返回一个int值，表示进程号 -1表示出错
    //在父进程中表示子进程的pid
    //在子进程中显示为0
    pid_t pid    = fork();

    //printf("%d\n",pid); //打印出两个进程号

    if(pid == 0)
    {
        printf("我是子进程desuwa%d,我的父进程是%d\n",getpid(),getppid());
    }
    else
    {
        printf("我是父进程desuwa%d,我的子进程是%d\n",getpid(),pid);
    }
    printf("我喜欢你");



    return 0;
}