#include "SpecialMallProcessor.h"
#include "../group/Global.h"
#include "../mail/WordMailManager.h"
#include "../share/ServerMsgDef.h"
#include "../libServerFrame/QueryManager.h"
#include "../GM/GameMsg_GM_share.h"
#include "../mall/GameMsg_SpecialMall.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"

extern std::string g_sPath;
extern CQueryManager* g_pQueryMgr;

#define UPDATE_INTERVAL     1000    //ms
#define SAVE_COUNT_INTERVAL 600000  //ms

typedef std::map<unsigned short, std::set<unsigned int> >   Map_US_Set;
typedef Map_US_Set::iterator                                Map_US_Set_Itr;

CSpecialMallProcessor::CSpecialMallProcessor(void)
    : GameMsg_Processor(true, true)
{
    m_nTimeElapsed = 0;
    m_nAddTimeElapsed = 0;
    m_nSaveCountTimeSlapsed = 0;
}

CSpecialMallProcessor::~CSpecialMallProcessor(void)
{
    Clear();
}

CSpecialMallProcessor& CSpecialMallProcessor::Instance()
{
    static CSpecialMallProcessor s_SpecialMallProcessor;

    return s_SpecialMallProcessor;    
}

void CSpecialMallProcessor::Initialize()
{
    RegisterMessage();

    LoadAllData();
}

void CSpecialMallProcessor::OnUpdate(int nTimeElapse)
{
    m_nTimeElapsed += nTimeElapse;
    m_nAddTimeElapsed += nTimeElapse;
    m_nSaveCountTimeSlapsed += nTimeElapse;

    if (m_nTimeElapsed >= UPDATE_INTERVAL)
    {
        m_nTimeElapsed -= UPDATE_INTERVAL;

        time_t tNow = time(NULL);

        m_cGroupBuying.OnUpdate(tNow);
        m_cLimitedEdition.OnUpdate(tNow);
        m_cAuction.OnUpdateWithInterval(tNow, UPDATE_INTERVAL / 1000);
    }

    m_cGroupBuying.UpdateCountAddition(m_nAddTimeElapsed);

    if (m_nSaveCountTimeSlapsed >= SAVE_COUNT_INTERVAL)
    {
        m_nSaveCountTimeSlapsed -= SAVE_COUNT_INTERVAL;

        std::list<CSaveGroupBuyingCount> listCount;
        listCount.clear();

        m_cGroupBuying.GetSaveCountInfo(listCount);

        for (std::list<CSaveGroupBuyingCount>::iterator bIt = listCount.begin(), eIt = listCount.end(); 
             bIt != eIt; ++bIt)
        {
            CSaveGroupBuyingCount *pCountInfo = new CSaveGroupBuyingCount(*bIt);

            g_pQueryMgr->AddQuery(QUERY_SaveGroupBuyingCount, 0, pCountInfo);
        }
    }
}

void CSpecialMallProcessor::Clear()
{
    m_cGroupBuying.Clear();
    m_cLimitedEdition.Clear();
    m_cAuction.Clear();
}

void CSpecialMallProcessor::SendItem(unsigned short idx, unsigned int roleID, const CItem &item, unsigned int price, bool bLastRange)
{
    if (0 == roleID)
    {
        // 无人竞拍
        // 直接写入SpecialMall和AuctionBid

        CSpecialMallInfoDB *pInfo = new CSpecialMallInfoDB;

        if (NULL != pInfo)
        {
            pInfo->m_uRoleID = roleID;
            pInfo->m_uIdx = idx;
            pInfo->m_uItemType = item.m_nItemType;
            pInfo->m_uItemCount = item.m_nItemCount;
            pInfo->m_nValidTime = item.m_nValidTime;
            pInfo->m_nBuyType = 1;//M币
            pInfo->m_uBuyPrice = price;
            pInfo->m_uUsedMoney = 0;
            pInfo->m_uUsedBindPt = 0;
            pInfo->m_uUsedPt = 0;
            pInfo->m_tEndTime = bLastRange ? m_cAuction.GetLastRangeEndTime() : m_cAuction.GetCurRangeEndTime();
            pInfo->m_eType = ESpecialMallType_Auction;

            g_pQueryMgr->AddQuery(QUERY_SaveSpecialMall, 0, pInfo);
        }
    }
    else
    {
        // 有人竞拍
        // 先更改状态
        // 回调后发放物品
        // 通知最高出价人

        CSetAuctionStateSuccessInfo *pInfo = new CSetAuctionStateSuccessInfo;

        if (NULL != pInfo)
        {
            pInfo->m_uRoleID = roleID;
            pInfo->m_uIdx = idx;
            pInfo->m_uItemType = item.m_nItemType;
            pInfo->m_uItemCount = item.m_nItemCount;
            pInfo->m_nValidTime = item.m_nValidTime;
            pInfo->m_uBidPrice = price;
            pInfo->m_tEndTime = bLastRange ? m_cAuction.GetLastRangeEndTime() : m_cAuction.GetCurRangeEndTime();

            CDelHelper delHelper(pInfo);

            g_pQueryMgr->AddQuery(QUERY_SetAuctionStateSuccess, 0, pInfo, 0, NULL, &delHelper);
        }
    }
}

