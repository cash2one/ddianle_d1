#include "BetComponent.h"
#include "../util/CommonFunc.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/ChatManager.h"
#include "../mall/CurrencyType.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../socket/Formatter.h"
#include "../item/ItemMsgDefine.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "MagicArray/GameMsg_CS_MagicLuckDraw.h"
#include "./MagicArray/MagicArrayManager.h"
#include "./MagicArray/MagicArrayDataManager.h"
#include "../quest/EntityQuestNewComponent.h"

#define LUCKDRAW_UPDATE_INTERVAL    2000    // ms
#define LUCKDRAW_ANNOUNCE_DELAYTIME 15      // s
#define MAX_DO_LUCKDRAW_COUNT       10


CBetComponent::CBetComponent(void)
    : CEntityComponent(true, true)
    , m_pRoleNetCOM(NULL)
    , m_pRoleAttrCOM(NULL)
    , m_pRoleItemCOM(NULL)
    , m_pQuestCOM(NULL)
    , m_bIsOpening(false)
    , m_nLuckInterval(0)
    , m_bMagicArrayOpen(false)
{
}

CBetComponent::~CBetComponent(void)
{
}

void CBetComponent::OnLogin()
{
    m_bMagicArrayOpen = CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_MagicArray);
}

void CBetComponent::OnLogout()
{
}

void CBetComponent::OnUpdate(const unsigned long& lTimeElapsed)
{
    m_nLuckInterval += (unsigned int)lTimeElapsed;
    if (m_nLuckInterval >= LUCKDRAW_UPDATE_INTERVAL)
    {
        m_nLuckInterval -= LUCKDRAW_UPDATE_INTERVAL;
    
        // magic array
        if (m_bMagicArrayOpen)
        {
            std::list<unsigned int> listMagicArrayID;
            m_MagicArrayManager.CheckCanFreeCall(listMagicArrayID);
            std::list<unsigned int>::iterator it = listMagicArrayID.begin();
            for (; it != listMagicArrayID.end(); ++it)
            {
                std::map<unsigned int, bool>::iterator itr = m_mapNoticeFreeCall.find(*it);
                if (itr != m_mapNoticeFreeCall.end())
                {
                    if (itr->second)
                    {
                        continue;
                    }
                }

                GameMsg_S2C_NoticeCanFreeCall callmsg;
                callmsg.m_nMagicArrayID = *it;
                m_pRoleNetCOM->SendPlayerMsg(&callmsg);
                m_mapNoticeFreeCall[*it] = true;
            }
        }
    }
}

void CBetComponent::Start()
{
    m_pRoleNetCOM = GetComponent<CRoleNet>();
    m_pRoleAttrCOM = GetComponent<CRoleAttribute>();
    m_pRoleItemCOM = GetComponent<CRoleItem>();
    m_pQuestCOM = GetComponent<CRoleQuestNew>();
}

bool CBetComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    m_MagicArrayManager.CreateFromDB(m_pRoleAttrCOM->GetRoleID(), pRoleInforDB->m_mapMagicArrayRecord, 
            pRoleInforDB->m_mapMagicArrayExchangeRecord);
    return true;
}

bool CBetComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    return m_MagicArrayManager.PacketToCache(pRoleInfoCache->m_mapMagicArrayRecord, pRoleInfoCache->m_mapMagicArrayExchangeRecord);
}


void CBetComponent::RegComponentNetMsgMap()
{
    // 魔法幸运阵
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestMagicArrayConfig);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PreViewAllReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestExchangeItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RefreshExchangeItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MagicCall); 
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MagicArrayExchange);

    RegMsgProcessor(MSG_C2S_RequestMagicArrayConfig, &CBetComponent::OnGetMagicArrayInfo);
    RegMsgProcessor(MSG_C2S_PreviewAllReward, &CBetComponent::OnPreviewAllReward);
    RegMsgProcessor(MSG_C2S_RequestExchangeConfig, &CBetComponent::OnGetExchangeItemInfo);
    RegMsgProcessor(MSG_C2S_RequestExchange, &CBetComponent::OnExchange);
    RegMsgProcessor(MSG_C2S_RefreshExchangeItem, &CBetComponent::OnRefreshExchangeItem);
   // RegMsgProcessor(MSG_C2S_GetMagicLuckDrawInfo, &CBetComponent::OnGetMagicLuckDrawInfo);
    RegMsgProcessor(MSG_C2S_MagicCall, &CBetComponent::OnMagicCall);
}


void CBetComponent::_SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNetCOM != NULL && pMsg != NULL)
        m_pRoleNetCOM->SendPlayerMsg(pMsg);
}

CEntityAttributeComponent * CBetComponent::GetRoleAttr()
{
    return m_pRoleAttrCOM;
}

CEntityItemComponent * CBetComponent::GetRoleItem()
{
	return m_pRoleItemCOM;
}

