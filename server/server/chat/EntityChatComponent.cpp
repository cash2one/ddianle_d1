#include "EntityChatComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../logic/CommandProcessor.h"

#include <cstdio>
#include <stdlib.h>
#include <stdarg.h>
#include <string>

#include "GameMsg_G2S_Chat.h"
#include "GameMsg_C2S_Chat.h"
#include "GameMsg_S2C_Chat.h"
#include "GameMsg_S2G_Chat.h"
#include "GameMsg_G2S_Chat_Admin.h"
#include "../logic/EntityComponentEventID.h"
#include "../logic/LogicCirculator.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../room/RoomComponent.h"
#include "../item/EntityItemComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../activity/EntityActivityComponent.h"
#include "ChatManager.h"
#include "../../datastructure/Localization.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../bet/BetComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../Rank/RankComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Medal/EntityMedalComponent.h"

#include "ChatMsgDef.h"
#include "SystemChat.h"
#include "../SystemSetting/SystemSettingMgr.h"

#ifndef WIN32
#define _vsnprintf  vsnprintf
#endif

extern CLogicCirculator* g_pCirculator;

#define     TALKINTERVALTIME        2
#define     MIN_AUTOCHAT_INTERVAL   500 // ms

CEntityChatComponent::CEntityChatComponent(void)
    : CEntityComponent(true,true)
    , m_nFontColor(0)
    , m_tLastLobbyChatTime(0)
    , m_nLastAutoChatTime(0)
    , m_pRoleNet(NULL)
    , m_pRoleFriend(NULL)
    , m_pRoleAttr(NULL)
    , m_pRoleItem(NULL)
    , m_pRoleQuest(NULL)
    , m_pRoleRoom(NULL)
    , m_pRoleDanceGroup(NULL)
    , m_pRoleCouple(NULL)
    , m_pRoleVIP(NULL)
    , m_pRoleActivity(NULL)
    , m_pRolePhotoRoom(NULL)
    , m_pRoleBet(NULL)
    , m_pRoleAmuseRoom(NULL)
    , m_pRoleQuestNew(NULL)
    , m_pRoleDungeon(NULL)
    , m_pRankComponent(NULL)
    , m_pRoleCheckIn(NULL)
    , m_pRoleMedal(NULL)
{
}

CEntityChatComponent::~CEntityChatComponent(void)
{
}

void CEntityChatComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Chat);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_NeedDanceGroupChatHistory);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GroupDanceChatHistory);

    RegMsgProcessor(MSG_C2S_CHAT, &CEntityChatComponent::OnChat);
    RegMsgProcessor(MSG_C2S_GroupDanceChatHistory, &CEntityChatComponent::OnGroupChatNeedHistoryMsg);
    RegMsgProcessor(MSG_G2S_GroupDanceChatHistory, &CEntityChatComponent::OnGroupChatRecvHistoryMsg);

}

void CEntityChatComponent::Start()
{
    m_pRoleAttr         = GetComponent<CRoleAttribute>();
    m_pRoleNet          = GetComponent<CRoleNet>();
    m_pRoleFriend       = GetComponent<CRoleFriend>();
    m_pRoleRoom         = GetComponent<CRoleRoom>();
    m_pRoleDanceGroup   = GetComponent<CRoleDanceGroup>();
    m_pRoleItem         = GetComponent<CRoleItem>();
    m_pRoleQuest        = GetComponent<CRoleQuest>();
    m_pRoleCouple       = GetComponent<CRoleCouple>();
    m_pRoleVIP          = GetComponent<CRoleVIP>();
    m_pRoleActivity     = GetComponent<CRoleActivity>();
    m_pRolePhotoRoom    = GetComponent<CRolePhotoRoom>();
    m_pRoleBet          = GetComponent<CRoleBet>();
    m_pRoleAmuseRoom    = GetComponent<CRoleAmuseRoom>();
    m_pRoleQuestNew     = GetComponent<CRoleQuestNew>();
    m_pRoleDungeon      = GetComponent<CRoleDungeon>();
    m_pRankComponent    = GetComponent<CRoleRank>();
    m_pRoleCheckIn      = GetComponent<CRoleCheckIn>();
    m_pRoleMedal        = GetComponent<CRoleMedal>();
}

