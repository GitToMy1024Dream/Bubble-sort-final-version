#pragma once

#include "Alloc.h"
#include "IteratorTraits.h"
#include "TypeTraits.h"

//�ڵ���ɫ
//��ɫΪ0����ɫΪ1
typedef bool RBTreeColorType;
const RBTreeColorType RED = false;
const RBTreeColorType BLACK = true;

template<class K,class V>
struct RBTreeNode
{	
	typedef RBTreeColorType ColorType; // ��ɫ����
	typedef RBTreeNode* base_ptr;

	base_ptr _pLeft;   //ָ����ڵ�
	base_ptr _pRight;  //ָ���ҽ��
	base_ptr _pParent; //ָ�򸸽ڵ�
	base_ptr color;

	//��Сֵ
	static base_ptr minimum(base_ptr x)
	{
		while (x->_pLeft != NULL) //һֱ�����ߣ��ͻ��ҵ���Сֵ
			x = x->_pLeft;        //���Ƕ���������������
		return x;       
	}

	//���ֵ
	static base_ptr maximum(base_ptr x)
	{
		while (x->_pRight != NULL)  //һֱ�����ߣ��ͻ�ֱ�����ֵ
			x = x->_pRight;         //���Ƕ���������������
		return x;
	}
};

//������ĵ�����
template<class K,class V,class Ref,class Ptr>
class RBTreeIterator
{
public:
	typedef RBTreeIterator<K, V, Ref, Ptr> Self;
	typedef RBTreeNode<K, V> Node;

public:
	RBTreeIterator()
		:_pNode(NULL)
	{}

	RBTreeIterator(Node* pNode)
		:_pNode(pNode)
	{}

	RBTreeIterator(const Self& it)
		:_pNode(it._pNode)
	{}

	Self& operator++()
	{
		_Increment();
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_Increment();
		return temp;
	}
	
	Self& operator--()
	{
		_Decrement();
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_Decrement();
		return temp;
	}

	Ref operator*()
	{
		return _pNode->_key;
	}

	Ptr operator->()
	{
		return &(_pNode->_key);
	}

	bool operator!= (const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}

private:
	/* ����һ���ڵ�(�ȵ�ǰ�ڵ�����С���) */
	void _Increment()
	{
		//�����������ڣ���һ��������������ߵĽ��
		if (_pNode->_pRight != NULL)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft != NULL)
				_pNode = _pNode->_pLeft;
		}
		//������������
		else
		{
			Node* pParent = _pNode->_pParent;
		
			//��pNode�����ҽ�㣬��һֱ���ݣ�ֱ����Ϊ�ҽ��Ϊֹ
			//��ʱ���ڵ�һ������һ���ڵ�
			while (_pNode == pParent->_pRight)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			//��ֹ���ڵ���ͷ���������������
			if (_pNode->_pRight != pParent)
			{
				_pNode = pParent;
			}
		}
	}

	/* ����һ���ڵ�(�ȵ�ǰ�ڵ�С�������) */
	void _Decrement()
	{
		//����ǰ�ڵ�Ϊ��ɫ���Ҹ��ڵ�ĸ��ڵ�����Լ�����һ�������ӽڵ�
		//������һ���������󣬼�pNodeΪͷ���
		if (_pNode->_color == RED && _pNode->_pParent->_pParent == _pNode)
			_pNode = _pNode->_pRight;

		//��������ڣ�����������һֱ�����ұ��ߣ���Ϊ��һ���ڵ�
		else if (_pNode->_pLeft)
		{
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}

		//�Ǹ��ڵ㣬���pNodeΪ��ڵ㣬һֱ���������ߣ�ֱ��
		//pNode��Ϊ��ڵ㣬��ʱ���ڵ�һ������һ���ڵ�
		else
		{
			Node* pParent = _pNode->_pParent;
			while (_pNode == pParent->_pLeft)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			_pNode = pParent;
		}
	}

private:
	Node* _pNode;
};


