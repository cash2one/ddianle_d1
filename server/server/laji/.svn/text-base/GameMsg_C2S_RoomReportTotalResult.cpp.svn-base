#include "GameMsg_C2S_RoomReportTotalResult.h"
#include "../socket/ParamPool.h"
#include "../server/ServerMsgDef.h"

GameMsg_C2S_RoomReportTotalResult::GameMsg_C2S_RoomReportTotalResult(void):_baseClass(MSG_C2S_RoomReportTotalResult)
{
	nCurRoomStartCount = 0;
	nResult = 0;
	nPerfect = 0;
	nGreat = 0;
	nNormal = 0;
	nFail = 0;
	nExp = 0;
}

GameMsg_C2S_RoomReportTotalResult::~GameMsg_C2S_RoomReportTotalResult(void)
{
}

bool GameMsg_C2S_RoomReportTotalResult::doDecode( CParamPool &IoBuff )
{
	nCurRoomStartCount = IoBuff.GetUInt();
	nResult = IoBuff.GetUInt();
	nPerfect = IoBuff.GetUChar();
	nGreat = IoBuff.GetUChar();
	nNormal = IoBuff.GetUChar();
	nFail = IoBuff.GetUChar();
	nExp = IoBuff.GetUChar();
	return true;
}


//end

