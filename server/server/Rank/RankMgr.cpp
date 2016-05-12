#include <stdint.h>
#include <limits.h>
#include "../share/StdAfx.h"
#include "RankMgr.h"

#include "../../socket/Log.h"

void CRankInfo::SetValue( unsigned int nValue )
{
    if ( nValue > UINT_MAX )
        m_nValue = UINT_MAX;
    else
        m_nValue = nValue;
}

bool CRankInfo::ChangeValue( int nChangeValue )
{
    if ( nChangeValue > 0 )
    {
        if ( m_nValue == UINT_MAX )
            return false;

        __int64 nNewValue = m_nValue + nChangeValue;
        if ( nNewValue > UINT_MAX )
            nNewValue = UINT_MAX;

        m_nValue = (unsigned int)nNewValue;

        return true;
    }
    else if ( nChangeValue < 0 )
    {
        nChangeValue = -nChangeValue;
        if ( m_nValue < (unsigned int)nChangeValue )
            m_nValue = 0;
        else
            m_nValue -= (unsigned int)nChangeValue;

        return true;
    }

    return false;
}

CRankRow::CRankRow(unsigned int nRowID, unsigned int nMaxRowCount)
: m_nBeginValue(0)
, m_nEndValue(UINT_MAX)
, m_nCurrentCount(0)
, m_cnRowID(nRowID)
, m_cnMaxRowCount(nMaxRowCount)
, m_aRankInfo(NULL)
{
	m_aRankInfo = new CRankInfo * [m_cnMaxRowCount];
	for (size_t i=0; i<m_cnMaxRowCount; i++)
	{
		m_aRankInfo[i] = NULL;
	}
}
CRankRow::~CRankRow()
{
	for (size_t i=0; i<m_cnMaxRowCount; i++)
	{
		delete m_aRankInfo[i];
		m_aRankInfo[i] = NULL;
	}
	delete [] m_aRankInfo;
}

//添加元素，返回元素排名，如果返回0表示添加失败
unsigned int CRankRow::AddRankInfo(CRankInfo * pRankInfo)
{
	unsigned int nRank = 0;
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::AddRankInfo. pRankInfo == NULL");
		return nRank;
	}
	if (m_nCurrentCount >= m_cnMaxRowCount)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::AddRankInfo. Row is Full");
		return nRank;
	}

	int nPos = 0;
	if (m_nCurrentCount != 0)
	{
		nPos = Find(pRankInfo->m_nValue);
	}
	if (nPos >= 0)
	{
		nRank = m_cnRowID * m_cnMaxRowCount + nPos + 1;
	}


	if (nRank == 0)
	{
		nRank = m_cnRowID * m_cnMaxRowCount + m_nCurrentCount + 1;
		m_aRankInfo[m_nCurrentCount] = pRankInfo;
	}
	else
	{
		for (int i=m_nCurrentCount; i>nPos; i--)
		{
			m_aRankInfo[i] = m_aRankInfo[i-1];
		}
		m_aRankInfo[nPos] = pRankInfo;
	}
	m_KeyInfoMap[pRankInfo->m_nKey] = pRankInfo;


	m_nCurrentCount++;
	if (pRankInfo->m_nValue > m_nBeginValue)
	{
		m_nBeginValue = pRankInfo->m_nValue;
	}
	if (pRankInfo->m_nValue < m_nEndValue)
	{
		m_nEndValue = pRankInfo->m_nValue;
	}
	return nRank;
}
//向前面插入记录
bool CRankRow::PushFront(CRankInfo * pRankInfo)
{
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushFront. pRankInfo == NULL");
		return false;
	}
	if (m_nCurrentCount >= m_cnMaxRowCount)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushFront. Row is Full");
		return false;
	}

	if (m_aRankInfo[m_cnMaxRowCount - 1])
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushFront. Last element is not null");
		return false;
	}
	for (int i=m_nCurrentCount; i>0; i--)
	{
		m_aRankInfo[i] = m_aRankInfo[i-1];
	}

	m_aRankInfo[0] = pRankInfo;
	m_KeyInfoMap[pRankInfo->m_nKey] = pRankInfo;

	m_nCurrentCount++;
	if (pRankInfo->m_nValue > m_nBeginValue)
	{
		m_nBeginValue = pRankInfo->m_nValue;
	}
	if (pRankInfo->m_nValue < m_nEndValue)
	{
		m_nEndValue = pRankInfo->m_nValue;
	}
	return true;
}

