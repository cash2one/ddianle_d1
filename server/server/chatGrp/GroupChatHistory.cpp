#include "GroupChatHistory.h"


CChatHistory::CChatHistory(const CChatHistory& rOther)
{
    m_nGroupId = rOther.m_nGroupId;
    m_nSenderRoleID = rOther.m_nSenderRoleID;
    m_nColor = rOther.m_nColor;
    m_nChannel = rOther.m_nChannel;
    m_strSenderName = rOther.m_strSenderName;
    m_strText = rOther.m_strText;
    m_nClickSex = rOther.m_nClickSex;
    m_nExtraType = rOther.m_nExtraType;
    m_bIsVIP = rOther.m_bIsVIP;
    m_nVIPLevel = rOther.m_nVIPLevel;
    m_bVoiceChat = rOther.m_bVoiceChat;
    m_WorldChatFlag = rOther.m_WorldChatFlag;
}

CChatHistory::CChatHistory(const GameMsg_S2G_GroupDanceChatHistoryRecord& rRecord)
{
    m_nGroupId = rRecord.m_nGroupId;
    m_nSenderRoleID = rRecord.m_nSenderRoleID;
    m_nColor = rRecord.m_nColor;
    m_nChannel = rRecord.m_nChannel;
    m_strSenderName = rRecord.m_strSenderName;
    m_strText = rRecord.m_strText;
    m_nClickSex = rRecord.m_nClickSex;
    m_nExtraType = rRecord.m_nExtraType;
    m_bIsVIP = rRecord.m_bIsVIP;
    m_nVIPLevel = rRecord.m_nVIPLevel;
    m_bVoiceChat = rRecord.m_bVoiceChat;
    m_WorldChatFlag = rRecord.m_WorldChatFlag;
}

CChatHistory::CChatHistory()
    : m_nGroupId(0)
    ,m_nSenderRoleID(0)
    ,m_nColor(0)
    ,m_nChannel(0)
    ,m_strSenderName("")
    ,m_strText("")
    ,m_nClickSex(0)
    ,m_nExtraType(-1)
    ,m_bIsVIP(false)
    ,m_nVIPLevel(0)
    ,m_bVoiceChat(false)
    ,m_WorldChatFlag(0)
{

}

CGroupChatHistory::CGroupChatHistory()
{}

CGroupChatHistory::~CGroupChatHistory()
{}

void CGroupChatHistory::RecordHistory(int nGroupId, CChatHistory& rRecord)
{
    const int nHistoryLen = 9;
    std::map<int, std::list<CChatHistory> >::iterator it = m_mapHistory.find(nGroupId);
    if (it == m_mapHistory.end())
    {
        std::list<CChatHistory> list;
        list.push_back(rRecord);
        m_mapHistory.insert(std::make_pair(nGroupId, list));
    }
    else
    {
        if ((int)it->second.size() > nHistoryLen)
        {
            it->second.pop_front();
            it->second.push_back(rRecord);
        }
        else
        {
            it->second.push_back(rRecord);
        }
    }
}

void CGroupChatHistory::DataCopyTo(GameMsg_S2G_GroupDanceChatHistoryRecord& data, CChatHistory & rRecord)
{
    rRecord = CChatHistory(data);
}

void CGroupChatHistory::GetHistory(int nGroupId, std::list<CChatHistory> &rListHistory)
{
    std::map<int, std::list<CChatHistory> >::iterator it = m_mapHistory.find(nGroupId);
    if (it == m_mapHistory.end())
    {
        rListHistory.clear();
    }
    else
    {
        rListHistory = it->second;
    }
}