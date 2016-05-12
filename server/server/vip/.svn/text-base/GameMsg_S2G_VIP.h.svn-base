#ifndef __GAMEMSG_S2G_VIP_H__
#define __GAMEMSG_S2G_VIP_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"


class GameMsg_S2G_UpdateVIPInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateVIPInfo);
	GameMsg_S2G_UpdateVIPInfo();
	~GameMsg_S2G_UpdateVIPInfo();
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
public:
	unsigned int m_nRoleID;
	bool m_bIsVIP;
	unsigned short m_nVIPLevel;
};


#endif


