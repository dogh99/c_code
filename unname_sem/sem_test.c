#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
//利用信号量实现互斥锁

#define THREAD_COUNT 20000

sem_t unname_sem;

void* add_th(void* arg)
{
    //申请资源，如果信号量的值大于1，则将值减1，如果信号量的值等于0，该线程会阻塞
    sem_wait(&unname_sem);
    int* p = (int*)arg;
    (*p)++;
    //释放资源,将信号量的值加1
    sem_post(&unname_sem);
    return (void*)0;

}

int main(int argc, char const *argv[])
{
    //信号量的地址，使用方式（0表示线程间通信，1表示进程间通信），初始值
    //返回值，成功返回0，失败返回1
    sem_init(&unname_sem,0,1);

    pthread_t th_id[THREAD_COUNT];
    int num = 0;
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(th_id + i,NULL,add_th,&num);
        /* code */
    }
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(th_id[i],NULL);
        /* code */
    }
    printf("num = %d\n",num);
    //销毁信号量
    sem_destroy(&unname_sem);    
    return 0;
}
