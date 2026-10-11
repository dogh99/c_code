/*请使用 C 语言编写一个函数，用于判断当前 CPU 采用大端字节序还是小端字节序。
函数接口：
int isLittleEndian(void);


返回值要求：
- 小端模式返回 1。
- 大端模式返回 0。
要求：
1. 不能使用编译器预定义的大小端宏。
2. 不能直接查询操作系统或 CPU 型号。
3. 函数需要在常见的 32 位和 64 位平台上适用。*/

#include <stdio.h>

int isLittleEndian(void)
{
    unsigned int val = 0x12345678;
    unsigned int* p = &val;
    unsigned char* p2 = (unsigned char*)p;
    if (*p2 == 0x12)
    {
        return 0;
    }
    else if (*p2 == 0x78)
    {
        return 1;
    }
    else{
        return -1;
    }
    
    
}

int main()
{

}

