#include <malloc.h>
#include <afx.h>

#ifndef __LEC_BUFFER_H__
#define __LEC_BUFFER_H__

class LBuffer
{
private:
	unsigned char* m_pBuf;
	int m_nLen;
	
public:		//���캯��/��������
	LBuffer();
	LBuffer(LBuffer& buf);
	LBuffer(int nInitMemSize);	//���캯������ʼ����������С
	LBuffer& operator=(LBuffer& buf);
	virtual ~LBuffer();
 
public:	//�ڴ洦����
	void IncreaseMemory(int nSize);	//��չ������
	void DecreaseMemory(int nSize);	//�������������ù���һ�㲻��
	void SetMemory(int nSize);		//ֱ�����û�������С
	unsigned char * GetBuffer();	//�õ��������׵�ַ
	int GetLenM();					//�õ���������С

public:	//����/���� ��д����
	unsigned char GetByte(int index);			//��ȡһ���ֽ�
	void SetByte(int index,unsigned char ch);	//����һ���ֽ�
	unsigned char operator[](int index) const;	//��ȡһ���ֽ�
	unsigned char& operator[](int index);		//����һ���ֽ�
	int GetLen();		//�õ��������е����ݳ���

public:	//�ϲ����ݹ���
	LBuffer& Append(unsigned char* buf,int len);//��buf���ڵ�ǰ��������ĩβ��
	LBuffer& Append(LBuffer& buf);				//��buf���ڵ�ǰ��������ĩβ��
	LBuffer& Append(unsigned char ch);			//��һ���ֽڽ��ڵ�ǰ��������ĩβ��
	LBuffer& Append(wchar_t * p);
	LBuffer& Append(char * p);
	template <class T> LBuffer& Append2(T t)
	{
		return Append((unsigned char*)(&t),sizeof(T));
	}

	LBuffer& operator+=(LBuffer& buf);			//��buf���ڵ�ǰ��������ĩβ��
	LBuffer& operator+=(unsigned char ch);		//��һ���ֽڽ��ڵ�ǰ��������ĩβ��
	LBuffer& operator+=(char * p);				//��һ���ַ������ڵ�ǰ��������ĩβ��
	LBuffer& operator+=(wchar_t * p);	//��һ���ַ�����unicode�����ڵ�ǰ��������ĩβ��
	LBuffer& operator+=(char ch);

public:	//ɾ�����ݹ���
	LBuffer& Del(int nStartPos,int nLen);	//��nStartPosλ��ɾ��nLen���ֽ�
	LBuffer& LDel(int nLen);
	LBuffer& RDel(int nLen);
	void Clear();

//****************	  ���ݲ��ҹ���	 *********************************

//	buf	:	��Ҫ���ҵ������׵�ַ
//	len	:	��Ҫ���ҵ����ݳ��ȣ����û�иò�������buf��0x00��0x0000����
//	nStartPos	:	���ҿ�ʼ��λ��
//	����ֵ		:	buf�ڻ������е�λ��
public:	
	int Find(LBuffer& buf,int nStartPos);
	int Find(unsigned char * buf,int len,int nStartPos);
	int Find(char * buf,int nStartPos);
	int Find(wchar_t * buf,int nStartPos);

	int FindReverse(LBuffer& buf,int nStartPos);
	int FindReverse(unsigned char * buf,int len,int nStartPos);
	int FindReverse(char * buf,int nStartPos);
	int FindReverse(wchar_t * buf,int nStartPos);


//	==============================
//	��������	:	Swap
//	��������	:	�������� ��ĳһ��λ�ô�(pos) �Ե�
//	pos			:	�Ե���λ��
//	��			:	"12345"��Swap(3)�󣬱�Ϊ"45123"
	void Swap(int pos);

	//	=======================================================
	//	��������	:	ReadLine
	//	��������	:	��ȡһ������
	//	ar			:	������Դ
	//	pSep		:	����pSep���ļ���������ֹͣ��ȡ
	//	nSepLen		:	pSep�ĳ���
	//	����ֵ		:	��ȡ���ֽ�����0��ʾ��ȡ����ʼλ�þ����ļ�����
	//	ע			:	�ڶ����������а���pSep
	int ReadLine(CArchive& ar,unsigned char* pSep,int nSepLen);

	int Load(CArchive& ar);
	void Save(CArchive& ar);

	template <class T> T Retrieve2(int pos)
	{
		T t;
		memmove(&t,GetBuffer()+pos,sizeof(T));
		return t;
	}
};


