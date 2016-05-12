#include "BigMamaConfigManager.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../socket/Utility.h"

BigMamaConfigManager::BigMamaConfigManager()
    : m_MaxStageInfoProb(0)
    , m_nBeginTime(0)
    , m_nEndTime(0)
    , m_nMaxRound(0)
    , m_nXiaoLianID(0)
    , m_nXiaoRuanID(0)
    , m_nAnnouncementSpace(60*5)
//     , m_bOpen(true)
{

}

BigMamaConfigManager::~BigMamaConfigManager()
{
}

BigMamaConfigManager & BigMamaConfigManager::Instance()
{
    static BigMamaConfigManager g_staticConfigManager;
	return g_staticConfigManager;
}

bool BigMamaConfigManager::LoadConfig(const char* pszFileName)
{
    bool bRes = true;

    CFileStream file;
    file.open(pszFileName, "rb");
    if (file.IsOpen())
    {
        LoadBigMamaStageInfo(file);
        LoadBigMamaExchange(file);
        LoadBigMamaBossConfig(file);

        file.close();
    }
    else
    {
        bRes = false;
    }

    return bRes;
}

bool BigMamaConfigManager::LoadBigMamaStageInfo(CFileStream &file)
{
    unsigned short ulNumber = 0;
    file.ReadUShort(ulNumber);

    for (unsigned short i = 0; i < ulNumber; i++)
    {
        BigMamaStageInfo stageInfo;
        stageInfo.Load(file);

        m_MaxStageInfoProb += stageInfo.m_nRatio;
        m_vecBigMamaStageInfo.push_back(stageInfo);
    }

    return true;

}

bool BigMamaConfigManager::LoadBigMamaExchange(CFileStream &file)
{
    unsigned short ulNumber = 0;
    file.ReadUShort(ulNumber);

    for (unsigned short i = 0; i < ulNumber; i++)
    {
        BigMamaExchange exchange;
        exchange.Load(file);

        if (exchange.m_nSex == 0)
        {
            m_vecMaleBigMamaExchange.push_back(exchange);
            m_vecFemaleBigMamaExchange.push_back(exchange);
        }
        else if(exchange.m_nSex == 1) // male
        {
            m_vecMaleBigMamaExchange.push_back(exchange);
        }
        else if (exchange.m_nSex == 2) // female
        { 
            m_vecFemaleBigMamaExchange.push_back(exchange);
        }
        
    }

    return true;
}

bool BigMamaConfigManager::LoadBigMamaBossConfig(CFileStream &file)
{
    unsigned short ulNumber = 0;
    file.ReadUShort(ulNumber);

    for (unsigned short i = 0; i < ulNumber; ++i)
    {
        BigMamaBossConfig bc;
        bc.Load(file);

        m_mapBigMamaBossConfig.insert(std::make_pair(bc.m_nBossID, bc));
    }

    return true;
}


void BigMamaConfigManager::SetBigMamaConfig(unsigned int nBeginTime, unsigned int nEndTime, 
    unsigned char nMaxRound, unsigned int nXiaoLian, 
    unsigned int nXiaoRuan, std::string strAnnouncement, unsigned int nAnnouncementSpace, const std::string &strIntroduce)
{
//     m_bOpen = open;
    m_nBeginTime = nBeginTime;
    m_nEndTime = nEndTime;
    m_nMaxRound = nMaxRound;
    m_nXiaoLianID = nXiaoLian;
    m_nXiaoRuanID = nXiaoRuan;
    m_strAnnouncement = strAnnouncement;
    m_nAnnouncementSpace = nAnnouncementSpace;
    m_strIntroduce = strIntroduce;

    return;
}

void BigMamaConfigManager::AddBigMamaReward(const BigMamaReward &reward)
{
    m_mapBigMamaReward.insert(std::make_pair(reward.m_rewardType, reward));

    return;
}

void BigMamaConfigManager::AddBigMamaLuckData(const BigMamaLuckData &luckData)
{
    m_vecBigMamaLuckData.push_back(luckData);
}


void BigMamaConfigManager::SetBigMamaGuideReward(std::list<CItem> &maleReard, std::list<CItem> &femaleReward)
{
    m_maleGuideItem.assign(maleReard.begin(), maleReard.end());
    m_femaleGuidItem.assign(femaleReward.begin(), femaleReward.end());
    
    return;
}

