#include "GameMsg_Medal.h"

GameMsg_C2S_GetMedalInfo::GameMsg_C2S_GetMedalInfo()
    :GameMsg_Base(MSG_C2S_GET_MEDALINFO)
{

}

GameMsg_C2S_GetMedalInfo::~GameMsg_C2S_GetMedalInfo()
{

}

bool GameMsg_C2S_GetMedalInfo::doDecode( CParamPool &IOBuff )
{
    m_vecMedalID.clear();
    int nSize = (int) IOBuff.GetUShort();
    while(nSize-- > 0)
    {
        m_vecMedalID.push_back(IOBuff.GetInt());
    }

    return true;
}


GameMsg_S2C_GetMedalInfo::GameMsg_S2C_GetMedalInfo()
    :GameMsg_Base(MSG_S2C_GET_MEDALINFO)
{

}

GameMsg_S2C_GetMedalInfo::~GameMsg_S2C_GetMedalInfo()
{
}

bool GameMsg_S2C_GetMedalInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerMedalInfo.size());
    for (std::list<PerMedalInfo>::iterator it = m_listPerMedalInfo.begin();
        m_listPerMedalInfo.end() != it; ++it)
    {
        (*it).doEncode(IOBuff);
    }
    return true;
}

GameMsg_S2C_GetMedalInfo::PerMedalInfo::PerMedalInfo()
    : nMedalID(0)
    , nState(0)
    , nNeedMedalMoney(0)
    , nType(0)
{
}

bool GameMsg_S2C_GetMedalInfo::PerMedalInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(nMedalID);
    IOBuff.AddInt(nState);
    IOBuff.AddInt(nNeedMedalMoney);
    IOBuff.AddInt(nType);
    IOBuff.AddStringW(strName);
    IOBuff.AddStringW(strIcon);
    IOBuff.AddStringW(strDescribe);
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMSg_C2S_GetConditionInfo::GameMSg_C2S_GetConditionInfo()
    :GameMsg_Base(MSG_C2S_GET_CONDITIONINFO)
{

}

GameMSg_C2S_GetConditionInfo::~GameMSg_C2S_GetConditionInfo()
{

}

bool GameMSg_C2S_GetConditionInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMSg_S2C_GetConditionInfo::GameMSg_S2C_GetConditionInfo()
    :GameMsg_Base(MSG_S2C_GET_CONDITIONINFO)
{

}

GameMSg_S2C_GetConditionInfo::~GameMSg_S2C_GetConditionInfo()
{
}

GameMSg_S2C_GetConditionInfo::PerConditionInfo::PerConditionInfo()
    : nCondititonID(0)
    , nRewardMedalID(0)
    , nCurProgress(0)
    , nTotalProtgress(0)
    , isHaveGot(0)
{
}

bool GameMSg_S2C_GetConditionInfo::PerConditionInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(nCondititonID);
    IOBuff.AddInt(nTypeID);
    IOBuff.AddInt(nRewardMedalID);
    IOBuff.AddInt(nCurProgress);
    IOBuff.AddInt(nTotalProtgress);
    IOBuff.AddUChar(isHaveGot);
    unsigned short nSize = (unsigned short)m_listDestID.size();
    IOBuff.AddUShort(nSize);
    for (std::list<unsigned int>::iterator it = m_listDestID.begin();
        m_listDestID.end() != it; ++it)
    {
        IOBuff.AddUInt( *it );
    }
    return true;
}

bool GameMSg_S2C_GetConditionInfo::doEncode(CParamPool &IOBuff)
{

    IOBuff.AddUShort((unsigned short)m_listPerConditionInfo.size() );
    for (std::list<PerConditionInfo>::iterator it = m_listPerConditionInfo.begin();
        m_listPerConditionInfo.end() != it; ++it)
    {
        (*it).doEncode(IOBuff);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_MedalHandBook::GameMsg_C2S_MedalHandBook()
:GameMsg_Base(MSG_C2S_MEDALHANDBOOK)
{

}

GameMsg_C2S_MedalHandBook::~GameMsg_C2S_MedalHandBook()
{

}

bool GameMsg_C2S_MedalHandBook::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_MedalHandBook::GameMsg_S2C_MedalHandBook()
    :GameMsg_Base(MSG_S2C_MEDALHANDBOOK)
{

}

GameMsg_S2C_MedalHandBook::~GameMsg_S2C_MedalHandBook()
{

}

bool GameMsg_S2C_MedalHandBook::PerMedalInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nMedalId);
    IOBuff.AddInt(m_nType);
    IOBuff.AddInt(m_nDuration);
    return true;
}
bool GameMsg_S2C_MedalHandBook::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerMedalInfo.size() );
    for (std::list<PerMedalInfo >::iterator it = m_listPerMedalInfo.begin();
        m_listPerMedalInfo.end() != it; ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_Medal_Exchange_Info::GameMsg_C2S_Medal_Exchange_Info()
    :GameMsg_Base(MSG_C2S_MEDAL_EXCHANGE_INFO)
{

}

GameMsg_C2S_Medal_Exchange_Info::~GameMsg_C2S_Medal_Exchange_Info()
{

}

bool GameMsg_C2S_Medal_Exchange_Info::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_S2C_Medal_Exchange_Info::GameMsg_S2C_Medal_Exchange_Info()
    :GameMsg_Base(MSG_S2C_MEDAL_EXCHANGE_INFO)
{

}

GameMsg_S2C_Medal_Exchange_Info::~GameMsg_S2C_Medal_Exchange_Info()
{

}

bool GameMsg_S2C_Medal_Exchange_Info::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerExchangeInfo.size() );
    for (std::list<PerExchangeInfo>::iterator it = m_listPerExchangeInfo.begin();
        m_listPerExchangeInfo.end() != it; ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}
