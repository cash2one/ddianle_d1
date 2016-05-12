#include "DataStruct_Rank.h"
#include "../socket/Formatter.h"
#include "../server/Rank/RankDefine.h"

#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////

CRankSaveEveryDataDB::CRankSaveEveryDataDB()
{
    m_nKey = 0;
    m_nDataType = 0;
    m_nValue = 0;
}

//////////////////////////////////////////////////////////////////////////

CRankUpdateDataStateDB::CRankUpdateDataStateDB()
{
    m_nKey = 0;
    m_nDataType = 0;
    m_nTime = 0;
}

//////////////////////////////////////////////////////////////////////////

CRankCoupleValue::CRankCoupleValue()
{
    m_nPairID = 0;
    m_nMaleRoleID = 0;
    m_nFemaleRoleID = 0;
    m_nSamllRoleID = 0;
    m_nValue = 0;
}

//////////////////////////////////////////////////////////////////////////

CRankCoupleInfo::CRankCoupleInfo()
    : CRankInfo(0, 0)
{
}

CRankCoupleInfo::CRankCoupleInfo(const CRankCoupleValue &rValue)
    : CRankInfo(rValue.m_nPairID, rValue.m_nValue)
{
    m_RankValue = rValue;
}

void CRankCoupleInfo::SetValue( unsigned int nValue )
{
    CRankInfo::SetValue( nValue );
    m_RankValue.m_nValue = m_nValue;
}

bool CRankCoupleInfo::ChangeValue( int nChangeValue )
{
    bool bRes = CRankInfo::ChangeValue( nChangeValue );
    if ( bRes )
        m_RankValue.m_nValue = m_nValue;

    return bRes;
}

//////////////////////////////////////////////////////////////////////////

CRankRoleCumulativeValue::CRankRoleCumulativeValue()
    : m_nRoleID( 0 )
    , m_nValue( 0 )
    , m_nOfflineTime( 0 )
{
}

CRankRoleCumulativeInfo::CRankRoleCumulativeInfo()
    : CRankInfo( 0, 0 )
{
}

CRankRoleCumulativeInfo::CRankRoleCumulativeInfo( const CRankRoleCumulativeValue &rValue )
    : CRankInfo( rValue.m_nRoleID, rValue.m_nValue )
{
    m_rankValue = rValue;
}

void CRankRoleCumulativeInfo::SetValue( unsigned int nValue )
{
    CRankInfo::SetValue( nValue );
    m_rankValue.m_nValue = m_nValue;
}

bool CRankRoleCumulativeInfo::ChangeValue( int nChangeValue )
{
    bool bRes = CRankInfo::ChangeValue( nChangeValue );
    if ( bRes )
        m_rankValue.m_nValue = m_nValue;

    return bRes;
}

//////////////////////////////////////////////////////////////////////////

RankGetAllDatasDB::RankGetAllDatasDB( unsigned short nDataType )
    : m_nDataType( nDataType )
    , m_nDetailType( 0 )
    , m_nStartTime( 0 )
    , m_nEndTime( 0 )
    , m_nMaxCount( 0 )
    , m_bShowGay( false )
{
}

RankGetAllDatasDB::~RankGetAllDatasDB()
{
    while ( !m_listRankInfo.empty() )
    {
        delete *m_listRankInfo.begin();
        m_listRankInfo.pop_front();
    }
}

RankRoleGetAllDatasDB::RankRoleGetAllDatasDB( unsigned short nDataType )
    : RankGetAllDatasDB( nDataType )
{
}

void RankRoleGetAllDatasDB::DecodeByRow( const char **row )
{
    if ( row == NULL )
        return;

    int i = 0;
    unsigned int nKey = 0, nValue = 0;

    if ( row[i] != NULL )
        nKey = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nValue = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    CRankInfo *pInfo = new CRankInfo( nKey, nValue );
    m_listRankInfo.push_back( pInfo );
}

RankMCoinGetAllDatasDB::RankMCoinGetAllDatasDB( unsigned short nDataType )
    : RankRoleGetAllDatasDB( nDataType )
{
}

void RankMCoinGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select roleid, sum(total) as t from ( "
        "select roleid, createtime, total from rank_mcoin "
        "where createtime >= %u and createtime < %u order by createtime desc ) as self "
        "group by roleid order by t desc, self.createtime limit %u",
        m_nStartTime, m_nEndTime, m_nMaxCount );
}

RankHornGetAllDatasDB::RankHornGetAllDatasDB( unsigned short nDataType )
    : RankRoleGetAllDatasDB( nDataType )
{
}

void RankHornGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select roleid, sum(total) as t from ( "
        "select roleid, updatetime, total from rank_horn "
        "where createtime >= %u and createtime < %u order by updatetime desc ) as self "
        "group by roleid order by t desc, self.updatetime limit %u",
        m_nStartTime, m_nEndTime, m_nMaxCount );
}

RankExpGetAllDatasDB::RankExpGetAllDatasDB( unsigned short nDataType )
    : RankRoleGetAllDatasDB( nDataType )
{
}

void RankExpGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select roleid, sum(total) as t from ( "
        "select roleid, updatetime, total from rank_exp "
        "where createtime >= %u and createtime < %u order by updatetime desc ) as self "
        "group by roleid order by t desc, self.updatetime limit %u",
        m_nStartTime, m_nEndTime, m_nMaxCount );
}

RankGroupHonorGetAllDatasDB::RankGroupHonorGetAllDatasDB( unsigned short nDataType )
    : RankGetAllDatasDB( nDataType )
{
}

void RankGroupHonorGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    if ( m_nDetailType == ERankDetailType_CurrentWeek )
    {
        Formatter::Format( rSQL,
            "select groupid, sum(total) as t,dismissed from ( "
            "select groupid, updatetime, total, dismissed from rank_grouphonor "
            "where createtime >= %u and createtime < %u and dismissed <> 1 order by updatetime desc ) as self "
            "group by groupid order by t desc, self.updatetime limit %u",
            m_nStartTime, m_nEndTime, m_nMaxCount );
    }
    else
    {
        Formatter::Format( rSQL,
            "select groupid, sum(total) as t, dismissed from ( "
            "select groupid, updatetime, total, dismissed from rank_grouphonor "
            "where createtime >= %u and createtime < %u order by updatetime desc ) as self "
            "group by groupid order by t desc, self.updatetime limit %u",
            m_nStartTime, m_nEndTime, m_nMaxCount );
    }
}

void RankGroupHonorGetAllDatasDB::DecodeByRow( const char **row )
{
    if ( row == NULL )
        return;

    int i = 0;
    unsigned int nKey = 0, nValue = 0;
    int dismissed = 0;

    if ( row[i] != NULL )
        nKey = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nValue = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        dismissed = atoi( row[i] );
    ++i;

    if ( dismissed == 0 )
    {
        CRankInfo *pInfo = new CRankInfo( nKey, nValue );
        m_listRankInfo.push_back( pInfo );
    }
}

RankIntimacyGetAllDatasDB::RankIntimacyGetAllDatasDB( unsigned short nDataType )
    : RankGetAllDatasDB( nDataType )
{
}

void RankIntimacyGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    if ( m_nDetailType == ERankDetailType_CurrentWeek )
    {
        Formatter::Format( rSQL,
            "select pairid, sum(total) as t, divorced, maleid, femaleid, smallroleid from ( "
            "select pairid, updatetime, total, divorced from rank_intimacy "
            "where createtime >= %u and createtime < %u and divorced <> 1 order by updatetime desc ) as self "
            "inner join couple on id = pairid ", m_nStartTime, m_nEndTime );
    }
    else
    {
        Formatter::Format( rSQL,
            "select pairid, sum(total) as t, divorced, maleid, femaleid, smallroleid from ( "
            "select pairid, updatetime, total, divorced from rank_intimacy "
            "where createtime >= %u and createtime < %u order by updatetime desc ) as self "
            "inner join couple on id = pairid ", m_nStartTime, m_nEndTime );
    }

    if ( !m_bShowGay )
    {
        rSQL.append( "where smallroleid = 0 " );
    }

    rSQL.append( "group by pairid order by t desc, self.updatetime limit " );
    rSQL.append( Formatter() << m_nMaxCount );
}

