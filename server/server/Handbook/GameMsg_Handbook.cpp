#include "GameMsg_Handbook.h"

GameMsg_S2G_GolbalCostumeData::GameMsg_S2G_GolbalCostumeData()
    :GameMsg_Base(MSG_S2G_HandBookCostumeGolbalInfo)
{}

GameMsg_G2S_GolbalCostumeData::GameMsg_G2S_GolbalCostumeData()
    :GameMsg_Base(MSG_G2S_HandBookCostumeGolbalInfo), m_isEnd(false)
{}

bool GameMsg_G2S_GolbalCostumeData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_isEnd);
    IOBuff.AddUShort((unsigned short)m_listPerInfo.size() );
    for (std::list<GolbalPerInfo>::iterator it = m_listPerInfo.begin();
        m_listPerInfo.end() != it; ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_S2G_GolbalEffectData::GameMsg_S2G_GolbalEffectData()
    :GameMsg_Base(MSG_S2G_HandBookEffectGolbalInfo)
{}

GameMsg_G2S_GolbalEffectData::GameMsg_G2S_GolbalEffectData()
    :GameMsg_Base(MSG_G2S_HandBookEffectGolbalInfo), m_isEnd(false)
{}

bool GameMsg_G2S_GolbalEffectData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_isEnd);
    IOBuff.AddUShort((unsigned short)m_listPerInfo.size() );
    for (std::list<GolbalPerInfo>::iterator it = m_listPerInfo.begin();
        m_listPerInfo.end() != it; ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

bool GolbalPerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nId);
    IOBuff.AddUInt(m_nPraisedCount);
    IOBuff.AddUInt(m_nEnvyCount);
    IOBuff.AddUInt(m_nCollectCount);
    return true;
}

bool GameMsg_G2S_GolbalEffectData::doDecode(CParamPool &IOBuff)
{
    m_isEnd = IOBuff.GetBool();
    unsigned int size = IOBuff.GetUShort();
    for (unsigned int i = 0; i < size ; i++)
    {
        GolbalPerInfo info;
        info.doDecode(IOBuff);
        m_listPerInfo.push_back(info);
    }
    return true;
}

bool GameMsg_G2S_GolbalCostumeData::doDecode(CParamPool &IOBuff)
{
    m_isEnd = IOBuff.GetBool();
    unsigned int size = IOBuff.GetUShort();
    for (unsigned int i = 0; i < size ; i++)
    {
        GolbalPerInfo info;
        info.doDecode(IOBuff);
        m_listPerInfo.push_back(info);
    }
    return true;
}

bool GolbalPerInfo::doDecode(CParamPool &IOBuff)
{
    m_nId = IOBuff.GetUInt();
    m_nPraisedCount = IOBuff.GetUInt();
    m_nEnvyCount = IOBuff.GetUInt();
    m_nCollectCount = IOBuff.GetUInt();
    return true;
}

GameMsg_S2G_ChangeGolbalData::GameMsg_S2G_ChangeGolbalData()
    :GameMsg_Base(MSG_S2G_HandBookChangeGolbalInfo),m_bIsAdd(false)
{

}

bool GameMsg_S2G_ChangeGolbalData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nHandBookType);
    IOBuff.AddInt(m_nId);
    IOBuff.AddInt(m_nChangeIndex);
    IOBuff.AddBool(m_bIsAdd);
    return true;
}

bool GameMsg_S2G_ChangeGolbalData::doDecode(CParamPool &IOBuff)
{
    m_nHandBookType = IOBuff.GetInt();
    m_nId = IOBuff.GetInt();
    m_nChangeIndex = IOBuff.GetInt();
    m_bIsAdd = IOBuff.GetBool();
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GolbalCostumeData::GameMsg_C2S_GolbalCostumeData()
    :GameMsg_Base(MSG_C2S_QueryHandBookCostumeInfo)
{}

GameMsg_S2C_GolbalCostumeData::GameMsg_S2C_GolbalCostumeData()
    :GameMsg_Base(Msg_S2C_QueryHandBookCostumeInfo),m_isEnd(false)
{}

bool GameMsg_S2C_GolbalCostumeData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerInfo.size() );
    for (std::list<GolbalPerInfo>::iterator it = m_listPerInfo.begin();
        m_listPerInfo.end() != it; ++it)
    {
        it->doEncode(IOBuff);
    }
    IOBuff.AddBool(m_isEnd);
    return true;
}

