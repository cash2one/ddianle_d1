/*
	通过ID查找在线玩家
*/

#ifndef __GAMEMSG_S2G_FINDPLAYERFROMID_H__
#define __GAMEMSG_S2G_FINDPLAYERFROMID_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_FindPlayer:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_FindPlayer);
	GameMsg_S2G_FindPlayer();
	~GameMsg_S2G_FindPlayer();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nFindFriendTypeIndex;
	unsigned int m_nFindRoleID;
	std::string m_strFindRoleName;
	unsigned char m_nFindLevelIndex;
	unsigned char m_nFindSexIndex;
	unsigned char m_nFindAgeIndex;
	unsigned char m_nFindConstellationIndex;
};

#endif
