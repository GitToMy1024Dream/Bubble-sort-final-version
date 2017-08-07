#pragma once
#include <iostream>
#include <stdarg.h>
#include <string.h>
using namespace std;

#pragma warning (disable:4996)

#define __DEBUG__
static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//���ڵ���׷�ݵ�trace_log
inline static void __trace_debug(const char* function, \
	const char* filename, \
	int line, char* format, ...)
{
#ifdef __DEBUG__
	FILE* fout = fopen("trace.log", "a+");
	//������ú�������Ϣ
	fprintf(stdout, "[%s:%d]%s", GetFileName(filename).c_str(), line, function);
	
	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fclose(fout);
#endif 
}
#define __TRACE_DEBUG(...) __trace_debug(__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__);

typedef void(*HandlerFunc)(); //��void(*)()��������HandlerFunc

//һ���ռ�������
template<int inst>   //Ԥ������
class _MallocAllocTemplate
{
public:
	//����size�ֽڵĿռ�
	static void* Allocate(size_t size)
	{
		__TRACE_DEBUG("����һ���ռ�����������[%ubytes]\n", size);

		void *ret = malloc(size); //һ��������ֱ��ʹ��malloc()���ٿռ�
		//��mallocʧ�ܣ������OOM_Malloc
		if (ret == 0)
		{
			ret = OOM_Malloc(size);
		}
		return ret;
	}

	//mallocʧ��ʱ���õĴ�����
	static void* OOM_Malloc(size_t size)
	{
		//һֱ�����ͷš����ã�ֱ��malloc�ɹ�
		for (;;)
		{
			//δ�����ڴ治�㴦����ƣ����׳��쳣
			if(_handler == NULL)   
				throw bad_alloc();
			_handler();  //���ô������̣���ͼ�ͷ��ڴ�
	
			void* ret = malloc(size);
			//���ٳɹ�����������ڴ�ĵ�ַ
			if (ret)
				return ret;
		}
	}

	//ֱ�ӵ���free�ͷſռ�
	static void Deallocate(void* p, size_t n)
	{
		__TRACE_DEBUG("һ���ռ��������ͷ��ڴ�[%p]\n", p);
		free(p);
	}

	//ͨ������������������ļ���������ʵ��set_new_hanlder
	//�����뷵��ֵ���Ǻ���ָ��
	static HandlerFunc SetMallocHandler(HandlerFunc f)
	{
		HandlerFunc old = f;
		_handler = f;   //���ڴ����ʧ�ܵľ������Ϊf���ò���ϵͳȥ�ͷ������Ŀռ�
		return old;
	}

private:
	static HandlerFunc _handler;//����ָ�룬�����ڴ治��ʱ�Ĵ������
};
template<int inst>
HandlerFunc _MallocAllocTemplate<inst>::_handler = 0; //��ʼ����Ĭ�ϲ������ڴ洦�����


//�����ռ�������
template<bool threads,int inst>
class _DefaultAllocTemplate
{
private:
	enum { __ALIGN = 8 }; //�޶���������
	enum { __MAX_BYTES = 128 }; //����ֽ���
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; //��������ĸ���

	//��������(��ֵ��������ȥ���ڴ��)
	union Obj
	{
		union Obj* _freeListLink;//ָ����һ����������ڵ��ָ��
		char client_data[1]; // The client sees this.
	};

	static Obj* _freeList[__NFREELISTS];

	//�����ڴ��
	static char* _startFree; //�ڴ�ص���ʼ��ַ
	static char* _endFree;   //�ڴ�ص�ĩβ��ַ
	static size_t _heapSize; //����ϵͳ������ڴ�Ĵ�С

public:
	//bytes�ֽ������������еĶ�Ӧλ��
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	//��bytes�ֽ�����ȡ8�ı���
	static size_t ROUND_UP(size_t bytes)
	{
		return (bytes + __ALIGN - 1)&(~(__ALIGN - 1));
	}

		//���ڴ����ȡ�ռ������������,ÿ�������СΪsize,����nobjs��
		static char* ChunkAlloc(size_t size, size_t& nobjs)
		{
			__TRACE_DEBUG("���ڴ��������ȡ��[%u]��[%ubytes]����\n", nobjs, size);

			size_t totalBytes = size*nobjs; //�ܹ�������ڴ��С
			size_t leftBytes = _endFree - _startFree;//�ڴ��ʣ��ռ��С

			//1.���㹻nobjs������
			if (leftBytes >= totalBytes)
			{
				__TRACE_DEBUG("�ڴ��ӵ���㹻[%u]��������ڴ�\n", nobjs);
				char *ret = _startFree;
				_startFree += totalBytes;//�����ڴ�ص���ʼ��ַ
				return ret;
			}

			//2.ֻ������[1,size)��nobjs����
			else if (leftBytes > size)
			{
				nobjs = leftBytes / size; //�������Ķ������
				__TRACE_DEBUG("�ڴ��ֻ��[%u]��������ڴ�\n", nobjs);
				totalBytes = size*nobjs;
				char *ret = _startFree;
				_startFree += totalBytes; //�����ڴ�ص���ʼλ��
				return ret;
			}
		
			//3.һ�����󶼲��ܷ���
			else
			{
				__TRACE_DEBUG("�ڴ����һ��[%ubytes]�Ķ���û��\n", size);

				//���������ڴ棬������������Ķ�Ӧλ��
				if (leftBytes > 0)
				{
					size_t index = FREELIST_INDEX(leftBytes);
					((Obj*)_startFree)->_freeListLink = _freeList[index];//ͷ�巨
					_freeList[index] = (Obj*)_startFree;
				}

				size_t newBytes = 2 * totalBytes + ROUND_UP(_heapSize >> 4);
				__TRACE_DEBUG("��ϵͳ����[%ubytes]���ڴ��\n", newBytes);
				_startFree = (char*)malloc(newBytes);//�����¿ռ�
				if (_startFree == NULL) //����ʧ��
				{
					__TRACE_DEBUG("��ϵͳ�����ڴ�ʧ��\n");

					//�ڴ��Ѿ��Խ�����Ҫ���������������ȥȡ
					size_t i = FREELIST_INDEX(size);
					//�������������ҵ�һ��������ڴ��
					for (size_t i = 0; i < __NFREELISTS; ++i)
					{
						if (_freeList[i])
						{
							//���µ���free_list
							Obj* ret = _freeList[i];
							_freeList[i] = ret->_freeListLink;
							_startFree = (char*)ret;
							_endFree = _startFree + (i + 1)*__ALIGN;
							return ChunkAlloc(size, nobjs); //���ٳɹ����ٴε���ChunkAllc�����ڴ�
						}
					}

					//free_listû�����ڴ��ʱ������һ��������
					_startFree = (char*)_MallocAllocTemplate<0>::Allocate(newBytes);

				}

				//��ϵͳ�����ڴ�ɹ�
				//���µ���_heapSize��_endFree
				_heapSize += newBytes;
				_endFree  = _startFree + newBytes;
				return ChunkAlloc(size, nobjs);//�ݹ����ChunkAlloc��Ϊ������nobjs

			}
		}