void RankIntimacyGetAllDatasDB::DecodeByRow( const char **row )
{
    if ( row == NULL )
        return;

    int i = 0;
    unsigned int nPairID = 0, nValue = 0;
    int divorced = 0;
    unsigned int nMaleRoleID = 0, nFemaleRoleID = 0, nSmallRoleID = 0;

    if ( row[i] != NULL )
        nPairID = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nValue = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        divorced = atoi( row[i] );
    ++i;

    if ( row[i] != NULL )
        nMaleRoleID = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nFemaleRoleID = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nSmallRoleID = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( divorced == 0 && nMaleRoleID != 0 && nFemaleRoleID != 0 )
    {
        CRankCoupleValue coupleValue;
        coupleValue.m_nPairID = nPairID;
        coupleValue.m_nValue = nValue;
        coupleValue.m_nMaleRoleID = nMaleRoleID;
        coupleValue.m_nFemaleRoleID = nFemaleRoleID;
        coupleValue.m_nSamllRoleID = nSmallRoleID;

        CRankCoupleInfo *pInfo = new CRankCoupleInfo( coupleValue );
        m_listRankInfo.push_back( pInfo );
    }
}

RankRoleCumulativeGetAllDatasDB::RankRoleCumulativeGetAllDatasDB( unsigned short nDataType )
    : RankGetAllDatasDB( nDataType )
{
}

void RankRoleCumulativeGetAllDatasDB::DecodeByRow( const char **row )
{
    if ( row == NULL )
        return;

    int i = 0;
    unsigned int nRoleID = 0, nValue = 0, nOfflineTime = 0;

    if ( row[i] != NULL )
        nRoleID = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nValue = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        nOfflineTime = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    CRankRoleCumulativeValue onlineValue;
    onlineValue.m_nRoleID = nRoleID;
    onlineValue.m_nValue = nValue;
    onlineValue.m_nOfflineTime = nOfflineTime;

    CRankRoleCumulativeInfo *pInfo = new CRankRoleCumulativeInfo( onlineValue );
    m_listRankInfo.push_back( pInfo );
}

RankClothsGetAllDatasDB::RankClothsGetAllDatasDB( unsigned short nDataType )
    : RankRoleCumulativeGetAllDatasDB( nDataType )
{
}

void RankClothsGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    if ( m_nDetailType == ERankDetailType_CurrentWeek )
    {
        Formatter::Format( rSQL,
            "select roleid, total, offlinetime from rank_cloths "
            "where offlinetime >= %u "
            "order by total desc, offlinetime desc limit %u",
            m_nStartTime, m_nMaxCount );
    }
    else
    {
        Formatter::Format( rSQL,
            "select roleid, total, 0 from rank_cloths_history "
            "where time >= %u and time < %u "
            "order by total desc, logid limit %u",
            m_nStartTime, m_nEndTime, m_nMaxCount );
    }
}

RankClothEffectGetAllDatasDB::RankClothEffectGetAllDatasDB( unsigned short nDataType )
    : RankRoleCumulativeGetAllDatasDB( nDataType )
{
}

void RankClothEffectGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    if ( m_nDetailType == ERankDetailType_CurrentWeek )
    {
        Formatter::Format( rSQL,
            "select roleid, total, offlinetime from rank_clotheffect "
            "where offlinetime >= %u "
            "order by total desc, offlinetime desc limit %u",
            m_nStartTime, m_nMaxCount );
    }
    else
    {
        Formatter::Format( rSQL,
            "select roleid, total, 0 from rank_clotheffect_history "
            "where time >= %u and time < %u "
            "order by total desc, logid limit %u",
            m_nStartTime, m_nEndTime, m_nMaxCount );
    }
}

RankMedalGetAllDatasDB::RankMedalGetAllDatasDB( unsigned short nDataType )
    : RankRoleCumulativeGetAllDatasDB( nDataType )
{
}

