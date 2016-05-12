#include "EntityCheckInComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../util/CommonFunc.h"
#include "../lua/InitLua.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../mail/MailMgr.h"
#include "../item/ItemProcess.h"
#include "../../datastructure/Localization.h"

#include "../../datastructure/datainfor/ConfigManager.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../vip/EntityVIPComponent.h"

#include "GameMsg_C2S_Check.h"
#include "GameMsg_C2S_GetCheckInInfo.h"
#include "GameMsg_S2C_CheckResult.h"
#include "GameMsg_S2C_GetCheckInInfoResult.h"
#include "../../datastructure/datainfor/ItemConfigManager.h"



CEntityCheckInComponent::CEntityCheckInComponent()
:CEntityComponent(true, true)
, m_pRoleAttr(NULL)
, m_pRoleNet(NULL)
, m_pRoleChat(NULL)
, m_pRoleItem(NULL)
, m_pRoleAch(NULL)
, m_pRoleQuest(NULL)
, m_pRoleVip(NULL)
{
}

CEntityCheckInComponent::~CEntityCheckInComponent()
{
}

void CEntityCheckInComponent::Start()
{
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleChat = GetComponent<CRoleChat>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleAch = GetComponent<CRoleAchievement>();
    m_pRoleQuest = GetComponent<CRoleQuest>();
    m_pRoleVip = GetComponent<CRoleVIP>();
}

void CEntityCheckInComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Check);         // 签到
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCheckInInfo);// N天奖励

    RegMsgProcessor(MSG_C2S_Check, &CEntityCheckInComponent::OnCheck);
    RegMsgProcessor(MSG_C2S_GetCheckInInfo, &CEntityCheckInComponent::OnGetCheckInInfo);
}

bool CEntityCheckInComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    if (pRoleInforDB != NULL)
    {
        m_CheckInInfo = pRoleInforDB->checkInInfo;
    }

    return true;
}

bool CEntityCheckInComponent::PacketToDB(ROLEINFO_DB*pRoleInforDB) const
{
    if (pRoleInforDB != NULL)
    {
        pRoleInforDB->checkInInfo = m_CheckInInfo;

        return true;
    }

    return false;
}

bool CEntityCheckInComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->checkInInfo = m_CheckInInfo;

    return true;
}

void CEntityCheckInComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityCheckInComponent::OnLogin()
{
}

void CEntityCheckInComponent::OnLogout()
{
}

void CEntityCheckInComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
}

void CEntityCheckInComponent::OnEvent(CComponentEvent& refEvent)
{
}

void CEntityCheckInComponent::GMClearCheckInData()
{
    m_CheckInInfo.m_nDayIndex = 0;
    m_CheckInInfo.m_nCheckInTime = 0;
    m_CheckInInfo.m_nVipOutTimeDayIndex = 0;

    SaveCheckInInfoToDB();
}

void CEntityCheckInComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNet != NULL && pMsg != NULL)
    {
        m_pRoleNet->SendPlayerMsg(pMsg);
    }
}

