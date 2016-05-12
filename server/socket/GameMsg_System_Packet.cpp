#include "GameMsg_System_Packet.h"
#include "MsgBaseStruct.h"

GameMsg_System_Packet::GameMsg_System_Packet(void)
:_baseClass(MSG_SYSTEM_Packet)
{
}

GameMsg_System_Packet::~GameMsg_System_Packet(void)
{
}


bool GameMsg_System_Packet::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddInt(nCheckSum);
	//if( pRealMsg )
	//{
	//	pRealMsg->encodeMssage( IoBuff );
	//}
	return TRUE;
}
bool GameMsg_System_Packet::doDecode( CParamPool &IoBuff )
{
	nCheckSum = IoBuff.GetInt();
	return TRUE;
}

//file end

