#include"chess.h"
void menu()
{
	printf(" \n                                        ************ɨ��Ӣ�۰�************\n");
	printf("                                           1--->Play ^^^^^^^^^^0---->Exit\n");
	printf("                                        ~~~~~~~~~Come to Play Game~~~~~~~~\n");
}
int main()
{
	int input=0;
	char mine[rows][lows]={0};
	char show[rows][lows]={0};
	srand((unsigned)time( NULL));
	menu();
	printf("��ѡ��->:");
	scanf("%d",&input);
    switch(input)
		{
		case 1:
			init(mine,show);
			break;
		case 0:
			break;
        default:printf("��������!");
			break;
		}
	system("pause");
	return 0;
}