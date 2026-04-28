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
    
    //初始化读集合
    fd_set  readset;
    FD_ZERO(&readset);//初始化set
    FD_SET(lfd, &readset); //设置监听套接字到fdset里


    int maxfd = lfd;
    while (1)
    {
        fd_set tmp = readset;
        int res = select(maxfd+1, &tmp, NULL, NULL, NULL);
        // select()函数会修改readset的值，保留有事件发生的
        if (FD_ISSET(lfd, &tmp)) //FD_ISSET的作用是判定文件描述符是否被标记为就绪状态
        {
            // 接收客户端的连接
            int cfd = accept(lfd, NULL, NULL);
            FD_SET(cfd, &readset);
            if (cfd > maxfd)
            {
                maxfd = cfd;
            }
        }
            for (int i = 0; i <= maxfd; i++)
            {
                if (i != lfd && FD_ISSET(i, &tmp))
                {
                    // 接收数据
                    char buffer[1024];
                    int len = recv(i, buffer, sizeof(buffer), 0);
                    if (len == -1)
                    {
                        perror("recv");
                        exit(1);
                    }
                    else if(len == 0)
                    {
                        printf("客户端已经断开了连接\n");
                        FD_CLR(i, &readset);
                        close(i);
                        break;
                    }
                    printf("read buf = %s\n",buffer);

                    // 把数据发回给客户端
                    res = send(i, buffer, strlen(buffer), 0);
                    if (res == -1)
                    {
                        perror("send");
                        exit(1);
                    }
                }
                
            }
        

    }
    
    close(lfd);

    return 0;
}
