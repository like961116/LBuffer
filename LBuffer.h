#include <malloc.h>
#include <afx.h>

#ifndef __LEC_BUFFER_H__
#define __LEC_BUFFER_H__

class LBuffer
{
private:
	unsigned char* m_pBuf;
	int m_nLen;
	
public:		//构造函数/析构函数
	LBuffer();
	LBuffer(LBuffer& buf);
	LBuffer(int nInitMemSize);	//构造函数，初始化缓冲区大小
	LBuffer& operator=(LBuffer& buf);
	virtual ~LBuffer();
 
public:	//内存处理函数
	void IncreaseMemory(int nSize);	//扩展缓冲区
	void DecreaseMemory(int nSize);	//缩减缓冲区，该功能一般不用
	void SetMemory(int nSize);		//直接设置缓冲区大小
	unsigned char * GetBuffer();	//得到缓冲区首地址
	int GetLenM();					//得到缓冲区大小

public:	//属性/数据 读写函数
	unsigned char GetByte(int index);			//读取一个字节
	void SetByte(int index,unsigned char ch);	//设置一个字节
	unsigned char operator[](int index) const;	//读取一个字节
	unsigned char& operator[](int index);		//设置一个字节
	int GetLen();		//得到缓冲区中的数据长度

public:	//合并数据功能
	LBuffer& Append(unsigned char* buf,int len);//将buf接在当前缓冲区的末尾处
	LBuffer& Append(LBuffer& buf);				//将buf接在当前缓冲区的末尾处
	LBuffer& Append(unsigned char ch);			//将一个字节接在当前缓冲区的末尾处
	LBuffer& Append(wchar_t * p);
	LBuffer& Append(char * p);
	template <class T> LBuffer& Append2(T t)
	{
		return Append((unsigned char*)(&t),sizeof(T));
	}

	LBuffer& operator+=(LBuffer& buf);			//将buf接在当前缓冲区的末尾处
	LBuffer& operator+=(unsigned char ch);		//将一个字节接在当前缓冲区的末尾处
	LBuffer& operator+=(char * p);				//将一个字符串接在当前缓冲区的末尾处
	LBuffer& operator+=(wchar_t * p);	//将一个字符串（unicode）接在当前缓冲区的末尾处
	LBuffer& operator+=(char ch);

public:	//删除数据功能
	LBuffer& Del(int nStartPos,int nLen);	//从nStartPos位置删除nLen个字节
	LBuffer& LDel(int nLen);
	LBuffer& RDel(int nLen);
	void Clear();

//****************	  数据查找功能	 *********************************

//	buf	:	需要查找的数据首地址
//	len	:	需要查找的数据长度，如果没有该参数，则buf以0x00或0x0000结束
//	nStartPos	:	查找开始的位置
//	返回值		:	buf在缓冲区中的位置
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
//	函数名称	:	Swap
//	函数功能	:	将缓冲区 从某一个位置处(pos) 对调
//	pos			:	对调的位置
//	例			:	"12345"，Swap(3)后，变为"45123"
	void Swap(int pos);

	//	=======================================================
	//	函数名称	:	ReadLine
	//	函数功能	:	读取一行数据
	//	ar			:	数据来源
	//	pSep		:	遇到pSep或文件结束，则停止读取
	//	nSepLen		:	pSep的长度
	//	返回值		:	读取的字节数，0表示读取的起始位置就是文件结束
	//	注			:	在读到的数据中包含pSep
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
	名称	:	LCycleBuffer
	功能	:	循环缓冲区
	作者	:	李科
	注		:			
*/
class LCycleBuffer  
{
private:
	unsigned char * m_pBuf;	//缓冲区首地址
	int m_nHead;			//缓冲区中数据的开始位置，从0开始
	int m_nTail;			//缓冲区中数据的结束位置，m_nTail==m_nHead表示缓冲区中没有数据
							//设置头尾指针的好处在于，写入操作和读取分别操作Head和Tail两个不同的指针
							//这样，读写操作甚至可以不用做同步处理，因此能够提高效率

public:
	LCycleBuffer(int size);		//构造函数，初始化缓冲区
	virtual ~LCycleBuffer();	

public:

	//	=================================
	//	函数名称	:	GetBuffer
	//	函数功能	:	获得缓冲区首地址
	unsigned char * GetBuffer();

	//	=================================
	//	函数名称	:	GetByte
	//	函数功能	:	获得缓冲区中指定位置的值
	//	index		:	从m_nHead开始的偏移量，index==0表示取m_pBuf[m_nHead]的值
	unsigned char GetByte(int index);

	//	=================================
	//	函数名称	:	GetLenM
	//	函数功能	:	获取缓冲区大小
	int GetLenM();

	//	=================================
	//	函数名称	:	GetLen
	//	函数功能	:	获得数据长度，该长度的计算公式为(m_nTail+GetLenM()-m_nHead)%GetLenM()
	//	注			:	由于没有做同步处理，因此该函数返回的只是一个大概值，具体数据长度还是要
	//					以m_nTail作为边界
	int GetLen();

	//	=================================
	//	函数名称	:	IsEmpty
	//	函数功能	:	判断缓冲区是否为空
	//	注			:	比GetLen效率高
	BOOL IsEmpty();

public:
	//	=================================
	//	函数名称	:	Write
	//	函数功能	:	向缓冲区中写入数据
	//	buf			:	需要写入的数据
	//	注			:	1）由于没有对读写进行同步，因此在写入数据时，有可能尾指针追上了头指针，
	//					此时在写入操作中也需要调整头指针的位置。如果这时正好也在读取数据，
	//					那么就有可能读到错误的头指针数值。
	//					2）读操作则没有这样的问题，因为如果头指针追上了尾指针，那么读操作就开始等待
	//					而不会修改尾指针
	//					3）解决的思路是：读操作每次都进入临界区，而写操作只当
	//					m_nTail+需要写入的数据长度>m_nHead时才进入临界区
	//					4）不过如果写入的速度>读取的速度，怎么用都不行，临界区也解决不了问题
	//					5）只有当平均读取速度>平均写入速度，而某一个小时间段写入速度>读取速度 时
	//					临界区才有作用，但是所谓循环缓冲区，就是针对这种情况设计的。
	//					再加上临界区，表面上是双保险，实际上是重复设计。
	void Write(LBuffer& buf);
	void Write(unsigned char* p,int len);
	
	//	=======================================================
	//	函数名称	:	Read
	//	函数功能	:	读取指定长度的数据
	//	len			:	需要读取的字节数
	//	buf			:	读取到的一块数据
	void Read(int len,LBuffer& ret);
	void Delete(int len);	//删除开始处的一段数据
	void Clear();	//清空缓冲区

public:
	//	=======================================================
	//	函数名称	:	Find
	//	函数功能	:	查找一段数据
	//	buf			:	需要查找的数据
	//	uc			:	需要查找的数据
	//	len			:	需要查找的数据 的长度
	//	返回值		:	数据起始位置距离头指针的偏移，-1表示没有找到
	//	注			:	考虑到多线程环境，找到数据后，再读取的时候，可能数据已经变了
	int Find(LBuffer& buf);
	int Find(unsigned char* buf,int len);
	int Find(unsigned char uc);

};

#endif