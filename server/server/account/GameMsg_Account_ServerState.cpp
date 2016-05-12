#include "GameMsg_Account_ServerState.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"

GameMsg_Account_ServerState::GameMsg_Account_ServerState(void):_baseClass(MSG_ACCOUNT_ServerState)
{
}

GameMsg_Account_ServerState::~GameMsg_Account_ServerState(void)
{
}
bool GameMsg_Account_ServerState::doDecode( CParamPool& IoBuff )
{
	return true;
}


//end file


