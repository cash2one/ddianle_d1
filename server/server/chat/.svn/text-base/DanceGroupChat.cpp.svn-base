#include "DanceGroupChat.h"
#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "../room/RoomManager.h"
#include "../room/RoomComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../logic/PlayerManager.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../chat/EntityChatComponent.h"
#include "GameMsg_S2G_Chat.h"
#include "../../datastructure/Localization.h"
#include "../EntityNetComponent/EntityNetComponent.h"

extern void BroadcastMsg(LPGameMsg_Base pMsg);
extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CDanceGroupChat::CDanceGroupChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CDanceGroupChat::~CDanceGroupChat()
{
}

bool CDanceGroupChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg != NULL && m_pRoleChat != NULL && GetRoleAttr() != NULL && GetRoleVIP() != NULL)
	{
		GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)pMsg;
		GameMsg_S2C_Chat sendMsg(GetRoleAttr()->GetRoleName(), GetRoleAttr()->GetRoleID(), eChatDanceGroup, CHATCOLOR_DANCEGROUPCHANNEL, 
								 pChatMsg->m_strText.c_str(), GetRoleAttr()->GetSex(), EChatExtraType_Default,
								 GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
		CEntityDanceGroupComponent * pDGCom = GetDanceGroup();
		if (pDGCom)
		{
			const std::map< unsigned int, CDanceGroupMember * > & rDGMemberMap = pDGCom->GetDanceGroupMemberMap();
			if (!rDGMemberMap.empty())
			{
				std::map< unsigned int, CDanceGroupMember * >::const_iterator it = rDGMemberMap.begin();
				for (; it != rDGMemberMap.end(); ++it)
				{
					unsigned int nRoleID = it->first;
					CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
					if (pRoleEntity)
					{
						pRoleEntity->SendPlayerMsg(&sendMsg);
					}
					else
					{
						//让GroupServer转发
						GameMsg_S2G_Chat msg(eChatPhrase_Forward, eChat_Success,pChatMsg->m_nChannel,
											 CHATCOLOR_DANCEGROUPCHANNEL,pChatMsg->m_nSenderRoleID, 
											 nRoleID,GetRoleAttr()->GetRoleName(),"",pChatMsg->m_strText.c_str(), 
											 0, EChatExtraType_Default,
											 GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(),
											 pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
						GetRoleNet()->Send2GroupServer(&msg);
					}
				}

                //舞团聊天历史记录
                GameMsg_S2G_GroupDanceChatHistoryRecord historyMsg(pDGCom->GetDanceGroupID(), GetRoleAttr()->GetRoleName(), GetRoleAttr()->GetRoleID(), eChatDanceGroup, CHATCOLOR_DANCEGROUPCHANNEL,
                    pChatMsg->m_strText.c_str(), GetRoleAttr()->GetSex(), EChatExtraType_Default,
                    GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat, pChatMsg->m_WorldChatFlag);
                SendMsg2GroupServer(&historyMsg);

				return true;
			}
			else if (m_pRoleChat != NULL)
			{
				if (0 != pDGCom->GetDanceGroupID() )
					m_pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_DanceGroup_Not_Load_Completed").c_str() );
				else
					m_pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_Not_In_Any_DanceGroup").c_str());
			}
		}
	}
	return false;
}

void CDanceGroupChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}
	GameMsg_G2S_Chat * pChatMsg = (GameMsg_G2S_Chat *)pMsg;

	GameMsg_S2C_Chat sendMsg(pChatMsg->m_strSenderName.c_str(), pChatMsg->m_nSenderRoleID, 
							 eChatDanceGroup, pChatMsg->m_nColor, pChatMsg->m_strText.c_str(), 
							 pChatMsg->m_nClickSex, pChatMsg->m_nExtraType,
							 pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
	CEntityNetComponent * pNetCom = GetRoleNet();
	if (pNetCom)
	{
		pNetCom->SendPlayerMsg(&sendMsg);
	}
}


//end file


