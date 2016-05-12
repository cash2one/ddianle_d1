#include "IEntityAchivementComponent.h"

IEntityAchievementComponent::IEntityAchievementComponent()
{
	m_nScore = 0;
	// 物品相关
	m_nHistoryClothNum = 0;
	m_nHistoryWishItemNum = 0;					
	m_nGainWishItemNum = 0;
	m_nHistoryBadageNum = 0;
	// 比赛成绩相关
	m_nHistoryTotalScore = 0;
	m_nNumWinBetween2To4Persons = 0;
	m_nNumWinIn4Persons = 0;
	m_nHistoryTaikoComboMusicNum = 0;
	m_nTaikoComboNum = 0;
	m_nAutoMatchWithOppositeSexNum = 0;
	m_nHistoryUseBouquetNum = 0;
}

IEntityAchievementComponent::~IEntityAchievementComponent()
{
}

unsigned int IEntityAchievementComponent::GetTotalScore() const
{
	unsigned int nTotalScore = 0;
	map<int, map<int, CFightResult> >::const_iterator itr = m_mapFightResult.begin();	// per mode
	for (; itr != m_mapFightResult.end(); itr++)
	{ 
		map< int, CFightResult >::const_iterator itr2 = itr->second.begin();
		for (; itr2 != itr->second.end(); itr2++)	// per rank
		{
			nTotalScore  += itr2->second.m_nHistoryTotalScore;
		}
	}

	return nTotalScore;
}

unsigned int IEntityAchievementComponent::GetSmallSpeakersUseCountInSevenDays() const
{
	return m_SmallSpeakersUseCountWeekly.m_nCount;
}

unsigned int IEntityAchievementComponent::GetBigSpeakersUseCountInSevenDays() const
{
	return m_BigSpeakersUseCountWeekly.m_nCount;
}

unsigned int IEntityAchievementComponent::GetSendFlowersCountInSevenDays() const
{
	return m_SendFlowersCountWeekly.m_nCount;
}


// end of file


