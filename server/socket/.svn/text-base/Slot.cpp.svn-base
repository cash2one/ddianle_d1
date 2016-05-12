#include "Slot.h"
#include "Utility.h"
#include "GameMsg_System_Connect.h"
#include "GameMsg_System_Disconnect.h"
#include "GameMsg_Map.h"
#include "Log.h"
using namespace ZeroSocket;

// socket 底层的bug。直接屏蔽。
#ifdef WIN32
#pragma warning(disable:4389 4548)
#endif

#ifdef WIN32
int writev(SOCKET s, LPWSABUF pBuff, DWORD dwBuffCount)
{
    DWORD dwBytesSent = 0;
    int nRes = WSASend(s, pBuff, dwBuffCount, &dwBytesSent, 0, NULL, NULL);
    if (nRes == 0)
        return dwBytesSent;
    else
    {
        int nErrorCode = WSAGetLastError();
        if (WSAEWOULDBLOCK == nErrorCode)
            return 0;

        return -1;
    }
}
#endif

char  SLOT::s_writebuffer[MAX_SEND_ENCODE_BUFFER_LEN];
CParamPool SLOT::s_WritePool(s_writebuffer, MAX_SEND_ENCODE_BUFFER_LEN);

void SLOT::SetSlotIndex(unsigned short nSlotIndex)
{
    nSlot = nSlotIndex;
    for (int i = 0; i < MAX_RECV_BUFF_PACKET_COUNT; i++)
        buffIn[i].nSlot = nSlotIndex;
}

SLOT::SLOT()
{
    nSlot = 0;
    nState = SLOT_STATE_FREE;

    nBytesRemain = 0;

    pInHead = pInTail = buffIn;
    pOutHead = pOutTail = buffOut;
    pMsgCache = NULL;
    m_nMsgCacheLen = 0;
    nSeek = 0;

    Clear();
}
void SLOT::Clear()
{
    nBytesRemain = 0;
    if (pOutHead != pOutTail)
    {
        OUT_ELEMENT* ptr = pOutHead;
        while (ptr != pOutTail)
        {
            ptr->clear();
            ptr++;
            if (ptr == buffOut + MAX_SEND_BUFF_PACKET_COUNT)
                ptr = buffOut;
        }
        pOutHead = pOutTail = buffOut;
    }
    if (pMsgCache != NULL)
        delete []pMsgCache;
    pMsgCache = NULL;
    nSeek = 0;
    m_nMsgCacheLen = 0;

    if (pInHead != pInTail)
    {
        IN_ELEMENT* ptr = pInHead;
        while (ptr != pInTail)
        {
            ptr->clear();
            ptr++;
            if (ptr == buffIn + MAX_RECV_BUFF_PACKET_COUNT)
                ptr = buffIn;
        }
        pInHead = pInTail = buffIn;
    }

    flux.Clear();
    qRecv.clear();
}

void SLOT::OnConnect()
{
    nState = SLOT_STATE_NORMAL;//改变状态到工作态
}

void SLOT::OnAccept(SOCKET fd, sockaddr_in &saClient)
{
    //清空既有数据
    Clear();

    nState = SLOT_STATE_NORMAL;//改变状态到工作态

    sock = fd;

    //记录地址和端口
    addr = saClient.sin_addr;
    port = ntohs(saClient.sin_port);

    //设置为非堵塞
#ifdef WIN32
    unsigned long flag = 1;
    ioctlsocket(sock, FIONBIO, &flag);
#else
    int flag = 0;
    fcntl(sock, F_GETFL, &flag);
    flag |= O_NONBLOCK;
    fcntl(sock, F_SETFL, flag);
#endif

    GameMsg_System_Connect * pMsg = new GameMsg_System_Connect();
    pMsg->nConnCode = 0;
    Recv(pMsg);
}

