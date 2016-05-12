#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

//////////////////////////////////////////////////////////////////////////
//memory pool

//内存池
//一次性申请大块的内存，分成固定大小的若干块，每块指向下一块，形成一个链
//需要申请时直接使用链头上的一块，把该块指向的下一块作为新的链头
//释放时把该块作为新链头，然后指向原来的链头
//如果空间不够就再一次申请一个大块
//最后完全不用了就一次全部释放
//优点：这样就可以避免频繁地使用内存分配函数（内存分配函数比较耗时）
//缺点：只能使用在固定大小的内存需求上，不同的内存需求就要创建不同的内存池
//注意：节点大小必须是4的倍数，以避免指针对齐的问题！！！！！！！

#ifdef WIN32
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <windows.h>
#pragma warning(disable:4311 4312 4786)
#else
#include <pthread.h>
#endif

#include <list>
using namespace std;

//内存池管理类
//实现机制：初始建立8个内存池，他们分别是16，32，64，128，256，512，1024，2048
//          字节的内存池，当使用的内存不是正好符合内存池提供的大小时，用大一级
//          的内存池来工作
//目的：弥补内存池不能支持任意大小内存的缺陷
//特点：用适当的内存浪费换取较高的内存分配效率
//

class CMemPoolMgr//内存池管理类
{
	//内存池类
	class CMemPool
	{
	public:
		//构造内存池       每个节点的大小        每次分配节点的数量
		CMemPool(unsigned short nNodeSize, unsigned short nNodeCount = 256);
		~CMemPool();

		//取得内存池中的一块
		void * Alloc();

		//不用的节点放回内存池
		void Free(void* &ptr);

		//获得节点大小
		unsigned short GetNodeSize(){return m_nNodeSize;};
		//检查指针是否在池内
		bool PointerInPool(void* ptr);

	protected:
		//申请一大块内存
		bool Resize();
	private:
		unsigned short m_nNodeSize;//每次需求的内存（节点）的大小
		unsigned short m_nNodeCount;//每次分配的节点的数量

		int* m_pHead;//链头
		int* m_pTemp;//临时指针，因为经常用到所以作为成员

		list< int* >m_listBlock;//保存内存块指针以便最后释放
	};

public:
	CMemPoolMgr();
	~CMemPoolMgr();

	//初始化内存池管理器
	bool Initialize(list< pair< unsigned short, unsigned short> > *plistPool = NULL, bool bThreadSafe = true);
	//释放内存池管理器
	void Release();
	
	//从内存池中获取指定长度的内存
	void * Alloc(unsigned short nSize);

	//释放内存池中的
	void Free(void* &ptr, unsigned short nSize = 0);//如果已知长度，效率更高

protected:
	CMemPool * FindMemPool(unsigned short nSize);
	CMemPool * FindMemPool(void* &ptr);

#ifdef WIN32
	CRITICAL_SECTION m_lockMemPool;
#else
	pthread_mutex_t m_lockMemPool; 
#endif

private:
	list< CMemPool* >m_listPool;
	bool m_bThreadSafe;
};

#endif

//End of file