void CEntityCheckInComponent::OnCheck(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    ResetByMonthChange();

    GameMsg_C2S_Check & c2smsg = (GameMsg_C2S_Check&)msg;
    int nError = CheckIn(c2smsg.m_nDayIndex);
    if (nError != ECheckInErrorMsg_Success)
    {
        GameMsg_S2C_CheckInFail failmsg;
        failmsg.m_nFailFlag = nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_CheckInSuccess sucmsg;
        sucmsg.m_nDayIndex = c2smsg.m_nDayIndex;
        CCheckInConfig * pCheckInConfig = CCheckInDataManager::Instance().GetCheckInConfig(c2smsg.m_nDayIndex);
        if (pCheckInConfig != NULL)
        {
            int nVipRate = 1;
            if (m_pRoleVip->IsVIP() && m_pRoleVip->VIPLevel() >= (int)pCheckInConfig->m_nVipRewardLevel
                && pCheckInConfig->m_nVipRewardLevel > 0)
            {
                nVipRate = pCheckInConfig->m_nVipRewardRate;
            }

            sucmsg.m_nMoney = pCheckInConfig->m_nMoney*nVipRate;
            sucmsg.m_nBindCoin = pCheckInConfig->m_nBindCoin*nVipRate;
            sucmsg.m_ItemReward = m_pRoleAttr->GetSex() == ESexType_Male ? pCheckInConfig->m_maleItemReward : pCheckInConfig->m_femaleItemReward;

            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                CItem maleitem(pCheckInConfig->m_maleItemReward);
                ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_maleItemReward.m_nItemType);
                if (pItemInfo != NULL)
                {
                    if (pItemInfo->IsEquip())
                    {
                        if (nVipRate > 1)
                        {
                            maleitem.m_nValidTime = -1;
                        }
                    }
                    else
                    {
                        maleitem.m_nItemCount = (itemcount_t)(maleitem.m_nItemCount*nVipRate);
                    }
                    sucmsg.m_ItemReward = maleitem;
                }
            }
            else
            {
                CItem femaleitem(pCheckInConfig->m_femaleItemReward);
                ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_femaleItemReward.m_nItemType);
                if (pItemInfo != NULL)
                {
                    if (pItemInfo->IsEquip())
                    {
                        if (nVipRate > 1)
                        {
                            femaleitem.m_nValidTime = -1;
                        }
                    }
                    else
                    {
                        femaleitem.m_nItemCount = (itemcount_t)(femaleitem.m_nItemCount*nVipRate);
                    }

                    sucmsg.m_ItemReward = femaleitem;
                }
            }
        }
        SendPlayerMsg(&sucmsg);

        // 通知其它系统
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("checkin"));
        NotifyAll(param);
    }
}

bool CEntityCheckInComponent::ResetByMonthChange()
{
    time_t tDayTime = m_CheckInInfo.m_nCheckInTime;
    tm * pDayTime = localtime(&tDayTime);
    tm tmDayTime(*pDayTime);

    time_t tNowTime = time(NULL);
    tm * pNowTime = localtime(&tNowTime);
    tm tmNowTime(*pNowTime);

    if (tmDayTime.tm_mon + tmDayTime.tm_year*12 < tmNowTime.tm_mon + tmNowTime.tm_year*12)
    {
        CCheckInDataManager::Instance().SetCurrentMonthConfig(tmNowTime.tm_mon + 1);

        if (m_CheckInInfo.m_nDayIndex == 0 || m_CheckInInfo.m_nCheckInTime == 0)
        {
            return false;
        }
        else
        {
            m_CheckInInfo.m_nDayIndex = 0;
            m_CheckInInfo.m_nCheckInTime = 0;
            m_CheckInInfo.m_nVipOutTimeDayIndex = 0;
            return true;
        }
    }

    return false;
}

void CEntityCheckInComponent::VipExpired()
{
    if (m_CheckInInfo.m_nDayIndex > 0 && m_CheckInInfo.m_nVipOutTimeDayIndex <= 0)
    {
        m_CheckInInfo.m_nVipOutTimeDayIndex = m_CheckInInfo.m_nDayIndex;

        SaveCheckInInfoToDB();
    }
}

bool CEntityCheckInComponent::IsTodayCheckIn()
{
    return IsSameDay(m_CheckInInfo.m_nCheckInTime, time(NULL));
}


