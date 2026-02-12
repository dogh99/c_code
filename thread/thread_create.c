#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 1024

char* buf;


void* input_read(void* argv)
{
    while (1)
    {
       ssize_t res =  read(STDIN_FILENO, buf, BUF_LEN) ;
       if ( res == 0)
       {
        break;
       }
       else if ( res == -1)
       {
        perror("read\n");
        exit(1);
       }  
    }
    
}

 void* output_read(void* argv)
{
    int i = 0;
    while (1)
    {
        /* code */
        if (buf[i])
        {
            fputc(buf[i],stdout);
            fputc('\n',stdout);
            buf[i] = 0;
            i++;
            if (i >= BUF_LEN)
            {
                i = 0; 
            }
        }else{
            sleep(1);
        }
    }
    

}


int main(int argc, char const *argv[])
//创建两个线程，第一个读取控制台信息，写入到缓存中
//将缓存信息写出到控制台
{

    buf = malloc(BUF_LEN);
    //创建线程
    pthread_t th_1;
    pthread_t th_2;

    pthread_create(&th_1,NULL,input_read,NULL);
    pthread_create(&th_2,NULL,output_read,NULL);

    pthread_join(th_1,NULL);
    pthread_join(th_2,NULL);

    free(buf);

    return 0;
}

