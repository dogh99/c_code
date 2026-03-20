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
    //什么两个端口号
    unsigned short local_num = 0x1f, network_num = 0;
    network_num = (0x1f);
    printf("%hX\n",network_num);
    local_num = ntohs(network_num);
    printf("%hX\n",local_num);

    return 0;
}
