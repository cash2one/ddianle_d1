#include "GameMsg_G2S_Announce.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_Announce::GameMsg_G2S_Announce():
GameMsg_Base(MSG_G2S_Announce)
{
}

GameMsg_G2S_Announce::~GameMsg_G2S_Announce()
{
}

bool GameMsg_G2S_Announce::doEncode(CParamPool &IOBuff)
{
	m_announce.doEncode(IOBuff);
	return true;
}

bool GameMsg_G2S_Announce::doDecode(CParamPool &IOBuff)
{
	m_announce.doDecode(IOBuff);
	return true;
}


// end of file

