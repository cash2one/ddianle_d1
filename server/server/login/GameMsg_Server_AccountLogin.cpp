#include "GameMsg_Server_AccountLogin.h"
#include "../share/ServerMsgDef.h"
GameMsg_Server_AccountLogin::GameMsg_Server_AccountLogin(void):_baseClass(MSG_SERVER_AccountLogin)
{
}

GameMsg_Server_AccountLogin::~GameMsg_Server_AccountLogin(void)
{
}
bool GameMsg_Server_AccountLogin::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUInt(nAccount);
	return true;
}
bool GameMsg_Server_AccountLogin::doDecode( CParamPool& IoBuff )
{
	nAccount = IoBuff.GetUInt();
	return true;
}


//end

