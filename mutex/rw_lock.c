#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int shared_data = 0;
pthread_rwlock_t rwlock;

void* lock_writer(void* arg)
{
    pthread_rwlock_wrlock(&rwlock);
    int temp = shared_data + 1;
    sleep(1);
    shared_data = temp;
    pthread_rwlock_unlock(&rwlock);
    
    printf("当前是%s,the res is %d\n",(char*)arg,shared_data);
}

void* lock_reader(void* arg)
{
    //pthread_rwlock_rdlock(&rwlock);
    printf("当前是%s\n",(char*)arg);

}
int main(int argc, char const *argv[])
{
    pthread_rwlock_init(&rwlock,NULL);
    pthread_t th_id1,th_id2,th_id3,th_id4,th_id5,th_id6;
    pthread_create(&th_id1,NULL,lock_writer,"writer1");
    pthread_create(&th_id2,NULL,lock_writer,"writer2");

    sleep(3);
    pthread_create(&th_id3,NULL,lock_reader,"reader3");
    pthread_create(&th_id4,NULL,lock_reader,"reader4");
    pthread_create(&th_id5,NULL,lock_reader,"reader5");
    pthread_create(&th_id6,NULL,lock_reader,"reader6");

    sleep(3);


    

    
 pthread_rwlock_destroy(&rwlock);
    return 0;
}
