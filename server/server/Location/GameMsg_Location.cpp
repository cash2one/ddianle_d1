#include "../share/ServerMsgDef.h"
#include "GameMsg_Location.h"


GameMsg_C2S_EnterZone::GameMsg_C2S_EnterZone()
:GameMsg_Base(MSG_C2S_EnterZone)
,m_nLocation(0)
{

}

GameMsg_C2S_EnterZone::~GameMsg_C2S_EnterZone()
{

};

bool GameMsg_C2S_EnterZone::doDecode(CParamPool & IOBuff)
{
	m_nLocation = IOBuff.GetUChar();

	return true;
}


unsigned char GameMsg_C2S_EnterZone::Location()
{
	return m_nLocation;
}






