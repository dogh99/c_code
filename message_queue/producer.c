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
     //配置与查询消息队列的属性
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;

    struct timespec time_info;
    
    
    //消息队列名称与共享内存一样必须以/开头   
    char* mq_name = "/producer_consumer_mq";
    //创建消息队列,使用mqd_t来接收返回值
    mqd_t mqdes = mq_open(mq_name,O_RDWR | O_CREAT, 0664,&attr);
    if(mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        exit(1);
    }   

    while (1)
    {
        char buffer[100];
        ssize_t read_count = read(0,buffer,100);
        clock_gettime(0,&time_info);
        time_info.tv_sec += 5;
        if(read_count == -1)
        {
            perror("read");
        }else if (read_count == 0) //ctrl+d会读入0,停止发送的消息
        {
            printf("文件结尾退出\n");
            char eof = EOF;
            mq_timedsend(mqdes,&eof,1,0,&time_info);
            break;
        }
        mq_timedsend(mqdes,buffer,strlen(buffer),0,&time_info);
        
        
    }
    close(mqdes);
   // mq_unlink(mq_name);
    return 0;
}
