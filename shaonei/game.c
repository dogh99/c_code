#include "game.h"
#include <stdio.h>
#include <stdlib.h>   // 提供 rand() 和 srand()

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
    display(data,Row,Col);

    FindMine(data_show,data,Row,Col);

}

void SetMine(char array[Row_s][Col_s], int row, int col)
{
    int x,y;
    for (int i = 0; i < COUNT; i++)
    {
        x = rand() % row + 1; //配合rand()1到9不会将雷放到边缘地带
        y = rand() % col + 1;
        if (array[x][y] == '0')
        {
            array[x][y] = '1';
        }
        else
        {
            i--;
        }
        
    }
    
}

void FindMine(char show[Row_s][Col_s], char data[Row_s][Col_s],int row, int col)
{
    //用户输入扫雷位置,并进行判断
    int x,y;
    while (1)
    {
        printf("请输入你想扫雷的位置:\n");
        scanf("%d,%d",&x,&y);
        if (x >= 1 && x <= row && y >= 1 && y <= col)
        {
            if (data[x][y] == '1')
            {
            printf("你掉进陷阱了!!!\n");
            display(data,row,col);
            break;
            }
            else
            {
                //递归探索
                recurveSearch(show,data,Row,Col,x,y);
                //显示数据
                display(show,Row,Col);
                //判断是否胜利

            }

        }
        else
        {
            printf("你输入的数据非法!重新输入!\n");
        }   
    }
    printf("666\n"); 
}

int GetMineNum(char data[Row_s][Col_s],int x, int y)
//函数输入为当前查找格子的坐标
{
     int num = 0;
                //获取当前格子周围地雷的数量
    for ( int i = x-1; i <= x+1 ; i++)
    {
        for (int j = y-1; j <= y+1; j++)
            {
                if (data[i][j] == '1')
                {
                    num++;
                }
            }
    }
    return num;
}

void recurveSearch(char show[Row_s][Col_s], char data[Row_s][Col_s],int row, int col,int x,int y)
{
//递归结束的条件,1遇到了空白格 2当前格子的外围有地雷
if (x >= 1 && x <= row && y >= 1 && y <= col)
{
    if (show[x][y] == ' ')
    {
        return;
    }
    if (GetMineNum(data, x,y))
    {
        show[x][y] = GetMineNum(data, x,y) + '0';
        return;
    }
    show[x][y] = ' ';
    recurveSearch(show,data,row,col,x-1,y-1); // 左上
    recurveSearch(show,data,row,col,x,y-1);   // 左
    recurveSearch(show,data,row,col,x+1,y-1); // 左下
    recurveSearch(show,data,row,col,x-1,y);   // 上
    recurveSearch(show,data,row,col,x+1,y);   // 下
    recurveSearch(show,data,row,col,x-1,y+1); // 右上
    recurveSearch(show,data,row,col,x,y+1);   // 右 
    recurveSearch(show,data,row,col,x+1,y+1); // 右下
}
}