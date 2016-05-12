#include "RankConfigData.h"
#include "GameMsg_Rank_SG.h"

#include "../SystemSetting/SystemSettingMgr.h"

#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"

#include <algorithm>


CRankConfigData::CRankConfigData(void)
    : GameMsg_Processor(true, false)
    , m_nStartWeekDay(0)
    , m_nStartHour(0)
    , m_nMaxOfflineTime(0)
    , m_bIsOpening(false)
{
}

CRankConfigData::~CRankConfigData(void)
{
}

CRankConfigData& CRankConfigData::Instance()
{
    static CRankConfigData s_RankingHallListManager;

    return s_RankingHallListManager;
}

void CRankConfigData::Initialize()
{
    m_bIsOpening = CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_Rank);

    if ( m_bIsOpening )
        RegComponentNetMsgMap();
}

void CRankConfigData::LoadStartTime(unsigned int nStartWeekDay, unsigned int nStartHour)
{
    if (nStartWeekDay > 6)
    {
        LOG_ERROR << "Rank start week day(" 
                  << nStartWeekDay 
                  << ") is wrong." << std::endl;
    }
    else if (nStartHour > 23)
    {
        LOG_ERROR << "Rank start hour(" 
                  << nStartHour 
                  << ") is wrong." << std::endl;
    }
    else
    {
        m_nStartWeekDay = (int)nStartWeekDay;
        m_nStartHour = (int)nStartHour;
    }
}

void CRankConfigData::LoadMaxOfflineTime( int nMaxOfflineTime )
{
    if ( m_nMaxOfflineTime < nMaxOfflineTime )
        m_nMaxOfflineTime = nMaxOfflineTime;
}

void CRankConfigData::AddRankHall(unsigned short nType)
{
    if (nType >= ERankDataType_Max)
    {
        LOG_ERROR << "Rank hall data type(" 
                  << nType 
                  << ") is wrong." << std::endl;

        return;
    }

    std::list<unsigned short>::const_iterator itr = std::find( m_listTypes.begin(), m_listTypes.end(), nType );
    if ( itr != m_listTypes.end() )
    {
        LOG_ERROR << "Rank hall data type("
                  << nType 
                  << ") is duplicate." << std::endl;

        return;
    }

    m_listTypes.push_back( nType );
}

void CRankConfigData::LoadRule( const std::string &rRewardRule, const std::string &rScoreRule )
{
    m_strRewardRule = rRewardRule;
    m_strScoreRule = rScoreRule;
}

void CRankConfigData::AddRankReward( unsigned short nDataType, unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin )
{
    if ( nDataType >= ERankDataType_Max )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall data type(%u) is wrong.", nDataType );
        return;
    }

    std::set<CRankReward> &rSetRewards = m_mapRewards[nDataType];
    CRankReward reward( nSecond, rItems, nMoney, nBindMCoin );
    if ( rSetRewards.count( reward ) == 0 )
        rSetRewards.insert( reward );
}

void CRankConfigData::GetStartTime(int &nStartWeekDay, int &nStartHour) const
{
    nStartWeekDay = m_nStartWeekDay;
    nStartHour = m_nStartHour;
}

void CRankConfigData::GetAllTypes( std::list<unsigned short> &rTypes ) const
{
    rTypes.assign( m_listTypes.begin(), m_listTypes.end() );
}

void CRankConfigData::GetAllRewards( unsigned short nDataType, std::list<CRankReward> &rRewards ) const
{
    std::map<unsigned short, std::set<CRankReward> >::const_iterator itr = m_mapRewards.find( nDataType );
    if ( itr == m_mapRewards.end() )
        return;

    const std::set<CRankReward> &rSetRewards = itr->second;
    rRewards.assign( rSetRewards.begin(), rSetRewards.end() );
}

bool CRankConfigData::IsBiggerThanMinValue( unsigned short nDataType, unsigned int nValue )
{
    if ( m_mapType2MinValue[nDataType] < nValue )
        return true;

    return false;
}

bool CRankConfigData::IsBeyondMaxOfflineTime( unsigned int nLastOfflineTime ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    nLastOfflineTime += m_nMaxOfflineTime;

    if ( nLastOfflineTime < nNow )
        return true;

    return false;
}

void CRankConfigData::RegComponentNetMsgMap()
{
    RegMsgCreatorAndProcessor( GameMsg_G2S_UpdateRankMinValue, &CRankConfigData::OnUpdateRankMinValue );
}

void CRankConfigData::OnUpdateRankMinValue( GameMsg_Base &msg, CSlotPeer &SlotPeer )
{
    GameMsg_G2S_UpdateRankMinValue *pUpdateMsg = dynamic_cast<GameMsg_G2S_UpdateRankMinValue *>(&msg);
    if ( pUpdateMsg == NULL )
        return;

    m_mapType2MinValue[pUpdateMsg->m_nDataType] = pUpdateMsg->m_nValue;
}

