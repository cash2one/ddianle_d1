#ifndef __INVITATION_ACTIVITY_MGR_H__
#define __INVITATION_ACTIVITY_MGR_H__

#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "ActivityDataStruct.h"

/**
*	玩家招募活动配置信息管理器
*/
class CInvitationActivityMgr
{
private:
    CInvitationActivityMgr();
    ~CInvitationActivityMgr();

public:
    static CInvitationActivityMgr & Instance();

public:
    void AddActivity(
        int weight, bool exhibit, const char *pszActivityImage, const char *pszActivityThumbnail,
        time_t tActivityBeginTime, time_t tActivityEndTime,
        const char* strActivityName, const char* strActivityContent,
        const std::list<CItem> &listMaleReward, std::list<CItem> &listFemaleItem, unsigned int nMoney,
        const char *pszRewardMailTitle, const char *pszRewardMailContent, int visibleMaxLevel);

    bool Init();

    void SetIsOpening(bool bIsOpening = true);

    bool IsActivityOpening();
    time_t ActivityStartTime();
    time_t ActivityEndTime();
    std::string & ActivityName();
    std::string & ActivityContent();

    bool Exhibit();
    int Weight();
    std::string &ActivityImageName();
    std::string &ActivityThumbnailImageName();

    std::string & MailTitle();
    std::string & MailContent();

    int VisibleMaxLevel();

    const CInvitationAcceptorReward& Reward(unsigned char nSex) const;

protected:
    bool m_bIsOpening;

    int m_nWeight;
    bool m_bExhibit;
    std::string m_strActivityImage;
    std::string m_strActivityThumbnailImage;

    time_t m_tActivityBeginTime;
    time_t m_tActivityEndTime;
    std::string m_strActivityName;
    std::string m_strActivityContent;

    CInvitationAcceptorReward m_maleReward;
    CInvitationAcceptorReward m_femaleReward;

    std::string m_strRewardMailTitle;
    std::string m_strRewardMailContent;

    int m_nVisibleMaxLevel;
};

class CInvitationInviterConfigMgr
{
private:
    CInvitationInviterConfigMgr();
    ~CInvitationInviterConfigMgr();

public:
    static CInvitationInviterConfigMgr & Instance();

    // 生成邀请码
    static void MakeInvitationCode(unsigned int nRoldID, char *szInvitationCode);

public:
    void AddActivity(
        const char* strActivityName, const char* strActivityContent, int nRequiredLevel,
        const char *strRewardMailTitle, const char *strRewardMailContent,
        const char *strActivityBeginTime, const char *strActivityEndTime);

    bool IsActivityOpen() const;

    void AddCumulativeReward(const CInvitationInviterReward &reward, unsigned char nSex);

    void AddShareAnimation(const char *pszMaleAnimation, const char *pszFemaleAnimation);

    const char * RandomShareAnimation(unsigned char nSex);

    std::string & ActivityName();
    std::string & ActivityContent();
    int AcceptorRequiredLevel() const;
    std::string & MailTitle();
    std::string & MailContent();
    unsigned int ActivityBeginTime() const {
        return (unsigned int)m_tActivityBeginTime;
    };
    unsigned int ActivityEndTime() const {
        return (unsigned int)m_tActivityEndTime;
    };

    const InvitationCumulativeReward& CumulativeReward(unsigned char nSex) const;
    const CInvitationInviterReward* GetCumulativeReward(unsigned int nLevel, unsigned char nSex) const;

protected:
    std::string m_strActivityName;
    std::string m_strActivityContent;
    int m_nRequiredLevel;
    std::string m_strRewardMailTitle;
    std::string m_strRewardMailContent;
    time_t m_tActivityBeginTime;
    time_t m_tActivityEndTime;

    InvitationCumulativeReward m_maleCumulativeReward;
    InvitationCumulativeReward m_femaleCumulativeReward;

    std::vector<std::string> m_vecMaleAnimation;
    std::vector<std::string> m_vecFemaleAnimation;
};

#endif //__INVITATION_ACTIVITY_MGR_H__

