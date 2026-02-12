#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

typedef struct Thread_Res
{
    char* p;
    int len;

    /* data */
}Thread_Res;

void* th_1(void* arg){
    Thread_Res* res = malloc(sizeof(Thread_Res));
    char code = *((char*)arg);
    char* buffer = malloc(100);
    while(1)
    {
        fgets(buffer,100,stdin);
        if(buffer[0] == code)
        {
            free(buffer);
            printf("th_1 run\n");
        res->p = "muamuamua";
        res->len = strlen(res->p);

        pthread_exit((void*) res);
        }else{

            printf("th1 waitting");
        }

    }
 
}


void* th_2(void* arg){
        Thread_Res* res = malloc(sizeof(Thread_Res));
    char code = *((char*)arg);
    char* buffer = malloc(100);
    while(1)
    {
        //如果一个进程的多个线程同时使用标准的输入输出,会造成冲突的问题.导致只有一个线程使用到标准的输入输出
        fgets(buffer,100,stdin);
        if(buffer[0] == code)
        {
            free(buffer);
            printf("th_2 run\n");
        res->p = "emmmmmmm";
        res->len = strlen(res->p);

        pthread_exit((void*) res);
        }else{

            printf("th2 waitting");
        }

    }

}

int main(int argc, char const *argv[])
{
    pthread_t th1;
    pthread_t th2;
    char red = 'a';
    char white = 'b';

    Thread_Res* res_1 = NULL;
    Thread_Res* res_2 = NULL;


    pthread_create(&th1,NULL,th_1,&red);
    pthread_create(&th2,NULL,th_2,&white);

    pthread_join(th1,(void**)&res_1);
    pthread_join(th2,(void**)&res_2);
    printf("th_1 is %s\n th_2 is %s\n",res_1->p,res_2->p);
    return 0;
}
