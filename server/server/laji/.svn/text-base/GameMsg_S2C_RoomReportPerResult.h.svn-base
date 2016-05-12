#ifndef __GameMsg_S2C_ROOMREPORTPERRESULT_H__
#define __GameMsg_S2C_ROOMREPORTPERRESULT_H__

#include "../socket/GameMsg_Base.h"

class GameMsg_S2C_RoomReportPerResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RoomReportPerResult(void);
	~GameMsg_S2C_RoomReportPerResult(void);

	unsigned int nRoleId;
	unsigned int nResult;	//总分数

	// -3 表示此节失误
	// -2 表示Normal
	// -1 表示Great
	//  0 表示什么都没有
	//  1 表示Perfect
	// >1 表示连续Perfect
	char nSate;				//状态

public :
	virtual bool doEncode( CParamPool &IoBuff );
};

#endif


