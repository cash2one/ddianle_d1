#include "QuestDef.h"
#include "QuestTrigger.h"
#include "QuestComplete.h"

QuestCompleteLevel::QuestCompleteLevel()
{
    m_Level = 0;
}
QuestCompleteLevel::~QuestCompleteLevel()
{
    m_Level = 0;
}
bool QuestCompleteLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Level);
    return true;
}
bool QuestCompleteLevel::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_Level;
}
QuestCompleteFriendCount::QuestCompleteFriendCount()
{
    m_FriendCount = 0;
}
QuestCompleteFriendCount::~QuestCompleteFriendCount()
{
    m_FriendCount = 0;
}
bool QuestCompleteFriendCount::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_FriendCount);
    return true;
}
bool QuestCompleteFriendCount::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_FriendCount;
}
QuestCompleteVipLevel::QuestCompleteVipLevel()
{
    m_VipLevel = 0;
}
QuestCompleteVipLevel::~QuestCompleteVipLevel()
{
    m_VipLevel = 0;
}

bool QuestCompleteVipLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_VipLevel);
    return true;
}
bool QuestCompleteVipLevel::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_VipLevel;
}
QuestCompleteHasLover::QuestCompleteHasLover()
{
    m_HasLover = false;
}
QuestCompleteHasLover::~QuestCompleteHasLover()
{
    m_HasLover = false;
}
bool QuestCompleteHasLover::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_HasLover);
    return true;
}
bool QuestCompleteHasLover::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (m_HasLover)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
QuestCompleteHasMarried::QuestCompleteHasMarried()
{
    m_HasMarried = false;
}
QuestCompleteHasMarried::~QuestCompleteHasMarried()
{
    m_HasMarried = false;
}
bool QuestCompleteHasMarried::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_HasMarried);
    return true;
}
bool QuestCompleteHasMarried::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (m_HasMarried)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
QuestCompleteIntimacy::QuestCompleteIntimacy()
{
    m_nIntimacy = 0;
}
QuestCompleteIntimacy::~QuestCompleteIntimacy()
{
    m_nIntimacy = 0;
}
bool QuestCompleteIntimacy::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nIntimacy);
    return true;
}
bool QuestCompleteIntimacy::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nIntimacy;
}
QuestCompleteHasDanceGroup::QuestCompleteHasDanceGroup()
{
    m_bHasDanceGroup = false;
}
QuestCompleteHasDanceGroup::~QuestCompleteHasDanceGroup()
{
    m_bHasDanceGroup = false;
}
bool QuestCompleteHasDanceGroup::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bHasDanceGroup);
    return true;
}
bool QuestCompleteHasDanceGroup::CheckCondition(int nValue,int nParam /* = 0 */)
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
QuestCompleteDanceGroupContribution::QuestCompleteDanceGroupContribution()
{
    m_Condition = 0;
}
QuestCompleteDanceGroupContribution::~QuestCompleteDanceGroupContribution()
{
    m_Condition = 0;
}
bool QuestCompleteDanceGroupContribution::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_Condition);
    return true;

}
bool QuestCompleteDanceGroupContribution::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_Condition;
}
QuestCompleteDanceGroupTotalContribution::QuestCompleteDanceGroupTotalContribution()
{
    m_Condition = 0;
}
QuestCompleteDanceGroupTotalContribution::~QuestCompleteDanceGroupTotalContribution()
{
    m_Condition = 0;
}
bool QuestCompleteDanceGroupTotalContribution::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_Condition);
    return true;
}
bool QuestCompleteDanceGroupTotalContribution::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_Condition;
}
QuestCompleteDanceGroupTotalHonor::QuestCompleteDanceGroupTotalHonor()
{
    m_nHonor = 0;
}
QuestCompleteDanceGroupTotalHonor::~QuestCompleteDanceGroupTotalHonor()
{
    m_nHonor = 0;
}
bool QuestCompleteDanceGroupTotalHonor::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_nHonor);
    return true;
}
bool QuestCompleteDanceGroupTotalHonor::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= m_nHonor;
}
QuestCompleteUseCloth::QuestCompleteUseCloth()
{

}

