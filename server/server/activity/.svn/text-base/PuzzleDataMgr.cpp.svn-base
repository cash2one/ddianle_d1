#include "PuzzleDataMgr.h"
#include "PuzzleActivity.h"
#include "SystemActivityManager.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/datainfor/ConfigManager.h"


CPuzzleInfo::CPuzzleInfo()
:m_nPuzzleID(0)
,m_bAnnounce(false)
,m_nMoneyReward(0)
{

}

CPuzzleInfo::~CPuzzleInfo()
{

}


CPuzzleDataMgr::CPuzzleDataMgr()
:m_bIsOpening(false)
,m_nPuzzleBeginTime(0)
,m_nPuzzleEndTime(0)
{

}

CPuzzleDataMgr::~CPuzzleDataMgr()
{

}

CPuzzleDataMgr& CPuzzleDataMgr::Instance()
{
	static CPuzzleDataMgr stcPuzzleMgr;
	return stcPuzzleMgr;
}

bool CPuzzleDataMgr::InitPuzzle()
{
	bool bValid = true;

	if ( m_nPuzzleEndTime > m_nPuzzleBeginTime )
	{
		for ( std::map<int, CPuzzleInfo>::const_iterator it = m_mapAllPuzzle.begin(); it != m_mapAllPuzzle.end(); ++it )
		{
			ItemConfig* pNormalCostItem = ConfigManager::Instance().GetItemConfigManager().GetByID( it->second.m_NormalCost.m_nItemType );
			if ( pNormalCostItem != NULL )
			{
				if ( !pNormalCostItem->IsExpandable() )
				{
					bValid = false;
					WriteLog(LOGLEVEL_ERROR, "Error: CPuzzleDataMgr::InitPuzzle. Normal cost item is not expandable. item=%d", it->second.m_NormalCost.m_nItemType );
				}
			}
			else
			{
				bValid = false;
				WriteLog(LOGLEVEL_ERROR, "Error: CPuzzleDataMgr::InitPuzzle. Normal cost item is not exist. item=%d", it->second.m_NormalCost.m_nItemType );
			}

			if ( it->second.m_SpecialCost.m_nItemType > 0 )
			{
				ItemConfig* pSpecialCostItem = ConfigManager::Instance().GetItemConfigManager().GetByID( it->second.m_SpecialCost.m_nItemType );
				if ( pSpecialCostItem != NULL )
				{
					if ( !pSpecialCostItem->IsExpandable() )
					{
						bValid = false;
						WriteLog(LOGLEVEL_ERROR, "Error: CPuzzleDataMgr::InitPuzzle. Special cost item is not expandable. item=%d", it->second.m_SpecialCost.m_nItemType );
					}
				}
				else
				{
					bValid = false;
					WriteLog(LOGLEVEL_ERROR, "Error: CPuzzleDataMgr::InitPuzzle. Special cost item is not exist. item=%d", it->second.m_SpecialCost.m_nItemType );
				}
			}

			if ( it->second.m_nMoneyReward < 0 )
			{
				bValid = false;
				WriteLog(LOGLEVEL_ERROR, "Error: CPuzzleDataMgr::InitPuzzle. Money reward is not valid. money=%d", it->second.m_nMoneyReward );
			}
		}
	}
	else
	{
		bValid = false;
	}

	if ( bValid )
	{
		CPuzzleActivity * pActivity = new CPuzzleActivity();
		pActivity->Init( m_nPuzzleBeginTime, m_nPuzzleEndTime );
		m_bIsOpening = pActivity->IsOpening();
		CSystemActivityManager::Instance().AddTimerActivity(pActivity);
	}

	return bValid;
}

bool CPuzzleDataMgr::IsOpening() const
{
	return m_bIsOpening;
}

void CPuzzleDataMgr::SetIsOpening(bool bIsOpening)
{
	m_bIsOpening = bIsOpening;
}

int CPuzzleDataMgr::PuzzleBeginTime() const
{
	return m_nPuzzleBeginTime;
}

int CPuzzleDataMgr::PuzzleEndTime() const
{
	return m_nPuzzleEndTime;
}

void CPuzzleDataMgr::AddPuzzleBriefInfo(int nBeginTime, int nEndTime, const char* pHelpTitle, const char* pHelpContent)
{
	m_nPuzzleBeginTime = nBeginTime;
	m_nPuzzleEndTime = nEndTime;

	m_strPuzzleHelpTitle.assign( pHelpTitle );
	m_strPuzzleHelpContent.assign( pHelpContent );
}

void CPuzzleDataMgr::AddPuzzleDetailInfo(int nPuzzleID, const char* pTitle, const char* pTextute, bool bAnnounce,
		int nNormalType, int nNormalCount,	int nSpecialType, int nSpecialCount, const char* pMaleReward, const char* pFemaleReward, int nMoneyReward)
{
	std::map<int, CPuzzleInfo>::iterator it = m_mapAllPuzzle.find( nPuzzleID );
	if ( it == m_mapAllPuzzle.end() )
	{
		CPuzzleInfo puzzleInfo;
		puzzleInfo.m_nPuzzleID = nPuzzleID;
		puzzleInfo.m_bAnnounce = bAnnounce;

		puzzleInfo.m_strTitle.assign( pTitle );
		puzzleInfo.m_strTexture.assign( pTextute );

		puzzleInfo.m_NormalCost.m_nItemType = (itemtype_t)nNormalType;
		puzzleInfo.m_NormalCost.m_nItemCount = (itemcount_t)nNormalCount;
		puzzleInfo.m_NormalCost.m_nValidTime = -1;

		puzzleInfo.m_SpecialCost.m_nItemType = (itemtype_t)nSpecialType;
		puzzleInfo.m_SpecialCost.m_nItemCount = (itemcount_t)nSpecialCount;
		puzzleInfo.m_SpecialCost.m_nValidTime = -1;

		TransferItemList( pMaleReward, puzzleInfo.m_lstMaleReward );
		TransferItemList( pFemaleReward, puzzleInfo.m_lstFemaleReward );
		puzzleInfo.m_nMoneyReward = nMoneyReward;

		m_mapAllPuzzle.insert( std::make_pair( nPuzzleID, puzzleInfo ) );
	}
	else
	{
		WriteLog(LOGLEVEL_WARNING, "Error: CPuzzleDataMgr::AddPuzzleDetailInfo. Same puzzle(%d) added, ignore", nPuzzleID );
	}
}

