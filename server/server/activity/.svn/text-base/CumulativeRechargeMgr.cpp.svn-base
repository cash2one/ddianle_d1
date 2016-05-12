#include "CumulativeRechargeMgr.h"
#include "../util/CommonFunc.h"
#include "CumulativeRechargeActivity.h"
#include "SystemActivityManager.h"


CCumulativeReward::CCumulativeReward()
:m_nLevel(0)
,m_nMoney(0)
,m_nBindMCoin(0)
{

}

CCumulativeReward::~CCumulativeReward()
{

}

void CCumulativeReward::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_listMaleReward.size());
	for (list<CItem>::iterator itr = m_listMaleReward.begin(); itr != m_listMaleReward.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)m_listFemaleReward.size());
	for (list<CItem>::iterator itr = m_listFemaleReward.begin(); itr != m_listFemaleReward.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
}

CCumulativeRechargeMgr::CCumulativeRechargeMgr()
:m_bIsOpening(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumblnailsName("")
,m_strActivityName("")
,m_strContent("")
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_nRechargeStartTime(0)
,m_nRechargeStopTime(0)
,m_strRechargeStartAnnouce("")
,m_strRechargeStopAnnouce("")
{

}

CCumulativeRechargeMgr::~CCumulativeRechargeMgr()
{
	for(CumulativeReward_t::iterator itr = m_mapRewards.begin(); itr != m_mapRewards.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}
	m_mapRewards.clear();
}

CCumulativeRechargeMgr & CCumulativeRechargeMgr::Instance()
{
	static CCumulativeRechargeMgr s_CumulativeMgr;

	return s_CumulativeMgr;
}

bool CCumulativeRechargeMgr::Init()
{
	if (m_nActivityBeginTime >= m_nActivityEndTime || m_nRechargeStartTime >= m_nRechargeStopTime || m_nRechargeStartTime < m_nActivityBeginTime) {
		return false;
	}

	CCummulativeRechargeActivity * pActivity = new CCummulativeRechargeActivity();
	pActivity->Init(m_nActivityBeginTime, m_nActivityEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
	
	return true;
}

void CCumulativeRechargeMgr::AddCumulativeBriefInfo(bool bExhibit,int nWeight, 
													std::string & rStrRegularImageName, std::string & rStrThumblenailsName,
													std::string & rStrActivityName, std::string & rStrContent, 
													std::string & rActivityBeginTime, std::string & rActivityEndTime, 
													std::string & rStrRechargeStartTime, std::string & rStrRechargeStopTime, 
													std::string & rStrRechargeStartAnnounce, std::string & rStrRechargeStopAnnounce)
{
	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = rStrRegularImageName;
	m_strThumblnailsName = rStrThumblenailsName;
	m_strActivityName = rStrActivityName;
	m_strContent = rStrContent;
	m_nActivityBeginTime = (unsigned int)SetTime(rActivityBeginTime.c_str());
	m_nActivityEndTime = (unsigned int)SetTime(rActivityEndTime.c_str());
	m_nRechargeStartTime = (unsigned int)SetTime(rStrRechargeStartTime.c_str());
	m_nRechargeStopTime = (unsigned int)SetTime(rStrRechargeStopTime.c_str());
	m_strRechargeStartAnnouce = rStrRechargeStartAnnounce;
	m_strRechargeStopAnnouce = rStrRechargeStopAnnounce;
}

void CCumulativeRechargeMgr::AddCummulativeRechargeInfo(CCumulativeReward & rewward)
{
	CCumulativeReward *pReward = new CCumulativeReward(rewward);	

	if (pReward != NULL) {
		m_mapRewards.insert(make_pair(rewward.m_nLevel, pReward));
	}
}

bool CCumulativeRechargeMgr::IsOpening()
{
	return m_bIsOpening;
}

void CCumulativeRechargeMgr::SetIsOpening( bool isOpening )
{
	m_bIsOpening = isOpening;
}

bool CCumulativeRechargeMgr::Exhibit()
{
	return m_bExhibit;
}

int CCumulativeRechargeMgr::Weight()
{
	return m_nWeight;
}

const std::string & CCumulativeRechargeMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CCumulativeRechargeMgr::ThumbnailsName()
{
	return m_strThumblnailsName;
}

const std::map< int,  CCumulativeReward* > & CCumulativeRechargeMgr::RewardMap()
{
	return m_mapRewards;
}

std::string & CCumulativeRechargeMgr::ActivityName()
{
	return m_strActivityName;
}

std::string & CCumulativeRechargeMgr::Detail()
{
	return m_strContent;
}

unsigned int CCumulativeRechargeMgr::ActivityBeginTime()
{
	return m_nActivityBeginTime;	
}

unsigned int CCumulativeRechargeMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

unsigned int CCumulativeRechargeMgr::RechargeStartTime()
{
	return m_nRechargeStartTime;
}

unsigned int CCumulativeRechargeMgr::RechargeStopTime()
{
	return m_nRechargeStopTime;
}

bool CCumulativeRechargeMgr::IsValidLevel( int nLevel )
{
	return  m_mapRewards.find(nLevel) != m_mapRewards.end();
}

bool CCumulativeRechargeMgr::IsValidRechargeTime(time_t nTime)
{
	return nTime >= (time_t)m_nRechargeStartTime && nTime <= (time_t)m_nRechargeStopTime;
}

const CCumulativeReward * CCumulativeRechargeMgr::GetRewardInfo( int nLevel )
{
	CumulativeReward_t::iterator itr = m_mapRewards.find(nLevel);
	if (itr != m_mapRewards.end()) {
		return itr->second;
	}
	
	return NULL;
}

