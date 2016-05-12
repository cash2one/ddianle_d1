/*
 * Query.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-9-18 10:15:22
 */

#ifndef __QUERY_H__
#define __QUERY_H__

#include "Deletor.h"
#include "SharedPtr.h"
#include "Windefine.h"

/**
* Query:
* 
*/

class QueryBase
{
protected:
    QueryBase( const QueryBase& ) {}
    QueryBase& operator= ( const QueryBase& );

public:
    QueryBase( int nQueryType, int nAttachment = 0, void* pAttachment = NULL, __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL );
    virtual ~QueryBase();

    // �ͷ��ڲ�ָ���Ա���ڴ�
    size_t ReleaseMemberMemory();

#ifdef _GAMEMSG_MEMPOOL
    void* operator new ( unsigned int nSize );
    void operator delete ( void* pvMem );
#endif

public:
    __int64 m_nSessionID;
    int m_nEntityIndex;
    int m_nRoleID;
    int m_nQueryType;               // ��ѯ����
    int m_nAttachment;              // ��������������
    void* m_pAttachment;            // ������ָ�����
    __int64 m_nTag;                 // ���ӱ��
    void* m_pTag;                   // ����ָ����
    int m_nResult;                  // ��ѯ�����0��ʾ�ɹ�����0��ʾ���ɹ�
    CDelHelper m_delHelper;         // ָ������ڴ��ͷŸ�����
    unsigned int m_nEnqueueTime;    // �����ѯ���е�ʱ��㣬���ڼ��DB�̵߳ĸ��غͶ����Ŷ�״��
};

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Query : public QueryBase
{
protected:
    Query( const Query<T>& ) {}
    Query& operator= ( const Query<T>& );

public:
    Query( int nQueryType, SharedPtr<T>& rSharedAttachment )
        : QueryBase( nQueryType )
        , m_sharedAttachment( rSharedAttachment )
    {
    }
    ~Query() {}

    bool IsValid() const
    {
        return m_sharedAttachment.IsValid();
    }

    T* Get() const
    {
        return m_sharedAttachment.Get();
    }

    T& GetRef() const
    {
        return m_sharedAttachment.GetRef();
    }

    T& operator*() const
    {
        return *m_sharedAttachment;
    }

    T* operator->() const
    {
        return m_sharedAttachment.operator->();
    }

    const SharedPtr<T>& GetSharedPtr() const
    {
        return m_sharedAttachment;
    }

private:
    SharedPtr<T> m_sharedAttachment;
};

#endif // __QUERY_H__

