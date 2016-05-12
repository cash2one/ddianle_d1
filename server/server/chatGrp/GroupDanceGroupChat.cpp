#include "../group/GroupServerCirculator.h"
#include "GroupDanceGroupChat.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include "../group/Global.h"

CGroupDanceGroupChat::CGroupDanceGroupChat()
{
}

CGroupDanceGroupChat::~CGroupDanceGroupChat()
{
}

void CGroupDanceGroupChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
} 

void CGroupDanceGroupChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}
	GameMsg_S2G_Chat *pChatMsg = (GameMsg_S2G_Chat *)pMsg;
	CGroupRoleEntity * pReceiverRole = CGroupPlayerManager::Instance().SearchRoleByID(pChatMsg->m_nReceiverRoleID);
	if (pReceiverRole == NULL)
	{
		return;
	}
	GameMsg_G2S_Chat g2sMsg(pChatMsg->m_nPhase, pChatMsg->m_nResult, pChatMsg->m_nChannel, pChatMsg->m_nColor,
		pChatMsg->m_nSenderRoleID, pChatMsg->m_nReceiverRoleID, pChatMsg->m_strSenderName.c_str(), pReceiverRole->GetName(),
		pChatMsg->m_strText.c_str(), pReceiverRole->GetSex(), pChatMsg->m_nExtraType, pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel,
		pChatMsg->m_bVoiceChat,pChatMsg->m_nWorldChatFlag);
	pReceiverRole->SendPlayerMsg(&g2sMsg);
}


//file end

