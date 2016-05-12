#include "GameMsg_System_Invalid.h"
#include "MsgBaseStruct.h"
GameMsg_System_Invalid::GameMsg_System_Invalid(void)
:_baseClass(MSG_SYSTEM_Invalid)
{
}

GameMsg_System_Invalid::~GameMsg_System_Invalid(void)
{
}

bool GameMsg_System_Invalid::doEncode( CParamPool &IoBuff )
{
	return TRUE;
}
bool GameMsg_System_Invalid::doDecode( CParamPool &IoBuff )
{
	return TRUE;
}

//file end

