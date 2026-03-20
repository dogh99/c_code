#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    char* shm_value_name = "/shm_name";
    int fd = shm_open(shm_value_name,O_CREAT | O_RDWR,0666);
    ftruncate(fd,sizeof(int));

    //初始化有名信号量
    char* sem_name = "/name_sem";
    sem_t* sem =  sem_open(sem_name,O_CREAT,0666,1);
    


    int* value = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    
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
    shm_unlink(sem_name);
    return 0;
}