void CEntityChatComponent::OnEvent(CComponentEvent& refEvent )
{
    if (refEvent.nID == eComponentEvent_NormalLogin)
    {
        //GameMsg_SilenceDuration msgDuration;
        //msgDuration.nRoleID = pNewPlayer->GetRoleID();
        //strcpy(msgDuration.szRoleName,pRoleInfoDB->szName);
        //msgDuration.nRoleIndexInServer = pNewPlayer->m_RoleTag.nIndexLogic;
        //SendMsg2GroupServer(&msgDuration);
    }
}

void CEntityChatComponent::OnChat(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_Chat *pChatMsg = (GameMsg_C2S_Chat *)&msg;
    const char *pText = pChatMsg->m_strText.c_str();

    // 命令行
    if (g_pCirculator->m_bEnableClientCommand && 
        pText[0] == '/' && pText[1] == '/'&& 
        GetRoleAttr() != NULL && !GetRoleAttr()->IsNormalPlayer() )
    {
        if (g_pCirculator->GetCommandProcessor() != NULL)
        {
            char szText[MAX_CHAT_TEXT_LEN] = {0};
            strncpy(szText, &pText[2], MAX_CHAT_TEXT_LEN-1);
            g_pCirculator->GetCommandProcessor()->ParseCommand(szText);
            g_pCirculator->GetCommandProcessor()->ProcessCommand(this);
        }

        return;
    }

    GameMsg_S2C_AutoChatResponse responseMsg;
    responseMsg.m_bChatSuccess = false;

    if ( pChatMsg->m_bAutoChat && !CanAutoChat() )
    {
        if ( m_pRoleNet != NULL )
        {
            m_pRoleNet->SendPlayerMsg( &responseMsg );
        }

        return;
    }

    IChat *pChatChannel = CChatManager::Instance().CreateChatChannelBy(pChatMsg->m_nChannel, this);
    if (pChatChannel != NULL && pChatChannel->CanTalk() )
    {
        if ( pChatChannel->OnChat(pChatMsg) )
        {
            responseMsg.m_bChatSuccess = true;

            _LogChat( m_pRoleAttr->GetRoleID(), (eChatChannel)pChatMsg->m_nChannel, pChatMsg->m_strText.c_str(), pChatMsg->m_bVoiceChat );
        }
    }

    if ( pChatMsg->m_bAutoChat && m_pRoleNet != NULL )
        m_pRoleNet->SendPlayerMsg( &responseMsg );

    SAFE_DELETE( pChatChannel );
}

void CEntityChatComponent::OnGroupChatNeedHistoryMsg(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_NeedGroupChatHistory needMsg;

    needMsg.m_nGroupId = m_pRoleDanceGroup->GetDanceGroupID();
    needMsg.m_nRoleId = m_pRoleAttr->GetRoleID();
    SendMsg2GroupServer(&needMsg);
}

void CEntityChatComponent::OnGroupChatRecvHistoryMsg(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GroupDanceChatHistory *pHistoryMsg = (GameMsg_G2S_GroupDanceChatHistory *)&msg;

    GameMsg_S2C_Chat sendMsg(pHistoryMsg->m_strSenderName.c_str(), pHistoryMsg->m_nSenderRoleID, eChatDanceGroup, CHATCOLOR_DANCEGROUPCHANNEL,
        pHistoryMsg->m_strText.c_str(), pHistoryMsg->m_nClickSex, EChatExtraType_Default,
        pHistoryMsg->m_bIsVIP, pHistoryMsg->m_nVIPLevel, pHistoryMsg->m_bVoiceChat, pHistoryMsg->m_WorldChatFlag);

    m_pRoleNet->SendPlayerMsg(&sendMsg);
}

