#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
    if(argc < 2){
        printf("参数不够,不能到二楼");
        return 1;
    }
    printf("我是%s 编号%d 我上二楼了.我的爹是%d\n",argv[1],getpid(),getppid());
    
    return 0;
}