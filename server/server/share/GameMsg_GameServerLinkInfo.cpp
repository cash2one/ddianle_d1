#include "GameMsg_GameServerLinkInfo.h"
#include "ServerMsgDef.h"


GameMsg_S2G_GameServerLinkInfo::GameMsg_S2G_GameServerLinkInfo()
    : GameMsg_Base( MSG_S2G_GameServerLinkInfo )
{
}

GameMsg_S2G_GameServerLinkInfo::~GameMsg_S2G_GameServerLinkInfo()
{
}

bool GameMsg_S2G_GameServerLinkInfo::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_linkInfo;

    return true;
}

bool GameMsg_S2G_GameServerLinkInfo::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_linkInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GameServerLinkInfoResult::GameMsg_G2S_GameServerLinkInfoResult()
    : GameMsg_Base( MSG_G2S_GameServerLinkInfoResult )
    , m_nServerType( SERVER_GAME )
{
}

GameMsg_G2S_GameServerLinkInfoResult::~GameMsg_G2S_GameServerLinkInfoResult()
{
}

bool GameMsg_G2S_GameServerLinkInfoResult::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nServerType;

    return true;
}

bool GameMsg_G2S_GameServerLinkInfoResult::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nServerType;

    return true;
}