void CSpecialMallProcessor::ClearBidPeople(std::set<unsigned short> &setIdxs)
{
    for (std::set<unsigned short>::const_iterator bIdxIt = setIdxs.begin(), eIdxIt = setIdxs.end(); 
        bIdxIt != eIdxIt; ++bIdxIt)
    {
        Map_US_Set_Itr outerIt = m_mapBidPeople.find(*bIdxIt);

        if (m_mapBidPeople.end() != outerIt)
        {
            for (std::set<unsigned int>::const_iterator bInnerIt = outerIt->second.begin(), eInnerIt = outerIt->second.end(); 
                bInnerIt != eInnerIt; ++bInnerIt)
                SendAuctionUpdateState(*bIdxIt, (unsigned int)-1, true);

            m_mapBidPeople.erase(outerIt);
        }
    }
}

void CSpecialMallProcessor::AuctionClose()
{
    m_mapBidPeople.clear();
}

void CSpecialMallProcessor::QueryGroupBuyingInfo()
{
    std::list<CGetLimitedSalesCount> *pGroup = new std::list<CGetLimitedSalesCount>;
    CDelHelper groupDelHelper(pGroup);
    g_pQueryMgr->AddQuery(QUERY_GetGroupBuyingCount, 0, pGroup, 0, NULL, &groupDelHelper);
}

void CSpecialMallProcessor::QueryLimitedEditionInfo()
{
    std::list<CGetLimitedSalesCount> *pEdition = new std::list<CGetLimitedSalesCount>;
    CDelHelper editionDelHelper(pEdition);
    g_pQueryMgr->AddQuery(QUERY_GetLimitedEditionCount, 0, pEdition, 0, NULL, &editionDelHelper);
}

void CSpecialMallProcessor::QueryAuctionInfo()
{
    std::map<unsigned short, CGetAuctionBidInfo> *pAuction = new std::map<unsigned short, CGetAuctionBidInfo>;
    CDelHelper auctionDelHelper(pAuction);
    g_pQueryMgr->AddQuery(QUERY_GetAuctionBidInfo, 0, pAuction, 0, NULL, &auctionDelHelper);
}

void CSpecialMallProcessor::InitLuaInfo(ESpecialMallType type, const CSpecialMallSettings *pSettings)
{
    switch (type)
    {
    case ESpecialMallType_GroupBuying:
        m_cGroupBuying.SetSettings(pSettings);
        break;

    case ESpecialMallType_LimitedEdition:
        m_cLimitedEdition.SetSettings(pSettings);
        break;

    case ESpecialMallType_Auction:
        m_cAuction.SetSettings(pSettings);
        break;

    case ESpecialMallType_None:
    default:
        LOG_ERROR << "Special mall type(" 
                  << type 
                  << ") is not valid." 
                  << std::endl;
        break;
    }
}

void CSpecialMallProcessor::AddTimeRangeInfo(ESpecialMallType type, time_t begin, time_t end, std::string &rTitle)
{
    switch (type)
    {
    case ESpecialMallType_GroupBuying:
        m_cGroupBuying.AddTimeRangeInfo(begin, end, rTitle);
        break;

    case ESpecialMallType_LimitedEdition:
        m_cLimitedEdition.AddTimeRangeInfo(begin, end, rTitle);
        break;

    case ESpecialMallType_Auction:
        m_cAuction.AddTimeRangeInfo(begin, end, rTitle);
        break;

    case ESpecialMallType_None:
    default:
        LOG_ERROR << "Special mall type(" 
                  << type 
                  << ") is not valid." 
                  << std::endl;
        break;
    }
}

