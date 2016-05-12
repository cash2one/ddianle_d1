#ifndef __GameMsg_System_PING_H__
#define __GameMsg_System_PING_H__

#include "GameMsg_Base.h"
typedef class GameMsg_System_Ping : public GameMsg_Base
{
private:
	typedef GameMsg_Base _baseClass;
public:
	GameMsg_System_Ping();
	~GameMsg_System_Ping(void);
	GAMEMSG_CREATEDECLARE(GameMsg_System_Ping);
public :
	unsigned int	nTickCount;
	unsigned int	nDirection;//0:S->C->S, 1:C->S->C
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );

}* LPGameMsg_System_Ping;

#endif


//file end