/*
	����	:	LCycleBuffer
	����	:	ѭ��������
	����	:	���
	ע		:			
*/
class LCycleBuffer  
{
private:
	unsigned char * m_pBuf;	//�������׵�ַ
	int m_nHead;			//�����������ݵĿ�ʼλ�ã���0��ʼ
	int m_nTail;			//�����������ݵĽ���λ�ã�m_nTail==m_nHead��ʾ��������û������
							//����ͷβָ��ĺô����ڣ�д������Ͷ�ȡ�ֱ����Head��Tail������ͬ��ָ��
							//��������д�����������Բ�����ͬ����������ܹ����Ч��

public:
	LCycleBuffer(int size);		//���캯������ʼ��������
	virtual ~LCycleBuffer();	

public:

	//	=================================
	//	��������	:	GetBuffer
	//	��������	:	��û������׵�ַ
	unsigned char * GetBuffer();

	//	=================================
	//	��������	:	GetByte
	//	��������	:	��û�������ָ��λ�õ�ֵ
	//	index		:	��m_nHead��ʼ��ƫ������index==0��ʾȡm_pBuf[m_nHead]��ֵ
	unsigned char GetByte(int index);

	//	=================================
	//	��������	:	GetLenM
	//	��������	:	��ȡ��������С
	int GetLenM();

	//	=================================
	//	��������	:	GetLen
	//	��������	:	������ݳ��ȣ��ó��ȵļ��㹫ʽΪ(m_nTail+GetLenM()-m_nHead)%GetLenM()
	//	ע			:	����û����ͬ��������˸ú������ص�ֻ��һ�����ֵ���������ݳ��Ȼ���Ҫ
	//					��m_nTail��Ϊ�߽�
	int GetLen();

	//	=================================
	//	��������	:	IsEmpty
	//	��������	:	�жϻ������Ƿ�Ϊ��
	//	ע			:	��GetLenЧ�ʸ�
	BOOL IsEmpty();

public:
	//	=================================
	//	��������	:	Write
	//	��������	:	�򻺳�����д������
	//	buf			:	��Ҫд�������
	//	ע			:	1������û�жԶ�д����ͬ���������д������ʱ���п���βָ��׷����ͷָ�룬
	//					��ʱ��д�������Ҳ��Ҫ����ͷָ���λ�á������ʱ����Ҳ�ڶ�ȡ���ݣ�
	//					��ô���п��ܶ��������ͷָ����ֵ��
	//					2����������û�����������⣬��Ϊ���ͷָ��׷����βָ�룬��ô�������Ϳ�ʼ�ȴ�
	//					�������޸�βָ��
	//					3�������˼·�ǣ�������ÿ�ζ������ٽ�������д����ֻ��
	//					m_nTail+��Ҫд������ݳ���>m_nHeadʱ�Ž����ٽ���
	//					4���������д����ٶ�>��ȡ���ٶȣ���ô�ö����У��ٽ���Ҳ�����������
	//					5��ֻ�е�ƽ����ȡ�ٶ�>ƽ��д���ٶȣ���ĳһ��Сʱ���д���ٶ�>��ȡ�ٶ� ʱ
	//					�ٽ����������ã�������νѭ��������������������������Ƶġ�
	//					�ټ����ٽ�������������˫���գ�ʵ�������ظ���ơ�
	void Write(LBuffer& buf);
	void Write(unsigned char* p,int len);
	
	//	=======================================================
	//	��������	:	Read
	//	��������	:	��ȡָ�����ȵ�����
	//	len			:	��Ҫ��ȡ���ֽ���
	//	buf			:	��ȡ����һ������
	void Read(int len,LBuffer& ret);
	void Delete(int len);	//ɾ����ʼ����һ������
	void Clear();	//��ջ�����

public:
	//	=======================================================
	//	��������	:	Find
	//	��������	:	����һ������
	//	buf			:	��Ҫ���ҵ�����
	//	uc			:	��Ҫ���ҵ�����
	//	len			:	��Ҫ���ҵ����� �ĳ���
	//	����ֵ		:	������ʼλ�þ���ͷָ���ƫ�ƣ�-1��ʾû���ҵ�
	//	ע			:	���ǵ����̻߳������ҵ����ݺ��ٶ�ȡ��ʱ�򣬿��������Ѿ�����
	int Find(LBuffer& buf);
	int Find(unsigned char* buf,int len);
	int Find(unsigned char uc);

};

#endif