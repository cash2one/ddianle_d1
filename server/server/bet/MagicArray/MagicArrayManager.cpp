#include "MagicArrayManager.h"
#include "MagicArrayDataManager.h"
#include "../../attribute/EntityAttributeComponent.h"
#include "../../libServerFrame/QueryManager.h"
#include "../../../datastructure/datainfor/ConfigManager.h"
#include "../../../datastructure/datainfor/MagicArrayConfig.h"
#include "../../item/EntityItemComponent.h"
#include "GameMsg_CS_MagicLuckDraw.h"
#include "../../chat/ChatManager.h"
#include "../../../socket/Formatter.h"
#include "../../../datastructure/Localization.h"
#include "../BetComponent.h"
#include "../../util/Split.h"

extern CQueryManager *g_pQueryMgr;
MagicArrayManager::MagicArrayManager()
{

}

MagicArrayManager::~MagicArrayManager()
{

}

void MagicArrayManager::CreateFromDB(unsigned int nRoleID, const std::map<unsigned int, MagicArrayRecord> & mapRecord,
    const MagicArrayExchangeRecord & exchangeRecord)
{
    std::map<unsigned int, MagicArrayRecord>::const_iterator it = mapRecord.begin();
    for (; it != mapRecord.end(); ++it)
    {
        m_mapMagciArrayRecord[it->first] = it->second;
        m_setIsFirstSave.insert(it->first);
    }

    if (ConfigManager::Instance().GetMagicArrayConfigMgr().IsInRefreshValidTime(exchangeRecord.m_nRefreshTime))
    {
        m_MagicArrayExchange = exchangeRecord;
    }
    else
    {
        if (!exchangeRecord.m_mapRefreshGroup.empty() || exchangeRecord.m_nRefreshTime > 0)
        {
            m_MagicArrayExchange.m_nRefreshTime = (unsigned int)time(NULL);
            SynceMagicArrayExchangeToDB(nRoleID, ESyncToDBType_Update);
            InsertIntoLog(nRoleID, CMagicArrayLog::EMagicArrayAction_ExchangeReset, 0);
        }
    }
}

bool MagicArrayManager::PacketToCache(std::map<unsigned int, MagicArrayRecord> & mapRecord, MagicArrayExchangeRecord & exchangeRecord) const
{
    mapRecord = m_mapMagciArrayRecord;
    exchangeRecord = m_MagicArrayExchange;

    return true;
}


unsigned int MagicArrayManager::GetCallAccumulatedCount(unsigned int nMagicArrayID)
{
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        return it->second.m_nAccumulatedCallCount;
    }

    return 0;
}

void MagicArrayManager::CheckCanFreeCall(std::list<unsigned int> & listMagicArrayID)
{
    std::list<unsigned int> listMagicArrayConfigID;
    ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayID(listMagicArrayConfigID);

    std::list<unsigned int>::iterator it = listMagicArrayConfigID.begin();
    for (; it != listMagicArrayConfigID.end(); ++it)
    {
        std::map<unsigned int, MagicArrayRecord>::iterator itr = m_mapMagciArrayRecord.find(*it);
        if (itr != m_mapMagciArrayRecord.end())
        {
            if (IsFreeCall(itr->first))
            {
                listMagicArrayID.push_back(itr->first);
            }
        }
        else
        {
            listMagicArrayID.push_back(*it);
        }
    }
}

unsigned int MagicArrayManager::GetCallCDLeftTime(unsigned int nMagicArrayID)
{
    MagicArrayConfig * pConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
    if (pConfig == NULL)
    {
        return (unsigned int)time(NULL);
    }

    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        unsigned int nNowTime = (unsigned int)time(NULL);
        if (it->second.m_nFreeCallTime + pConfig->m_nFreeCallCD > nNowTime)
        {
            return (it->second.m_nFreeCallTime + pConfig->m_nFreeCallCD) - nNowTime;
        }
        else
        {
            return 0;
        }
    }

    return 0;
}

bool MagicArrayManager::IsFirstSave(unsigned int nMagicArrayID)
{
    std::set<unsigned int>::iterator it = m_setIsFirstSave.find(nMagicArrayID);
    if (it != m_setIsFirstSave.end())
    {
        return false;
    }

    return true;
}

