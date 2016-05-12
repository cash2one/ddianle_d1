#include "GameMsg_Horse.h"

GameMsg_C2S_MountHorse::GameMsg_C2S_MountHorse()
    :GameMsg_Base(MSG_C2S_MountHorse)
    , m_nHorseTypeID(0)
{

}

GameMsg_C2S_MountHorse::~GameMsg_C2S_MountHorse()
{

}

bool GameMsg_C2S_MountHorse::doDecode( CParamPool &IOBuff )
{
    m_nHorseTypeID = IOBuff.GetUInt();
    m_nSceneId = IOBuff.GetUShort();
    return true;
}


GameMsg_S2C_MountHorseRet::GameMsg_S2C_MountHorseRet()
    :GameMsg_Base(MSG_S2C_MountHorseRet)
    ,m_byFlag(0)
{

}

GameMsg_S2C_MountHorseRet::~GameMsg_S2C_MountHorseRet()
{

}

bool GameMsg_S2C_MountHorseRet::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUChar(m_byFlag);
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_UnmountHorseRet::GameMsg_S2C_UnmountHorseRet()
    :GameMsg_Base(MSG_S2C_UnmountHorseRet)
    , m_byFlag(0)
{

}

GameMsg_S2C_UnmountHorseRet::~GameMsg_S2C_UnmountHorseRet()
{

}

bool GameMsg_S2C_UnmountHorseRet::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_byFlag;
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_SendHorseInvite::GameMsg_C2S_SendHorseInvite()
    :GameMsg_Base(MSG_C2S_SendHorseInvite)
{

}

GameMsg_C2S_SendHorseInvite::~GameMsg_C2S_SendHorseInvite()
{

}

bool GameMsg_C2S_SendHorseInvite::doDecode( CParamPool &IOBuff )
{
    int nSize = (int)IOBuff.GetUShort();
    for (int i = 0 ; i< nSize ; ++i)
    {
        m_ListInviteID.push_back( IOBuff.GetUInt() );
    }
    m_nSceneId = IOBuff.GetUShort();
    return true;
}

GameMsg_S2C_SendHorseInviteRet::GameMsg_S2C_SendHorseInviteRet()
    :GameMsg_Base(MSG_S2C_SendHorseInvuteRet)
    ,m_byFlag(0)
{

}

GameMsg_S2C_SendHorseInviteRet::~GameMsg_S2C_SendHorseInviteRet()
{

}

bool GameMsg_S2C_SendHorseInviteRet::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_byFlag;
    return true;
}


GameMsg_S2C_RecvHorseInvite::GameMsg_S2C_RecvHorseInvite()
:GameMsg_Base(MSG_S2C_RecvHorseInvite)
, m_nInviteSendRoleId(0)
, m_nHorseType(0)
, m_strInviterName("")
, m_nSceneId(0)
{

}

GameMsg_S2C_RecvHorseInvite::~GameMsg_S2C_RecvHorseInvite()
{

}

bool GameMsg_S2C_RecvHorseInvite::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nHorseType);
    IOBuff.AddUInt(m_nInviteSendRoleId);
    IOBuff.AddStringW(m_strInviterName);
    IOBuff.AddUShort(m_nSceneId);
    return true;
}

GameMsg_C2S_RecvHorseInviteRet::GameMsg_C2S_RecvHorseInviteRet()
    :GameMsg_Base(MSG_C2S_RecvHorseInviteRet)
    ,m_bIsAccept(true)
    ,m_nHorseType(0)
    ,m_nInviteSendRoleId(0)
{

}

GameMsg_C2S_RecvHorseInviteRet::~GameMsg_C2S_RecvHorseInviteRet()
{

}

bool GameMsg_C2S_RecvHorseInviteRet::doDecode( CParamPool &IOBuff )
{
    m_bIsAccept = IOBuff.GetBool();
    m_nHorseType = IOBuff.GetUInt();
    m_nInviteSendRoleId = IOBuff.GetUInt();
    m_nSenceId = IOBuff.GetUShort();
    return true;
}


GameMsg_S2C_RecvHorseInviteRetRes::GameMsg_S2C_RecvHorseInviteRetRes()
    :GameMsg_Base(MSG_S2C_RecvHorseInviteRetRes)
{

}

GameMsg_S2C_RecvHorseInviteRetRes::~GameMsg_S2C_RecvHorseInviteRetRes()
{

}

bool GameMsg_S2C_RecvHorseInviteRetRes::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort(m_nFlag);
    return true;
}

