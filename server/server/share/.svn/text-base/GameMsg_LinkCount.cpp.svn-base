#include "StdAfx.h"
#include "GameMsg_LinkCount.h"
#include "ServerMsgDef.h"
GameMsg_LinkCount::GameMsg_LinkCount(void):_baseClass(MSG_SERVER_LinkCount)
{
}

GameMsg_LinkCount::~GameMsg_LinkCount(void)
{
}
bool GameMsg_LinkCount::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUShort(nCount);
	return true;
}
bool GameMsg_LinkCount::doDecode( CParamPool& IoBuff )
{
	nCount = IoBuff.GetUShort();
	return true;
}

//end