void MagicArrayManager::InsertIntoLog(unsigned int nRoleID, CMagicArrayLog::EMagicArrayAction eAction, unsigned int nMagicArrayID)
{
    CMagicArrayLog * pLog = new CMagicArrayLog();
    if (pLog != NULL)
    {
        pLog->m_nRoleID = nRoleID;
        pLog->m_eAction = eAction;
        pLog->m_nMagicArrayID = nMagicArrayID;
        g_pQueryMgr->AddQuery(QUERY_LogRoleMagicArray, 0, pLog);
    }
}

void MagicArrayManager::UpdateIsFirstCall(unsigned int nMagicArrayID)
{
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        it->second.m_bHaveFirstCall = true;
    }
    else
    {
        MagicArrayRecord record;
        record.m_nMagicArrayID = nMagicArrayID;
        record.m_bHaveFirstCall = true;
        m_mapMagciArrayRecord[nMagicArrayID] = record;
    }
}

void MagicArrayManager::UpdateFreeCallTime(unsigned int nMagicArrayID)
{
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        it->second.m_nFreeCallTime = (unsigned int)time(NULL);
    }
    else
    {
        MagicArrayRecord record;
        record.m_nMagicArrayID = nMagicArrayID;
        record.m_nFreeCallTime = (unsigned int)time(NULL);
        m_mapMagciArrayRecord[nMagicArrayID] = record;
    }
}

void MagicArrayManager::UpdateCallAccumulatedCount(unsigned int nMagicArrayID, int nCount)
{
    MagicArrayConfig * pConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
    if (pConfig == NULL)
    {
        return;
    }

    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        it->second.m_nAccumulatedCallCount = nCount;

        if (it->second.m_nAccumulatedCallCount >= pConfig->m_nAccumulatedCount)
        {
            it->second.m_nAccumulatedCallCount -= pConfig->m_nAccumulatedCount;
        }
    }
    else
    {
        MagicArrayRecord record;
        record.m_nMagicArrayID = nMagicArrayID;
        record.m_nAccumulatedCallCount = nCount;
        m_mapMagciArrayRecord[nMagicArrayID] = record;
    }
}

bool MagicArrayManager::IsFreeCall(unsigned int nMagicArrayID)
{
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        MagicArrayConfig * pConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
        if (pConfig == NULL)
        {
            return false;
        }

        if (it->second.m_nFreeCallTime + pConfig->m_nFreeCallCD > (unsigned int)time(NULL))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return true;
}

bool MagicArrayManager::IsFirstOnceCall(unsigned int nMagicArrayID)
{
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
    if (it != m_mapMagciArrayRecord.end())
    {
        return it->second.m_bHaveFirstCall ? false : true;
    }

    return true;
}

void MagicArrayManager::SynceMagicArrayExchangeToDB(unsigned int nRoleID, ESyncToDBType eType)
{
    switch (eType)
    {
    case ESyncToDBType_Insert:
        {
            MagicArrayExchangeRecord * pRecord = new MagicArrayExchangeRecord(m_MagicArrayExchange);
            if (pRecord != NULL)
            {
                g_pQueryMgr->AddQuery(QUERY_Insert_MagicArrayExchange, nRoleID, pRecord);
            }
        }
        break;
    case ESyncToDBType_Update:
        {
            MagicArrayExchangeRecord * pRecord = new MagicArrayExchangeRecord(m_MagicArrayExchange);
            if (pRecord != NULL)
            {
                g_pQueryMgr->AddQuery(QUERY_Update_MagicArrayExchange, nRoleID, pRecord);
            }
        }
        break;
    case ESyncToDBType_None:
    default:
        break;
    }
}

