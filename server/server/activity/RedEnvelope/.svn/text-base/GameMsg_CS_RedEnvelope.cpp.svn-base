#include "GameMsg_CS_RedEnvelope.h"
#include "../../share/ServerMsgDef.h"
#include <algorithm>

GameMsg_C2S_GetCanOpenRedEnvelopeCount::GameMsg_C2S_GetCanOpenRedEnvelopeCount()
    : GameMsg_Base(MSG_C2S_GetCanOpenRedEnevlopeCount)
{

}

GameMsg_C2S_GetCanOpenRedEnvelopeCount::~GameMsg_C2S_GetCanOpenRedEnvelopeCount()
{

}

bool GameMsg_C2S_GetCanOpenRedEnvelopeCount::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_GetCanOpenRedEnvelopeCount::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_UpdateCanOpenRedEnvelopeCount::GameMsg_S2C_UpdateCanOpenRedEnvelopeCount()
    : GameMsg_Base(MSG_S2C_UpdateCanOpenRedEnevlopeCount)
    , m_nCanGetNum(0)
{

}

GameMsg_S2C_UpdateCanOpenRedEnvelopeCount::~GameMsg_S2C_UpdateCanOpenRedEnvelopeCount()
{

}

bool GameMsg_S2C_UpdateCanOpenRedEnvelopeCount::doDecode(CParamPool &IOBuff)
{
    m_nCanGetNum = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2C_UpdateCanOpenRedEnvelopeCount::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nCanGetNum);
    return true;
}

GameMsg_C2S_GetRedEnvelopeList::GameMsg_C2S_GetRedEnvelopeList()
    : GameMsg_Base(MSG_C2S_GetRedEnvelopeList)
{

}

GameMsg_C2S_GetRedEnvelopeList::~GameMsg_C2S_GetRedEnvelopeList()
{

}

bool GameMsg_C2S_GetRedEnvelopeList::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_GetRedEnvelopeList::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetRedEnvelopeListResult::GameMsg_S2C_GetRedEnvelopeListResult()
    : GameMsg_Base(MSG_S2C_GetRedEnvelopeListResult)
    , m_bAnonymity(false)
    , m_bActivityShow(false)
    , m_nRoleID(0)
{

}

GameMsg_S2C_GetRedEnvelopeListResult::~GameMsg_S2C_GetRedEnvelopeListResult()
{

}

bool GameMsg_S2C_GetRedEnvelopeListResult::doDecode(CParamPool &IOBuff)
{

    return true;
}

bool GameMsg_S2C_GetRedEnvelopeListResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bAnonymity);
    IOBuff.AddBool(m_bActivityShow);
    IOBuff.AddUShort((unsigned short)m_listBriefRedEnvelope.size());
    std::list<BriefRedEnvelope>::iterator itr = m_listBriefRedEnvelope.begin();
    for (; itr != m_listBriefRedEnvelope.end(); ++itr)
    {
        itr->doEncode(IOBuff);        
    }
    return true;
}

GameMsg_C2S_GetRedEnvelopeDetails::GameMsg_C2S_GetRedEnvelopeDetails()
    : GameMsg_Base(MSG_C2S_GetRedEnvelopeDetailes)
    , m_nRedEnvelopeID(0)
{

}

GameMsg_C2S_GetRedEnvelopeDetails::~GameMsg_C2S_GetRedEnvelopeDetails()
{

}

bool GameMsg_C2S_GetRedEnvelopeDetails::doDecode(CParamPool &IOBuff)
{
    m_nRedEnvelopeID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_C2S_GetRedEnvelopeDetails::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeID);
    return true;
}

GameMsg_S2C_GetRedEnvelopeDetailsFail::GameMsg_S2C_GetRedEnvelopeDetailsFail()
    : GameMsg_Base(MSG_S2C_GetRedEnvelopeDetailesFail)
    , m_nError(0)
{

}

GameMsg_S2C_GetRedEnvelopeDetailsFail::~GameMsg_S2C_GetRedEnvelopeDetailsFail()
{

}

bool GameMsg_S2C_GetRedEnvelopeDetailsFail::doDecode(CParamPool &IOBuff)
{
    m_nError = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2C_GetRedEnvelopeDetailsFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nError);
    return true;
}

