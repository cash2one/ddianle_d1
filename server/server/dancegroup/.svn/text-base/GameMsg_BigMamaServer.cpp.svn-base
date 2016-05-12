#include "DanceGroupMsgDef.h"
#include "GameMsg_BigMamaServer.h"



GameMsg_S2G_BigMamaStageInfoRequest::GameMsg_S2G_BigMamaStageInfoRequest()
    : GameMsg_Base(MSG_S2G_BigMamaStageInfoRequest)
    , m_nDanceGroupID(0)
{

}
GameMsg_S2G_BigMamaStageInfoRequest::~GameMsg_S2G_BigMamaStageInfoRequest()
{}
bool GameMsg_S2G_BigMamaStageInfoRequest::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    return true;
}
bool GameMsg_S2G_BigMamaStageInfoRequest::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    return true;
}



GameMsg_G2S_BigMamaStageInfoNotify::GameMsg_G2S_BigMamaStageInfoNotify()
    : GameMsg_Base(MSG_G2S_BigMamaStageInfoNotify)
    , m_result(0)
{

}
GameMsg_G2S_BigMamaStageInfoNotify::~GameMsg_G2S_BigMamaStageInfoNotify()
{}
bool GameMsg_G2S_BigMamaStageInfoNotify::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_result);
    if (0 == m_result)
        m_property.doEncode(IOBuff);
    return true;
}
bool GameMsg_G2S_BigMamaStageInfoNotify::doDecode(CParamPool& IOBuff)
{
    m_result = IOBuff.GetUChar();
    if (0 == m_result)
        m_property.doDecode(IOBuff);

    return true;
}


GameMsg_S2G_BigMamaBeatSucess::GameMsg_S2G_BigMamaBeatSucess()
    : GameMsg_Base(MSG_S2G_BigMamaBeatSucess)
    , m_nDanceGroupID(0)
    , m_nRoleID(0)
    , m_nRound(0)
{

}
GameMsg_S2G_BigMamaBeatSucess::~GameMsg_S2G_BigMamaBeatSucess()
{

}
bool GameMsg_S2G_BigMamaBeatSucess::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar(m_nRound);
    return true;
}
bool GameMsg_S2G_BigMamaBeatSucess::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_nRound = IOBuff.GetUChar();

    return true;
}


GameMsg_G2S_BigMamaStateNotify::GameMsg_G2S_BigMamaStateNotify()
    : GameMsg_Base(MSG_G2S_BigMamaStateNotify)
    , m_nState(0)
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}
GameMsg_G2S_BigMamaStateNotify::~GameMsg_G2S_BigMamaStateNotify()
{}
bool GameMsg_G2S_BigMamaStateNotify::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nState);
    if (1 == m_nState)
    {
        IOBuff.AddUInt(m_nStartTime);
        IOBuff.AddUInt(m_nEndTime);
    }
    return true;
}
bool GameMsg_G2S_BigMamaStateNotify::doDecode(CParamPool& IOBuff)
{
    m_nState = IOBuff.GetUChar();
    if (1 == m_nState)
    {
        m_nStartTime = IOBuff.GetUInt();
        m_nEndTime = IOBuff.GetUInt();
    }

    return true;
}


GameMsg_G2S_BigMamaSyncStageInfo::GameMsg_G2S_BigMamaSyncStageInfo()
    : GameMsg_Base(MSG_G2S_BigMamaSyncStageInfo)
{

}
GameMsg_G2S_BigMamaSyncStageInfo::~GameMsg_G2S_BigMamaSyncStageInfo()
{}
bool GameMsg_G2S_BigMamaSyncStageInfo::doEncode(CParamPool& IOBuff)
{
    m_property.doEncode(IOBuff);
    return true;
}
bool GameMsg_G2S_BigMamaSyncStageInfo::doDecode(CParamPool& IOBuff)
{
    m_property.doDecode(IOBuff);

    return true;
}


GameMsg_G2S_BigMamaInfoNotifyOnLogin::GameMsg_G2S_BigMamaInfoNotifyOnLogin()
    : GameMsg_Base(MSG_G2S_BigMamaInfoNotifyOnLogin)
    , m_bOpened(false)
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}
GameMsg_G2S_BigMamaInfoNotifyOnLogin::~GameMsg_G2S_BigMamaInfoNotifyOnLogin()
{}
bool GameMsg_G2S_BigMamaInfoNotifyOnLogin::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bOpened);
    if (m_bOpened)
    {
        IOBuff.AddUInt(m_nStartTime);
        IOBuff.AddUInt(m_nEndTime);
        m_property.doEncode(IOBuff);
    }
    return true;
}
bool GameMsg_G2S_BigMamaInfoNotifyOnLogin::doDecode(CParamPool& IOBuff)
{
    m_bOpened = IOBuff.GetBool();
    if (m_bOpened)
    {
        m_nStartTime = IOBuff.GetUInt();
        m_nEndTime = IOBuff.GetUInt();
        m_property.doDecode(IOBuff);
    }

    return true;
}

GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup::GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup()
    : GameMsg_Base(MSG_G2S_BigMamaInfoNotifyOnJoinDanceGroup)
    , m_bOpened(false)
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}
GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup::~GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup()
{}
bool GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bOpened);
    if (m_bOpened)
    {
        IOBuff.AddUInt(m_nStartTime);
        IOBuff.AddUInt(m_nEndTime);
        m_property.doEncode(IOBuff);
    }
    return true;
}
bool GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup::doDecode(CParamPool& IOBuff)
{
    m_bOpened = IOBuff.GetBool();
    if (m_bOpened)
    {
        m_nStartTime = IOBuff.GetUInt();
        m_nEndTime = IOBuff.GetUInt();
        m_property.doDecode(IOBuff);
    }

    return true;
}


GameMsg_S2G_BigMamaFightRequest::GameMsg_S2G_BigMamaFightRequest()
    : GameMsg_Base(MSG_S2G_BigMamaFightRequest)
    , m_nDanceGroupID(0)
    , m_nRoleID(0)
    , m_nPhoneOS(0)
{
}
GameMsg_S2G_BigMamaFightRequest::~GameMsg_S2G_BigMamaFightRequest()
{}
bool GameMsg_S2G_BigMamaFightRequest::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar(m_nPhoneOS);

    return true;
}

bool GameMsg_S2G_BigMamaFightRequest::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_nPhoneOS = IOBuff.GetUChar();

    return true;
}


GameMsg_G2S_BigMamaFightResult::GameMsg_G2S_BigMamaFightResult()
    : GameMsg_Base(MSG_G2S_BigMamaFightResult)
    , m_nResult(EFightBigMamaResult_Sucess)
    , m_nStageID(0)
    , m_nRound(0)
    , m_nPhoneOS(0)
{

}
GameMsg_G2S_BigMamaFightResult::~GameMsg_G2S_BigMamaFightResult()
{}
bool GameMsg_G2S_BigMamaFightResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nResult);
    IOBuff.AddUInt(m_nStageID);
    IOBuff.AddUChar(m_nRound);
    IOBuff.AddUChar(m_nPhoneOS);

    return true;
}
bool GameMsg_G2S_BigMamaFightResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = (EFightBigMamaResult)IOBuff.GetUChar();
    m_nStageID = IOBuff.GetUInt();
    m_nRound = IOBuff.GetUChar();
    m_nPhoneOS = IOBuff.GetUChar();

    return true;
}

GameMsg_S2G_BigMamaGetRewardRequest::GameMsg_S2G_BigMamaGetRewardRequest()
    : GameMsg_Base(MSG_S2G_BigMamaGetRewardRequest)
    , m_nDanceGroupID(0)
    , m_nRoleID(0)
    , m_bJoined(false)
{

}
GameMsg_S2G_BigMamaGetRewardRequest::~GameMsg_S2G_BigMamaGetRewardRequest()
{}
bool GameMsg_S2G_BigMamaGetRewardRequest::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddBool(m_bJoined);

    return true;
}
bool GameMsg_S2G_BigMamaGetRewardRequest::doDecode(CParamPool& IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_bJoined = IOBuff.GetBool();

    return true;
}



GameMsg_G2S_BigMamaGetRewardResult::GameMsg_G2S_BigMamaGetRewardResult()
    : GameMsg_Base(MSG_G2S_BigMamaGetRewardResult)
    , m_nErrorType(EBigMamaRewardResult_Sucess)
    , m_rewardType(0)
{

}
GameMsg_G2S_BigMamaGetRewardResult::~GameMsg_G2S_BigMamaGetRewardResult()
{}
bool GameMsg_G2S_BigMamaGetRewardResult::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nErrorType);
    IOBuff.AddUChar(m_rewardType);

    return true;
}
bool GameMsg_G2S_BigMamaGetRewardResult::doDecode(CParamPool& IOBuff)
{
    m_nErrorType = (EBigMamaRewardResult)IOBuff.GetUChar();
    m_rewardType = IOBuff.GetUChar();

    return true;
}



