#pragma once

#include "Alloc.h"
#include "Construct.h"
#include <string>

//vector�ĵ���������
#if 0
template<class T,class Ptr,class Ref>
class VectorIterator
{
public:
	typedef T ValueType;
	typedef VectorIterator<T, T*, T&> Self;
public:
	VectorIterator(T* p = NULL)    //���캯��
		:_p(p)
	{}

	VectorIterator(const T& v)     //��������
		:_p(v._p)
	{}

	Ref operator*()                //����*�����
	{
		return *_p;
	}
	 
	Ptr operator->()               //����->�����
	{
		return &(operator*());
	}

	Self& operator++()      //ǰ��++
	{
		_p++;
		return *this;
	}

	Self operator++(int)    //����++
	{
		Self tmp(*this);
		_p++;
		return tmp;
	}

	Self& operator--()      //ǰ��--
	{
		_p--;
		return *this;
	}

	Self operator--(int)    //����--
	{
		Self tmp(*this);
		_p--;
		return tmp;
	}

	bool operator!=(const Self& s)
	{
		return _p != s._p;
	}

	bool operator==(const Self& s)
	{
		return _p == s._p;
	}

private:
	T* _p;
};

#endif

template<class T,class Alloc = alloc>
class Vector
{
public:
	typedef T ValueType;
	typedef T* Iterator;
	typedef T& Ref;
	typedef const T* ConstIterator;

	typedef SimpleAlloc<T, Alloc> dataAlloc; 
public:
	Vector()   //�չ��캯��
		:_start(0)
		,_finish(0)
		,_endOfStorage(0)
	{}

	//����һ��vector������n��ֵΪvalue��Ԫ��
	Vector(size_t n, const T& value = T())
	{
		unInitFill(n, value);
	}

	//β��
	void PushBack(const T& value)
	{
		_CheckCapacity();
		*_finish = value;
		++_finish;
	}

	//βɾ
	void PopBack()
	{
		--_finish;
		Destroy(_finish);
	}

	//���[first,last]�е�����Ԫ��
	Iterator Erase(Iterator first, Iterator last)
	{
		Iterator index = copy(last, _finish, first);
		Destroy(index, _finish);
		_finish = _finish - (last - first);
		return first;
	}


	//���
	void Clear()
	{
		Erase(begin(), end());
	}

	//��������
	~Vector()
	{
		Destroy(_start, _endOfStorage);
		dataAlloc::Deallocate(&*_start, Capacity());
	}
	//���ص�һ��Ԫ�صĵ�ַ
	Iterator begin()
	{
		return _start;
	}
	//�������һ��Ԫ�صĵ�ַ
	Iterator end()
	{
		return _finish;
	}

	//vector����ЧԪ�صĸ���
	size_t Size()const
	{
		return _finish - _start;
	}

	//vector�������ɵ�Ԫ�ظ���
	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	//�ж�vector�Ƿ�Ϊ��
	bool Empty()const
	{
		return _start == _finish;
	}

	//����[]
	Ref operator[](size_t n)
	{
		return *(begin() + n);
	}

	//������Ԫ��
	Ref front()
	{
		return *begin();
	}
	//����βԪ��
	Ref back()
	{
		return *(end() - 1);
	}

protected:
	//����Ƿ���Ҫ����
	void _CheckCapacity()
	{
		if (_finish == _endOfStorage)
		{
			size_t capacity = Capacity();
			size_t newSize = 2 * capacity + 3;

			T* tmp = dataAlloc::Allocate(newSize);
			for (size_t idx = 0; idx < newSize; ++idx)
				Construct(tmp + idx, T());

			//��������->memcpy
			//�Զ�������-> for=
			//����Ԫ��
			for (size_t idx = 0; idx < Size(); ++idx)
				tmp[idx] = _start[idx];

			//�ͷžɿռ�
			//vector<int>  
			//vector<string>
			if (_start)
			{
				Destroy(_start, _finish);
				dataAlloc::Deallocate(_start, capacity);
			}
			//ָ���¿ռ�,���µ�ַ
			_start = tmp;
			_finish = _start + capacity;
			_endOfStorage = _start + newSize;
		}
	}

	//��䲢��ʼ��
	void unInitFill(size_t n, const T& value)
	{
		_start = dataAlloc::Allocate(n);
		for (size_t idx = 0; idx < n; ++idx)
		{
			Construct(_start + idx, value);
		}
	}

private:
	Iterator _start;//Ŀǰʹ�ÿռ��ͷ
	Iterator _finish;//Ŀǰʹ�ÿռ��β
	Iterator _endOfStorage;//Ŀǰ���ÿռ��β
};


void Test4()
{
	Vector<int> v;
	v.PushBack(10);
	v.PushBack(20);
	v.PushBack(30);
	v.PushBack(40);
	cout << "Distance:" << distance(v.begin(), v.end()) << endl;

	Vector<int>::Iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

void Test5()
{
	Vector<string> v;
	v.PushBack("apple");
	v.PushBack("noodles");
	v.PushBack("flower");
	v.PushBack("hello world");
	cout << "Distance:" << distance(v.begin(), v.end()) << endl;
	cout << "begin:" << v.begin() << endl;
	cout << "end:" << v.end() << endl;
	cout << "front:" << v.front() << endl;
	cout << "back:" << v.back() << endl;
	cout << "empty:" << v.Empty() << endl;
	cout << "Capacity:" << v.Capacity() << endl;
	cout << "operator[]:"<<v.operator[](3) << endl;
	Vector<string>::Iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}