void MagicArrayManager::SyncMagicArrayRecordToDB(unsigned int nRoleID, unsigned int nMagicArrayID)
{
    if (IsFirstSave(nMagicArrayID))
    {
        std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
        if (it != m_mapMagciArrayRecord.end())
        {
            MagicArrayRecord * pMagicArrayRecord = new MagicArrayRecord(it->second);
            CDelHelper delHelper(pMagicArrayRecord);
            g_pQueryMgr->AddQuery(QUERY_Insert_MagicArrayRecord, nRoleID, pMagicArrayRecord, 0, NULL, &delHelper);

            m_setIsFirstSave.insert(nMagicArrayID);
        }
    }
    else
    {   
        std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.find(nMagicArrayID);
        if (it != m_mapMagciArrayRecord.end())
        {
            MagicArrayRecord * pMagicArrayRecord = new MagicArrayRecord(it->second);
            CDelHelper delHelper(pMagicArrayRecord);
            g_pQueryMgr->AddQuery(QUERY_Update_MagicArrayRecord, nRoleID, pMagicArrayRecord, 0, NULL, &delHelper);
        }
    }
}

int MagicArrayManager::TenMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem)
{
    // check
    MagicArrayConfig * pMagicArrayConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
    if (pMagicArrayConfig == NULL)
    {
        return EMagicArrayErrorFlag_ConfigNot;
    }

    // item
    CItem costitem;
    std::list<CItem>::reverse_iterator itr = pMagicArrayConfig->m_listTenCostItem.rbegin();
    for (; itr != pMagicArrayConfig->m_listTenCostItem.rend(); ++itr)
    {
        if (betComponent.GetRoleItem()->GetItemCount(itr->m_nItemType, false, false) < itr->m_nItemCount)
        {
            continue;
        }

        costitem = *itr;
    }

    // delete money or item
    if (costitem.m_nItemType == 0)
    {
        if (!betComponent.GetRoleAttr()->IsEnoughCurrency((EMallCurrencyType)pMagicArrayConfig->m_nCurrencyType, pMagicArrayConfig->m_nTenCallValue))
        {
            return EMagicArrayErrorFlag_MoneyNotEnough;
        }

        // deduct money
        int a[3] = { (int)EChangeBillCause_Del_MagicArrayCall, (int)EChangeMoneyCause_Del_MagicArrayCall, (int)EChangeBindBillCause_Del_MagicArrayCall };
        std::vector<int> vecCause(a, a + 3);
        betComponent.GetRoleAttr()->DeductCurrency((EMallCurrencyType)pMagicArrayConfig->m_nCurrencyType, pMagicArrayConfig->m_nTenCallValue, vecCause);
    }
    else
    {
        betComponent.GetRoleItem()->RemoveItemByType(costitem.m_nItemType, costitem.m_nItemCount, EItemAction_Add_MagicArrayCall);
    }

    // fixed item
    std::list<CItem>::iterator itt = pMagicArrayConfig->m_listTenFixedItem.begin();
    if (itt != pMagicArrayConfig->m_listTenFixedItem.end())
    {
        fixedItem = *itt;
        betComponent.GetRoleItem()->AddItem(itt->m_nItemType, itt->m_nItemCount, itt->m_nValidTime, EItemAction_Add_MagicArrayCall, 0, true, false, false);
    }

    // calc reward
    std::list<MagicArrayReward> listMagicArrayReward;
    int nSex = (int)betComponent.GetRoleAttr()->GetSex();
    int nTotalCallCount = pMagicArrayConfig->m_nFirstTenCallRateCount + pMagicArrayConfig->m_nSecondTenCallRateCount;
    if (GetCallAccumulatedCount(nMagicArrayID) + nTotalCallCount >= pMagicArrayConfig->m_nAccumulatedCount)
    {
        for (unsigned int i = 0; i < pMagicArrayConfig->m_nFirstTenCallRateCount; ++i)
        {
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listFirstRollRate, listMagicArrayReward);
        }
    }
    else
    {
        for (unsigned int i = 0; i < pMagicArrayConfig->m_nFirstTenCallRateCount; ++i)
        {
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listFirstTenCallRate, listMagicArrayReward);
        }
    }
    
    for (unsigned int i = 0; i < pMagicArrayConfig->m_nSecondTenCallRateCount; ++i)
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listSecondTenCallRate, listMagicArrayReward);
    }

    // update accumulated count
    UpdateCallAccumulatedCount(nMagicArrayID, GetCallAccumulatedCount(nMagicArrayID) + nTotalCallCount);

    // add reward
    std::list<MagicArrayReward>::iterator it = listMagicArrayReward.begin();
    for (; it != listMagicArrayReward.end(); ++it)
    {
        ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_Item.m_nItemType);
        if (pItemConfig == NULL)
        {
            continue;
        }

        if (betComponent.GetRoleItem()->AddItem(it->m_Item.m_nItemType, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime, EItemAction_Add_MagicArrayCall, 0, true, false, false)
            == EMagicArrayErrorFlag_Scuess)
        {
            CallReward callreward;
            if (it->m_nGroupID <= 2)
            {
                callreward.m_bSpecial = true;
            }
            callreward.m_RewardItem = it->m_Item;
            listRewardItem.push_back(callreward);

            if (it->m_Item.m_bAnnounce)
            {
                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("MagicArray_Announce"),
                    betComponent.GetRoleAttr()->GetRoleName(), pItemConfig->m_strName.c_str());
                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }
        else
        {
            std::list<CItem> listCompensationReward;
            ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayCompensation(nMagicArrayID, it->m_nGroupID, nSex, listCompensationReward);
            betComponent.GetRoleItem()->AddItem(listCompensationReward, EItemAction_Add_MagicArrayCall, 0, true);

            std::list<CItem>::iterator itt = listCompensationReward.begin();
            for (; itt != listCompensationReward.end(); ++itt)
            {
                CallReward callreward;
                callreward.m_RewardItem = *itt;
                listRewardItem.push_back(callreward);
            }
        }
    }

    // sync to db
    SyncMagicArrayRecordToDB(betComponent.GetRoleAttr()->GetRoleID(), nMagicArrayID);

    // log
    InsertIntoLog(betComponent.GetRoleAttr()->GetRoleID(), CMagicArrayLog::EMagicArrayAction_TenCall, nMagicArrayID);
    return EMagicArrayErrorFlag_Scuess;
}