void CEntityCheckInComponent::SendReissRewardByMail(CCheckInConfig * pCheckInConfig)
{
    int nVipRate = pCheckInConfig->m_nVipRewardRate - 1;
    nVipRate = nVipRate > 1 ? nVipRate : 1;

    // item reward
    if (m_pRoleAttr->GetSex() == ESexType_Male)
    {
        CItem maleitem(pCheckInConfig->m_maleItemReward);
        ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_maleItemReward.m_nItemType);
        if (pItemInfo != NULL)
        {
            if (pItemInfo->IsEquip())
            {
                if (nVipRate >= 1)
                {
                    maleitem.m_nValidTime = -1;
                }
            }
            else
            {
                maleitem.m_nItemCount = (itemcount_t)(maleitem.m_nItemCount*nVipRate);
            }

            std::string strMailTitle = CLocalization::Instance().GetString("CheckIn_Mail_Item_Title");
            std::string strMailContent = CLocalization::Instance().GetString("CheckIn_Mail_Item_Content");
            CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_CheckIn, strMailTitle, strMailContent, maleitem );
        }
    }
    else
    {
        CItem femaleitem(pCheckInConfig->m_femaleItemReward);
        ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_femaleItemReward.m_nItemType);
        if (pItemInfo != NULL)
        {
            if (pItemInfo->IsEquip())
            {
                if (nVipRate >= 1)
                {
                    femaleitem.m_nValidTime = -1;
                }
            }
            else
            {
                femaleitem.m_nItemCount = (itemcount_t)(femaleitem.m_nItemCount*nVipRate);
            }

            std::string strMailTitle = CLocalization::Instance().GetString("CheckIn_Mail_Item_Title");
            std::string strMailContent = CLocalization::Instance().GetString("CheckIn_Mail_Item_Content");
            CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_CheckIn, strMailTitle, strMailContent, femaleitem );
        }
    }

    // money reward
    if (pCheckInConfig->m_nMoney > 0)
    {
        m_pRoleAttr->ChangeMoney(pCheckInConfig->m_nMoney*nVipRate, EChangeMoneyCause_Add_CheckIn, 0);

        std::string strMailTitle = CLocalization::Instance().GetString("CheckIn_Mail_Money_Title");
        std::string strContent;
        SafeFormat(strContent, CLocalization::Instance().GetString("CheckIn_Mail_Money_Content"), pCheckInConfig->m_nMoney*nVipRate);
        CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_CheckIn, strMailTitle, strContent );
    }

    // bindcoin reward
    if (pCheckInConfig->m_nBindCoin > 0)
    {
        m_pRoleAttr->ChangeBindBill(pCheckInConfig->m_nBindCoin*nVipRate, EChangeBindBillCause_Add_CheckIn);

        std::string strMailTitle = CLocalization::Instance().GetString("CheckIn_Mail_BindCoin_Title");
        std::string strMailContent;
        SafeFormat(strMailContent, CLocalization::Instance().GetString("CheckIn_Mail_BindCoin_Content"), pCheckInConfig->m_nBindCoin*nVipRate);
        CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_CheckIn, strMailTitle, strMailContent );
    }
}

void CEntityCheckInComponent::ReissueRewardByVipOpen(int nVipLevel)
{
    bool bSendMailReward = false;
    unsigned int nDayIndex = m_CheckInInfo.m_nDayIndex;
    CCheckInConfig * pCheckInConfig = CCheckInDataManager::Instance().GetCheckInConfig(m_CheckInInfo.m_nDayIndex);
    do
    {
        if (pCheckInConfig == NULL)
        {
            break;
        }

        if (m_CheckInInfo.m_nVipOutTimeDayIndex > 0 && pCheckInConfig->m_nDayIndex > m_CheckInInfo.m_nVipOutTimeDayIndex
            && pCheckInConfig->m_nVipRewardLevel <= (unsigned int)nVipLevel && pCheckInConfig->m_nVipRewardLevel > 0)
        {
            SendReissRewardByMail(pCheckInConfig);
            bSendMailReward = true;
            _LogRoleCheckIn(m_pRoleAttr->GetRoleID(), CRoleCheckInLog::ECheckInAction_Recheck, pCheckInConfig->m_nDayIndex, m_pRoleVip->IsVIP(), m_pRoleVip->VIPLevel());
        }
        else if (m_CheckInInfo.m_nVipOutTimeDayIndex == 0 && pCheckInConfig->m_nVipRewardLevel <= (unsigned int)nVipLevel
            && pCheckInConfig->m_nVipRewardLevel > 0)
        {
            SendReissRewardByMail(pCheckInConfig);
            bSendMailReward = true;
            _LogRoleCheckIn(m_pRoleAttr->GetRoleID(), CRoleCheckInLog::ECheckInAction_Recheck, pCheckInConfig->m_nDayIndex, m_pRoleVip->IsVIP(), m_pRoleVip->VIPLevel());
        }

        pCheckInConfig = CCheckInDataManager::Instance().GetLastCheckInConfig(nDayIndex);
        if (pCheckInConfig != NULL)
        {
            nDayIndex = pCheckInConfig->m_nDayIndex;
        }

    } while (pCheckInConfig != NULL);

    if (m_CheckInInfo.m_nVipOutTimeDayIndex > 0)
    {
        m_CheckInInfo.m_nVipOutTimeDayIndex = 0;
        SaveCheckInInfoToDB();
    }

    if (bSendMailReward)
    {
        GameMsg_S2C_CheckInMailRewardNotice noticemsg;
        SendPlayerMsg(&noticemsg);
    }
}

