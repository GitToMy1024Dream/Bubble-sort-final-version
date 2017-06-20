#include <stdio.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 1024
MYSQL mysql;//mysql���
void Connect(const char* host,const char* user,const char* passwd,const char* db)
{
	mysql_init(&mysql);//��ʼ��
	if(!mysql_real_connect(&mysql,host,user,passwd,db,3306,NULL,0))
	{
		printf("mysql_real_connect faile\n");
		exit(0);
	}
	printf("mysql connect success\n");
}
//����
void Insert(const char* msg)
{
	if(msg)
	{
		char buf[SIZE] = "insert into book(name,tel,mail,addr) values";
		strcat(buf,msg);
		int len = strlen(buf);
		//ִ�гɹ�����0��ʧ�ܷ��ط�0
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query insert faile\n");
			return ;
		}
		printf("Insert success.\n");
	}
}
//ɾ��
void Delete(const char* condition)
{
	char buf[SIZE] = "delete from book where ";
	strcat(buf,condition);
	int len = strlen(buf);
	//�ɹ�����0��ʧ�ܷ��ط�0
	if(mysql_real_query(&mysql,buf,len))
	{
		printf("mysql_real_query delete faile\n");
		return ;
	}
	printf("Delete success.\n");
}
//����
void Select(const char* condition)
{
	char buf[SIZE] = {0};
	//����ĳһ����Ϣ
	if(condition)
	{
		strcpy(buf,"select * from book where ");
		strcat(buf,condition);
	}
	//��ӡ���е�ȫ����Ϣ
	else 
	{
		strcpy(buf,"select * from book");
	}
	//�ɹ�����0��ʧ�ܷ��ط�0
	int len = strlen(buf);
	if(mysql_real_query(&mysql,buf,len))
	{
		printf("mysql_read_query select faile\n");
		return ;
	}
	MYSQL_RES* res;//����ָ������
	MYSQL_ROW* row;//������ȡĳһ�е�ֵ
	//��������洢����
	if(!(res=mysql_store_result(&mysql)))
	{
		printf("mysql_store_result faile\n");
		mysql_free_result(res);
		return ;
	}
	unsigned int num = mysql_num_fields(res);//��ȡ������е�����
	printf("%s\t %-3s\t %-3s\t %-5s\n","name","tel","mail","addr");
	//��ȡ������е�ÿһ����Ϣ
	while((row=mysql_fetch_row(res)))
	{
		unsigned int i = 0;
		//��ӡͬ�в�ͬ�е���Ϣ
		for(i = 0;i < num;i++)
		{
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);//�ͷ��ڴ�
}
//����ĳһ�е���Ϣ
void Updata(const char* column,const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "update book set ";
		strcat(buf,column);
		strcat(buf,"where ");
		strcat(buf,condition);
		int len = strlen(buf);
		//�ɹ�����0��ʧ�ܷ��ط�0
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query updata faile\n");
			return ;
		}
		printf("Updata success.\n");
	}
}
//��ձ��е���������
void Clear(const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "truncate table ";
		strcat(buf,condition);
		int len = strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query clear faile\n");
			return ;
		}
		printf("Clear success.\n");
	}
}
//����
void Sort(const char* condition)
{
	if(condition)
	{
		char buf[SIZE] = "select * from book order by ";
		strcat(buf,condition);
		int len = strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query sort faile\n");
			return ;
		}
		printf("Sort success.\n");
	}
	MYSQL_RES* res;//����ָ������
	MYSQL_ROW* row;//������ȡĳһ�е�ֵ
	//��������洢����
	if(!(res=mysql_store_result(&mysql)))
	{
		printf("mysql_store_result faile\n");
		mysql_free_result(res);
		return ;
	}
	unsigned int num = mysql_num_fields(res);//��ȡ������е�����
	printf("%s\t %-3s\t %-3s\t %-5s\n","name","tel","mail","addr");
	//��ȡ������е�ÿһ����Ϣ
	while((row=mysql_fetch_row(res)))
	{
		unsigned int i = 0;
		//��ӡͬ�в�ͬ�е���Ϣ
		for(i = 0;i < num;i++)
		{
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);//�ͷ��ڴ�
}
void menu()
{
	int choice = 0;
	while(1)
	{
		printf("************ Contact  ***************\n");
		printf(" ____________________________________ \n");
		printf("|    0.exit  *******  1.insert      |\n");
		printf("|    2.delete *****   3.select      |\n");
		printf("|    4.updata  ***    5.display     |\n");
		printf("|    6.clear    *     7.sort        |\n");
		printf("�^�^�^�^�^�^�^�^�^�^�^�^�^�^�^�^�^�^ \n");
		printf("��ѡ��");
		scanf("%d",&choice);
		char buf[100] = {0};
		char condition[100] = {0};
		switch(choice)
		{
			case 0:
				exit(1);
				break;
			case 1:	
				printf("������Ҫ�������ϵ����Ϣ(name,tel,mail,addr):\n");
				scanf("%s",buf);
				Insert(buf);
				break;
			case 2:
				printf("������Ҫɾ����name,tel,mail,addr(��������һ��:name='Marry'):\n");
				scanf("%s",buf);
				Delete(buf);
				break;
			case 3:
				printf("������Ҫ��ѯ��name,tel,mail,addr(��������һ��:name='Marry'):\n");
				scanf("%s",condition);
				Select(condition);
				break;
			case 4:
				printf("������Ҫ���µ�name,tel,mail,addr(����һ��):name='Marry':\n");
				scanf("%s",buf);
				Updata(buf,condition);
				break;
			case 5:
			    Select(NULL);
				break;
			case 6:
				printf("��������Ҫɾ���ı�\n");
				scanf("%s",condition);
				Clear(condition);
				break;
			case 7:
				printf("��������Ҫ��ʲô����(name desc(����) asc(����))?\n");
				scanf("%s",condition);
				Sort(condition);
				break;
			default:
				break;
		}
	}
}
int main()
{
	//����mysql���ݿ�
	Connect("localhost","root","wy19951024","contact");
	menu();
	mysql_close(&mysql);//�ر���mysql�Ĺ���
	return 0;
}
