#ifndef __GameMsg_S2C_ROOMREPORTTOTALRESULT_H__
#define __GameMsg_S2C_ROOMREPORTTOTALRESULT_H__

#include "../socket/GameMsg_Base.h"


class GameMsg_S2C_RoomReportTotalResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RoomReportTotalResult(void);
	~GameMsg_S2C_RoomReportTotalResult(void);

	unsigned char nPlayerCount;
public:
	virtual bool doEncode( CParamPool &IoBuff );
};
#endif

