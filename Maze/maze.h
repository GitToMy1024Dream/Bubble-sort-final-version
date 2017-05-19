#pragma once
#include <iostream>
#include <assert.h>
#define  DataType int 
#include <stack>
#pragma warning (disable:4996)
using namespace std;
//�Թ�����ڵ�
struct Pos
{
	DataType _row;
	DataType _col;
	Pos(DataType row,DataType col)
		:_row(row)
		,_col(col)
	{}
};
//��ȡ�ļ��е��ַ�
void GetMaze(DataType* Maze, DataType n);
//��ӡ�Թ���ͼ
void PrintMap(DataType* Maze, size_t n);
//�Ƿ�Ϊͨ·
bool IsCheckPath(DataType* Maze, size_t n, Pos _p);
//�����������ҵ��ַ�
void GetMazePath(DataType* Maze, size_t n, Pos _p, stack<Pos>& _s);