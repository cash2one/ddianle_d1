#ifndef __GameMsg_System_SLOT_H__
#define __GameMsg_System_SLOT_H__

#include "GameMsg_Base.h"

typedef class GameMsg_System_Slot :
	public GameMsg_Base
{
private:
	typedef GameMsg_Base _baseClass;
public:
	GameMsg_System_Slot(void);
	~GameMsg_System_Slot(void);
	GAMEMSG_CREATEDECLARE(GameMsg_System_Slot);

public:
	unsigned short nSlot;
	unsigned short nRandom;
	char	key[16];
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
}* LPGameMsg_System_Slot;

#endif


//file end

