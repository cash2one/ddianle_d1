#include "DanceGroupDataMgr.h"
#include "../server/util/CommonFunc.h"


CDGLevelUpRequireRes::CDGLevelUpRequireRes()
:m_nGroupLevel(0)
,m_nResA(0)
,m_nResB(0)
,m_nResC(0)
{

}

CDGLevelUpRequireRes::~CDGLevelUpRequireRes()
{

}

CeremonyNewerPrize::CeremonyNewerPrize()
    : m_nRanking(0)
{

}
CeremonyNewerPrize::~CeremonyNewerPrize(){}

CeremonyNormalPrize::CeremonyNormalPrize()
    : m_nMaxPraiser(0)
{

}
CeremonyNormalPrize::~CeremonyNormalPrize()
{}



///////////////////////////////////////////////////////////////////
CDanceGroupDataMgr::CDanceGroupDataMgr()
:m_nMaxDanceGroupLevel(0)
,m_nMemberLoginGroupHonorBonusLevelNeed(10)
,m_nMemberLoginGropHonorBonus(1)
,m_nMinRoomDancersGroupHonorBonusNeed(3)
,m_RoomGroupHonorBonusGeneID(7006)
,m_nMemberMatchBonusLeve1(1)
,m_nIncDGHonorNumPerDonatedResA(0)
,m_nIncDGHonorNumPerDonatedResB(0)
,m_nIncDGHonorNumPerDonatedResC(0)
,m_nIncMemberContributionNumPerDonatedResA(0)
,m_nIncMemberContributionNumPerDonatedResB(0)
,m_nIncMemberContributionNumPerDonatedResC(0)
,m_nCeremonyFreeApplyTimes(1)
,m_nCeremonyApplyCostType(0)
,m_nCeremonyApplyCostNum(0)
,m_nCeremonyFreePraiseTimes(1)
,m_nCeremonyPraiseCostType(0)
,m_nCeremonyPraiseCostNum(0)
,m_nCeremonyApplyDuration(60*60*24)
,m_nCeremonyStartDuration(10*60)
{

}

CDanceGroupDataMgr::~CDanceGroupDataMgr()
{
	for (std::map<unsigned short, CDGLevelUpRequireRes * >::iterator itr = m_mapDanceGroupLevelUpRequireRes.begin(); itr != m_mapDanceGroupLevelUpRequireRes.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}

	m_mapDanceGroupLevelUpRequireRes.clear();
}

CDanceGroupDataMgr & CDanceGroupDataMgr::Instance()
{
	static CDanceGroupDataMgr s_DanceGroupMgr;

	return s_DanceGroupMgr;
}

void CDanceGroupDataMgr::AddDanceGroupInfo(unsigned short nMaxDanceGroupLevel, int nMemberLoginGroupHonorBonusLevelNeed, int nMemberLoginGropHonorBonus,
					   int nMinRoomDancersGroupHonorBonusNeed, int nMemberMatchBonusLeve1,
					   unsigned int nIncDGHonorNumPerDonatedResA, unsigned int nIncDGHonorNumPerDonatedResB, unsigned int nIncDGHonorNumPerDonatedResC,
					   unsigned int nIncMemberContributionNumPerDonatedResA, unsigned int nIncMemberContributionNumPerDonatedResB, unsigned int nIncMemberContributionNumPerDonatedResC)
{
	m_nMaxDanceGroupLevel = nMaxDanceGroupLevel;
	m_nMemberLoginGroupHonorBonusLevelNeed = nMemberLoginGroupHonorBonusLevelNeed;
	m_nMemberLoginGropHonorBonus = nMemberLoginGropHonorBonus;
	m_nMinRoomDancersGroupHonorBonusNeed = nMinRoomDancersGroupHonorBonusNeed;
	m_nMemberMatchBonusLeve1 = nMemberMatchBonusLeve1;

	m_nIncDGHonorNumPerDonatedResA = nIncDGHonorNumPerDonatedResA;
	m_nIncDGHonorNumPerDonatedResB = nIncDGHonorNumPerDonatedResB;
	m_nIncDGHonorNumPerDonatedResC = nIncDGHonorNumPerDonatedResC;

	m_nIncMemberContributionNumPerDonatedResA = nIncMemberContributionNumPerDonatedResA;
	m_nIncMemberContributionNumPerDonatedResB = nIncMemberContributionNumPerDonatedResB;
	m_nIncMemberContributionNumPerDonatedResC = nIncMemberContributionNumPerDonatedResC;
}

