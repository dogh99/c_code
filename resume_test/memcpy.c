/*不使用任何标准库内存拷贝函数，使用 C 语言实现 memcpy 的核心功能。
函数接口如下：
void *my_memcpy(void *dst,
                const void *src,
                size_t n);


功能要求：
1. 将源地址 src 开始的 n 个字节复制到目标地址 dst。
2. 函数返回目标地址 dst。
3. 能够正确处理普通字符数组、整型数组和结构体所占用的字节数据。
4. 能够处理 n == 0 的情况。
5. 本题约定 n > 0 时，源和目标地址均有效且内存区域不重叠。
6. 不允许调用 memcpy、memmove 或其他标准库拷贝函数。
追加问题：
如果要提高大块内存的复制速度，你会从哪些方面考虑优化？请说明对齐、不同数据宽度访问及可移植性方面的约束。*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

void* my_memcpy(void *dst, const void *src, size_t n)
{
    const size_t alignment = _Alignof(unsigned long);
    unsigned char* d = (unsigned char*)dst;
    unsigned char* s = (unsigned char*)src;
    if (n == 0)
    {
        return dst;
    }
    

    while (n > 0)
    {
        if (n >= sizeof(unsigned long) && (unsigned long)d % alignment == 0 && (unsigned long)s % alignment == 0)
        {

            //*(unsigned long*)d = *(unsigned long*)s;
            unsigned long* ld = (unsigned long*)d;
            unsigned long* ls = (unsigned long*)s;
            *ld = *ls;
            d = d + sizeof(unsigned long);
            s = s + sizeof(unsigned long);
            n = n - sizeof(unsigned long);
            continue;
        }
        *d = *s;
        d++;
        s++;
        n--;
    }
    return dst;
    
}

int main()
{
    return 0;
}

