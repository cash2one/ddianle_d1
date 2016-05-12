#ifndef	__GAMEMSG_S2G_UPDATEROOMINFO_H__		
#define	__GAMEMSG_S2G_UPDATEROOMINFO_H__	

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_UpdateRoomInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateRoomInfo);

	GameMsg_S2G_UpdateRoomInfo();
	~GameMsg_S2G_UpdateRoomInfo();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nRoleID;
	CWorldRoleUpdateRoomInfo m_RoomUpdateInfo;
};

#endif

// end of file