void CDanceGroupDataMgr::AddDGLevelUpRequireResInfo( CDGLevelUpRequireRes *pInfo )
{
	if (pInfo != NULL) {
		m_mapDanceGroupLevelUpRequireRes.insert(std::make_pair(pInfo->m_nGroupLevel, pInfo));
	}
}

CDGLevelUpRequireRes * CDanceGroupDataMgr::GetLevelUpRequireResBy( unsigned short nLevel )
{
	std::map<unsigned short, CDGLevelUpRequireRes * >::iterator itr = m_mapDanceGroupLevelUpRequireRes.find(nLevel);
	if (itr != m_mapDanceGroupLevelUpRequireRes.end() && itr->second != NULL) {
		return itr->second;
	}

	return NULL;
}

unsigned short CDanceGroupDataMgr::MaxDanceGroupLevel()
{
	return m_nMaxDanceGroupLevel;
}

int CDanceGroupDataMgr::MemberLoginGroupHonorBonusLevelNeed()
{
	return m_nMemberLoginGroupHonorBonusLevelNeed;
}

int CDanceGroupDataMgr::MemberLoginGropHonorBonus()
{
	return m_nMemberLoginGropHonorBonus;
}

int CDanceGroupDataMgr::MinRoomDancersGroupHonorBonusNeed()
{
	return m_nMinRoomDancersGroupHonorBonusNeed;
}

unsigned short CDanceGroupDataMgr::RoomGroupHonorBonusGeneID()
{
	return m_RoomGroupHonorBonusGeneID;
}

int CDanceGroupDataMgr::MemberMatchGroupHonorBonusLeve1()
{
	return m_nMemberMatchBonusLeve1;
}

unsigned int CDanceGroupDataMgr::IncDGHonorNumPerDonatedResA()
{
	return m_nIncDGHonorNumPerDonatedResA;
}

unsigned int CDanceGroupDataMgr::IncDGHonorNumPerDonatedResB()
{
	return m_nIncDGHonorNumPerDonatedResB;
}

unsigned int CDanceGroupDataMgr::IncDGHonorNumPerDonatedResC()
{
	return m_nIncDGHonorNumPerDonatedResC;
}

unsigned int CDanceGroupDataMgr::IncMemberContributionNumPerDonatedResA()
{
	return m_nIncMemberContributionNumPerDonatedResA;
}

unsigned int CDanceGroupDataMgr::IncMemberContributionNumPerDonatedResB()
{
	return m_nIncMemberContributionNumPerDonatedResB;
}

unsigned int CDanceGroupDataMgr::IncMemberContributionNumPerDonatedResC()
{
	return m_nIncMemberContributionNumPerDonatedResC;
}

void CDanceGroupDataMgr::SetCeremonyApplyInfo(unsigned char freeApplyTimes, unsigned char applyCostType, unsigned int applyCostNum)
{
    m_nCeremonyFreeApplyTimes = freeApplyTimes;
    m_nCeremonyApplyCostType = applyCostType;
    m_nCeremonyApplyCostNum = applyCostNum;

    

    return ;
}

void CDanceGroupDataMgr::SetCeremonyPraiseInfo(unsigned char freePraiseTime, unsigned char praiseCostType, 
    unsigned int praiseCostNum, unsigned int nPraiseCD)
{
    m_nCeremonyFreePraiseTimes = freePraiseTime;
    m_nCeremonyPraiseCostType = praiseCostType;
    m_nCeremonyPraiseCostNum = praiseCostNum;
    m_nCeremonyPraiseCD = nPraiseCD;

    return ;
}

