#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
// * char * path:打开文件的路径
// * int _oflag: 打开文件的模式
// *
// (1) O_RDONLY 只读模式
// (2) O WRONLY 只写模式
// (3) O_RDWR 读写模式
// (4) O_CREAT 如果不存在创建文件
// (5) O_APPEND 追加写模式
// (6) O_TRUNC 截断文件长度为0
// *
// ...可变参数: 0 CREAT 创建文件的权限 0664
// return: 文件描述符如果打开文件失败返回-1 同时设置全局变量errno表示对应的错误
// int open (const char *_path, int _oflag, ...)

//且Linux系统有文件权限的保护,默认创建的文件会被删除掉其他用户的写权限
   int fd =  open("megumi.txt",O_RDONLY | O_CREAT,0777); //数字表示三类人的权限

   if(fd == -1)
   {
    printf("打开文件失败\n");
   }

    return 0;
}