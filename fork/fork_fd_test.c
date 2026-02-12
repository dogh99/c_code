#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("megumi.txt",O_CREAT | O_WRONLY | O_APPEND,0644);

    char buffer[1024];

    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        strcpy(buffer,"这是子进程写入的数据\n" );
    }
    else{
        sleep(1);
        strcpy(buffer,"这是父进程写入的数据\n" );
    }
    ssize_t bytes_write =  write(fd, buffer, strlen(buffer));
    if(bytes_write == -1){
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);

    if(pid == 0){
        printf("子进程写入完毕");
    }else{
        printf("父进程写入完毕");
    }

    return 0;
}