#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void* client_read(void* arg)
{
    char* read_buf = NULL;
    int sockfd = *(int*)arg;

    read_buf = malloc(sizeof(char) * 1024);

    ssize_t count = 0;
    while (count = recv(sockfd,read_buf,1024,0))
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

void* client_write(void* arg)
{
    char* write_buf = NULL;
    int sockfd = *(int*)arg;
    write_buf = malloc(sizeof(char) * 1024);
    ssize_t count = 0;

    if (!write_buf)
    {
        perror("malloc");
        exit(-1);
    }
    while (fgets(write_buf,1024,stdin) != NULL)
    {
        int count = send(sockfd,write_buf,1024,0);
        if (count < 0)
        {
            perror("send");
        }
        
    }
    printf("接收到控制台的关闭请求 不在写入,关闭连接\n");
    shutdown(sockfd,SHUT_WR);
    free(write_buf);
    return NULL;

}


int main(int argc, char const *argv[])
{
    struct sockaddr_in server_addr;

    memset(&server_addr,0,sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
    {
        perror("socket");
        /* code */
    }
    
    int temp_res = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

    printf("连接上服务端%s,%d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));

    pthread_t th_read,th_write;

    pthread_create(&th_read,NULL,client_read,(void*)&sockfd);
    pthread_create(&th_write,NULL,client_write,(void*)&sockfd);

    pthread_join(th_read,NULL);
    pthread_join(th_write,NULL);

    printf("释放资源\n");
    close(sockfd);

    return 0;
}
