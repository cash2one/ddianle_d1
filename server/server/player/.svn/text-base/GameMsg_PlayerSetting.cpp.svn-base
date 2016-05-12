#include "GameMsg_PlayerSetting.h"
#include "../share/ServerMsgDef.h"

GameMSG_S2C_SendPlayerSetting::GameMSG_S2C_SendPlayerSetting()
    :GameMsg_Base(MSG_S2C_SendPlayerSetting)
{

}

GameMSG_S2C_SendPlayerSetting::~GameMSG_S2C_SendPlayerSetting()
{

}

bool GameMSG_S2C_SendPlayerSetting::doEncode( CParamPool &IOBuff )
{
    m_playerSetting.doEncode(IOBuff);

    return true;
}

GameMsg_C2S_OptionSetting::GameMsg_C2S_OptionSetting()
    :GameMsg_Base(MSG_C2S_OptionSetting)
    ,m_nOption(0)
    ,m_nState(0)
{

}

GameMsg_C2S_OptionSetting::~GameMsg_C2S_OptionSetting()
{

}

bool GameMsg_C2S_OptionSetting::doDecode( CParamPool &IOBuff )
{
    m_nOption = IOBuff.GetUChar();
    m_nState = IOBuff.GetInt();

    return true;
}

GameMsg_S2C_OptionSettingSuc::GameMsg_S2C_OptionSettingSuc()
    :GameMsg_Base(MSG_S2C_OptionSettingSuc)
    ,m_nOption(0)
    ,m_nState(0)
{

}

GameMsg_S2C_OptionSettingSuc::~GameMsg_S2C_OptionSettingSuc()
{

}

bool GameMsg_S2C_OptionSettingSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUChar(m_nOption);
    IOBuff.AddInt(m_nState);

    return true;
}

GameMsg_S2C_OptionSettingFail::GameMsg_S2C_OptionSettingFail()
    :GameMsg_Base(MSG_S2C_OptionSettingFail)
    ,m_nOption(0)
    ,m_strError("")
{

}

GameMsg_S2C_OptionSettingFail::~GameMsg_S2C_OptionSettingFail()
{

}

bool GameMsg_S2C_OptionSettingFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUChar(m_nOption);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

//////////////////////////////////////////////////////
GameMsg_C2S_PlayerMoveTo::GameMsg_C2S_PlayerMoveTo()
    :GameMsg_Base(MSG_C2S_PlayerMoveTo)
    ,m_nSceneID(0)
    ,m_fOrient(0.0f)
    ,m_fDirectionX(0.0f)
    ,m_fDirectionZ(0.0f)
    ,m_fPosX(0.0f)
    ,m_fPosY(0.0f)
    ,m_fPosZ(0.0f)
{

}

GameMsg_C2S_PlayerMoveTo::~GameMsg_C2S_PlayerMoveTo()
{

}

bool GameMsg_C2S_PlayerMoveTo::doDecode( CParamPool &IOBuff )
{
    m_nSceneID = IOBuff.GetUShort();
    m_fOrient = IOBuff.GetFloat();
    m_fDirectionX = IOBuff.GetFloat();
    m_fDirectionZ = IOBuff.GetFloat();
    m_fPosX = IOBuff.GetFloat();
    m_fPosY = IOBuff.GetFloat();
    m_fPosZ = IOBuff.GetFloat();

    return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_PlayerMoveTo::GameMsg_S2C_PlayerMoveTo()
    :GameMsg_Base(MSG_S2C_PlayerMoveTo)
    ,m_nSceneID(0)
    ,m_nRoleID(0)
    ,m_fDirectionX(0.0f)
    ,m_fDirectionZ(0.0f)
    ,m_fPosX(0.0f)
    ,m_fPosY(0.0f)
    ,m_fPosZ(0.0f)
{

}

GameMsg_S2C_PlayerMoveTo::~GameMsg_S2C_PlayerMoveTo()
{

}

bool GameMsg_S2C_PlayerMoveTo::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort(m_nSceneID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddFloat(m_fDirectionX);
    IOBuff.AddFloat(m_fDirectionZ);
    IOBuff.AddFloat(m_fPosX);
    IOBuff.AddFloat(m_fPosY);
    IOBuff.AddFloat(m_fPosZ);

    return true;
}


//////////////////////////////////////////////////////
GameMsg_C2S_PlayerMoveType::GameMsg_C2S_PlayerMoveType()
    :GameMsg_Base(MSG_C2S_PlayerMoveType)
    ,m_nSceneID(0)
    ,m_nMoveType(0)
{

}

GameMsg_C2S_PlayerMoveType::~GameMsg_C2S_PlayerMoveType()
{

}

bool GameMsg_C2S_PlayerMoveType::doDecode( CParamPool &IOBuff )
{
    m_nSceneID = IOBuff.GetUShort();
    m_nMoveType = IOBuff.GetUChar();

    return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_PlayerMoveType::GameMsg_S2C_PlayerMoveType()
    :GameMsg_Base(MSG_S2C_PlayerMoveType)
    ,m_nSceneID(0)
    ,m_nRoleID(0)
    ,m_nMoveType(0)
{

}

GameMsg_S2C_PlayerMoveType::~GameMsg_S2C_PlayerMoveType()
{

}

bool GameMsg_S2C_PlayerMoveType::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort(m_nSceneID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar(m_nMoveType);

    return true;
}

/////////////////////////////////////////////////////
GameMsg_C2S_PlayerMotion::GameMsg_C2S_PlayerMotion()
    :GameMsg_Base(MSG_C2S_PlayerMotion)
    ,m_nTransFormID(0)
{

}

GameMsg_C2S_PlayerMotion::~GameMsg_C2S_PlayerMotion()
{

}

bool GameMsg_C2S_PlayerMotion::doDecode( CParamPool &IOBuff )
{
    m_nTransFormID = IOBuff.GetUShort();

    return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_PlayerMotion::GameMsg_S2C_PlayerMotion()
    :GameMsg_Base(MSG_S2C_PlayerMotion)
    ,m_nRoleID(0)
    ,m_nTransFormID(0)
{

}

GameMsg_S2C_PlayerMotion::~GameMsg_S2C_PlayerMotion()
{

}

bool GameMsg_S2C_PlayerMotion::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt( m_nRoleID );
    IOBuff.AddUShort( m_nTransFormID );

    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_PlayerDownloading::GameMsg_C2S_PlayerDownloading()
    : GameMsg_Base(MSG_C2S_PlayerDownloading)
    , m_nProgress(0)
{

}

GameMsg_C2S_PlayerDownloading::~GameMsg_C2S_PlayerDownloading()
{
}

bool GameMsg_C2S_PlayerDownloading::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nProgress);

    return true;
}

bool GameMsg_C2S_PlayerDownloading::doDecode( CParamPool &IOBuff )
{
    m_nProgress = IOBuff.GetUChar();

    return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_PlayerDownloading::GameMsg_S2C_PlayerDownloading()
    : GameMsg_Base(MSG_S2C_PlayerDownloading)
    , m_nRoleID(0)
    , m_nProgress(0)
{

}

GameMsg_S2C_PlayerDownloading::~GameMsg_S2C_PlayerDownloading()
{
}

bool GameMsg_S2C_PlayerDownloading::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar(m_nProgress);

    return true;
}


bool GameMsg_S2C_PlayerDownloading::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nProgress = IOBuff.GetUChar();

    return true;
}

