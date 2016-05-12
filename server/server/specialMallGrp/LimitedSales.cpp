#include "LimitedSales.h"
#include "SpecialMallProcessor.h"
#include "../libServerFrame/DBInterface.h"
#include "../mall/GameMsg_SpecialMall.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../datastructure/datainfor/LimitedGoodsInfo.h"
#include "../../socket/Log.h"

#define ADDITION_INTERVAL   5   //second

CLimitedSalesSettings::CLimitedSalesSettings()
{
    m_uAdditionLower = 0;
    m_uAdditionUpper = 0;
}

CLimitedSalesSettings::CLimitedSalesSettings(const CLimitedSalesSettings &rSettings)
{
    *this = rSettings;
}

void CLimitedSalesSettings::operator=(const CLimitedSalesSettings &rSettings)
{
    if (this != &rSettings)
    {
        m_strMailTitle = rSettings.m_strMailTitle;
        m_strMailContent = rSettings.m_strMailContent;
        m_uAdditionLower = rSettings.m_uAdditionLower;
        m_uAdditionUpper = rSettings.m_uAdditionUpper;
    }
}

//////////////////////////////////////////////////////////////////////////

CLimitedSalesGMSettings::CLimitedSalesGMSettings()
{
    m_uStartTime = 0;
    m_uEndTime = 0;
    m_uInterval = 0;
    m_uAdditionLower = 0;
    m_uAdditionUpper = 0;
}

CLimitedSalesGMSettings::CLimitedSalesGMSettings(const CLimitedSalesGMSettings &rSettings)
{
    *this = rSettings;
}

void CLimitedSalesGMSettings::operator=(const CLimitedSalesGMSettings &rSettings)
{
    if (this != &rSettings)
    {
        m_uStartTime = rSettings.m_uStartTime;
        m_uEndTime = rSettings.m_uEndTime;
        m_uInterval = rSettings.m_uInterval;
        m_uAdditionLower = rSettings.m_uAdditionLower;
        m_uAdditionUpper = rSettings.m_uAdditionUpper;
    }
}

//////////////////////////////////////////////////////////////////////////

typedef std::map<unsigned short, LimitedGoodsInfo *>   Map_US_Goods;
typedef std::map<unsigned short, Map_US_Goods>          Map_US_MapUSGoods;

typedef Map_US_Goods::iterator                          Map_US_Goods_Itr;
typedef Map_US_Goods::const_iterator                    Map_US_Goods_CItr;

typedef Map_US_MapUSGoods::iterator                     Map_US_MapUSGoods_Itr;
typedef Map_US_MapUSGoods::const_iterator               Map_US_MapUSGoods_CItr;

CLimitedSales::CLimitedSales(void)
{
    m_eType = ELimitedSalesType_Start;
    m_bFirstQuery = true;
    m_bUseGMSettings = false;
}

CLimitedSales::~CLimitedSales(void)
{
    Clear();
}

bool CLimitedSales::LoadGoodsInfo(const std::string &strGlobalPath, ELimitedSalesType type)
{
    if (strGlobalPath.length() <= 0 || type <= ELimitedSalesType_Start || type >= ELimitedSalesType_End)
        return false;

    std::string strFileName(strGlobalPath);
    const char *pName = NULL;

    switch (type)
    {
    case ELimitedSalesType_Group:
        pName = GROUP_BUYING_FILE_NAME;
        break;
    case ELimitedSalesType_Edition:
        pName = LIMITED_EDITION_FILE_NAME;
        break;
    case ELimitedSalesType_Start:
    case ELimitedSalesType_End:
    default:
        break;
    }

    if (NULL != pName)
        strFileName.append(pName);

    CFileStream file;
    file.open(strFileName.c_str(), "rb");

    if (file.IsOpen())
    {
        m_eType = type;
        Clear();

        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);

        for (unsigned short i = 0; i < ulNumber; ++i)
        {
            LimitedGoodsInfo *pGoodsInfo = new LimitedGoodsInfo;

            if (NULL != pGoodsInfo && pGoodsInfo->LoadFromFile(file) && 
                pGoodsInfo->m_uSerializeID <= GetRangeSize())
            {
                AddGoodsInfo(pGoodsInfo);
            }
            else
            {
                delete pGoodsInfo;
                pGoodsInfo = NULL;
            }
        }

        file.close();

        return true;
    }

    return false;
}

