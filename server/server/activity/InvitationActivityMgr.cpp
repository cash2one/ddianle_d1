#include "InvitationActivityMgr.h"
#include "InvitationActivity.h"
#include "SystemActivityManager.h"
#include "../../socket/Md5.h"
#include "../../socket/Utility.h"
#include "../../socket/Formatter.h"
#include "../util/CommonFunc.h"


CInvitationActivityMgr::CInvitationActivityMgr()
: m_bIsOpening(false)
, m_nWeight(0)
, m_bExhibit(false)
, m_tActivityBeginTime(0)
, m_tActivityEndTime(0)
, m_nVisibleMaxLevel(0)
{
}
CInvitationActivityMgr::~CInvitationActivityMgr()
{
}

CInvitationActivityMgr & CInvitationActivityMgr::Instance()
{
    static CInvitationActivityMgr s_instance;
    return s_instance;
}

void CInvitationActivityMgr::AddActivity(
    int weight, bool exhibit, const char *pszActivityImage, const char *pszActivityThumbnail,
    time_t tActivityBeginTime, time_t tActivityEndTime,
    const char* strActivityName, const char* strActivityContent,
    const std::list<CItem> &listMaleReward, std::list<CItem> &listFemaleItem, unsigned int nMoney,
    const char *pszRewardMailTitle, const char *pszRewardMailContent, int nVisibleMaxLevel)

{
    m_nWeight = weight;
    m_bExhibit = exhibit;
    m_strActivityImage = pszActivityImage;
    m_strActivityThumbnailImage = pszActivityThumbnail;
    m_tActivityBeginTime = tActivityBeginTime;
    m_tActivityEndTime = tActivityEndTime;
    m_strActivityName = strActivityName;
    m_strActivityContent = strActivityContent;
    m_maleReward.m_nMoney = nMoney;
    m_femaleReward.m_nMoney = nMoney;
    m_maleReward.m_listItem = listMaleReward;
    m_femaleReward.m_listItem = listFemaleItem;
    m_strRewardMailTitle = pszRewardMailTitle;
    m_strRewardMailContent = pszRewardMailContent;
    m_nVisibleMaxLevel = nVisibleMaxLevel;
}

bool CInvitationActivityMgr::Init()
{
    if (m_tActivityBeginTime > m_tActivityEndTime)
        return false;

    CInvitationActivity *pActivity = new CInvitationActivity;
    pActivity->Init(m_tActivityBeginTime, m_tActivityEndTime);
    SetIsOpening(pActivity->IsOpening());
    CSystemActivityManager::Instance().AddTimerActivity(pActivity);
    return true;
}

void CInvitationActivityMgr::SetIsOpening(bool bIsOpening/* = true*/)
{
    m_bIsOpening = bIsOpening;
}

bool CInvitationActivityMgr::IsActivityOpening()
{
    return m_bIsOpening;
}


bool CInvitationActivityMgr::Exhibit()
{
    return m_bExhibit;
}

int CInvitationActivityMgr::Weight()
{
    return m_nWeight;
}

std::string &CInvitationActivityMgr::ActivityImageName()
{
    return m_strActivityImage;
}

std::string &CInvitationActivityMgr::ActivityThumbnailImageName()
{
    return m_strActivityThumbnailImage;
}


time_t CInvitationActivityMgr::ActivityStartTime()
{
    return m_tActivityBeginTime;
}

time_t CInvitationActivityMgr::ActivityEndTime()
{
    return m_tActivityEndTime;
}

std::string& CInvitationActivityMgr::ActivityName()
{
    return m_strActivityName;
}

std::string& CInvitationActivityMgr::ActivityContent()
{
    return m_strActivityContent;
}

std::string & CInvitationActivityMgr::MailTitle()
{
    return m_strRewardMailTitle;
}

std::string & CInvitationActivityMgr::MailContent()
{
    return m_strRewardMailContent;
}

int CInvitationActivityMgr::VisibleMaxLevel()
{
    return m_nVisibleMaxLevel;
}

const CInvitationAcceptorReward& CInvitationActivityMgr::Reward(unsigned char nSex) const
{
    if (nSex == ESexType_Male)
        return m_maleReward;

    return m_femaleReward;
}


//////////////////////////////////////////////////////////////////////////
CInvitationInviterConfigMgr::CInvitationInviterConfigMgr() :
m_nRequiredLevel(0),
m_tActivityBeginTime(0),
m_tActivityEndTime(0)
{
}
CInvitationInviterConfigMgr::~CInvitationInviterConfigMgr()
{
}

