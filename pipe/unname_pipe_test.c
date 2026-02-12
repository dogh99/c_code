#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){ 
    //将程序传递进来的第一个参数，通过管道传输给子进程
    pid_t cpid;
    int pipefd[2];

    if(argc < 2){
        printf("参数量不够");
        exit(EXIT_FAILURE);
    }

    //创建管道,[0]为读,[1]为写
    if(pipe(pipefd) == -1){
        perror("创建管道失败");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if(cpid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(cpid == 0){
        //子进程读取管道的数据然后打印到控制台
        close(pipefd[1]); //关闭写端
        char str[100] = {0};
        printf("%d开始读\n",getpid());
        while( read(pipefd[0], str, 1) > 0){
            write(STDOUT_FILENO, str ,1);
        }
        write(STDOUT_FILENO,"\n",1);

        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else{
        //父进程,写入管道数据提供给子进程
        close(pipefd[0]); //关闭读
        printf("%d开始写\n",getpid());

        for(int i = 1; i<argc; i++ ){
        write(pipefd[1],argv[i],strlen(argv[i]));
        }
        close(pipefd[1]); //关闭写,宣布写完了.当内核发现所有指向管道写端（fd[1]）的“钥匙”都被关掉了，它才会向读端发送 EOF。
        waitpid(cpid, NULL, 0);
    }

    return 0;
}