void CLimitedSales::Clear()
{
    for (Map_US_MapUSGoods_Itr bOuterIt = m_mapGoodsInfo.begin(), eOuterIt = m_mapGoodsInfo.end(); 
         bOuterIt != eOuterIt; ++bOuterIt)
    {
        for (Map_US_Goods_Itr bInnerIt = bOuterIt->second.begin(), eInnerIt = bOuterIt->second.end(); 
             bInnerIt != eInnerIt; ++bInnerIt)
        {
            delete (LimitedGoodsInfo *)bInnerIt->second;
        }

        bOuterIt->second.clear();
    }

    m_mapGoodsInfo.clear();
}

void CLimitedSales::SetSettings(const CSpecialMallSettings *pSettings)
{
    const CLimitedSalesSettings *pLimitedSettings = dynamic_cast<const CLimitedSalesSettings *>(pSettings);

    if (NULL != pLimitedSettings)
    {
        if (m_cSettings.m_uAdditionLower > m_cSettings.m_uAdditionUpper)
        {
            LOG_ERROR << "Addition lower(" << m_cSettings.m_uAdditionLower 
                      << ") > addition upper(" << m_cSettings.m_uAdditionUpper 
                      << ")" << std::endl;
        }
        else
        {
            m_cSettings = *pLimitedSettings;
        }
    }
}

void CLimitedSales::SetGMSettings(const CLimitedSalesGMSettings *pSettings)
{
    if (NULL != pSettings)
    {
        time_t tNow = time(NULL);

        if (pSettings->m_uStartTime >= pSettings->m_uEndTime)
        {
            LOG_ERROR << "Start time(" << pSettings->m_uStartTime 
                      << ") is bigger than end time(" << pSettings->m_uEndTime 
                      << ")" << std::endl;
        }
        else if (pSettings->m_uEndTime <= (unsigned int)tNow)
        {
            LOG_WARNING << "End time is smaller than now" << std::endl;
        }
        else if (pSettings->m_uAdditionLower > pSettings->m_uAdditionUpper)
        {
            LOG_ERROR << "Addition lower(" << pSettings->m_uAdditionLower 
                      << ") > addition upper(" << pSettings->m_uAdditionUpper 
                      << ")" << std::endl;
        }
        else
        {
            m_cGMSettings = *pSettings;

            LOG_DEBUG << "Change GM Settings" << std::endl;
        }
    }
}

void CLimitedSales::GetState(unsigned char *pState, std::string *pTitle)
{
    if (NULL != pState)
    {
        if (IsOpening())
            *pState = ESpecialMallState_OnSale;
        else if (CanParticipateAgain())
            *pState = ESpecialMallState_ComingSoon;
        else
            *pState = ESpecialMallState_Finished;
    }

    if (NULL != pTitle)
        *pTitle = GetCurRangeExtend();
}