void SLOT::Disconnect(EDisconnectReason eReason)
{
    if (nState == SLOT_STATE_NORMAL)
    {
        nState = SLOT_STATE_CLOSE;

        GameMsg_System_Disconnect* msgDisconnect = new GameMsg_System_Disconnect;
        //TODO set the reason
        qRecv.add(IN_ELEMENT(msgDisconnect, nSlot));
    }
}

INLINE void SLOT::Recv()
{
    IN_ELEMENT *e;
    while (true)
    {
        qRecv.get(e);
        if (e == NULL)
        {
            break;
        }
        Recv(e->pMsg);
        if (e->pMsg->nMsg == MSG_SYSTEM_Disconnect)
        {
            //shutdown(sock, SD_RECEIVE);
            //此后再调recv会取回已经在接收缓冲中的数据，没有则返回０
        }
    }

    if (nState != SLOT_STATE_NORMAL)return;

    int nLen = recv(sock, buffer + nBytesRemain, MAX_RECV_BUFF_SIZE - nBytesRemain, 0);
    if (nLen > 0)
    {
        nBytesRemain += nLen;
        CParamPool pool(buffer, nBytesRemain);

        DecodeMsg_Cumulative(pool);

        if (pool.HasRemaining())
        {
            pool.Settle();
        }
        nBytesRemain = pool.Remaining();
    }
    else
    {
#ifdef WIN32
        int nErr = WSAGetLastError();
        if (nErr != WSAEWOULDBLOCK)
#else
        int nErr = errno;
        if (nErr != EAGAIN)
#endif
        {
            WriteLog(LOGLEVEL_ERROR, "WARNING: SLOT::Recv failed. nLen=%d, nErr=%d, nSlot=%u, nBytesRemain=%u", nLen, nErr, (unsigned int)nSlot, (unsigned int)nBytesRemain);
            GameMsg_System_Disconnect * pMsg = new GameMsg_System_Disconnect;
            Recv(pMsg);

            if (nState == SLOT_STATE_NORMAL)
                nState++;
        }
    }
}
void SLOT::DecodeMsg_Cumulative(CParamPool &pool)
{
    for (;;)
    {
        unsigned int nOldPosiont = pool.GetCurPosition();

        bool bdecode = doDecode(pool);
        if (bdecode)
        {
            if (pool.GetCurPosition() == nOldPosiont)
            {
                break;
            }

            if (!pool.HasRemaining())
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

}
bool SLOT::doDecode(CParamPool &pool)
{
    if (pool.Remaining() >= 4)
    {
        int pos = pool.GetCurPosition();
        unsigned short size = pool.GetUShort();
        if (size >= 4)
        {
            if (size - 2 <= pool.Remaining())
            {
                CParamPool MsgPool(pool, pool.GetCurPosition(), size - 2);
                pool.SetCurPosition(pos + size);

                unsigned short nMsgType = MsgPool.GetUShort();
                LPGameMsg_Base pMsg = GameMsg_Map::Instantce().CreateMsgFromType(nMsgType);
                if (pMsg)
                {
                    if (pMsg->decodeMssage(MsgPool, size))
                    {
                        Recv(pMsg);
                        flux.In(size);
                        return true;
                    }

                    if (MsgPool.HasRemaining())
                    {
                        WriteLog(LOGLEVEL_ERROR, "[ERROR] 消息%d解析出错，没有读完所有已接收数据\n", nMsgType);
                    }
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "[ERROR] 消息%d没有注册\n", nMsgType);
                    return true;
                }
            }
            else
            {
                if (size > MAX_PACKET_SIZE)
                {
                    int nMsgType = 0;
                    if (pool.Remaining() > 2)
                    {
                        CParamPool MsgPool(pool, pool.GetCurPosition(), pool.Remaining());
                        nMsgType = (int)MsgPool.GetUShort();
                    }
                    WriteLog(LOGLEVEL_DEBUG, "[INFO] SLOT::doDecode recv buffer < expected size pos=%d, size=%d, remain=%d, nMsgType=%d",
                        (int)pos, (int)size, pool.Remaining(), nMsgType);
                }
                pool.SetCurPosition(pos);
            }
        }
        else
        {
            WriteLog(LOGLEVEL_DEBUG, "[INFO] SLOT::doDecode size<4 pos=%d, size=%d, remain=%d",
                (int)pos, (int)size, pool.Remaining());
            return true;
        }
    }
    return false;
}
bool SLOT::Recv(GameMsg_Base *pMsg)
{
    IN_ELEMENT* ptr = pInTail;
    ptr++;
    if (ptr == buffIn + MAX_RECV_BUFF_PACKET_COUNT)
        ptr = buffIn;
    if (ptr != pInHead)
    {
        *pInTail = pMsg;
        pInTail = ptr;
        return true;
    }
    else
    {
#ifndef WIN32
        ASSERT(0);
#endif
        return false;
    }
}

int SLOT::In(IN_ELEMENT* &pBuff, int nBuffSize)
{
    IN_ELEMENT *tail = pInTail;
    int nCount = tail - pInHead;
    if (nCount > 0)
    {
        if (nCount >= nBuffSize) nCount = nBuffSize;
        memcpy(pBuff, pInHead, sizeof(IN_ELEMENT)* nCount);
        pBuff += nCount;
        pInHead = buffIn + ((pInHead - buffIn) + nCount) % MAX_RECV_BUFF_PACKET_COUNT;
        return nCount;
    }
    else if (nCount < 0)
    {
        int nCount1 = buffIn + MAX_RECV_BUFF_PACKET_COUNT - pInHead;
        int nCount2 = tail - buffIn;
        if (nCount1 >= nBuffSize)
        {
            nCount1 = nBuffSize;
            memcpy(pBuff, pInHead, sizeof(IN_ELEMENT)* nCount1);
            pBuff += nCount1;
            pInHead = buffIn + ((pInHead - buffIn) + nCount1) % MAX_RECV_BUFF_PACKET_COUNT;
            return nCount1;
        }
        else
        {
            memcpy(pBuff, pInHead, sizeof(IN_ELEMENT)* nCount1);
            pInHead = buffIn;
            pBuff += nCount1;
            nBuffSize -= nCount1;
            if (nCount2 >= nBuffSize)
            {
                nCount2 = nBuffSize;
            }
            memcpy(pBuff, pInHead, sizeof(IN_ELEMENT)* nCount2);
            pBuff += nCount2;
            pInHead += nCount2;
            return nCount1 + nCount2;
        }
    }

    if (nState == SLOT_STATE_CLOSE)
        nState = SLOT_STATE_FREE;

    return 0;
}

INLINE void SLOT::Send()
{
    if (pMsgCache != NULL)
    {
        int nLen = send(sock, pMsgCache + nSeek, m_nMsgCacheLen - nSeek, 0);
        if (nLen == 0)
        {
            //正常情况，因为系统缓冲区满而暂时不能发送,等待下次再发
            return;
        }
        if (nLen > 0)
        {
            if (nLen < m_nMsgCacheLen - nSeek)
            {
                nSeek += nLen;
            }
            else
            {
                delete []pMsgCache;
                pMsgCache = NULL;
                nSeek = 0;
                m_nMsgCacheLen = 0;
            }
        }
        else
        {
            int nErr = 0;
#ifndef WIN32
            nErr = errno;
            if (nErr == EAGAIN)
            {
                //WriteLog( " Send failed 2，nSlot = %d,state = %d,ISEAGAIN\n",nSlot,nState,errno);
                return;
            }
#endif
            WriteLog(LOGLEVEL_ERROR, "WARNING: SLOT::Send failed. nLen=%d, nErr=%d, nSlot=%u, m_nMsgCacheLen=%d, nSeek=%d", nLen, nErr, nSlot, m_nMsgCacheLen, nSeek);
            Disconnect(EDisconnectReason_SendError);
        }
    }
    else
    {
        int count = pOutTail - pOutHead;
        if (count < 0)
        {
            count = buffOut + MAX_SEND_BUFF_PACKET_COUNT - pOutHead;
            ASSERT(count > 0);
        }
        if (count > 0)
        {
            Send(pOutHead, count);
        }
        else
        {
            //已有的数据发送完毕，如果是在等待退出则关闭连接
            if (nState == SLOT_STATE_CLOSE)
            {
                SLOT_BASE::Clear();
            }
        }
    }

}

void SLOT::Send(OUT_ELEMENT* pOut, int count)
{
    int nSize = 0;
    for (int i = 0; i < count; i++)
    {
        nSize += pOut[i].iov_len;
    }
    int nLen = writev(sock, pOut, count);
    if (nLen == 0)
    {
        //正常情况，
        return;
    }
    if (nLen > 0)
    {
        if (nLen == nSize)
        {
            flux.Out(nLen, count);
            for (int k = 0; k < count; k++)
            {
                pOut[k].clear();
            }
            pOutHead = buffOut + ((pOutHead - buffOut) + count) % MAX_SEND_BUFF_PACKET_COUNT;
        }
        else
        {
            int nPos = 0;
            int k;
            for (k = 0; k < count; k++)
            {
                nPos += pOut[k].iov_len;
                if (nPos > nLen)break;
                pOut[k].clear();
            }
            flux.Out(nLen, k);
            pMsgCache = (char *)pOut[k].iov_base;
            nSeek = nLen + pOut[k].iov_len - nPos;
            m_nMsgCacheLen = pOut[k].iov_len;
            if (nSeek == 0)
            {
                pMsgCache = NULL;
                m_nMsgCacheLen = 0;
                pOutHead = buffOut + (pOut - buffOut + k) % MAX_SEND_BUFF_PACKET_COUNT;
            }
            else
            {
                pOutHead = buffOut + (pOut - buffOut + k + 1) % MAX_SEND_BUFF_PACKET_COUNT;
            }
        }
    }
    else
    {
        int nErr = 0;
#ifndef WIN32
        nErr = errno;
        if (nErr == EAGAIN)
        {
            //WriteLog( " Send failed 1，nSlot = %d,state = %d,ISEAGAIN\n",nSlot,nState,errno);
            return;
        }
#endif
        pMsgCache = NULL;
        nSeek = 0;
        WriteLog(LOGLEVEL_ERROR, "WARNING: SLOT::Send failed. nLen=%d, nErr=%d, nSlot=%u, nSize=%d, count=%d.", nLen, nErr, nSlot, nSize, count);
        Disconnect(EDisconnectReason_SendError);
    }
}

INLINE bool SLOT::Out( GameMsg_Base* pMsg )
{
    if ( pMsg == NULL )
        return false;

    if ( nState != SLOT_STATE_NORMAL )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Slot is not in using. State=%d.", nState );
        return false;
    }

    OUT_ELEMENT *ptr = pOutTail;
    ++ptr;

    if ( ptr == buffOut + MAX_SEND_BUFF_PACKET_COUNT )
        ptr = buffOut;

    if ( ptr == pOutHead )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Send queue full." );
        return false;
    }

    s_WritePool.ClearData();
    if ( !SlotEncodeGameMsg( pMsg, s_WritePool ) )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Encode msg failed." );
        return false;
    }
    else if ( s_WritePool.GetCurPosition() > MAX_PACKET_SIZE )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Msg size(%u) beyond the max packet size.", pMsg->nSize );
        return false;
    }

    pOutTail->iov_base = s_WritePool.CloneCurrentBuffer();
    pOutTail->iov_len = s_WritePool.GetCurPosition();
    if ( pOutTail->iov_base == NULL )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Create buffer failed." );
    }
    else if ( pOutTail->iov_len != (unsigned long)s_WritePool.GetValidDataSize() )
    {
        WriteLog( LOGLEVEL_ERROR, "[SLOT::Out failed]Msg pos(%lu) not equal size(%u).",
            pOutTail->iov_len, s_WritePool.GetValidDataSize() );
    }
    else
    {
        pOutTail = ptr;
        return true;
    }

    pOutTail->clear();
    return false;
}

