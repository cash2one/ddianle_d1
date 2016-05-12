#include "PrivateChat.h"
#include "../../socket/GameMsg_Base.h"
#include "../logic/PlayerManager.h"

#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"

#include "GameMsg_S2G_Chat.h"
#include "GameMsg_S2C_Chat_Private2Self.h"
#include "../friend/EntityFriendComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../vip/EntityVIPComponent.h"
#include "../../datastructure/Localization.h"
#include "../EntityNetComponent/EntityNetComponent.h"

void SendMsg2GroupServer(GameMsg_Base *pMsg);

CPrivateChat::CPrivateChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CPrivateChat::~CPrivateChat()
{
}

bool CPrivateChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg != NULL && GetRoleAttr() != NULL && GetRoleNet() != NULL && GetRoleVIP() != NULL)
	{
		CEntityAttributeComponent *pRoleAttr = GetRoleAttr();

		unsigned char nColor = CHATCOLOR_PRIVATECHANNEL;

		GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)pMsg;
		CRoleEntity *pRecvPlayer = CPlayerManager::Instance().GetEntityByRoleID(pChatMsg->m_nReceiverRoleID);
		
        if (pRecvPlayer != NULL)
        {
            CRoleAttribute *pRecvRoleAttr = pRecvPlayer->GetComponent<CRoleAttribute>();
            CRoleFriend *pRecvRoleFriend = pRecvPlayer->GetComponent<CRoleFriend>();
            if (pRecvRoleAttr != NULL && pRecvRoleFriend != NULL)
            {
                bool bCanChat = true;
                CRoomComponent *pRecvRoleRoom = pRecvPlayer->GetComponent<CRoleRoom>();
                CRoom *pRoom = pRecvRoleRoom->Room();
                if (pRoleAttr->IsNormalPlayer() && pRoom != NULL && (pRoom->RoomType() == eRoomType_LiveRoomPlayer 
                    || pRoom->RoomType() == eRoomType_LiveRoomGroup || pRoom->RoomType() == eRoomType_LiveRoomOther )
                    && (pRecvRoleRoom->RoleRoomType() == RoleRoomType_Dancer || pRecvRoleRoom->RoleRoomType() == RoleRoomType_GM)
                    && !pRecvRoleFriend->IsFriend(pChatMsg->m_nSenderRoleID))
                {
                    bCanChat = false;
                }

                if (bCanChat)
                {
                    if ( !pRecvRoleFriend->IsInBlackList(pChatMsg->m_nSenderRoleID) )
                    {
                        GameMsg_S2C_Chat sendMsg(pRoleAttr->GetRoleName(), pChatMsg->m_nSenderRoleID, pChatMsg->m_nChannel, nColor, pChatMsg->m_strText.c_str(),
                            pRoleAttr->GetSex(), EChatExtraType_Default,
                            GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), 
                            pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
                        pRecvPlayer->SendPlayerMsg(&sendMsg);
                        pRecvRoleFriend->AddRecentlyChatMate(pRoleAttr->GetRoleID(), pRoleAttr->GetRoleName());
                    }

                    GameMsg_S2C_Chat_Private2Self selfMsg(pChatMsg->m_nReceiverRoleID, pRecvPlayer->GetRoleName(), nColor, pChatMsg->m_strText.c_str(), pRecvPlayer->GetSex(), pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag, pRecvPlayer->IsVIP(), (unsigned short)pRecvPlayer->GetVIPLevel());
                    GetRoleNet()->SendPlayerMsg(&selfMsg);
                    if(GetRoleFriend()!= NULL && pRecvRoleAttr != NULL && !GetRoleFriend()->IsInBlackList(pChatMsg->m_nReceiverRoleID))
                    {
                        GetRoleFriend()->AddRecentlyChatMate(pRecvRoleAttr->GetRoleID(), pRecvRoleAttr->GetRoleName());
                    }
                    return true;
                }
                else
                {
                    m_pRoleChat->SysChat(CLocalization::Instance().GetString("Live_Room_Private_Chat_Tip").c_str());
                }
            }
        }
        else
        {// 私聊对象不在本线
            GameMsg_S2G_Chat msg(eChatPhrase_Forward,eChat_Success,pChatMsg->m_nChannel,nColor,pChatMsg->m_nSenderRoleID,
                pChatMsg->m_nReceiverRoleID,pRoleAttr->GetRoleName(),"",pChatMsg->m_strText.c_str(), 0, EChatExtraType_Default
                ,GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
            GetRoleNet()->Send2GroupServer(&msg);
            return true;
        }
	}
	return false;
}

void CPrivateChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
	if (pMsg != NULL)
	{
		GameMsg_G2S_Chat *pChatMsg = (GameMsg_G2S_Chat *)pMsg;

		if (pChatMsg->m_nPhase == eChatPhrase_Forward){
			ProcessForwardMsg(pChatMsg);
		}
		else if (pChatMsg->m_nPhase == eChatPhrase_Backward){
			ProcessBackWardMsg(pChatMsg);
		}
	}
}

void CPrivateChat::ProcessForwardMsg(GameMsg_G2S_Chat *pChatMsg)
{
	if (pChatMsg != NULL && GetRoleFriend() != NULL && GetRoleNet() != NULL && GetRoleVIP() != NULL){
		unsigned char nResult = eChat_Success;
	
		if (GetRoleFriend()->IsInBlackList(pChatMsg->m_nSenderRoleID)){
			nResult = eChat_ChatShield;
		} else {//无限制条件，则发送至对方
			GameMsg_S2C_Chat sendMsg(pChatMsg->m_strSenderName.c_str(), pChatMsg->m_nSenderRoleID, eChatPrivate, 
									 pChatMsg->m_nColor, pChatMsg->m_strText.c_str(), pChatMsg->m_nClickSex, EChatExtraType_Default,
									 pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
			GetRoleNet()->SendPlayerMsg(&sendMsg);
			if (GetRoleFriend() != NULL) {
				GetRoleFriend()->AddRecentlyChatMate(pChatMsg->m_nSenderRoleID, pChatMsg->m_strSenderName.c_str());
			}
		}

		GameMsg_S2G_Chat msg(eChatPhrase_Backward, nResult, pChatMsg->m_nChannel, pChatMsg->m_nColor, pChatMsg->m_nSenderRoleID, 
							 pChatMsg->m_nReceiverRoleID, pChatMsg->m_strSenderName.c_str(), pChatMsg->m_strReceiverName.c_str(),
							 pChatMsg->m_strText.c_str(), 0, EChatExtraType_Default,
							 GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
		GetRoleNet()->Send2GroupServer(&msg);
	}
}

void CPrivateChat::ProcessBackWardMsg(GameMsg_G2S_Chat *pChatMsg)
{
	if (pChatMsg != NULL && m_pRoleChat != NULL && GetRoleNet() != NULL){
		unsigned char nResult = pChatMsg->m_nResult;
		if (nResult == Chat_ePlayerNotExisted){
			m_pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind2").c_str());
		} else if (nResult == eChat_ChatShield ){ 
			//  in receive's blacklist
			GameMsg_S2C_Chat_Private2Self sendMsg(pChatMsg->m_nReceiverRoleID, pChatMsg->m_strReceiverName.c_str(), pChatMsg->m_nColor,pChatMsg->m_strText.c_str(), 0, pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag, pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel);
			GetRoleNet()->SendPlayerMsg(&sendMsg);
			if (GetRoleFriend() != NULL) {
				GetRoleFriend()->AddRecentlyChatMate(pChatMsg->m_nReceiverRoleID, pChatMsg->m_strReceiverName.c_str());
			}
		} else if (nResult == eChat_Success ){ //back to self
			 GameMsg_S2C_Chat_Private2Self sendMsg(pChatMsg->m_nReceiverRoleID, pChatMsg->m_strReceiverName.c_str(), pChatMsg->m_nColor,pChatMsg->m_strText.c_str(), pChatMsg->m_nClickSex, pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag, pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel);
			 GetRoleNet()->SendPlayerMsg(&sendMsg);
			 if (GetRoleFriend() != NULL && !GetRoleFriend()->IsInBlackList(pChatMsg->m_nReceiverRoleID)) {
				 GetRoleFriend()->AddRecentlyChatMate(pChatMsg->m_nReceiverRoleID, pChatMsg->m_strReceiverName.c_str());
			 }
		 }
	}
}

// end of file

