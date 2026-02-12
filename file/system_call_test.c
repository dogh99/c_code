#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int fd = open("megumi.txt",O_RDONLY);//获取文件描述符
    printf("%d\n",fd);
    if (fd == -1)
    {
        printf("文件打开失败");
        exit(EXIT_FAILURE);   //0表示成功,其他都是失败
    }
    ssize_t byte_read; //存储读取到的数据长度

    char buffer[1024]; //用于存放读取的数据

    /**
     read()
    * int_fd: 文件描述符
    * void *__buf: 存放数据
    * size_t _nbytes: 读取数据的长度
    return: ssize_t -> long int: 读取到数据的字节长度 成功大于0 失败-1
    ssize_t read (int _fd, void * buf, size_t _nbytes)
    */

    //byte_read = read(fd, buffer, 4); //进行一次读取
    while ((byte_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO,buffer,byte_read);//STDOUT_FILENO是文件描述符类型的标准输出,即输出到屏幕
        //write()返回值是写出数据的长度
    }
    if(byte_read == -1)
    {
        printf("读取数据失败\n");
        close(fd);
        exit(EXIT_FAILURE);
    }


    return 0;
}