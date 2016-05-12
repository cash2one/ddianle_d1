#ifndef	__GAMEMSG_G2S_GETTEMPLISTONLINEINFO_H__
#define __GAMEMSG_G2S_GETTEMPLISTONLINEINFO_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/DataStruct_Base.h"

using namespace std;
class CParamPool;

class GameMsg_G2S_GetTempListOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetTempListOnlineInfo);
	GameMsg_G2S_GetTempListOnlineInfo();
	~GameMsg_G2S_GetTempListOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nListType;
	list<CFriendOnlineInfo> m_listMate;
};

#endif



