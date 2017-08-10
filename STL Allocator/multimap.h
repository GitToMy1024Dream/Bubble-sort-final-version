#pragma once

template<class Key,class Compare = less<Key>,class Alloc = alloc>
class multimap
{
public:
	// ... �����ط���map��ͬ
	// ע��������Щ���������õ���insert_equal��������insert_unique
	template <class Inputiterator>
	multimap(InputIterator first, InputIterator last)
		: t(Compare()) {
		t.InsertEqual(first, last);
	}

	template <class inputiterator>
	multimap(InputIterator first, InputIterator last, const Compare& comp)
		: t(comp) {
		t.InsertEqual(first, last);
	}

	// ����Ԫ��, ע��, �����Ԫ��key�����ظ�
	Iterator Insert(const ValueType& x)
	{
		return t.InsertEqual(x); 
	}

	// ��position������Ԫ��, ����position�����Ǹ���ʾ, ���������λ�ò��ܽ��в���,
	// STL����в���, ��ᵼ�ºܲ��Ч��
	Iterator Insert(Iterator position, const ValueType& x)
	{
		return t.InsertEqual(position, x);
	}
	// ����һ�������ڵ�Ԫ��
	template <class inputiterator>
	void Insert(InputIterator first, InputIterator last)
	{
		t.InsertEqual(first, last);
	}
	// ������map��ͬ
};