int MagicArrayManager::OnceMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem)
{
    // check
	MagicArrayConfig * pMagicArrayConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
	if (pMagicArrayConfig == NULL)
	{
		return EMagicArrayErrorFlag_ConfigNot;
	}

    // item or money
    int nSex = (int)betComponent.GetRoleAttr()->GetSex();
    if (!IsFreeCall(nMagicArrayID))
    {
        CItem costitem;
        std::list<CItem>::reverse_iterator itr = pMagicArrayConfig->m_listOnceCostItem.rbegin();
        for (; itr != pMagicArrayConfig->m_listOnceCostItem.rend(); ++itr)
        {
            if (betComponent.GetRoleItem()->GetItemCount(itr->m_nItemType, false, false) < itr->m_nItemCount)
            {
                continue;
            }

            costitem = *itr;
        }

        if (costitem.m_nItemType == 0)
        {
            if (!betComponent.GetRoleAttr()->IsEnoughCurrency((EMallCurrencyType)pMagicArrayConfig->m_nCurrencyType, pMagicArrayConfig->m_nOnceCallValue))
            {
                return EMagicArrayErrorFlag_MoneyNotEnough;
            }

            // deduct money
            int a[3] = { (int)EChangeBillCause_Del_MagicArrayCall, (int)EChangeMoneyCause_Del_MagicArrayCall, (int)EChangeBindBillCause_Del_MagicArrayCall };
            std::vector<int> vecCause(a, a + 3);
            betComponent.GetRoleAttr()->DeductCurrency((EMallCurrencyType)pMagicArrayConfig->m_nCurrencyType, pMagicArrayConfig->m_nOnceCallValue, vecCause);
        }
        else
        {
            betComponent.GetRoleItem()->RemoveItemByType(costitem.m_nItemType, costitem.m_nItemCount, EItemAction_Add_MagicArrayCall);
        }
    }

    // log
    if (IsFreeCall(nMagicArrayID))
    {
        InsertIntoLog(betComponent.GetRoleAttr()->GetRoleID(), CMagicArrayLog::EMagicArrayAction_FreeCall, nMagicArrayID);
    }
    else
    {
        InsertIntoLog(betComponent.GetRoleAttr()->GetRoleID(), CMagicArrayLog::EMagicArrayAction_OnceCall, nMagicArrayID);
    }
   
    // fixed item
    std::list<CItem>::iterator itt = pMagicArrayConfig->m_listOnceFixedItem.begin();
    if (itt != pMagicArrayConfig->m_listOnceFixedItem.end())
    {
        fixedItem = *itt;
        betComponent.GetRoleItem()->AddItem(itt->m_nItemType, itt->m_nItemCount, itt->m_nValidTime, EItemAction_Add_MagicArrayCall, 0, true, false, false);
    }

    // calc reward
    std::list<MagicArrayReward> listMagicArrayReward;
    if (GetCallAccumulatedCount(nMagicArrayID) == pMagicArrayConfig->m_nAccumulatedCount)
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listAccumulatedRollRate, listMagicArrayReward);
    }
    else
    {
        if (!IsFreeCall(nMagicArrayID))
        {
            if (IsFirstOnceCall(nMagicArrayID))
            {
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listFirstRollRate, listMagicArrayReward);
                UpdateIsFirstCall(nMagicArrayID);
            }
            else
            {
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listOnceCallRate, listMagicArrayReward);
            }
        }
        else
        {
            UpdateFreeCallTime(nMagicArrayID);
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, nSex, pMagicArrayConfig->m_listOnceCallRate, listMagicArrayReward);
        }
    }

    // update accumulated count
    UpdateCallAccumulatedCount(nMagicArrayID, GetCallAccumulatedCount(nMagicArrayID) + 1);

    // add reward
    std::list<MagicArrayReward>::iterator itr = listMagicArrayReward.begin();
    for (; itr != listMagicArrayReward.end(); ++itr)
    {
        ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_Item.m_nItemType);
        if (pItemConfig == NULL)
        {
            continue;
        }

        if (betComponent.GetRoleItem()->AddItem(itr->m_Item.m_nItemType, itr->m_Item.m_nItemCount, itr->m_Item.m_nValidTime, EItemAction_Add_MagicArrayCall, 0, true, false, false)
            == EMagicArrayErrorFlag_Scuess)
        {
            CallReward callreward;
            if (itr->m_nGroupID <= 2)
            {
                callreward.m_bSpecial = true;
            }
            callreward.m_RewardItem = itr->m_Item;
            listRewardItem.push_back(callreward);

            if (itr->m_Item.m_bAnnounce)
            {
                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("MagicArray_Announce"),
                    betComponent.GetRoleAttr()->GetRoleName(), pItemConfig->m_strName.c_str());
                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }
        else
        {
            std::list<CItem> listCompensationReward;
            ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayCompensation(nMagicArrayID, itr->m_nGroupID, nSex, listCompensationReward);
            betComponent.GetRoleItem()->AddItem(listCompensationReward, EItemAction_Add_MagicArrayCall, 0, true);

            std::list<CItem>::iterator itt = listCompensationReward.begin();
            for (; itt != listCompensationReward.end(); ++itt)
            {
                CallReward callreward;
                callreward.m_RewardItem = *itt;
                listRewardItem.push_back(callreward);
            }
        }
    }

    // update DB
    SyncMagicArrayRecordToDB(betComponent.GetRoleAttr()->GetRoleID(), nMagicArrayID);
   
    return EMagicArrayErrorFlag_Scuess;
}

