#include "GiftCodeInfoMgr.h"
#include "GiftCodeActivityShow.h"
#include "SystemActivityManager.h"


CGiftCodeInfoMgr::CGiftCodeInfoMgr()
:m_bIsShowing(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_tShowBeginTime(0)
,m_tShowEndTime(0)
,m_tActivityBeginTime(0)
,m_tActivityEndTime(0)
{

}

CGiftCodeInfoMgr::~CGiftCodeInfoMgr()
{

}

CGiftCodeInfoMgr & CGiftCodeInfoMgr::Instance()
{
	static CGiftCodeInfoMgr s_giftCodeInfoMgr;

	return s_giftCodeInfoMgr;
}

void CGiftCodeInfoMgr::AddBriefInfo( bool bExhibit, int nWeight, 
									std::string & strRegularImageName, std::string & strThumbnailsName, 
									std::string & strActivityTitle, std::string & strActivityContent,
									time_t tShowBeginTime, time_t tShowEndTime, 
									time_t tActivityBeginTime, time_t tActivityEndTime )
{
	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = strRegularImageName;
	m_strThumbnailsName = strThumbnailsName;
	m_strActivityTitle = strActivityTitle;
	m_strActivityContent = strActivityContent;
	m_tShowBeginTime = tShowBeginTime;
	m_tShowEndTime = tShowEndTime;
	m_tActivityBeginTime = tActivityBeginTime;
	m_tActivityEndTime = tActivityEndTime;
}

void CGiftCodeInfoMgr::Init()
{
	CGiftCodeActivityShow * pActivity = new CGiftCodeActivityShow();
	pActivity->Init(m_tShowBeginTime, m_tShowEndTime);
	m_bIsShowing = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}

bool CGiftCodeInfoMgr::IsActivityOpening()
{
	time_t nNow = time(NULL);
	
	return nNow >= m_tActivityBeginTime && nNow <= m_tActivityEndTime; 
}

void CGiftCodeInfoMgr::SetIsShowing( bool isShowing )
{
	m_bIsShowing = isShowing;
}

bool CGiftCodeInfoMgr::IsShowing()
{
	return m_bIsShowing;
}

bool CGiftCodeInfoMgr::Exhibit()
{
	return m_bExhibit;
}

int CGiftCodeInfoMgr::Weight()
{
	return m_nWeight;
}

const std::string & CGiftCodeInfoMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CGiftCodeInfoMgr::ThumbnailsName()
{
	return m_strThumbnailsName;
}

time_t CGiftCodeInfoMgr::ActivityStartTime()
{
	return m_tActivityBeginTime;
}

time_t CGiftCodeInfoMgr::ActivityEndTime()
{
	return m_tActivityEndTime;
}

std::string & CGiftCodeInfoMgr::ActivityTitle()
{
	return m_strActivityTitle;
}

std::string & CGiftCodeInfoMgr::ActivityContent()
{
	return m_strActivityContent;
}









