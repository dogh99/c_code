//回文判断
#include <stdio.h>
#include <string.h>


int ifcircle(char* str)
{
    if( str == NULL)
    {
        return 0;
    }

    char* p = str;
    char* q = str + strlen(str) - 1;
    while (p < q)
    {
        if (*p != *q)
        {
            return 0;
        }
        p++;
        q--;
    }
    return 1;
    
}


int main()
{
    char str[10];
    scanf("%s", str);
    int ret;
    ret = ifcircle(str);
    printf("%d",ret);
    return 0;
}