void CEntityCheckInComponent::ReissueRewardByVipLevelChange(int nOldVipLevel, int nNewVipLevel)
{
    bool bSendMailReward = false;
    unsigned int nDayIndex = m_CheckInInfo.m_nDayIndex;
    CCheckInConfig * pCheckInConfig = CCheckInDataManager::Instance().GetCheckInConfig(m_CheckInInfo.m_nDayIndex);
    do
    {
        if (pCheckInConfig == NULL)
        {
            break;
        }

        if (pCheckInConfig->m_nVipRewardLevel > (unsigned int)nOldVipLevel
            && pCheckInConfig->m_nVipRewardLevel <= (unsigned int)nNewVipLevel
            && pCheckInConfig->m_nVipRewardRate > 1)
        {
            SendReissRewardByMail(pCheckInConfig);
            bSendMailReward = true;
            _LogRoleCheckIn(m_pRoleAttr->GetRoleID(), CRoleCheckInLog::ECheckInAction_Recheck, pCheckInConfig->m_nDayIndex, m_pRoleVip->IsVIP(), m_pRoleVip->VIPLevel());
        }

        pCheckInConfig = CCheckInDataManager::Instance().GetLastCheckInConfig(nDayIndex);
        if (pCheckInConfig != NULL)
        {
            nDayIndex = pCheckInConfig->m_nDayIndex;
        }

    } while (pCheckInConfig != NULL);

    if (bSendMailReward)
    {
        GameMsg_S2C_CheckInMailRewardNotice noticemsg;
        SendPlayerMsg(&noticemsg);
    }
}

