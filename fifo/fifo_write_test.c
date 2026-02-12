#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

//使用有名管道进行进程间通信,使用的是内核空间
int main(int argc, char const *argv[])
{
    char* pipe_path = "/tmp/myfifo";
    
    if (mkfifo(pipe_path,0664) != 0){
        perror("mkfifo");
        if (errno != 17 )
        exit(1);
    }

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
    //读取控制台数据写入到管道中
    while ((read_num = read(STDIN_FILENO,buf,100)) > 0){
        write(fd,buf,read_num);
    }

    if( read_num < 0){
        perror("read");
        close(fd);
        exit(1);
    }
    printf("发送数据到管道完成");
    close(fd);
    //释放管道,清除特殊文件
    unlink(pipe_path);


    return 0;
}
