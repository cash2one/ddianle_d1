#ifndef	__GAMEMSG_G2S_FRIENDLOGIN_H__
#define	__GAMEMSG_G2S_FRIENDLOGIN_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_G2S_FriendLogin:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_FriendLogin);
	GameMsg_G2S_FriendLogin(unsigned int nLoginID = 0, const char *szName = "");
	~GameMsg_G2S_FriendLogin();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	CFriendOnlineInfo m_FriendOnlineInfo;
};

#endif
// end of file