	//Ϊ���������������ռ�
	static char* Refill(size_t n)
	{
		size_t nobjs = 20;  //ȱʡȡ��20��������
		char* chunk = ChunkAlloc(n, nobjs);//����ChunkAlloc�����ڴ����

		__TRACE_DEBUG("���ڴ��������[%u]���Ķ���,\
һ�������û�ʹ�ã�ʣ�¹���������������\n", nobjs);
		
		//ֻ���뵽һ���ڴ��ʱ��ֱ�ӷ��ظ��û�ʹ�ã�free_list���½ڵ�
		if (nobjs == 1)
			return chunk;

		//��ʣ���nobjs-1���ڴ������������Ķ�Ӧλ������
		size_t index = FREELIST_INDEX(n);
		_freeList[index] = (Obj*)(chunk + n);//���ڶ�������ĵ�ַ����free_list��
		Obj* cur = _freeList[index];

		//��ʣ�µĿ����ι���������������
		for (size_t i = 2; i < nobjs; ++i)
		{
			Obj* next = (Obj*)((char*)cur + n);
			cur->_freeListLink = next;
			cur = next;
		}
		cur->_freeListLink = NULL;//free_list�����һ���ڴ��ָ��NULL

		return chunk;//���ص�һ���ڴ��
	}

	//����nbytes�Ŀռ�
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("���ö����ռ�����������[%ubytes]\n", n);
		
		//����128�ֽڵ���һ���ռ�������
		if (n > (size_t)__MAX_BYTES)
		{
			return _MallocAllocTemplate<0>::Allocate(n);
		}

		//Ѱ��16��free_list���ʵ���һ��
		size_t index = FREELIST_INDEX(n);
		//free_list�д��ڶ�Ӧ���ڴ��ʱ������һ�鷵�ظ��û�
		if (_freeList[index])
		{
			__TRACE_DEBUG("�����������[%d]��λ��ȡһ���ڴ��\n", index);
			Obj* ret = _freeList[index];
			_freeList[index] = ret->_freeListLink;//ָ��ڶ����ڴ��
			return ret;
		}
		else
		{
			return Refill(ROUND_UP(n)); //����Refill()���ڴ��������ռ������free_list
		}

	}


	//�ͷſռ�
	static void Deallocate(void *ptr, size_t n)
	{
		//����128�ֽڵ���һ���ռ��������ͷ�
		if (n > __MAX_BYTES)
		{
			_MallocAllocTemplate<0>::Deallocate(ptr, n);
		}
		else
		{
			//���ͷŵ��ڴ��ҽ�����������Ķ�Ӧλ��
			size_t index = FREELIST_INDEX(n);

			__TRACE_DEBUG("���ͷŵ�[%ubytes]�Ķ������free_list[%u]��\n", n, index);
			((Obj*)ptr)->_freeListLink = _freeList[index];//ͷ�巨
			_freeList[index] = (Obj*)ptr; //����freelist��ָ��
		}
	}

};


//��ʼ������
template<bool threads,int inst>
typename _DefaultAllocTemplate<threads,inst>::Obj*
_DefaultAllocTemplate<threads, inst>::_freeList[__NFREELISTS] = { NULL };

template <bool threads, int inst>
char* _DefaultAllocTemplate<threads, inst>::_startFree = NULL;

template <bool threads, int inst>
char* _DefaultAllocTemplate<threads, inst>::_endFree = NULL;

template <bool threads, int inst>
size_t _DefaultAllocTemplate<threads, inst>::_heapSize = 0;

#ifdef __USE_MALLOC
typedef _MallocAllocTemplate<0> alloc;
#else
typedef _DefaultAllocTemplate<true, 0> alloc;
#endif 


//��������׼�ӿ�
template<class T,class Alloc>
class SimpleAlloc
{
public:
	static T* Allocate(size_t n)
	{
		return n == 0 ? 0 : (T*)Alloc::Allocate(n * sizeof(T));
	}
	
	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof(T));
	}

	static void Deallocate(T* p, size_t n)
	{
		if (n != 0)
		{
			Alloc::Deallocate(p, n * sizeof(T));
		}
	}

	static void Deallocate(T* p)
	{
		Alloc::Deallocate(p, sizeof(T));
	}
};