void CSpecialMallProcessor::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SpecialMall_State);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GroupBuying_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GroupBuying_Buy);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LimitedEdition_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LimitedEdition_Buy);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Auction_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Auction_Bid);

    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_SpecialMall_PayResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Auction_CheckTopPrice);

    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GroupBuyControl);

    RegMsgProcessor(MSG_C2S_SpecialMall_State, &CSpecialMallProcessor::OnGetSpecialMallState);
    RegMsgProcessor(MSG_C2S_GroupBuying_Items, &CSpecialMallProcessor::OnGetGroupBuyingItems);
    RegMsgProcessor(MSG_C2S_GroupBuying_Buy, &CSpecialMallProcessor::OnGroupBuyingBuy);
    RegMsgProcessor(MSG_C2S_LimitedEdition_Items, &CSpecialMallProcessor::OnGetLimitedEditionItems);
    RegMsgProcessor(MSG_C2S_LimitedEdition_Buy, &CSpecialMallProcessor::OnLimitedEditionBuy);
    RegMsgProcessor(MSG_C2S_Auction_Items, &CSpecialMallProcessor::OnGetAuctionItems);
    RegMsgProcessor(MSG_C2S_Auction_Bid, &CSpecialMallProcessor::OnAuctionBid);

    RegMsgProcessor(MSG_S2G_SpecialMall_PayResult, &CSpecialMallProcessor::OnSpecialMallPayResult);
    RegMsgProcessor(MSG_S2G_Auction_CheckTopPrice, &CSpecialMallProcessor::OnAuctionCheckTopPrice);

    RegMsgProcessor(MSG_GM_M2G_GroupBuyControl, &CSpecialMallProcessor::OnGMGroupBuyControl);

    RegQueryProcessor(QUERY_GetGroupBuyingCount, &CSpecialMallProcessor::OnGetGroupBuyingCount);
    RegQueryProcessor(QUERY_GetLimitedEditionCount, &CSpecialMallProcessor::OnGetLimitedEditionCount);
    RegQueryProcessor(QUERY_GetAuctionBidInfo, &CSpecialMallProcessor::OnGetAuctionBidInfo);
    RegQueryProcessor(QUERY_SetAuctionStateSuccess, &CSpecialMallProcessor::OnDBSetAuctionStateSuccess);
}

void CSpecialMallProcessor::LoadAllData()
{
    if (!m_cGroupBuying.LoadGoodsInfo(g_sPath, ELimitedSalesType_Group) )
    {
        LOG_ERROR << "Load group buying goods info return false!" << std::endl;

        return;
    }

    if (!m_cLimitedEdition.LoadGoodsInfo(g_sPath, ELimitedSalesType_Edition) )
    {
        LOG_ERROR << "Load limited sales goods info return false!" << std::endl;

        return;
    }

    if (!m_cAuction.LoadGoodsInfo(g_sPath))
    {
        LOG_ERROR << "Load auction goods info return false!" << std::endl;

        return;
    }
}

void CSpecialMallProcessor::AddBidPeople(unsigned short idx, unsigned int roleID)
{
    Map_US_Set_Itr itr = m_mapBidPeople.insert(Map_US_Set::value_type(idx, std::set<unsigned int>() ) ).first;

    itr->second.insert(roleID);
}

void CSpecialMallProcessor::OnGetSpecialMallState(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity)
    {
        GameMsg_S2C_SpecialMall_StateResult resMsg;

        m_cGroupBuying.GetState(&resMsg.m_nGroupBuyingState, &resMsg.m_strGroupBuyingTitle);
        m_cLimitedEdition.GetState(&resMsg.m_nLimitedEditionState, &resMsg.m_strLimitedEditionTitle);
        m_cAuction.GetState(&resMsg.m_nAuctionState, &resMsg.m_strAuctionTitle, &resMsg.m_strAuctionRule);

        pEntity->SendPlayerMsg(&resMsg);
    }
}

