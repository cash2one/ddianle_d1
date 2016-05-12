#include "GlobalChat.h"
#include "../../socket/GameMsg_Base.h"
#include "../logic/PlayerManager.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
//#include "../../liblog/LogType.h"

#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"

extern void BroadcastMsg(LPGameMsg_Base pMsg);

CGlobalChat::CGlobalChat(CEntityChatComponent *pRoleChat):IChat(pRoleChat)
{
}

CGlobalChat::~CGlobalChat()
{
}

void CGlobalChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
}

bool CGlobalChat::OnChat(GameMsg_Base *pMsg)
{	
	if (pMsg != NULL)
	{
		GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)pMsg;
		if (GetRoleAttr() != NULL && GetRoleItem() != NULL && GetRoleVIP() != NULL) {
			if (GetRoleItem()->SearchItem(ITEMID_SMALLSPEAKER) != NULL) {
				GameMsg_S2C_Chat ChatMsg(GetRoleAttr()->GetRoleName(), GetRoleAttr()->GetRoleID(), 
															eChatGlobal, CHATCOLOR_GLOBALCHANNEL, pChatMsg->m_strText.c_str(),
															GetRoleAttr()->GetSex(), EChatExtraType_Default,
															GetRoleVIP()->IsVIP(), (unsigned short)GetRoleVIP()->VIPLevel(), 
															pChatMsg->m_bVoiceChat,pChatMsg->m_WorldChatFlag);
				if ((GetRoleItem()->RemoveItemByType(ITEMID_SMALLSPEAKER, 1, EItemAction_Del_UseInChatting))) {
					BroadcastMsg(&ChatMsg);
					return true;
				} else {
					WriteLog(LOGLEVEL_ERROR, "duducted small speaker failed,roleid: %u , rolename: %s\n",GetRoleAttr()->GetRoleID(), GetRoleAttr()->GetRoleName());
				}

			}
		}
	}
	return false;
}




//end file