//向后面插入记录
bool CRankRow::PushBack(CRankInfo * pRankInfo)
{
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushBack. pRankInfo == NULL");
		return false;
	}
	if (m_nCurrentCount >= m_cnMaxRowCount)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushBack. Row is Full");
		return false;
	}

	if (m_aRankInfo[m_cnMaxRowCount - 1])
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::PushBack. Last element is not null");
		return false;
	}

	m_aRankInfo[m_nCurrentCount] = pRankInfo;
	m_KeyInfoMap[pRankInfo->m_nKey] = pRankInfo;

	m_nCurrentCount++;
	if (pRankInfo->m_nValue > m_nBeginValue)
	{
		m_nBeginValue = pRankInfo->m_nValue;
	}
	if (pRankInfo->m_nValue < m_nEndValue)
	{
		m_nEndValue = pRankInfo->m_nValue;
	}
	return true;
}

//弹出Key对应元素
CRankInfo * CRankRow::PopRankInfo(unsigned int nKey, unsigned int & rnRanking)
{
	rnRanking = 0;
	CRankInfo * pRankInfo = GetRankInfo(nKey);
	if (pRankInfo == NULL)
	{
		return NULL;
	}

	int nPos = Find(pRankInfo->m_nValue);
	for (int i=nPos-1; i>=0; i--)
	{
		if (m_aRankInfo[i]->m_nKey == nKey)
		{
			rnRanking = m_cnRowID * m_cnMaxRowCount + i + 1;
			for (size_t j=i; j<m_nCurrentCount-1; j++)
			{
				m_aRankInfo[j] = m_aRankInfo[j+1];
			}
			m_aRankInfo[m_nCurrentCount - 1] = NULL;
			m_nCurrentCount--;
			if (IsEmpty())
			{
				m_nBeginValue = 0;
				m_nEndValue = UINT_MAX;
			}
			else
			{
				CRankInfo * pNewTailer = m_aRankInfo[m_nCurrentCount - 1];
				if (pNewTailer->m_nValue > m_nEndValue)
				{
					m_nEndValue = pNewTailer->m_nValue;
				}
				CRankInfo * pNewHeader = m_aRankInfo[0];
				if (pNewHeader->m_nValue < m_nBeginValue)
				{
					m_nBeginValue = pNewHeader->m_nValue;
				}
			}
			break;
		}
	}

	m_KeyInfoMap.erase(pRankInfo->m_nKey);
	return pRankInfo;
}
//弹出第一个元素
CRankInfo * CRankRow::PopFront()
{
	CRankInfo * pRankInfo = NULL;
	if (!IsEmpty())
	{
		pRankInfo = m_aRankInfo[0];
		m_KeyInfoMap.erase(pRankInfo->m_nKey);

		for (size_t i=0; i<m_nCurrentCount-1; i++)
		{
			m_aRankInfo[i] = m_aRankInfo[i+1];
		}
		m_aRankInfo[m_nCurrentCount-1] = NULL;
		m_nCurrentCount--;
		if (IsEmpty())
		{
			m_nBeginValue = 0;
			m_nEndValue = UINT_MAX;
		}
		else
		{
			CRankInfo * pNewHeader = m_aRankInfo[0];
			if (pNewHeader->m_nValue < m_nBeginValue)
			{
				m_nBeginValue = pNewHeader->m_nValue;
			}
		}
	}
	else
	{
		WriteLog(LOGLEVEL_WARNING, "Warn: CRankRow::PopFirstRankInfo. This is no element in the row.");
	}
	return pRankInfo;
}
//弹出最后一个元素
CRankInfo * CRankRow::PopBack()
{
	CRankInfo * pRankInfo = NULL;
	if (!IsEmpty())
	{
		pRankInfo = m_aRankInfo[m_nCurrentCount-1];
		m_KeyInfoMap.erase(pRankInfo->m_nKey);
		m_aRankInfo[m_nCurrentCount-1] = NULL;
		m_nCurrentCount--;
		if (IsEmpty())
		{
			m_nBeginValue = 0;
			m_nEndValue = UINT_MAX;
		}
		else
		{
			CRankInfo * pNewTailer = m_aRankInfo[m_nCurrentCount-1];
			if (pNewTailer->m_nValue > m_nEndValue)
			{
				m_nEndValue = pNewTailer->m_nValue;
			}
		}
	}
	else
	{
		WriteLog(LOGLEVEL_WARNING, "Warn: CRankRow::PopBack. This is no element in the row.");
	}
	return pRankInfo;
}