void CLimitedSales::GetGroupBuyingItems(unsigned char nSex, const GameMsg_C2S_GroupBuying_Items *pInMsg, GameMsg_S2C_GroupBuying_ItemsResult *pResMsg)
{
    if (NULL != pInMsg && NULL != pResMsg && ELimitedSalesType_Group == m_eType)
    {
        GetState(&pResMsg->m_nState, NULL);

        if (ESpecialMallState_Finished == pResMsg->m_nState)
            return;

        Map_US_MapUSGoods_CItr outerIt = m_mapGoodsInfo.find(GetCurRange());

        if (m_mapGoodsInfo.end() == outerIt)
        {
            pResMsg->m_nState = ESpecialMallState_Finished;

            return;
        }

        pResMsg->m_bSearch = pInMsg->m_bSearch;
        pResMsg->m_strSearchKey = pInMsg->m_strSearchKey;
        pResMsg->m_uCurPage = pInMsg->m_uNextPage;
        pResMsg->m_uTotalCount = 0;

        const ItemConfigManager &rItemsInfoMgr = ConfigManager::Instance().GetItemConfigManager();
        const ItemConfig *pItemInfo = NULL;
        unsigned short beginCount = pInMsg->m_uNextPage * pInMsg->m_uCount;
        unsigned short endCount = beginCount + pInMsg->m_uCount;

        for (Map_US_Goods_CItr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
             bInnerIt != eInnerIt; ++bInnerIt)
        {
            pItemInfo = rItemsInfoMgr.GetByID(bInnerIt->second->m_cGoodsInfo.m_uGoodsID);

            if (NULL == pItemInfo || (ESexType_No != pItemInfo->m_SexNeed && nSex != pItemInfo->m_SexNeed))
                continue;

            if (pInMsg->m_bSearch && (std::string::npos == pItemInfo->m_strName.find(pInMsg->m_strSearchKey) ) )
                continue;

            ++pResMsg->m_uTotalCount;

            if (beginCount < pResMsg->m_uTotalCount && pResMsg->m_uTotalCount <= endCount)
            {
                CGroupBuyingItemData data;

                data.m_uIdx = bInnerIt->second->m_cGoodsInfo.m_uIdx;
                bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&data.m_cItem);
                data.m_uPrice = bInnerIt->second->m_cGoodsInfo.m_uSellPrice;
                data.m_nPriceType = bInnerIt->second->m_cGoodsInfo.m_nSellType;
                data.m_uDiscount = bInnerIt->second->m_uBuyPrice;
                data.m_nDiscountType = bInnerIt->second->m_nBuyType;
                data.m_nCustomCount = bInnerIt->second->m_nCount;

                pResMsg->m_listItems.push_back(data);
            }
        }

        pResMsg->m_fCD = CalcCD();
    }
}

void CLimitedSales::GetLimitedEditionItems(unsigned char nSex, const GameMsg_C2S_LimitedEdition_Items *pInMsg, GameMsg_S2C_LimitedEdition_ItemsResult *pResMsg)
{
    if (NULL != pInMsg && NULL != pResMsg && ELimitedSalesType_Edition == m_eType)
    {
        GetState(&pResMsg->m_nState, NULL);

        if (ESpecialMallState_Finished == pResMsg->m_nState)
            return;

        Map_US_MapUSGoods_CItr outerIt = m_mapGoodsInfo.find(GetCurRange());

        if (m_mapGoodsInfo.end() == outerIt)
        {
            pResMsg->m_nState = ESpecialMallState_Finished;

            return;
        }

        pResMsg->m_bSearch = pInMsg->m_bSearch;
        pResMsg->m_strSearchKey = pInMsg->m_strSearchKey;
        pResMsg->m_uCurPage = pInMsg->m_uNextPage;
        pResMsg->m_uTotalCount = 0;

        const ItemConfigManager &rItemsInfoMgr = ConfigManager::Instance().GetItemConfigManager();
        const ItemConfig *pItemInfo = NULL;
        unsigned short beginCount = pInMsg->m_uNextPage * pInMsg->m_uCount;
        unsigned short endCount = beginCount + pInMsg->m_uCount;

        for (Map_US_Goods_CItr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
             bInnerIt != eInnerIt; ++bInnerIt)
        {
            pItemInfo = rItemsInfoMgr.GetByID(bInnerIt->second->m_cGoodsInfo.m_uGoodsID);

            if (NULL == pItemInfo || (ESexType_No != pItemInfo->m_SexNeed && nSex != pItemInfo->m_SexNeed))
                continue;

            if (pInMsg->m_bSearch && (std::string::npos == pItemInfo->m_strName.find(pInMsg->m_strSearchKey) ) )
                continue;

            ++pResMsg->m_uTotalCount;

            if (beginCount < pResMsg->m_uTotalCount && pResMsg->m_uTotalCount <= endCount)
            {
                CLimitedEditionItemData data;

                data.m_uIdx = bInnerIt->second->m_cGoodsInfo.m_uIdx;
                bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&data.m_cItem);
                data.m_uPrice = bInnerIt->second->m_cGoodsInfo.m_uSellPrice;
                data.m_nPriceType = bInnerIt->second->m_cGoodsInfo.m_nSellType;
                data.m_uDiscount = bInnerIt->second->m_uBuyPrice;
                data.m_nDiscountType = bInnerIt->second->m_nBuyType;
                data.m_nRemainder = bInnerIt->second->m_nCount;

                pResMsg->m_listItems.push_back(data);
            }
        }

        pResMsg->m_fCD = CalcCD();
    }
}

