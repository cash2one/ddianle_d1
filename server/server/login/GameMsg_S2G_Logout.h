#ifndef	__GAMEMSG_S2G_LOGOUT_H__
#define __GAMEMSG_S2G_LOGOUT_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_Logout:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_Logout);
	GameMsg_S2G_Logout(unsigned int nRoleID = 0, int nLine = -1);
	~GameMsg_S2G_Logout();
public:
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	int m_nLine;
};


#endif

