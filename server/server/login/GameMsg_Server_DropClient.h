#ifndef __GameMsg_Server_Drop_H__
#define __GameMsg_Server_Drop_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;
class GameMsg_Server_DropClient :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Server_DropClient);
	GameMsg_Server_DropClient(void);
	~GameMsg_Server_DropClient(void);
public:
	virtual bool doEncode( CParamPool &IoBuff );
	virtual bool doDecode( CParamPool &IoBuff );

	unsigned short m_nDropReason;
};

#endif

//file end

