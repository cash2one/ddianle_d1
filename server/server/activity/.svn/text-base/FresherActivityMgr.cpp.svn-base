#include "FresherActivityMgr.h"

#include "../util/CommonFunc.h"

CFresherActivityManager::CFresherActivityManager()
:m_bIsOpen(false)
, m_nRecvAgainMoney(0)
, m_nDurationDays(0)
, m_nCumulationDays(0)
{
}
CFresherActivityManager::~CFresherActivityManager()
{
}


CFresherActivityManager & CFresherActivityManager::Instance()
{
    static CFresherActivityManager s_instance;
    return s_instance;
}



void CFresherActivityManager::SetActivity(const char *pszName, const char *pszContent,
    const char *pszMailTitle, const char *pszMailContent,
    int nRecvAgainMoney, int nDurationDays, int nCumulationDays)
{
    m_strActivityTitle = pszName;
    m_strActivityContent = pszContent;
    m_strMailTitle = pszMailTitle;
    m_strMailContent = pszMailContent;

    m_nRecvAgainMoney = nRecvAgainMoney;
    m_nDurationDays = nDurationDays;
    m_nCumulationDays = nCumulationDays;

    m_bIsOpen = true;
}

void CFresherActivityManager::AddFresherReward(const CFresherReward & malereward, const CFresherReward & femalereward)
{
    m_mapMaleReward[malereward.m_nDay] = malereward;
    m_mapFemaleReward[femalereward.m_nDay] = femalereward;
}

void CFresherActivityManager::AddCumulationReward(const char *maleItem, const char *femaleItem, int nMoney, int nBindCoin, const char *vipMaleItem, const char *vipFemaleItem, int nVipMoney, int nVipBindCoin)
{
    m_maleCumulationReward.m_nDay = 0;
    m_maleCumulationReward.m_nMoney = nMoney;
    m_maleCumulationReward.m_nBindCoin = nBindCoin;
    m_maleCumulationReward.m_nVipMoney = nVipMoney;
    m_maleCumulationReward.m_nVipBindCoin = nVipBindCoin;
    TransferItemList(maleItem, m_maleCumulationReward.m_listItem);
    TransferItemList(vipMaleItem, m_maleCumulationReward.m_listVipItem);


    m_femaleCumulationReward.m_nDay = 0;
    m_femaleCumulationReward.m_nMoney = nMoney;
    m_femaleCumulationReward.m_nBindCoin = nBindCoin;
    m_femaleCumulationReward.m_nVipMoney = nVipMoney;
    m_femaleCumulationReward.m_nVipBindCoin = nVipBindCoin;
    TransferItemList(femaleItem, m_femaleCumulationReward.m_listItem);
    TransferItemList(vipFemaleItem, m_femaleCumulationReward.m_listVipItem);
}


const CFresherReward *CFresherActivityManager::GetFresherReward(int nDay, unsigned char m_nSex) const
{
    if (m_nSex == ESexType_Male)
    {
        std::map< int, CFresherReward >::const_iterator it = m_mapMaleReward.find(nDay);
        if (it == m_mapMaleReward.end())
            return NULL;
        return &it->second;
    }

    std::map< int, CFresherReward >::const_iterator it = m_mapFemaleReward.find(nDay);
    if (it == m_mapFemaleReward.end())
        return NULL;
    return &it->second;
}

const CFresherReward &CFresherActivityManager::GetCumulationReward(unsigned char m_nSex) const
{
    if (m_nSex == ESexType_Male)
        return m_maleCumulationReward;

    return m_femaleCumulationReward;
}

bool CFresherActivityManager::IsOpen()
{
    return m_bIsOpen;
}

