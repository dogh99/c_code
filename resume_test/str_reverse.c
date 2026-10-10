//请使用 C 语言实现一个字符串翻转函数。
//给定一个以 '\0' 结尾的可修改字符串，将字符串中的字符顺序完全颠倒。
//例如，输入字符串为 "hello"，要求将其原地翻转。
#include <stdio.h>
#include <string.h>


void reverseString(char *str)
{
    char temp;
    if (*str =='\0')
    {
        return;
    }
    
    int n = strlen(str);
    char* head = str;
    char* tail = str + n - 1;
    while(tail > head)
    {
        temp = *tail;
        *tail = *head;
        *head = temp;
        tail--;
        head++;
    }
}

int main()
{
    char str[] = "hello";
    reverseString(str);
    printf("%s", str);
    return 0;
}