void CEntityChatComponent::OnGroupChatMessage(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_Chat *pChatMsg = (GameMsg_G2S_Chat *)&msg;
    IChat *pChatChannel = CChatManager::Instance().CreateChatChannelBy(pChatMsg->m_nChannel, this);

    if (pChatChannel != NULL)
        pChatChannel->OnGroupChatMessage(pChatMsg);

    SAFE_DELETE( pChatChannel );
}

void CEntityChatComponent::SysChat(const char *szFormat, ...)//发送至本服的玩家
{
    char buffer[MAX_CHAT_BUFF_SIZE];
    va_list args;

    va_start(args, szFormat);
    _vsnprintf(buffer, sizeof(buffer), szFormat, args);
    va_end(args);

    buffer[MAX_CHAT_BUFF_SIZE - 1] = '\0';

    IChat *pChannel = CChatManager::Instance().CreateChatChannelBy(eChatSystem, this);
    static_cast<CSystemChat *>(pChannel)->SysChat(buffer);

    SAFE_DELETE( pChannel );
}

void CEntityChatComponent::SendPlayerDanceGroupAnnouncement( const std::string &rText )
{
    IChat *pChannel = CChatManager::Instance().CreateChatChannelBy( eChatDanceGroup, this );

    if ( pChannel != NULL )
    {
        GameMsg_G2S_Chat msg;
        msg.m_nChannel = eChatDanceGroup;
        msg.m_nColor = CHATCOLOR_DANCEGROUPCHANNEL;
        msg.m_strText = rText;
        msg.m_nExtraType = EChatExtraType_Announcement;

        pChannel->OnGroupChatMessage( &msg );
    }

    SAFE_DELETE( pChannel );
}

void CEntityChatComponent::OnLogin()
{
    SyncAutoChatInfo();
}

void CEntityChatComponent::SetCurFontColor(int nColor)
{
    m_nFontColor = nColor;
}

int CEntityChatComponent::GetCurFontColor()
{
    return m_nFontColor;
}

void CEntityChatComponent::SetLobbyChatTime(time_t tTime)
{
    m_tLastLobbyChatTime = tTime;
}

time_t CEntityChatComponent::LastLobbyChatTime()
{
    return m_tLastLobbyChatTime;
}

CeremonyRoom* CEntityChatComponent::GetCeremony()
{
    ENSURE_WITH_LOG_CMD(NULL != m_pRoleDanceGroup, return NULL, "error to get role[%u] danceGroupent...", m_pRoleAttr->GetRoleID());

    return m_pRoleDanceGroup->GetCeremonyRoom();
}


void CEntityChatComponent::_LogChat( unsigned int nRoleID, eChatChannel eChannel, const char* szText, bool bVoiceChat )
{
    CRoleChatLog *pLog = new CRoleChatLog();

    if ( pLog != NULL )
    {
        pLog->m_nRoleID = nRoleID;
        pLog->m_eChannel = eChannel;
        pLog->m_bVoiceChat = bVoiceChat;
        pLog->m_strText = szText;

        AddQuery(QUERY_LogRoleChat, 0, pLog);
    }
}

bool CEntityChatComponent::CanAutoChat()
{
    unsigned int nTick = GetTickCount();
    unsigned int nInterval = CChatManager::Instance().GetAutoChatInterval() * 1000;

    if (nInterval > MIN_AUTOCHAT_INTERVAL * 2)
        nInterval -= MIN_AUTOCHAT_INTERVAL;
    else
        nInterval = MIN_AUTOCHAT_INTERVAL;

    if ( (nTick - m_nLastAutoChatTime) >= nInterval)
    {
        m_nLastAutoChatTime = nTick;

        return true;
    }

    return false;
}

void CEntityChatComponent::SyncAutoChatInfo()
{
    if (m_pRoleNet == NULL)
        return;

    GameMsg_S2C_SyncAutoChatInfo syncMsg;
    syncMsg.m_fInterval = (float)CChatManager::Instance().GetAutoChatInterval();
    syncMsg.m_fMaxPauseTime = (float)CChatManager::Instance().GetAutoChatMaxPauseTime();

    m_pRoleNet->SendPlayerMsg(&syncMsg);
}

// end of file

