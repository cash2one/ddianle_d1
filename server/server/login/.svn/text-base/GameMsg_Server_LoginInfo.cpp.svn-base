
#include "GameMsg_Server_LoginInfo.h"
#include "../../socket/ParamPool.h"
#include "../../server/share/ServerMsgDef.h"

GameMsg_Server_LoginInfo::GameMsg_Server_LoginInfo(void):_baseClass(MSG_SERVER_LoginInfo)
{
}

GameMsg_Server_LoginInfo::~GameMsg_Server_LoginInfo(void)
{
}

bool GameMsg_Server_LoginInfo::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUInt(nAccount);
	return true;
}
bool GameMsg_Server_LoginInfo::doDecode( CParamPool& IoBuff )
{
	nAccount = IoBuff.GetUInt();
	return true;
}

//end

