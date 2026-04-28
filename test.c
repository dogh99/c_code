#include <stdio.h>

int main()
{
    int a = 100;
    printf("outer a no address is %p\n",&a);
    {
         a = 200;
        printf("in a no address is %p\n",&a);

    }
    return 0;
}