bool GameMsg_S2C_Medal_Exchange_Info::PerExchangeInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nMedalId);
    IOBuff.AddInt(m_nPrice);
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMSg_C2S_Medal_Show::GameMSg_C2S_Medal_Show()
    :GameMsg_Base(MSG_C2S_MEDAL_SHOW)
{

}

GameMSg_C2S_Medal_Show::~GameMSg_C2S_Medal_Show()
{

}

bool GameMSg_C2S_Medal_Show::doDecode(CParamPool &IOBuff)
{
    m_listMedalID.clear();
    int nSize = (int) IOBuff.GetUShort();
    while(nSize-- > 0)
    {
        m_listMedalID.push_back(IOBuff.GetInt());
    }
    return true;
}


GameMSg_S2C_Medal_Show::GameMSg_S2C_Medal_Show():GameMsg_Base(MSG_S2C_MEDAL_SHOW)
{

}

GameMSg_S2C_Medal_Show::~GameMSg_S2C_Medal_Show()
{

}

bool GameMSg_S2C_Medal_Show::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nReCode );
    IOBuff.AddUShort( (unsigned short)m_listPerShowID.size() );
    for (std::list<int>::iterator it = m_listPerShowID.begin();
        m_listPerShowID.end() != it; ++it)
    {
        IOBuff.AddInt(*it);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_Medal_Exchange::GameMsg_C2S_Medal_Exchange()
    :GameMsg_Base(MSG_C2S_MEDAL_EXCHANGE)
{

}

GameMsg_C2S_Medal_Exchange::~GameMsg_C2S_Medal_Exchange()
{

}

bool GameMsg_C2S_Medal_Exchange::doDecode(CParamPool &IOBuff)
{
    m_nMedalID = IOBuff.GetInt();
    m_byScene = IOBuff.GetUChar();
    return true;
}


GameMsg_S2C_Medal_Exchange::GameMsg_S2C_Medal_Exchange()
    :GameMsg_Base(MSG_S2C_MEDAL_EXCHANGE)
{

}

GameMsg_S2C_Medal_Exchange::~GameMsg_S2C_Medal_Exchange()
{

}

bool GameMsg_S2C_Medal_Exchange::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRetCode);
    IOBuff.AddInt(m_nMedalID);
    IOBuff.AddInt(m_nMedalType);
    return true;
}
//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_Medal_ActivityInfo::GameMsg_C2S_Medal_ActivityInfo()
    :GameMsg_Base(MSG_C2S_MEDAL_ACT_INFO)
{

}

GameMsg_S2C_Medal_ActivityInfo::GameMsg_S2C_Medal_ActivityInfo()
    :GameMsg_Base(MSG_S2C_MEDAL_ACT_INFO)
{

}

bool GameMsg_S2C_Medal_ActivityInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerInfo.size() );
    for(std::list<perShowInfo> ::iterator it = m_listPerInfo.begin()
        ;it != m_listPerInfo.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_Medal_GetConditionReward::GameMsg_C2S_Medal_GetConditionReward()
    :GameMsg_Base(MSG_C2S_MEDAL_CONDITION_REWARD)
{

}

GameMsg_S2C_Medal_GetConditionReward::GameMsg_S2C_Medal_GetConditionReward()
    :GameMsg_Base(MSG_S2C_MEDAL_CONDITION_REWARD)
{

}

bool GameMsg_S2C_Medal_GetConditionReward::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRetCode);
    IOBuff.AddInt(m_nMedalID);
    IOBuff.AddInt(m_nMedalType);
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MedalInit::GameMsg_S2C_MedalInit()
    :GameMsg_Base(MSG_S2C_MEDAL_SHOW_INIT)
{

}

bool GameMsg_S2C_MedalInit::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listPerShowMedal.size());

    for(std::list<perShowMedal> ::iterator it = m_listPerShowMedal.begin()
        ;it != m_listPerShowMedal.end(); ++it)
    {
        IOBuff.AddInt(it->m_MedalID);
        IOBuff.AddStringW(it->m_strIcon);
    }
    return true;

}

GameMsg_S2C_PushCompleteMedal::GameMsg_S2C_PushCompleteMedal()
    : GameMsg_Base( MSG_S2C_PUSH_CONDITION_COMPLETE_MEDAL )
{

}

GameMsg_S2C_PushCompleteMedal::~GameMsg_S2C_PushCompleteMedal()
{

}

bool GameMsg_S2C_PushCompleteMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_setAllMedalId.size() );
    for (std::set<int>::iterator it = m_setAllMedalId.begin(); 
        it != m_setAllMedalId.end(); ++it)
    {
        IOBuff.AddInt(*it);
    }

    return true;
}


GameMsg_C2S_GetMedalScore::GameMsg_C2S_GetMedalScore()
    :GameMsg_Base(MSG_C2S_GetMedalScore)
{}

bool GameMsg_C2S_GetMedalScore::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetMedalScore::GameMsg_S2C_GetMedalScore()
    :GameMsg_Base(MSG_S2C_GetMedalScore)
{}

bool GameMsg_S2C_GetMedalScore::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nScore);
    return true;
};


GameMsg_C2S_Medal_GetScoreDescribe::GameMsg_C2S_Medal_GetScoreDescribe()
    :GameMsg_Base(MSG_C2S_MEDALSCORE_DESCRIBE)
{

}

GameMsg_S2C_Medal_GetScoreDescribe::GameMsg_S2C_Medal_GetScoreDescribe()
    : GameMsg_Base(MSG_S2C_MEDALSCORE_DESCRIBE)
{

}