void CBetComponent::OnPreviewAllReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PreViewAllReward c2smsg = (GameMsg_C2S_PreViewAllReward&)msg;

    GameMsg_S2C_PreViewAllRewardResult resultmsg;
    resultmsg.m_nMagicArrayID = c2smsg.m_nMagicArrayID;
    resultmsg.m_nGroupID = c2smsg.m_nGroupID;
    resultmsg.m_nPage = c2smsg.m_nPage;
    resultmsg.m_nPageCount = c2smsg.m_nPageCount;
    resultmsg.m_nSex = (int)m_pRoleAttrCOM->GetSex();
    m_pRoleNetCOM->SendPlayerMsg(&resultmsg);
}

void CBetComponent::OnGetMagicArrayInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    m_MagicArrayManager.SyncMagicArrayInfo(*this);
}

void CBetComponent::OnRefreshExchangeItem(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    int nError = m_MagicArrayManager.RefreshExchangeItem(*this);
    if (nError != EMagicArrayErrorFlag_Scuess)
    {
        GameMsg_S2C_RequestExchangeItemFail failmsg;
        failmsg.m_nError = nError;
        m_pRoleNetCOM->SendPlayerMsg(&failmsg);
    }
}

void CBetComponent::OnGetExchangeItemInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    m_MagicArrayManager.SyncExchangeItemInfo(*this);
}

void CBetComponent::OnExchange(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_MagicArrayExchange & c2smsg = (GameMsg_C2S_MagicArrayExchange&)msg;
    int nError = m_MagicArrayManager.ExchangeItem(*this, c2smsg.m_nExchangeID);

    GameMsg_S2C_MagicArrayExchangeResult resultmsg;
    resultmsg.m_nErrorFlag = nError;
    m_pRoleNetCOM->SendPlayerMsg(&resultmsg);
}

void CBetComponent::OnMagicCall(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_MagicCall & c2smsg = (GameMsg_C2S_MagicCall&)msg;

    int nError = 0;
    CItem fixedItem;
    std::list<CallReward> listRewardItem;
    if (c2smsg.m_nCallType == (unsigned short)EMagicArrayCallType_Once)
    {
        nError = m_MagicArrayManager.OnceMagicCall(*this, c2smsg.m_nMagicArrayID, fixedItem, listRewardItem);

        // set update falg
        m_mapNoticeFreeCall[c2smsg.m_nMagicArrayID] = false;
    }
    else
    {
        nError = m_MagicArrayManager.TenMagicCall(*this, c2smsg.m_nMagicArrayID, fixedItem, listRewardItem);
    }

    do
    { 
        if (nError != EMagicArrayErrorFlag_Scuess)
            break; // 没成功不算

        MagicArrayConfig* config = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(c2smsg.m_nMagicArrayID);
        ENSURE_WITH_LOG_CMD(NULL != config, break, "error to get magicArrayID[%d] config.", c2smsg.m_nMagicArrayID);

        int counts = (c2smsg.m_nCallType == (unsigned short)EMagicArrayCallType_Once ? 1 : 10);
        if (EMallCurrencyType_Money == config->m_nCurrencyType)
        {
            m_pQuestCOM->OnUseMoneyMagicArray(counts);
        }
        else if (EMallCurrencyType_MCoin == config->m_nCurrencyType)
        {
            m_pQuestCOM->OnUseCoinMagicArray(counts);
        }

        // 通知activeness模块
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("magicarray_call"));
        NotifyAll(param);

    } while (0);

    // sync to client
    GameMsg_S2C_MagicCallResult resultmsg;
    resultmsg.m_nErrorFlag = nError;
    resultmsg.m_nMagicArrayID = c2smsg.m_nMagicArrayID;
    resultmsg.m_nFixedItemType = fixedItem.m_nItemType;
    resultmsg.m_nFixedItemCount = fixedItem.m_nItemCount;
    resultmsg.m_listRewardItem = listRewardItem;
    resultmsg.m_nLeftCDTime = m_MagicArrayManager.GetCallCDLeftTime(c2smsg.m_nMagicArrayID);
    m_pRoleNetCOM->SendPlayerMsg(&resultmsg);
}

