#ifndef __GAMEMSG_S2C_GETROLEINFORESULT_H__
#define __GAMEMSG_S2C_GETROLEINFORESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2C_GetRoleInfoResult:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRoleInfoResult);
    GameMsg_S2C_GetRoleInfoResult();
    ~GameMsg_S2C_GetRoleInfoResult();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    PlayerInfo m_playerInfo;

};

#endif

