#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;

    struct timespec time_info;
    
    char* mq_name = "/producer_consumer_mq";
    mqd_t mqdes = mq_open(mq_name,O_RDWR | O_CREAT, 0664,&attr);
    if(mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        exit(1);
    }
    char read_buffer[100];

    while (1)
    {
        /* code */
        memset(read_buffer,0,sizeof(read_buffer));
        clock_gettime(0,&time_info);
        time_info.tv_sec += 5;
        //读取消息队列的数据并进行异常判断
        if(mq_timedreceive(mqdes,read_buffer,sizeof(read_buffer),NULL,&time_info) == -1)
        {
            perror("receive");
        }
        if(read_buffer[0] == EOF)
        {
            printf("收到结束信息\n");
            break;
        }        
        printf("接收到了发送的消息\n%s\n",read_buffer);
    }

    close(mqdes);
    mq_unlink(mq_name);

    return 0;
}
