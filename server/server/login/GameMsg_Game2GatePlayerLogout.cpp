#include "GameMsg_Game2GatePlayerLogout.h"
#include "../share/ServerMsgDef.h"

GameMsg_Game2GatePlayerLogout::GameMsg_Game2GatePlayerLogout(void):_baseClass(MSG_Game2GatePlayerLogout)
{
}

GameMsg_Game2GatePlayerLogout::~GameMsg_Game2GatePlayerLogout(void)
{
}
bool GameMsg_Game2GatePlayerLogout::doEncode( CParamPool& IoBuff )
{
	return true;
}

//file end

