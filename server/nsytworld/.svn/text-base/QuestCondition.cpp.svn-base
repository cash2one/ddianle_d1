#include "QuestDef.h"
#include "QuestTrigger.h"
#include "QuestCondition.h"

QuestConditionLevelMin::QuestConditionLevelMin()
{
    m_nMinLevel = 0;
}
QuestConditionLevelMin::~QuestConditionLevelMin()
{
    m_nMinLevel = 0;
}

bool QuestConditionLevelMin::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_nMinLevel);
    return true;
}

EQuest_Condition_Type QuestConditionLevelMin::GetConditionType() 
{
    return EQuest_Condition_LevelMin;
}

bool QuestConditionLevelMin::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nMinLevel;
}

QuestConditionVipLevel::QuestConditionVipLevel()
{
    m_nVipLevel = 0;
}
QuestConditionVipLevel::~QuestConditionVipLevel()
{
    m_nVipLevel = 0;
}
bool QuestConditionVipLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_nVipLevel);
    return true;
}

EQuest_Condition_Type QuestConditionVipLevel::GetConditionType() 
{
    return EQuest_Condition_VipLevel;
}

bool QuestConditionVipLevel::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nVipLevel;
}
QuestConditionSex::QuestConditionSex()
{
    m_nSex = ESexType_No;
}
QuestConditionSex::~QuestConditionSex()
{
    m_nSex = ESexType_No;
}
bool QuestConditionSex::Load(CFileStream* pFs)
{
    char cBuff = 0;
    pFs->ReadChar(cBuff);
    m_nSex = (ESexType)cBuff;
    return true;
}

bool QuestConditionSex::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue == (int)m_nSex;
}

EQuest_Condition_Type QuestConditionSex::GetConditionType()
{
    return EQuest_Condition_Sex;
}

QuestConditionXingZuo::QuestConditionXingZuo()
{

}
QuestConditionXingZuo::~QuestConditionXingZuo()
{

}
bool QuestConditionXingZuo::Load(CFileStream* pFs)
{
    char cBuff = 0;
    pFs->ReadChar(cBuff);
    m_nXingZuo = (eConstellation)cBuff;
    return true;
}

EQuest_Condition_Type QuestConditionXingZuo::GetConditionType()
{
    return EQuest_Condition_XingZuo;
}

bool QuestConditionXingZuo::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue == (int)m_nXingZuo;
}

QuestConditionHasLover::QuestConditionHasLover()
{
    m_bHasLover = false;
}
QuestConditionHasLover::~QuestConditionHasLover()
{
    m_bHasLover = false;
}

EQuest_Condition_Type QuestConditionHasLover::GetConditionType() 
{
    return EQuest_Condition_HasLover;
}

bool QuestConditionHasLover::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bHasLover);
    return true;
}
bool QuestConditionHasLover::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (m_bHasLover)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
QuestConditionHasMarried::QuestConditionHasMarried()
{
    m_bHasMarried = false;
}
QuestConditionHasMarried::~QuestConditionHasMarried()
{
    m_bHasMarried = false;
}
bool QuestConditionHasMarried::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bHasMarried);
    return true;
}

EQuest_Condition_Type QuestConditionHasMarried::GetConditionType() 
{
    return EQuest_Condition_HasMarried;
}

bool QuestConditionHasMarried::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (m_bHasMarried)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
QuestConditionIntimacy::QuestConditionIntimacy()
{
    m_nIntimacy = 0;
}
QuestConditionIntimacy::~QuestConditionIntimacy()
{
    m_nIntimacy = 0;
}
bool QuestConditionIntimacy::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nIntimacy);
    return true;
}
bool QuestConditionIntimacy::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nIntimacy;
}

EQuest_Condition_Type QuestConditionIntimacy::GetConditionType() 
{
    return EQuest_Condition_Intimacy;
}

QuestConditionPreQuest::QuestConditionPreQuest()
{
    m_nPreQuestID = 0;
}
QuestConditionPreQuest::~QuestConditionPreQuest()
{
    m_nPreQuestID = 0;
}
bool QuestConditionPreQuest::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_nPreQuestID);
    return true;
}
bool QuestConditionPreQuest::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue == nParam;
}

EQuest_Condition_Type QuestConditionPreQuest::GetConditionType() 
{
    return EQuest_Condition_PreQuest;
}

QuestConditionObtainItem::QuestConditionObtainItem()
{
}
QuestConditionObtainItem::~QuestConditionObtainItem()
{
    std::vector<IQuestCondition* >::iterator it = m_SubCondition.begin();
    for (; it != m_SubCondition.end() ; it++)
    {
        IQuestCondition* pCond = *it;
        delete pCond,pCond = NULL;
    }
    m_SubCondition.clear();
}

