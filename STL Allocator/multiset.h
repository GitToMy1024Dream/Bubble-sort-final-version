#pragma once

#include "Alloc.h"
#include "RBTree.h"
#include "Construct.h"
#include "IteratorTraits.h"

template<class Key,class Compare = less<Key>,class Alloc = alloc>
class multiset
{
public:
	//multisetһ����Ҫinsert_equal()������ʹ��insert_unique()

	template <class inputiterator>
	multiset(InputIterator first, InputIterator last)
		: t(Compare()) 
	{
		t.InsertEqual(first, last);
	}
	template <class inputiterator>
	multiset(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp) 
	{
		t.InsertEqual(first, last);
	}

	// ����Ԫ��, ע��, �����Ԫ��key�����ظ�
	Iterator Insert(const value_type& x)
	{
		return t.InsertEqual(x);
	}

	// ��position������Ԫ��, ����position�����Ǹ���ʾ, ���������λ�ò��ܽ��в���,
	// STL����в���, ��ᵼ�ºܲ��Ч��
	Iterator Insert(Iterator position, const ValueType& x)
	{
		typedef typename repType::Iterator rep_iterator;
		return t.InsertEqual((rep_iterator&)position, x);
	}

	//������set��ͬ
};