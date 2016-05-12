#include "ExchangeItemInfoMgr.h"
#include "../util/CommonFunc.h"
#include "ExchangeItemActivity.h"
#include "SystemActivityManager.h"


CExchangeItemInfo::CExchangeItemInfo()
:m_nLevel(0)
,m_nRequiredItemNum(0)
,m_nMoney(0)
{

}

CExchangeItemInfo::~CExchangeItemInfo()
{

}

CExchangeItemInfoMgr::CExchangeItemInfoMgr()
:m_bIsOpening(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_nRequiredItemType(0)
,m_nExhibitBeginTime(0)
,m_nExhibitEndTime(0)
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_strActivityName("")
,m_strActivityIntro("")
,m_strActivityBeginAnnounce("")
,m_strActivityEndAnnouce("")
{

}

CExchangeItemInfoMgr::~CExchangeItemInfoMgr()
{
	for (ExchangeItem_t::iterator itr = m_mapExchangeInfo.begin(); itr != m_mapExchangeInfo.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}

	m_mapExchangeInfo.clear();
}

CExchangeItemInfoMgr & CExchangeItemInfoMgr::Instance()
{
	static CExchangeItemInfoMgr s_ExchangeItemInfoMgr;

	return s_ExchangeItemInfoMgr;
}

void CExchangeItemInfoMgr::Init()
{
	CExchangeItemActivity * pActivity = new CExchangeItemActivity();
	pActivity->Init(m_nExhibitBeginTime, m_nExhibitEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}

void CExchangeItemInfoMgr::AddActivityBriefInfo( bool bExhibit, int nWeight, 
												 std::string & rStrRegularImageName, std::string & rStrThumbnailsName,
												 itemtype_t nItemType, 
												 std::string & rStrExhibitBeginTime, std::string & rStrExhibitEndTime,
												 std::string & rStrExchangeBeginTime, std::string & rStrExchangeEndTime, 
												 std::string & rStrActivityName, std::string & rStrActivityIntro, 
												 std::string & rStrActivityBeginAnnouce, std::string & rStrActivityEndAnnounce )
{
	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = rStrRegularImageName;
	m_strThumbnailsName = rStrThumbnailsName;
	m_nRequiredItemType = nItemType;
	m_nExhibitBeginTime = SetTime(rStrExhibitBeginTime.c_str());
	m_nExhibitEndTime = SetTime(rStrExhibitEndTime.c_str());
	m_nActivityBeginTime = SetTime(rStrExchangeBeginTime.c_str());
	m_nActivityEndTime = SetTime(rStrExchangeEndTime.c_str());
	m_strActivityName = rStrActivityName;
	m_strActivityIntro = rStrActivityIntro;
	m_strActivityBeginAnnounce = rStrActivityBeginAnnouce;
	m_strActivityEndAnnouce = rStrActivityEndAnnounce;
}

void CExchangeItemInfoMgr::AddExchangeItemInfo( CExchangeItemInfo & exchangeItemInfo )
{
	CExchangeItemInfo *pExchangeInfo = new CExchangeItemInfo(exchangeItemInfo);
	if (pExchangeInfo != NULL) {
		m_mapExchangeInfo.insert(make_pair(pExchangeInfo->m_nLevel, pExchangeInfo));
	}
}

void CExchangeItemInfoMgr::SetIsOpening(bool isOpening)
{
	m_bIsOpening = isOpening;
}

bool CExchangeItemInfoMgr::IsOpening()
{
	return m_bIsOpening;
}

bool CExchangeItemInfoMgr::IsExchangeItemOpen()
{
	time_t nNow = time(NULL);

	return nNow >= m_nActivityBeginTime && nNow <= m_nActivityEndTime;
}


itemtype_t CExchangeItemInfoMgr::RequiredItemType()
{
	return m_nRequiredItemType;
}

bool CExchangeItemInfoMgr::Exhibit()
{
	return m_bExhibit;
}

int CExchangeItemInfoMgr::Weight()
{
	return m_nWeight;
}

const std::string & CExchangeItemInfoMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CExchangeItemInfoMgr::ThumbnailsName()
{
	return m_strThumbnailsName;
}

time_t CExchangeItemInfoMgr::ExhibitBeginTime()
{
	return m_nExhibitBeginTime;
}

time_t CExchangeItemInfoMgr::ExhibitEndTime()
{
	return m_nExhibitEndTime;
}

time_t CExchangeItemInfoMgr::ActivityStartTime()
{
	return m_nActivityBeginTime;
}

time_t CExchangeItemInfoMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

std::string & CExchangeItemInfoMgr::ActivityName()
{
	return m_strActivityName;
}

std::string & CExchangeItemInfoMgr::ActivityIntro()
{
	return m_strActivityIntro;
}

std::string & CExchangeItemInfoMgr::ActivityStartAnnouce()
{
	return m_strActivityBeginAnnounce;
}

std::string & CExchangeItemInfoMgr::ActivityEndAnnouce()
{
	return m_strActivityEndAnnouce;
}

const ExchangeItem_t & CExchangeItemInfoMgr::ExchangeInfoMap()
{
	return m_mapExchangeInfo;
}

bool CExchangeItemInfoMgr::IsValidLevel( int nLevel )
{
	return  m_mapExchangeInfo.find(nLevel) != m_mapExchangeInfo.end();
}

const CExchangeItemInfo * CExchangeItemInfoMgr::GetExchangeItemInfo( int nLevel )
{
	ExchangeItem_t::iterator itr = m_mapExchangeInfo.find(nLevel);
	if (itr != m_mapExchangeInfo.end()) {
		return itr->second;
	}

	return NULL;
}







