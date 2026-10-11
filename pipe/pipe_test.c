#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t child = fork();
    if (child == 0)
    {
        //子进程读数据
        close(pipe_fd[1]);
        char* read_buf;
        size_t used = 0;
        read_buf = malloc(sizeof(char) * 100);
        while(read(pipe_fd[0], read_buf + used, sizeof(char)))
        {
            used++;
        }
        *(read_buf + used + 1) = '\0';
        printf("我收到%s", read_buf);
        close(pipe_fd[0]);
        
    }
    else{
        //父进程写端
        close(pipe_fd[0]);
        char* love_buf = "i love u"; //字面量,放在可读区域
        char* p = love_buf;
        char* write_buf;
        write_buf = malloc(sizeof(char) * 100);
        int i = 0;
        for(i; *p != '\0'; i++)
        {
            printf("发送了%c\n", *p);
            *(write_buf + i) = *p;
            write(pipe_fd[1], p, sizeof(char));
            p++;
            sleep(1);
        }
        *(write_buf + i + 1) = '\0';
        close(pipe_fd[1]);
        
    }

    return 0;
}