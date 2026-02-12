#include <stdio.h>
#include <stdlib.h>

int main()
{
    //使用标准库函数创建子进程
    //使用Linux命令创建子进程
    //return 成功返回0，失败返回错误编号
    int res = system("ping -c 100 www.baidu.com");
    if(res != 0)
    {
        perror("system");
        exit(EXIT_FAILURE);
    }
    return 0;
}