void CSpecialMallProcessor::OnGetGroupBuyingItems(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_GroupBuying_Items *pInMsg = dynamic_cast<GameMsg_C2S_GroupBuying_Items *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        GameMsg_S2C_GroupBuying_ItemsResult resMsg;

        m_cGroupBuying.GetGroupBuyingItems(pEntity->m_nSex, pInMsg, &resMsg);

        pEntity->SendPlayerMsg(&resMsg);
    }
}

void CSpecialMallProcessor::OnGroupBuyingBuy(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_GroupBuying_Buy *pInMsg = dynamic_cast<GameMsg_C2S_GroupBuying_Buy *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        GameMsg_G2S_SpecialMall_Pay payMsg;
        payMsg.m_nSpecialMallType = ESpecialMallType_GroupBuying;
        payMsg.m_uIdx = pInMsg->m_uIdx;
        payMsg.m_cItem.m_nItemType = pInMsg->m_uGoodsID;

        unsigned char errorType = m_cGroupBuying.BuyItem(payMsg);

        if (ESpecialMallError_None == errorType)
            pEntity->SendPlayerMsg(&payMsg);
        else
            SendBuyRes(pEntity, payMsg.m_nSpecialMallType, errorType, payMsg.m_cItem);
    }
}

void CSpecialMallProcessor::OnGetLimitedEditionItems(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_LimitedEdition_Items *pInMsg = dynamic_cast<GameMsg_C2S_LimitedEdition_Items *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        GameMsg_S2C_LimitedEdition_ItemsResult resMsg;

        m_cLimitedEdition.GetLimitedEditionItems(pEntity->m_nSex, pInMsg, &resMsg);

        pEntity->SendPlayerMsg(&resMsg);
    }
}

void CSpecialMallProcessor::OnLimitedEditionBuy(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_LimitedEdition_Buy *pInMsg = dynamic_cast<GameMsg_C2S_LimitedEdition_Buy *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        GameMsg_G2S_SpecialMall_Pay payMsg;
        payMsg.m_nSpecialMallType = ESpecialMallType_LimitedEdition;
        payMsg.m_uIdx = pInMsg->m_uIdx;
        payMsg.m_cItem.m_nItemType = pInMsg->m_uGoodsID;

        unsigned char errorType = m_cLimitedEdition.BuyItem(payMsg);

        if (ESpecialMallError_None == errorType)
            pEntity->SendPlayerMsg(&payMsg);
        else
            SendBuyRes(pEntity, payMsg.m_nSpecialMallType, errorType, payMsg.m_cItem);
    }
}

void CSpecialMallProcessor::OnGetAuctionItems(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_Auction_Items *pInMsg = dynamic_cast<GameMsg_C2S_Auction_Items *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        GameMsg_G2S_Auction_ItemsResult resMsg;

        m_cAuction.GetItems(pEntity->m_nRoleID, pEntity->m_nSex, pInMsg, &resMsg);

        pEntity->SendPlayerMsg(&resMsg);
    }
}

void CSpecialMallProcessor::OnAuctionBid(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_C2S_Auction_Bid *pInMsg = dynamic_cast<GameMsg_C2S_Auction_Bid *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        unsigned int oldRoleID = 0;
        unsigned int oldPrice = 0;

        GameMsg_G2S_Auction_BidResult resMsg;
        resMsg.m_uIdx = pInMsg->m_uIdx;
        resMsg.m_cItem.m_nItemType = pInMsg->m_uGoodsID;
        resMsg.m_uPrice = pInMsg->m_uPrice;
        resMsg.m_nErrorType = m_cAuction.BidItem(pEntity->m_nRoleID, pEntity->m_szRoleName, resMsg, oldRoleID, oldPrice);

        if (ESpecialMallError_BidTimeOut != resMsg.m_nErrorType)
            AddBidPeople(resMsg.m_uIdx, pEntity->m_nRoleID);

        if (ESpecialMallError_None == resMsg.m_nErrorType)
        {
            SendAuctionSurpassed(resMsg.m_uIdx, oldRoleID, oldPrice, resMsg.m_cItem);

            SendAuctionUpdateState(resMsg.m_uIdx, pInMsg->m_uPrice);
        }

        pEntity->SendPlayerMsg(&resMsg);
    }
}

