#ifndef __GameMsg_LINKCOUNT_H__
#define __GameMsg_LINKCOUNT_H__
#include "../../socket/GameMsg_Base.h"
class CParamPool;
class GameMsg_LinkCount :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_LinkCount);
	GameMsg_LinkCount(void);
	~GameMsg_LinkCount(void);

	unsigned short nCount;
public:
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};
#endif


//file end

