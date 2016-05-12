#include "SystemChat.h"
#include "../../socket/GameMsg_Base.h"

#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "GameMsg_G2S_Chat.h"
#include "../logic/PlayerManager.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../../datastructure/Localization.h"


CSystemChat::CSystemChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CSystemChat::~CSystemChat()
{
}

bool CSystemChat::OnChat(GameMsg_Base *pMsg)
{
	/*
	if (pMsg != NULL)
	{
		GameMsg_G2S_Chat *pG2SMsg = (GameMsg_G2S_Chat *)pMsg;

		GameMsg_S2C_Chat ChatMsg("系统", pG2SMsg->m_nSenderRoleID, eChatSystem, pG2SMsg->m_nColor, pG2SMsg->m_strText.c_str());
		if ( pG2SMsg->m_nReceiverRoleID != 0)
		{
		m_pRoleEntity->SendPlayerMsg(&ChatMsg);
		}
		else
		{
		BroadcastMsg(&ChatMsg);
		}
	}
	*/
	return true;
}

void CSystemChat::OnGroupChatMessage(GameMsg_Base *pMsg)	//由组服务器主动发送的系统公告
{
	if (pMsg != NULL)
	{
		GameMsg_G2S_Chat *pG2SMsg = (GameMsg_G2S_Chat *)pMsg;
		GameMsg_S2C_Chat ChatMsg(CLocalization::Instance().GetString("System").c_str(), 0, 
													eChatSystem, pG2SMsg->m_nColor, pG2SMsg->m_strText.c_str(), 
													0, EChatExtraType_Default,
													pG2SMsg->m_bIsVIP, pG2SMsg->m_nVIPLevel, pG2SMsg->m_bVoiceChat,pG2SMsg->m_WorldChatFlag);
		if ( pG2SMsg->m_nReceiverRoleID != 0)
		{
			CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(pG2SMsg->m_nReceiverRoleID);
			if (pRole != NULL)
			{
				pRole->SendPlayerMsg(&ChatMsg);
			}
		}
	}
}

void CSystemChat::SysChat(const char* buffer)
{
	if (GetRoleNet() != NULL)
	{
		GameMsg_S2C_Chat *pChatMsg2Client = NULL;
		pChatMsg2Client = GameMsg_S2C_Chat::Create(CLocalization::Instance().GetString("System").c_str(),0,
												   eChatSystem,buffer,CHATCOLOR_SYSTEMCHANNEL, 
												   0, EChatExtraType_Default,
												   false, 0, false,0);
		if (pChatMsg2Client != NULL){
			GetRoleNet()->SendPlayerMsg(pChatMsg2Client);
		}
		delete pChatMsg2Client;
		pChatMsg2Client = NULL;
	}
}

//end file


