#include "Snake.h"
//��ʼ��
void GameInit()
{
	srand((unsigned int)time(NULL));
	//��һ������
	initgraph(WND_WIDTH, WND_HEIGHT); //���ص� ƴͼ ��ͼƬ���� ͼ��
	setbkcolor(BGCOLOR);   //���úñ�����ɫ
	cleardevice();     //ʹ�����úõı�����ɫˢ�±���

	mciSendString(L"open music.mp3 alias back", 0, 0, 0);
	mciSendString(L"play back repeat", 0, 0, 0);

	//��ʼ��һ��С��
	rectangle(0,0,SNAKESIZE,SNAKESIZE);

	snake.sCoor[0].x = 0; //0-640	
	snake.sCoor[0].y = 0;
	snake.n = 1;
	snake.ch = right;

	food.flag = 0;  //ʳ���Ǳ��Ե���
}

//����  ����ͼ��ͼƬ �������Ķ�����ϵ��һ��
//������
void GameMove()
{
	//�ƶ��Ĺ���
	for (int i = snake.n; i > 0; i--)
	{
		snake.sCoor[i].x = snake.sCoor[i - 1].x;
		snake.sCoor[i].y = snake.sCoor[i - 1].y;
	}

	//���ݲ�ͬ�ķ��� ȷ����ͷ���ƶ�
	switch (snake.ch)
	{
	case up:snake.sCoor[0].y -= SNAKESIZE; break;
	case down:snake.sCoor[0].y += SNAKESIZE; break;
	case left:snake.sCoor[0].x -= SNAKESIZE; break;
	case right:snake.sCoor[0].x += SNAKESIZE; break;
	}
}

//����
void GameDraw()
{
	for (int i = snake.n - 1; i >= 0; i--)
	{
		rectangle(snake.sCoor[i].x, snake.sCoor[i].y,
			snake.sCoor[i].x + SNAKESIZE,
			snake.sCoor[i].y + SNAKESIZE);
	}
}

//ͨ������ʵ���ߵķ���ĸı�
void ChangeSnake()
{
	int move;			//ascii
	move = getch();		//����ֱ�ӳ�Ч�� (�س�)  getch()
	switch (move)
	{
	case up:if(snake.ch!=down)snake.ch = up; break;
	case down:if(snake.ch!=up) snake.ch = down; break;
	case left:if(snake.ch!=right) snake.ch = left; break;
	case right:if(snake.ch!=left) snake.ch = right; break;
	}
}

//�õ�ʳ�������
void FoodCoor()
{
	food.fCoor.x = rand() % (WND_WIDTH / SNAKESIZE)*SNAKESIZE;  //0 10 20 30 40 50 60 70 80 ... 630
	food.fCoor.y = rand() % (WND_HEIGHT / SNAKESIZE)*SNAKESIZE;

	food.flag = 1;  //�������µ�ʳ�� ���ű���.
}

//��ʳ��
void DrawFood()
{
	roundrect(food.fCoor.x, food.fCoor.y,
		food.fCoor.x + SNAKESIZE, food.fCoor.y + SNAKESIZE,
		SNAKESIZE, SNAKESIZE);
}

//��ʳ��
void EatFood()
{
	if (snake.sCoor[0].x == food.fCoor.x&&snake.sCoor[0].y == food.fCoor.y)
	{
		snake.n++;
		food.flag = 0;
	}
}

void BreakSnake()
{
	//1��ͷ����ײǽ
	if (snake.sCoor[0].x < 0 || snake.sCoor[0].x>640 || snake.sCoor[0].y < 0 ||
		snake.sCoor[0].y>480)
	{
		//������Ϸ
		OverGame();
	}

	//2��ͷ����ײ���Լ�
	for (int i = snake.n - 2; i > 0; i--)
	{
		if (snake.sCoor[0].x == snake.sCoor[i].x&&
			snake.sCoor[0].y == snake.sCoor[i].y)
		{
			//����
			OverGame();
		}
	}
}

//
void OverGame()
{
	cleardevice();
	setcolor(YELLOW);
	settextstyle(50, 0, L"����");
	outtextxy(100, 100, L"10��У�м��ͣ�");
	outtextxy(200, 200, L"���� ���� �����ˣ�");
	getch();
	closegraph();
}