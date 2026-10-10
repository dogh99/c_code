#include <stdio.h>

int main()
{
    int N;
    int arr[20] = {0};
    scanf("%d", &N);
    for(size_t i = 0; i < N; i++)
    {
        scanf("%d", &arr[i]);
    }

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = N - 1; j > i; j--)
        {
            int temp = 0;
            if (arr[j] < arr[j-1])
            {
                temp = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = temp;
            }
            
        }
        
    }
    for (size_t i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}