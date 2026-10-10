#include<stdio.h>
#include <stdint.h>

int main()
{
    uint16_t x= 0x0001;
    uint8_t* y = (uint8_t*) &x;

    if(*y == 0x00)
    {
        printf("big\n");
    }
    else if(*y == 0x01)
    {
         printf("small\n");
    }
    return 0;

}