#include "ChatManager.h"

#include "ChatMsgDef.h"

#include "../logic/PlayerManager.h"
#include "EntityChatComponent.h"

#include "../../socket/GameMsg_Map.h"

#include "../../datastructure/Localization.h"
#include "GameMsg_S2G_Chat.h"

#include "GlobalChat.h"
#include "SystemChat.h"
#include "PrivateChat.h"
#include "WorldChat.h"
#include "CurrentChat.h"
#include "DanceGroupChat.h"
#include "AdminChat.h"

#include <string>
#include <stdarg.h>

#ifndef WIN32
#define _vsnprintf  vsnprintf
#endif

extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CChatManager::CChatManager()
    : GameMsg_Processor(true, true)
    , m_nAutoChatInterval(0)
    , m_nAutoChatMaxPauseTime(0)
{
}

CChatManager::~CChatManager()
{
}

bool CChatManager::Initialize()
{
    RegChatManagerNetMsgMap();

    return CheckConfig();
}

void CChatManager::SetAutoChatInfos(unsigned int nInterval, unsigned int nMaxPauseTime)
{
    m_nAutoChatInterval = nInterval;
    m_nAutoChatMaxPauseTime = nMaxPauseTime;
}

void CChatManager::RegChatManagerNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Chat);

    RegMsgProcessor(MSG_G2S_Chat, &CChatManager::OnGroupChat);
}

IChat * CChatManager:: CreateChatChannelBy(unsigned char nChannel, CEntityChatComponent *pRoleChat)
{
    IChat * pChannel = NULL;

    if (nChannel == eChatWorld)
    {
        pChannel = new CWorldChat(pRoleChat);
    }
    else if (nChannel == eChatGlobal)
    {
        pChannel = new CGlobalChat(pRoleChat);
    }
    else if (nChannel == eChatCurrent)
    {
        pChannel = new CCurrentChat(pRoleChat);
    }
    else if (nChannel == eChatPrivate)
    {
        pChannel = new CPrivateChat(pRoleChat);
    }
    else if (nChannel == eChatSystem)
    {
        pChannel = new CSystemChat(pRoleChat);
    }
    else if (nChannel == eChatDanceGroup)
    {
        pChannel = new CDanceGroupChat(pRoleChat);
    }
    else if (nChannel == eChatAdmin)
    {
        pChannel = new CAdminChat(pRoleChat);
    }

    return pChannel;
}

void CChatManager::AdminChat(const char * szFormat, ...)
{
    char buffer[MAX_CHAT_BUFF_SIZE] = {0};
    va_list args;

    va_start(args, szFormat);
    _vsnprintf(buffer, sizeof(buffer), szFormat, args);
    va_end(args);

    buffer[MAX_CHAT_BUFF_SIZE - 1] = '\0';

    GameMsg_S2G_Chat sendMsg(eChatPhrase_Forward, eChat_Success, eChatAdmin, 
        CHATCOLOR_ADMINCHANNEL, 0, 0, 
        CLocalization::Instance().GetString("AdminAnnounce").c_str(), 
        "", buffer, 0, EChatExtraType_Default, false, 0, false, 0);

    SendMsg2GroupServer(&sendMsg);
}

void CChatManager::SendAnnounce(bool bSysChat, bool bAdminChat, std::string &strContent)
{
    GameMsg_S2G_Announce msgAnnounce;

    msgAnnounce.m_announce.m_bShowInChatWindow = bSysChat;
    msgAnnounce.m_announce.m_bIsAdminChat = bAdminChat;
    msgAnnounce.m_announce.m_strContent = strContent;

    SendMsg2GroupServer(&msgAnnounce);
}

void CChatManager::OnGroupChat( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_G2S_Chat &refMsg = (GameMsg_G2S_Chat &)msg;

    CEntityChatComponent *pChat = NULL;
    unsigned int nSearchID = 0;

    if (refMsg.m_nPhase == eChatPhrase_Forward)
        nSearchID = refMsg.m_nReceiverRoleID;
    else if (refMsg.m_nPhase == eChatPhrase_Backward)
        nSearchID = refMsg.m_nSenderRoleID;

    if (nSearchID != 0)
    {
        CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nSearchID);

        if (pRole != NULL)
        {
            pChat = pRole->GetComponent<CRoleChat>();
        }
    }

    IChat *pChatChannel = CChatManager::Instance().CreateChatChannelBy(refMsg.m_nChannel, pChat);
    if (pChatChannel != NULL)
        pChatChannel->OnGroupChatMessage(&refMsg);

    SAFE_DELETE( pChatChannel );
}

bool CChatManager::CheckConfig() const
{
    if (m_nAutoChatInterval == 0)
    {
        WriteLog(LOGLEVEL_ERROR, "Autochat interval is 0.");

        return false;
    }

    return true;
}