void CDanceGroupDataMgr::SetCeremonyBaseInfo(unsigned int applyDuration, unsigned int startDuration, unsigned int nEnterNewGroupCD, unsigned int nEnterOldGroupCD)
{
    m_nCeremonyApplyDuration = applyDuration;
    m_nCeremonyStartDuration = startDuration;

    m_nEnterNewGroupCD = nEnterNewGroupCD;
    m_nEnterOldGroupCD = nEnterOldGroupCD;

    return ;
}


void CDanceGroupDataMgr::SetBaseInfo( const char* szMale, const char* szFemale,
											 unsigned short nNormalCount, int nNormalPower, int nNormalContribution,
											 unsigned short nMoneyCount, int nMoneyNum, int nMoneyPower, int nMoneyContribution,
											 unsigned short nPointCount, int nPointNum, int nPointPower, int nPointContribution,
											 string strRuleContent )
{
	m_maleList.clear();
	m_femaleList.clear();
	TransferItemList( szMale, m_maleList );
	TransferItemList( szFemale, m_femaleList );

	m_nActivityStarNormalCount = nNormalCount;
	m_nActivityStarNormalPower = nNormalPower;
	m_nActivityStarNormalContribution = nNormalContribution;

	m_nActivityStarMoneyCount = nMoneyCount;
	m_nActivityStarMoneyNum = nMoneyNum;
	m_nActivityStarMoneyPower = nMoneyPower;
	m_nActivityStarMoneyContribution = nMoneyContribution;

	m_nActivityStarPointCount = nPointCount;
	m_nActivityStarPointNum = nPointNum;
	m_nActivityStarPointPower = nPointPower;
	m_nActivityStarPointContribution = nPointContribution;

	m_strRuleContent = strRuleContent;
}//end

void CDanceGroupDataMgr::SetStarProgress( int nStarLevel, int nStarProgress, unsigned int nConLow, unsigned int nConMid, unsigned int nConHigh, const char *name )
{
    std::map<unsigned short, CDGActivityStarBaseInfo>::iterator iter = m_mStarProgress.find((unsigned short)nStarLevel);
	if( iter == m_mStarProgress.end() )
	{
		CDGActivityStarBaseInfo baseInfo;
        baseInfo.m_nStarLevel = (unsigned short)nStarLevel;
        baseInfo.m_nProgress = (unsigned short)nStarProgress;
		baseInfo.m_nContributionLow = nConLow;
		baseInfo.m_nContributionMid = nConMid;
		baseInfo.m_nContributionHigh = nConHigh;
		baseInfo.m_strName = name;
		m_mStarProgress.insert( make_pair( baseInfo.m_nStarLevel, baseInfo ) );
	}
}//end

void CDanceGroupDataMgr::AddStarPacket( unsigned short nDay, unsigned short nIndex, const char* szMale, const char* szFemale )
{
	//male
	std::map<unsigned short, DGStarDayItem>::iterator iter = m_mMaleItem.find( nDay );
	if( iter!= m_mMaleItem.end() )
	{
		std::list<CItem> tmpList;
		TransferItemList( szMale, tmpList );
		iter->second.insert( make_pair(nIndex, tmpList) );
	}
	else
	{
		DGStarDayItem dayItem;
		std::list<CItem> tmpList;
		TransferItemList( szMale, tmpList );
		dayItem.insert( make_pair( nIndex, tmpList ) );
		m_mMaleItem.insert( make_pair( nDay, dayItem ) );
	}
	
	
	//female
	iter = m_mFemaleItem.find( nDay );
	if( iter != m_mFemaleItem.end() )
	{
		std::list<CItem> tmpList;
		TransferItemList( szFemale, tmpList );
		iter->second.insert( make_pair( nIndex, tmpList ) );
	}
	else
	{
		DGStarDayItem dayItem;
		std::list<CItem> tmpList;
		TransferItemList( szFemale, tmpList );
		dayItem.insert( make_pair( nIndex, tmpList ) );
		m_mFemaleItem.insert( make_pair( nDay, dayItem ) );
	}

}//end