INLINE bool SLOT::SlotEncodeGameMsg(LPGameMsg_Base pMsg, CParamPool& pool)
{
    bool bResult = FALSE;
    if (pMsg)
    {
        bResult = pMsg->encodeMssage(pool);
    }
    return bResult;
}
SlotSendGroup::SlotSendGroup(SLOT* slot_base, unsigned int slot_count, int* pflag)
{
    _slot_base = slot_base;
    _slot_count = slot_count;
    _flag = pflag;
    _thread.Create(ThreadProc, this);
}

SlotSendGroup::~SlotSendGroup()
{
    _thread.Wait();
}

thread_return_type SlotSendGroup::ThreadProc(void* pParam)
{
    SlotSendGroup* pGroup = (SlotSendGroup*)pParam;
    while (*pGroup->_flag)
    {
        pGroup->Send();
        millisecondSleep(10);
    }

    return 0;
};

INLINE void SlotSendGroup::Send()
{
    SLOT* pSlot = _slot_base;
    for (unsigned int i = 0; i < _slot_count; i++, pSlot++)
    {
        if (pSlot->sock != INVALID_SOCKET)
            pSlot->Send();
    }
}


SlotRecvGroup::SlotRecvGroup(SLOT* slot_base, unsigned int slot_count, int* pflag)
{
    _slot_base = slot_base;
    _slot_count = slot_count;
    _flag = pflag;
#ifdef WIN32
#else
    fdarray = new pollfd[_slot_count];
    array = new SLOT*[_slot_count];
#endif

    _thread.Create(ThreadProc, this);
}

