#include "InTimeOnlineInfoMgr.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"
#include "InTimeOnlineActivity.h"
#include "SystemActivityManager.h"


CInTimeActivityInfo::CInTimeActivityInfo()
:m_nActivityIndex(0)
,m_tActivityBeginTime(0)
,m_tActivityEndTime(0)
,m_nTriggerHour(0)
,m_nTriggerMin(0)
,m_nMoney(0)
,m_strMailTitle("")
,m_strMailContent("")
{

}

CInTimeActivityInfo::~CInTimeActivityInfo()
{

}

void CInTimeActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt((unsigned int)m_tActivityBeginTime);
	IOBuff.AddUInt((unsigned int)m_tActivityEndTime);
	IOBuff.AddInt(m_nTriggerHour);
	IOBuff.AddInt(m_nTriggerMin);

	IOBuff.AddUShort((unsigned short)m_listMaleItems.size());
	for (std::list<CItem>::iterator itr = m_listMaleItems.begin(); itr != m_listMaleItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)m_listFemaleItems.size());
	for (std::list<CItem>::iterator itr = m_listFemaleItems.begin(); itr != m_listFemaleItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
}

//////////////////////////////////////////////////////////////////////////////////////
CInTimeOnlineInfoMgr::CInTimeOnlineInfoMgr()
:m_bIsShowOpening(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumblnailsName("")
,m_strActivityName("")
,m_strActivityDetail("")
,m_tShowTimeBegin(0)
,m_tShowTimeEnd(0)
,m_strActivityBeginAnnounce("")
,m_strActivityEndAnnounce("")
{

}

CInTimeOnlineInfoMgr::~CInTimeOnlineInfoMgr()
{
    while ( !m_listActivities.empty() )
    {
        delete m_listActivities.front();
        m_listActivities.pop_front();
    }
}

CInTimeOnlineInfoMgr & CInTimeOnlineInfoMgr::Instance()
{
	static CInTimeOnlineInfoMgr s_InTimeOnlineInfoMgr;

	return s_InTimeOnlineInfoMgr;
}

void CInTimeOnlineInfoMgr::Init()
{
	CShowInTimeOnlineActivityInfo * pActivity = new CShowInTimeOnlineActivityInfo();
	pActivity->Init(m_tShowTimeBegin, m_tShowTimeEnd);
	m_bIsShowOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}

void CInTimeOnlineInfoMgr::AddActivity(int nActivityIndex, const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime,
									   const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward, 
									   const char * szTitle, const char * szContent)
{
	CInTimeActivityInfo *pActivityInfo = new CInTimeActivityInfo;
	if (pActivityInfo != NULL) {

		int nHour = 0;
		int nMinute = 0;
		sscanf(szTriggeringTime, "%2d:%2d", &nHour, &nMinute);
		list<CItem> listMaleItem;
		if (!TransferItemList(szMaleReward, listMaleItem))
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CInTimeOnlineInfoMgr::AddActivity. Transfer male item list error. ActivityID=%d, itemlist=%s", nActivityIndex, szMaleReward);
		}
		list<CItem> listFemale;
		if (!TransferItemList(szFemaleReward, listFemale))
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CInTimeOnlineInfoMgr::AddActivity. Transfer female item list error. ActivityID=%d, itemlist=%s", nActivityIndex, szFemaleReward);
		}

		pActivityInfo->m_nActivityIndex = nActivityIndex;
		pActivityInfo->m_tActivityBeginTime = SetTime(szBeginTime);
		pActivityInfo->m_tActivityEndTime = SetTime(szEndTime);
		pActivityInfo->m_nTriggerHour = nHour;
		pActivityInfo->m_nTriggerMin = nMinute;
		pActivityInfo->m_listMaleItems = listMaleItem;
		pActivityInfo->m_listFemaleItems = listFemale;
		pActivityInfo->m_nMoney = nMoneyReward;
		pActivityInfo->m_strMailTitle = szTitle;
		pActivityInfo->m_strMailContent = szContent;

		m_listActivities.push_back(pActivityInfo);
	}
}

void CInTimeOnlineInfoMgr::AddBriefInfo(bool bExhibit, int nWeight,
										std::string & rStrRegularImageName, std::string & rStrThumblnailName, 
										std::string & rStrActivityName, std::string & rStrDetail,
										time_t tShowTimeBegin, time_t tShowTimeEnd,
										std::string & rStrShowBeginAnnounce,
										std::string & rStrShowEndAnnounce)
{
	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = rStrRegularImageName;
	m_strThumblnailsName = rStrThumblnailName;
	m_strActivityName = rStrActivityName;
	m_strActivityDetail = rStrDetail;
	m_tShowTimeBegin = tShowTimeBegin;
	m_tShowTimeEnd = tShowTimeEnd;
	m_strActivityBeginAnnounce = rStrShowBeginAnnounce;
	m_strActivityEndAnnounce = rStrShowEndAnnounce;
}

void CInTimeOnlineInfoMgr::SetIsShowOpening(bool bIsOpening)
{
	m_bIsShowOpening = bIsOpening;
}

bool CInTimeOnlineInfoMgr::IsShowOpening()
{
	return m_bIsShowOpening;
}

bool CInTimeOnlineInfoMgr::Exhibit()
{
	return m_bExhibit;
}

int CInTimeOnlineInfoMgr::Weight()
{
	return m_nWeight;
}

const std::string & CInTimeOnlineInfoMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CInTimeOnlineInfoMgr::ThumblnailsName()
{
	return m_strThumblnailsName;
}

const std::string & CInTimeOnlineInfoMgr::ActivityName()
{
	return m_strActivityName;
}

const std::string & CInTimeOnlineInfoMgr::ActivityDetail()
{
	return m_strActivityDetail;
}

time_t CInTimeOnlineInfoMgr::ShowTimeBegin()
{
	return m_tShowTimeBegin;
}

time_t CInTimeOnlineInfoMgr::ShowTimeEnd()
{
	return m_tShowTimeEnd;
}

const std::string & CInTimeOnlineInfoMgr::ActivityBeginAnnounce()
{
	return m_strActivityBeginAnnounce;
}

const std::string & CInTimeOnlineInfoMgr::ActivityEndAnnounce()
{
	return m_strActivityEndAnnounce;
}

const InTimeActivityInfo_t & CInTimeOnlineInfoMgr::Activities()
{
	return m_listActivities;
}

