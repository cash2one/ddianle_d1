#include "QuestTrigger.h"
#include "QuestReward.h"
#include "../datastructure/DataStruct_Base.h"

QuestRewardExp::QuestRewardExp()
{
    m_Exp = 0;
}

QuestRewardExp::~QuestRewardExp()
{
    m_Exp = 0;
}

bool QuestRewardExp::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Exp);
    return true;
}

EQuest_Reward_Type QuestRewardExp::GetRewardType()
{
    return EQuest_Reward_Exp;
}

unsigned int QuestRewardExp::GetExp()
{
    return m_Exp;
}

QuestRewardMoney::QuestRewardMoney()
{
    m_Money = 0;
}
QuestRewardMoney::~QuestRewardMoney()
{
    m_Money = 0;
}

bool QuestRewardMoney::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Money);
    return true;
}

EQuest_Reward_Type QuestRewardMoney::GetRewardType() 
{
    return EQuest_Reward_Money ;
}

unsigned int QuestRewardMoney::GetMoney()
{
    return m_Money;
}

QuestRewardHonor::QuestRewardHonor()
{
    m_Honor = 0;
}
QuestRewardHonor::~QuestRewardHonor()
{
    m_Honor = 0;
}
bool QuestRewardHonor::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Honor);
    return true;
}

EQuest_Reward_Type QuestRewardHonor::GetRewardType() 
{
    return EQuest_Reward_Honor ;
}

unsigned int QuestRewardHonor::GetHonor()
{
    return m_Honor;
}

QuestRewardContribution::QuestRewardContribution()
{
    m_Contribution = 0;
}

QuestRewardContribution::~QuestRewardContribution()
{
    m_Contribution = 0;
}

bool QuestRewardContribution::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Contribution);
    return true;
}

EQuest_Reward_Type QuestRewardContribution::GetRewardType() 
{
    return EQuest_Reward_Contribution ;
}

unsigned int QuestRewardContribution::GetContribution()
{
    return m_Contribution;
}

QuestRewardVipValue::QuestRewardVipValue()
{
    m_VipValue = 0;
}
QuestRewardVipValue::~QuestRewardVipValue()
{
    m_VipValue = 0;
}
bool QuestRewardVipValue::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_VipValue);
    return true;
}

EQuest_Reward_Type QuestRewardVipValue::GetRewardType() 
{
    return EQuest_Reward_VipValue ;
}

unsigned int QuestRewardVipValue::GetValue()
{
    return m_VipValue;
}

QuestRewardIntimacy::QuestRewardIntimacy()
{
    m_Intimacy = 0;
}

QuestRewardIntimacy::~QuestRewardIntimacy()
{
    m_Intimacy = 0;
}

bool QuestRewardIntimacy::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Intimacy);
    return true;
}

EQuest_Reward_Type QuestRewardIntimacy::GetRewardType() 
{ 
    return EQuest_Reward_Intimacy ;
}

unsigned int QuestRewardIntimacy::GetIntimacy()
{
    return m_Intimacy;
}

QuestRewardItem::QuestRewardItem()
{

}
QuestRewardItem::~QuestRewardItem()
{
    m_MaleItem.clear();
    m_FemaleItem.clear();
}
bool QuestRewardItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        CQuestItem item;
        pFs->ReadUInt(item.m_nItemID);
        pFs->ReadUShort(item.m_nItemCount);
        pFs->ReadInt(item.m_nValidTime);

        m_MaleItem.push_back(item);
    }
    nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        CQuestItem item;
        pFs->ReadUInt(item.m_nItemID);
        pFs->ReadUShort(item.m_nItemCount);
        pFs->ReadInt(item.m_nValidTime);

        m_FemaleItem.push_back(item);
    }

    return true;
}

EQuest_Reward_Type QuestRewardItem::GetRewardType()
{ 
    return EQuest_Reward_Item ;
}

bool QuestRewardItem::GetRewardItem(int nSex,std::list<CItem>& items)
{
    if (nSex == ESexType_Male)
    {
        std::vector<CQuestItem>::iterator it = m_MaleItem.begin();
        for (;it != m_MaleItem.end() ; it++)
        {
            CItem item;
            item.m_nItemType = it->m_nItemID;
            item.m_nItemCount = it->m_nItemCount;
            item.m_nValidTime = it->m_nValidTime;
            items.push_back(item);
        }
        return true;
    }
    else if (nSex == ESexType_Female)
    {
        std::vector<CQuestItem>::iterator it = m_FemaleItem.begin();
        for (;it != m_FemaleItem.end() ; it++)
        {
            CItem item;
            item.m_nItemType = it->m_nItemID;
            item.m_nItemCount = it->m_nItemCount;
            item.m_nValidTime = it->m_nValidTime;
            items.push_back(item);
        }
        return true;
    }
    return false;
}

QuestRewardBingMCoin::QuestRewardBingMCoin()
{
    m_BindMCoin = 0;
}

QuestRewardBingMCoin::~QuestRewardBingMCoin()
{

}

bool QuestRewardBingMCoin::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_BindMCoin);
    return true;
}

EQuest_Reward_Type QuestRewardBingMCoin::GetRewardType()
{ 
    return EQuest_Reward_BindMCoin ;
}

int QuestRewardBingMCoin::GetBindMCoin() 
{ 
    return m_BindMCoin ;
}

QuestVipRewardExp::QuestVipRewardExp()
{
    m_nExp = 0;
}

