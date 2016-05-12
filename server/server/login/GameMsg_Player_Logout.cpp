#include "GameMsg_Player_Logout.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"
GameMsg_Player_Logout::GameMsg_Player_Logout(void):_baseClass(MSG_PLAYER_LOGOUT)
{
}

GameMsg_Player_Logout::~GameMsg_Player_Logout(void)
{
}
bool GameMsg_Player_Logout::doDecode( CParamPool& IoBuff )
{
	return true;
}
//file end

