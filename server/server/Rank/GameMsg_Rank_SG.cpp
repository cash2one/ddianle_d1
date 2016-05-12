#include "GameMsg_Rank_SG.h"
#include "../share/ServerMsgDef.h"


//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetRankHall::GameMsg_S2G_GetRankHall()
    : GameMsg_Base(MSG_S2G_Rank_GetHall)
    , m_nCurrentPage(0)
    , m_nCount(0)
{
}

bool GameMsg_S2G_GetRankHall::doDecode(CParamPool &IOBuff)
{
    m_nCurrentPage = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUChar();

    return true;
}

bool GameMsg_S2G_GetRankHall::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUChar(m_nCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GetRankHallResult::GameMsg_G2S_GetRankHallResult()
    : GameMsg_Base(MSG_G2S_Rank_GetHallResult)
    , m_nCurrentPage(0)
    , m_nTotal(0)
{
}

bool GameMsg_G2S_GetRankHallResult::doDecode(CParamPool &IOBuff)
{
    m_nCurrentPage = IOBuff.GetUShort();
    m_nTotal = IOBuff.GetUShort();

    unsigned char count = IOBuff.GetUChar();

    for (unsigned char i = 0; i < count; ++i)
    {
        CRankTopOneInfo topOneInfo;

        topOneInfo.doDecode(IOBuff);

        m_listTopOneInfo.push_back(topOneInfo);
    }

    return true;
}

bool GameMsg_G2S_GetRankHallResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUShort(m_nTotal);

    IOBuff.AddUChar((unsigned char)m_listTopOneInfo.size() );
    for (std::list<CRankTopOneInfo>::const_iterator itr = m_listTopOneInfo.begin(); 
        itr != m_listTopOneInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetRankDatas::GameMsg_S2G_GetRankDatas()
    : GameMsg_Base(MSG_S2G_Rank_GetDatas)
    , m_nType(0)
    , m_nDetailType(0)
    , m_nCurrentPage(0)
    , m_nCount(0)
    , m_nKey(0)
{
}

bool GameMsg_S2G_GetRankDatas::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetUChar();
    m_nDetailType = IOBuff.GetUChar();
    m_nCurrentPage = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUChar();
    m_nKey = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_GetRankDatas::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nType);
    IOBuff.AddUChar(m_nDetailType);
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUChar(m_nCount);
    IOBuff.AddUInt(m_nKey);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GetRankDatasResult::GameMsg_G2S_GetRankDatasResult()
    : GameMsg_Base(MSG_G2S_Rank_GetDatasResult)
    , m_nMyRank(0)
    , m_nCurrentPage(0)
    , m_nTotal(0)
    , m_nDataType(ERankDataType_Max)
    , m_nDetailType(0)
{
}

bool GameMsg_G2S_GetRankDatasResult::doDecode(CParamPool &IOBuff)
{
    m_nMyRank = IOBuff.GetUShort();
    m_nCurrentPage = IOBuff.GetUShort();
    m_nTotal = IOBuff.GetUShort();

    unsigned char count = IOBuff.GetUChar();

    for (unsigned char i = 0; i < count; ++i)
    {
        CRankOfflineInfo offlineInfo;

        offlineInfo.doDecode(IOBuff);

        m_listOfflineInfo.push_back(offlineInfo);
    }

    m_nDataType = IOBuff.GetUShort();
    m_nDetailType = IOBuff.GetUChar();

    return true;
}

bool GameMsg_G2S_GetRankDatasResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nMyRank);
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUShort(m_nTotal);

    IOBuff.AddUChar((unsigned char)m_listOfflineInfo.size() );

    for (std::list<CRankOfflineInfo>::iterator itr = m_listOfflineInfo.begin(); 
         itr != m_listOfflineInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    IOBuff.AddUShort(m_nDataType);
    IOBuff.AddUChar(m_nDetailType);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_ChangeRankValue::GameMsg_S2G_ChangeRankValue()
    : GameMsg_Base(MSG_S2G_Rank_ChangeValue)
    , m_nDataType(0)
    , m_nKey(0)
    , m_nChangeValue(0)
    , m_nTotalValue(0)
    , m_nStartTime(0)
{
}

bool GameMsg_S2G_ChangeRankValue::doDecode(CParamPool &IOBuff)
{
    m_nDataType = IOBuff.GetUShort();
    m_nKey = IOBuff.GetUInt();
    m_nChangeValue = IOBuff.GetUInt();
    m_nTotalValue = IOBuff.GetUInt();
    m_nStartTime = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_ChangeRankValue::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nDataType);
    IOBuff.AddUInt(m_nKey);
    IOBuff.AddUInt(m_nChangeValue);
    IOBuff.AddUInt(m_nTotalValue);
    IOBuff.AddUInt( m_nStartTime );

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_UpdateRankMinValue::GameMsg_G2S_UpdateRankMinValue()
    : GameMsg_Base(MSG_G2S_Rank_UpdateMinValue)
    , m_nDataType(0)
    , m_nValue(0)
{
}

bool GameMsg_G2S_UpdateRankMinValue::doDecode(CParamPool &IOBuff)
{
    m_nDataType = IOBuff.GetUShort();
    m_nValue = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_UpdateRankMinValue::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nDataType);
    IOBuff.AddUInt(m_nValue);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetRankAllDataForMedal::GameMsg_S2G_GetRankAllDataForMedal()
    :GameMsg_Base(MSG_S2G_Rank_GetAllRankDataForMedal)
    , m_nRoleId(0)
    , m_GroupId(0)
    , m_IntimacyId(0)
{

}

bool GameMsg_S2G_GetRankAllDataForMedal::doDecode(CParamPool &IOBuff)
{
    m_nRoleId = IOBuff.GetInt();
    m_GroupId = IOBuff.GetInt();
    m_IntimacyId = IOBuff.GetInt();
    return true;
}

bool GameMsg_S2G_GetRankAllDataForMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRoleId);
    IOBuff.AddInt(m_GroupId);
    IOBuff.AddInt(m_IntimacyId);
    return true;
}
//////////////////////////////////////////////////////////////////////////
GameMsg_G2S_GetRankAllDataForMedal::GameMsg_G2S_GetRankAllDataForMedal()
    :GameMsg_Base(MSG_G2S_Rank_GetAllRankDataResultForMedal)
    , m_RankMCoin(0)
    , m_RankHorn(0)
    , m_RankExp(0)
    , m_RankGroupHonor(0)
    , m_RankIntimacy(0)
{

}

