#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
    //邀请之间
    char* name = "lao";
    printf("%s %d继续在一楼",name,getpid());
    __pid_t pid = fork();
    if( pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        char* new_name = "xing";
        char* exe_path = "/home/megumi/c_code/POXIS/execve/erlou";
        char* args[] = {"/home/megumi/c_code/POXIS/execve/erlou",new_name,NULL};
        char* envs[] = {NULL};
        execve(exe_path,args,envs);
    }
    else{
        //sleep(3);
        printf("%s %d继续学习",name,getpid());
    }

    return 0;
}