QuestCompleteUseCloth::~QuestCompleteUseCloth()
{
    std::vector<IQuestComplete* > ::iterator it = m_SubComplete.begin();
    for (; it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        delete pComp,pComp = NULL;
    }
    m_SubComplete.clear();
}

bool QuestCompleteUseCloth::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        IQuestComplete* pQuest = IQuestComplete::Create(EQuest_Complete_Item);
        pQuest->Load(pFs);
        m_SubComplete.push_back(pQuest);
    }
    return true;
}
bool QuestCompleteUseCloth::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外部检测
    return false;
}

void QuestCompleteUseCloth::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<IQuestComplete* >::iterator it = m_SubComplete.begin();
    for (;it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        pComp->SetQuestID(id);
    }
}

QuestCompleteUseItem::QuestCompleteUseItem()
{

}

QuestCompleteUseItem::~QuestCompleteUseItem()
{
    std::vector<IQuestComplete* > ::iterator it = m_SubComplete.begin();
    for (; it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        delete pComp,pComp = NULL;
    }
    m_SubComplete.clear();
}


bool QuestCompleteUseItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        IQuestComplete* pQuest = IQuestComplete::Create(EQuest_Complete_Item);
        pQuest->Load(pFs);
        m_SubComplete.push_back(pQuest);
    }
    return true;
}

void QuestCompleteUseItem::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<IQuestComplete* >::iterator it = m_SubComplete.begin();
    for (;it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        pComp->SetQuestID(id);
    }
}

bool QuestCompleteUseItem::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外部检测
    return false;
}

unsigned short QuestCompleteUseItem::GetItemCount(itemtype_t itemID)
{
    std::vector<IQuestComplete* >::iterator it = m_SubComplete.begin();
    for (;it != m_SubComplete.end() ; it++)
    {
        QuestCompleteItem* pQuestItem = (QuestCompleteItem*)(*it);
        if (pQuestItem->m_ItemID == itemID)
        {
            return pQuestItem->m_Count;
        }
    }
    return 0;
}

QuestCompleteObtainItem::QuestCompleteObtainItem()
{

}

QuestCompleteObtainItem::~QuestCompleteObtainItem()
{
    std::vector<IQuestComplete* > ::iterator it = m_SubComplete.begin();
    for (; it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        delete pComp,pComp = NULL;
    }
    m_SubComplete.clear();
}

bool QuestCompleteObtainItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        IQuestComplete* pQuest = IQuestComplete::Create(EQuest_Complete_Item);
        pQuest->Load(pFs);
        m_SubComplete.push_back(pQuest);
    }
    return true;
}
bool QuestCompleteObtainItem::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //物品在外部检测
    return false;
}

void QuestCompleteObtainItem::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<IQuestComplete* >::iterator it = m_SubComplete.begin();
    for (;it != m_SubComplete.end() ; it++)
    {
        IQuestComplete* pComp = *it;
        pComp->SetQuestID(id);
    }
}

void QuestCompleteObtainItem::GetQuestItems(std::vector<CQuestItem>& items)
{
    std::vector<IQuestComplete* >::iterator it = m_SubComplete.begin();
    for (;it != m_SubComplete.end() ; it++)
    {
        QuestCompleteItem* pComp = (QuestCompleteItem*)(*it);
        CQuestItem item;
        item.m_nItemID = pComp->m_ItemID;
        item.m_nItemCount = pComp->m_Count;
        item.m_nValidTime = pComp->m_ValidTime;
        items.push_back(item);
    }
}

