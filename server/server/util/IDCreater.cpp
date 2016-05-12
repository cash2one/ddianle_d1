#include "../libServerFrame/QueryManager.h"
#include "IDCreater.h"
#include "../../socket/Utility.h"

extern CQueryManager* g_pQueryMgr;
extern CHost s_Host;

#define BATCH_ID_COUNT			10000

CIDCreater::CIDCreater() : GameMsg_Processor(true, true), m_nID32ProcessInternal(0), m_nID64ProcessInternal(0), m_nID64(0), m_nID64Seg(0)
{
    RegComponentNetMsgMap();

    GetBatchID64();

    m_nID32ProcessInternal = 1000000;//��һ����ʼ��֮ǰ����ֵ����һЩ�̶�

    unsigned int nTemp = GetCurrentProcessId();//��16λȡ��ǰ����ID�ĵ�15λ
    m_nID64ProcessInternal = nTemp & 0x00007FFF;
    m_nID64ProcessInternal <<= 16;
    s_Host.GetHostAddr(&nTemp);//�θ�16λȡIP��ַ�ĺ�16λ
    m_nID64ProcessInternal += nTemp & 0x0000FFFF;
    m_nID64ProcessInternal <<= 32;
}

void CIDCreater::RegComponentNetMsgMap()
{
    RegQueryProcessor(QUERY_GetBatchID64, &CIDCreater::OnQueryIDFinish);
}

unsigned int CIDCreater::GetID32()
{
    return m_nID32ProcessInternal++;
}

__int64 CIDCreater::GetID64()
{
    __int64 nID = m_nID64;
    __int64 nLast = m_nID64%BATCH_ID_COUNT;
    if (nLast == 0)
    {
        m_nID64 = m_nID64Seg;
    }
    else
    {
        m_nID64++;
        if (nLast == BATCH_ID_COUNT / 2)
            GetBatchID64();
    }

    return nID;
}

void CIDCreater::GetBatchID64()
{
    __int64 *pID = new __int64;
    *pID = 0;
    g_pQueryMgr->AddQuery(QUERY_GetBatchID64, 0, pID, 0, this);
}

void CIDCreater::OnQueryIDFinish(QueryBase & pQuery)
{
    __int64 nID = *((__int64*)pQuery.m_pAttachment);
    if (nID != 0)
    {
        m_nID64Seg = (nID - 1) * BATCH_ID_COUNT + 1;
        if (m_nID64 == 0)m_nID64 = m_nID64Seg;
        delete (__int64*)pQuery.m_pAttachment;
    }
}

