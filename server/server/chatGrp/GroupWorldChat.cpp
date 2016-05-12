#include "../group/GroupServerCirculator.h"
#include "GroupWorldChat.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include "../group/Global.h"

extern void BroadcastMsg2GameServer(LPGameMsg_Base pMsg);

CGroupWorldChat::CGroupWorldChat()
{
}

CGroupWorldChat::~CGroupWorldChat()
{
}

void CGroupWorldChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
}

void CGroupWorldChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg != NULL)
	{
		GameMsg_S2G_Chat *pChatMsg = (GameMsg_S2G_Chat *)pMsg;
		GameMsg_G2S_Chat sendMsg(eChatPhrase_Forward, eChat_Success, pChatMsg->m_nChannel, pChatMsg->m_nColor, 
			pChatMsg->m_nSenderRoleID, pChatMsg->m_nReceiverRoleID, pChatMsg->m_strSenderName.c_str(), 
			pChatMsg->m_strReceiverName.c_str(), pChatMsg->m_strText.c_str(), pChatMsg->m_nClickSex, pChatMsg->m_nExtraType,
			pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat,pChatMsg->m_nWorldChatFlag);
		BroadcastMsg2GameServer(&sendMsg);
	}
}

//file end

