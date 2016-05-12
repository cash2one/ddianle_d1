// 补签协议

#ifndef		__GAMEMSG_C2S_RECHECK_H__
#define		__GAMEMSG_C2S_RECHECK_H__

#include "../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_ReCheck:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReCheck);
	GameMsg_C2S_ReCheck();
	~GameMsg_C2S_ReCheck();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nWeekDay;	// 周几，0-6,分别表示 周日--周六
};

#endif
// end of file


