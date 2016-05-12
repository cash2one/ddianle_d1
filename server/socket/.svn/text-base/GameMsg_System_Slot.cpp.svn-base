#include "GameMsg_System_Slot.h"
#include "MsgBaseStruct.h"

GameMsg_System_Slot::GameMsg_System_Slot(void)
:_baseClass(MSG_SYSTEM_Slot)
{
	nSlot = 0;
	nRandom = 0;
}

GameMsg_System_Slot::~GameMsg_System_Slot(void)
{
}

bool GameMsg_System_Slot::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUShort(nSlot);
	IoBuff.AddUShort(nRandom);
	IoBuff.AddMemory(key,16);
	return TRUE;

}
bool GameMsg_System_Slot::doDecode( CParamPool &IoBuff )
{
	nSlot = IoBuff.GetUShort();
	nRandom = IoBuff.GetUShort();
	IoBuff.GetMemory(key,16);
	return TRUE;
}


//file end