const CRankInfo* CRankRow::Front() const
{
    if (!IsEmpty() )
        return m_aRankInfo[0];

    return NULL;
}

bool CRankRow::IsInRow(unsigned int nKey)
{
    CRankInfo *pRankInfo = GetRankInfo(nKey);

    if (NULL != pRankInfo)
    {
        int nPos = Find(pRankInfo->m_nValue);

        for (int i = nPos - 1; i >= 0; --i)
        {
            if (m_aRankInfo[i]->m_nKey == nKey)
                return true;
        }
    }

    return false;
}

//根据Key获取玩家排名
unsigned int CRankRow::GetRank(unsigned int nKey)
{
	unsigned int nRank = 0;

	CRankInfo * pRankInfo = GetRankInfo(nKey);
	if (pRankInfo == NULL)
	{
		return nRank;
	}

	int nPos = Find(pRankInfo->m_nValue);
	for (int i=nPos-1; i>=0; i--)
	{
		if (m_aRankInfo[i]->m_nKey == nKey)
		{
			nRank = CRankRow::m_cnMaxRowCount * m_cnRowID + i + 1;
			break;
		}
	}
	return nRank;
}

//获取范围,返回实际获取的数量
unsigned int CRankRow::GetRange(unsigned int nStartPos, unsigned int nCount, std::list< CRankInfo * > & rRankInfoList) const
{
	if (nStartPos >= m_nCurrentCount)
	{
		return 0;
	}
	unsigned int nRetCount = 0;
	for (size_t i=nStartPos; nRetCount<nCount && i<m_nCurrentCount; i++)
	{
		rRankInfoList.push_back(m_aRankInfo[i]);
		nRetCount++;
	}
	return nRetCount;
}
//获取排行信息
CRankInfo * CRankRow::GetRankInfo(unsigned int nKey)
{
	std::map< unsigned int, CRankInfo * >::iterator it = m_KeyInfoMap.find(nKey);
	if (it != m_KeyInfoMap.end())
	{
		return it->second;
	}
	return NULL;
}

//查找第一个小于Value的位置
int CRankRow::Find(unsigned int nValue)
{
	int nLeft = 0;
	int nRight = m_nCurrentCount - 1;
	int nMid = (nLeft + nRight) / 2;
	while (nLeft <= nRight)
	{
		if (m_aRankInfo[nMid]->m_nValue < nValue)
		{
			nRight = nMid - 1;
		}
		else
		{
			nLeft = nMid + 1;
		}
		nMid = (nLeft + nRight) / 2;
	}
	return nLeft;
}

CRankMgr::CRankMgr()
    : m_nTotalCount(0)
    , m_nMaxTotalCount((unsigned int)-1)
    , m_cnMaxRowCount(EDefaultMaxRowCount)
{
}

CRankMgr::CRankMgr(unsigned int nMaxRowCount)
    : m_nTotalCount(0)
    , m_nMaxTotalCount((unsigned int)-1)
    , m_cnMaxRowCount(nMaxRowCount)
{
}

