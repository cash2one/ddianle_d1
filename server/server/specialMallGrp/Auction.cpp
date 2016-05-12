#include "Auction.h"
#include "SpecialMallProcessor.h"
#include "../libServerFrame/DBInterface.h"
#include "../mall/GameMsg_SpecialMall.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../datastructure/datainfor/AuctionGoodsInfo.h"

CAuctionSettings::CAuctionSettings(const CAuctionSettings &rSettings)
{
    *this = rSettings;
}

void CAuctionSettings::operator=(const CAuctionSettings &rSettings)
{
    if (this != &rSettings)
    {
        m_strMailTitle = rSettings.m_strMailTitle;
        m_strMailContent = rSettings.m_strMailContent;
        m_strRule = rSettings.m_strRule;
    }
}

//////////////////////////////////////////////////////////////////////////

typedef std::map<unsigned short, AuctionGoodsInfo *>   Map_US_Goods;
typedef std::map<unsigned short, Map_US_Goods>          Map_US_MapUSGoods;

typedef Map_US_Goods::iterator                          Map_US_Goods_Itr;
typedef Map_US_Goods::const_iterator                    Map_US_Goods_CItr;

typedef Map_US_MapUSGoods::iterator                     Map_US_MapUSGoods_Itr;
typedef Map_US_MapUSGoods::const_iterator               Map_US_MapUSGoods_CItr;

CAuction::CAuction(void)
{
    m_bFirstQuery = true;
}

CAuction::~CAuction(void)
{
    Clear();
}

bool CAuction::LoadGoodsInfo(const std::string &strGlobalPath)
{
    if (strGlobalPath.length() <= 0)
        return false;

    std::string strFileName(strGlobalPath);
    strFileName.append(AUCTION_FILE_NAME);

    CFileStream file;
    file.open(strFileName.c_str(), "rb");

    if (file.IsOpen())
    {
        Clear();

        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);

        for (unsigned short i = 0; i < ulNumber; ++i)
        {
            AuctionGoodsInfo *pGoodsInfo = new AuctionGoodsInfo;

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

void CAuction::Clear()
{
    for (Map_US_MapUSGoods_Itr bOuterIt = m_mapGoodsInfo.begin(), eOuterIt = m_mapGoodsInfo.end(); 
         bOuterIt != eOuterIt; ++bOuterIt)
    {
        for (Map_US_Goods_Itr bInnerIt = bOuterIt->second.begin(), eInnerIt = bOuterIt->second.end(); 
             bInnerIt != eInnerIt; ++bInnerIt)
        {
            delete (AuctionGoodsInfo *)bInnerIt->second;
        }

        bOuterIt->second.clear();
    }

    m_mapGoodsInfo.clear();
}

void CAuction::SetSettings(const CSpecialMallSettings *pSettings)
{
    const CAuctionSettings *pAuctionSettings = dynamic_cast<const CAuctionSettings *>(pSettings);

    if (NULL != pAuctionSettings)
        m_cSettings = *pAuctionSettings;
}

void CAuction::OnUpdateWithInterval(time_t tTime, unsigned int interval)
{
    CTimeMgr::OnUpdate(tTime);

    if (IsOpening() && 0 != interval)
    {
        Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

        if (m_mapGoodsInfo.end() == outerIt)
            return;

        std::set<unsigned short> setIdxs;
        CItem item;

        for (Map_US_Goods_Itr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
             bInnerIt != eInnerIt; ++bInnerIt)
        {
            if (0 == bInnerIt->second->m_fCountDown)
                continue;
            else if (bInnerIt->second->m_fCountDown > interval)
                bInnerIt->second->m_fCountDown -= 1;
            else if (bInnerIt->second->m_fCountDown <= interval)
            {
                setIdxs.insert(bInnerIt->second->m_cGoodsInfo.m_uIdx);

                bInnerIt->second->m_fCountDown = 0;

                bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&item);
                CSpecialMallProcessor::Instance().SendItem(bInnerIt->second->m_cGoodsInfo.m_uIdx, 
                    bInnerIt->second->m_uRoleID, item, bInnerIt->second->m_uCurPrice, false);
            }
        }

        if (setIdxs.size() > 0)
            CSpecialMallProcessor::Instance().ClearBidPeople(setIdxs);
    }
}

bool CAuction::IsOpening()
{
    bool bOpening = CTimeMgr::IsOpening();

    if (bOpening && IsAllItemTimeOut())
    {
        SetOpening(false);

        bOpening = false;
    }

    return bOpening;
}

void CAuction::GetState(unsigned char *pState, std::string *pTitle, std::string *pRule)
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

    if (NULL != pRule)
        *pRule = m_cSettings.m_strRule;
}

