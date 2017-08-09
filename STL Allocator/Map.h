#pragma once

#include "Alloc.h"
#include "Construct.h"
#include "IteratorTraits.h"
#include "RBTree.h"


//pair��ֵ�ԵĶ���
template<class T1,class T2>
struct pair
{
	typedef T1 firstType;  
	typedef T2 secondType; 

	T1 first;     //key
	T2 second;	  //value

	pair()
		:first(T1())
		,second(T2())
	{}

	pair(const T1& a,const T2& b)
		:first(a)
		,second(b)
	{}

};

//map�����ݽṹ
template<class Key,class T,class Compare = less<key>,class Alloc = alloc >
class Map
{
public:
	typedef Key KeyType;  //��ֵ����
	typedef T DataType;   //ʵֵ����
	typedef T MapType;
	typedef pair<const Key, T> ValueType;//Ԫ���ͱ�
	typedef Compare KeyCompare; //��ֵ�ȽϺ���

	//Ԫ�رȽϺ���
	class ValueCompare
		:public binary_function<ValueType, ValueType, bool>
	{
		friend class Map<Key, T, Compare, Alloc>; //��Map����ΪValueCompare����Ԫ
	protected:
		Compare comp;
		ValueCompare(Compare c): comp(c)
		{}
	public:
		bool operator()(const ValueType& x, const ValueType& y)const //�º���������()
		{
			return comp(x.first, y.first);
		}
	};

	//map�ĵײ���ú������Ϊ�ײ�����
	typedef RBTree<KeyType, ValueType, KeyCompare, Alloc> repType;
	repType t;   //tΪ�ڲ�RBTree������

public:
	//�����ṩiterator_traits<i>֧��
	typedef typename repType::Pointer Pointer;
	typedef typename repType::ConstPointer ConstPointer;
	typedef typename repType::reference reference;
	typedef typename repType::ConstReference ConstReference;
	typedef typename repType::Iterator Iterator;
	//map������set������iterator����Ϊconst ���ͣ���Ϊ������ͨ����������޸�Ԫ�ص�value
	typedef typename repType::ConstIterator ConstIterator;
	
	//���������
	typedef typename repType::ReverseIterator ReverseIterator;
	typename typename repType::ConstReverseIterator ConstReverseIterator;

	typedef typename repType::SizeType SizeType;
	typedef typename repType::DifferenceType DifferenceType;


	//����ĵ��������������з���ȫ�����ú�����ķ������������
	Iterator begin() 
	{
		return t.begin(); 
	}
	ConstIterator begin() const 
	{
		return t.begin(); 
	}
	
	Iterator end() 
	{
		return t.end();
	}
	ConstIterator end() const 
	{ 
		return t.end();
	}
	ReverseIterator rbegin() 
	{
		return t.rbegin();
	}
	ConstReverseIterator rbegin() const
	{
		return t.rbegin(); 
	}
	ReverseIterator rend() 
	{
		return t.rend();
	}
	ConstReverseIterator rend() const
	{
		return t.rend(); 
	}
	bool empty() const
	{
		return t.empty();
	}
	SizeType size() const
	{
		return t.size(); 
	}
	SizeType MaxSize() const 
	{
		return t.MaxSize(); 
	}

	T& operator[](const KeyType& k)
	{
		return (*((Insert(ValueType(k, T()))).first)).second;
	}

	// ��������key�Ƚϵĺ���
	KeyCompare KeyComp() const
	{
		return t.KeyComp();
	}

	// ����map������, value��keyʹ��ͬһ���ȽϺ���, ʵ�������ǲ���ʹ��value�ȽϺ���
	ValueCompare ValueComp() const
	{
		return ValueCompare(t.KeyComp());
	}

	// ע��: ������һ������������, ������ʵ�key������, ���½���һ��
	T& operator[](const KeyType& k)
	{
		return (*((Insert(ValueType(k, T()))).first)).second;
	}

	//�չ��캯����ֱ�ӵ��ú�����Ŀչ��캯��
	Map()
		:t(Compare)
	{}

	//�������캯��
	explicit Map(const Compare& comp)
		:t(comp)
	{}

	template<class InputIterator>
	Map(InputIterator first, InputIterator last)
		: t(Compare())
	{
		t.InsertUnique(first, last);
	}

	template<class InputIterator>
	Map(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp)
	{
		t.InsertUnique(first, last);
	}

	Map(const Map<Key,T,Compare,Alloc>& x)
		:t(x.t)
	{}

private:
	// ������ͬ��key, ֻ�������һ��, ��bool��ʶ
	pair<Iterator, bool> Insert(const ValueType& x) 
	{ 
		return t.InsertUnique(x);
	}

	// ��position������Ԫ��, ����position�����Ǹ���ʾ, ���������λ�ò��ܽ��в���,
	// STL����в���, ��ᵼ�ºܲ��Ч��
	iterator Insert(Iterator position, const ValueType& x)
	{
		return t.InsertUnique(position, x);
	}
	// ��[first��last)�����ڵ�Ԫ�ز��뵽map��
	template <class Inputiterator>
	void Insert(InputIterator first, InputIterator last)
	{
		t.InsertUnique(first, last);
	}
	
	// ����ָ��λ�õ�Ԫ��, �ᵼ���ڲ��ĺ������������
	void erase(Iterator position) 
	{
		t.erase(position);
	}

	// �᷵�ز���Ԫ�صĸ���, ��ʵ���Ǳ�ʶmap��ԭ���Ƿ���ָ����Ԫ��
	size_type erase(const KeyType& x)
	{
		return t.erase(x); 
	}
	void erase(Iterator first, Iterator last)
	{
		t.erase(first, last);
	}

	//�������map����
	void clear()
	{
		t.clear();
	}

	//���Ҽ�ֵΪkey��Ԫ��
	Iterator find(const KeyType& x)
	{
		return t.Find();
	}
	ConstIterator find(const KeyType& x)const
	{
		return t.Find(x);
	}
	//����ָ��Ԫ�صĸ���
	SizeType count(const KeyType& x)const
	{
		return t.NodeCount(x);
	}

public:
	//���رȽ������==��������Ϊ��Ԫ
	friend bool operator== __STL_NULL_TMPL_ARGS(const Map&, const Map&);
	
	//���رȽ������<��������Ϊ��Ԫ
	friend bool operator< __STL_NULL_TMPL_ARGS(const Map&, const Map&);

	inline bool operator==(const Map<Key, T, Compare, Alloc>& y)
	{
		return x.t == y.t;
	}

	inline bool operator<(const Map<Key, T, Compare, Alloc>& y,const Map<Key,T,Compare,Alloc>)
	{
		return x.t < y.t;
	}

};