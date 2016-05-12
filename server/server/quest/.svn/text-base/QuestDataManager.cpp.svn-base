#include "QuestDataManager.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../nsytworld/QuestDoc.h"
#include "../libServerFrame/QueryManager.h"
#include "../util/CommonFunc.h"

CFresherQuestDoc::CFresherQuestDoc()
    : m_nDay(0)
    , m_nQuestID(0)
    , m_nTargetType(-1)
    , m_nTarget(-1)
{

}

CFresherQuestDoc::CFresherQuestDoc(int nDay, int nQuestID, int nTargetType, int nTarget, const char *pszDesc, 
    const char *pszMaleItem, const char *pszFemaleItem, int nMoney, int nBingMCoin, 
    const char * pszVipMaleItem, const char * pszVipFemaleItem, int nVipMoney, int nVipBindMCoin)
    : m_nDay(nDay)
    , m_nQuestID(nQuestID)
    , m_nTargetType(nTargetType)
    , m_nTarget(nTarget)
    , m_strQuestDesc(pszDesc)
{
    std::list< CItem > rewardItemList;

    TransferItemList(pszMaleItem, rewardItemList);
    m_MaleReward.m_listItems = rewardItemList;
    m_MaleReward.m_nRewardType = ERewardType_Normal;
    m_MaleReward.m_nMoney = nMoney;
    m_MaleReward.m_nBindMCoin = nBingMCoin;
    rewardItemList.clear();

    TransferItemList(pszFemaleItem, rewardItemList);
    m_FemaleReward.m_listItems = rewardItemList;
    m_FemaleReward.m_nRewardType = ERewardType_Normal;
    m_FemaleReward.m_nMoney = nMoney;
    m_FemaleReward.m_nBindMCoin = nBingMCoin;
    rewardItemList.clear();

    TransferItemList(pszVipMaleItem, rewardItemList);
    m_VipMaleReward.m_listItems = rewardItemList;
    m_VipMaleReward.m_nRewardType = ERewardType_VIP;
    m_VipMaleReward.m_nMoney = nVipMoney;
    m_VipMaleReward.m_nBindMCoin = nVipBindMCoin;
    rewardItemList.clear();

    TransferItemList(pszVipFemaleItem, rewardItemList);
    m_VipFemaleReward.m_listItems = rewardItemList;
    m_VipFemaleReward.m_nRewardType = ERewardType_VIP;
    m_VipFemaleReward.m_nMoney = nVipMoney;
    m_VipFemaleReward.m_nBindMCoin = nVipBindMCoin;
    rewardItemList.clear();
}

CFresherQuestDoc::~CFresherQuestDoc()
{
}

void CFresherQuestDoc::doEncode(CParamPool &ioBuff, unsigned char nSex)
{
    ioBuff.AddUChar((unsigned char)m_nDay);
    ioBuff.AddUShort((unsigned short)m_nQuestID);
    ioBuff.AddUChar((unsigned char)m_nTargetType);
    ioBuff.AddInt(m_nTarget);
    ioBuff.AddStringW(m_strQuestDesc.c_str());
    if (nSex == ESexType_Male)
    {
        m_MaleReward.doEncode(ioBuff);
        m_VipMaleReward.doEncode(ioBuff);
    }
    else
    {
        m_FemaleReward.doEncode(ioBuff);
        m_VipFemaleReward.doEncode(ioBuff);
    }
}

CFresherQuestDataManager::CFresherQuestDataManager()
    : m_nQuestDuration(0)
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}

CFresherQuestDataManager::~CFresherQuestDataManager()
{
    for (std::map<int, CFresherQuestDoc* >::iterator i = m_mapFresherQuest.begin();
        i != m_mapFresherQuest.end(); ++i)
    {
        if (i->second != NULL)
        {
            delete i->second;
            i->second = NULL;
        }
    }
}

CFresherQuestDataManager & CFresherQuestDataManager::Instance()
{
    static CFresherQuestDataManager s_instance;
    return s_instance;
}

void CFresherQuestDataManager::SetFresherQuest(
    const char *pszName, const char *pszDesc,
    const char *pszMailTitle, const char *pszMailContent,
    int nDuration, const char *pszStartTime, const char *pszEndTime,
    const char *pszMailVipTitle, const char *pszMailVipContent)
{
    m_strQuestName = pszName;
    m_strQuestDescription = pszDesc;
    m_strMailTitle = pszMailTitle;
    m_strMailContent = pszMailContent;
    m_nQuestDuration = nDuration;
    m_nStartTime = (unsigned int)SetTime(pszStartTime);
    m_nEndTime = (unsigned int)SetTime(pszEndTime);
    m_strVipRewardTitle = pszMailVipTitle;
    m_strVipRewardContent = pszMailVipContent;
}

const static int kMinFresherQuestID = 10000;

int CFresherQuestDataManager::MakeFresherQuestID(int nDay)
{
    return (kMinFresherQuestID + nDay);
}

int CFresherQuestDataManager::ToDayIndex(int nQuestID)
{
    return (nQuestID - kMinFresherQuestID);
}

