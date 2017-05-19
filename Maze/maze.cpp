#include"maze.h"
//��ȡ�ļ��е��ַ�
void GetMaze(DataType* Maze, DataType n)
{
	FILE* fp = fopen("map.txt", "r");//��ֻ����ʽ���ļ�
	assert(fp);//����fp����fpΪNULLֱ�ӷ���
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n;)
		{
			char _cnt = fgetc(fp);//��ȡ�ļ��еĵ����ַ�
			if (_cnt == '0' || _cnt == '1')
			{
				Maze[i * n + j] = _cnt - '0';//��ȡÿһ�������Ӧ���ַ�'1'��'0'
				j++;
			}
		}
	}
	fclose(fp);//�ǵùر��ļ�����ֹ�ڴ�й©
}
//��ӡ�Թ���ͼ
void PrintMap(DataType* Maze, size_t n)
{
	//�������鼴��
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			cout << Maze[i*n + j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}
//�Ƿ�Ϊͨ·
bool IsCheckPath(DataType* Maze, size_t n, Pos _p)
{
	//����
	//1.����δ����
	//2.�����Ϊ1����Ϊͨ·
	if ((_p._row >= 0 && _p._row < n) &&
		(_p._col >= 0 && _p._col < n) &&
		(Maze[_p._row*n + _p._col] == 1))
	{
		return true;
	}
	else
		return false;
}
//�����������ҵ��ַ�(�����㷨)
void GetMazePath(DataType* Maze, size_t n, Pos _p, stack<Pos>& _s)
{
	//�߽���
	assert(_p._col >= 0 && _p._col < n
		&& _p._row >= 0 && _p._row < n);
	Pos _cur = _p;//���浱ǰ��ڵ�
	Pos _next = _cur;
	Maze[_cur._row * n + _cur._col] = 2;//����ڵ���Ϊ2��
	//��ʾ�߹��������
	_s.push(_p);//����ǰ��ڵ�ѹջ
	while (!_s.empty())//ջ��Ϊ��ʱ
	{
		//���ջ��Ԫ�صĺ�/�����굽��߽磬֤����ͨ·
		if (_s.top()._row == (n - 1) || _s.top()._col == (n - 1))
		{
			return;//ֱ������ѭ������ʾ�ɹ�
		}
		//��
		_next = _s.top();//��ȡ��ǰջ��Ԫ��
		_next._row -= 1;//������һ��
		//����ͨ·����
		if (IsCheckPath((DataType*)Maze, n, _next))
		{
			_s.push(_next);//����ǰջ��Ԫ��ѹջ
			Maze[_next._row * n + _next._col] = 2;//����Ϊ2
			//system("cls");//��������
			//PrintMap((DataType*)Maze, n);//��ӡ�������ڵ��Խ�
			//��ʵʱ������Ԫ�صı仯
			continue;//���ص�while����������
		}
		//�����������ϵķ������ƣ��ɲο��ϵ�ע��
		//��
		_next = _s.top();
		_next._col += 1;
		if (IsCheckPath((DataType*)Maze, n, _next))
		{
			_s.push(_next);
			Maze[_next._row*n + _next._col] = 2;
			//system("cls");
			//PrintMap((DataType*)Maze, n);
			continue;
		}
		//��
		_next = _s.top();
		_next._row += 1;
		if (IsCheckPath((DataType*)Maze, n, _next))
		{
			_s.push(_next);
			Maze[_next._row*n + _next._col] = 2;
			//system("cls");
			//PrintMap((DataType*)Maze, n);
			continue;
		}
		//��
		_next = _s.top();
		_next._col -= 1;
		if (IsCheckPath((DataType*)Maze, n, _next))
		{
			_s.push(_next);
			Maze[_next._row * n + _next._col] = 2;
			//system("cls");
			//PrintMap((DataType*)Maze, n);
			continue;
		}
		//û��ͨ·��������һ��
		Pos _etn = _s.top();//��ȡջ��Ԫ��
		Maze[_etn._row*n + _etn._col] = 3;//��Ϊ3����ʾ�߹�����δ��ͨ
		//system("cls");
		//PrintMap((DataType*)Maze, n);
		_s.pop();//��ջ����������һ��ĵ�������
	}
}