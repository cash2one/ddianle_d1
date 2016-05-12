#ifndef __GameMsg_System_DISCONNECT_H__
#define __GameMsg_System_DISCONNECT_H__
#include "GameMsg_Base.h"

typedef class GameMsg_System_Disconnect :
	public GameMsg_Base
{
private:
	typedef GameMsg_Base _baseClass;
public:
	GameMsg_System_Disconnect(void);
	~GameMsg_System_Disconnect(void);
	GAMEMSG_CREATEDECLARE(GameMsg_System_Disconnect);

public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
}* LPGameMsg_System_Disconnect;

#endif


//file end

