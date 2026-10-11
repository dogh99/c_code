#include <stdio.h>
#include <string.h>




char first_not_in(const char *str1, const char *str2)
{
    const char* p = str1;
    const char* q = str2;
    for (size_t i = 0; i < strlen(str1); i++)
    {
        size_t j = 0;
        for (; j < strlen(str2); j++)
        {
            if (* (p + i) == *(q + j))//若有相同的直接跳出
            {
                break;
            }
        }
        if(j == strlen(str2))
            return *(p + i);
        
    }
    return -1;
    
}

char first_not_in_hash(const char *str1, const char *str2)
{
    int seen[256] = {0};
    int len2 = strlen(str2);
    int len1 = strlen(str1);

    for(size_t i = 0; i < len2; i++)
    {
        seen[(int)str2[i]] = 1; 
    }
    for (size_t i = 0; i < len1; i++)
    {
        if (seen[(int)str1[i]] == 0)
        {
            return str1[i];
        }
        
    }
    return -1;

}



int main()
{
    char* str1 = "apple";
    char* str2 = "appe";
    int ret = 0;
    ret = first_not_in_hash(str1, str2);
    printf("%c\n", ret); 
    return 0;
}