int CEntityCheckInComponent::CheckIn(unsigned int nDayIndex)
{
    CCheckInConfig * pCheckInConfig = CCheckInDataManager::Instance().GetCheckInConfig(nDayIndex);
    if (pCheckInConfig == NULL)
    {
        return ECheckInErrorMsg_ConfigError;
    }

    // have checkin
    if (m_CheckInInfo.m_nDayIndex >= nDayIndex)
    {
        return ECheckInErrorMsg_HaveCheckIn;
    }

    // index 
    CCheckInConfig * pNextCheckInConfig = CCheckInDataManager::Instance().GetNextCheckInConfig(m_CheckInInfo.m_nDayIndex);
    if (pNextCheckInConfig == NULL || pNextCheckInConfig->m_nDayIndex != nDayIndex)
    {
        return ECheckInErrorMsg_DayIndexError;
    }

    // vip add
    int nVipRate = 1;
    if (m_pRoleVip->IsVIP() && m_pRoleVip->VIPLevel() >= (int)pCheckInConfig->m_nVipRewardLevel
        && pCheckInConfig->m_nVipRewardLevel >= 1)
    {
        nVipRate = pCheckInConfig->m_nVipRewardRate;
    }

    nVipRate = nVipRate > 1 ? nVipRate : 1;
    // item reward
    if (m_pRoleAttr->GetSex() == ESexType_Male)
    {
        CItem maleitem(pCheckInConfig->m_maleItemReward);
        ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_maleItemReward.m_nItemType);
        if (pItemInfo != NULL)
        {
            if (pItemInfo->IsEquip())
            {
                if (nVipRate > 1)
                {
                    maleitem.m_nValidTime = -1;
                }
            }
            else
            {
                maleitem.m_nItemCount = (itemcount_t)(maleitem.m_nItemCount*nVipRate);
            }

            std::list<CItem> listitem;
            listitem.push_back(maleitem);
            CItemProcess::AddItems(*(CRoleEntity *)Entity(), listitem, EItemAction_Add_ChekcInReward, 0, true);
        }
    }
    else
    {
        CItem femaleitem(pCheckInConfig->m_femaleItemReward);
        ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pCheckInConfig->m_femaleItemReward.m_nItemType);
        if (pItemInfo != NULL)
        {
            if (pItemInfo->IsEquip())
            {
                if (nVipRate > 1)
                {
                    femaleitem.m_nValidTime = -1;
                }
            }
            else
            {
                femaleitem.m_nItemCount = (itemcount_t)(femaleitem.m_nItemCount*nVipRate);
            }

            std::list<CItem> listitem;
            listitem.push_back(femaleitem);
            CItemProcess::AddItems(*(CRoleEntity *)Entity(), listitem, EItemAction_Add_ChekcInReward, 0, true);
        }
    }

    // money reward
    m_pRoleAttr->ChangeMoney(pCheckInConfig->m_nMoney*nVipRate, EChangeMoneyCause_Add_CheckIn, 0);

    // bindcoin reward
    m_pRoleAttr->ChangeBindBill(pCheckInConfig->m_nBindCoin*nVipRate, EChangeBindBillCause_Add_CheckIn);

    // set check info
    m_CheckInInfo.m_nDayIndex = nDayIndex;
    m_CheckInInfo.m_nCheckInTime = (unsigned int)time(NULL);

    // save to db
    SaveCheckInInfoToDB();

    // log
    _LogRoleCheckIn(m_pRoleAttr->GetRoleID(), CRoleCheckInLog::ECheckInAction_Check, nDayIndex, m_pRoleVip->IsVIP(), m_pRoleVip->VIPLevel());

    return ECheckInErrorMsg_Success;
}

void CEntityCheckInComponent::OnGetCheckInInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (ResetByMonthChange())
    {
        SaveCheckInInfoToDB();
    }

    GameMsg_S2C_GetCheckInInfoResult resultmsg;
    resultmsg.m_CheckInInfo = m_CheckInInfo;
    resultmsg.m_cSex = m_pRoleAttr->GetSex();

    SendPlayerMsg(&resultmsg);
}

unsigned int CEntityCheckInComponent::GetCurrentCheckCount()
{
    return CCheckInDataManager::Instance().GetHaveCheckInCount(m_CheckInInfo.m_nDayIndex);
}

void CEntityCheckInComponent::SaveCheckInInfoToDB()
{
    CCheckInInfo * pCheckInInfo = new CCheckInInfo(m_CheckInInfo);
    CDelHelper delHelper(pCheckInInfo);
    AddQuery(QUERY_UpdateCheckInInfo, m_pRoleAttr->GetRoleID(), pCheckInInfo, 0, NULL, &delHelper);
}

void CEntityCheckInComponent::_LogRoleCheckIn(unsigned int nRoleID, CRoleCheckInLog::ECheckInAction eCheckInAction, unsigned int nDayIndex, bool bIsVip, unsigned int nVipLevel)
{
    CRoleCheckInLog * pLog = new CRoleCheckInLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eCheckInAction = eCheckInAction;
    pLog->m_nDayIndex = nDayIndex;
    pLog->m_bIsVip = bIsVip;
    pLog->m_nVipLevel = nVipLevel;
    AddQuery(QUERY_LogRoleCheckIn, 0, pLog);
}

