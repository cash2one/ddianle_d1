#include "WorldChat.h"
#include "../../socket/GameMsg_Base.h"
#include "../item/EntityItemComponent.h"
//#include "../../liblog/LogType.h"

#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "GameMsg_S2G_Chat.h"
#include "GameMsg_G2S_Chat.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../Rank/RankComponent.h"

extern void BroadcastMsg(LPGameMsg_Base pMsg);

CWorldChat::CWorldChat(CEntityChatComponent *pRoleChat)
    : IChat(pRoleChat)
{
}

CWorldChat::~CWorldChat()
{
}

bool CWorldChat::OnChat(GameMsg_Base *pMsg)
{
    if (pMsg != NULL)
    {
        GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)pMsg;
        CEntityAttributeComponent *pRoleAttr = GetRoleAttr();

        if(GetRoleItem() != NULL && pRoleAttr != NULL && GetRoleNet() != NULL && GetRoleVIP() != NULL)
        {
            itemtype_t nItemID = ConfigManager::Instance().GetHornMgr().GetHornItemBySpeckType(pChatMsg->m_WorldChatFlag);

            if (nItemID == 0)
            {
                return false;
            }
			if (GetRoleItem()->SearchItem(nItemID))
            {
                if ( (GetRoleItem()->RemoveItemByType(nItemID, 1, EItemAction_Del_UseInChatting) ) )// ¿Û³ýÎïÆ·´óÀ®°È
                {
                    GameMsg_S2G_Chat sendMsg(eChatPhrase_Forward, eChat_Success, eChatWorld, CHATCOLOR_WORLDCHANNEL, 
                        pChatMsg->m_nSenderRoleID, pChatMsg->m_nReceiverRoleID, pRoleAttr->GetRoleName(), "", 
                        pChatMsg->m_strText.c_str(), pRoleAttr->GetSex(), EChatExtraType_Default, GetRoleVIP()->IsVIP(), 
                        (unsigned short)GetRoleVIP()->VIPLevel(), pChatMsg->m_bVoiceChat, pChatMsg->m_WorldChatFlag, pChatMsg->m_bAutoChat);

                    GetRoleNet()->Send2GroupServer(&sendMsg);

                    CRankComponent *pRankComponent = GetRankComponent();
                    if (NULL != pRankComponent)
                    {
                        pRankComponent->SubmitRankValue(ERankDataType_Horn, 1);
                    }

                    return true;
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "deduct big speaker failed: roleid: %u, rolename: %s\n", 
                        pRoleAttr->GetRoleID(), pRoleAttr->GetRoleName() );
                }
            }
        }
    }

    return false;
}

void CWorldChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
    if (pMsg != NULL)
    {
        GameMsg_G2S_Chat *pChatMsg = (GameMsg_G2S_Chat *)pMsg;
        GameMsg_S2C_Chat sendMsg = GameMsg_S2C_Chat(pChatMsg->m_strSenderName.c_str(), pChatMsg->m_nSenderRoleID, eChatWorld, 
            CHATCOLOR_WORLDCHANNEL, pChatMsg->m_strText.c_str(), pChatMsg->m_nClickSex, EChatExtraType_Default, 
            pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat, pChatMsg->m_WorldChatFlag);

        BroadcastMsg(&sendMsg);
    }
}

//end file


