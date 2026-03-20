#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    printf("192.168.6.101的16进制表示为0x%X,0x%X,0x%X,0x%X\n",192,168,6,101);
    //声明结构体接收数据
    struct sockaddr_in server_addr;
    struct in_addr server_in_addr;

    in_addr_t server_in_addr_t;

    memset(&server_addr,0,sizeof(server_addr));
    memset(&server_in_addr,0,sizeof(server_in_addr));
    memset(&server_in_addr_t,0,sizeof(server_in_addr_t));

    //不推荐使用
    server_in_addr_t = inet_addr("192.168.6.101");
    printf("inet_addr : 0x%X\n",server_in_addr_t);

    //推荐
    inet_aton("192.168.6.101",&server_in_addr);
    printf("inet_aton: 0x%X\n",server_in_addr.s_addr);

    //万能方法
    inet_pton(AF_INET,"192.168.6.101",&server_in_addr.s_addr);
    printf("inet_pton : 0x%X",server_in_addr.s_addr);
    
    //本地网络地址
    printf("本地网络地址:0X%X\n",inet_lnaof(server_in_addr));


    return 0;
}