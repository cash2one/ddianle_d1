#include "GameMsg_S2C_RoomReportTotalResult.h"
#include "../socket/ParamPool.h"
#include "../server/ServerMsgDef.h"

GameMsg_S2C_RoomReportTotalResult::GameMsg_S2C_RoomReportTotalResult(void):_baseClass(MSG_S2C_RoomReportTotalResult)
{
	nPlayerCount = 0;
}

GameMsg_S2C_RoomReportTotalResult::~GameMsg_S2C_RoomReportTotalResult(void)
{
}
bool GameMsg_S2C_RoomReportTotalResult::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUChar(nPlayerCount);
	return false;
}

//end

