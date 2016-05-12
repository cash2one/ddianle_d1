#include "AchievementConfig.h"
#include "./FileStream.h"


AchievementItemReward::AchievementItemReward(itemtype_t nItemID, unsigned short nItemCount)
{
    m_nItemID = nItemID;	
    m_nItemCount = nItemCount;
}

AchievementItemReward::~AchievementItemReward()
{
}

void AchievementItemReward::Read(CFileStream &file)
{
    file.ReadUInt(m_nItemID);
    file.ReadUShort(m_nItemCount);
}

void AchievementItemReward::Write(CFileStream  &file)
{
    file.WriteUInt(m_nItemID);
    file.WriteUShort(m_nItemCount);
}

bool AchievementItemReward::IsValid() const
{
    if (m_nItemID == 0 || m_nItemCount == 0)
    {
        return false;
    }

    return true;
}

itemtype_t AchievementItemReward::GetItemID()
{
    return m_nItemID;
}

unsigned short AchievementItemReward::GetItemCount()
{
    return m_nItemCount;
}

AchievementConfig::AchievementConfig()
    :m_nAchievementID(0)
    ,m_nClassID(0)
    ,m_nSeriesID(0)
    ,m_strAchievementName("")
    ,m_nLevel(0)
    ,m_nTitleID(0)
    ,m_strDetail("")
    ,m_nHandlerID(0)
    ,m_nCount(0)
    ,m_nScore(0)
    ,m_nRewardMoney(0)
    ,m_bNotify(false)
{
}

AchievementConfig::~AchievementConfig()
{
}

bool AchievementConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nAchievementID);
    file.ReadUShort(m_nClassID);
    file.ReadUShort(m_nSeriesID);
    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strAchievementName, nSize);
    file.ReadUChar(m_nLevel);
    file.ReadUShort(m_nTitleID);
    file.ReadUShort(nSize);
    file.ReadString(m_strDetail, nSize);
    file.ReadUShort(nSize);
    file.ReadString(m_strIcon, nSize);
    file.ReadUShort(m_nHandlerID);
    file.ReadUInt(m_nCount);
    file.ReadUInt(m_nScore);
    file.ReadUInt(m_nRewardMoney);

    file.ReadUShort(nSize);
    for (int i = 0; i < nSize; i++)
    {
        AchievementItemReward awardItem;
        awardItem.Read(file);
        m_listRewardItem.push_back(awardItem);
    }
    file.ReadBool(m_bNotify);

    return true;
}

bool AchievementConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nAchievementID);
    file.WriteUShort(m_nClassID);
    file.WriteUShort(m_nSeriesID);
    file.WriteUShort((unsigned short)m_strAchievementName.length());
    file.WriteString(m_strAchievementName);
    file.WriteUChar(m_nLevel);
    file.WriteUShort(m_nTitleID);
    file.WriteUShort((unsigned short)m_strDetail.length());
    file.WriteString(m_strDetail);
    file.WriteUShort((unsigned short)m_strIcon.length());
    file.WriteString(m_strIcon);
    file.WriteUShort(m_nHandlerID);
    file.WriteUInt(m_nCount);
    file.WriteUInt(m_nScore);
    file.WriteUInt(m_nRewardMoney);

    file.WriteUShort((unsigned short)m_listRewardItem.size());

    for (list<AchievementItemReward>::iterator itr = m_listRewardItem.begin(); itr != m_listRewardItem.end(); itr++)
    {
        itr->Write(file);
    }
    file.WriteBool(m_bNotify);

    return true;
}

unsigned short AchievementConfig::GetAchievementID()
{
    return m_nAchievementID;
}

const char * AchievementConfig::GetAchievementName() const 
{
    return m_strAchievementName.c_str(); 
}

unsigned short AchievementConfig::GetRewardTitleID() const 
{
    return m_nTitleID;
}

const char * AchievementConfig::GetDetail() const 
{ 
    return m_strDetail.c_str(); 
}

unsigned int AchievementConfig::GetScore()
{
    return m_nScore;
}

unsigned int AchievementConfig::GetRewardMoney() 
{
    return m_nRewardMoney; 
}

unsigned short AchievementConfig::GetHanlerID()
{
    return m_nHandlerID;
}

unsigned short AchievementConfig::GetClassID()
{
    return m_nClassID;
}

unsigned short AchievementConfig::GetSeriesID()
{
    return m_nSeriesID;
}

unsigned int AchievementConfig::GetRequireValue()
{
    return m_nCount;
}

std::list<AchievementItemReward> & AchievementConfig::GetRewardItemList()
{
    return m_listRewardItem;
}


//end file