QuestCompleteOpenWeb::QuestCompleteOpenWeb()
{

}
QuestCompleteOpenWeb::~QuestCompleteOpenWeb()
{

}
bool QuestCompleteOpenWeb::Load(CFileStream* pFs)
{
    pFs->ReadString2(m_strWeb);
    return true;
}
bool QuestCompleteOpenWeb::CheckCondition(int nValue,int nParam /* = 0 */)
{
    //打开页面，在外部判断
    return false;
}
QuestCompleteUseFunction::QuestCompleteUseFunction()
{
    m_Function = EUF_None;
    m_Times = 0;
    m_Param = 0;
}
QuestCompleteUseFunction::~QuestCompleteUseFunction()
{
    m_Function = EUF_None;
    m_Times = 0;
    m_Param = 0;
}
bool QuestCompleteUseFunction::Load(CFileStream* pFs)
{
    char functionID = 0;
    pFs->ReadChar(functionID);
    m_Function = (ECompleteUseFunction)functionID;
    pFs->ReadUShort(m_Times);
    pFs->ReadInt(m_Param);
    return true;
}
bool QuestCompleteUseFunction::CheckCondition(int nValue,int nParam /* = 0 */)
{
    if (nValue == 0)
    {
        //如果次数为0，则判断是否为当前动作
        return false;
    }
    if (nParam == m_Function)
    {
        return nValue >= m_Times;
    }
    else
        return false;
}
CQuestMusicNew::CQuestMusicNew()
{
    m_MusicID = 0;
    m_Count = 0;
}

CQuestMusicNew::~CQuestMusicNew()
{
    m_MusicID = 0;
    m_Count = 0;

    std::map<EQuest_Complete_Type,IQuestComplete* >::iterator it = m_MusicQuest.begin();
    for (; it != m_MusicQuest.end() ; it++)
    {
        IQuestComplete* pComp = it->second;
        delete pComp , pComp = NULL;
    }
    m_MusicQuest.clear();
}

bool CQuestMusicNew::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_MusicID);
    pFs->ReadUShort(m_Count);

    unsigned short nQuestCount = 0;
    pFs->ReadUShort(nQuestCount);

    for (unsigned short i = 0 ; i < nQuestCount ; i++)
    {
        unsigned short uType = 0;
        pFs->ReadUShort(uType);
        IQuestComplete* pComp = IQuestComplete::Create((EQuest_Complete_Type)uType);
        pComp->Load(pFs);

        m_MusicQuest.insert(make_pair(pComp->GetCompleteType(),pComp));
    }
    return true;
}
QuestCompleteUseMusic::QuestCompleteUseMusic()
{

}

QuestCompleteUseMusic::~QuestCompleteUseMusic()
{
    std::vector<CQuestMusicNew*>::iterator it = m_MusicList.begin();
    for (; it != m_MusicList.end() ; it++)
    {
        CQuestMusicNew* pMusic = *it;
        delete pMusic,pMusic = NULL;
    }
    m_MusicList.clear();
}


bool QuestCompleteUseMusic::Load(CFileStream* pFs)
{
    unsigned short uCount = 0;
    pFs->ReadUShort(uCount);

    for (unsigned short i = 0 ; i < uCount ; i++)
    {
        CQuestMusicNew* pMusic = new CQuestMusicNew;
        pMusic->Load(pFs);
        m_MusicList.push_back(pMusic);
    }

    return true;
}
bool QuestCompleteUseMusic::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    //音乐在外部检测
    return false;
}

void QuestCompleteUseMusic::SetQuestID(unsigned short id)
{
    IQuestTrigger::SetQuestID(id);
    std::vector<CQuestMusicNew*>::iterator it = m_MusicList.begin();
    for (;it!=m_MusicList.end();it++)
    {
        CQuestMusicNew* pMusic = *it;
        pMusic->SetQuestID(id);
    }
}

QuestCompleteItem::QuestCompleteItem()
{
    m_ItemID = 0;
    m_Count = 0;
    m_ValidTime = 0;
}
QuestCompleteItem::~QuestCompleteItem()
{
    m_ItemID = 0;
    m_Count = 0;
    m_ValidTime = 0;
}
bool QuestCompleteItem::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_ItemID);
    pFs->ReadUShort(m_Count);
    pFs->ReadInt(m_ValidTime);

    return true;
}