void MagicArrayManager::SyncMagicArrayInfo(CBetComponent & betComponent)
{
    GameMsg_S2C_RequestMagicArrayConfigResult resultmsg;
    std::map<unsigned int, unsigned int> mapCDTime;
    std::map<unsigned int, MagicArrayRecord>::iterator it = m_mapMagciArrayRecord.begin();
    for (; it != m_mapMagciArrayRecord.end(); ++it)
    {
        mapCDTime[it->first] = GetCallCDLeftTime(it->first);
    }
    resultmsg.m_mapCDRecord = mapCDTime;
    resultmsg.m_mapRecord = m_mapMagciArrayRecord;

    betComponent._SendPlayerMsg(&resultmsg);
}

int MagicArrayManager::ExchangeItem(CBetComponent & betComponent, unsigned int nExchangeID)
{
    CItem costitem;
    CItem exchangeitem;
    int nSex = (int)betComponent.GetRoleAttr()->GetSex();
    if (m_MagicArrayExchange.m_mapRefreshGroup.empty())
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().GetBaseExchangeConfig(nExchangeID, nSex, exchangeitem, costitem);
    }
    else
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().GetRrfreshExchangeConfig(nExchangeID, nSex, exchangeitem, costitem);
    }

    if (betComponent.GetRoleItem()->GetItemCount(costitem.m_nItemType, false, false) < costitem.m_nItemCount)
    {
        return EMagicArrayErrorFlag_ItemNotEnough;
    }

    // delete cost item
    betComponent.GetRoleItem()->RemoveItemByType(costitem.m_nItemType, costitem.m_nItemCount, EItemAction_Del_MagicArrayExchange);

    // add exchange item
    betComponent.GetRoleItem()->AddItem(exchangeitem.m_nItemType, exchangeitem.m_nItemCount, exchangeitem.m_nValidTime, EItemAction_Add_MagicArrayExchange, 0, true, false, false);

    // log
    InsertIntoLog(betComponent.GetRoleAttr()->GetRoleID(), CMagicArrayLog::EMagicArrayAction_Exchange, 0);
    
    return EMagicArrayErrorFlag_Scuess;
}

