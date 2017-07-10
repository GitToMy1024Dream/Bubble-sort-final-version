#pragma once
#include  <stdio.h>
#include  <time.h>
#include  <conio.h>
#include  <stdlib.h>
#include  <graphics.h>  //ͼ�ν���ͷ�ļ�
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define WND_WIDTH   640   //���ڵĿ�
#define WND_HEIGHT  480   //��
#define BGCOLOR		RGB(220,120,150)  //������ɫ
#define SNAKESIZE   10
#define SNAKEMAX	100

//ö�ٷ���
typedef enum Ch {up=72,down=80,left=75,right=77}CH;

typedef struct Coor  //����
{
	int x;
	int y;
}COOR;

//�ṹһ����
typedef struct Snake
{
	int n;   //�ж��ٽ���  ÿһ���߶���һ��С����
	COOR sCoor[SNAKEMAX];  //100��������ɵ�����
	CH ch;
}SNAKE;

typedef struct Food
{
	COOR fCoor;   //����

	int flag;    //ʳ���Ƿ񱻳Ե��ı��
}FOOD;

SNAKE snake;
FOOD food;

//��ʼ��
void GameInit();
void GameMove();
void GameDraw();
void ChangeSnake();
void FoodCoor();
void DrawFood();
//��ʳ��
void EatFood();
void OverGame();
void BreakSnake();



