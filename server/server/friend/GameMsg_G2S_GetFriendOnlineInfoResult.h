#ifndef	__GAMEMSG_G2S_GETFRIENDONLINEINFORESULT_H__
#define	__GAMEMSG_G2S_GETFRIENDONLINEINFORESULT_H__ 

#include "../../socket/GameMsg_Base.h"
#include <list> 
#include "../../datastructure/DataStruct_DB.h"

using namespace std;

class CParamPool;

class GameMsg_G2S_GetFriendOnlineInfoResult:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetFriendOnlineInfoResult);
	GameMsg_G2S_GetFriendOnlineInfoResult();
	~GameMsg_G2S_GetFriendOnlineInfoResult();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	list<CFriendOnlineInfo> m_listFriendOnlineInfo;
};

#endif
// end of file

