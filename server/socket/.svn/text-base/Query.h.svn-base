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

    // 释放内部指针成员的内存
    size_t ReleaseMemberMemory();

#ifdef _GAMEMSG_MEMPOOL
    void* operator new ( unsigned int nSize );
    void operator delete ( void* pvMem );
#endif

public:
    __int64 m_nSessionID;
    int m_nEntityIndex;
    int m_nRoleID;
    int m_nQueryType;               // 查询类型
    int m_nAttachment;              // 附带的整数参数
    void* m_pAttachment;            // 附带的指针参数
    __int64 m_nTag;                 // 附加标记
    void* m_pTag;                   // 附加指针标记
    int m_nResult;                  // 查询结果，0表示成功，非0表示不成功
    CDelHelper m_delHelper;         // 指针参数内存释放辅助类
    unsigned int m_nEnqueueTime;    // 进入查询队列的时间点，用于监测DB线程的负载和队列排队状况
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

