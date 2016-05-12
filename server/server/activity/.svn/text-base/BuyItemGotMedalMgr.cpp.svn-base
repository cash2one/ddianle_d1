#include "BuyItemGotMedalMgr.h"
#include "../../datastructure/Macro_Define.h"
#include <time.h>


//////////////////////////////////////////////////////////////////////////

BuyItemGotMedalMgr::BuyItemGotMedalMgr(void)
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
{
}

void BuyItemGotMedalMgr::Initialize()
{
    if ( m_nOpenTime < m_nBeginTime )
        m_nOpenTime = m_nBeginTime;
    if ( m_nEndTime < m_nCloseTime )
        m_nCloseTime = m_nEndTime;

    CheckValid( (unsigned int)time( NULL ) );
}

void BuyItemGotMedalMgr::OnUpdate( unsigned int nNow )
{
    if ( !m_bValid )
        return;

    CheckValid( nNow );

    if ( m_nOpenTime <= nNow && nNow < m_nEndTime )
        m_bOpening = true;
    else
        m_bOpening = false;
}

bool BuyItemGotMedalMgr::IsValidIndex( unsigned char nIndex ) const
{
    return m_mapRewardInfos.find( nIndex ) != m_mapRewardInfos.end();
}

bool BuyItemGotMedalMgr::CanGetRewards( unsigned char nIndex, unsigned short nCurrentCount ) const
{
    std::map<unsigned char, BuyItemGotMedalRewards>::const_iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr == m_mapRewardInfos.end() )
        return false;

    return ( itr->second.GetItemCount() <= nCurrentCount );
}

void BuyItemGotMedalMgr::GetRewards( unsigned char nIndex, unsigned char nSex, std::list<int> &rReward ) const
{
    std::map<unsigned char, BuyItemGotMedalRewards>::const_iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr == m_mapRewardInfos.end() )
        return;

    if ( nSex == ESexType_Male )
        rReward = itr->second.GetMaleReward();
    else
        rReward = itr->second.GetFemaleReward();
}

void BuyItemGotMedalMgr::InitInfoByLua( bool bExhibit, int nWeight, 
    unsigned int nBeginTime, unsigned int nEndTime, unsigned int nOpenTime, 
    unsigned int nCloseTime, const std::string &rTitle, const std::string &rContent, 
    const std::string &rSmallImage, const std::string &rBigImage )
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
}

void BuyItemGotMedalMgr::AddRewardInfo( unsigned char nIndex, unsigned int nItemType, 
    unsigned short nItemCount, const std::list<int> &rMaleReward, const std::list<int> &rFemaleReward )
{
    if ( nItemCount == 0 )
        return;

    std::map<unsigned char, BuyItemGotMedalRewards>::iterator itr = m_mapRewardInfos.find( nIndex );

    if ( itr != m_mapRewardInfos.end() )
        return;

    BuyItemGotMedalRewards reward;
    reward.SetItemType( nItemType );
    reward.SetItemCount( nItemCount );
    reward.SetMaleReward( rMaleReward );
    reward.SetFemaleReward( rFemaleReward );

    m_mapRewardInfos.insert( std::make_pair( nIndex, reward ) );
}

void BuyItemGotMedalMgr::CheckValid( unsigned int nNow )
{
    if ( m_nCloseTime <= nNow )
        m_bValid = false;
    else
        m_bValid = true;
}

