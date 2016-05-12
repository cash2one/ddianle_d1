#ifndef	__GAMEMSG_G2S_GETBLACKLISTONLINEINFO__
#define __GAMEMSG_G2S_GETBLACKLISTONLINEINFO__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

using namespace std;
class CParamPool;

class GameMsg_G2S_GetBlackListOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetBlackListOnlineInfo);
	GameMsg_G2S_GetBlackListOnlineInfo();
	~GameMsg_G2S_GetBlackListOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	list<CFriendOnlineInfo>	m_listBlackOnlineInfo;
};

#endif




