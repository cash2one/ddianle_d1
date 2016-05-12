#include "GameMsg_G2S_TimeEgg.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_OpenTimeEgg::GameMsg_G2S_OpenTimeEgg()
:GameMsg_Base(MSG_G2S_OpenTimeEgg)
{
	m_nEggID = 0;
}

GameMsg_G2S_OpenTimeEgg::~GameMsg_G2S_OpenTimeEgg()
{

}

bool GameMsg_G2S_OpenTimeEgg::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nEggID);
	return true;
}

bool GameMsg_G2S_OpenTimeEgg::doDecode( CParamPool &IOBuff )
{
	m_nEggID = IOBuff.GetUInt();
	return true;
}

