#include "AnnouncementMgr.h"
#include <algorithm>
#include "../../datastructure/DataStruct_Base.h"

AnnouncementMgr::AnnouncementMgr() :
m_nVersion(0),
m_nLastIndex(-1)
{
}

AnnouncementMgr::~AnnouncementMgr()
{
}

AnnouncementMgr & AnnouncementMgr::Instance()
{
    static AnnouncementMgr s_AnnouncementMgr;

    return s_AnnouncementMgr;
}

void AnnouncementMgr::AddAnnouncementInfo()
{

    CAnnouncement info;
    m_vecAnnouncement.push_back(info);
    ++m_nLastIndex;
}

void AnnouncementMgr::AddBriefInfo(unsigned char nType, string strTitle, string strContent, string strDate, bool bHot, unsigned char nLinkTo, unsigned short nPID)
{
    CAnnouncement & info = m_vecAnnouncement[m_nLastIndex];
    info.m_nType = nType;
    info.m_strTitle = strTitle;
    info.m_strContent = strContent;
    info.m_strDate = strDate;
    info.m_bHot = bHot;
    info.m_nLinkTo = nLinkTo;
    info.m_nPID = nPID;
}

void AnnouncementMgr::AddIcons(string &strIcon)
{
    CAnnouncement & info = m_vecAnnouncement[m_nLastIndex];
    info.m_vecStrIcon.push_back(strIcon);
}

void AnnouncementMgr::AddVersion(int nVersion)
{
    m_nVersion = nVersion;
}

int AnnouncementMgr::Version()
{
    return m_nVersion;
}

