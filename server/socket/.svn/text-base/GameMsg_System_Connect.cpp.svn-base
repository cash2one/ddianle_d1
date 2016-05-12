#include "GameMsg_System_Connect.h"
#include "MsgBaseStruct.h"

GameMsg_System_Connect::GameMsg_System_Connect(void)
:_baseClass(MSG_SYSTEM_Connect)
{
	nConnCode = 0;
}

GameMsg_System_Connect::~GameMsg_System_Connect(void)
{
}


bool GameMsg_System_Connect::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddInt(nConnCode);
	return TRUE;
}
bool GameMsg_System_Connect::doDecode( CParamPool &IoBuff )
{
	nConnCode = IoBuff.GetInt();
	return TRUE;
}


//file end

