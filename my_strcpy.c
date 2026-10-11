#include <stdio.h>
#include <string.h>


void *my_memcpy(void *dest, const void *src, size_t n)
{
    unsigned char* d = (unsigned char*)dest;
    unsigned char* s = (unsigned char*)src;

    for(size_t i = 0; i < n; i++)
    {
        *(d+i) = *(s+i);
    }
    return dest;
}