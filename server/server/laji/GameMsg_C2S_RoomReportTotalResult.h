#ifndef __GameMsg_S2C_ROOMREPORTTOTALRESULT_H__
#define __GameMsg_S2C_ROOMREPORTTOTALRESULT_H__

#include "../socket/GameMsg_Base.h"

class GameMsg_C2S_RoomReportTotalResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RoomReportTotalResult);
	GameMsg_C2S_RoomReportTotalResult(void);
	~GameMsg_C2S_RoomReportTotalResult(void);

	unsigned int nCurRoomStartCount;//当前场次
	unsigned int nResult;	//总分数
	char nPerfect;			//完美数
	char nGreat;			//非常棒数
	char nNormal;			//一般数
	char nFail;				//失误数

	char nExp;				//这个经验值是 奖励的区间序列，策划暂时定为 1—10
public:
	virtual bool doDecode( CParamPool &IoBuff );
};

#endif

