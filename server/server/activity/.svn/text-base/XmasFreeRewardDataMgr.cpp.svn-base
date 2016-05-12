#include "XmasFreeRewardDataMgr.h"
#include "../../socket/ParamPool.h"
#include "XmasFreeRewardActivity.h"
#include "SystemActivityManager.h"


CXmasFreeRewardInfo::CXmasFreeRewardInfo()
:m_nLevel(0)
,m_nMoney(0)
{

}

CXmasFreeRewardInfo::~CXmasFreeRewardInfo()
{

}

void CXmasFreeRewardInfo::doEncode( CParamPool & IOBuff, unsigned char nSex )
{
	IOBuff.AddUChar((unsigned char)m_nLevel);
	std::list<CItem> listItems;
	if (nSex == ESexType_Male) {
		listItems = m_listMaleReward;
	} else {
		listItems = m_listFemaleReward;	
	}
	
	IOBuff.AddUShort((unsigned short)listItems.size());
	for (std::list<CItem>::iterator itr = listItems.begin(); itr != listItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
}

//---------------------------------------------------------------CXmasFreeRewardDataMgr
CXmasFreeRewardDataMgr::CXmasFreeRewardDataMgr()
:m_bIsOpening(false)
,m_nActivityStartTime(0)
,m_nActivityEndTime(0)
{

}

CXmasFreeRewardDataMgr::~CXmasFreeRewardDataMgr()
{

}

CXmasFreeRewardDataMgr & CXmasFreeRewardDataMgr::Instance()
{
	static CXmasFreeRewardDataMgr s_XmaxFreeRewardDataMgr;

	return s_XmaxFreeRewardDataMgr;
}

bool CXmasFreeRewardDataMgr::IsOpening()
{
	return m_bIsOpening;
}

void CXmasFreeRewardDataMgr::SetIsOpening(bool isOpening)
{
	m_bIsOpening = isOpening;
}

void CXmasFreeRewardDataMgr::Init()
{
	CXmasFreeRewardActivity * pActivity = new CXmasFreeRewardActivity();
	pActivity->Init(m_nActivityStartTime, m_nActivityEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}

void CXmasFreeRewardDataMgr::AddFreeRewardActivityInfo(unsigned int nBeginTime, unsigned int nEndTime)
{
	m_nActivityStartTime = nBeginTime;
	m_nActivityEndTime = nEndTime;
}

void CXmasFreeRewardDataMgr::AddFreeRewardInfo( CXmasFreeRewardInfo *pRewardInfo )
{
	if (pRewardInfo != NULL) {
		m_mapFreeReward.insert(make_pair(pRewardInfo->m_nLevel, pRewardInfo));
	}
}

CXmasFreeRewardInfo * CXmasFreeRewardDataMgr::GetXmasFreeRewardInfo( int nLevel )
{
	std::map<int, CXmasFreeRewardInfo *>::iterator itr = m_mapFreeReward.find(nLevel);
	if (itr != m_mapFreeReward.end() && itr->second != NULL) {
		return itr->second;
	}

	return NULL;
}

const std::map<int, CXmasFreeRewardInfo *> & CXmasFreeRewardDataMgr::FreeRewardMap()
{
	return m_mapFreeReward;
}

unsigned int CXmasFreeRewardDataMgr::ActivityStartTime()
{
	return m_nActivityStartTime;
}

unsigned int CXmasFreeRewardDataMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

//void CXmasFreeRewardDataMgr::doEncode( CParamPool & IOBuff, unsigned char nSex )
//{
//	IOBuff.AddUInt(m_nActivityStartTime);
//	IOBuff.AddUInt(m_nActivityEndTime);
//	
//	IOBuff.AddUShort((unsigned short)m_mapFreeReward.size());
//	for (std::map<int, CXmasFreeRewardInfo *>::iterator itr = m_mapFreeReward.begin(); itr != m_mapFreeReward.end(); ++itr) {
//		if (itr->second != NULL) {
//			itr->second->doEncode(IOBuff, nSex);
//		}
//	}
//
//}