void CSpecialMallProcessor::OnSpecialMallPayResult(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_SpecialMall_PayResult *pInMsg = dynamic_cast<GameMsg_S2G_SpecialMall_PayResult *>(&rMsg);
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pEntity && NULL != pInMsg)
    {
        CItem item;

        switch (pInMsg->m_nSpecialMallType)
        {
        case ESpecialMallType_GroupBuying:
            m_cGroupBuying.PayResult(pInMsg->m_uRange, pInMsg->m_uIdx, pInMsg->m_bSuccess, item);
            break;

        case ESpecialMallType_LimitedEdition:
            m_cLimitedEdition.PayResult(pInMsg->m_uRange, pInMsg->m_uIdx, pInMsg->m_bSuccess, item);
            break;

        default:
            LOG_ERROR << "Special mall type(" 
                      << pInMsg->m_nSpecialMallType 
                      << ") is not valid." 
                      << std::endl;
            break;
        }

        SendBuyRes(pEntity, pInMsg->m_nSpecialMallType, (unsigned char)(pInMsg->m_bSuccess ? ESpecialMallError_None : ESpecialMallError_MoneyNotEnough), item);
    }
}

void CSpecialMallProcessor::OnAuctionCheckTopPrice(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_Auction_CheckTopPrice *pMsg = dynamic_cast<GameMsg_S2G_Auction_CheckTopPrice *>(&rMsg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);

    if (NULL != pMsg && NULL != pRoleEntity)
    {
        GameMsg_G2S_Auction_CheckTopPriceResult checkTopRes;
        bool bTopPrice;

        for (std::map<long long, CAuctionTopPriceData>::const_iterator itr = pMsg->m_mapTopInfo.begin(); 
             itr != pMsg->m_mapTopInfo.end(); ++itr)
        {
            bTopPrice = m_cAuction.CheckTopPrice(pRoleEntity->m_nRoleID, itr->second.m_uIdx, itr->second.m_tEndTime);

            checkTopRes.m_mapTopInfo.insert(std::make_pair(itr->first, bTopPrice));
        }

        pRoleEntity->SendPlayerMsg(&checkTopRes);
    }
}

void CSpecialMallProcessor::OnGMGroupBuyControl(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_M2G_GroupBuyControl *pMsg = dynamic_cast<GameMsg_M2G_GroupBuyControl *>(&rMsg);

    if (NULL != pMsg)
    {
        CLimitedSalesGMSettings gmSettings;
        gmSettings.m_uStartTime = pMsg->m_nStartTime;
        gmSettings.m_uEndTime = pMsg->m_nEndTime;
        gmSettings.m_uInterval = pMsg->m_nInterval;
        gmSettings.m_uAdditionLower = pMsg->m_nMinPeople;
        gmSettings.m_uAdditionUpper = pMsg->m_nMaxPeople;

        m_cGroupBuying.SetGMSettings(&gmSettings);
    }
}

void CSpecialMallProcessor::OnGetGroupBuyingCount(QueryBase &query)
{
    std::list<CGetLimitedSalesCount> *pDBInfo = static_cast<std::list<CGetLimitedSalesCount> *>(query.m_pAttachment);

    if (NULL != pDBInfo)
    {
        m_cGroupBuying.UpdateCountInfo(pDBInfo);

        delete pDBInfo, pDBInfo = NULL;
    }
}

void CSpecialMallProcessor::OnGetLimitedEditionCount(QueryBase &query)
{
    std::list<CGetLimitedSalesCount> *pDBInfo = static_cast<std::list<CGetLimitedSalesCount> *>(query.m_pAttachment);

    if (NULL != pDBInfo)
    {
        m_cLimitedEdition.UpdateCountInfo(pDBInfo);

        delete pDBInfo, pDBInfo = NULL;
    }
}

void CSpecialMallProcessor::OnGetAuctionBidInfo(QueryBase &query)
{
    std::map<unsigned short, CGetAuctionBidInfo> *pDBInfo = static_cast<std::map<unsigned short, CGetAuctionBidInfo> *>(query.m_pAttachment);

    if (NULL != pDBInfo)
    {
        m_cAuction.UpdateBidInfo(pDBInfo);

        delete pDBInfo, pDBInfo = NULL;
    }
}

