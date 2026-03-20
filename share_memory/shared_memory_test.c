#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    //创建一个共享内存对象
    char shm_name[100] ={0};
    sprintf(shm_name,"/lettrt%d",getpid()); 
    
    //前一个参数是文件打开标志,后一个参数是文件访问权限
    int fd = shm_open(shm_name,O_RDWR | O_CREAT,0644);
    if(fd < 0){
    perror("shm");
    exit(1);
    }
    //设置共享内存对象的大小
    ftruncate(fd,1024);

    //进行内存映射,第一个参数是映射的起始位置,第三个是允许读写,第四个是对其他进程可见,fd与0是映射的对象与偏移量
    char* share= mmap(NULL,1024,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(share == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
        exit(1);
    }else if(pid == 0){
        strcpy(share,"你是个好人\n");
        printf("新学员完成回信%d\n",getpid());
    }else{
        waitpid(pid,NULL,0);
        printf("进程%d收到子进程%d的消息%s\n",getpid(),pid,share);
    }

    //释放映射区
    munmap(share,1024);
    //释放共享内存对象
    shm_unlink(shm_name);
    return 0;
}
