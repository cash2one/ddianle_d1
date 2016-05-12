#ifndef	__GAMEMSG_S2G_FRIENDLOGIN_H__
#define __GAMEMSG_S2G_FRIENDLOGIN_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_FriendLogin:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_FriendLogin);
	GameMsg_S2G_FriendLogin(unsigned int nLoginID = 0, unsigned int nNotifiedID = 0);
	~GameMsg_S2G_FriendLogin();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nLoginID;
	//unsigned int m_nNotifiedID;
};

#endif
// end of file

