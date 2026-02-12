#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0 ;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//初始化条件变量

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* produce_th(void* arg)
{
    int item = 1;
    
    while (1)
    {
        /* code */
        pthread_mutex_lock(&mutex);
        if (count == BUFFER_SIZE)
        {
            /* code */
            pthread_cond_wait(&cond,&mutex); //会交出锁,进入等待状态，卡在这里
        }
        buffer[count] = item++;
        count++;
        printf("res is %d\n",buffer[count-1]);

        //唤醒
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
    
}

void* consume_th(void* arg)
{
    while (1)
    {
        /* code */
        pthread_mutex_lock(&mutex);

        if (count == 0)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        printf("get the res %d\n",buffer[--count]);
        
        pthread_cond_signal(&cond);


        pthread_mutex_unlock(&mutex);

    }
    
}

int main(int argc, char const *argv[])
{
    pthread_t th_read,th_write;
    pthread_create(&th_read,NULL,produce_th,NULL);
    pthread_create(&th_write,NULL,consume_th,NULL);

    pthread_join(th_read,NULL);
    pthread_join(th_write,NULL);

    
    return 0;
}
