#ifndef __ZERO_SOCKET_SLOT_H__
#define __ZERO_SOCKET_SLOT_H__

#include "SocketDef.h"
#include "MsgBaseStruct.h"
#include "GameMsg_Base.h"
#include "Utility.h"
#include "SocketLibCommonDef.h"

namespace ZeroSocket
{

    template< typename T, unsigned int PoolSize >
    class queue
    {
    public:
        // Typedefs
        typedef size_t    size_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;
    public:
        queue()
        {
            head = tail = mpPool;
        }
        ~queue()
        {
            clear();
        }
        bool add(value_type value)
        {
            pointer ptr = tail;
            ptr++;
            if (ptr == mpPool + PoolSize)
                ptr = mpPool;

            if (ptr == head)
            {//full
                return false;
            }
            else
            {
                *tail = value;
                tail++;
                if (tail == mpPool + PoolSize)
                    tail = mpPool;
                return true;
            }
        }
        void get(pointer& pHead)
        {
            if (head != tail)
            {
                pHead = head;
                head++;
                if (head == mpPool + PoolSize)
                    head = mpPool;
            }
            else//empty
                pHead = NULL;
        }
        void clear()
        {
            while (tail != head)
            {
                head->clear();
                head++;
                if (head == mpPool + PoolSize)
                    head = mpPool;
            }
        }
    protected:
    private:
    public:
        value_type mpPool[PoolSize];
        pointer head;
        pointer tail;
    };

#define MAX_SEND_BUFF_PACKET_COUNT		81920  //每个SOCKET的发送缓冲区的封包最大数量
#define MAX_RECV_BUFF_PACKET_COUNT		40960 //接收缓冲区堆积的封包最大数量
#define MAX_SEND_ENCODE_BUFFER_LEN		(MAX_PACKET_SIZE*2) //发送时用于序列化的缓冲区大小。
    //#define MAX_SEND_ENCODE_BUFFER_LEN		65535 //发送时用于序列化的缓冲区大小。

    //接收缓冲区大小
#define	MAX_RECV_BUFF_SIZE		(MAX_PACKET_SIZE*2)

    struct OUT_ELEMENT :public iovec
    {
        OUT_ELEMENT(){
            iov_base = NULL; iov_len = 0;
        };

        void clear()
        {
            if (iov_base != NULL)
            {
                delete [](char *)iov_base;
                iov_base = NULL;
            }
        }
    };

    struct IN_ELEMENT
    {
        GameMsg_Base* pMsg;
        unsigned short nSlot;

        IN_ELEMENT(){
            pMsg = NULL; nSlot = 0;
        };
        IN_ELEMENT(GameMsg_Base* pGameMsg, unsigned short nSlotIndex)
        {
            pMsg = pGameMsg;
            nSlot = nSlotIndex;
        }
        void clear()
        {
            delete pMsg;
            pMsg = NULL;
        }
        void operator=(GameMsg_Base* pGameMsg)
        {
            pMsg = pGameMsg;
        }
    };


    //基本的socket
    struct SLOT_BASE
    {
        int				sock;
        in_addr			addr;
        unsigned short	port;

        SLOT_BASE()
        {
            sock = INVALID_SOCKET;
            Clear();
        }
        void Clear()
        {
            if (sock != INVALID_SOCKET)
            {
                closesocket(sock);
            }
            sock = INVALID_SOCKET;
            addr.s_addr = 0;
            port = 0;
        }
    };

#define SLOT_STATE_FREE			0	//未被占用
#define SLOT_STATE_NORMAL		1	//使用中
#define SLOT_STATE_CLOSE		2	//关闭中

    typedef struct SLOT :public SLOT_BASE
    {
    public:
        SLOT();
        ~SLOT()	{
            Clear();
        };
        void SetSlotIndex(unsigned short nSlotIndex);
        void Clear();
        INLINE void Recv();
        INLINE void Send();
        void OnAccept(SOCKET fd, sockaddr_in &sa);
        void OnConnect();
        INLINE bool Out(GameMsg_Base* pMsg);
        FLOWMETER* GetFlux(){
            return &flux;
        };
        int In(IN_ELEMENT* &pBuff, int nBuffSize);
        bool OutToSelf(GameMsg_Base* pMsg){
            return Recv(pMsg);
        };
        void Disconnect(EDisconnectReason eReason);
        int GetState(){
            return nState;
        };
    protected:
        FLOWMETER	flux;

        IN_ELEMENT buffIn[MAX_RECV_BUFF_PACKET_COUNT];
        IN_ELEMENT *pInHead;
        IN_ELEMENT *pInTail;
        unsigned short nSlot;
        int		nState;

        unsigned int  nBytesRemain;
        char			buffer[MAX_RECV_BUFF_SIZE];

        OUT_ELEMENT buffOut[MAX_SEND_BUFF_PACKET_COUNT];
        OUT_ELEMENT *pOutHead;
        OUT_ELEMENT *pOutTail;
        char*	pMsgCache;
        int		nSeek;
        int m_nMsgCacheLen;

        queue< IN_ELEMENT, 8 > qRecv;

        void Send(OUT_ELEMENT* pOut, int count);
        bool Recv(GameMsg_Base *pMsg);
        void DecodeMsg_Cumulative(CParamPool &pool);
        bool SlotEncodeGameMsg(LPGameMsg_Base pMsg, CParamPool& pool);
        bool doDecode(CParamPool &pool);

        static char  s_writebuffer[MAX_SEND_ENCODE_BUFFER_LEN];
        static CParamPool s_WritePool;
    }*LPSLOT;

    class SlotSendGroup
    {
    public:
        SlotSendGroup(SLOT* slot_base, unsigned int slot_count, int* pFlag);
        ~SlotSendGroup();
    protected:
        static thread_return_type ThreadProc(void* pParam);
        INLINE void Send();
    private:
        Thread _thread;
        ThreadLock _lock;
        SLOT* _slot_base;
        unsigned int _slot_count;
        int* _flag;
    };

    class SlotRecvGroup
    {
    public:
        SlotRecvGroup(SLOT* slot_base, unsigned int slot_count, int* pFlag);
        ~SlotRecvGroup();
    protected:
        static thread_return_type ThreadProc(void* pParam);
        INLINE void Recv();
    private:
        Thread _thread;
        int* _flag;
        SLOT* _slot_base;
        unsigned int _slot_count;
#ifdef WIN32
#else
        struct pollfd *fdarray;
        SLOT* *array;
#endif
    };

}//end of namespace ZeroSocket

#endif//__ZERO_SOCKET_SLOT_H__


//file end

