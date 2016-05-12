#include "FestivalBlessActivityDataMgr.h"



//-----------------------------------------------CFestivalBlessActivityDataMgr
CFestivalBlessActivityDataMgr::CFestivalBlessActivityDataMgr()
:m_bIsShowOpening(false)
,m_bIsBlessOpening(false)
,m_nShowActivityBeginTime(0)
,m_nShowActivityEndTime(0)
,m_nBlessBeginTime(0)
,m_nBlessEndTime(0)
,m_nBlessRequireItemType(0)
{

}

CFestivalBlessActivityDataMgr::~CFestivalBlessActivityDataMgr()
{

}

CFestivalBlessActivityDataMgr & CFestivalBlessActivityDataMgr::Instance()
{
	static CFestivalBlessActivityDataMgr s_festivalBlessActivityDataMgr;

	return s_festivalBlessActivityDataMgr;
}

bool CFestivalBlessActivityDataMgr::Init()
{
	return true;
}

void CFestivalBlessActivityDataMgr::AddBlessActivityInfo( unsigned int nShowBeginTime, unsigned int nShowEndTime, 
														  unsigned int nBlessBeginTime, unsigned int nBlessEndTime, 
														  itemtype_t nBlessRequireItemType )
{
	m_nShowActivityBeginTime = nShowBeginTime;
	m_nShowActivityEndTime = nShowEndTime;
	m_nBlessBeginTime = nBlessBeginTime;
	m_nBlessEndTime = nBlessEndTime;
	m_nBlessRequireItemType = nBlessRequireItemType; 
}

void CFestivalBlessActivityDataMgr::AddBlessWords( CFestivalBlessWords *pBlessWords )
{
	if (pBlessWords != NULL) {
		m_mapFestivalBlessWords.insert(make_pair(pBlessWords->m_nIndex, pBlessWords));
	}
}

void CFestivalBlessActivityDataMgr::AddBlessRewards( CFestivalBlessedReward *pBlessReward )
{
	if (pBlessReward != NULL) {
		m_mapFestivalBlessedReward.insert(make_pair(pBlessReward->m_nLevel, pBlessReward));
	}
}

void CFestivalBlessActivityDataMgr::AddSendBlessReward(CFestivalSendBlessReward & rSendBlessReward)
{
	m_SendBlessReward = rSendBlessReward;
}

CFestivalSendBlessReward & CFestivalBlessActivityDataMgr::SendBlessReward()
{
	return m_SendBlessReward;
}

bool CFestivalBlessActivityDataMgr::IsShowOpening()
{
	unsigned int nTimeNow = (unsigned int)time(NULL);
	return nTimeNow >= m_nShowActivityBeginTime && nTimeNow < m_nShowActivityEndTime;
}

void CFestivalBlessActivityDataMgr::SetIsShowOpening( bool bIsShowingOpening )
{
	m_bIsShowOpening = bIsShowingOpening;
}

bool CFestivalBlessActivityDataMgr::IsBlessOpening()
{
	unsigned int nTimeNow = (unsigned int)time(NULL);
	return nTimeNow >= m_nBlessBeginTime && nTimeNow < m_nBlessEndTime;
}

void CFestivalBlessActivityDataMgr::SetIsBlessOpening( bool bIsBlessOpening )
{
	m_bIsBlessOpening = bIsBlessOpening;
}

unsigned int CFestivalBlessActivityDataMgr::BlessBeginTime()
{
	return m_nBlessBeginTime;
}

unsigned int CFestivalBlessActivityDataMgr::BlessEndTime()
{
	return m_nBlessEndTime;
}

itemtype_t CFestivalBlessActivityDataMgr::BlessRequireItem()
{
	return m_nBlessRequireItemType;
}

CFestivalBlessWords * CFestivalBlessActivityDataMgr::GetFestivalBlessWord( int nIndex )
{
	std::map<int, CFestivalBlessWords*>::iterator itr = m_mapFestivalBlessWords.find(nIndex);
	if (itr != m_mapFestivalBlessWords.end() && itr->second != NULL) {
		return itr->second;
	}

	return NULL;
}

CFestivalBlessedReward * CFestivalBlessActivityDataMgr::GetFestivalBlessReward( int nLevel )
{
	std::map<int, CFestivalBlessedReward*>::iterator itr = m_mapFestivalBlessedReward.find(nLevel);
	if (itr != m_mapFestivalBlessedReward.end() && itr->second != NULL) {
		return itr->second;
	}

	return NULL;
}

const std::map<int, CFestivalBlessedReward*> & CFestivalBlessActivityDataMgr::FestivalBlessRewardMap()
{
	return m_mapFestivalBlessedReward;
}

const std::map<int, CFestivalBlessWords*> & CFestivalBlessActivityDataMgr::FestivalBlessWordsMap()
{
	return m_mapFestivalBlessWords;
}













