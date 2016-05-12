#include "GameMsg_System_Ping.h"
#include "MsgBaseStruct.h"

GameMsg_System_Ping::GameMsg_System_Ping(void)
:_baseClass(MSG_SYSTEM_Ping)
{
}

GameMsg_System_Ping::~GameMsg_System_Ping(void)
{
}

bool GameMsg_System_Ping::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddInt(nTickCount);
	IoBuff.AddInt(nDirection);
	return TRUE;

}
bool GameMsg_System_Ping::doDecode( CParamPool &IoBuff )
{
	nTickCount = IoBuff.GetInt();
	nTickCount = IoBuff.GetInt();
	return TRUE;

}

//file end

