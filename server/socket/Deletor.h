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
//ɾ������
enum EDelType
{
	EDelType_None,//��ʼֵ��������
	EDelType_One,//ɾ����������
	EDelType_Ary//ɾ������
};
//��ö�����ռ�ֽ���
template<class T>
size_t GetSize(const T& )
{
	return sizeof(T);
}
//���������ռ�ֽ�����������Ԫ�أ�
template<class T>
size_t GetSize(const list<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//���������ռ�ֽ�����������Ԫ�أ�
template<class T>
size_t GetSize(const vector<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//��ü�����ռ�ֽ�����������Ԫ�أ�
template<class T>
size_t GetSize(const set<T>& t)
{
	return sizeof(t) + t.size() * sizeof(T);
}
//���ӳ�����ռ�ֽ�����������Ԫ�أ�
template<class K, class V>
size_t GetSize(const map<K, V>& t)
{
	return sizeof(t) + t.size() * (sizeof(K) + sizeof(V));
}

//��ɾ�����ͽ���ɾ��
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
//�������ӿ���
class IDeletor
{
public:
	IDeletor():m_bDeled(false), m_nCount(0){}
	virtual ~IDeletor(){}

	//����ֵ��ʾ��ɾ�����ֽ���������������Ļ��ḽ�ӹ����������Ԫ�ص��ֽ���
	virtual size_t Del() = 0;

protected:
	bool m_bDeled;//��ʾ�Ƿ���ɾ�������ڷ�ֹ�ظ�ɾ��
	size_t m_nCount;//��ʾ����Ķ��������0��ʾ����new����0��ʾnew[]
};
//����1��ָ���������
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
	T* m_pT;//�������ָ��
	EDelType m_type;//ɾ������
};
//����2��ָ���������
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
	CDeletor1<T1> m_deletor1;//����1��������
	CDeletor1<T2> m_deletor2;//����2��������
};
//�����������࣬ע�⣡�ڲ���Աָ������������Ŀ������ƶ�ת�ƣ�Ŀǰר����AddQuery
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

