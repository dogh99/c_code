#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

sem_t* full;
sem_t* empty;

int shared_num;


int rand_num()
{
    srand(time(NULL));
    return rand();
}

void* produce(void* arg)
{
    for (size_t i = 0; i < 5; i++)
    {
        sem_wait(empty);
        printf("第%ld轮传输\n",i+1);
        sleep(1);
        shared_num = rand_num();
        sem_post(full);
    }
    
}

void* consume(void* arg)
{
 for (size_t i = 0; i < 5; i++)
    {
        sem_wait(full);
        printf("消费者开始读取数据%d\n",shared_num);
        sleep(1);
        sem_post(empty);
    }
    
}

int main(int argc, char const *argv[])
{
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));
    sem_init(empty,0,1);
    sem_init(full,0,0);

    pthread_t th1,th2;
    pthread_create(&th1,NULL,produce,NULL);
    pthread_create(&th2,NULL,consume,NULL);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);

    sem_destroy(full);
    sem_destroy(empty);

    free(full);
    free(empty);


    return 0;
}
