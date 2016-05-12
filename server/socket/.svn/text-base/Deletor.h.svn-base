#ifndef __DELETOR_H__
#define __DELETOR_H__

#include <list>
#include <vector>
#include <set>
#include <map>
#include <stdlib.h>

using namespace std;

#ifdef WIN32
#pragma warning( disable: 4127 )
#endif // WIN32

#define SafeDel(p) do{if(p!=NULL){delete p;p=NULL;}}while(0)
#define SafeDelAry(p) do{if(p!=NULL){delete [] p;p=NULL;}}while(0)
//删除类型
enum EDelType
{
	EDelType_None,//初始值，无意义
	EDelType_One,//删除单个对象
	EDelType_Ary//删除数组
};
//获得对象所占字节数
template<class T>
size_t GetSize(const T& )
{
	return sizeof(T);
}
//获得链表所占字节数（含所有元素）
template<class T>
size_t GetSize(const list<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//获得向量所占字节数（含所有元素）
template<class T>
size_t GetSize(const vector<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//获得集合所占字节数（含所有元素）
template<class T>
size_t GetSize(const set<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//获得映射表所占字节数（含所有元素）
template<class K, class V>
size_t GetSize(const map<K, V>& t)
{
	return sizeof(t) + t.size() * (sizeof(K) + sizeof(V));
}

//按删除类型进行删除
template <typename T>
void DelByType(EDelType type, T*& p)
{
	if ( type == EDelType_One )
	{
		SafeDel(p);
	}
	else if ( type == EDelType_Ary )
	{
		SafeDelAry(p);
	}
}
//析构器接口类
class IDeletor
{
public:
	IDeletor():m_bDeled(false), m_nCount(0){}
	virtual ~IDeletor(){}

	//返回值表示被删除的字节数，如果是容器的话会附加估算出的所有元素的字节数
	virtual size_t Del() = 0;

protected:
	bool m_bDeled;//表示是否已删除，用于防止重复删除
	size_t m_nCount;//表示分配的对象个数，0表示单个new，非0表示new[]
};
//管理1个指针的析构器
template <typename T>
class CDeletor1 : public IDeletor
{
public:
	CDeletor1(T* pT, size_t nCount=0):m_pT(pT)
	{
		m_bDeled = false;
		m_type = EDelType_None;
		if ( pT != NULL )
		{
			if ( nCount == 0 )
			{
				nCount = 1;
				m_type = EDelType_One;
			}
			else
			{
				m_type = EDelType_Ary;
			}
		}
		else
		{
			nCount = 0;
		}
		m_nCount = nCount;
	}
	~CDeletor1(){}
	size_t Del()
	{
		if ( !m_bDeled && m_nCount > 0 )
		{
			size_t nSize = GetSize(*m_pT) * m_nCount;
			DelByType(m_type, m_pT);
			m_type = EDelType_None;
			m_nCount = 0;
			m_bDeled = true;
			return nSize;
		}
		return 0;
	}
private:
	T* m_pT;//被管理的指针
	EDelType m_type;//删除类型
};
//管理2个指针的析构器
template <typename T1, typename T2>
class CDeletor2 : public IDeletor
{
public:
	CDeletor2(T1* pT1, T2* pT2, size_t nCount1=0, size_t nCount2=0):m_deletor1(pT1, nCount1), m_deletor2(pT2, nCount2)
	{
		m_bDeled = false;
	}
	~CDeletor2(){}
	size_t Del()
	{
		if ( !m_bDeled )
		{
			size_t nSize = 0;
			nSize += m_deletor1.Del();
			nSize += m_deletor2.Del();
			m_bDeled = true;
			return nSize;
		}
		return 0;
	}
private:
	CDeletor1<T1> m_deletor1;//类型1的析构器
	CDeletor1<T2> m_deletor2;//类型2的析构器
};
//析构器辅助类，注意！内部成员指针会随着类对象的拷贝复制而转移，目前专用于AddQuery
class CDelHelper
{
public:
	CDelHelper():m_pDeletor(NULL){}
	~CDelHelper(){SafeDel(m_pDeletor);}
	template <typename T>
	CDelHelper(T* pT, size_t nCount=0){m_pDeletor = new CDeletor1<T>(pT, nCount);}
	template <typename T1, typename T2>
	CDelHelper(T1* pT1, T2* pT2, size_t nCount1=0, size_t nCount2=0){m_pDeletor = new CDeletor2<T1, T2>(pT1, pT2, nCount1, nCount2);}
	CDelHelper(CDelHelper& rhs)
	{
		m_pDeletor = rhs.m_pDeletor;
		rhs.m_pDeletor = NULL;
	}
	CDelHelper& operator = (CDelHelper& rhs)
	{
		if ( &rhs != this )
		{
			m_pDeletor = rhs.m_pDeletor;
			rhs.m_pDeletor = NULL;
		}
		return *this;
	}

	size_t Del()
	{
		if ( m_pDeletor != NULL )
		{
			return m_pDeletor->Del();
		}
		return 0;
	}

private:
	IDeletor* m_pDeletor;
};
#endif