//RBTree�����ݽṹ
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc = alloc>
class RBTree
{
protected:
	typedef void* voidPointer;  //��ָ��
	typedef RBTreeNode* basePtr; //����ڵ��ָ��
	typedef SimpleAlloc<RBTreeNode, alloc> RBTreeNodeAllocator;//�ڵ�Ŀռ�������
	typedef RBTreeColorType ColorType; //�ڵ���ɫ
public:
	typedef Key KeyType;					//��
	typedef Value ValueType;			    //ֵ
	typedef ValueType* Pointer;				//ֵָ��
	typedef const ValueType* ConstPointer;  //constֵָ��
	typedef ValueType& Reference;			//ֵ����
	typedef const ValueType& ConstReference;//constֵ����
	typedef RBTreeNode* LinkType;           //�ڵ�ָ��
	typedef size_t SizeType;                //size_t����������

protected:
	SizeType NodeCount;       //������Ľ����Ŀ 
	LinkType Header;		  //ͷ��㣬����ڵ㻥Ϊ˫��
	Compare KeyCompare;		  //�Ƚϼ�ֵ��С�ĺ���

public:
	typedef RBTreeIterator<ValueType, Reference, Pointer> Iterator; //���������ĵ�����
protected:

	/* ���ÿռ�����������һ���ڵ� */
	LinkType GetNode()
	{
		return RBTreeNodeAllocator::Allocate();
	}
	/* ���ÿռ��������ͻ�һ���ڵ� */
	void PutNode(LinkType p)
	{
		RBTreeNodeAllocator::Deallocate(p);
	}
	/* ���벢��ʼ���ڵ� */
	LinkType CreateNode(const ValueType& x)
	{
		LinkType tmp = GetNode();   //����һ���ڵ�
		Construct(&tmp->_value, x); //���ù��캯����ʼ���ڵ�
		return tmp;
	}

	/* ��¡�ڵ� */
	LinkType CloneNode(LinkType x)
	{
		LinkType tmp = CreateNode(x->_value); //���벢��ʼ���ڵ�
		tmp->color = x->color;
		tmp->_pLeft = 0;
		tmp->_pRight = 0;
		return tmp;
	}

	/* �ͻ��ڵ� */
	void DestroyNode(LinkType p)
	{
		Destroy(&p->_value);  //���������������ٽڵ�
		PutNode(p);           //�ͷ��ڴ�
	}

protected:
	//������������������ȡ��header�ĳ�Ա

	/* ��ø��ڵ� */
	LinkType& root()const
	{
		return (LinkType&)Header->_pParent;
	}

	/* �������������ߵĽ�� */
	LinkType& leftmost()const
	{
		return (LinkType)Header->_pLeft;
	}

	/* ������������ұߵĽ�� */
	LinkType& rightmost()const
	{
		return (LinkType)Header->_pRight;
	}

	//��������������������ȡ�ýڵ�x�ĳ�Ա

	/* ���ؽڵ�����ӽڵ� */
	static LinkType& left(LinkType x)
	{
		return (LinkType&)(x->_pLeft);
	}

	/* ���ؽڵ�������ӽڵ� */
	static LinkType& right(LinkType x)
	{
		return (LinkType&)(x->_pRight);
	}

	/* ���ؽڵ�ĸ��ڵ� */
	static LinkType& parent(LinkType x)
	{
		return (LinkType&)(x->_pParent);
	}

	/* ���ؽڵ��ֵ */
	static Reference value(LinkType x)
	{
		return x->value;
	}

	/* ���ؽڵ����ɫ */
	static ColorType& color(LinkType x)
	{
		return (ColorType)(x->color);
	}

	/* ������Сֵ�ڵ� */
	static LinkType minimum(LinkType x)
	{
		return (LinkType)RBTreeNode::minimum(x);
	}

	/* �������ֵ�ڵ� */
	static LinkType maximum(LinkType x)
	{
		return (LinkType)RBTreeNode::maximum(x);
	}
private:
	/* ��ʼ������� */
	void Init()
	{
		Header = GetNode(); //��ʼ��Header�ڵ㣬header�ڵ���root�ڵ㻥Ϊ���ڵ�
		color(Header) = RED; //����HeaderΪ��ɫ�ڵ�
		root() = 0;   //root()��ú�����ĸ��ڵ�
		leftmost() = Header; //����header�ڵ��������ָ���Լ�
		rightmost() = Header; //����header�ڵ��������ָ���Լ�
	}
public:
	/* ���캯�� */
	RBTree(const Compare& comp = Compare())
		:NodeCount(0)
		, KeyCompare(comp)
	{
		Init();
	}