void MagicArrayManager::SyncExchangeItemInfo(CBetComponent & betComponent)
{
    GameMsg_S2C_RequestExchangeItemResult resultmsg;
    resultmsg.m_nSex = (int)betComponent.GetRoleAttr()->GetSex();
    resultmsg.m_mapExchangeGroup = m_MagicArrayExchange.m_mapRefreshGroup;
    betComponent._SendPlayerMsg(&resultmsg);
}

int MagicArrayManager::RefreshExchangeItem(CBetComponent & betComponent)
{
    // check money
    int nCurrencyType = 0, nCurrencyValue = 0;
    ConfigManager::Instance().GetMagicArrayConfigMgr().GetRefreshExchangeCost(nCurrencyType, nCurrencyValue);
    BUILD_CURRENCY_MAP_ONETYPE(currencyMap, nCurrencyType, nCurrencyValue);
    if (!betComponent.GetRoleAttr()->CanBill(currencyMap))
    {
        return EMagicArrayErrorFlag_RefreshNotEnoughMoney;
    }

    // deduct money
    betComponent.GetRoleAttr()->Bill(currencyMap, EChangeBillCause_Del_MagicArrayRefreshExchaneg, EChangeBindBillCause_Del_MagicArrayRefreshExchange, EChangeMoneyCause_Del_MagicArrayCall);    
//     int a[3] = { (int)EChangeBillCause_Del_MagicArrayRefreshExchaneg, (int)EChangeMoneyCause_Del_MagicArrayCall, (int)EChangeBindBillCause_Del_MagicArrayRefreshExchange };
//     std::vector<int> vecCause(a, a + 3);
//     betComponent.GetRoleAttr()->DeductCurrency((EMallCurrencyType)nCurrencyType, nCurrencyValue, vecCause);

    // refresh exchange
    ESyncToDBType eType = m_MagicArrayExchange.m_nRefreshTime ==  0 ? ESyncToDBType_Insert : ESyncToDBType_Update;
    m_MagicArrayExchange.m_mapRefreshGroup.clear();
    ConfigManager::Instance().GetMagicArrayConfigMgr().RefreshExchangeGroup(m_MagicArrayExchange.m_mapRefreshGroup);
    m_MagicArrayExchange.m_nRefreshTime = (unsigned int)time(NULL);

    // sync to db
    SynceMagicArrayExchangeToDB(betComponent.GetRoleAttr()->GetRoleID(), eType);

    // sync to client
    GameMsg_S2C_RequestExchangeItemResult resultmsg;
    resultmsg.m_nSex = (int)betComponent.GetRoleAttr()->GetSex();
    resultmsg.m_mapExchangeGroup = m_MagicArrayExchange.m_mapRefreshGroup;
    betComponent._SendPlayerMsg(&resultmsg);

    // log
    InsertIntoLog(betComponent.GetRoleAttr()->GetRoleID(), CMagicArrayLog::EMagicArrayAction_RefreshExchange, 0);

    return EMagicArrayErrorFlag_Scuess;
}




