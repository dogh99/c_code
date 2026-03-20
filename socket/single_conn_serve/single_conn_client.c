#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define handle_error(cmd, result) \
    if(result < 0) \
    { \
        perror(cmd);\
        return -1;\
    }\

void* read_from_client(void* arg)
{
    char* read_buf = NULL;
    int client_fd  = *(int*)arg;

    read_buf = malloc(sizeof(char)*1024); 
    ssize_t count = 0;

    if (!read_buf)
    {
        perror("malloc");
        exit(-1);
    }
    //接收数据
    while (count = recv(client_fd,read_buf,1024,0))
    {
        if (count < 0)
        {
            perror("recv");
        }
        
        fputs(read_buf,stdout);
    }
    printf("服务端关闭\n");
    free(read_buf);
    return NULL;
    
}

void* write_from_client(void* arg)
{
    //接收控制台输入的信息
    char* write_buf = NULL;
    int client_fd  = *(int*)arg;

    write_buf = malloc(sizeof(char)*1024); 
    ssize_t count = 0;

    if (!write_buf)
    {
        perror("malloc");
        exit(-1);
    }
    while (fgets(write_buf,1024,stdin) != NULL)
    {
        int count = send(client_fd,write_buf,1024,0);
        if (count < 0)
        {
            perror("send");
        }
        
    }
    printf("接收到控制台的关闭请求 不在写入,关闭连接\n");
    shutdown(client_fd,SHUT_WR);
    free(write_buf);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t th_read,th_write;
    struct sockaddr_in server_addr, client_addr;
    //清空
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));

    //客户端一般又内核自己分配不需要bind()
    // //填写客户端地址
    // client_addr.sin_family = AF_INET;
    // inet_pton(AF_INET,"192.168.1.133",&client_addr.sin_addr);
    // client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // //client_addr.sin_port = htons(8888); 

    //填写服务端地址
    server_addr.sin_family = AF_INET;
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
    server_addr.sin_port = htons(6666); 

    //创建socket流程
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);

    //绑定
    int temp_res = bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr));
    handle_error("bind",temp_res);

    //主动连接服务端
    temp_res = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    handle_error("connect",temp_res);

    printf("连接上服务端%s,%d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));

    //创建子线程用于收发消息
    pthread_create(&th_read,NULL,read_from_client,(void*)&sockfd);
    pthread_create(&th_write,NULL,write_from_client,(void*)&sockfd);

    pthread_join(th_read,NULL);
    pthread_join(th_write,NULL);

    printf("释放资源\n");
    close(sockfd);
    return 0;
}
