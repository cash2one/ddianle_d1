#include "GameMsg_Server_LoginResult.h"
#include "../../socket/ParamPool.h"
#include "../../server/share/ServerMsgDef.h"

GameMsg_Server_LoginResult::GameMsg_Server_LoginResult(void):_baseClass(MSG_SERVER_LoginResult)
{
	nErrCode = 0;
}

GameMsg_Server_LoginResult::~GameMsg_Server_LoginResult(void)
{
}

bool GameMsg_Server_LoginResult::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddShort(nErrCode);
	return true;
}

//end