void RankMedalGetAllDatasDB::GetSQL( std::string &rSQL ) const
{
    if ( m_nDetailType == ERankDetailType_CurrentWeek )
    {
        Formatter::Format( rSQL,
            "select roleid, total, offlinetime from rank_medal "
            "where offlinetime >= %u "
            "order by total desc, offlinetime desc limit %u",
            m_nStartTime, m_nMaxCount );
    }
    else
    {
        Formatter::Format( rSQL,
            "select roleid, total, 0 from rank_medal_history "
            "where time >= %u and time < %u "
            "order by total desc, logid limit %u",
            m_nStartTime, m_nEndTime, m_nMaxCount );
    }
}

RankGetAllDatasDB* RankGetAllDatasDBFactory::Create( unsigned short nDataType )
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
        return new RankMCoinGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Horn:
        return new RankHornGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Exp:
        return new RankExpGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Group_Honor:
        return new RankGroupHonorGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Intimacy:
        return new RankIntimacyGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Cloths:
        return new RankClothsGetAllDatasDB( nDataType );
        break;

    case ERankDataType_ClothEffect:
        return new RankClothEffectGetAllDatasDB( nDataType );
        break;

    case ERankDataType_Medal:
        return new RankMedalGetAllDatasDB( nDataType );
        break;

    default:
        return NULL;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

RankGetRoleDataDB::RankGetRoleDataDB( unsigned short nDataType )
    : m_nDataType( nDataType )
    , m_nKey( 0 )
    , m_nStartTime( 0 )
    , m_nValue( 0 )
{
}

void RankGetRoleDataDB::DecodeByRow( const char **row )
{
    if ( row != NULL && row[0] != NULL )
        m_nValue = (unsigned int)strtoul( row[0], NULL, 10 );
}

RankMCoinGetRoleDataDB::RankMCoinGetRoleDataDB( unsigned short nDataType )
    : RankGetRoleDataDB( nDataType )
{
}

void RankMCoinGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select sum(total) from rank_mcoin "
        "where createtime >= %u and roleid = %u",
        m_nStartTime, m_nKey );
}

RankHornGetRoleDataDB::RankHornGetRoleDataDB( unsigned short nDataType )
    : RankGetRoleDataDB( nDataType )
{
}

void RankHornGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select sum(total) from rank_horn "
        "where createtime >= %u and roleid = %u",
        m_nStartTime, m_nKey );
}

RankExpGetRoleDataDB::RankExpGetRoleDataDB( unsigned short nDataType )
    : RankGetRoleDataDB( nDataType )
{
}

void RankExpGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "select sum(total) from rank_exp "
        "where createtime >= %u and roleid = %u",
        m_nStartTime, m_nKey );
}

RankRoleCumulativeGetRoleDataDB::RankRoleCumulativeGetRoleDataDB( unsigned short nDataType )
    : RankGetRoleDataDB( nDataType )
{
}

void RankRoleCumulativeGetRoleDataDB::DecodeByRow( const char **row )
{
    if ( row == NULL )
        return;

    int i = 0;
    if ( row[i] != NULL )
        m_nValue = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;

    if ( row[i] != NULL )
        m_nStartTime = (unsigned int)strtoul( row[i], NULL, 10 );
    ++i;
}

RankClothsGetRoleDataDB::RankClothsGetRoleDataDB( unsigned short nDataType )
    : RankRoleCumulativeGetRoleDataDB( nDataType )
{
}

void RankClothsGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL, "select total, offlinetime from rank_cloths where roleid = %u", m_nKey );
}

RankClothEffectGetRoleDataDB::RankClothEffectGetRoleDataDB( unsigned short nDataType )
    : RankRoleCumulativeGetRoleDataDB( nDataType )
{
}

void RankClothEffectGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL, "select total, offlinetime from rank_clotheffect where roleid = %u", m_nKey );
}

RankMedalGetRoleDataDB::RankMedalGetRoleDataDB( unsigned short nDataType )
    : RankRoleCumulativeGetRoleDataDB( nDataType )
{
}

void RankMedalGetRoleDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL, "select total, offlinetime from rank_medal where roleid = %u", m_nKey );
}