void CAuction::GetMailInfo(std::string *pMailTitle, std::string *pMailContent)
{
    if (NULL != pMailTitle)
        *pMailTitle = m_cSettings.m_strMailTitle;

    if (NULL != pMailContent)
        *pMailContent = m_cSettings.m_strMailContent;
}

void CAuction::GetItems(unsigned int nRoleID, unsigned char nSex, const GameMsg_C2S_Auction_Items *pInMsg, GameMsg_G2S_Auction_ItemsResult *pResMsg)
{
    if (NULL != pInMsg && NULL != pResMsg)
    {
        GetState(&pResMsg->m_nState, NULL, NULL);

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
        pResMsg->m_tEndTime = (unsigned int)GetCurRangeEndTime();

        const ItemConfigManager &rItemsInfoMgr = ConfigManager::Instance().GetItemConfigManager();
        const ItemConfig *pItemInfo = NULL;
        unsigned short beginCount = pInMsg->m_uNextPage * pInMsg->m_uCount;
        unsigned short endCount = beginCount + pInMsg->m_uCount;
        unsigned int minAdd = 0;

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
                CAuctionItemData data;

                data.m_uIdx = bInnerIt->second->m_cGoodsInfo.m_uIdx;
                bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&data.m_cItem);
                data.m_uCurPrice = bInnerIt->second->m_uCurPrice;
                data.m_uRoleID = bInnerIt->second->m_uRoleID;
                data.m_strRoleName = bInnerIt->second->m_strRoleName;
                data.m_fCountDown = bInnerIt->second->m_fCountDown;
                data.m_bAlreayTop = (nRoleID == data.m_uRoleID);

                minAdd = (unsigned int)(bInnerIt->second->m_cGoodsInfo.m_uSellPrice / 100.0 * bInnerIt->second->m_uAddPercent + 0.5f);
                data.m_uMinAdd = minAdd < bInnerIt->second->m_uMinAdd ? bInnerIt->second->m_uMinAdd : minAdd;

                pResMsg->m_listItems.push_back(data);
            }
        }

        pResMsg->m_fCD = CalcCD();
    }
}

unsigned char CAuction::BidItem(unsigned int roleID, const char *pName, GameMsg_G2S_Auction_BidResult &rResMsg, unsigned int &rOldRoleID, unsigned int &rOldPrice)
{
    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (!IsOpening() || m_mapGoodsInfo.end() == outerIt)
        return ESpecialMallError_NotOpen;

    Map_US_Goods_Itr innerIt = outerIt->second.find(rResMsg.m_uIdx);

    if (outerIt->second.end() == innerIt)
        return ESpecialMallError_NotFound;
    else if (innerIt->second->m_cGoodsInfo.m_uGoodsID != rResMsg.m_cItem.m_nItemType)
        return ESpecialMallError_NotFound;
    else if (innerIt->second->m_fCountDown <= 0)
        return ESpecialMallError_BidTimeOut;

    innerIt->second->m_cGoodsInfo.CopyInfoToCItem(&rResMsg.m_cItem);

    if (rResMsg.m_uPrice <= innerIt->second->m_uCurPrice)
        return ESpecialMallError_BidNotEnough;
    else if (roleID == innerIt->second->m_uRoleID)
        return ESpecialMallError_BidAlreadyTop;

    rOldPrice = innerIt->second->m_uCurPrice;
    rOldRoleID = innerIt->second->m_uRoleID;

    innerIt->second->m_uCurPrice = rResMsg.m_uPrice;
    innerIt->second->m_uRoleID = roleID;
    if (NULL != pName)
        innerIt->second->m_strRoleName = pName;

    return ESpecialMallError_None;
}

