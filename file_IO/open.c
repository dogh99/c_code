#include <stdio.h>

int main()
{
    FILE* iofile = fopen("test.txt","r+");
    if(iofile != NULL)
    {
        printf("666");
    }
    else
    {
        printf("打开失败了desu");
    }

    int put_res = fputs("么么么",iofile);
    if(put_res == EOF)
    {
        printf("写入失败");
    }

    int res = fclose(iofile);
    if(res != EOF) 
    {
    printf("文件关闭了desu");
    }
    return 0;
}