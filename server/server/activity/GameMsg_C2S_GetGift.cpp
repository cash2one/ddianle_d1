#include "GameMsg_C2S_GetGift.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_GetGift::GameMsg_C2S_GetGift()
: GameMsg_Base(MSG_C2S_GetGift)
{
	m_nIndexID = 0;
}

GameMsg_C2S_GetGift::~GameMsg_C2S_GetGift()
{
}
bool GameMsg_C2S_GetGift::doDecode(CParamPool &IOBuff)
{
	m_nIndexID = IOBuff.GetInt();
	return true;
}

// end of file