RankGetRoleDataDB* RankGetRoleDataDBFactory::Create( unsigned short nDataType )
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
        return new RankMCoinGetRoleDataDB( nDataType );
        break;

    case ERankDataType_Horn:
        return new RankHornGetRoleDataDB( nDataType );
        break;

    case ERankDataType_Exp:
        return new RankExpGetRoleDataDB( nDataType );
        break;

    case ERankDataType_Cloths:
        return new RankClothsGetRoleDataDB( nDataType );
        break;

    case ERankDataType_ClothEffect:
        return new RankClothEffectGetRoleDataDB( nDataType );
        break;

    case ERankDataType_Medal:
        return new RankMedalGetRoleDataDB( nDataType );
        break;

    default:
        return NULL;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

RankSaveDataDB::RankSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : m_nDataType( nDataType )
    , m_nKey( nKey )
    , m_nValue( nValue )
    , m_nTime( 0 )
{
}

bool RankSaveDataDB::CanProcessValue() const
{
    if ( m_nValue == 0 )
        return false;

    return true;
}

RankMCoinSaveDataDB::RankMCoinSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

void RankMCoinSaveDataDB::GetSQL( std::string &rSQL ) const
{
    Formatter::Format( rSQL,
        "insert into rank_mcoin(roleid, createtime, total) "
        "values(%u, %u, %d) "
        "on duplicate key update total = total + %d",
        m_nKey, m_nTime, m_nValue, m_nValue );
}

RankHornSaveDataDB::RankHornSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

void RankHornSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_horn(roleid, createtime, total, updatetime) "
        "values(%u, %u, %d, %u) "
        "on duplicate key update total = total + %d, updatetime = %u",
        m_nKey, m_nTime, m_nValue, nNow, m_nValue, nNow );
}

RankExpSaveDataDB::RankExpSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

void RankExpSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_exp(roleid, createtime, total, updatetime) "
        "values(%u, %u, %d, %u) "
        "on duplicate key update total = total + %d, updatetime = %u",
        m_nKey, m_nTime, m_nValue, nNow, m_nValue, nNow );
}

RankGroupHonorSaveDataDB::RankGroupHonorSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

void RankGroupHonorSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_grouphonor(groupid, createtime, total, updatetime, dismissed) "
        "values(%u, %u, %d, %u, 0) "
        "on duplicate key update total = total + %d, updatetime = %u",
        m_nKey, m_nTime, m_nValue, nNow, m_nValue, nNow );
}

RankIntimacySaveDataDB::RankIntimacySaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

void RankIntimacySaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_intimacy(pairid, createtime, total, updatetime, divorced) "
        "values(%u, %u, %d, %u, 0) "
        "on duplicate key update total = total + %d, updatetime = %u",
        m_nKey, m_nTime, m_nValue, nNow, m_nValue, nNow );
}

RankRoleCumulativeSaveDataDB::RankRoleCumulativeSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankSaveDataDB( nDataType, nKey, nValue )
{
}

RankClothsSaveDataDB::RankClothsSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankRoleCumulativeSaveDataDB( nDataType, nKey, nValue )
{
}

void RankClothsSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_cloths(roleid, total, offlinetime) "
        "values(%u, %d, %u) "
        "on duplicate key update total = total + %d, offlinetime = %u",
        m_nKey, m_nValue, nNow, m_nValue, nNow );
}

RankClothEffectSaveDataDB::RankClothEffectSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankRoleCumulativeSaveDataDB( nDataType, nKey, nValue )
{
}

void RankClothEffectSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_clotheffect(roleid, total, offlinetime) "
        "values(%u, %d, %u) "
        "on duplicate key update total = total + %d, offlinetime = %u",
        m_nKey, m_nValue, nNow, m_nValue, nNow );
}

RankMedalSaveDataDB::RankMedalSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue )
    : RankRoleCumulativeSaveDataDB( nDataType, nKey, nValue )
{
}

void RankMedalSaveDataDB::GetSQL( std::string &rSQL ) const
{
    unsigned int nNow = (unsigned int)time( NULL );
    Formatter::Format( rSQL,
        "insert into rank_medal(roleid, total, offlinetime) "
        "values(%u, %d, %u) "
        "on duplicate key update total = total + %d, offlinetime = %u",
        m_nKey, m_nValue, nNow, m_nValue, nNow );
}

