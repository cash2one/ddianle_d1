#include "GameMsg_Server_AccountLogout.h"
#include "../share/ServerMsgDef.h"

GameMsg_Server_AccountLogout::GameMsg_Server_AccountLogout(unsigned int _nAccout):_baseClass(MSG_SERVER_AccountLogout)
{
	nAccount = _nAccout;
}

GameMsg_Server_AccountLogout::~GameMsg_Server_AccountLogout()
{
}

bool GameMsg_Server_AccountLogout::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUInt(nAccount);
	return true;
}

bool GameMsg_Server_AccountLogout::doDecode( CParamPool& IoBuff )
{
	nAccount = IoBuff.GetUInt();
	return true;
}

//end