GameMsg_S2C_GetRedEnvelopeDetailsResult::GameMsg_S2C_GetRedEnvelopeDetailsResult()
    : GameMsg_Base(MSG_S2C_GetRedEnvelopeDetailesResult)
{

}

GameMsg_S2C_GetRedEnvelopeDetailsResult::~GameMsg_S2C_GetRedEnvelopeDetailsResult()
{

}

bool GameMsg_S2C_GetRedEnvelopeDetailsResult::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_S2C_GetRedEnvelopeDetailsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_RedEnvelope.m_strSendRoleName);
    IOBuff.AddUInt(m_RedEnvelope.m_nCurrencyType);
    IOBuff.AddUInt(m_RedEnvelope.m_nCurrencyValue);
    IOBuff.AddUInt((unsigned int)m_RedEnvelope.m_mapDetail.size());

    IOBuff.AddUShort((unsigned short)m_RedEnvelope.m_mapOpenRole.size());
    std::map<unsigned int, OpenRoleInfo>::iterator it = m_RedEnvelope.m_mapOpenRole.begin();
    for (;it != m_RedEnvelope.m_mapOpenRole.end(); ++it)
    {
        IOBuff.AddStringW(it->second.m_strRoleName);
        IOBuff.AddUInt(it->second.m_nCurrencyValue);
    }
    return true;
}

GameMsg_S2C_NewRedEnvelopeNotice::GameMsg_S2C_NewRedEnvelopeNotice()
    : GameMsg_Base(MSG_S2C_NewRedEnvelopesNotice)
    , m_strSendRoleName("")
{

}

GameMsg_S2C_NewRedEnvelopeNotice::~GameMsg_S2C_NewRedEnvelopeNotice()
{

}

bool GameMsg_S2C_NewRedEnvelopeNotice::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strSendRoleName);
    return true;
}

bool GameMsg_S2C_NewRedEnvelopeNotice::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strSendRoleName);
    return true;
}

GameMsg_C2S_SetAnonymity::GameMsg_C2S_SetAnonymity()
    : GameMsg_Base(MSG_C2S_SetAnonymity)
{

}

GameMsg_C2S_SetAnonymity::~GameMsg_C2S_SetAnonymity()
{

}

bool GameMsg_C2S_SetAnonymity::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_SetAnonymity::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_UpdateAnonymity::GameMsg_S2C_UpdateAnonymity()
    : GameMsg_Base(MSG_S2C_UpdateAnonymity)
    , m_bAnonymity(false)
{

}

GameMsg_S2C_UpdateAnonymity::~GameMsg_S2C_UpdateAnonymity()
{

}

bool GameMsg_S2C_UpdateAnonymity::doDecode(CParamPool &IOBuff)
{
    m_bAnonymity = IOBuff.GetBool();
    return true;
}

bool GameMsg_S2C_UpdateAnonymity::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bAnonymity);
    return true;
}

GameMsg_C2S_OpenRedEnvelope::GameMsg_C2S_OpenRedEnvelope()
    : GameMsg_Base(MSG_C2S_OpenRedEnvelope)
    , m_nRedEnvelopeID(0)
{

}

GameMsg_C2S_OpenRedEnvelope::~GameMsg_C2S_OpenRedEnvelope()
{

}

bool GameMsg_C2S_OpenRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nRedEnvelopeID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_C2S_OpenRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeID);
    return true;
}

GameMsg_S2C_OpenRedEnvelopeResult::GameMsg_S2C_OpenRedEnvelopeResult()
    : GameMsg_Base(MSG_S2C_OpenRedEnvelopesResult)
    , m_nResult(0)
    , m_nCurrencyType(0)
    , m_nCurrencyValue(0)
    , m_nOpenTime(0)
{

}

GameMsg_S2C_OpenRedEnvelopeResult::~GameMsg_S2C_OpenRedEnvelopeResult()
{

}

bool GameMsg_S2C_OpenRedEnvelopeResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUInt();
    m_nCurrencyType = IOBuff.GetInt();
    m_nCurrencyValue = IOBuff.GetInt();
    m_nOpenTime = IOBuff.GetUInt();
    return true;
}

bool GameMsg_S2C_OpenRedEnvelopeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeID);
    IOBuff.AddUInt(m_nResult);
    IOBuff.AddInt(m_nCurrencyType);
    IOBuff.AddInt(m_nCurrencyValue);
    IOBuff.AddUInt(m_nOpenTime);
    return true;
}



