#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/epoll.h>

#define EPOLLSIZE 1024

int main(int argc, char const *argv[])
{
    //首先创建一个用于监听的套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);

    //绑定本机IP与端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10086);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (res == -1)
    {
        perror("bind");
        return 0;
    }
    
    //进行监听，并进行异常判断
    res = listen(lfd, 100);
    if (res == -1)
    {
        perror("listen");
        return 0;
    }
    
    // 首先创建一个epoll实例
    int epfd = epoll_create(1);
    if (epfd == -1)
    {
        perror("epoll create\n");
        exit(0);
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    struct epoll_event* evs = malloc(sizeof(struct epoll_event)*EPOLLSIZE);
    while (1)
    {
        printf("server is running\n");
        int num = epoll_wait(epfd, evs, EPOLLSIZE, -1);
        printf("epoll ready = %d\n", num);
        for (size_t i = 0; i < num; i++)
        {
            int fd = evs[i].data.fd;
            if (fd == lfd)
            {
                int cfd = accept(fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev); //ev的内容会拷贝给内核，所以原本的ev可以替换掉了
            }
            else
            {
                char* buff = malloc(sizeof(char)*1024);
                int len = recv(fd, buff, 1024*sizeof(char), 0);
                if (len == -1)
                {
                    perror("recv error\n");
                }
                
                else if (len > 0)
                {
                    printf("get the information from client: %s", buff);
                    send(fd, buff, len, 0);
                    free(buff);
                }
                else
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL ,fd, NULL);
                    close(fd);
                    printf("no say\n");    
                }
            }
            
        }
    }
    close(lfd);

    return 0;
}
