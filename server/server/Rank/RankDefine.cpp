#include "RankDefine.h"

#include "../../socket/ParamPool.h"


CRankTopOneInfo::CRankTopOneInfo()
    : m_nType(0)
    , m_strTitle("")
    , m_bActiveOpend(false)
    , m_strDisplayInfo1("")
    , m_strDisplayInfo2("")
    , m_bInitialized(false)
{
}

void CRankTopOneInfo::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strTitle);
    m_bActiveOpend = IOBuff.GetBool();

    std::string strHeaderName;
    unsigned char count = IOBuff.GetUChar();

    for (unsigned char i = 0; i < count; ++i)
    {
        IOBuff.GetStringW(strHeaderName);

        m_listHeaderName.push_back(strHeaderName);
    }

    IOBuff.GetStringW(m_strDisplayInfo1);
    IOBuff.GetStringW(m_strDisplayInfo2);
    m_bInitialized = IOBuff.GetBool();
}

void CRankTopOneInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddUChar(m_nType);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddBool( m_bActiveOpend );

    IOBuff.AddUChar((unsigned char)m_listHeaderName.size() );
    for (std::list<std::string>::const_iterator itr = m_listHeaderName.begin(); 
        itr != m_listHeaderName.end(); ++itr)
    {
        IOBuff.AddStringW(*itr);
    }

    IOBuff.AddStringW(m_strDisplayInfo1);
    IOBuff.AddStringW(m_strDisplayInfo2);
    IOBuff.AddBool(m_bInitialized);
}

//////////////////////////////////////////////////////////////////////////

CRankOfflineInfo::CRankOfflineInfo()
    : m_nRank( 0 )
    , m_nSmallRoleID( 0 )
{
}

void CRankOfflineInfo::doDecode( CParamPool &IOBuff )
{
    m_nRank = IOBuff.GetUShort();

    std::string strOtherInfo;
    unsigned char i = 0;
    unsigned char count = IOBuff.GetUChar();

    for ( i = 0; i < count; ++i )
    {
        IOBuff.GetStringW( strOtherInfo );

        m_listOtherInfo.push_back( strOtherInfo );
    }

    m_nSmallRoleID = IOBuff.GetUInt();

    count = IOBuff.GetUChar();
    for ( i = 0; i < count; ++i )
    {
        RoleImageOfflineData offlineRoleInfo;
        offlineRoleInfo.doDecode( IOBuff );

        m_listOfflineData.push_back( offlineRoleInfo );
    }
}

void CRankOfflineInfo::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( m_nRank );

    IOBuff.AddUChar( (unsigned char)m_listOtherInfo.size() );
    for ( std::list<std::string>::const_iterator itr = m_listOtherInfo.begin();
        itr != m_listOtherInfo.end(); ++itr )
    {
        IOBuff.AddStringW( *itr );
    }

    IOBuff.AddUInt( m_nSmallRoleID );

    IOBuff.AddUChar( (unsigned char)m_listOfflineData.size() );
    for ( std::list<RoleImageOfflineData>::iterator itr = m_listOfflineData.begin();
        itr != m_listOfflineData.end(); ++itr )
    {
        itr->doEncode( IOBuff );
    }
}

//////////////////////////////////////////////////////////////////////////

CRankReward::CRankReward( unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin )
    : m_nSecond( nSecond )
    , m_listItems( rItems )
    , m_nMoney( nMoney )
    , m_nBindMCoin( nBindMCoin )
{
}

bool CRankReward::operator< (const CRankReward &rReward) const
{
    return m_nSecond < rReward.m_nSecond;
}