CRankMgr::CRankMgr(unsigned int nMaxRowCount, unsigned int nMaxTotalCount)
    : m_nTotalCount(0)
    , m_nMaxTotalCount(nMaxTotalCount)
    , m_cnMaxRowCount(0)
{
    if (nMaxRowCount > nMaxTotalCount)
        m_cnMaxRowCount = nMaxTotalCount;
    else
        m_cnMaxRowCount = nMaxRowCount;
}

CRankMgr::~CRankMgr()
{
	Reset();
}
//数据重置
void CRankMgr::Reset()
{
	for (size_t i=0; i<m_RankRowVec.size(); i++)
	{
		delete m_RankRowVec[i]; m_RankRowVec[i] = NULL;
	}
	m_RankRowVec.clear();
	m_KeyRowMap.clear();
	m_KeyInfoMap.clear();
	m_nTotalCount = 0;
}
//添加元素，返回名次，如果返回值为0表示插入失败
unsigned int CRankMgr::AddNewRankInfo(CRankInfo * pRankInfo)
{
	unsigned int nRank = 0;
	if (pRankInfo == NULL)
	{
		return nRank;
	}
	if (_GetRankInfoByKey(pRankInfo->m_nKey) != NULL)
	{
		WriteLog(LOGLEVEL_DEBUG, "Debug: CRankMgr::AddNewRankInfo. Role Exist. Key(%u)", pRankInfo->m_nKey);
		return nRank;
	}

    if (m_nTotalCount == m_nMaxTotalCount)
    {
        unsigned int nMinValue = GetMinValue();

        if (pRankInfo->m_nValue <= nMinValue)
            return nRank;

        PopBack();
    }

	CRankRow * pRankRow = _GetRowToInsert_Up(pRankInfo->m_nValue);
	if (pRankRow == NULL)
	{
		//增加新列
		pRankRow = new CRankRow(m_RankRowVec.size(), m_cnMaxRowCount);

		m_RankRowVec.push_back(pRankRow);

		//增加新元素
		nRank = pRankRow->AddRankInfo(pRankInfo);
		m_KeyRowMap[pRankInfo->m_nKey] = pRankRow;
		m_KeyInfoMap[pRankInfo->m_nKey] = pRankInfo;
		m_nTotalCount++;
	}
	else
	{
		//判断是否需要弹出元素
		CRankInfo * pAddRankInfo = NULL;
		if (pRankRow->IsFull())
		{
			pAddRankInfo = pRankRow->PopBack();
		}

		//增加新元素
		nRank = pRankRow->AddRankInfo(pRankInfo);
		m_KeyRowMap[pRankInfo->m_nKey] = pRankRow;
		m_KeyInfoMap[pRankInfo->m_nKey] = pRankInfo;
		m_nTotalCount++;

		_SortOutDown(pRankRow->GetRowID() + 1, pAddRankInfo);
	}

	return nRank;
}

CRankInfo * CRankMgr::GetRankInfo(unsigned int nKey)
{
	CRankRow * pRankRow = _GetRowByKey(nKey);
	if (pRankRow == NULL)
	{
		return NULL;
	}

	return pRankRow->GetRankInfo(nKey);
}

//移除元素，需要管理内存
CRankInfo * CRankMgr::RemoveRankInfo(unsigned int nKey)
{
	CRankInfo * pReturnRankInfo = NULL;
	unsigned int nRank = 0;
	CRankRow * pRankRow = _GetRowByKey(nKey);
	if (pRankRow == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::RemoveRankInfo. pRankRow == NULL, Key=%d", nKey);
		return NULL;
	}
	pReturnRankInfo = pRankRow->PopRankInfo(nKey, nRank);
	if (pReturnRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::RemoveRankInfo. pRankRow->PopRankInfo Failed.Key=%d", nKey);
		return NULL;
	}

    --m_nTotalCount;
    m_KeyInfoMap.erase(nKey);
    m_KeyRowMap.erase(nKey);

	for (size_t i=pRankRow->GetRowID(); i<m_RankRowVec.size()-1; i++)
	{
		CRankInfo * pPopRankInfo = m_RankRowVec[i+1]->PopFront();
		m_RankRowVec[i]->PushBack(pPopRankInfo);
		m_KeyRowMap[pPopRankInfo->m_nKey] = m_RankRowVec[i];
	}

	if (!m_RankRowVec.empty() && m_RankRowVec[m_RankRowVec.size()-1]->IsEmpty())
	{
		CRankRow * pLastRow = m_RankRowVec[m_RankRowVec.size()-1];
		delete pLastRow; pLastRow = NULL;
		m_RankRowVec.pop_back();
	}
	return pReturnRankInfo;
}