bool CDanceGroupDataMgr::GetPacketListByDay( DGStarDayItem &mList, unsigned char sex, unsigned short nDay )
{
	if( sex == ESexType_Male )
	{
		std::map<unsigned short, DGStarDayItem>::iterator iter = m_mMaleItem.find( nDay );
		if( iter != m_mMaleItem.end() )
		{
			DGStarDayItem::iterator i = iter->second.begin(), iEnd = iter->second.end();
			for( ; i != iEnd; ++i )
			{
				mList.insert( make_pair( i->first, i->second ) );
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( sex == ESexType_Female )
	{
		std::map<unsigned short, DGStarDayItem>::iterator iter = m_mFemaleItem.find( nDay );
		if( iter != m_mFemaleItem.end() )
		{
			DGStarDayItem::iterator i = iter->second.begin(), iEnd = iter->second.end();
			for( ; i != iEnd; ++i )
			{
				mList.insert( make_pair( i->first, i->second ) );
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return true;
}
void CDanceGroupDataMgr::AddDGShopItem( CDGShopItemInfo shopItemInfo, unsigned int nDayIndex )
{
    std::map<unsigned short, CDGShopInfo>::iterator iter = m_mDGShopInfo.find((unsigned short)nDayIndex);
	if( iter != m_mDGShopInfo.end() )
	{
		iter->second.m_dayItemList.insert( make_pair( shopItemInfo.m_nId, shopItemInfo ) );
	}
	else
	{
		CDGShopInfo shopInfo;
        shopInfo.m_nDayIndex = (unsigned short)nDayIndex;
		shopInfo.m_dayItemList.insert( make_pair( shopItemInfo.m_nId, shopItemInfo ) );

        m_mDGShopInfo.insert(make_pair(shopInfo.m_nDayIndex, shopInfo));
	}
}

CDGShopInfo& CDanceGroupDataMgr::GetDayShopInfo()
{/*
	int nDay = GetCurMonthDay(); //month for a circle
	nDay %= m_mDGShopInfo.size();
	nDay += 1;
*/
	int nDay = GetWeekDay(); //week for a circle

    return m_mDGShopInfo.find((unsigned short)nDay)->second;
}

std::list<CItem>& CDanceGroupDataMgr::GetRewardListBySex( ESexType eSex )
{
	if( eSex == ESexType_Male )
	{
		return m_maleList;
	}
	else
	{
		return m_femaleList;
	}
}

void CDanceGroupDataMgr::AddNewManPrize(CeremonyNewerPrize &prize)
{
    m_ceremonyNewerPrizeSorted.push_back(prize);
}

void CDanceGroupDataMgr::AddNormalPrize(CeremonyNormalPrize &prize)
{
    m_ceremonyNormalPrizeSorted.push_back(prize);
}


CeremonyNewerPrize* CDanceGroupDataMgr::GetCeremonyNewerPrize(int ranking)
{
    if ((size_t)ranking < m_ceremonyNewerPrizeSorted.size())
        return &(m_ceremonyNewerPrizeSorted[ranking]);

    return NULL;
}

CeremonyNormalPrize* CDanceGroupDataMgr::GetCeremonyNormalPrize(int maxPrizer)
{
    CeremonyNormalPrize* ret = NULL;
    for (size_t i = 0; i < m_ceremonyNormalPrizeSorted.size(); ++i)
    {
        ret = &(m_ceremonyNormalPrizeSorted[i]);
        if (maxPrizer <= (int)ret->m_nMaxPraiser)
            break;
    }

    return ret; // ×îºóµÄ
}



//end
