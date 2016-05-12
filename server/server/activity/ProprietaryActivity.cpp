#include "ProprietaryActivity.h"
#include "SystemActivityManager.h"

void CProprietaryActivityShow::Init(time_t tBeginTime, time_t tEndTime)
{
	CTimeActivity::Init( tBeginTime, tEndTime );
}
void CProprietaryActivityShow::OnActivityBegin()
{
	CProprietaryActivityInfoMgr::Instance().SetisOpening( true );
}

void CProprietaryActivityShow::OnActivityEnd()
{
	CProprietaryActivityInfoMgr::Instance().SetisOpening( false );
}

//////////////////////////////////////////////
CProprietaryActivityInfoMgr::CProprietaryActivityInfoMgr()
:m_bIsOpening( false )
,m_bExhibit( false )
,m_nWeight( 0 )
{
	m_strRegularImageName = "";
	m_strThumbnailName = "";
	m_strActivityTitle = "";
	m_strActivityContent = "";
	m_nShowBeginTime = 0;
	m_nShowEndTime = 0;
}

void CProprietaryActivityInfoMgr::Init()
{
	CProprietaryActivityShow* pActivity = new CProprietaryActivityShow();
	pActivity->Init( m_nShowBeginTime, m_nShowEndTime );
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity( pActivity );
}

void CProprietaryActivityInfoMgr::AddInfo( bool bExhibit, int nWeight,
			 std::string& strRegularImageName, std::string& strThumbnailName,
			 std::string& strActivityTitle, std::string& strActivityContent,
			 unsigned int nShowBeginTime, unsigned int nShowEndTime,
			 std::list<CItem>& maleItemList, std::list<CItem>& femaleItemList, int nMoney )
{
	m_reward.m_nMoney = nMoney;
	m_reward.m_listMaleReward = maleItemList;
	m_reward.m_listFemaleReward = femaleItemList;

	m_bExhibit = bExhibit;
	m_nWeight = nWeight;
	m_strRegularImageName = strRegularImageName;
	m_strThumbnailName = strThumbnailName;
	m_strActivityTitle = strActivityTitle;
	m_strActivityContent = strActivityContent;
	m_nShowBeginTime = nShowBeginTime;
	m_nShowEndTime = nShowEndTime;
}
