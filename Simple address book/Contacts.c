#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning (disable:4996)
enum Contact   //ö�ٱ���
{
	Quit,      //Ĭ��Ϊ0���������ε���+1
	Add,       //1
	Delect,    //2
	Select,    //3
	Alter,     //4
 	Show,      //5
	Empty,     //6
	Sort	      //7
};
//��ӡ�˵�
void menu()            
{
	printf("#########################################################\n");
	printf("##############      C����ʵ�ּ���ͨѶ¼     #############\n");
	printf("##############         1.�����ϵ��         #############\n");
	printf("##############         2.ɾ����ϵ��         #############\n");
	printf("##############         3.������ϵ��         #############\n");
	printf("##############         4.�޸���ϵ��         #############\n");
	printf("##############         5.��ʾ������ϵ��     #############\n");
	printf("##############         6.�����ϵ��         #############\n");
	printf("##############         7.����ϵ����������   #############\n");
	printf("##############         0.�˳�               #############");
}
//����һ���ṹ��
typedef struct Peo
{
	char name[10];
	char sex[5];
	char Old[10];
	char Phone_num[20];
	char adrress[30];
}people;
people Peo[1000]; //����һ�����Դ洢1000�û��Ľṹͼ����
//�����ϵ��
int Add_Peo(people Peo[], int n)   
{
	int i = 0;
	char select = 0;
	char Rem[20];
	while (select != 'N')   //�ж��Ƿ���������ϵ��
	{
		printf("����:");
		scanf("%s", &Peo[n + i].name);
		printf("�Ա�:");
		scanf("%s", &Peo[n + i].sex);
		printf("����:");
		scanf("%s", &Peo[n + i].Old);
		printf("�绰:");
		scanf("%s", &Peo[n + i].Phone_num);
		printf("��ַ:");
		scanf("%s", &Peo[n + i].adrress);
		gets(Rem);           //��ȡ�ַ���
		printf("\n�Ƿ�������?(Y/N):");
		scanf("%c", &select);
		i++;
	}
	return (n + i);    
}
//ɾ����ϵ��
int Del_Peo(people Peo[], int n)
{
	int i = 0;
	int j = 0;
	char Name[15];
	printf("��������Ҫɾ����ϵ�˵�������");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{       //�ж��������ṹ�������д��ڵ��Ƿ�ƥ�䣬�˴�Ϊ��ƥ�䣬��δ������ϵ������
		i++;
	}
	if (i == n)  //�ṹ��ĳ��ڣ�˵��������ϵ������
	{
		printf("û�д���ϵ�ˣ�\n\n");
		return n;
	}
	for (j = i; j < n - 1; j++) //�ҵ�ƥ�����ϵ�ˣ�ɾ���󣬽�ɾ����֮�������ǰ��һλ
	{
		strcpy(Peo[j].name, Peo[j + 1].name);
		strcpy(Peo[j].sex, Peo[j + 1].sex);
		strcpy(Peo[j].Old, Peo[j + 1].Old);
		strcpy(Peo[j].Phone_num, Peo[j + 1].Phone_num);
		strcpy(Peo[j].adrress, Peo[j + 1].adrress);
	}
	printf("ɾ���ɹ���\n");
	return (n - 1);
}
//������ϵ��
void Sel_Peo(people Peo[], int n)
{
	char Name[15];
	int i = 0;
	printf("������Ҫ�鿴��ϵ�˵�������");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{
		i++;
	}
	if (i == n)
	{
		printf("���ҵ���ϵ�˲����ڣ�\n\n");
	}
	else
	{
		printf("����:%s\n", Peo[i].name);
		printf("�Ա�:%s\n", Peo[i].sex);
		printf("����:%s\n", Peo[i].Old);
		printf("�绰:%s\n", Peo[i].Phone_num);
		printf("��ַ:%s\n", Peo[i].adrress);
	}
	printf("\n");
}
//�޸���ϵ����Ϣ
void Alter_Peo(people Peo[], int n)
{
	int i = 0;
	char Name[15];
	printf("��������Ҫ�޸ĵ���ϵ������:");
	scanf("%s", Name);
	while ((strcmp(Peo[i].name, Name) != 0) && (i < n))
	{
		i++;
	}
	if (i == n)
	{
		printf("δ�鵽����ϵ�ˣ�\n\n");
	}
	else
	{
		printf("����:");
		scanf("%s", &Peo[i].name);
		printf("�Ա�:");
		scanf("%s", &Peo[i].sex);
		printf("����:");
		scanf("%s", &Peo[i].Old);
		printf("�绰:");
		scanf("%s", &Peo[i].Phone_num);
		printf("��ַ:");
		scanf("%s", &Peo[i].adrress);
	}
}
//��ʾ������ϵ����Ϣ
void Show_Peo(people Peo[], int n)
{
	int i = 0;
	printf("   ����    �Ա�    ����    �绰           ��ַ\n");
	for (i = 0; i < n; i++)
	{
		printf("%7s%6s%8s%14s%15s\n", Peo[i].name, Peo[i].sex, Peo[i].Old, Peo[i].Phone_num, Peo[i].adrress);
	}
	printf("\n");
}
//�����ϵ����Ϣ
void Empty_Peo(people Peo[], int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		*Peo[i].name = '\0';
		*Peo[i].sex = '\0';
		*Peo[i].Old = '\0';
		*Peo[i].Phone_num = '\0';
		*Peo[i].adrress = '\0';
	}
	printf("\n");
}
//����ϵ����������
void Sort_Peo(people Peo[], int n)
{
	int i = 0;
	int j = 0;
	struct Peo tmp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i; j < n; j++)
		{
			if ((strcmp(Peo[i].name, Peo[j].name)) > 0)
			{
				tmp = Peo[i];
				Peo[i] = Peo[j];
				Peo[j] = tmp;
			}
		}
	}
}
int main()
{
	int input = 1;
	int ret = 0;
	while (input)
	{
		menu();
		printf("\n��ѡ��-->");
		scanf("%d", &input);
		switch (input)
		{
		case Add:
			ret = Add_Peo(Peo, ret);
			break;
		case Delect:
			ret = Del_Peo(Peo, ret);
			break;
		case Select:
			Sel_Peo(Peo, ret);
			break;
		case Alter:
			Alter_Peo(Peo, ret);
			break;
		case Show:
			Show_Peo(Peo, ret);
			break;
		case Empty:
			Empty_Peo(Peo, ret);
			break;
		case Sort:
			Sort_Peo(Peo, ret);
			break;
		case Quit:
			exit(0);
			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}