#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    char* pipe_path = "/tmp/myfifo";

    //read不需要再创建有名管道了
    // if (mkfifo(pipe_path,0664) != 0){
    //     perror("mkfifo");
    //     exit(1);
    // }

    int fd = open(pipe_path, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    char buf[100];
    ssize_t read_num;
    

    while ((read_num = read(fd,buf,100))>0)
    {
        write(STDOUT_FILENO,buf,read_num);
    }
    

    if( read_num < 0){
        perror("read");
        close(fd);
        exit(1);
    }
    printf("接受数据管道完成");
    

    close(fd);

    return 0;
}
