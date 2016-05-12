#include "GameMsg_System_Disconnect.h"
#include "MsgBaseStruct.h"

GameMsg_System_Disconnect::GameMsg_System_Disconnect(void)
:_baseClass(MSG_SYSTEM_Disconnect)
{
	nMsg = MSG_SYSTEM_Disconnect;
}

GameMsg_System_Disconnect::~GameMsg_System_Disconnect(void)
{
}
bool GameMsg_System_Disconnect::doEncode( CParamPool &IoBuff )
{
	return TRUE;
}
bool GameMsg_System_Disconnect::doDecode( CParamPool &IoBuff )
{
	return TRUE;
}

//file end