unsigned char CLimitedSales::BuyItem(GameMsg_G2S_SpecialMall_Pay &rPayMsg)
{
    unsigned short range = GetCurRange();
    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(range);

    if (!IsOpening() || m_mapGoodsInfo.end() == outerIt)
        return ESpecialMallError_NotOpen;

    Map_US_Goods_Itr innerIt = outerIt->second.find(rPayMsg.m_uIdx);

    if (outerIt->second.end() == innerIt)
        return ESpecialMallError_NotFound;
    else if (innerIt->second->m_cGoodsInfo.m_uGoodsID != rPayMsg.m_cItem.m_nItemType)
        return ESpecialMallError_NotFound;
    else if (ELimitedSalesType_Edition == m_eType && 0 == (innerIt->second->m_nCount - innerIt->second->m_nLockCount) )
        return ESpecialMallError_SoldOut;

    // lock
    innerIt->second->m_nLockCount++;

    innerIt->second->m_cGoodsInfo.CopyInfoToCItem(&rPayMsg.m_cItem);
    rPayMsg.m_uRange = range;
    rPayMsg.m_uBuyPrice = innerIt->second->m_uBuyPrice;
    rPayMsg.m_nBuyType = innerIt->second->m_nBuyType;
    rPayMsg.m_tEndTime = GetCurRangeEndTime();
    rPayMsg.m_strMailTitle = m_cSettings.m_strMailTitle;
    rPayMsg.m_strMailContent = m_cSettings.m_strMailContent;

    return ESpecialMallError_None;
}

void CLimitedSales::PayResult(unsigned short uRange, unsigned short idx, bool bPaySuccess, CItem &item)
{
    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(uRange);

    if (m_mapGoodsInfo.end() == outerIt)
        return;

    Map_US_Goods_Itr innerIt = outerIt->second.find(idx);

    if (outerIt->second.end() == innerIt)
        return;

    if (innerIt->second->m_nLockCount > 0)
    {
        if (bPaySuccess)
        {
            if (ELimitedSalesType_Edition == m_eType)
                innerIt->second->m_nCount--;
            else if (ELimitedSalesType_Group == m_eType)
                innerIt->second->m_nCount++;
        }

        innerIt->second->m_nLockCount--;
    }

    innerIt->second->m_cGoodsInfo.CopyInfoToCItem(&item);
}

void CLimitedSales::UpdateCountAddition(int &timeElapsed)
{
    CompareGMSettings();

    int interval = 0;

    if (m_bUseGMSettings)
        interval = m_cGMSettings.m_uInterval;
    else
        interval = ADDITION_INTERVAL;

    interval *= 1000; // second to ms

    if (timeElapsed >= interval)
    {
        timeElapsed -= interval;

        CountAddition();
    }
}

void CLimitedSales::GetSaveCountInfo(std::list<CSaveGroupBuyingCount> &rListCount)
{
    if (ELimitedSalesType_Group != m_eType)
        return;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (m_mapGoodsInfo.end() == outerIt)
        return;

    for (Map_US_Goods_Itr bIt = outerIt->second.begin(), eIt = outerIt->second.end(); 
        bIt != eIt; ++bIt)
    {
        CSaveGroupBuyingCount countInfo;
        countInfo.m_uIdx = bIt->second->m_cGoodsInfo.m_uIdx;
        countInfo.m_uCount = bIt->second->m_nCount;
        countInfo.m_tEndTime = GetCurRangeEndTime();

        rListCount.push_back(countInfo);
    }
}

