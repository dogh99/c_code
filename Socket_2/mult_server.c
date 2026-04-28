#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>


struct SockInfo
{
    struct sockaddr_in caddr;
    int fd;
};

void* thread_comm(void* arg)
{
    int cfd = ((struct SockInfo*)arg)->fd;
    struct sockaddr_in caddr = ((struct SockInfo*)arg)->caddr;

    char ip[32];
    printf("连接上的客户端IP地址为%s,端口为%d\n",inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)),ntohs(caddr.sin_port));

    while (1)
{
    char buff[1024];
    int len = recv(cfd, buff, sizeof(buff),0);
    if (len > 0)
    {
        printf("the clien say : %s \n",buff);
        send(cfd, buff, len, 0);
    }
    else if(len == 0)
    {
        break;
    }
    else
    {
        perror("recv");
        break;
    }
}
    close(cfd);

    return NULL;
}


int main(int argc, char const *argv[])
{
//socket,创建监听的套接字
int fd = socket(AF_INET, SOCK_STREAM, 0);
if (fd == -1)
{
    perror("socket");
    return -1;
}

//bind 将创建的套接字绑定本地的IP端口
struct sockaddr_in saddr;
saddr.sin_family = AF_INET;
saddr.sin_port = htons(10086);
saddr.sin_addr.s_addr = INADDR_ANY;

int res = bind(fd, (struct sockaddr*)&saddr,sizeof(saddr));
if(res == -1)
{
    perror("bind");
    return -1;
}

//listen

res = listen(fd , 100);
if (res == -1)
{
    perror("listen");
}

//分配空间存储结构体SockInfo信息
int capacity = 100;
struct SockInfo* sockarr = (struct SockInfo*)malloc(sizeof(struct SockInfo)*capacity);
struct SockInfo* pc = sockarr;

//accept 阻塞并等待客户端的连接
struct sockaddr_in caddr;
int addrlen = sizeof(caddr);

while (1)
{
    int cfd = accept(fd, (struct sockaddr*)&caddr, &addrlen); //这里的caddr是输出参数
    if (cfd == -1)
    {
        perror("accept");
        break;
    }
    if (pc >= sockarr + capacity)
    {
        printf("连接的客户端已经到达上限了\n");
        return 0;
    }
    pc->caddr = caddr;
    pc->fd = cfd;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_comm, pc);
    pthread_detach(thread_id);
    pc++;
}

return 0;
}