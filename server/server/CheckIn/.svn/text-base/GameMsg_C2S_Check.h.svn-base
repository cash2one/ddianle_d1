#ifndef		__GAMEMSG_C2S_CHECK_H__
#define		__GAMEMSG_C2S_CHECK_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_Check:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_Check);
	GameMsg_C2S_Check();
	~GameMsg_C2S_Check();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	int		m_nDayIndex;
};

#endif
// end of file

