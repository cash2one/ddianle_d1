#include "GameMsg_GS_RedEnvelope.h"
#include "../../share/ServerMsgDef.h"

GameMsg_S2G_CreateRedEnvelope::GameMsg_S2G_CreateRedEnvelope()
    : GameMsg_Base(MSG_S2G_CreateRedEnvelope)
    , m_nRoleID(0)
    , m_strRoleName("")
    , m_nMCoin(0)
    , m_nDanceGroupID(0)
    , m_bIsAnonymity(false)
{

}

GameMsg_S2G_CreateRedEnvelope::~GameMsg_S2G_CreateRedEnvelope()
{

}

bool GameMsg_S2G_CreateRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nMCoin = IOBuff.GetInt();
    m_nDanceGroupID = IOBuff.GetUInt();
    m_bIsAnonymity = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2G_CreateRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddInt(m_nMCoin);
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddBool(m_bIsAnonymity);

    return true;
}

GameMsg_G2S_NewRedEnvelope::GameMsg_G2S_NewRedEnvelope()
    : GameMsg_Base(MSG_G2S_NewRedEnvelope)
    , m_nSendRoleID(0)
    , m_strRoleName("")
    , m_nMCoin(0)
{

}

GameMsg_G2S_NewRedEnvelope::~GameMsg_G2S_NewRedEnvelope()
{

}

bool GameMsg_G2S_NewRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nSendRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nMCoin = IOBuff.GetUInt();
    return true;
}

bool GameMsg_G2S_NewRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSendRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUInt(m_nMCoin);
    return true;
}

GameMsg_S2G_OpenRedEnvelope::GameMsg_S2G_OpenRedEnvelope()
    : GameMsg_Base(MSG_S2G_OpenRedEnvelope)
    , m_nRoleID(0)
    , m_nDanceGroupID(0)
    , m_nRedEnvelopeID(0)
{

}

GameMsg_S2G_OpenRedEnvelope::~GameMsg_S2G_OpenRedEnvelope()
{

}

bool GameMsg_S2G_OpenRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nRedEnvelopeID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    return true;
}

bool GameMsg_S2G_OpenRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUInt(m_nRedEnvelopeID);
    IOBuff.AddStringW(m_strRoleName);
    return true;
}

GameMsg_G2S_OpenRedEnvelope::GameMsg_G2S_OpenRedEnvelope()
    : GameMsg_Base(MSG_G2S_OpenRedEnvelope)
    , m_nResultFlag(0)
    , m_nCurrencyType(0)
    , m_nCurrencyValue(0)
    , m_nRedEnvelopeID(0)
    , m_nOpenTime(0)
{

}

GameMsg_G2S_OpenRedEnvelope::~GameMsg_G2S_OpenRedEnvelope()
{

}

bool GameMsg_G2S_OpenRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nResultFlag = IOBuff.GetUInt();
    m_nCurrencyType = IOBuff.GetInt();
    m_nCurrencyValue = IOBuff.GetInt();
    m_nRedEnvelopeID = IOBuff.GetUInt();
    m_nOpenTime = IOBuff.GetUInt();
    return true;
}

bool GameMsg_G2S_OpenRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nResultFlag);
    IOBuff.AddInt(m_nCurrencyType);
    IOBuff.AddInt(m_nCurrencyValue);
    IOBuff.AddUInt(m_nRedEnvelopeID);
    IOBuff.AddUInt(m_nOpenTime);
    return true;
}

GameMsg_S2G_GetRedEnvelopeList::GameMsg_S2G_GetRedEnvelopeList()
    : GameMsg_Base(MSG_S2G_GetRedEnvelopeList)
    , m_nRoleID(0)
    , m_nDanceGroupID(0)
{

}

GameMsg_S2G_GetRedEnvelopeList::~GameMsg_S2G_GetRedEnvelopeList()
{

}

bool GameMsg_S2G_GetRedEnvelopeList::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nDanceGroupID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2G_GetRedEnvelopeList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nDanceGroupID);
    return true;
}