bool QuestConditionObtainItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);

    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        IQuestCondition* pCond = IQuestCondition::Create(EQuest_Condition_Item);
        pCond->Load(pFs);

        m_SubCondition.push_back(pCond);
    }
    return true;
}

EQuest_Condition_Type QuestConditionObtainItem::GetConditionType() 
{
    return EQuest_Condition_ObtainItem;
}

bool QuestConditionObtainItem::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外面检测
    return false;
}

void QuestConditionObtainItem::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<IQuestCondition*>::iterator it = m_SubCondition.begin();
    for (;it != m_SubCondition.end() ; it++)
    {
        IQuestCondition* pCond = *it;
        pCond->SetQuestID(id);
    }
}

QuestConditionUseItem::QuestConditionUseItem()
{

}

QuestConditionUseItem::~QuestConditionUseItem()
{
    std::vector<IQuestCondition* >::iterator it = m_SubCondition.begin();
    for (; it != m_SubCondition.end() ; it++)
    {
        IQuestCondition* pCond = *it;
        delete pCond,pCond = NULL;
    }
    m_SubCondition.clear();
}

bool QuestConditionUseItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);

    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        IQuestCondition* pCond = IQuestCondition::Create(EQuest_Condition_Item);
        pCond->Load(pFs);

        m_SubCondition.push_back(pCond);
    }
    return true;
}

EQuest_Condition_Type QuestConditionUseItem::GetConditionType() 
{
    return EQuest_Condition_UseItem;
}

bool QuestConditionUseItem::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外面检测
    return false;
}

void QuestConditionUseItem::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<IQuestCondition*>::iterator it = m_SubCondition.begin();
    for (;it != m_SubCondition.end() ; it++)
    {
        IQuestCondition* pCond = *it;
        pCond->SetQuestID(id);
    }
}

QuestConditionHasDanceGroup::QuestConditionHasDanceGroup()
{
    m_bHasDanceGroup = false;
}
QuestConditionHasDanceGroup::~QuestConditionHasDanceGroup()
{
    m_bHasDanceGroup = false;
}
bool QuestConditionHasDanceGroup::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bHasDanceGroup);
    return true;
}

EQuest_Condition_Type QuestConditionHasDanceGroup::GetConditionType() 
{
    return EQuest_Condition_HasDanceGroup;
}

bool QuestConditionHasDanceGroup::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (m_bHasDanceGroup)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
QuestConditionDanceGroupContribution::QuestConditionDanceGroupContribution()
{
    m_nCondition = 0;
}
QuestConditionDanceGroupContribution::~QuestConditionDanceGroupContribution()
{
    m_nCondition = 0;
}
bool QuestConditionDanceGroupContribution::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nCondition);
    return true;
}

EQuest_Condition_Type QuestConditionDanceGroupContribution::GetConditionType() 
{
    return EQuest_Condition_DanceGroupContribution;
}

bool QuestConditionDanceGroupContribution::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nCondition;
}
QuestConditionDanceGroupLevel::QuestConditionDanceGroupLevel()
{
    m_nLevel = 0;
}
QuestConditionDanceGroupLevel::~QuestConditionDanceGroupLevel()
{
    m_nLevel = 0;
}

bool QuestConditionDanceGroupLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_nLevel);
    return true;
}

EQuest_Condition_Type QuestConditionDanceGroupLevel::GetConditionType() 
{
    return EQuest_Condition_DanceGroupLevel;
}

bool QuestConditionDanceGroupLevel::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nLevel;
}
QuestConditionItem::QuestConditionItem()
{
    m_Item.m_nItemCount = 0;
    m_Item.m_nItemID = 0;
    m_Item.m_nValidTime = 0;
}
QuestConditionItem::~QuestConditionItem()
{
    m_Item.m_nItemCount = 0;
    m_Item.m_nItemID = 0;
    m_Item.m_nValidTime = 0;
}
bool QuestConditionItem::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Item.m_nItemID);
    pFs->ReadUShort(m_Item.m_nItemCount);
    pFs->ReadInt(m_Item.m_nValidTime);
    return true;
}
bool QuestConditionItem::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外面检测
    return false;
}

EQuest_Condition_Type QuestConditionItem::GetConditionType() 
{
    return EQuest_Condition_Item;
}

QuestConditionChargeVersion::QuestConditionChargeVersion()
{
    m_Value = true;
}

QuestConditionChargeVersion::~QuestConditionChargeVersion()
{
    m_Value = false;
}

bool QuestConditionChargeVersion::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_Value);
    return true;
}

EQuest_Condition_Type QuestConditionChargeVersion::GetConditionType()
{
    return EQuest_Condition_ChargeVersion; 
}

bool QuestConditionChargeVersion::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return m_Value && nValue == Version_Charge;
}

