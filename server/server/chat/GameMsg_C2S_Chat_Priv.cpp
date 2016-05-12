
#include "GameMsg_C2S_Chat_Priv.h"
#include "../server/managermsgdef.h"
 
GameMsg_C2S_Chat_Priv::GameMsg_C2S_Chat_Priv(void):_baseClass(MSG_C2S_CHAT_PRIV)
{
	strContext = "";
}

GameMsg_C2S_Chat_Priv::~GameMsg_C2S_Chat_Priv(void)
{
}
GameMsg_C2S_Chat_Priv * GameMsg_C2S_Chat_Priv::Create(const char* szContext , unsigned int nReceiverRoleID, unsigned short dwColor1 ,unsigned short dwColor2 )
{
//	unsigned short nLen = short(strlen(szContext));
//	unsigned short nSize = sizeof(GameMsg_C2S_Chat_Priv)+nLen;
	GameMsg_C2S_Chat_Priv* pMsg = new GameMsg_C2S_Chat_Priv;
	pMsg->nMsg = MSG_C2S_CHAT_PRIV;

	pMsg->m_nReceiverRoleID = nReceiverRoleID;
	pMsg->strContext, szContext;
	pMsg->dwColor[0] = dwColor1;
	pMsg->dwColor[1] = dwColor2;
	return pMsg;
}
bool GameMsg_C2S_Chat_Priv::doEncode( CParamPool& IoBuff )
{
	return false;
}
bool GameMsg_C2S_Chat_Priv::doDecode( CParamPool& IoBuff )
{
	return false;
}
