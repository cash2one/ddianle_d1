#include "GameMsg_S2C_RoomReportPerResult.h"
#include "../socket/ParamPool.h"
#include "../server/ServerMsgDef.h"

GameMsg_S2C_RoomReportPerResult::GameMsg_S2C_RoomReportPerResult(void)
:_baseClass(MSG_S2C_RoomReportPerResult)
{
	nRoleId = 0;
	nResult = 0;
	nSate = 0;
}

GameMsg_S2C_RoomReportPerResult::~GameMsg_S2C_RoomReportPerResult(void)
{
}
bool GameMsg_S2C_RoomReportPerResult::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUInt(nRoleId);
	IoBuff.AddUInt(nResult);
	IoBuff.AddUChar(nSate);
	return true;
}

//end

