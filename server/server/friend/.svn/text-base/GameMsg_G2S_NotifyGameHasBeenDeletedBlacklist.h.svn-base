#ifndef	__GAMEMSG_G2S_NOTIFYGAMEHASBEENDELETEDBLACKLIST_H__
#define __GAMEMSG_G2S_NOTIFYGAMEHASBEENDELETEDBLACKLIST_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist);
	GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist();
	GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist(unsigned int nSrcRoleID);
	~GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcRoleID;
};

#endif
// end of file