void CLimitedSales::UpdateCountInfo(std::list<CGetLimitedSalesCount> *pCountInfo)
{
    if (NULL == pCountInfo || !IsOpening())
        return;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (m_mapGoodsInfo.end() == outerIt)
        return;

    Map_US_Goods_Itr innerIt;

    for (std::list<CGetLimitedSalesCount>::const_iterator bIt = pCountInfo->begin(), eIt = pCountInfo->end(); 
         bIt != eIt; ++bIt)
    {
        innerIt = outerIt->second.find(bIt->m_uIdx);

        if (outerIt->second.end() != innerIt)
        {
            if (ELimitedSalesType_Group == m_eType && innerIt->second->m_nCount < (int)bIt->m_uCount)
            {
                innerIt->second->m_nCount = bIt->m_uCount;
            }
            else if (ELimitedSalesType_Edition == m_eType)
            {
                if (innerIt->second->m_nCount > (int)bIt->m_uCount)
                    innerIt->second->m_nCount -= (int)bIt->m_uCount;
                else
                    innerIt->second->m_nCount = 0;
            }
        }
    }
}

void CLimitedSales::AddGoodsInfo(LimitedGoodsInfo *pGoodsInfo)
{
    if (NULL == pGoodsInfo)
        return;

    Map_US_Goods goods;
    Map_US_Goods *pGoods = &goods;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(pGoodsInfo->m_uSerializeID);
    bool bOuterFind = ( outerIt != m_mapGoodsInfo.end() );

    if (bOuterFind)
    {
        pGoods = &outerIt->second;
        Map_US_Goods_Itr innerIt = pGoods->find(pGoodsInfo->m_cGoodsInfo.m_uIdx);

        if (innerIt != pGoods->end())
        {
            delete (LimitedGoodsInfo *)innerIt->second;
            pGoods->erase(innerIt);
        }
    }

    bool bRes = pGoods->insert(Map_US_Goods::value_type(pGoodsInfo->m_cGoodsInfo.m_uIdx, pGoodsInfo)).second;

    if (bRes && !bOuterFind)
        bRes = m_mapGoodsInfo.insert(Map_US_MapUSGoods::value_type(pGoodsInfo->m_uSerializeID, *pGoods)).second;

    if (!bRes)
    {
        delete pGoodsInfo;
        pGoodsInfo = NULL;
    }
}

void CLimitedSales::CompareGMSettings()
{
    time_t tNow = time(NULL);

    if (m_cGMSettings.m_uStartTime <= (unsigned int)tNow && tNow <= (time_t)m_cGMSettings.m_uEndTime)
        m_bUseGMSettings = true;
    else
        m_bUseGMSettings = false;
}

void CLimitedSales::CountAddition()
{
    if (!IsOpening())
        return;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (m_mapGoodsInfo.end() == outerIt)
        return;

    int countAddition = 0;
    unsigned int additionLower = 0;
    unsigned int additionUpper = 0;

    if (m_bUseGMSettings)
    {
        additionLower = m_cGMSettings.m_uAdditionLower;
        additionUpper = m_cGMSettings.m_uAdditionUpper;
    }
    else
    {
        additionLower = m_cSettings.m_uAdditionLower;
        additionUpper = m_cSettings.m_uAdditionUpper;
    }

    for (Map_US_Goods_Itr bIt = outerIt->second.begin(), eIt = outerIt->second.end(); 
         bIt != eIt; ++bIt)
    {
        countAddition = RandomAdditionNum(additionLower, additionUpper);

        if (0 == countAddition)
            continue;

        bIt->second->m_nCount += countAddition;
    }
}

int CLimitedSales::RandomAdditionNum(unsigned int lower, unsigned int upper)
{
    int additionNum = 0;

    if (lower == upper)
        additionNum = lower;
    else if (lower > upper)
        additionNum = 0;
    else
        additionNum = lower + rand() % (upper - lower);

    return additionNum;
}

void CLimitedSales::SetOpening(bool bOpening)
{
    CTimeMgr::SetOpening(bOpening);

    if (bOpening && m_bFirstQuery)
    {
        m_bFirstQuery = false;

        if (ELimitedSalesType_Group == m_eType)
            CSpecialMallProcessor::Instance().QueryGroupBuyingInfo();
        else if (ELimitedSalesType_Edition == m_eType)
            CSpecialMallProcessor::Instance().QueryLimitedEditionInfo();
    }
}

