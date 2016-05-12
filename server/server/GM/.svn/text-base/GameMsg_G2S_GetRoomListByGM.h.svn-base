#ifndef __GameMsg_G2S_GetRoomListByGM_H_
#define __GameMsg_G2S_GetRoomListByGM_H_

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include <string>
#include <vector>

class GameMsg_G2S_GetRoomListByGM : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRoomListByGM);
	GameMsg_G2S_GetRoomListByGM();
	~GameMsg_G2S_GetRoomListByGM();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nSlot;
	unsigned short m_nGMAskRoomType;
};

class GameMsg_S2G_GetRoomListByGMRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRoomListByGMRes);
	GameMsg_S2G_GetRoomListByGMRes();
	~GameMsg_S2G_GetRoomListByGMRes();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nSlot;

	std::vector<GMRoomInfo> m_RoomList;
};

#endif