void CSpecialMallProcessor::OnDBSetAuctionStateSuccess(QueryBase &query)
{
    CSetAuctionStateSuccessInfo *pInfo = static_cast<CSetAuctionStateSuccessInfo *>(query.m_pAttachment);

    if (NULL != pInfo)
    {
        if (query.m_nResult == Query_Res_OK && pInfo->m_bSuccess)
        {
            std::string strMailTitle;
            std::string strMailContent;

            m_cAuction.GetMailInfo(&strMailTitle, &strMailContent);

            CWMailManager::Instance().SendSystemMailToPlayer( pInfo->m_uRoleID, EMailType_SpecialMall, strMailTitle,
                strMailContent, pInfo->m_uItemType, pInfo->m_uItemCount, pInfo->m_nValidTime );

            CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(pInfo->m_uRoleID);

            if (NULL != pRoleEntity)
            {
                GameMsg_G2S_Auction_BidSuccess msg;
                msg.m_uIdx = pInfo->m_uIdx;
                msg.m_cItem.m_nItemType = pInfo->m_uItemType;
                msg.m_cItem.m_nItemCount = pInfo->m_uItemCount;
                msg.m_cItem.m_nValidTime = pInfo->m_nValidTime;
                msg.m_uPrice = pInfo->m_uBidPrice;

                pRoleEntity->SendPlayerMsg(&msg);
            }
            else
            {
                g_pQueryMgr->AddQuery(QUERY_SaveVIPConsumedPt, pInfo->m_uRoleID, NULL, pInfo->m_uBidPrice);
            }
            
            // 累计消费统计
            g_pQueryMgr->AddQuery(QUERY_Insert_CumulativeSpendBill, pInfo->m_uRoleID, NULL, pInfo->m_uBidPrice );
        }

        delete pInfo, pInfo = NULL;
    }
}

void CSpecialMallProcessor::SendBuyRes(CGroupRoleEntity *pEntity, unsigned char nSpecialMallType, unsigned char errorType, const CItem &item)
{
    switch (nSpecialMallType)
    {
    case ESpecialMallType_GroupBuying:
        {
            if (NULL != pEntity)
            {
                GameMsg_S2C_GroupBuying_BuyResult resMsg;
                resMsg.m_nErrorType = errorType;
                resMsg.m_cItem = item;

                pEntity->SendPlayerMsg(&resMsg);
            }
        }
        break;

    case ESpecialMallType_LimitedEdition:
        {
            if (NULL != pEntity)
            {
                GameMsg_S2C_LimitedEdition_BuyResult resMsg;
                resMsg.m_nErrorType = errorType;
                resMsg.m_cItem = item;

                pEntity->SendPlayerMsg(&resMsg);
            }
        }
        break;

    default:
        LOG_ERROR << "Special mall type(" 
                  << nSpecialMallType 
                  << ") is not valid." 
                  << std::endl;
        break;
    }
}

void CSpecialMallProcessor::SendAuctionSurpassed(unsigned short idx, unsigned int roleID, unsigned int price, CItem &item)
{
    CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByID(roleID);

    if (NULL != pEntity)
    {
        GameMsg_G2S_Auction_Surpassed msg;
        msg.m_uIdx = idx;
        msg.m_cItem = item;
        msg.m_uPrice = price;

        pEntity->SendPlayerMsg(&msg);
    }
}

void CSpecialMallProcessor::SendAuctionUpdateState(unsigned short idx, unsigned int price, bool bTimeOut /* = false */)
{
    Map_US_Set_Itr outerItr = m_mapBidPeople.find(idx);

    if (m_mapBidPeople.end() == outerItr)
        return;

    GameMsg_G2S_Auction_UpdateState msg;
    msg.m_uIdx = idx;
    msg.m_uCurPrice = price;
    msg.m_bTimeOut = bTimeOut;

    for (std::set<unsigned int>::const_iterator bInnerIt = outerItr->second.begin(), eInnerIt = outerItr->second.end(); 
         bInnerIt != eInnerIt; ++bInnerIt)
    {
        CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByID(*bInnerIt);

        if (NULL != pEntity)
            pEntity->SendPlayerMsg(&msg);
    }
}