//更新排行值
CRankInfo * CRankMgr::UpdateRankInfo(unsigned int nKey, unsigned int nValue, unsigned int & rnSrcRank, unsigned int & rnDesRank)
{
	rnSrcRank = 0;
	rnDesRank = 0;
	CRankInfo * pRankInfo = _GetRankInfoByKey(nKey);
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_DEBUG, "Debug: CRankMgr::UpdateRankInfo. Role Not Exist. Key(%u)", nKey);
		return NULL;
	}
	return ChangeRankInfo(nKey, nValue - pRankInfo->m_nValue, rnSrcRank, rnDesRank);
}

//玩家数据变更
CRankInfo * CRankMgr::ChangeRankInfo(unsigned int nKey, int nChangedValue, unsigned int & rnSrcRank, unsigned int & rnDesRank)
{
	CRankInfo * pRankInfo = NULL;
	rnSrcRank = 0;
	rnDesRank = 0;
	if (nChangedValue == 0)
	{
		return pRankInfo;
	}

	CRankRow * pRankRow = _GetRowByKey(nKey);
	if (pRankRow == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::ChangeValue. pRankRow == NULL, Key=%d", nKey);
		return pRankInfo;
	}
	
	pRankInfo = pRankRow->PopRankInfo(nKey, rnSrcRank);
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::ChangeValue. pRankRow->PopRankInfo Failed.Key=%d", nKey);
		return pRankInfo;
	}
	if (nChangedValue > 0)
	{
        if ( !pRankInfo->ChangeValue( nChangedValue ) )
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::ChangeValue. pRankInfo->m_nValue == UINT_MAX");
			return NULL;
		}
		//值增长了
		rnDesRank = _InsertRankInfo_Up(pRankInfo);
	}
	else if (nChangedValue < 0)
	{
        pRankInfo->ChangeValue( nChangedValue );
		rnDesRank = _InsertRankInfo_Down(pRankInfo, pRankRow->GetRowID());
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::ChangeValue. Error, Do nothing");
		// Error, Do nothing
	}
	return pRankInfo;
}

//总数
unsigned int CRankMgr::TotalCount() const
{
	return m_nTotalCount;
}

//获取玩家的排名
unsigned int CRankMgr::GetRankByKey(unsigned int nKey)
{
	unsigned int nRank = 0;
	CRankRow * pRankRow = _GetRowByKey(nKey);
	if (pRankRow)
	{
		nRank = pRankRow->GetRank(nKey);
	}
	return nRank;
}
//获取范围信息
void CRankMgr::GetRange(unsigned int nBeginRank, unsigned int nEndRank, std::list< CRankInfo * > & rRankInfoList) const
{
	if (nBeginRank == 0 || nEndRank == 0 || nBeginRank > nEndRank || nBeginRank > m_nTotalCount)
	{
		return;
	}
	unsigned int nCount = nEndRank - nBeginRank + 1;
	int nStartRow = (nBeginRank-1) / m_cnMaxRowCount;
	unsigned int nStartPos = (nBeginRank-1) % m_cnMaxRowCount;
	if (nStartRow >= (int)m_RankRowVec.size())
	{
		return;
	}
	for (int i=nStartRow; i<(int)m_RankRowVec.size(); i++)
	{
		CRankRow * pLoopRankRow = m_RankRowVec[i];
		unsigned int nGetCount = pLoopRankRow->GetRange(nStartPos, nCount, rRankInfoList);
		if (nGetCount >= nCount)
		{
			break;
		}
		nStartPos = 0;
		nCount -= nGetCount;
	}
}