void CBetComponent::OnGMRollMagicArrayReward(unsigned int nMagicArrayID, unsigned int nGmType, unsigned int nRollCount)
{
    MagicArrayConfig * pMagicArrayConfig = ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayConfig(nMagicArrayID);
    if (pMagicArrayConfig == NULL)
    {
        return;
    }

    if (nGmType == 1)
    {
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤类型roleid = %u, call type = %d, count = %d*********************",
            m_pRoleAttrCOM->GetRoleID(), 1, nRollCount);

        unsigned int nTempCount = 0;
        for (unsigned int i = 0; i < nRollCount; ++i)
        {
            std::list<MagicArrayReward> listReward;
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(),
                pMagicArrayConfig->m_listOnceCallRate, listReward);
            
            nTempCount++;
            std::list<MagicArrayReward>::iterator it = listReward.begin();
            for (; it != listReward.end(); ++it)
            {
                WriteLog(LOGLEVEL_DEBUG, "index = %u, 物品信息 itmeid = %u, group = %d,  count = %d, time = %d",
                    i + 1, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
            }
        }
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤结束*********************");
    }
    else if(nGmType == 2)
    {
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤类型roleid = %u, call type = %d, count = %d*********************",
            m_pRoleAttrCOM->GetRoleID(), 2, nRollCount);

        for (unsigned int i = 0; i < nRollCount; ++i)
        {
            std::list<MagicArrayReward> listReward;
            for (unsigned int j = 0; j < pMagicArrayConfig->m_nFirstTenCallRateCount; ++j)
            {
                listReward.clear();
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(), pMagicArrayConfig->m_listFirstTenCallRate, listReward);
                std::list<MagicArrayReward>::iterator it = listReward.begin();
                for (; it != listReward.end(); ++it)
                {
                    WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 是否是概率A = %d， 物品信息 itmeid = %u, groud = %d, count = %d, time = %d",
                        i+1, 1, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
                }
            }

            for (unsigned int j = 0; j < pMagicArrayConfig->m_nSecondTenCallRateCount; ++j)
            {
                listReward.clear();
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(), pMagicArrayConfig->m_listSecondTenCallRate, listReward);
                std::list<MagicArrayReward>::iterator it = listReward.begin();
                for (; it != listReward.end(); ++it)
                {
                    WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 是否是概率A = %d， 物品信息 itmeid = %u, groud = %d, count = %d, time = %d",
                        i+1, 0, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
                }
            }
        }
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤结束*********************");
    }
    else if (nGmType == 3)
    {
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤类型roleid = %u, call type = %d, count = %d*********************",
            m_pRoleAttrCOM->GetRoleID(), 3, nRollCount);

        for (unsigned int i = 0; i < nRollCount; ++i)
        {
            std::list<MagicArrayReward> listReward;
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(),
                pMagicArrayConfig->m_listFirstRollRate, listReward);

            std::list<MagicArrayReward>::iterator it = listReward.begin();
            for (; it != listReward.end(); ++it)
            {
                WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 物品信息 itmeid = %u, group = %d, count = %d, time = %d",
                     i+1, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
            }
        }
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤结束*********************");
    }
    else if (nGmType == 4)
    {
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤类型roleid = %u, call type = %d, count = %d*********************",
            m_pRoleAttrCOM->GetRoleID(), 4, nRollCount);

        for (unsigned int i = 0; i < nRollCount; ++i)
        {
            std::list<MagicArrayReward> listReward;
            ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(),
                pMagicArrayConfig->m_listAccumulatedRollRate, listReward);

            std::list<MagicArrayReward>::iterator it = listReward.begin();
            for (; it != listReward.end(); ++it)
            {
                WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 物品信息 itmeid = %u, group = %d, count = %d, time = %d",
                    i + 1, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
            }
        }
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤结束*********************");
    }
    else if (nGmType == 5)
    {
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤类型roleid = %u, call type = %d, count = %d*********************",
            m_pRoleAttrCOM->GetRoleID(), 5, nRollCount);

        for (unsigned int i = 0; i < nRollCount; ++i)
        {
            std::list<MagicArrayReward> listReward;
            for (unsigned int j = 0; j < 1; ++j)
            {
                listReward.clear();
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(), pMagicArrayConfig->m_listAccumulatedRollRate, listReward);
                std::list<MagicArrayReward>::iterator it = listReward.begin();
                for (; it != listReward.end(); ++it)
                {
                    WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 是否是概率A = %d， 物品信息 itmeid = %u, groud = %d, count = %d, time = %d",
                        i + 1, 2000, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
                }
            }

            for (unsigned int j = 0; j < pMagicArrayConfig->m_nSecondTenCallRateCount; ++j)
            {
                listReward.clear();
                ConfigManager::Instance().GetMagicArrayConfigMgr().RollMagicArrayReward(nMagicArrayID, (int)m_pRoleAttrCOM->GetSex(), pMagicArrayConfig->m_listSecondTenCallRate, listReward);
                std::list<MagicArrayReward>::iterator it = listReward.begin();
                for (; it != listReward.end(); ++it)
                {
                    WriteLog(LOGLEVEL_DEBUG, "MagicArray: index = %u, 是否是概率A = %d， 物品信息 itmeid = %u, groud = %d, count = %d, time = %d",
                        i + 1, 0, it->m_Item.m_nItemType, it->m_nGroupID, it->m_Item.m_nItemCount, it->m_Item.m_nValidTime);
                }
            }
        }
        WriteLog(LOGLEVEL_DEBUG, "MagicArray: ************** 召唤结束*********************");
    }
}

