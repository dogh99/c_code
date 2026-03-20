#include "game.h"
#include <stdio.h>

void Menu()
{
    printf("*****************************************\n");
    printf("*********1.play  0.exit******************\n");
    printf("*****************************************\n");
    
}

void EnterGame()
{
    int input = 0;
    do
    {   
        Menu();
        printf("请输入相应的数字\n");
        scanf("%d",&input);
        switch (input)
        {
        case 0:
        printf("退出游戏\n");
            break;
        case 1:
        printf("开始游戏\n");
            StarGame();
            break;
        default:
        printf("无效数据\n");
            break;
        }
    }while(input);
}

void init(char array[Row_s][Col_s],int row, int col, char c)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            array[i][j] = c;
        }
    }
}

void display(char array[Row_s][Col_s],int row, int col)
{
    printf("  ");

    for (int i = 0; i < Row; i++)
    {
        printf("%d ",i+1);
    }
    printf("\n");
    
    for (int i = 1; i < row+1; i++)
    {
        printf("%d ",i);
        for (int j = 1; j < col+1; j++)
        {
            printf("%c ",array[i][j]);
        }
        printf("\n");
    }
}


void StarGame()
{
    char data[Row_s][Col_s];
    char data_show[Row_s][Col_s];
    init(data,Row_s,Col_s,'0');//字符0代表没雷，字符1代表有雷
    init(data_show,Row_s,Col_s,'*');
    
    display(data,Row,Col);
    display(data_show,Row,Col);

    SetMine(data,Row,Col);
}

void SetMine(char array[Row][Col], int row, int col)
{
    int x,y;
    for (int i = 0; i < COUNT; i++)
    {
        x = rand() % Row;
        y = rand() % Col;
        if (array[x+1][y+1] = '0')
        {
            array[x+1][y+1] = '1';
        }
        else
        {
            i--;
        }
        
    }
    
}