const CRankInfo* CRankMgr::Front() const
{
    if (m_RankRowVec.size() > 0)
    {
        CRankRow *pRankRow = m_RankRowVec[0];

        if (NULL != pRankRow)
            return pRankRow->Front();
    }

    return NULL;
}

unsigned int CRankMgr::GetMinValue() const
{
    if (m_RankRowVec.size() > 0)
    {
        CRankRow *pRankRow = m_RankRowVec[m_RankRowVec.size() - 1];

        if (NULL != pRankRow)
            return pRankRow->GetEndValue();
    }

    return 0;
}

bool CRankMgr::IsInRank(unsigned int nKey)
{
    CRankRow *pRankRow = _GetRowByKey(nKey);

    if (NULL != pRankRow)
        return pRankRow->IsInRow(nKey);

    return false;
}

void CRankMgr::PopBack()
{
    if (m_RankRowVec.empty() )
        return;

    CRankRow *pLastRow = m_RankRowVec[m_RankRowVec.size() - 1];

    if (NULL == pLastRow)
        return;

    CRankInfo *pLastInfo = pLastRow->PopBack();

    if (NULL == pLastInfo)
        return;

    --m_nTotalCount;
    m_KeyInfoMap.erase(pLastInfo->m_nKey);
    m_KeyRowMap.erase(pLastInfo->m_nKey);

    delete pLastInfo;
    pLastInfo = NULL;

    if (pLastRow->IsEmpty() )
    {
        delete pLastRow;
        pLastRow = NULL;

        m_RankRowVec.pop_back();
    }
}

//根据值查找该插入的列，返回NULL表示需要新增列了
CRankRow * CRankMgr::_GetRowToInsert_Up(unsigned int nValue)
{
	CRankRow * pRankRow = NULL;
	for (int i=0; i<(int)m_RankRowVec.size(); i++)
	{
		if (nValue > m_RankRowVec[i]->GetEndValue())
		{
			pRankRow = m_RankRowVec[i];
			break;
		}
		else if (!m_RankRowVec[i]->IsFull())
		{
			pRankRow = m_RankRowVec[i];
			break;
		}
	}
	if (pRankRow == NULL && !m_RankRowVec.empty() && !m_RankRowVec[m_RankRowVec.size()-1]->IsFull())
	{
		pRankRow = m_RankRowVec[m_RankRowVec.size()-1];
	}
	return pRankRow;
}

//根据值查找该插入的列，返回NULL表示出错了。为名次下降时查找用。
CRankRow * CRankMgr::_GetRowToInsert_Down(unsigned int nValue, unsigned int nSrcRowID)
{
	CRankRow * pRankRow = NULL;
 	for (int i=m_RankRowVec.size()-1; i>=(int)nSrcRowID; i--)
	{
		if (nValue <= m_RankRowVec[i]->GetBeginValue())
		{
			pRankRow = m_RankRowVec[i];
			break;
		}
	}
	if (pRankRow == NULL && m_RankRowVec.size() > nSrcRowID)
	{
		pRankRow = m_RankRowVec[nSrcRowID];
	}
	return pRankRow;
}

