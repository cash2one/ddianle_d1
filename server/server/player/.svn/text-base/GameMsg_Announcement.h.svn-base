#ifndef __GAMEMSG_ANNOUNCEMENT_H__
#define __GAMEMSG_ANNOUNCEMENT_H__

#include <vector>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

using namespace std;

class CParamPool;

class GameMsg_C2S_GetAnnouncement:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetAnnouncement);
	GameMsg_C2S_GetAnnouncement();
	~GameMsg_C2S_GetAnnouncement();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nVersion;
};

class GameMsg_S2C_UPdateAnnouncement:
	public GameMsg_Base
{
public:	
	GameMsg_S2C_UPdateAnnouncement();
	~GameMsg_S2C_UPdateAnnouncement();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	int m_nVersion;
	vector<CAnnouncement> m_vecAnnoucement;
};

class GameMsg_S2C_AnnouncementUpdated:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AnnouncementUpdated();
	~GameMsg_S2C_AnnouncementUpdated();
public:
	bool doEncode(CParamPool &IOBuff);
};





#endif