bool GameMsg_S2C_GolbalCostumeData::doDecode(CParamPool &IOBuff)
{
    m_isEnd = IOBuff.GetBool();
    unsigned int size = IOBuff.GetUShort();
    for (unsigned int i = 0; i< size ; ++i)
    {
        GolbalPerInfo info;
        info.doDecode(IOBuff);
        m_listPerInfo.push_back(info);
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_CostumeCollectData::GameMsg_C2S_CostumeCollectData()
    :GameMsg_Base(MSG_C2S_MyCollect)
{

}

GameMsg_S2C_CostumeCollectData::GameMsg_S2C_CostumeCollectData()
    :GameMsg_Base(MSG_S2C_MyCollect)
{

}

bool GameMsg_S2C_CostumeCollectData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort( (unsigned short)m_listCollect.size() );
    for (list<unsigned int>::iterator it = m_listCollect.begin() ;
        it != m_listCollect.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_CostumePersonalData::GameMsg_C2S_CostumePersonalData()
    :GameMsg_Base(MSG_C2S_CostumePersonalInfo)
{

}

GameMsg_S2C_CostumePersonalData::GameMsg_S2C_CostumePersonalData()
    :GameMsg_Base(MSG_S2C_CostumePersonalInfo)
    ,m_bIsPraised(false)
    ,m_bIsCollected(false)
    ,m_bIsEnvied(false)
{

}

bool GameMsg_S2C_CostumePersonalData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bIsPraised);
    IOBuff.AddBool(m_bIsCollected);
    IOBuff.AddBool(m_bIsEnvied);
    return true;
}

GameMsg_C2S_EffectPersonalData::GameMsg_C2S_EffectPersonalData()
    :GameMsg_Base(MSG_C2S_EffectPersonalInfo)
{

}

GameMsg_S2C_EffectPersonalData::GameMsg_S2C_EffectPersonalData()
    :GameMsg_Base(MSG_S2C_EffectPersonalInfo)
    ,m_bIsPraised(false)
    ,m_bIsCollected(false)
    ,m_bIsEnvied(false)
    ,m_nPraisedTotal(0)
    ,m_nCollectedTotal(0)
    ,m_nEnviedTotal(0)
{

}

bool GameMsg_S2C_EffectPersonalData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bIsPraised);
    IOBuff.AddBool(m_bIsCollected);
    IOBuff.AddBool(m_bIsEnvied);

    IOBuff.AddUInt(m_nPraisedTotal);
    IOBuff.AddUInt(m_nCollectedTotal);
    IOBuff.AddUInt(m_nEnviedTotal);
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_SetCostumeState::GameMsg_C2S_SetCostumeState()
    :GameMsg_Base(MSG_C2S_SetCostumeState)
{

}
bool GameMsg_C2S_SetCostumeState::doDecode(CParamPool &IOBuff)
{
    m_nId = IOBuff.GetUInt();
    m_byType = IOBuff.GetUChar();
    m_bFlag = IOBuff.GetBool();
    return true;
}

GameMsg_S2C_SetCostumeState::GameMsg_S2C_SetCostumeState()
    :GameMsg_Base(MSG_S2C_SetCostumeState)
{

}

bool GameMsg_S2C_SetCostumeState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_byRetCode);
    return true;
}

GameMsg_C2S_SetEffectState::GameMsg_C2S_SetEffectState()
    :GameMsg_Base(MSG_C2S_SetEffectState)
{

}
bool GameMsg_C2S_SetEffectState::doDecode(CParamPool &IOBuff)
{
    m_nId = IOBuff.GetUInt();
    m_byType = IOBuff.GetUChar();
    m_bFlag = IOBuff.GetBool();
    return true;
}

GameMsg_S2C_SetEffectState::GameMsg_S2C_SetEffectState()
    :GameMsg_Base(MSG_S2C_SetEffectState)
{

}

bool GameMsg_S2C_SetEffectState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_byRetCode);
    return true;
}


