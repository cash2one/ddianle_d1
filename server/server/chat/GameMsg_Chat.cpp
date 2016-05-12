#include "../share/StdAfx.h"
#include "GAMEMSG_CHAT.h"

GAMEMSG_CHAT::GAMEMSG_CHAT(void):_baseClass(MSG_CHAT_CHAT)
{
	strSenderName = "";
	strReceiverName = "";
	strContext = "";
}

GAMEMSG_CHAT::~GAMEMSG_CHAT(void)
{
}
GAMEMSG_CHAT* GAMEMSG_CHAT::Create(const char* szSenderName,unsigned int nEntityID, eChatChannel eChannel
							,const char* szContext , const char* szReceiverName , unsigned int nEntityIDReceiver
							, unsigned short dwColor1,unsigned short dwColor2  )
{
	unsigned short nLen = short(strlen(szContext));
//	unsigned short nSize = sizeof(GAMEMSG_CHAT)+nLen;
	GAMEMSG_CHAT* pMsg = new GAMEMSG_CHAT;
	pMsg->nMsg = MSG_CHAT_CHAT;

	int nStrLen;
	if(szSenderName)
	{
		strSenderName = szSenderName;
	}

	if(szReceiverName)
	{
		strReceiverName = szReceiverName;
	}

	pMsg->nEntityID = nEntityID;
	pMsg->nEntityIDReceiver = nEntityIDReceiver;
	pMsg->nChannel = short(eChannel);
	//strcpy(pMsg->szContext, szContext);
	strContext = szContext; 
	pMsg->dwColor[0] = dwColor1;
	pMsg->dwColor[1] = dwColor2;
	return pMsg;

}

bool GAMEMSG_CHAT::doEncode( CParamPool& IoBuff )
{
	return FALSE;
}
bool GAMEMSG_CHAT::doDecode( CParamPool& IoBuff )
{
	return FALSE;
}
