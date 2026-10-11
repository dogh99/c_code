#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    pid_t child = fork();

    if(child < 0)
    {
        perror("fork erroer");
        return -1;
    }

    if(child == 0)
    {
        printf("child process %ld\n", (long)child);
    }

    printf("my process is %ld, my parent process is %ld\n", (long)getpid(), (long)getppid());

    if (child == 0)
    {
        return 0;
    }
     sleep(1);
    printf("[父进程 %ld] 子进程 PID=%ld；15 秒后调用 waitpid()。\n",
           (long)getpid(), (long)child);
    printf("现在另开终端运行：ps -o pid,ppid,stat,cmd -p %ld\n", (long)child);
    sleep(15);

    return 0;
    


    return 0;
}