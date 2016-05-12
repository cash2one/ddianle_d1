#include "GameMsg_C2S_RoomReportPerResult.h"
#include "../socket/ParamPool.h"
#include "../server/ServerMsgDef.h"
GameMsg_C2S_RoomReportPerResult::GameMsg_C2S_RoomReportPerResult(void):_baseClass(MSG_C2S_RoomReportPerResult)
{
}

GameMsg_C2S_RoomReportPerResult::~GameMsg_C2S_RoomReportPerResult(void)
{
}
bool GameMsg_C2S_RoomReportPerResult::doDecode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(nCurRoomStartCount);
	IOBuff.AddUInt(nResult);
	IOBuff.AddUChar(nSate);
	return true;
}


//end

