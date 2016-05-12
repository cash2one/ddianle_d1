#ifndef __GameMsg_System_PACKET_H__
#define __GameMsg_System_PACKET_H__

#include "GameMsg_Base.h"

typedef class GameMsg_System_Packet :
	public GameMsg_Base
{
private:
	typedef GameMsg_Base _baseClass;
public:
	GameMsg_System_Packet(void);
	~GameMsg_System_Packet(void);
	GAMEMSG_CREATEDECLARE(GameMsg_System_Packet);

public:
	unsigned int nCheckSum;
	GameMsg_Base * pRealMsg;
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );

}* LPGameMsg_System_Packet;

#endif

//file end