	/* �������� */
	~RBTree()
	{
	    //Clear();
		PutNode(Header);
	}

public:
	/* ��ȡ���ڵ��ֵ,��header��Ϊ���ڵ� */
	ValueType rootValue()
	{
		return Value((LinkType)Header->_pParent);
	}

	/* ���رȽϴ�С�ĺ��� */
	Compare keyComp()const
	{
		return keyComp;
	}

	/* ����һ����������ָ����������ߵĽ�� */
	Iterator begin()
	{
		return leftmost;
	}

	/* ����һ����������ָ���������ұߵĽ�� */
	Iterator end() 
	{
		return Header;
	}

	/* �жϺ�����Ƿ�Ϊ�� */
	bool empty()const
	{
		return NodeCount == 0;
	}

	/* �жϺ������С */
	SizeType size()const
	{
		return NodeCount;
	}

	/* �������������� */
	SizeType maxSize()const
	{
		return SizeType(-1);
	}

public:
	//������Ļ�������

	/* ������ֵ���ڵ��ֵ�������ظ������ظ��������Ч
	   ����ֵ��pair����һ��Ԫ����RBTree�ĵ�������ָ��
	   �����ڵ㣬�ڶ���Ԫ�ر�ʾ����ɹ���� */
	std::pair<Iterator,bool> InsertUnique(const Value& v)
	{
		LinkType y = Header; //��y��ͷ���
		LinkType x = root(); //x�Ǻ�����ĸ��ڵ㣬Ĭ�ϳ�ʼֵΪnull
		bool Comp = true; //�Ƚϴ�С�Ĳ���ֵ
		//x!=null��˵�������ҵ��˲����½ڵ��λ��
		while (x != 0)
		{
			y = x; //y������x�ڵ�ĸ��ڵ�
			
			Comp = KeyCompare(KeyOfValue()(v), Key(x));
			//���CompΪtrue��˵��������ֵС�ڽڵ�x��ֵ�����������ߣ���xΪx��������
			//��֮��˵��������ֵ���ڽڵ�x��ֵ�����������ߣ���xΪx��������
			x = Comp ? left(x) : right(x);
		}
		//��ʱyָ������ĸ��ڵ�,��ΪҶ�ӽ��
		Iterator j = Iterator(y); 

		//��������ֵ�ȸ��ڵ��ֵС�����������ӽڵ�
		if (Comp)
		{
			//begin()����leftmost()���������������ߵĽ��
			if (begin() == j)
			{
				//x�ǲ���㣬y�ǲ����ĸ��ڵ㣬v�ǲ����ֵ
				return std::pair<Iterator, bool>(_Insert(x, y, v), true);
			}
			else
			{
				j--; //����j
			}
		}
		//��ֵ������н��ֵ�ظ������Բ���
		if (keyComp(Key(j.node), KeyOfValue()(v)))
		{
			return std::pair<Iterator, bool>(Insert(x, y, v), true);
		}
		//�������һ����˵����ֵһ�������м�ֵ�ظ������ܲ���
		return std::pair<Iterator, bool>(j, false);
	}

	/* ������ֵ������ֵ�����ظ�
	   ���غ�����ĵ��������õ�����ָ���½ڵ� */
	Iterator InsertEqual(const ValueType& v)
	{
		LinkType y = Header;
		LinkType x = root();//�Ӹ��ڵ㿪ʼ
		while (x != 0) //�Ӹ��ڵ�����Ѱ�Һ��ʵĲ����
		{
			y = x;
			//�������ң���С����
			x = keyComp(KeyOfValue()(v), Key(x)) ? left(x) :right(x);
		}
		return Insert(x, y, v);//xΪ����㣬yΪx�ĸ��ڵ㣬vΪ����ļ�ֵ
	}

	/* Ѱ�Һ�������Ƿ��м�ֵΪk�Ľ�� */
	Iterator Find(const ValueType& x)
	{
		LinkType y = Header; //��yΪͷ���
		LinkType x = root(); //��xΪ���ڵ�
		while (x != 0)
		{
			//KeyCompare()�ǱȽϴ�С�ĺ���
			if (!KeyCompare(Key(x), k))//x > k
			{
				y = x;
				x = left(x); //������ֵ������
			}
			else  //x < k
			{
				x = right(x); //����Сֵ������
			}
		}
		Iterator j = Iterator(y);
		return (j == end() || KeyCompare(k, Key(j.node))) ? end() : j;
	}
	
private:
	//�����Ĳ���ִ�г���

	/*  _x:��������
		_y:����ڵ�ĸ��ڵ�
		_v:����ڵ�ļ�ֵ */
	Iterator Insert(basePtr _x, basePtr _y, const ValueType& _v)
	{
		LinkType x = (LinkType)_x; //xָ������
		LinkType y = (LinkType)_y; //yָ������ĸ��ڵ�
		LinkType z;
		
		/* 1.y == Header�������ĸ��ڵ�Ϊheader��ע��Header���ڱ���
		     header����������һ���֣�����header��parentָ����ڵ㣩��
			 y == Header˵�������Ϊ���ڵ�  
		   2.x == 0��˵���������Ҷ�ӽڵ��·���Ҷ�ӽڵ����������
		     ��������Ϊnull����Ҳ������Ҷ�ӽڵ��¹��µĽڵ㣻x != 0
			 ˵��������������ڲ�ĳ���ڵ���  
		   3.KeyCompare(KeyOfValue()(v), Key(y))��������ڵ��ֵ
		     Ҫ�ȸ��ڵ��ֵС����ζ������Ҫ���뵽���ڵ���������ϣ� */

		if (y == Header || x != 0 || KeyCompare(KeyOfValue()(v), Key(y)))
		{
			z = CreateNode(v); //�����½ڵ�
			left(y) = z; //��ڵ��������Ϊz
			
			// y == Header:�����ĸ��ڵ�Ϊheader��˵�����ڵ㻹û�г�ʼ��
			if (y == Header)
			{
				root() = z; //z��Ϊ�˸��ڵ�
				rightmost() = z; //��z�����������ұߵĽ��
			}
			//������ڵ�������������ߵĽ��
			else if (y == leftmost)
			{
				leftmost() = z;//���½ڵ���Ϊ����������ߵĽ��
			}
		}
		else
		{
			z = CreateNode(v); //����һ���½ڵ�
			right(y) = z; //����ڵ㵽���ڵ���ұ�
			
			//������ڵ������������ұߵĽ��
			if (y == rightmost)
			{
				rightmost() = z; //���½ڵ���Ϊ�������ұߵĽ��
			}
		}

		parent(z) = y; //���½ڵ�ĸ��ڵ�Ϊy
		left(z) = 0;   //���½ڵ�����ҽڵ�Ϊnull
		right(z) = 0;

		RBTreeRebalance(z, Header->parent); //�������ṹ��������
		++NodeCount; //���ӽڵ�ĸ���
		return Iterator(z);
	}


