#ifndef __GAMEMSG_PLAYER_LOGOUT_H__
#define __GAMEMSG_PLAYER_LOGOUT_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_Player_Logout :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_Logout);
	GameMsg_Player_Logout(void);
	~GameMsg_Player_Logout(void);
public:
	virtual bool doDecode( CParamPool& IoBuff );
};

#endif

//file end