CInvitationInviterConfigMgr & CInvitationInviterConfigMgr::Instance()
{
    static CInvitationInviterConfigMgr s_instance;
    return s_instance;
}

void CInvitationInviterConfigMgr::AddActivity(
    const char* strActivityName, const char* strActivityContent, int nRequiredLevel,
    const char *strRewardMailTitle, const char *strRewardMailContent,
    const char *strActivityBeginTime, const char *strActivityEndTime)
{
    m_strActivityName = strActivityName;
    m_strActivityContent = strActivityContent;
    m_nRequiredLevel = nRequiredLevel;
    m_strRewardMailTitle = strRewardMailTitle;
    m_strRewardMailContent = strRewardMailContent;
    m_tActivityBeginTime = SetTime(strActivityBeginTime);
    m_tActivityEndTime = SetTime(strActivityEndTime);
}

bool CInvitationInviterConfigMgr::IsActivityOpen() const
{
    time_t tNow = time(NULL);
    return (tNow >= m_tActivityBeginTime && tNow <= m_tActivityEndTime);
}

void CInvitationInviterConfigMgr::AddCumulativeReward(const CInvitationInviterReward &reward, unsigned char nSex)
{
    if (nSex == ESexType_Male)
    {
        m_maleCumulativeReward[reward.m_nLevel] = reward;
    }
    else
    {
        m_femaleCumulativeReward[reward.m_nLevel] = reward;
    }
}

void CInvitationInviterConfigMgr::MakeInvitationCode(unsigned int nRoldID, char *szInvitationCode)
{
    unsigned char szDigest[16] = { 0 };
    std::string strBuff;

    Formatter(strBuff) << "Invitation(" << nRoldID << "|" << (unsigned)time(NULL) << ")";
    MDString(strBuff.c_str(), szDigest);

    const static char kInvitationCodeTable[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";
    const static int  kInvitationCodeTableSize = sizeof(kInvitationCodeTable);

    unsigned short* pDigest = (unsigned short*)szDigest;
    for (int i = 0; i < MAX_INVITATION_CODE_LEN - 1; i++)
        szInvitationCode[i] = kInvitationCodeTable[pDigest[i] % (kInvitationCodeTableSize - 1)];
    szInvitationCode[MAX_INVITATION_CODE_LEN - 1] = '\0';
}

void CInvitationInviterConfigMgr::AddShareAnimation(const char *pszMaleAnimation, const char *pszFemaleAnimation)
{
    m_vecMaleAnimation.push_back(string(pszMaleAnimation));
    m_vecFemaleAnimation.push_back(string(pszFemaleAnimation));
}

const char *CInvitationInviterConfigMgr::RandomShareAnimation(unsigned char nSex)
{
    int nRandomIndex = 0;
    if (m_vecMaleAnimation.size() > 0)
    {
        nRandomIndex = Random(0, (int)(m_vecMaleAnimation.size() - 1));

        if (nSex == ESexType_Male)
            return m_vecMaleAnimation[nRandomIndex].c_str();

        return m_vecFemaleAnimation[nRandomIndex].c_str();
    }
    return "";
}

std::string& CInvitationInviterConfigMgr::ActivityName()
{
    return m_strActivityName;
}

std::string& CInvitationInviterConfigMgr::ActivityContent()
{
    return m_strActivityContent;
}

int CInvitationInviterConfigMgr::AcceptorRequiredLevel() const
{
    return m_nRequiredLevel;
}

std::string & CInvitationInviterConfigMgr::MailTitle()
{
    return m_strRewardMailTitle;
}

std::string & CInvitationInviterConfigMgr::MailContent()
{
    return m_strRewardMailContent;
}

const InvitationCumulativeReward& CInvitationInviterConfigMgr::CumulativeReward(unsigned char nSex) const
{
    if (nSex == ESexType_Male)
        return m_maleCumulativeReward;

    return m_femaleCumulativeReward;
}

const CInvitationInviterReward* CInvitationInviterConfigMgr::GetCumulativeReward(unsigned int nLevel, unsigned char nSex) const
{
    if (nSex == ESexType_Male)
    {
        InvitationCumulativeReward::const_iterator it = m_maleCumulativeReward.find(nLevel);
        if (it != m_maleCumulativeReward.end())
            return &it->second;
    }
    else
    {
        InvitationCumulativeReward::const_iterator it = m_femaleCumulativeReward.find(nLevel);
        if (it != m_femaleCumulativeReward.end())
            return &it->second;
    }

    return NULL;
}

