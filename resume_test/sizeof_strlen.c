#include <stdio.h>
#include <string.h>

int main(void)
{
    char a[] = "hello";
    char b[20] = "hello";
    char *p = "hello";

    char c[5] = {'h', 'e', 'l', 'l', 'o'};

    printf("%zu\n", sizeof(a));
    printf("%zu\n", strlen(a));

    printf("%zu\n", sizeof(b));
    printf("%zu\n", strlen(b));

    printf("%zu\n", sizeof(p));
    printf("%zu\n", strlen(p));

    printf("%zu\n", sizeof(c));
    printf("%zu\n", strlen(c));

    return 0;
}