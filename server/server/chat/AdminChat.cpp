#include "AdminChat.h"
#include "GameMsg_S2G_Chat.h"
#include "GameMsg_G2S_Chat.h"
#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "../logic/PlayerManager.h"

#include "../../datastructure/Localization.h"
#include "../EntityNetComponent/EntityNetComponent.h"

extern void BroadcastMsg(LPGameMsg_Base pMsg);
extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CAdminChat::CAdminChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CAdminChat::~CAdminChat()
{
}

bool CAdminChat::OnChat(GameMsg_Base *pMsg)
{
	return true;
}

void CAdminChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
	if (pMsg != NULL)
	{
		GameMsg_G2S_Chat *pChatMsg = (GameMsg_G2S_Chat *)pMsg;
		std::string strSender = CLocalization::Instance().GetString("AdminAnnounce");
		GameMsg_S2C_Chat sendMsg = GameMsg_S2C_Chat(strSender.c_str(), 0, (unsigned char)eChatAdmin, (unsigned char)CHATCOLOR_ADMINCHANNEL, 
													pChatMsg->m_strText.c_str(), 0, pChatMsg->m_nExtraType, 
													pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag); 
		BroadcastMsg(&sendMsg);
	}
}

void CAdminChat::AdminChat(const char* buffer)
{
	GameMsg_S2G_Chat sendMsg = GameMsg_S2G_Chat(eChatPhrase_Forward, eChat_Success, eChatAdmin, CHATCOLOR_ADMINCHANNEL, 
												0, 0, CLocalization::Instance().GetString("AdminAnnounce").c_str(), "",	
												buffer, 0, EChatExtraType_Default, 
												false, 0, false,0);
	SendMsg2GroupServer(&sendMsg);
}