void CFresherQuestDataManager::AddFresherQuest(int nDay, int nQuestTargetType, int nQuestTarget, const char *pszDesc, 
    const char *pszMaleItem, const char *pszFemaleItem, int nMoney, int nBindMCoin, 
    const char * pszVipMaleItem , const char * pszVipFemaleItem, int nVipMoney, int nVipBindMCoin)
{
    int nQuestID = MakeFresherQuestID(nDay);
    CFresherQuestDoc *pDoc = new CFresherQuestDoc(
        nDay, nQuestID, nQuestTargetType, nQuestTarget, pszDesc,
        pszMaleItem, pszFemaleItem, nMoney, nBindMCoin,
        pszVipMaleItem, pszVipFemaleItem, nVipMoney, nVipBindMCoin);

    m_mapFresherQuest.insert(std::make_pair(nDay, pDoc));

    CQuestDoc *pQuest = new CQuestDoc;

    // base info
    pQuest->m_CQusetBaseInfo.m_nQuestID = (unsigned short)nQuestID;
    pQuest->m_CQusetBaseInfo.m_nType = Quest_Fresher;
    pQuest->m_CQusetBaseInfo.m_strQustName = m_strQuestName;

    pQuest->m_strDescribe = pszDesc;
    pQuest->m_strFinalWords = "a";
    pQuest->m_strPrompts = "a";

    // reward
    pQuest->m_nRewrad.m_nRewardExp = 0;
    pQuest->m_nRewrad.m_nRewardMoney = nMoney;
    std::list<CItem>::iterator it = pDoc->m_MaleReward.m_listItems.begin();
    for (; it != pDoc->m_MaleReward.m_listItems.end(); ++it)
    {
        CQuestItem *pItem = new CQuestItem;
        pItem->m_nItemID = it->m_nItemType;
        pItem->m_nItemCount = it->m_nItemCount;
        pItem->m_nValidTime = it->m_nValidTime;
        pQuest->m_nRewrad.m_mapRewardItemMale.insert(std::make_pair(pItem->m_nItemID, pItem));
    }
    it = pDoc->m_FemaleReward.m_listItems.begin();
    for (; it != pDoc->m_FemaleReward.m_listItems.end(); ++it)
    {
        CQuestItem *pItem = new CQuestItem;
        pItem->m_nItemID = it->m_nItemType;
        pItem->m_nItemCount = it->m_nItemCount;
        pItem->m_nValidTime = it->m_nValidTime;
        pQuest->m_nRewrad.m_mapRewardItemFemale.insert(std::make_pair(pItem->m_nItemID, pItem));
    }
    it = pDoc->m_VipMaleReward.m_listItems.begin();
    for (; it != pDoc->m_VipMaleReward.m_listItems.end(); ++it)
    {
        CQuestItem *pItem = new CQuestItem;
        pItem->m_nItemID = it->m_nItemType;
        pItem->m_nItemCount = it->m_nItemCount;
        pItem->m_nValidTime = it->m_nValidTime;
        pQuest->m_nRewrad.m_mapRewardVipItemMale.insert(std::make_pair(pItem->m_nItemID, pItem));
    }
    it = pDoc->m_VipFemaleReward.m_listItems.begin();
    for (; it != pDoc->m_VipFemaleReward.m_listItems.end(); ++it)
    {
        CQuestItem *pItem = new CQuestItem;
        pItem->m_nItemID = it->m_nItemType;
        pItem->m_nItemCount = it->m_nItemCount;
        pItem->m_nValidTime = it->m_nValidTime;
        pQuest->m_nRewrad.m_mapRewardVipItemFemale.insert(std::make_pair(pItem->m_nItemID, pItem));
    }

    // target
    switch (nQuestTargetType)
    {
    case TARGET_LEVEL:
        pQuest->m_attrTarget.m_nTargetLevel = (unsigned short)nQuestTarget;
        break;
    case TARGET_FRIENDCOUNT:
        pQuest->m_attrTarget.m_nTargetFriendNum = (unsigned short)nQuestTarget;
        break;
    case TARGET_MUSICGRADE:
        pQuest->m_attrTarget.m_nTargetEx1 = nQuestTarget;
        // 添加一个伪音乐目标，实际数据存在attrTarget中
        pQuest->m_mapMusicTarget.insert(std::make_pair((unsigned short)0, new CQuestMusic));
        break;
    case TARGET_BUYITEMCOUNT:
        pQuest->m_attrTarget.m_nPayCount = nQuestTarget;
        break;
    case TARGET_JOINDANCEGROUP:
        pQuest->m_attrTarget.m_nTargetEx2 = nQuestTarget;
        break;
    case TARGET_HASLOVER:
        pQuest->m_attrTarget.m_nTargetEx3 = nQuestTarget;
        break;
    default:
        break;
    }

    ConfigManager::Instance().GetFresherQuestManager().Add(pQuest);
}

void CFresherQuestDataManager::LoadFresherQuestBinaryData()
{
    extern std::string g_sPath;
    std::string file = g_sPath;
    file += FresherQuestFileName;
    ConfigManager::Instance().GetFresherQuestManager().SaveQuest(file.c_str());
    ConfigManager::Instance().GetFresherQuestManager().LoadQuestBinaryData(file.c_str());
}

CFresherQuestDoc *CFresherQuestDataManager::GetFresherQuest(int nDay)
{
    if (m_mapFresherQuest.find(nDay) == m_mapFresherQuest.end())
        return NULL;

    return m_mapFresherQuest.find(nDay)->second;
}

unsigned short CFresherQuestDataManager::GetFresherQuestID(int nDay)
{
    CFresherQuestDoc *pDoc = GetFresherQuest(nDay);
    if (pDoc == NULL)
        return 0;

    return (unsigned short)pDoc->m_nQuestID;
}

bool CFresherQuestDataManager::IsOpen() const
{
    unsigned int nNow = (unsigned int)time(NULL);
    return (nNow >= m_nStartTime && nNow <= m_nEndTime);
}

void CFresherQuestDataManager::doEncode(CParamPool &ioBuff, unsigned char nSex)
{
    ioBuff.AddUShort((unsigned short)m_mapFresherQuest.size());
    for (std::map<int, CFresherQuestDoc* >::iterator it = m_mapFresherQuest.begin();
        it != m_mapFresherQuest.end(); ++it)
    {
        it->second->doEncode(ioBuff, nSex);
    }
}

