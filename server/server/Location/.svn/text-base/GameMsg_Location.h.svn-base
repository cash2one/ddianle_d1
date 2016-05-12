#ifndef		__GAMEMSG_LOCATION_H__
#define		__GAMEMSG_LOCATION_H__	

#include "../../socket/GameMsg_Base.h"


class GameMsg_C2S_EnterZone:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterZone);
	GameMsg_C2S_EnterZone();
	~GameMsg_C2S_EnterZone();
public:
	virtual bool doDecode(CParamPool & IOBuff);

	unsigned char Location();
private:
public:
	unsigned char m_nLocation;
};




#endif

