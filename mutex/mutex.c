#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 20000

static pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;



void* add_th(void* arg)
{
    int* p = (int*)arg;
    //加锁
    pthread_mutex_lock(&counter);
    (*p)++;
    //解锁
    pthread_mutex_unlock(&counter);
    return (void*)0;
}

int main(int argc, char const *argv[])
{
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
    return 0;
}