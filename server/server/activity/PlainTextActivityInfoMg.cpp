#include "PlainTextActivityInfoMgr.h"
#include "PlainTextActivity.h"
#include "SystemActivityManager.h"


//////////////////////////////////////////
CPlainTextActivityReward::CPlainTextActivityReward()
:m_strRewardName("")
,m_nMoney(0)
{

}

CPlainTextActivityReward::~CPlainTextActivityReward()
{

}

//////////////////////////////////////////
CPlainTextActivityInfoMgr::CPlainTextActivityInfoMgr()
:m_bIsOpening(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailName("")
,m_strActivityTitle("")
,m_strActivityContent("")
,m_nShowBeginTime(0)
,m_nShowEndTime(0)
{

}

CPlainTextActivityInfoMgr::~CPlainTextActivityInfoMgr()
{

}

CPlainTextActivityInfoMgr & CPlainTextActivityInfoMgr::Instance()
{
	static CPlainTextActivityInfoMgr s_InfoBoardMgr;

	return s_InfoBoardMgr;
}

void CPlainTextActivityInfoMgr::Init()
{
	CPlainTextActivityShow * pActivity = new CPlainTextActivityShow();
	pActivity->Init(m_nShowBeginTime, m_nShowEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}


void CPlainTextActivityInfoMgr::AddRewardInfo( std::string & strRewardName, std::list<CItem> & listMaleItem, std::list<CItem> & listFemale, int nMoney )
{
	CPlainTextActivityReward reward;
	reward.m_strRewardName = strRewardName;
	reward.m_listMaleReward = listMaleItem;
	reward.m_listFemaleReward = listFemale;
	reward.m_nMoney = nMoney;

	m_rewards.push_back(reward);
}

void CPlainTextActivityInfoMgr::AddBriefInfo( bool bExhibit, int nWeight, 
								 std::string & strRegularImageName, std::string & strThumbnailName, 
								 std::string & strActivityTitle, std::string & strActivityContent, 
								 unsigned int nShowBeginTime, unsigned int nShowEndTime )
{
	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = strRegularImageName;
	m_strThumbnailName = strThumbnailName;
	m_strActivityTitle= strActivityTitle;
	m_strActivityContent = strActivityContent;
	m_nShowBeginTime = nShowBeginTime;
	m_nShowEndTime = nShowEndTime;
}

bool CPlainTextActivityInfoMgr::IsOpening()
{
	return m_bIsOpening;
}

void CPlainTextActivityInfoMgr::SetIsOpening( bool isOpening )
{
	m_bIsOpening = isOpening;
}

bool CPlainTextActivityInfoMgr::Exhibit()
{
	return m_bExhibit;
}

int CPlainTextActivityInfoMgr::Weight()
{
	return m_nWeight;
}

const std::string & CPlainTextActivityInfoMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CPlainTextActivityInfoMgr::ThumbnailsName()
{
	return m_strThumbnailName;
}

const std::string & CPlainTextActivityInfoMgr::ActivityTitle()
{
	return m_strActivityTitle;
}

const std::string & CPlainTextActivityInfoMgr::ActivityContent()
{
	return m_strActivityContent;
}

const PlainTextRewardContainer_t & CPlainTextActivityInfoMgr::Rewards()
{
	return m_rewards;
}






