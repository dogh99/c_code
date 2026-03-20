#pragma once
#define Row 9
#define Col 9
#define Row_s 11
#define Col_s 11
#define COUNT 10

void Menu();

void EnterGame();

void StarGame();

void init(char array[Row_s][Col_s],int row, int col, char c);

void SetMine(char array[Row][Col], int row, int col);