void CAuction::UpdateBidInfo(std::map<unsigned short, CGetAuctionBidInfo> *pInfo)
{
    if (NULL == pInfo || 0 == pInfo->size() || !IsOpening())
        return;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (m_mapGoodsInfo.end() == outerIt)
        return;

    std::map<unsigned short, CGetAuctionBidInfo>::const_iterator infoIt;
    time_t beginTime = GetCurRangeBeginTime();
    unsigned int offset = 0;
    CItem item;

    for (Map_US_Goods_Itr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
         bInnerIt != eInnerIt; )
    {
        infoIt = pInfo->find(bInnerIt->second->m_cGoodsInfo.m_uIdx);

        if (infoIt != pInfo->end())
        {
            if (0 == infoIt->second.m_nTime)
            {
                outerIt->second.erase(bInnerIt++);
                continue;
            }
            else if (infoIt->second.m_nTime < (unsigned int)beginTime)
            {
                ++bInnerIt;
                continue;
            }

            offset = infoIt->second.m_nTime - (unsigned int)beginTime;

            bInnerIt->second->m_uCurPrice = infoIt->second.m_uCurPrice;
            bInnerIt->second->m_uRoleID = infoIt->second.m_uRoleID;
            bInnerIt->second->m_strRoleName = infoIt->second.m_strRoleName;

            if (bInnerIt->second->m_fCountDown > offset)
                bInnerIt->second->m_fCountDown -= offset;
            else
            {
                bInnerIt->second->m_fCountDown = 0;

                bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&item);
                CSpecialMallProcessor::Instance().SendItem(bInnerIt->second->m_cGoodsInfo.m_uIdx, 
                    bInnerIt->second->m_uRoleID, item, bInnerIt->second->m_uCurPrice, false);
            }
        }

        ++bInnerIt;
    }
}

bool CAuction::CheckTopPrice(unsigned int nRoleID, unsigned short nIdx, unsigned int nEndTime)
{
    // 所以异常情况均判断为最高出价人
    if (!IsOpening())
        return true;
    else if (GetCurRangeEndTime() != (time_t)nEndTime)
        return true;

    Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (m_mapGoodsInfo.end() == outerIt)
        return true;

    Map_US_Goods_CItr innerIt = outerIt->second.find(nIdx);

    if (outerIt->second.end() == innerIt)
        return true;
    else if (innerIt->second->m_uRoleID == nRoleID)
        return true;

    return false;
}

void CAuction::AddGoodsInfo(AuctionGoodsInfo *pGoodsInfo)
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
            delete (AuctionGoodsInfo *)innerIt->second;
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

bool CAuction::IsAllItemTimeOut() const
{
    Map_US_MapUSGoods_CItr outerIt = m_mapGoodsInfo.find(GetCurRange());

    if (!CTimeMgr::IsOpening() || m_mapGoodsInfo.end() == outerIt)
        return false;

    for (Map_US_Goods_CItr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
         bInnerIt != eInnerIt; ++bInnerIt)
    {
        if (bInnerIt->second->m_fCountDown > 0)
            return false;
    }

    return true;
}

void CAuction::SetOpening(bool bOpening)
{
    CTimeMgr::SetOpening(bOpening);

    if (!bOpening)
    {
        Map_US_MapUSGoods_Itr outerIt = m_mapGoodsInfo.find(GetCurRange() - 1);

        if (outerIt != m_mapGoodsInfo.end())
        {
            CItem item;

            for (Map_US_Goods_Itr bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
                 bInnerIt != eInnerIt; ++bInnerIt)
            {
                if (bInnerIt->second->m_fCountDown > 0)
                {
                    bInnerIt->second->m_cGoodsInfo.CopyInfoToCItem(&item);

                    CSpecialMallProcessor::Instance().SendItem(bInnerIt->second->m_cGoodsInfo.m_uIdx, 
                        bInnerIt->second->m_uRoleID, item, bInnerIt->second->m_uCurPrice, true);
                }
            }
        }

        CSpecialMallProcessor::Instance().AuctionClose();
    }
    else if (m_bFirstQuery)
    {
        m_bFirstQuery = false;

        CSpecialMallProcessor::Instance().QueryAuctionInfo();
    }
}