RankSaveDataDB* RankSaveDataDBFactory::Create( unsigned short nDataType, unsigned int nKey, int nValue )
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
        return new RankMCoinSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Horn:
        return new RankHornSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Exp:
        return new RankExpSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Group_Honor:
        return new RankGroupHonorSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Intimacy:
        return new RankIntimacySaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Cloths:
        return new RankClothsSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_ClothEffect:
        return new RankClothEffectSaveDataDB( nDataType, nKey, nValue );
        break;

    case ERankDataType_Medal:
        return new RankMedalSaveDataDB( nDataType, nKey, nValue );
        break;

    default:
        return NULL;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

RankSaveHistoryDB::RankSaveHistoryDB( unsigned short nDataType )
    : m_nDataType( nDataType )
{
}

RankClothsSaveHistoryDB::RankClothsSaveHistoryDB( unsigned short nDataType )
    : RankSaveHistoryDB( nDataType )
{
}

void RankClothsSaveHistoryDB::GetSQL( std::string &rSQL ) const
{
    if ( m_mapHistoryDatas.empty() )
        return;

    std::string tmp;
    unsigned int nNow = (unsigned int)time( NULL );

    for ( std::map<unsigned int, unsigned int>::const_iterator itr = m_mapHistoryDatas.begin();
        itr != m_mapHistoryDatas.end(); ++itr )
    {
        tmp.append( Formatter() << "(" << itr->first << ", " << itr->second << ", " << nNow << "), " );
    }

    std::string::size_type nLast = tmp.find_last_of( ',' );
    tmp.erase( nLast );

    Formatter::Format( rSQL, "insert into rank_cloths_history(roleid, total, time) values %s", tmp.c_str() );
}

RankClothEffectSaveHistoryDB::RankClothEffectSaveHistoryDB( unsigned short nDataType )
    : RankSaveHistoryDB( nDataType )
{
}

void RankClothEffectSaveHistoryDB::GetSQL( std::string &rSQL ) const
{
    if ( m_mapHistoryDatas.empty() )
        return;

    std::string tmp;
    unsigned int nNow = (unsigned int)time( NULL );

    for ( std::map<unsigned int, unsigned int>::const_iterator itr = m_mapHistoryDatas.begin();
        itr != m_mapHistoryDatas.end(); ++itr )
    {
        tmp.append( Formatter() << "(" << itr->first << ", " << itr->second << ", " << nNow << "), " );
    }

    std::string::size_type nLast = tmp.find_last_of( ',' );
    tmp.erase( nLast );

    Formatter::Format( rSQL, "insert into rank_clotheffect_history(roleid, total, time) values %s", tmp.c_str() );
}

RankMedalSaveHistoryDB::RankMedalSaveHistoryDB( unsigned short nDataType )
    : RankSaveHistoryDB( nDataType )
{
}

void RankMedalSaveHistoryDB::GetSQL( std::string &rSQL ) const
{
    if ( m_mapHistoryDatas.empty() )
        return;

    std::string tmp;
    unsigned int nNow = (unsigned int)time( NULL );

    for ( std::map<unsigned int, unsigned int>::const_iterator itr = m_mapHistoryDatas.begin();
        itr != m_mapHistoryDatas.end(); ++itr )
    {
        tmp.append( Formatter() << "(" << itr->first << ", " << itr->second << ", " << nNow << "), " );
    }

    std::string::size_type nLast = tmp.find_last_of( ',' );
    tmp.erase( nLast );

    Formatter::Format( rSQL, "insert into rank_medal_history(roleid, total, time) values %s", tmp.c_str() );
}

RankSaveHistoryDB* RankSaveHistoryDBFactory::Create( unsigned short nDataType )
{
    switch ( nDataType )
    {
    case ERankDataType_Cloths:
        return new RankClothsSaveHistoryDB( nDataType );
        break;

    case ERankDataType_ClothEffect:
        return new RankClothEffectSaveHistoryDB( nDataType );
        break;

    case ERankDataType_Medal:
        return new RankMedalSaveHistoryDB( nDataType );
        break;

    default:
        return NULL;
        break;
    }
}