std::list<CItem>& BigMamaConfigManager::GetGuideMaleItem()
{
    return m_maleGuideItem;
}

std::list<CItem>& BigMamaConfigManager::GetGuideFemaleItem()
{
    return m_femaleGuidItem;
}


unsigned int BigMamaConfigManager::StartTime() const
{
    return m_nBeginTime;
}

unsigned int BigMamaConfigManager::EndTime() const
{
    return m_nEndTime;
}

unsigned char BigMamaConfigManager::MaxRound() const
{
    return m_nMaxRound;
}

unsigned int BigMamaConfigManager::XiaoLianID() const
{
    return m_nXiaoLianID;
}

unsigned int BigMamaConfigManager::XiaoRuanID() const
{
    return m_nXiaoRuanID;
}

std::string BigMamaConfigManager::AnnouncementString() const
{
    return m_strAnnouncement;
}

unsigned int BigMamaConfigManager::AnnouncementSpace() const
{
    return m_nAnnouncementSpace;
}

std::string BigMamaConfigManager::IntroductString() const
{
    return m_strIntroduce;
}

// bool BigMamaConfigManager::IsOpen() const
// {
//     return m_bOpen;
// }

BigMamaReward* BigMamaConfigManager::GetBigMamaReward(EBigMamaRewardsType rewardType)
{
    BigMamaRewardMap::iterator iter = m_mapBigMamaReward.find(rewardType);
    if (iter != m_mapBigMamaReward.end())
        return &(iter->second);

    return NULL;
}

// 取得关卡，如果关卡为空，则取得小恋或者小软
BigMamaStageInfo* BigMamaConfigManager::RankStage(unsigned short nDanceGroupLevel, unsigned int &npcID)
{
    npcID = 0;
    
    // 先随机小恋或者小软
    unsigned int r = Random(1, 10000);
    unsigned int sum = 0;
    for (unsigned int i = 0; i < m_vecBigMamaLuckData.size(); ++i)
    {
        sum += (m_vecBigMamaLuckData[i].m_nBaseProb + m_vecBigMamaLuckData[i].m_nProbGrowRatio * nDanceGroupLevel);
        if (r <= sum)
        {
            npcID = m_vecBigMamaLuckData[i].m_nNpcID; // 
            return NULL;
        }
    }

    // 如果没有随中，在随机关卡
//    BigMamaStageInfo *retStage = NULL;
    r = Random(1, m_MaxStageInfoProb);
    sum = 0;
    for (unsigned int i = 0; i < m_vecBigMamaStageInfo.size(); ++i)
    {
        sum += m_vecBigMamaStageInfo[i].m_nRatio;
        if (r <= sum)
            return &m_vecBigMamaStageInfo[i];
    }

    if (!m_vecBigMamaStageInfo.empty())
        return &m_vecBigMamaStageInfo[0]; // 如果没空，直接返回这个大妈

    return NULL;
}

BigMamaStageInfo* BigMamaConfigManager::GetStageInfo(unsigned short nStageID)
{
    for (unsigned int i = 0; i < m_vecBigMamaStageInfo.size(); ++i)
    {
        if (m_vecBigMamaStageInfo[i].m_nStageID == nStageID)
            return &m_vecBigMamaStageInfo[i];
    }

    return NULL;
}

BigMamaExchange* BigMamaConfigManager::GetExchange(ESexType sex, unsigned char index)
{
    if (sex == ESexType_Female)
    {
        if (index < m_vecFemaleBigMamaExchange.size())
            return &m_vecFemaleBigMamaExchange[index];
    }
    else if (sex == ESexType_Male)
    {
        if (index < m_vecMaleBigMamaExchange.size())
            return &m_vecMaleBigMamaExchange[index];
    }

    return NULL;
}

std::vector<BigMamaExchange>& BigMamaConfigManager::GetExchangeList(ESexType sex)
{
    if (sex == ESexType_Female)
    {
        return m_vecFemaleBigMamaExchange;
    }
    else
    {
        return m_vecMaleBigMamaExchange;
    }
}

BigMamaBossConfig* BigMamaConfigManager::GetBigMamaBossConfig(int bossID)
{
    std::map<int, BigMamaBossConfig>::iterator iter = m_mapBigMamaBossConfig.find(bossID);
    if (iter != m_mapBigMamaBossConfig.end())
        return &iter->second;

    return NULL;
}