QuestVipRewardExp::~QuestVipRewardExp()
{

}

bool QuestVipRewardExp::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nExp);
    return true;
}

EQuest_Reward_Type QuestVipRewardExp::GetRewardType()
{ 
    return EQuest_VipReward_Exp ;
}

int QuestVipRewardExp::GetExp() 
{ 
    return m_nExp;
}

QuestVipRewardHonor::QuestVipRewardHonor()
{
    m_nHonor = 0;
}

QuestVipRewardHonor::~QuestVipRewardHonor()
{

}

bool QuestVipRewardHonor::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nHonor);
    return true;
}

EQuest_Reward_Type QuestVipRewardHonor::GetRewardType()
{ 
    return EQuest_VipReward_Honor ;
}

int QuestVipRewardHonor::GetHonor() 
{ 
    return m_nHonor ; 
}

QuestVipRewardIntimacy::QuestVipRewardIntimacy()
{
    m_nIntimacy = 0;
}

QuestVipRewardIntimacy::~QuestVipRewardIntimacy()
{

}

bool QuestVipRewardIntimacy::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nIntimacy);
    return true;
}

EQuest_Reward_Type QuestVipRewardIntimacy::GetRewardType() 
{
    return EQuest_VipReward_Intimacy ;
}

int QuestVipRewardIntimacy::GetIntimacy() 
{ 
    return m_nIntimacy ;
}

QuestVipRewardMoney::QuestVipRewardMoney()
{
    m_nMoney = 0;
}

QuestVipRewardMoney::~QuestVipRewardMoney()
{

}

bool QuestVipRewardMoney::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nMoney);
    return true;
}

EQuest_Reward_Type QuestVipRewardMoney::GetRewardType() 
{
    return EQuest_VipReward_Money ; 
}

int QuestVipRewardMoney::GetMoney() 
{ 
    return m_nMoney ;
}

QuestVipRewardVipValue::QuestVipRewardVipValue()
{
    m_nVipValue = 0;
}

QuestVipRewardVipValue::~QuestVipRewardVipValue()
{

}

bool QuestVipRewardVipValue::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nVipValue);
    return true;
}

EQuest_Reward_Type QuestVipRewardVipValue::GetRewardType() 
{ 
    return EQuest_VipReward_VipValue ; 
}

int QuestVipRewardVipValue::GetVipValue() 
{ 
    return m_nVipValue ; 
}

QuestVipRewardContribution::QuestVipRewardContribution()
{
    m_nContribution = 0;
}

QuestVipRewardContribution::~QuestVipRewardContribution()
{

}

bool QuestVipRewardContribution::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nContribution);
    return true;
}

EQuest_Reward_Type QuestVipRewardContribution::GetRewardType()
{ 
    return EQuest_VipReward_Contribution ; 
}

int QuestVipRewardContribution::GetContribution() 
{ 
    return m_nContribution ; 
}

QuestVipRewardBindMCoin::QuestVipRewardBindMCoin()
{
    m_nBindMCoin = 0;
}

QuestVipRewardBindMCoin::~QuestVipRewardBindMCoin()
{

}

bool QuestVipRewardBindMCoin::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nBindMCoin);
    return true;
}

EQuest_Reward_Type QuestVipRewardBindMCoin::GetRewardType() 
{ 
    return EQuest_VipReward_BindMCoin ; 
}

int QuestVipRewardBindMCoin::GetBindMCoin() 
{ 
    return m_nBindMCoin ; 
}


QuestVipRewardItem::QuestVipRewardItem()
{
    m_MaleItem.clear();
    m_FemaleItem.clear();
}

QuestVipRewardItem::~QuestVipRewardItem()
{
    m_MaleItem.clear();
    m_FemaleItem.clear();
}

bool QuestVipRewardItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        CQuestItem item;
        pFs->ReadUInt(item.m_nItemID);
        pFs->ReadUShort(item.m_nItemCount);
        pFs->ReadInt(item.m_nValidTime);

        m_MaleItem.push_back(item);
    }
    nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        CQuestItem item;
        pFs->ReadUInt(item.m_nItemID);
        pFs->ReadUShort(item.m_nItemCount);
        pFs->ReadInt(item.m_nValidTime);

        m_FemaleItem.push_back(item);
    }

    return true;
}

EQuest_Reward_Type QuestVipRewardItem::GetRewardType() 
{ 
    return EQuest_VipReward_Item ; 
}

bool QuestVipRewardItem::GetRewardItem(int nSex,std::list<CItem>& items)
{
    if (nSex == ESexType_Male)
    {
        std::vector<CQuestItem>::iterator it = m_MaleItem.begin();
        for (;it != m_MaleItem.end() ; it++)
        {
            CItem item;
            item.m_nItemType = it->m_nItemID;
            item.m_nItemCount = it->m_nItemCount;
            item.m_nValidTime = it->m_nValidTime;
            items.push_back(item);
        }
        return true;
    }
    else if (nSex == ESexType_Female)
    {
        std::vector<CQuestItem>::iterator it = m_FemaleItem.begin();
        for (;it != m_FemaleItem.end() ; it++)
        {
            CItem item;
            item.m_nItemType = it->m_nItemID;
            item.m_nItemCount = it->m_nItemCount;
            item.m_nValidTime = it->m_nValidTime;
            items.push_back(item);
        }
        return true;
    }
    return false;
}