QuestCompleteOpenComponent::QuestCompleteOpenComponent()
{
    m_ComponentID = 0;
}
QuestCompleteOpenComponent::~QuestCompleteOpenComponent()
{
    m_ComponentID = 0;
}
bool QuestCompleteOpenComponent::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_ComponentID);
    return true;
}
bool QuestCompleteOpenComponent::CheckCondition(int nValue,int nParam )
{
    return nParam == (int)m_ComponentID;
}

QuestCompleteMagiclamp::QuestCompleteMagiclamp()
    : m_nStageID(0)
    , m_nStar(0)
{

}
QuestCompleteMagiclamp::~QuestCompleteMagiclamp(){}

bool QuestCompleteMagiclamp::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_nStageID);
    pFs->ReadUShort(m_nStar);
    pFs->ReadString2(m_strCharpter);
    pFs->ReadString2(m_strProclose);

    return true;
}
bool QuestCompleteMagiclamp::CheckCondition(int nValue,int nParam)
{
    return false;
}

QuestCompleteBuyItem::QuestCompleteBuyItem(){}
QuestCompleteBuyItem::~QuestCompleteBuyItem()
{
    for ( std::vector<QuestCompleteItem*>::iterator itr = m_SubComplete.begin();
        itr != m_SubComplete.end(); ++itr )
    {
        delete *itr, *itr = NULL;
    }
    m_SubComplete.clear();
}

bool QuestCompleteBuyItem::Load(CFileStream* pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++)
    {
        QuestCompleteItem* pQuest = (QuestCompleteItem*)IQuestComplete::Create(EQuest_Complete_Item);
        pQuest->Load(pFs);
        m_SubComplete.push_back(pQuest);
    }
    return true;
}

bool QuestCompleteBuyItem::ValidItem(itemtype_t itemID, int validTime)
{
    for (size_t i = 0; i < m_SubComplete.size(); ++i)
    {
        if (m_SubComplete[i]->m_ItemID == itemID)
        {
            if (-1 == validTime) return true; // 购买永久时限的总是可以满足
            if (-1 == m_SubComplete[i]->m_ValidTime) // 要求购买永久时限，必须购买的是永久时限
                return validTime == -1;
            return m_SubComplete[i]->m_ValidTime <= validTime; // 其他的都是比较时间
        }
    }

    return false;
}


QuestCompleteClotheEffectReplace::QuestCompleteClotheEffectReplace()
    : m_num(0)
{

}

QuestCompleteClotheEffectReplace::~QuestCompleteClotheEffectReplace(){}

bool QuestCompleteClotheEffectReplace::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_num);
    
    return true;
}

bool QuestCompleteClotheEffectReplace::CheckCondition(int nValue,int nParam)
{
    return (int)m_num <= nValue;
}

QuestCompleteClotheEffectLevelUp::QuestCompleteClotheEffectLevelUp()
    : m_num(0)
{

}
QuestCompleteClotheEffectLevelUp::~QuestCompleteClotheEffectLevelUp(){}

bool QuestCompleteClotheEffectLevelUp::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_num);

    return true;
}

bool QuestCompleteClotheEffectLevelUp::CheckCondition(int nValue,int nParam)
{
    return (int)m_num <= nValue;
}

//////////////////////////////////////////////////////////////////////////
// 金券魔法阵
QuestCompleteMoneyMagicArray::QuestCompleteMoneyMagicArray()
    : m_num(0)
{

}
QuestCompleteMoneyMagicArray::~QuestCompleteMoneyMagicArray(){}

bool QuestCompleteMoneyMagicArray::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_num);

    return true;
}

bool QuestCompleteMoneyMagicArray::CheckCondition(int nValue, int nParam)
{
    return (int)m_num <= nValue;
}

