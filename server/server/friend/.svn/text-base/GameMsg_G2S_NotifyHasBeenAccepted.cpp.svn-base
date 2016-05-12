#include "GameMsg_G2S_NotifyHasBeenAccepted.h"
#include "../share/ServerMsgDef.h"


GameMsg_G2S_NotifyHasBeenAccpted::GameMsg_G2S_NotifyHasBeenAccpted():
GameMsg_Base(MSG_G2S_NotifiedHasBeenAccepted)
{
}

GameMsg_G2S_NotifyHasBeenAccpted::~GameMsg_G2S_NotifyHasBeenAccpted()
{
}

bool GameMsg_G2S_NotifyHasBeenAccpted::doEncode(CParamPool &IOBuff)
{
	m_friendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_G2S_NotifyHasBeenAccpted::doDecode(CParamPool &IOBuff)
{
	m_friendInfo.doDecode(IOBuff);

	return true;
}

// end of file