	/* ����ƽ������(�ı���ɫ����ת����) 
	   x: �����ڵ�
	   y: ����ڵ�
	   root: ���ڵ� */
	inline void RBTreeRebalance(RBTreeNode* x, RBTreeNode*& root)
	{
		x->color = RED; //�²���Ľ���Ǻ�ɫ��

		//�²���Ľ�㲻�Ǹ��ڵ㲢�Ҹ��ڵ�Ϊ��ɫ
		while (x != root && x->parent->color == RED)
		{
			//���ڵ�Ϊ�游�ڵ�����ӽڵ�
			if (x->parent == x->parent->parent->left)
			{
				RBTreeNode* y = x->parent->parent->right; //��yΪ����ڵ�
				//1.����ڵ������Ϊ��ɫ
				if (y && y->color == RED)
				{
					x->parent->color = BLACK; //���ĸ��ڵ�Ϊ��ɫ
					y->color = bool;          //��������ڵ�Ϊ��ɫ
					x->parent->parent->color = RED; //�����游�ڵ�Ϊ��ɫ
					x = x->parent->parent;
				}
				//2.������ڵ㣬��������ڵ�Ϊ��ɫ
				else
				{
					//3.���½ڵ�Ϊ���ڵ�����ӽڵ�
					if (x == x->parent->right)
					{
						x = x->parent;
						// ����һ�´�while��ʼ�������жϷ�֧�����ǿ��Եó�������ת��������  
						// 1.�����ڵ㲻�Ǹ��ڵ�  
						// 2.�����ڵ�ĸ��ڵ��Ǻ�ɫ  
						// 3.���ڵ����游�ڵ�����ӽڵ�  
						// 4.�����ڵ㲻���ڣ����߲����ڵ�Ϊ��  
						// 5.�����ڵ�Ϊ���ڵ�����ӽڵ� 
						RBTreeRotateLeft(x, root);  //���²���ڵ�Ϊ������ת
					}

					x->parent->color = BLACK; //�޸ĸ��ڵ����ɫΪ��ɫ
					x->parent->parent->color = RED; //�޸��游�ڵ����ɫΪ��ɫ
					RBTreeRotateRight(x->parent->parent, root); //���²���ڵ���游�ڵ�Ϊ������ת
				}
			}
			//���ڵ�Ϊ�游�ڵ�����ӽڵ�
			else
			{
				RBTreeNode* y = x->parent->parent = > left; //��yΪ����ڵ�
				//4.���������Ϊ��ɫ
				if (y && y->color == RED)
				{
					x->parent->color = BLACK;      //���ĸ��ڵ����ɫΪ��ɫ
					y->color = BLACK;              //��������ڵ����ɫΪ��ɫ 
					x->parent->parent->color = RED;//�����游�ڵ����ɫΪ��ɫ
					x = x->parent->parent; //�������ϼ��
				}
				//5.������ڵ㣬������ڵ�Ϊ��ɫ
				else
				{
					//6.����½ڵ�Ϊ���ڵ�����ӽڵ�
					if (x == x->parent->left)
					{
						x = x->parent;

						// ����һ�´�while��ʼ�������жϷ�֧�����ǿ��Եó�������ת��������  
						// 1.�����ڵ㲻�Ǹ��ڵ�  
						// 2.�����ڵ�ĸ��ڵ��Ǻ�ɫ  
						// 3.���ڵ����游�ڵ�����ӽڵ�  
						// 4.�����ڵ㲻���ڣ����߲����ڵ�Ϊ��  
						// 5.�����ڵ�Ϊ���ڵ�����ӽڵ�  

						RBTreeRotateRight(x, root); //���²���ڵ�Ϊ������ת
					} 
					x->parent->color = BLACK;       //���²���ڵ�ĸ��ڵ����Ϊ��ɫ
					x->parent->parent->color = RED; //���²���ڵ���游�ڵ����Ϊ��ɫ
					RBTreeRotateLeft(x->parent->parent, root);//���²���ڵ���游�ڵ�Ϊ���������ת
				}
			}
		}
		root->color = BLACK; //���ڵ���ԶΪ��ɫ
	}

	/* ����ת
	   �½ڵ��Ϊ��ɫ��������봦�ĸ��ڵ�Ϊ��ɫ����
	   Υ��������Ĺ��򣬴�ʱ��Ҫ����ת */

	inline void RBTreeRotateLeft(RBTreeNode* x, RBTreeNode*& root)
	{
		RBTreeNode* y = x->right; // ��yΪ��ת������ӽڵ�
		x->right = y->left;  //���ӶԻ�λ��
		if (y->left != 0)
		{
			y->left->parent = x; //�趨���ڵ�
			y->parent = x->parent;
		}
		//��y���x������
		if (x == root)
			root = y;  //��yΪ���ڵ�
		else if (x == x->parent->left)  //xΪ�丸�ڵ�����ӽڵ�
			x->parent->left = y;        //����ת��ĸ��ڵ�����ӽڵ�Ϊy
		else 
			x->parent->right = y;       //����ת��ĸ��ڵ�����ӽڵ�Ϊy
		y->left = x;                    //�����ӽڵ��������Ϊx
		x->parent = y;                  //�����ӽڵ�Ϊ��ת��ĸ��ڵ�
	}

	/* ����ת
	   �½ڵ��Ϊ��ڵ㣬������봦���ڵ�Ϊ��ɫ����Υ��
	   �˺�����Ĺ��򣬴�ʱ�����������ת */

	inline void RBTreeRotateRight(RBTreeNode* x, RBTreeNode*& root)
	{
		//xΪ��ת��
		RBTreeNode* y = x->left; //yΪ��ת������ӽڵ�
		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;  //�趨���ڵ�
		y->parent = x->parent;

		//��y��ȫȡ��x�ĵ�λ(���뽫x���丸�ڵ�Ĺ�ϵ��ȫ���ܹ���)
		if (x == root)      //xΪ���ڵ�
			root = y;       //��yȡ�����ڵ�
		else if (x == x->parent->right)   //xΪ�丸�ڵ�����ӽڵ�
			x->parent->right = y;
		else                              //xΪ�丸�ڵ�����ӽڵ�
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

};