//////////////////////////////////////////////////////////////////////////
// M币魔法阵
QuestCompleteCoinMagicArray::QuestCompleteCoinMagicArray()
    : m_num(0)
{

}
QuestCompleteCoinMagicArray::~QuestCompleteCoinMagicArray(){}

bool QuestCompleteCoinMagicArray::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_num);

    return true;
}

bool QuestCompleteCoinMagicArray::CheckCondition(int nValue, int nParam)
{
    return (int)m_num <= nValue;
}


MusicCompleteScene::MusicCompleteScene()
{
    m_SceneID = 0;
}
MusicCompleteScene::~MusicCompleteScene()
{
    m_SceneID = 0;
}

bool MusicCompleteScene::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_SceneID);
    return true;
}
bool MusicCompleteScene::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return m_SceneID == nValue;
}
MusicCompleteDiff::MusicCompleteDiff()
{
    m_Diff = 0;
}
MusicCompleteDiff::~MusicCompleteDiff()
{
    m_Diff = 0;
}
bool MusicCompleteDiff::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Diff);
    return true;
}
bool MusicCompleteDiff::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return m_Diff == nValue;
}
MusicCompleteSceneHuman::MusicCompleteSceneHuman()
{
    m_Count = 0;
}
MusicCompleteSceneHuman::~MusicCompleteSceneHuman()
{
    m_Count = 0;
}
bool MusicCompleteSceneHuman::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Count);
    return true;
}
bool MusicCompleteSceneHuman::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Count;
}
MusicCompleteSceneFriend::MusicCompleteSceneFriend()
{
    m_Count = 0;
}
MusicCompleteSceneFriend::~MusicCompleteSceneFriend()
{
    m_Count = 0;
}
bool MusicCompleteSceneFriend::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Count);
    return true;
}
bool MusicCompleteSceneFriend::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Count;
}
MusicCompleteHasLover::MusicCompleteHasLover()
{
    m_bHasLover = false;
}
MusicCompleteHasLover::~MusicCompleteHasLover()
{
    m_bHasLover = false;
}
bool MusicCompleteHasLover::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bHasLover);
    return true;
}
bool MusicCompleteHasLover::CheckCondition(int nValue,int nParam /* = 0 */ )
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
MusicCompleteDanceGroup::MusicCompleteDanceGroup()
{
    m_Count = 0;
}
MusicCompleteDanceGroup::~MusicCompleteDanceGroup()
{
    m_Count = 0;
}
bool MusicCompleteDanceGroup::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Count);
    return true;
}
bool MusicCompleteDanceGroup::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Count;
}
MusicCompleteRank::MusicCompleteRank()
{
    m_Rank = 0;
}
MusicCompleteRank::~MusicCompleteRank()
{
    m_Rank = 0;
}
bool MusicCompleteRank::Load(CFileStream* pFs)
{
    pFs->ReadChar(m_Rank);
    return true;
}
bool MusicCompleteRank::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue <= m_Rank;
}
MusicCompleteEvalLevel::MusicCompleteEvalLevel()
{
    m_Level = 0;
}
MusicCompleteEvalLevel::~MusicCompleteEvalLevel()
{
    m_Level = 0;
}

