#ifndef __GameMsg_GAME2GATEPLAYERLOGOUT_H__
#define __GameMsg_GAME2GATEPLAYERLOGOUT_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_Game2GatePlayerLogout :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_Game2GatePlayerLogout(void);
	~GameMsg_Game2GatePlayerLogout(void);

public:
	virtual bool doEncode( CParamPool& IoBuff );
};

#endif
//file end

