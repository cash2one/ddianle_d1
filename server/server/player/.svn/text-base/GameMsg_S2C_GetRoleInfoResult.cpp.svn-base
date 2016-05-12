#include "GameMsg_S2C_GetRoleInfoResult.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/DataStruct_DB.h"


GameMsg_S2C_GetRoleInfoResult::GameMsg_S2C_GetRoleInfoResult()
    : GameMsg_Base(MSG_S2C_GetRoleInfo)
{
}

GameMsg_S2C_GetRoleInfoResult::~GameMsg_S2C_GetRoleInfoResult()
{
}

bool GameMsg_S2C_GetRoleInfoResult::doEncode(CParamPool &IOBuff)
{
    m_playerInfo.doEncode(IOBuff);

    return true;
}