SlotRecvGroup::~SlotRecvGroup()
{
    _thread.Wait();
#ifdef WIN32
#else
    delete[] fdarray;
    delete[] array;
#endif	
}

thread_return_type SlotRecvGroup::ThreadProc(void* pParam)
{
    SlotRecvGroup* pGroup = (SlotRecvGroup*)pParam;

    while (*pGroup->_flag)
    {
        pGroup->Recv();
    }

    return 0;
};

INLINE void SlotRecvGroup::Recv()
{
#ifdef WIN32
    timeval tv;
    unsigned int i;
    int ret;
    fd_set set;
    int maxfd = 0;
    tv.tv_sec = 0;
    tv.tv_usec = 10000;
    FD_ZERO(&set);
    SLOT* pSlot = _slot_base;
    for (i = 0; i < _slot_count; i++, pSlot++)
    {
        if (pSlot->sock != INVALID_SOCKET)
        {
            FD_SET(pSlot->sock, &set);
            maxfd = max(maxfd, pSlot->sock);
        }
    }

    ret = select(maxfd + 1, &set, NULL, NULL, &tv);
    if (ret > 0)
    {
        pSlot = _slot_base;
        for (i = 0; ret > 0 && i < _slot_count; i++, pSlot++)
        {
            if (pSlot->sock != INVALID_SOCKET &&
                FD_ISSET(pSlot->sock, &set))
            {
                ret--;
                pSlot->Recv();
            }
        }
    }
    else if (ret == 0) // timeout
    {
    }
    else // waiting for retry
    {
        millisecondSleep(10);
    }
#else
    SLOT* ptr = _slot_base;
    unsigned int nfds = 0;
    for (unsigned int i = 0; i < _slot_count; i++, ptr++)
    {
        if (ptr->sock > 0)
        {
            fdarray[nfds].fd = ptr->sock;
            fdarray[nfds].events = POLLIN;
            fdarray[nfds].revents = 0;
            array[nfds] = ptr;
            nfds++;
        }
    }
    poll(fdarray, nfds, 10);
    for (unsigned int k = 0; k < nfds; k++)
    {
        if (fdarray[k].revents & POLLIN)
        {
            array[k]->Recv();
        }
    }
#endif
}


//file end

