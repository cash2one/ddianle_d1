#ifndef __GameMsg_S2C_ROOMREPORTPERRESULT_H__
#define __GameMsg_S2C_ROOMREPORTPERRESULT_H__

#include "../socket/GameMsg_Base.h"

class GameMsg_C2S_RoomReportPerResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RoomReportPerResult);
	GameMsg_C2S_RoomReportPerResult(void);
	~GameMsg_C2S_RoomReportPerResult(void);

	/*
	 ** -3 表示此节失误
	 ** -2 表示Normal
	 ** -1 表示Great
	 ** 0 表示什么都没有
	 ** 1 表示Perfect
	 ** >1 表示连续Perfect
	*/
	unsigned int nCurRoomStartCount;//当前场次
	unsigned int nResult;	//单节分数
	char nSate;				//状态

public:
	virtual bool doDecode( CParamPool &IoBuff );
};
#endif

