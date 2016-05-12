#include "../../socket/GameMsg_Base.h"
#include "../../socket/Formatter.h"
#include "CurrentChat.h"
#include "EntityChatComponent.h"
#include "../room/RoomManager.h"
#include "../room/LiveRoomManager.h"
#include "../room/RoomComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../logic/PlayerManager.h"
#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "../Location/LocationMgr.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../../datastructure/Localization.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../CeremonyRoom/ceremonyRoom.h"

CCurrentChat::CCurrentChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CCurrentChat::~CCurrentChat()
{
}

void CCurrentChat::SendRoomPlayerMessage(CRoomComponent* pPlayer, GameMsg_Base *msg)
{
	if (pPlayer != NULL)
	{
		if (pPlayer->FriendCOM() != NULL && pPlayer->AttrCOM() != NULL)
		{
			if (!pPlayer->FriendCOM()->IsInBlackList(GetRoleAttr()->GetRoleID())) 
			{
				pPlayer->SendPlayerMsg(msg);
			}
		}
	}
}

bool CCurrentChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg == NULL || m_pRoleChat == NULL || GetRoleAttr() == NULL || GetRoleVIP() == NULL)
	{
		return false; 
	}

	int nColor = CHATCOLOR_CURRENTCHANNEL;
	time_t tNow = time(NULL);
	nColor = (m_pRoleChat->GetCurFontColor() != 0 ? m_pRoleChat->GetCurFontColor() : CHATCOLOR_CURRENTCHANNEL);

	GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)pMsg;
	GameMsg_S2C_Chat sendMsg(GetRoleAttr()->GetRoleName(), GetRoleAttr()->GetRoleID(), (unsigned char)eChatCurrent, 
		(unsigned char)nColor, pChatMsg->m_strText.c_str(), GetRoleAttr()->GetSex(), EChatExtraType_Default, 
		GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
	bool bSent = false;
	if ( GetRoleRoom() != NULL && GetRoleRoom()->Room() != NULL )
	{
		CRoom *pRoom = GetRoleRoom()->Room();
		ERoomType eType = pRoom->RoomType();
		if (eType == eRoomType_Common)
		{
			for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
			{
				CRoomComponent* pPlayer= pRoom->Player(RoleRoomType_Dancer, i);
				SendRoomPlayerMessage(pPlayer, &sendMsg);
			}

			for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
			{
				CRoomComponent* pPlayer= pRoom->Player(RoleRoomType_Audience, i);
				SendRoomPlayerMessage(pPlayer, &sendMsg);
			}
			bSent = true;
		}
		else if (eType == eRoomType_LiveRoomPlayer || eType == eRoomType_LiveRoomGroup || eType == eRoomType_LiveRoomOther)
		{
			if (GetRoleAttr()->IsGM())
			{
				std::string name = CLocalization::Instance().GetString("Live_Room_GM_Name_Prefix");
				name.append(sendMsg.m_strSenderName);
				sendMsg.m_strSenderName = name;
			}

			for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
			{
				CRoomComponent* pPlayer= pRoom->Player(RoleRoomType_Dancer, i);
				SendRoomPlayerMessage(pPlayer, &sendMsg);
			}

			for (int i = 0; i < MAX_ROOM_GM_COUNT; ++i)
			{
				CRoomComponent* pPlayer= pRoom->Player(RoleRoomType_GM, i);
				SendRoomPlayerMessage(pPlayer, &sendMsg);
			}

			std::map<unsigned int, CRoomComponent*> audienceMap = pRoom->LiveRoomAudicenceMap();
			for (std::map<unsigned int, CRoomComponent*>::iterator it = audienceMap.begin();
				it != audienceMap.end(); ++it)
			{
				CRoomComponent* pPlayer= it->second;
				SendRoomPlayerMessage(pPlayer, &sendMsg);
			}
			bSent = true;
		}
	}
	else if ( CRoomManager::Instance().GetLobbyPlayer(GetRoleAttr()->GetRoleID()) != NULL )
	{
		CRoomManager::Instance().SendLobbyChatMsg( GetRoleAttr()->GetRoleID(), &sendMsg );
		m_pRoleChat->SetLobbyChatTime(tNow);
		bSent = true;
	}
	else if ( CLiveRoomManager::Instance().GetLobbyPlayer(GetRoleAttr()->GetRoleID()) != NULL )
	{
		CLiveRoomManager::Instance().SendLobbyChatMsg( GetRoleAttr()->GetRoleID(), &sendMsg );
		m_pRoleChat->SetLobbyChatTime(tNow);
		bSent = true;
	}
	else if (GetRoleCouple() != NULL && GetRoleCouple()->WeddingRoom() != NULL)
	{
		GetRoleCouple()->WeddingRoom()->SendMsgToAll(&sendMsg);
		bSent = true;
	}
	else if (GetRolePhotoRoom() != NULL && GetRolePhotoRoom()->PhotoRoom() != NULL )
	{
		GetRolePhotoRoom()->PhotoRoom()->SendMsgToAll(&sendMsg);
		bSent = true;
	}
	else if ( GetRoleAmuseRoom() != NULL && GetRoleAmuseRoom()->AmuseRoom() != NULL )
	{
		GetRoleAmuseRoom()->AmuseRoom()->SendMsgToAll(&sendMsg);
		bSent = true;
	}
    else if ( GetCeremonyRoom() != NULL )
    {
        GetCeremonyRoom()->RoomBroadcast(sendMsg);
    }
	return bSent;
}

void CCurrentChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
}

bool CCurrentChat::CanTalk()
{
	if (m_pRoleChat == NULL || GetRoleAttr() == NULL || GetRoleRoom() == NULL)
	{
		return false; 
	}

	CRoomComponent *pRoomCOM = GetRoleRoom();
	CRoom *pRoom = pRoomCOM->Room();
	ERoomType eType = eRoomType_Unknown;
	if (pRoom != NULL)
	{
		eType = pRoom->RoomType();
	}
	if (pRoomCOM->RoleRoomType() == RoleRoomType_Audience
		&& (eType == eRoomType_LiveRoomPlayer 
		|| eType == eRoomType_LiveRoomGroup 
		|| eType == eRoomType_LiveRoomOther))
	{
		m_pRoleChat->SysChat(CLocalization::Instance().GetString("Live_Room_Audience_Chat_Tip").c_str());
		return false;
	}

	int nTalkTimeInterval = 0;
	int nTalkRequiredLevel = 0;
	time_t nTimeNow = time(NULL);

	if ( CRoomManager::Instance().GetLobbyPlayer(GetRoleAttr()->GetRoleID()) != NULL ){	//´óÌü
		nTalkTimeInterval = CSystemSettingMgr::Instance().LobbyChatCDTime();
		nTalkRequiredLevel = CSystemSettingMgr::Instance().LobbyChatRequireLevel();
	}

	if (GetRoleAttr()->GetLevel() < nTalkRequiredLevel) {
		std::string strTip;
		SafeFormat(strTip, CLocalization::Instance().GetString("Chat_System_NotEnoughLevel"), nTalkRequiredLevel);
		m_pRoleChat->SysChat(strTip.c_str());
		return false;
	}

	if (m_pRoleChat->LastLobbyChatTime() + nTalkTimeInterval > nTimeNow) {
		m_pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind1").c_str());
		return false;
	}

	return true;
}


//end file


