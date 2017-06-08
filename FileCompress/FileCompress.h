#pragma once
#include <iostream>
#include <string.h>
#include <assert.h>
#include "heap.h"
#include "huffmanTree.h"
#pragma warning (disable:4996)
using namespace std;
struct FileInfo//�ļ���Ϣ
{
	char _ch;//�ַ�
	long long _apperCount;//�ַ����ֵĴ���
	string _strCode;//����
	FileInfo(long long apearCount = 0)
		:_apperCount(apearCount)
	{}
	bool operator!=(const FileInfo& _fileCount)const
	{
		return _apperCount != _fileCount._apperCount;
	}
	bool operator<(const FileInfo& _fileCount)const
	{
		return _apperCount < _fileCount._apperCount;
	}
	size_t operator+(const FileInfo& _fileCount)
	{
		return _apperCount + _fileCount._apperCount;
	}
};
class FileCompress
{
public:
	FileCompress()
	{
		//��ʼ��
		for (size_t idx = 0; idx < 256; idx++)
		{
			_fileInfo[idx]._ch = idx;
		}
	}
	//ѹ���ļ�
	void CompressFile(const string _strFilePath)//��ȡ�ļ�·��
	{
		FILE* fIn = fopen(_strFilePath.c_str(), "rb");//fopen��const char*���ͣ���ȡһ��C���͵��ַ���string::c_str
		assert(fIn != NULL);
		char* pReadBuff = new char[1024];//����һ�������������ÿ�ζ�ȡ���ַ�
		//һ�ζ�ȡ1024���ַ�
		while (true)
		{
			size_t readSize = fread(pReadBuff, 1, 1024, fIn);//�ɹ���ȡ���ַ�
			if (readSize == 0)
				break;
			for (size_t idx = 0; idx < readSize; ++idx)
			{
				//��ͳ���ַ����ֵĴ���
				_fileInfo[pReadBuff[idx]]._apperCount++;
			}
		}
		//��ͨ����������huffman��
		FileInfo invaild(0);
		HuffmanTree<FileInfo> _hf(_fileInfo, sizeof(_fileInfo)/sizeof(_fileInfo[0]), invaild);

		//�ۻ�ȡ����������
		_GenerateHuffmanCode(_hf.Root());
		FILE* fOut = fopen("1.hzp", "wb");

		//����ѹ����Ϣ(�����ѹ��)
		 //1.����ѹ����Ϣ
		//1)��׺
		string strFileHeadInfo(GetFilePostFix(_strFilePath));//ͷ����Ϣ
		strFileHeadInfo += '\n';
		string strCodeInfo;//�ַ���Ϣ
		size_t lineCount = 0;//�к�
		char szappearCount[32];//�ַ����ֵ�����;
		//2)�����ַ����ֵĴ���
		for (size_t idx = 0; idx < 256; ++idx)
		{
			if (_fileInfo[idx]._apperCount != 0)
			{
				strCodeInfo += _fileInfo[idx]._ch;//���浱ǰ�ַ�
				strCodeInfo += ',';//�ַ������ã��ָ�
				memset(szappearCount, 0, 32);
				itoa(_fileInfo[idx]._apperCount, szappearCount, 10);//�ַ����ֵĴ���
				strCodeInfo += szappearCount;//ÿһ�е��ַ�����
				strCodeInfo += '\n';
				lineCount++;
			}
		}
		itoa(lineCount, szappearCount, 10);//ת����ȥ
		strFileHeadInfo += szappearCount;//���ص�ͷ����Ϣ�ĺ���
		strFileHeadInfo += '\n';
		strFileHeadInfo += strCodeInfo;//��������Ϣ���ص�����
		fwrite(strFileHeadInfo.c_str(), 1, strFileHeadInfo.length(), fOut);
		//fwrite(strCodeInfo.c_str(), 1, strCodeInfo.length(), fOut);
		
		fseek(fIn,0,SEEK_SET);//���ļ�������ʼλ��
		char value = 0;
		int pos = 1;
		char *Wbuff = new char[1024];
		size_t index = 0;
		assert(fOut);
		//�����¸�д
		while (true)
		{
			size_t readSize = fread(pReadBuff, 1, 1024, fIn);
			if (readSize == 0)
				break;
			for (size_t idx = 0; idx < readSize; ++idx)
			{
				string& strCode = _fileInfo[pReadBuff[idx]]._strCode;
				int i = 0;
				while (i < strCode.size())
				{
					value <<= 1;
					if (strCode[i] == '1')
						value |= 1;
					i++;
					if (8 == pos)
					{
						pos = 1;
						Wbuff[index++] = value;
						if (index == 1024)
						{
							fwrite(Wbuff, 1, 1024, fOut);
							index = 0;
						}
						value = 0;
					}
					else
						pos++;
				}
			}
		}
		//�������һ���ֽڣ�����0000 0011��ֻ����λʱ����11���ڸ�λ
		if (pos < 8)
		{
			value <<= (8 - pos + 1);//���һ���ַ���λ��δ����
			Wbuff[index++] = value;
			fwrite(Wbuff, 1, index, fOut);
		}
		fclose(fOut);
		fclose(fIn);
		delete[] Wbuff;
		delete[] pReadBuff;
	}
	//��ѹ���ļ�
	void UnCompressFile(const string _strFilePath)
	{
		//1.��ȡѹ����Ϣ
		FILE* fIn = fopen(_strFilePath.c_str(), "rb");
		assert(fIn);
		//��ȡ�ļ��ĺ�׺
		string strFilePostFix;
		GetLine(fIn, strFilePostFix);
		//��Ч�ַ�������
		string strLineNode;
		GetLine(fIn, strLineNode);
		size_t lineCount = atoi(strLineNode.c_str());
		string strCodeInfo;
		for (size_t idx = 0; idx < lineCount; ++idx)
		{
			GetLine(fIn, strCodeInfo);
			_fileInfo[strCodeInfo[0]]._apperCount = atoi(strCodeInfo.substr(2,strCodeInfo.length()-2).c_str());
			strCodeInfo = "";
		}
		//����huffman��
		HuffmanTree<FileInfo> ht(_fileInfo, sizeof(_fileInfo) / sizeof(_fileInfo[0]), FileInfo(0));
		HuffmanTreeNode<FileInfo>* pCur = ht.Root();
		//д���µ��ļ�
		FILE* fOut = fopen("2.txt", "wb");
		assert(fOut);
		int pos = 7;
		size_t writeSize = 0;
		char *RdBuff = new char[1024];//��
		char *WrBuff = new char[1024];//д
		long long fileSize = pCur->_weight._apperCount;//Դ�ļ���С
		//��ȡ������Ϣ
		while (true)
		{
			size_t readSize = fread(RdBuff, 1, 1024, fIn);//һ�ζ�ȡ1���ֽ�
			if (0 == readSize)
				break;
			for (size_t idx = 0; idx < 1024;)
			{
				//���Ըñ���λ��0����1
				if (RdBuff[idx] & (1 << pos))
				{
					pCur = pCur->_pRight;
				}
				else
					pCur = pCur->_pLeft;
				if (pCur->_pLeft == NULL && pCur->_pRight == NULL)//Ҷ�ӽڵ�
				{
					//�����ַ�
					WrBuff[writeSize++] = pCur->_weight._ch;
					if (writeSize == 1024)
					{
						fwrite(WrBuff, 1, 1024, fOut);
						writeSize = 0;
					}
					fileSize--;
					if (fileSize == 0)
						break;
					pCur = ht.Root();//�ص�����λ��
				}
				--pos;
				if (pos < 0)
				{
					pos = 7;
					idx++;
				}
			}
		}
		if (writeSize)
		{
			fwrite(WrBuff, 1, writeSize, fOut);
		}
		delete[] RdBuff;
		delete[] WrBuff;
		fclose(fIn);
		fclose(fOut);
	}
	//��ȡһ�е���Ϣ
	void GetLine(FILE* pf, string& strRet)
	{
		//feof�ж��Ƿ񵽴��ļ���β
		if (feof(pf))
		{
			return;//�ļ���β
		}
		char c = fgetc(pf);//��ȡһ���ַ�
		//���Ƿ���\n
		while (c != '\n')
		{
			strRet += c;
			if (feof(pf))
			{
				break;//�ļ���β
			}
			c = fgetc(pf);
		}
	}
private:
	//��ȡ����������,�ҵ�Ҷ�ӽڵ�
	void  _GenerateHuffmanCode(HuffmanTreeNode<FileInfo>* pRoot)
	{
		if (pRoot)
		{
			_GenerateHuffmanCode(pRoot->_pLeft);
			_GenerateHuffmanCode(pRoot->_pRight);
			if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
			{
				HuffmanTreeNode<FileInfo>* pCur = pRoot;
				HuffmanTreeNode<FileInfo>* pParent = pCur->_pParent;
				//ͨ���ַ�ch��ȡ��fileinfo�����е�λ��
				string& _strCode = _fileInfo[pRoot->_weight._ch]._strCode;
				//�Ե����ϻ�ȡ����(�����Ƿ���)
				while(pParent)
				{
					if (pCur == pParent->_pLeft)
						_strCode += '0';
					else
						_strCode += '1';
					pCur = pParent;
					pParent = pParent->_pParent;
				}
				//���ñ���
				reverse(_strCode.begin(), _strCode.end());
			}
		}
	}
	string GetFilePostFix(const string& FilePath)
	{
		size_t pos = FilePath.find_last_of('.');//Ѱ�Һ�׺�ġ�
		return FilePath.substr(pos, FilePath.length() - pos);//��ȡ�ַ����ĺ�׺
	}
public:
	FileInfo _fileInfo[256];//��Ϊ�ַ���256��״̬
};



