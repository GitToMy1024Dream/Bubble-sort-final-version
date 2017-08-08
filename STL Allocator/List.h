#pragma once
#include "Alloc.h"
#include "Construct.h"
#include "IteratorTraits.h"

template<class T>
struct ListNode
{
	ListNode<T>* _pNext;//���ָ��
	ListNode<T>* _pPre; //ǰ��ָ��
	T _data;
	ListNode(const T& data = T())
		:_pNext(NULL)
		,_pPre(NULL)
		,_data(data)
	{}
};

//list�ĵ�����
template<class T,class Ptr,class Ref>
class ListIterator
{
public:
	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef int DifferenceType;
	
	typedef ListIterator<T, T*, T&> Self;

	ListIterator(ListNode<T>* pNode = NULL)
		:_pNode(pNode)
	{}

	ListIterator(const Self& s)
		:_pNode(s._pNode)
	{}

	Ref operator*()
	{
		return _pNode->_data;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_pNode = _pNode->_pNext;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		_pNode = _pNode->_pNext;
		return tmp;
	}

	Self& operator--()
	{
		_pNode = _pNode->_pPre;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		_pNode = _pNode->_pPre;
		return tmp;
	}

	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}

public:
	ListNode<T>* _pNode;
};

//��ͷ����˫��ѭ������
template<class T,class Alloc = alloc>
class List
{
	typedef SimpleAlloc<ListNode<T>,Alloc> NodeAlloc;
public:
	typedef ListIterator<T, T*, T&> Iterator;
	typedef RerveseIterator<Iterator> RerveseIterator;
	typedef T& Ref;
public:
	//���캯��
	List()
	{
		_pHead = BuyNode(T());
		_pHead->_pNext = _pHead;
		_pHead->_pPre = _pHead;
	}

	//��posλ��ǰ����һ���ڵ�
	Iterator Insert(Iterator pos, const T& x)
	{
		ListNode<T>* pNewNode = BuyNode(x);
		ListNode<T>* pCur = pos._pNode;
		pNewNode->_pNext = pCur;
		pCur->_pPre->_pNext = pNewNode;
		pNewNode->_pPre = pCur->_pPre;
		pCur->_pPre = pNewNode;

		return Iterator(pNewNode);
	}

	//β��
	void PushBack(const T& data)
	{
		/*ListNode<T>* pNewNode = BuyNode(data);
		ListNode<T>* pTail = _pHead->_pPre;
		pTail->_pNext = pNewNode;
		pNewNode->_pPre = pTail;
		pNewNode->_pNext = _pHead;
		_pHead->_pPre = pNewNode;*/
		Insert(End(), data);
	}

	//ͷ��
	void PushFront(const T& x)
	{
		Insert(Begin(), x);
	}

	//�Ƴ�������pos��ָ�Ľ��
	Iterator Erase(Iterator pos)
	{
		ListNode<T>* nextNode = pos._pNode->_pNext;
		ListNode<T>* prevNode = pos._pNode->_pPre;
		prevNode->_pNext = nextNode;
		nextNode->_pPre = prevNode;
		DestroyNode(pos._pNode);
		return Iterator(nextNode);
	}


	//βɾ
	void PopBack()
	{
		/*ListNode<T>* pTail = _pHead->_pPre;
		pTail->_pPre->_pNext = pTail->_pNext;
		_pHead->_pPre = pTail->_pPre;
		DestroyNode(pTail);*/
		Iterator tmp = End();
		Erase(--tmp);
	}

	//ͷɾ
	void PopFront()
	{
		Erase(Begin());
	}
	//���
	void Clear()
	{
		Destroy(Begin(), End());
		_pHead->_pNext = _pHead;
		_pHead->_pPre = _pHead;
	}

	//��������
	~List()
	{
		Clear();
		DestroyNode(_pHead);
	}

	//����ֵΪvalue������Ԫ���Ƴ�
	void Remove(const T& value)
	{
		Iterator first = Begin();
		Iterator last = End();
		//����һ��
		while (first != last)
		{
			Iterator next = first;
			++next;
			if (*first == value) //ֵΪvalue��ɾ��
				Erase(first);
			first = next;
		}
	}

	//�Ƴ���ֵ��ͬ������Ԫ��
	void Unique()
	{
		Iterator first = Begin();
		Iterator last = End();
		//������
		if (first == last)
			return;
		Iterator next = first;
		//����һ��
		while (++next != last)
		{
			if (*first == *next)  //����������ͬԪ�ؾ�ɾ��
				Erase(next);
			else                
				first = next;    //firstʼ�ձ�֤��next����
			next = first;        //�������η�Χ
		}
	}


public:
	//begin��ͷ������һ���ڵ�
	Iterator Begin()
	{
		return Iterator(_pHead->_pNext);
	}
	Iterator Begin()const
	{
		return Iterator(_pHead->_pNext);
	}

	//end��ͷ���
	Iterator End()
	{
		return Iterator(_pHead);
	}
	Iterator End()const
	{
		return Iterator(_pHead);
	}

	//�����������rbegin��end
	RerveseIterator RBegin()
	{
		return RerveseIterator(End());
	}
	//�����������rend��begin
	RerveseIterator REnd()
	{
		return RerveseIterator(Begin());
	}

	//�ж������Ƿ�Ϊ��
	bool Empty()const
	{
		return _pHead->_pNext == _pHead;
	}

	//�����н��ĸ���
	size_t Size()const
	{
		size_t res = 0;
		Distance(Begin(), End());
		return res;
	}

	//ȡͷ��������
	Ref front()
	{
		return *Begin();
	}

	//ȡβ�ڵ������
	Ref back()
	{
		return *(--End());
	}
private:
	//�����½ڵ�
	ListNode<T>* BuyNode(const T& data)
	{
		ListNode<T>* pNewNode = NodeAlloc::Allocate();
		Construct(&(pNewNode->_data), data);
		return pNewNode;
	}
	//���ٽڵ�
	void DestroyNode(ListNode<T>* p)
	{
		Destroy(p);
		NodeAlloc::Deallocate(p);
	}

private:
	ListNode<T>* _pHead;
};

void Test6()
{
	List<int> l;
	l.PushBack(10);
	l.PushBack(20);
	l.PushBack(30);
	l.PushBack(40);
	List<int>::Iterator it = l.Begin();
	while (it != l.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	cout << "front:" << l.front() << endl;
	cout << "empty:" << l.Empty() << endl;
	cout << "size:" << l.Size() << endl;
	cout << "back:" << l.back() << endl;
	l.PopBack();
	l.PopBack();
	List<int>::Iterator its = l.Begin();
	while (its != l.End())
	{
		cout << *its << " ";
		++its;
	}
	cout << endl;
}
void Test7()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	List<int>::RerveseIterator it = l.RBegin();
	while (it != l.REnd())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	cout << "front:" << l.front() << endl;
	cout << "empty:" << l.Empty() << endl;
	cout << "size:" << l.Size() << endl;
	cout << "back:" << l.back() << endl;
}

void Test8()
{
	List<std::string> L;
	L.PushBack("apple");
	L.PushBack("query");
	L.PushBack("content");
	L.PushBack("mygirl");

	List<std::string>::Iterator it = L.Begin();
	while (it != L.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	cout << "front:" << L.front() << endl;
	cout << "empty:" << L.Empty() << endl;
	cout << "size:" << L.Size() << endl;
	cout << "back:" << L.back() << endl;
}
