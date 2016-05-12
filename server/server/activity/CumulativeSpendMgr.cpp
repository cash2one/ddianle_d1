#include "CumulativeSpendMgr.h"
#include "../util/CommonFunc.h"
#include "CumulativeSpendActivity.h"
#include "SystemActivityManager.h"
#include "../mall/CurrencyType.h"


CCumulativeSpendReward::CCumulativeSpendReward()
:m_nLevel(0)
,m_nMoney(0)
,m_nBindMCoin(0)
{

}

CCumulativeSpendReward::~CCumulativeSpendReward()
{

}

void CCumulativeSpendReward::doEncode( CParamPool & IOBuff )
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

CCumulativeSpendMgr::CCumulativeSpendMgr()
:m_bIsOpening(false)
,m_bExhibit(false)
,m_nCurrencyType(EMallCurrencyType_MCoin)
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

CCumulativeSpendMgr::~CCumulativeSpendMgr()
{
	for(CCumulativeSpendReward_t::iterator itr = m_mapRewards.begin(); itr != m_mapRewards.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}
	m_mapRewards.clear();
}

CCumulativeSpendMgr & CCumulativeSpendMgr::Instance()
{
	static CCumulativeSpendMgr s_CumulativeMgr;

	return s_CumulativeMgr;
}

