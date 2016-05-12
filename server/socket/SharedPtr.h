/*
 * SharedPtr.h
 * Description: 智能指针，参考 C++11 (VS2010.NET memory文件) 实现。
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-9-14 14:59:08
 */

#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <assert.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
// 引用计数基类，管理引用的指针和引用计数，线程安全
class RefCountBase
{
protected:
    RefCountBase()
        : m_nCount(1)
    {
        InitLock();
    }

public:
    virtual ~RefCountBase()
    {
        DeleteLock();
    }

    bool IsValid() const
    {
        return m_nCount != 0;
    }

    void IncreaseRef()
    {
        Lock();
        ++m_nCount;
        Unlock();
    }

    void DecreaseRef()
    {
        Lock();
        unsigned int nCount = --m_nCount;
        Unlock();

        if ( nCount == 0 )
            DeleteSelf();
    }

protected:
    virtual void DeleteSelf() = 0;

private:
    void InitLock()
    {
#ifdef WIN32
        InitializeCriticalSection( &m_lock );
#else
        pthread_mutex_init( &m_lock, NULL );
#endif
    }

    void DeleteLock()
    {
#ifdef WIN32
        DeleteCriticalSection( &m_lock );
#else
        pthread_mutex_destroy( &m_lock );
#endif
    }

    void Lock()
    {
#ifdef WIN32
        EnterCriticalSection( &m_lock );
#else
        pthread_mutex_lock( &m_lock );
#endif
    }

    void Unlock()
    {
#ifdef WIN32
        LeaveCriticalSection( &m_lock );
#else
        pthread_mutex_unlock( &m_lock );
#endif
    }

private:
    unsigned int m_nCount;
#ifdef WIN32
    CRITICAL_SECTION m_lock;
#else
    pthread_mutex_t m_lock;
#endif
};

//////////////////////////////////////////////////////////////////////////
// 普通指针的引用计数类
template <typename T>
class RefCount : public RefCountBase
{
public:
    RefCount( T* ptr )
        : m_ptr( ptr )
    {
    }

protected:
    virtual void DeleteSelf()
    {
        delete m_ptr, m_ptr = NULL;
        delete this;
    }

private:
    T* m_ptr;
};

// 带Dtor的特殊指针引用计数类
// 通过Dtor function/functor 释放指针
template <typename T, typename Dtor>
class RefCountDel : public RefCountBase
{
public:
    RefCountDel( T* ptr, Dtor dtor )
        : m_ptr( ptr )
        , m_dtor( dtor )
    {
    }

protected:
    virtual void DeleteSelf()
    {
        m_dtor( m_ptr ), m_ptr = NULL;
        delete this;
    }

private:
    T* m_ptr;
    Dtor m_dtor;
};

//////////////////////////////////////////////////////////////////////////
// 数组指针Dtor
class ArrayPtrDtor
{
public:
    ArrayPtrDtor() {}

    template <typename T>
    void operator() ( T* ptr )
    {
        delete[] ptr;
    }
};

//////////////////////////////////////////////////////////////////////////
// 智能指针类
template <typename T>
class SharedPtr
{
public:
    SharedPtr()
        : m_ptr( NULL )
        , m_pRefCount( NULL )
    {
    }

    // 普通指针的构造函数
    explicit SharedPtr( T* ptr )
        : m_ptr( ptr )
        , m_pRefCount( NULL )
    {
        if ( m_ptr != NULL )
        {
            m_pRefCount = new(std::nothrow) RefCount<T>( ptr );
            if ( m_pRefCount == NULL )
            {
                delete m_ptr, m_ptr = NULL;
            }
        }
    }

    // 带Dtor的特殊指针构造函数
    template <typename Dtor>
    SharedPtr( T* ptr, Dtor dtor )
        : m_ptr( ptr )
        , m_pRefCount( NULL )
    {
        if ( m_ptr != NULL )
        {
            m_pRefCount = new(std::nothrow) RefCountDel<T, Dtor>( ptr, dtor );
            if ( m_pRefCount == NULL )
            {
                dtor( m_ptr ), m_ptr = NULL;
            }
        }
    }

    SharedPtr( const SharedPtr<T>& ref )
        : m_ptr( ref.m_ptr )
        , m_pRefCount( ref.m_pRefCount )
    {
        if ( m_pRefCount != NULL )
            m_pRefCount->IncreaseRef();
    }

    ~SharedPtr()
    {
        if ( m_pRefCount != NULL )
            m_pRefCount->DecreaseRef();
    }

    SharedPtr<T>& operator= ( const SharedPtr<T>& ref )
    {
        SharedPtr( ref ).Swap( *this );

        return *this;
    }

    void Swap( SharedPtr<T>& ref )
    {
        std::swap( m_ptr, ref.m_ptr );
        std::swap( m_pRefCount, ref.m_pRefCount );
    }

    bool IsValid() const
    {
        return ( m_pRefCount != NULL && m_pRefCount->IsValid() );
    }

    T* Get() const
    {
        return m_ptr;
    }

    T& GetRef() const
    {
        assert( m_ptr != NULL );
        return *m_ptr;
    }

    T& operator*() const
    {
        assert( m_ptr != NULL );
        return *m_ptr;
    }

    T* operator->() const
    {
        assert( m_ptr != NULL );
        return m_ptr;
    }

private:
    T* m_ptr;
    RefCountBase* m_pRefCount;
};


//////////////////////////////////////////////////////////////////////////
// 智能指针和STL结合使用时用到的各工具函数

template <typename T>
void swap( SharedPtr<T>& left, SharedPtr<T>& right )
{
    left.Swap( right );
}

template <typename T1, typename T2>
bool operator== ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return ( s1.Get() == s2.Get() );
}

template <typename T1, typename T2>
bool operator!= ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return !( s1 == s2 );
}

template <typename T1, typename T2>
bool operator< ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return ( s1.Get() < s2.Get() );
}

template <typename T1, typename T2>
bool operator>= ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return !( s1 < s2 );
}

template <typename T1, typename T2>
bool operator> ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return ( s1.Get() > s2.Get() );
}

template <typename T1, typename T2>
bool operator<= ( const SharedPtr<T1>& s1, const SharedPtr<T2>& s2 )
{
    return !( s1.Get() > s2.Get() );
}

#endif // __SHAREDPTR_H__

