#include "CoupleManager.h"


#include "../../socket/Log.h"
#include "../libServerFrame/QueryManager.h"
#include "../group/Global.h"
#include <algorithm>

extern CQueryManager* g_pQueryMgr;

CBlessingRankInfo::CBlessingRankInfo( const CBlessingValueRankInfo & rankinfo ) : 
CRankInfo( rankinfo.m_nPairID, rankinfo.m_nBlessingValue )
,m_BlessingValueRankInfo( rankinfo )
{

}

CBlessingRankInfo::~CBlessingRankInfo()
{
}


CBlessingValueRankMgr::CBlessingValueRankMgr()
:CRankMgr(EDefaultMaxRowCount, MAX_BLESSINGRANK_COUNT)
,m_bLoadRankInfoSuc(false)
{
	
}

CBlessingValueRankMgr::~CBlessingValueRankMgr()
{
}

void CBlessingValueRankMgr::LoadRankInfo()
{
	g_pQueryMgr->AddQuery( QUERY_Couple_LoadBlessingValueRank, 0, &m_RankVec );
}

void CBlessingValueRankMgr::LoadRankInfoSuc()
{
    std::set<unsigned int> setRoleID;
	std::vector<CBlessingValueRankInfo>::iterator it = m_RankVec.begin();
	for ( ; it != m_RankVec.end(); ++it )
	{
		CBlessingRankInfo * pRankInfo = new CBlessingRankInfo( *it );
		if ( pRankInfo != NULL )
		{
            if (0 == CRankMgr::AddNewRankInfo(pRankInfo))
            {
                delete pRankInfo;
                pRankInfo = NULL;
            }
		}

        setRoleID.insert(it->m_nMaleRoleID);
        setRoleID.insert(it->m_nFemaleRoleID);
	}

    // 加载离线信息
    CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData(setRoleID);

	m_bLoadRankInfoSuc = true;
}

void CBlessingValueRankMgr::UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname )
{
	CBlessingValueRankInfo rankinfo;
	rankinfo.m_nPairID = nPairID;

	std::vector<CBlessingValueRankInfo>::iterator it = std::find( m_RankVec.begin(), m_RankVec.end(), rankinfo );
	if ( it != m_RankVec.end() )
	{
		CRankInfo * pRankInfo = CRankMgr::GetRankInfo( it->m_nPairID);
		if ( pRankInfo != NULL )
		{
			CBlessingRankInfo * pBlessingRankInfo = (CBlessingRankInfo*)pRankInfo;
			if ( it->m_nMaleRoleID == nRoleID )
			{
				it->m_strMaleRoleName = strNickname;
				pBlessingRankInfo->m_BlessingValueRankInfo.m_strMaleRoleName = strNickname;
			}
			else if ( it->m_nFemaleRoleID == nRoleID )
			{
				it->m_strFemaleRoleName = strNickname;
				pBlessingRankInfo->m_BlessingValueRankInfo.m_strFemaleRoleName = strNickname;
			}
		}
	}
}

void CBlessingValueRankMgr::UpdateBlessingValueInfo( CBlessingValueRankInfo & rankinfo )
{
	// 是否存在
	std::vector<CBlessingValueRankInfo>::iterator it = std::find( m_RankVec.begin(), m_RankVec.end(), rankinfo );
	if ( it != m_RankVec.end() )
	{
		// 更新
		*it = rankinfo;
		unsigned int rnSrcRank= 0; unsigned int rnDesRank = 0;
		CRankInfo * pRankInfo = CRankMgr::UpdateRankInfo( rankinfo.m_nPairID, rankinfo.m_nBlessingValue, rnSrcRank, rnDesRank );
		if ( pRankInfo != NULL )
		{
			CBlessingRankInfo * pBlessingRankInfo = (CBlessingRankInfo*)pRankInfo;
			pBlessingRankInfo->m_BlessingValueRankInfo.m_nBlessingValue = rankinfo.m_nBlessingValue;
		}
	}
	else
	{
		if ( rankinfo.m_nBlessingValue <= 0 )
		{
			return;
		}

		m_RankVec.push_back( rankinfo );
		CBlessingRankInfo * prankinfo = new CBlessingRankInfo(  rankinfo );
		if ( prankinfo != NULL )
		{
            if (0 == CRankMgr::AddNewRankInfo(prankinfo))
            {
                delete prankinfo;
                prankinfo = NULL;
            }
        }
	}
}

void CBlessingValueRankMgr::DelBlessingValueInfo( unsigned int nPairID )
{
	CBlessingValueRankInfo rankinfo;
	rankinfo.m_nPairID = nPairID;

	std::vector<CBlessingValueRankInfo>::iterator it = std::find( m_RankVec.begin(), m_RankVec.end(), rankinfo );
	if ( it != m_RankVec.end() )
	{
		CRankInfo * pRankInfo = CRankMgr::RemoveRankInfo( nPairID );
		if ( pRankInfo != NULL )
		{
			CBlessingRankInfo * pBlessingRankInfo = (CBlessingRankInfo*)pRankInfo;
			delete pBlessingRankInfo;
		}
		m_RankVec.erase( it );
	}
}

