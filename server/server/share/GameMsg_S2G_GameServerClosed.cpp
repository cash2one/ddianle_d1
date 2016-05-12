#include "GameMsg_S2G_GameServerClosed.h"

#include "ServerMsgDef.h"

GameMsg_S2G_GameServerClosed::GameMsg_S2G_GameServerClosed(void)
    : GameMsg_Base(MSG_S2G_GameServerClosed)
{
}

bool GameMsg_S2G_GameServerClosed::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_S2G_GameServerClosed::doEncode(CParamPool &IOBuff)
{
    return true;
}