bool CCumulativeSpendMgr::Init()
{
	if (m_nActivityBeginTime >= m_nActivityEndTime || m_nRechargeStartTime >= m_nRechargeStopTime || m_nRechargeStartTime < m_nActivityBeginTime) {
		return false;
	}

	CCummulativeSpendActivity * pActivity = new CCummulativeSpendActivity();
	pActivity->Init(m_nActivityBeginTime, m_nActivityEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
	
	return true;
}

void CCumulativeSpendMgr::AddCumulativeSpendInfo(bool bExhibit, int nCurrencyType, int nWeight, 
													std::string & rStrRegularImageName, std::string & rStrThumblenailsName,
													std::string & rStrActivityName, std::string & rStrContent, 
													std::string & rActivityBeginTime, std::string & rActivityEndTime, 
													std::string & rStrRechargeStartTime, std::string & rStrRechargeStopTime, 
													std::string & rStrRechargeStartAnnounce, std::string & rStrRechargeStopAnnounce)
{
	m_bExhibit = bExhibit;
	m_nCurrencyType = nCurrencyType;
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

void CCumulativeSpendMgr::AddCummulativeSpendRewardInfo(CCumulativeSpendReward & rewward)
{
	CCumulativeSpendReward *pReward = new CCumulativeSpendReward(rewward);	

	if (pReward != NULL) 
	{
		m_mapRewards.insert(make_pair(rewward.m_nLevel, pReward));
	}
}

bool CCumulativeSpendMgr::IsOpening()
{
	return m_bIsOpening;
}

void CCumulativeSpendMgr::SetIsOpening( bool isOpening )
{
	m_bIsOpening = isOpening;
}

bool CCumulativeSpendMgr::Exhibit()
{
	return m_bExhibit;
}

int CCumulativeSpendMgr::Weight()
{
	return m_nWeight;
}

const std::string & CCumulativeSpendMgr::RegularImageName()
{
	return m_strRegularImageName;
}

const std::string & CCumulativeSpendMgr::ThumbnailsName()
{
	return m_strThumblnailsName;
}

const std::map< int,  CCumulativeSpendReward* > & CCumulativeSpendMgr::RewardMap()
{
	return m_mapRewards;
}

std::string & CCumulativeSpendMgr::ActivityName()
{
	return m_strActivityName;
}

std::string & CCumulativeSpendMgr::Detail()
{
	return m_strContent;
}

unsigned int CCumulativeSpendMgr::ActivityBeginTime()
{
	return m_nActivityBeginTime;	
}

unsigned int CCumulativeSpendMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

unsigned int CCumulativeSpendMgr::SpendStartTime()
{
	return m_nRechargeStartTime;
}

unsigned int CCumulativeSpendMgr::SpendStopTime()
{
	return m_nRechargeStopTime;
}

bool CCumulativeSpendMgr::IsValidLevel( int nLevel )
{
	return  m_mapRewards.find(nLevel) != m_mapRewards.end();
}

bool CCumulativeSpendMgr::IsValidSpendTime(time_t nTime)
{
	return nTime >= (time_t)m_nRechargeStartTime && nTime <= (time_t)m_nRechargeStopTime;
}

const CCumulativeSpendReward * CCumulativeSpendMgr::GetRewardInfo( int nLevel )
{
	CCumulativeSpendReward_t::iterator itr = m_mapRewards.find(nLevel);
	if (itr != m_mapRewards.end()) {
		return itr->second;
	}
	
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

CumulativeSpendGotMedalMgr::CumulativeSpendGotMedalMgr()
    : m_bOpening( false )
    , m_bValid( false )
    , m_bExhibit( false )
    , m_nWeight( 0 )
    , m_nBeginTime( 0 )
    , m_nEndTime( 0 )
    , m_nOpenTime( 0 )
    , m_nCloseTime( 0 )
    , m_strTitle( "" )
    , m_strContent( "" )
    , m_strSmallImage( "" )
    , m_strBigImage( "" )
    , m_nCumulativeType( 1 )
{
}

void CumulativeSpendGotMedalMgr::Initialize()
{
    if ( m_nOpenTime < m_nBeginTime )
        m_nOpenTime = m_nBeginTime;
    if ( m_nEndTime < m_nCloseTime )
        m_nCloseTime = m_nEndTime;

    CheckValid( (unsigned int)time( NULL ) );
}

void CumulativeSpendGotMedalMgr::OnUpdate( unsigned int nNow )
{
    if ( !m_bValid )
        return;

    CheckValid( nNow );

    if ( m_nOpenTime <= nNow && nNow < m_nEndTime )
        m_bOpening = true;
    else
        m_bOpening = false;
}

bool CumulativeSpendGotMedalMgr::IsValidIndex( unsigned char nIndex ) const
{
    return m_mapRewardInfos.find( nIndex ) != m_mapRewardInfos.end();
}

int CumulativeSpendGotMedalMgr::GetMaxLevel() const
{
    if ( m_mapRewardInfos.empty() )
        return 0;
    else
        return m_mapRewardInfos.rbegin()->first + 1;
}

int CumulativeSpendGotMedalMgr::GetRequiredAmount( unsigned char nIndex ) const
{
    std::map<unsigned char, CumulativeSpendGotMedalRewards>::const_iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr != m_mapRewardInfos.end() )
        return itr->second.GetRequiredNum();

    return -1;
}

void CumulativeSpendGotMedalMgr::GetRewards( unsigned char nIndex, unsigned char nSex, std::list<int> &rReward ) const
{
    std::map<unsigned char, CumulativeSpendGotMedalRewards>::const_iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr == m_mapRewardInfos.end() )
        return;

    if ( nSex == ESexType_Male )
        rReward = itr->second.GetMaleReward();
    else
        rReward = itr->second.GetFemaleReward();
}

void CumulativeSpendGotMedalMgr::InitInfoByLua( bool bExhibit, int nWeight, 
    unsigned int nBeginTime, unsigned int nEndTime, unsigned int nOpenTime, 
    unsigned int nCloseTime, const std::string &rTitle, const std::string &rContent, 
    const std::string &rSmallImage, const std::string &rBigImage, unsigned char nCumulativeType )
{
    m_bExhibit = bExhibit;
    m_nWeight = nWeight;
    m_nBeginTime = nBeginTime;
    m_nEndTime = nEndTime;
    m_nOpenTime = nOpenTime;
    m_nCloseTime = nCloseTime;
    m_strTitle = rTitle;
    m_strContent = rContent;
    m_strSmallImage = rSmallImage;
    m_strBigImage = rBigImage;
    m_nCumulativeType = nCumulativeType;

    ASSERT( m_nCumulativeType == 1 || m_nCumulativeType == 2 );
}

void CumulativeSpendGotMedalMgr::AddRewardInfo( unsigned char nIndex, int nRequiredNum, 
    const std::list<int> &rMaleReward, const std::list<int> &rFemaleReward )
{
    if (nRequiredNum <= 0)
        return;

    std::map<unsigned char, CumulativeSpendGotMedalRewards>::iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr != m_mapRewardInfos.end() )
        return;

    CumulativeSpendGotMedalRewards reward;
    reward.SetRequiredNum( nRequiredNum );
    reward.SetMaleReward( rMaleReward );
    reward.SetFemaleReward( rFemaleReward );

    m_mapRewardInfos.insert( std::make_pair( nIndex, reward ) );
}

void CumulativeSpendGotMedalMgr::CheckValid( unsigned int nNow )
{
    if ( m_nCloseTime <= nNow )
        m_bValid = false;
    else
        m_bValid = true;
}

