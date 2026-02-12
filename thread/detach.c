#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* test(void* arg)
{
    printf("muamuamuamua\n");
    sleep(2);
    printf("emmmmmmm\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t th;
    pthread_create(&th,NULL,test,NULL);

    pthread_detach(th);

    printf("main thread open\n");
    sleep(5);
    return 0;
}
