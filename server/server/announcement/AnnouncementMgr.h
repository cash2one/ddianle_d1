#ifndef __ANNOUNCEMENTMGR_H__
#define __ANNOUNCEMENTMGR_H__

#include <string>
#include <vector>
using namespace std;

class CAnnouncement;

class AnnouncementMgr
{
private:
    AnnouncementMgr();
    ~AnnouncementMgr();

public:
    static AnnouncementMgr & Instance();
    void AddAnnouncementInfo();
    void AddBriefInfo(unsigned char nType, string strTitle, string strContent, string strDate, bool bHot, unsigned char nLinkTo, unsigned short nPID);
    void AddIcons(string &strIcon);

    void AddVersion(int nVersion);
    int Version();

public:
    vector<CAnnouncement > m_vecAnnouncement;
    int m_nVersion;

private:
    int m_nLastIndex;
};

#endif

