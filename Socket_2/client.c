#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    //首先创建用于通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //客户端不需要对套接字进行绑定,内核会自动分配IP与端口,但需要初始化
    // struct sockaddr_in caddr;
    // caddr.sin_family = AF_INET;
    // caddr.sin_addr.s_addr = INADDR_ANY;

    //填写服务端的信息
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10086);
    saddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "192.168.1.103", &saddr.sin_addr.s_addr);

    //连接服务端
    int res = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (res == -1)
    {
        perror("connect");
        /* code */
    }
    else{
    printf("成功连接客户端\n");
   
    //发送数据
    char* buff = (char*)malloc(sizeof(char)*1024);
    sprintf(buff,"我喜欢你\n");
    send(fd, buff, strlen(buff), 0);
    sleep(1);
    }
    
    close(fd);

    return 0;
}
