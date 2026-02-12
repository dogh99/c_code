#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

//使用消息队列实现父子进程通讯
int main(int argc, char const *argv[])
{ 
    //配置与查询消息队列的属性
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;

    
    //消息队列名称与共享内存一样必须以/开头   
    char* mq_name = "/father_son_mq";
    //创建消息队列,使用mqd_t来接收返回值
    mqd_t mqdes = mq_open(mq_name,O_RDWR | O_CREAT, 0664,&attr);
    if(mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        exit(1);
    }   
    pid_t pid = fork();
    if(pid == 0)
    {//子进程,接收消息
        char read_buffer[100];
        struct timespec time_info;
        for(size_t i = 0; i<10; i++){
            memset(read_buffer,0,100);

            //设置接收数据的等待时间
            clock_gettime(0,&time_info);
            time_info.tv_sec += 15;
            //接收消息队列的数据并打印
            mq_timedreceive(mqdes,read_buffer,100,NULL,&time_info);
            printf("子进程接收到数据%s",read_buffer);
        }
    }
    else{   
        //父进程,发送数据
        char send_buffer[100];
        struct timespec time_info;
        for(size_t i = 0; i<10; i++)
        {
            memset(send_buffer,0,100);
            sprintf(send_buffer,"父进程的第%ld次发送\n",i+1);

            //获取硬件时间并存放在timespec类型的结构体指针里面
            clock_gettime(0,&time_info);
            time_info.tv_nsec += 5; 
            if (mq_timedsend(mqdes,send_buffer,strlen(send_buffer),0, &time_info) == -1){
                perror("send");
            }

            printf("父进程发送一条消息,休眠1s\n");
            sleep(1);
        }
    }
    //释放回收
    close(mqdes);
    if (pid > 0){
        mq_unlink(mq_name);
    }
    return 0;
}
