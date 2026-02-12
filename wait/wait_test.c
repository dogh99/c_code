#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    printf("老学员在校区\n");

    int subprocess;
    pid_t pid_son = fork();
    if(pid_son < 0){
        perror("fork");
        return 1;
    }
    else if(pid_son == 0)
    {
        char* args[] = {"/usr/bin/ping","-c","10","www.baidu.com",NULL};
        char* envs[] = {NULL};
        printf("%d开始练习\n",getpid());
        execve(args[0],args,envs);
    }
    else{
        printf("老爹%d等待新学员%d练习\n",getpid(),pid_son);
        //第二个参数用来接收子进程退出时的状态信息。类型int* 
        waitpid(pid_son,&subprocess,0);
    }

    printf("等待完成\n%d\n",subprocess);
    return 0;
}
