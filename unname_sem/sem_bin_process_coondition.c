#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    //创建共享内存
    char* shm_value_name = "shm";
    int value_fd = shm_open(shm_value_name,O_CREAT | O_RDWR, 0666);

    //在共享内存上创建信号量
    char* shm_sem_name = "sem_shm";
    int sem_fd = shm_open(shm_sem_name,O_CREAT | O_RDWR, 00666);

    ftruncate(sem_fd,sizeof(sem_t));
    sem_t* sem = mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE, MAP_SHARED, sem_fd,0);
    //初始化信号量
    sem_init(sem,1,1);


    //设置共享内存的大小
    ftruncate(value_fd,sizeof(int));

    //进行内存映射
    int* value = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, value_fd,0);
    *value = 0;

    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
        exit(1);
    }else if(pid ==0)
    {
        //子进程
        sem_wait(sem);

        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);
        printf("this is child the final value is %d\n",*value);
    }else
    {
        //父进程
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);

        waitpid(pid,NULL,0);
        printf("this is father the final value is %d\n",*value);
    }

    munmap(value,sizeof(int));
    shm_unlink(shm_value_name);
    if(pid > 0)
    {
        sem_destroy(sem);
    }
    munmap(sem,sizeof(sem_t));
    shm_unlink(shm_sem_name);
    return 0;
}
