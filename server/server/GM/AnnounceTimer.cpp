#include "AnnounceTimer.h"
#include "GameMsg_GM_share.h"
#include "GMCirculator.h"
#include "AnnounceMgr.h"


extern CGMCirculator* g_pGMCirculator;

CAnnounceTimer::CAnnounceTimer()
:m_nTriggerTime(0)
, m_nTimerType(ETimerType_Announce)
{

}

CAnnounceTimer::~CAnnounceTimer()
{

}

void CAnnounceTimer::OnEvent()
{
    GameMsg_M2S_Announce msgAnnounce;
    msgAnnounce.m_announce.m_bShowInChatWindow = m_annouceInfo.m_bIsSysChat;
    msgAnnounce.m_announce.m_bIsAdminChat = m_annouceInfo.m_bIsAdminChat;
    msgAnnounce.m_announce.m_strContent = m_annouceInfo.m_strContent;
    msgAnnounce.m_announce.m_nPID = m_annouceInfo.m_nPID;

    g_pGMCirculator->SendMsgToGroup(&msgAnnounce);
    WriteLog(LOGLEVEL_DEBUG, "Send annoucement content = %s, pid[%d]\n", m_annouceInfo.m_strContent.c_str(), m_annouceInfo.m_nPID);

    CAnnounceMgr::Instance().HandleNextAnnounce(m_annouceInfo);
}

time_t CAnnounceTimer::TriggerTime()
{
    return m_nTriggerTime;
}

ETimerType CAnnounceTimer::TimerType()
{
    return ETimerType_Announce;
}

unsigned int CAnnounceTimer::TimerID()
{
    return m_annouceInfo.m_nID;
}

