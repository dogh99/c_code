#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe erroe\n");
        return -1;
    }

    pid_t child;
    child = fork();

    if (child == -1)
    {
        perror("fork error\n");
    }

    if()


}