GameMsg_G2S_GetRedEnvelopeList::GameMsg_G2S_GetRedEnvelopeList()
    : GameMsg_Base(MSG_G2S_GetRedEnvelopeList)
{

}

GameMsg_G2S_GetRedEnvelopeList::~GameMsg_G2S_GetRedEnvelopeList()
{

}

bool GameMsg_G2S_GetRedEnvelopeList::doDecode(CParamPool &IOBuff)
{
    unsigned short nIDCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nIDCount; ++i)
    {
        m_listGetRedEnvelopeID.push_back(IOBuff.GetUInt());
    }

    unsigned short nBriefCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nBriefCount; ++i)
    {
        BriefRedEnvelope brief;
        brief.doDecode(IOBuff);
        m_listRedEnvelope.push_back(brief);
    }
    return true;
}

bool GameMsg_G2S_GetRedEnvelopeList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listGetRedEnvelopeID.size());
    std::list<unsigned int>::iterator itr = m_listGetRedEnvelopeID.begin();
    for (; itr != m_listGetRedEnvelopeID.end(); ++itr)
    {
        IOBuff.AddUInt(*itr);
    }

    IOBuff.AddUShort((unsigned short)m_listRedEnvelope.size());
    std::list<BriefRedEnvelope>::iterator it = m_listRedEnvelope.begin();
    for (; it != m_listRedEnvelope.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}


GameMsg_S2G_GetRedEnvelopeDetails::GameMsg_S2G_GetRedEnvelopeDetails()
    : GameMsg_Base(MSG_S2G_GetRedEnvelopeDetails)
    , m_nRoleID(0)
    , m_nRedEnvelopeID(0)
{

}

GameMsg_S2G_GetRedEnvelopeDetails::~GameMsg_S2G_GetRedEnvelopeDetails()
{

}

bool GameMsg_S2G_GetRedEnvelopeDetails::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nRedEnvelopeID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2G_GetRedEnvelopeDetails::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nRedEnvelopeID);
    return true;
}

GameMsg_G2S_GetRedEnvelopeDetails::GameMsg_G2S_GetRedEnvelopeDetails()
    : GameMsg_Base(MSG_G2S_GetRedEnvelopeDetails)
{

}

GameMsg_G2S_GetRedEnvelopeDetails::~GameMsg_G2S_GetRedEnvelopeDetails()
{

}

bool GameMsg_G2S_GetRedEnvelopeDetails::doDecode(CParamPool &IOBuff)
{
    m_RedEnvelope.doDecode(IOBuff);
    return true;
}

bool GameMsg_G2S_GetRedEnvelopeDetails::doEncode(CParamPool &IOBuff)
{
    m_RedEnvelope.doEncode(IOBuff);
    return true;
}

GameMsg_S2G_GetCanOpenRedEnvelopeCount::GameMsg_S2G_GetCanOpenRedEnvelopeCount()
    : GameMsg_Base(MSG_S2G_GetCanOpenRedEnvelopeCount)
    , m_nRoleID(0)
    , m_nDanceGroupID(0)
{

}

GameMsg_S2G_GetCanOpenRedEnvelopeCount::~GameMsg_S2G_GetCanOpenRedEnvelopeCount()
{

}

bool GameMsg_S2G_GetCanOpenRedEnvelopeCount::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nDanceGroupID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2G_GetCanOpenRedEnvelopeCount::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nDanceGroupID);
    return true;
}

GameMsg_G2S_GetCanOpenRedEnvelopeCount::GameMsg_G2S_GetCanOpenRedEnvelopeCount()
    : GameMsg_Base(MSG_G2S_GetCanOpenRedEnvelopeCount)
    , m_nCanOpenCount(0)
{

}

GameMsg_G2S_GetCanOpenRedEnvelopeCount::~GameMsg_G2S_GetCanOpenRedEnvelopeCount()
{

}

bool GameMsg_G2S_GetCanOpenRedEnvelopeCount::doDecode(CParamPool &IOBuff)
{
    m_nCanOpenCount = IOBuff.GetUInt();
    return true;
}

bool GameMsg_G2S_GetCanOpenRedEnvelopeCount::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nCanOpenCount);
    return true;
}