bool MusicCompleteEvalLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Level);
    return true;
}
bool MusicCompleteEvalLevel::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Level;
}
MusicCompleteFree::MusicCompleteFree()
{
    m_bFree = false;
}
MusicCompleteFree::~MusicCompleteFree()
{
    m_bFree = false;
}
bool MusicCompleteFree::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bFree);
    return true;
}
bool MusicCompleteFree::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    if (m_bFree)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
MusicCompleteTeam::MusicCompleteTeam()
{
    m_bTeam = false;
}
MusicCompleteTeam::~MusicCompleteTeam()
{
    m_bTeam = false;
}
bool MusicCompleteTeam::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bTeam);
    return true;
}
bool MusicCompleteTeam::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    if (m_bTeam)
    {
        return nValue != 0;
    }
    else
    {
        return nValue == 0;
    }
}
MusicCompleteScore::MusicCompleteScore()
{
    m_nScore = 0;
}
MusicCompleteScore::~MusicCompleteScore()
{
    m_nScore = 0;
}
bool MusicCompleteScore::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_nScore);
    return true;
}
bool MusicCompleteScore::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= (int)m_nScore;
}
MusicCompletePerfect::MusicCompletePerfect()
{
    m_Perfect = 0;
}
MusicCompletePerfect::~MusicCompletePerfect()
{
    m_Perfect = 0;
}
bool MusicCompletePerfect::Load(CFileStream* pFs)
{
    pFs->ReadInt(m_Perfect);
    return true;
}
bool MusicCompletePerfect::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Perfect;
}
MusicCompleteConPerfect::MusicCompleteConPerfect()
{
    m_ConPerfect = 0;
}
MusicCompleteConPerfect::~MusicCompleteConPerfect()
{
    m_ConPerfect = 0;
}
bool MusicCompleteConPerfect::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_ConPerfect);
    return true;
}
bool MusicCompleteConPerfect::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_ConPerfect;
}
MusicCompleteCombo::MusicCompleteCombo()
{
    m_nCombo = 0;
}
MusicCompleteCombo::~MusicCompleteCombo()
{
    m_nCombo = 0;
}
bool MusicCompleteCombo::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_nCombo);
    return true;
}
bool MusicCompleteCombo::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_nCombo;
}
MusicCompleteMissMax::MusicCompleteMissMax()
{
    m_MissMax = 0;
}
MusicCompleteMissMax::~MusicCompleteMissMax()
{
    m_MissMax = 0;
}
bool MusicCompleteMissMax::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_MissMax);
    return true;
}
bool MusicCompleteMissMax::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue <= m_MissMax;
}

CQuestMusicNew* QuestCompleteUseMusic::GetMusicComplete(unsigned short musicID)
{
    std::vector<CQuestMusicNew*>::iterator it = m_MusicList.begin();
    for (;it != m_MusicList.end() ; it++)
    {
        CQuestMusicNew* pMusic = *it;
        if (pMusic->m_MusicID == musicID)
        {
            return pMusic;
        }
    }
    return NULL;
}

unsigned short QuestCompleteUseMusic::GetMusicCount(unsigned short musicID)
{
    std::vector<CQuestMusicNew*>::iterator it = m_MusicList.begin();
    for (;it != m_MusicList.end() ; it++)
    {
        CQuestMusicNew* pMusic = *it;
        if (pMusic->m_MusicID == musicID)
        {
            return pMusic->m_Count;
        }
    }
    return 0;
}

void CQuestMusicNew::SetQuestID(unsigned short id)
{
    std::map<EQuest_Complete_Type,IQuestComplete* >::iterator it = m_MusicQuest.begin();
    for (; it != m_MusicQuest.end() ; it++)
    {
        IQuestComplete* pComp = it->second;
        pComp->SetQuestID(id);
    }
}

MusicCompleteModel::MusicCompleteModel()
{

}

MusicCompleteModel::~MusicCompleteModel()
{

}

bool MusicCompleteModel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Model);
    return true;
}

bool MusicCompleteModel::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue == (int)m_Model;
}

QuestCompleteMatchMusicEnd::QuestCompleteMatchMusicEnd()
{
    m_Times = 0;
}

QuestCompleteMatchMusicEnd::~QuestCompleteMatchMusicEnd()
{

}

bool QuestCompleteMatchMusicEnd::Load(CFileStream* pFs)
{
    pFs->ReadUInt(m_Times);
    return true;
}

bool QuestCompleteMatchMusicEnd::CheckCondition(int nValue,int nParam /* = 0 */)
{
    return nValue >= (int)m_Times;
}

