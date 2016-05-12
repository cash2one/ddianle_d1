#include "WChat.h"
#include "GroupPrivateChat.h"
#include "GroupWorldChat.h"
#include "GroupDanceGroupChat.h"
#include "GroupAdminChat.h"

#include "../dancegroup/DanceGroupProcessor.h"

#include "../group/GroupServerCirculator.h"

#include "../../socket/GameMsg_Map.h"
#include "../chat/ChatMsgDef.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include "../GM/GameMsg_G2S_Announce.h"

#include "../chatGrp/GroupChatHistory.h"
#include "../group/Global.h"



CWChatMgr::CWChatMgr()
: GameMsg_Processor(true, true)
, m_nAutoChatSendInterval(0)
, m_nLastAutoChatSendTime(0)
{
}

CWChatMgr::~CWChatMgr()
{
    while (!m_queueAutoChatMsg.empty())
    {
        GameMsg_S2G_Chat *pChatMsg = m_queueAutoChatMsg.front();

        m_queueAutoChatMsg.pop();
        SAFE_DELETE(pChatMsg);
    }
}

void CWChatMgr::Initialize()
{
    RegisterMessage();
    RegisterMessageProcessor();
}

void CWChatMgr::SetAutoChatSendInterval(unsigned int nInterval)
{
    m_nAutoChatSendInterval = nInterval;
}

void CWChatMgr::OnUpdate(int nTimeElapse)
{
    if (m_queueAutoChatMsg.empty())
    {
        return;
    }
    else if (CanSendAutoChatMsg())
    {
        GameMsg_S2G_Chat *pChatMsg = m_queueAutoChatMsg.front();

        OnChat(pChatMsg);

        m_queueAutoChatMsg.pop();
        SAFE_DELETE(pChatMsg);
    }
}

void CWChatMgr::SendDanceGroupAnnouncement(unsigned int nGroupID, const std::string &rText)
{
    do
    {
        BREAK_IF(nGroupID == 0);
        BREAK_IF(rText.empty());

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(nGroupID);
        BREAK_IF(pDGInfo == NULL);

        const std::map<unsigned int, CDanceGroupMember *> &rMembers = pDGInfo->GetAllMember();
        BREAK_IF(rMembers.empty());

        IGroupChat *pChannel = CreateChatChannelBy(eChatDanceGroup);
        BREAK_IF(pChannel == NULL);

        GameMsg_S2G_Chat msg(eChatPhrase_Forward, eChat_Success, eChatDanceGroup, CHATCOLOR_DANCEGROUPCHANNEL,
            0, 0, "", "", rText.c_str(), 0, EChatExtraType_Announcement, false, 0, false, 0);

        for (std::map<unsigned int, CDanceGroupMember *>::const_iterator itr = rMembers.begin();
            itr != rMembers.end(); ++itr)
        {
            msg.m_nReceiverRoleID = itr->first;

            pChannel->OnChat(&msg);
        }

        SAFE_DELETE(pChannel);
    } while (0);
}

void CWChatMgr::SendAllAnnouncement(CAnnounceBase &ann)
{
    GameMsg_G2S_Announce msgAnnounce;
    msgAnnounce.m_announce = ann;

    BroadcastMsg2GameServer(&msgAnnounce);
}


void CWChatMgr::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Chat);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Announce);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GroupDanceChatHistoryRecord);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NeedGroupChatHistory);
}

void CWChatMgr::RegisterMessageProcessor()
{
    RegMsgProcessor(MSG_S2G_Chat, &CWChatMgr::OnNetMessage);
    RegMsgProcessor(MSG_S2G_Announce, &CWChatMgr::OnSendAnnounce);
    RegMsgProcessor(MSG_S2G_GroupDanceChatHistoryRecord, &CWChatMgr::OnRecordChatHistory);
    RegMsgProcessor(MSG_S2G_GroupDanceChatHistory, &CWChatMgr::OnNeedChatHistory);
}

IGroupChat *CWChatMgr::CreateChatChannelBy(unsigned char nChannel)
{
    IGroupChat *pChannel = NULL;

    if (nChannel == eChatWorld)
    {
        pChannel = new CGroupWorldChat;
    }
    else if (nChannel == eChatPrivate)
    {
        pChannel = new CGroupPrivateChat;
    }
    else if (nChannel == eChatDanceGroup)
    {
        pChannel = new CGroupDanceGroupChat;
    }
    else if (nChannel == eChatAdmin)
    {
        pChannel = new CGroupAdminChat;
    }

    return pChannel;
}

void CWChatMgr::OnNetMessage(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_Chat *pChatMsg = (GameMsg_S2G_Chat *)&msg;

    if (pChatMsg != NULL)
    {
        if (PreprocessMsg(pChatMsg))
        {
            return;
        }

        OnChat(pChatMsg);
    }
}

void CWChatMgr::OnSendAnnounce(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_Announce &msgReceived = (GameMsg_S2G_Announce &)msg;

    GameMsg_G2S_Announce msgAnnounce;
    msgAnnounce.m_announce = msgReceived.m_announce;

    BroadcastMsg2GameServer(&msgAnnounce);
}

void CWChatMgr::OnChat(GameMsg_S2G_Chat *pChatMsg)
{
    if (pChatMsg == NULL)
        return;

    IGroupChat *pChannel = CreateChatChannelBy(pChatMsg->m_nChannel);

    if (pChannel != NULL)
        pChannel->OnChat(pChatMsg);

    SAFE_DELETE(pChannel);
}

void CWChatMgr::OnRecordChatHistory(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GroupDanceChatHistoryRecord *pMsg = (GameMsg_S2G_GroupDanceChatHistoryRecord*)&msg;
    CChatHistory history;
    CGroupChatHistory::Instance().DataCopyTo(*pMsg, history);
    CGroupChatHistory::Instance().RecordHistory(pMsg->m_nGroupId, history);
}

void CWChatMgr::OnNeedChatHistory(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_NeedGroupChatHistory *HistoryMsg = (GameMsg_S2G_NeedGroupChatHistory*) &msg;

    CGroupRoleEntity *pSender = CGroupPlayerManager::Instance().SearchRoleByID(HistoryMsg->m_nRoleId);
    if (pSender == NULL)
    {
        return;
    }

    std::list<CChatHistory> listHistory;
    CGroupChatHistory::Instance().GetHistory(HistoryMsg->m_nGroupId, listHistory);
    for (std::list<CChatHistory>::iterator it = listHistory.begin();
        it != listHistory.end(); ++it)
    {
        GameMsg_G2S_GroupDanceChatHistory msg(*it);
        pSender->SendPlayerMsg(&msg);
    }
}

bool CWChatMgr::PreprocessMsg(GameMsg_S2G_Chat *pChatMsg)
{
    if (!pChatMsg->m_bAutoChat)
        return false;
    else if (m_queueAutoChatMsg.empty() && CanSendAutoChatMsg())
        return false;

    GameMsg_S2G_Chat *pSave = new GameMsg_S2G_Chat(*pChatMsg);

    m_queueAutoChatMsg.push(pSave);

    return true;
}

bool CWChatMgr::CanSendAutoChatMsg()
{
    if (m_nAutoChatSendInterval == 0)
        return true;

    unsigned int nTick = GetTickCount();

    if ((nTick - m_nLastAutoChatSendTime) >= m_nAutoChatSendInterval)
    {
        m_nLastAutoChatSendTime = nTick;

        return true;
    }

    return false;
}