bool GameMsg_G2S_GetRankAllDataForMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_RankMCoin);
    IOBuff.AddInt(m_RankHorn);
    IOBuff.AddInt(m_RankExp);
    IOBuff.AddInt(m_RankGroupHonor);
    IOBuff.AddInt(m_RankIntimacy);
    IOBuff.AddInt(m_RankMedalScore);
    IOBuff.AddInt(m_RankClothEffect);
    IOBuff.AddInt(m_RankEquipCount);
    return true;
}

bool GameMsg_G2S_GetRankAllDataForMedal::doDecode(CParamPool &IOBuff)
{
    m_RankMCoin         = IOBuff.GetInt();
    m_RankHorn          = IOBuff.GetInt();
    m_RankExp           = IOBuff.GetInt();
    m_RankGroupHonor    = IOBuff.GetInt();
    m_RankIntimacy      = IOBuff.GetInt();
    m_RankMedalScore    = IOBuff.GetInt();
    m_RankClothEffect   = IOBuff.GetInt();
    m_RankEquipCount    = IOBuff.GetInt();
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetRankAllData::GameMsg_S2G_GetRankAllData()
    :GameMsg_Base(MSG_S2G_Rank_GetAllRankData)
{

}

bool GameMsg_S2G_GetRankAllData::doDecode(CParamPool &IOBuff)
{
    m_nRoleId = IOBuff.GetInt();
    m_GroupId = IOBuff.GetInt();
    m_IntimacyId = IOBuff.GetInt();
    return true;
}

bool GameMsg_S2G_GetRankAllData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRoleId);
    IOBuff.AddInt(m_GroupId);
    IOBuff.AddInt(m_IntimacyId);
    return true;
}

GameMsg_G2S_GetRankAllData::GameMsg_G2S_GetRankAllData()
    : GameMsg_Base(MSG_G2S_Rank_GetAllRankDataResult)
    , m_RankMCoin(0)
    , m_RankHorn(0)
    , m_RankExp(0)
    , m_RankGroupHonor(0)
    , m_RankIntimacy(0)
{

}

bool GameMsg_G2S_GetRankAllData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_RankMCoin);
    IOBuff.AddInt(m_RankHorn);
    IOBuff.AddInt(m_RankExp);
    IOBuff.AddInt(m_RankGroupHonor);
    IOBuff.AddInt(m_RankIntimacy);
    IOBuff.AddInt(m_RankMedalScore);
    IOBuff.AddInt(m_RankClothEffect);
    IOBuff.AddInt(m_RankEquipCount);
    return true;
}

bool GameMsg_G2S_GetRankAllData::doDecode(CParamPool &IOBuff)
{
    m_RankMCoin         = IOBuff.GetInt();
    m_RankHorn          = IOBuff.GetInt();
    m_RankExp           = IOBuff.GetInt();
    m_RankGroupHonor    = IOBuff.GetInt();
    m_RankIntimacy      = IOBuff.GetInt();
    m_RankMedalScore    = IOBuff.GetInt();
    m_RankClothEffect   = IOBuff.GetInt();
    m_RankEquipCount    = IOBuff.GetInt();
    return true;
}


//////////////////////////////////////////////////////////////////////////

