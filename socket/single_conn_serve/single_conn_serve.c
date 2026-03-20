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
    printf("客户端关闭\n");
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

    //填写服务端地址
    server_addr.sin_family = AF_INET; //表示IPV4协议
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//表示接收任意网络接口的连接
    //inet_pton(AF_INET,"0.0.0.0",&server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(6666); 

    //创建socket流程
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);

    //绑定地址,将文件描述符和地址绑定在一起
    int temp_res = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    handle_error("bind",temp_res);
    //进入监听状态
    temp_res = listen(sockfd,128);
    handle_error("listen",temp_res);
    //获取客户端的连接,如果有新的客户端连接,返回一个套接字用于与该客户端通信
    socklen_t client_addr_len =  sizeof(client_addr);
    int commfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_len);
    handle_error("accept",commfd);

    printf("与客户端%s %d建立了连接,文件描述符是%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),commfd);

    //创建子线程用于收发消息
    pthread_create(&th_read,NULL,read_from_client,(void*)&commfd);
    pthread_create(&th_write,NULL,write_from_client,(void*)&commfd);

    pthread_join(th_read,NULL);
    pthread_join(th_write,NULL);

    printf("释放资源\n");
    close(commfd);
    close(sockfd);

    return 0;
}