int CBlessingValueRankMgr::GetSelfRank( unsigned int nKey )
{
	return CRankMgr::GetRankByKey( nKey );
}

unsigned int CBlessingValueRankMgr::GetRankTotalCount()
{
	return CRankMgr::TotalCount();
}

void CBlessingValueRankMgr::GetBlessingRank( int nBeginRank, int nEndRank, std::list<CRankInfo*> & bList )
{
	CRankMgr::GetRange( nBeginRank, nEndRank, bList );
}

void CBlessingValueRankMgr::GetBlessingRankList(int nPage, int nSinglePageCount, std::vector<CBlessingValueRankInfo> & rList)
{
    std::list<CRankInfo*>  lList;
	int nTotalCount = CRankMgr::TotalCount();

	int nBeginInde = (nPage + 1)*nSinglePageCount;
    int nPageCount = 0;
	if ( nBeginInde > nTotalCount )
	{
		nPageCount = nSinglePageCount - (nBeginInde - nTotalCount);
		nPageCount = nPageCount <= 0 ? 0 : nPageCount;
	}
	else
	{
		nPageCount= nSinglePageCount;
	}

	CRankMgr::GetRange( nPage*nSinglePageCount + 1, nPage*nSinglePageCount + nPageCount, lList );

	std::list<CRankInfo*>::iterator it = lList.begin();
	for ( ; it != lList.end(); ++it )
	{
		CBlessingRankInfo * pRankInfo = ( CBlessingRankInfo *) *it;
		rList.push_back( pRankInfo->m_BlessingValueRankInfo );
	}
}

void CWeddingRecord::AddRecord( const WeddingRecordInfo & record )
{
	WeddingRecordInfo * pRecord = FindWeddingRecord( record.m_nPairID );
	if ( pRecord == NULL )
	{
		m_WiddingRecord.push_front( record );

		if (m_WiddingRecord.size() > EWeddingRecordCount )
		{
			m_WiddingRecord.pop_back();
		}
	}
	else
	{
		*pRecord = record;
	}
}

void CWeddingRecord::DeleteRecord( unsigned int nPairID )
{
	std::deque<WeddingRecordInfo>::iterator it = m_WiddingRecord.begin();
	for ( ; it != m_WiddingRecord.end(); ++it )
	{
		if ( (*it).m_nPairID == nPairID )
		{
			m_WiddingRecord.erase( it );
			break;
		}
	}
}

void  CWeddingRecord::UpdateRecordIsRunning( unsigned int nPairID, bool bIsRunning )
{
	WeddingRecordInfo * pRecord = FindWeddingRecord( nPairID );
	unsigned int nIsRunning = bIsRunning ? 1 : 0 ;
	if ( pRecord != NULL && pRecord->m_nIsRunning != nIsRunning )
	{	
		pRecord->m_nIsRunning = nIsRunning;
	}
}

void CWeddingRecord::UpdateBlessingValue( unsigned int nPairID, unsigned int nBlessingValue )
{
	WeddingRecordInfo * pRecord = FindWeddingRecord( nPairID );
	if ( pRecord != NULL )
	{	
		pRecord->m_nBlessingValue = nBlessingValue;
	}
}

void CWeddingRecord::UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname )
{
	WeddingRecordInfo * pRecord = FindWeddingRecord( nPairID );
	if ( pRecord != NULL )
	{	
		if ( pRecord->m_nMaleID == nRoleID )
		{
			pRecord->m_strMaleName = strNickname;
		}
		else if ( pRecord->m_nFemaleID == nRoleID )
		{
			pRecord->m_strFemaleName = strNickname;
		}
	}
}

void CWeddingRecord::AddBlessingRoleIDInList( unsigned int nPairID, unsigned int nRoleID )
{
	WeddingRecordInfo * pRecord = FindWeddingRecord( nPairID );
	if ( pRecord != NULL && !IsInList( nRoleID, pRecord->m_BlessingRoleIDList ))
	{	
		pRecord->m_BlessingRoleIDList.push_back( nRoleID );
	}
}

void CWeddingRecord::GetRecordList( std::vector<WeddingRecordInfo> & record )
{
	record.insert( record.end(), m_WiddingRecord.begin(), m_WiddingRecord.end() );
}

WeddingRecordInfo * CWeddingRecord::FindWeddingRecord( unsigned int nPairID )
{
	std::deque<WeddingRecordInfo>::iterator it = m_WiddingRecord.begin();
	for ( ; it != m_WiddingRecord.end(); ++it )
	{
		if ( (*it).m_nPairID == nPairID )
		{
           return &(*it);
		}
	}

	return NULL;
}

bool CWeddingRecord::IsInList( unsigned int nRoleID, std::vector<unsigned int> & IDList )
{
	std::vector<unsigned int>::iterator it = IDList.begin();
	for ( ; it != IDList.end(); ++it )
	{
		if ( *it == nRoleID)
		{
			return true;
		}
	}
	return false;
}
