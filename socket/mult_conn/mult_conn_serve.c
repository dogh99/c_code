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

void* read_write(void* arg)
//读取客户端的数据并回复一个收到
{
    int clientfd = *(int*)arg;
    char* read_buf = NULL;
    char* write_buf = NULL;
    ssize_t count = 0, send_count = 0;

    read_buf = malloc(1024 * sizeof(char));
    write_buf = malloc(1024 * sizeof(char));

    while (1)
    {
      count = recv(clientfd,read_buf,1024,0);
        if (count < 0)
        {
            perror("count");
            break;
        }
        printf("从%d客户端收到数据%s\n",clientfd,read_buf);
        //把收到消息写到写缓存里面
        strcpy(write_buf,"收到\n");
        send_count = send(clientfd,write_buf,1024,0);
        if (send_count < 0)
        {
            perror("send");
            /* code */
        }
    }

    close(clientfd);
    free(read_buf);
    free(write_buf);
}

int main(int argc, char const *argv[])
{
    int sockfd,temp_result;
    pthread_t pid_read_write;

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(server_addr));

    server_addr.sin_family = AF_INET; //类型ipv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //监听本机上的所有ip
    server_addr.sin_port = htons(6666); //端口号

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);

    //把套接字与本地地址绑定在一起
    temp_result = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    handle_error("bind",temp_result);

    //进入监听状态
    temp_result = listen(sockfd,128);
    handle_error("listen",temp_result);


    //获取客户端的连接,需要能够接受多个连接
    socklen_t client_len = sizeof(client_addr);

    while (1)
    {
        int clientfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
        handle_error("accept",clientfd);

        printf("与客户端%s,端口号%d建立了连接\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        //和每一个客户端使用一个线程进行交互,把客户端发送的信息打印到控制台回复收到
        pthread_create(&pid_read_write,NULL,read_write,(void*)&clientfd);

        //需要等待线程结束 但是不能挂起等待
        pthread_detach(pid_read_write);

    }

    printf("释放资源");

    close(sockfd);
    
    return 0;
}
