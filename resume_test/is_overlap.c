/*请编写一个函数，用于判断任意两段指定长度的内存是否重叠。*/


#include <stdio.h>


int isOverlap(const void *src, const void *dst, size_t n)
{
    unsigned char* s = (unsigned char*)src;
    unsigned char* d = (unsigned char*)dst;


    for(size_t i = 0; i < n; i++)
    {
        for(size_t j = 0; j < n; j++)
        {
            if (s + i == d + j)
            {
                return 1;
            }
            
        }
    }
}


int main()
{

}