//向上插入一条记录，当名次上升时使用，返回插入后的排名
unsigned int CRankMgr::_InsertRankInfo_Up(CRankInfo * pRankInfo)
{
	unsigned int nRank = 0;
	CRankRow * pRankRow = _GetRowToInsert_Up(pRankInfo->m_nValue);
	
	if (pRankRow == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::_InsertRankInfo_Up. pRankRow == NULL, Value=%d", pRankInfo->m_nValue);
		return nRank;
	}
	CRankInfo * pAddRankInfo = NULL;
	if (pRankRow->IsFull())
	{
		pAddRankInfo = pRankRow->PopBack();
	}
	nRank = pRankRow->AddRankInfo(pRankInfo);
	m_KeyRowMap[pRankInfo->m_nKey] = pRankRow;

	if (_SortOutDown(pRankRow->GetRowID()+1, pAddRankInfo))
	{
		//走到这里是不可能的，因为内部元素其实没有变
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::_InsertRankInfo_Up. 有数据多出来，说明内部有问题. TotalCount:%u, m_RankRowVec.size():%u", m_nTotalCount, m_RankRowVec.size());
	}
	return nRank;
}
//向下插入一条记录，当名次下降时使用，返回插入后的排名
unsigned int CRankMgr::_InsertRankInfo_Down(CRankInfo * pRankInfo, unsigned int nSrcRowID)
{
	unsigned int nRank = 0;
	if (pRankInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::_InsertRankInfo_Down. pRankInfo == NULL");
		return nRank;
	}
	CRankRow * pRankRow = _GetRowToInsert_Down(pRankInfo->m_nValue, nSrcRowID);
	if (pRankRow == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRankMgr::_InsertRankInfo_Down. pRankRow == NULL, Value=%d", pRankInfo->m_nValue);
		return nRank;
	}
	for (unsigned int i= nSrcRowID; i<pRankRow->GetRowID(); i++)
	{
		CRankInfo * pPopRankInfo = m_RankRowVec[i+1]->PopFront();
		m_RankRowVec[i]->PushBack(pPopRankInfo);
		m_KeyRowMap[pPopRankInfo->m_nKey] = m_RankRowVec[i];
	}
	nRank = pRankRow->AddRankInfo(pRankInfo);
	m_KeyRowMap[pRankInfo->m_nKey] = pRankRow;

	return nRank;
}

//根据角色编号获取Row
CRankRow * CRankMgr::_GetRowByKey(unsigned int nKey)
{
	CRankRow * pRankRow = NULL;
	std::map< unsigned int, CRankRow * >::iterator it = m_KeyRowMap.find(nKey);
	if (it != m_KeyRowMap.end())
	{
		pRankRow = it->second;
	}
	return pRankRow;
}

//根据角色编号获取擂台赛信息
CRankInfo * CRankMgr::_GetRankInfoByKey(unsigned int nKey)
{
	CRankInfo * pRankInfo = NULL;
	std::map< unsigned int, CRankInfo * >::iterator it = m_KeyInfoMap.find(nKey);
	if (it!=m_KeyInfoMap.end())
	{
		pRankInfo = it->second;
	}
	return pRankInfo;
}

//向下整理排行榜，如果增加了新的列，返回true，否则false
bool CRankMgr::_SortOutDown(unsigned int nBeginRow, CRankInfo * pExtraRankInfo)
{
	if (pExtraRankInfo == NULL)
	{
		return false;
	}
	CRankInfo * pAddRankInfo = pExtraRankInfo;
	for (size_t i=nBeginRow; i<m_RankRowVec.size(); i++)
	{
		CRankRow * pLoopRankRow = m_RankRowVec[i];
		if (pLoopRankRow->IsFull())
		{
			CRankInfo * pPopRankInfo = pLoopRankRow->PopBack();
			pLoopRankRow->PushFront(pAddRankInfo);
			m_KeyRowMap[pAddRankInfo->m_nKey] = pLoopRankRow;
			pAddRankInfo = pPopRankInfo;
		}
		else
		{
			pLoopRankRow->PushFront(pAddRankInfo);
			m_KeyRowMap[pAddRankInfo->m_nKey] = pLoopRankRow;
			pAddRankInfo = NULL;
			break;
		}
	}
	//列位不够了
	if (pAddRankInfo)
	{
		CRankRow * pNewRankRow = new CRankRow(m_RankRowVec.size(), m_cnMaxRowCount);
		m_RankRowVec.push_back(pNewRankRow);
		pNewRankRow->PushBack(pAddRankInfo);
		m_KeyRowMap[pAddRankInfo->m_nKey] = pNewRankRow;
		return true;
	}
	else
	{
		return false;
	}
}

