#include "EntityVIPComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "VIPStruct.h"
#include "VIPDataMgr.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../room/RoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../room/Room.h"
#include "../../datastructure/Localization.h"
#include "../item/ItemProcess.h"
#include "../../datastructure/Localization.h"
#include "VIPStruct.h"
#include "../chat/ChatManager.h"
#include "GameMsg_VIP.h"
#include "GameMsg_S2G_VIP.h"
#include "../player/GameMsg_Buff.h"
#include "../mall/Mall.h"
#include "../mall/EntityMallComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../roomLib/GameMsgRoomBroadcast.h"

void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CEntityVIPComponent::CEntityVIPComponent()
: CEntityComponent(true, true)
, m_pRoleAttr(NULL)
, m_pRoleNet(NULL)
, m_pRoleChat(NULL)
, m_pRoleItem(NULL)
, m_pRoleAch(NULL)
, m_pRoleQuest(NULL)
, m_pRoleMail(NULL)
, m_pRoleFriend(NULL)
, m_pRoleRoom(NULL)
, m_pRoleDanceGroup(NULL)
, m_pRoleCouple(NULL)
, m_pRolePhotoRoom(NULL)
, m_pRoleAmuseRoom(NULL)
, m_pRoleCheckIn(NULL)
, m_tLastSecond(0)
{
}

CEntityVIPComponent::~CEntityVIPComponent()
{
}

void CEntityVIPComponent::Start()
{
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleChat = GetComponent<CRoleChat>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleAch = GetComponent<CRoleAchievement>();
    m_pRoleQuest = GetComponent<CRoleQuest>();
    m_pRoleMail = GetComponent<CRoleMail>();
    m_pRoleFriend = GetComponent<CRoleFriend>();
    m_pRoleRoom = GetComponent<CRoleRoom>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleCheckIn = GetComponent<CRoleCheckIn>();
}

void CEntityVIPComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetVIPDailyReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestBuyVip);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendOpenVipFrom);

    RegMsgProcessor(MSG_C2S_GetVIPDailyReward, &CEntityVIPComponent::OnGetVIPDailyReward);
    RegMsgProcessor(MSG_C2S_RequestBuyVip, &CEntityVIPComponent::OnRequestBuyVIP);
    RegMsgProcessor(MSG_C2S_SendOpenVipFrom, &CEntityVIPComponent::OnSendOpenVipFrom);
}

bool CEntityVIPComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
    m_VIPInfo = pRoleInfoDB->VIPInfo;
    m_VIPInfo.m_nExp = m_VIPInfo.m_nExp;
    m_VIPInfo.m_bValid = m_VIPInfo.m_nExpiredTime > (long long)time(NULL);

    return true;
}

bool CEntityVIPComponent::PacketToDB(ROLEINFO_DB*pRoleInforDB) const
{
    if (pRoleInforDB != NULL) {
        pRoleInforDB->VIPInfo = m_VIPInfo;

        return true;
    }

    return false;
}

void CEntityVIPComponent::SerializeComponent(CParamPool &IOBuff)
{
    time_t nNow = time(NULL);
    IOBuff.AddBool(m_VIPInfo.m_nExpiredTime > (long long)nNow);
    IOBuff.AddUShort((unsigned short)m_VIPInfo.m_nLevel);
    IOBuff.AddInt(m_VIPInfo.m_nExp);
    IOBuff.AddInt64(m_VIPInfo.m_nExpiredTime);
    IOBuff.AddBool(m_VIPInfo.CanGetDailyReward());

    CVIPDataMgr::Instance().Serialize(IOBuff, m_pRoleAttr->GetSex());
}

void CEntityVIPComponent::OnLogin()
{
    if (!IsVIP())
    {
        OnVIPExpired();
    }
    else
    {
        VIPOnlineAnnounce();
    }
}
void CEntityVIPComponent::OnLogout()
{
}

void CEntityVIPComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
    time_t tNow = time(NULL);
    if (m_VIPInfo.m_nExpiredTime < (long long)tNow && m_VIPInfo.m_bValid) // VIP失效
    {
        m_VIPInfo.m_bValid = false;
        NotifyClientUpdateVIPInfo();
        OnVIPInfoChanged(m_VIPInfo.m_nLevel, m_VIPInfo.m_nLevel);
        OnVIPExpired();
    }
    else if (m_VIPInfo.m_nExpiredTime >(long long)tNow && !m_VIPInfo.m_bValid) // vip生效(修改服务器时间)
    {
        NotifyClientUpdateVIPInfo();
        OnVIPInfoChanged(0, 0);
        m_VIPInfo.m_bValid = true;
    }

    if (GetLocalHourOfDay(m_tLastSecond) == 23 && GetLocalHourOfDay(tNow) == 0)
    {
        NotifyClientUpdateVIPInfo();
    }
    m_tLastSecond = tNow;
}

void CEntityVIPComponent::OnEvent(CComponentEvent& refEvent)
{

}

void CEntityVIPComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (GetRoleNet() != NULL && pMsg != NULL) {
        GetRoleNet()->SendPlayerMsg(pMsg);
    }
}

void CEntityVIPComponent::UpdateVIPInfoToDB()
{
    CRoleVIPInfo *pVIPInfo = new CRoleVIPInfo(m_VIPInfo);
    CDelHelper delHelper(pVIPInfo);
    AddQuery(QUERY_SaveVIPInfo, GetRoleAttr()->GetRoleID(), pVIPInfo, 0, NULL, &delHelper);
}

void CEntityVIPComponent::UpdateVIPInfoToGroup()
{
    if (m_pRoleAttr != NULL) {
        GameMsg_S2G_UpdateVIPInfo msgUpdate;
        msgUpdate.m_nRoleID = m_pRoleAttr->GetRoleID();
        msgUpdate.m_bIsVIP = m_VIPInfo.IsVIP();
        msgUpdate.m_nVIPLevel = (unsigned short)m_VIPInfo.m_nLevel;
        ::SendMsg2GroupServer(&msgUpdate);
    }
}

void CEntityVIPComponent::SendVIPLevelUpReward()
{
    const CVIPLevelPrivInfo * pVIPPrivInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(m_VIPInfo.m_nLevel);
    if (pVIPPrivInfo != NULL)
    {
        const CVIPLevelUpReward & levelUpReward = pVIPPrivInfo->m_VIPLevelUpReward;
        std::list<CItem> listItem;
        if (m_pRoleAttr != NULL)
        {
            m_pRoleAttr->ChangeMoney(levelUpReward.m_nMoney, EChangeMoneyCause_Add_VIPLevelUp, 0);
            listItem = m_pRoleAttr->GetSex() == ESexType_Male ? levelUpReward.m_listMaleItem : levelUpReward.m_listFemaleItem;
        }
        if (!listItem.empty())
        {
            std::string strMailTitle = CLocalization::Instance().GetString("VIP_Level_Up_Rewrad_Mail_Title");
            std::string strMaileContent;
            SafeFormat(strMaileContent, CLocalization::Instance().GetString("VIP_Level_Up_Rewrad_Mail_Content"), m_VIPInfo.m_nLevel);

            CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listItem, EItemAction_Add_VIPLevelUp, 0, true, EMailType_VIPLevelUp, strMailTitle, strMaileContent);
        }
    }
}

void CEntityVIPComponent::LevelUpRewardNotification(int nLevel)
{
    const CVIPLevelPrivInfo * pVIPPrivInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(nLevel);
    if (pVIPPrivInfo != NULL) {
        const CVIPLevelUpReward & levelUpReward = pVIPPrivInfo->m_VIPLevelUpReward;
        std::list<CItem> listItem;
        if (m_pRoleAttr != NULL) {
            listItem = m_pRoleAttr->GetSex() == ESexType_Male ? levelUpReward.m_listMaleItem : levelUpReward.m_listFemaleItem;
        }
        if (!listItem.empty()) {
            GameMsg_S2C_VIPLevelUpReward msgLevelUp;
            msgLevelUp.m_nVIPLevel = (unsigned short)nLevel;
            SendPlayerMsg(&msgLevelUp);
        }
    }
}

void CEntityVIPComponent::VIPLevelUpAnnounce(int nVIPLevel)
{
    const CVIPLevelPrivInfo *pVIPPrivInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(nVIPLevel);
    if (pVIPPrivInfo != NULL)
    {
        std::string strContent;
        SafeFormat(strContent, CLocalization::Instance().GetString("VIP_Level_Up_Announce"),
            m_pRoleAttr->GetRoleName(), nVIPLevel);

        if (pVIPPrivInfo->m_nLevelUpAnnounce == 1)// 红字公告
        {
            CChatManager::Instance().SendAnnounce(true, false, strContent);
        }
        else if (pVIPPrivInfo->m_nLevelUpAnnounce == 2) // 走马灯公告
        {
            CChatManager::Instance().SendAnnounce(false, true, strContent);
        }
    }
}

void CEntityVIPComponent::VIPOnlineAnnounce()
{
    unsigned int nLastAnnounceTime = m_VIPInfo.m_nLastOnlineAnnounceTime;
    unsigned int nNow = (unsigned int)time(NULL);

    const CVIPLevelPrivInfo *pVIPPrivInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(m_VIPInfo.m_nLevel);
    if (pVIPPrivInfo != NULL && pVIPPrivInfo->m_bOnlineAnnounce
        && nNow - nLastAnnounceTime >= (unsigned int)pVIPPrivInfo->m_nOnlineAnnounceCoolDown)
    {
        std::string strContent;
        SafeFormat(strContent, CLocalization::Instance().GetString("VIP_Online_Announce"),
            m_VIPInfo.m_nLevel, m_pRoleAttr->GetRoleName());

        CChatManager::Instance().SendAnnounce(true, false, strContent);// 红字公告
        //CChatManager::Instance().SendAnnounce(false, true, strContent);// 走马灯公告

        m_VIPInfo.m_nLastOnlineAnnounceTime = nNow;
        AddQuery(QUERY_VIP_UpdateOnlineAnnounceTime, m_pRoleAttr->m_nRoleID, NULL, nNow, NULL);
    }
}

void CEntityVIPComponent::OnGetVIPDailyReward(GameMsg_Base & rMsg, CSlotPeer & rSlotPeer)
{
    if (IsVIP() && m_VIPInfo.CanGetDailyReward())
    {
        int nExpBonus = 0;
        int nBindMBill = 0;
        std::list<CItem> listItem;
        CVIPDataMgr::Instance().GetDailyExpBonus(m_VIPInfo.m_nLevel, nExpBonus, nBindMBill, listItem);
        m_VIPInfo.m_nLastGetRewardTime = (unsigned int)time(NULL);

        // 奖励
        ChangeVIPInfo(0, nExpBonus, ERoleVipExpCause_AddByDailyReward);
        m_pRoleAttr->ChangeBindBill(nBindMBill, EChangeBindBillCause_Add_VIPDailyReward);
        CItemProcess::AddItems(*(CRoleEntity *)Entity(), listItem, EItemAction_Add_VIPDailyReward, 0, true);

        GameMsg_S2C_GetVIPDailyRewardSuc msgSuc;
        msgSuc.m_nDailyExpBonus = nExpBonus;
        msgSuc.m_nBindMBill = nBindMBill;
        msgSuc.m_listItemReward = listItem;
        SendPlayerMsg(&msgSuc);

        // 通知观察者
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("vip_daily_reward"));
        NotifyAll(param);
    }
}

void CEntityVIPComponent::OnRequestBuyVIP(GameMsg_Base & rMsg, CSlotPeer & rSlotPeer)
{
    int nError = 0;
    std::string strError;

    itemtype_t nVIPCardID = ((GameMsg_C2S_RequestBuyVip &)rMsg).m_nVIPCardID;
    MallGood *pMallGoods = CMall::Instance().FindGoods(nVIPCardID);
    ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nVIPCardID);
    CEntityMallComponent *pMallComp = GetComponent<CRoleMall>();
    CEntityGeneComponent *pGeneComp = GetComponent<CRoleGene>();

    // 参数检查
    if (pMallGoods == NULL || pItemInfo == NULL || pMallGoods->m_nPrice <= 0
        || pMallComp == NULL || pGeneComp == NULL)
    {
        nError = GameMsg_S2C_RequestBuyVipRes::eBuyVipErrorType_NoVIPCard;
        strError = "BuyVipError_NoVIPCard";

        WriteLog(LOGLEVEL_ERROR, "[VIP]role[%u] buy vip[%d] error: NoVIPCard",
            m_pRoleAttr->GetRoleID(), (int)nVIPCardID);
    }
    else
    {
        bool bUseCoupon = (pMallGoods->m_nUseChangePercent < 100);
        CurrencyMap mapCostCurrency;
        CMall::Instance().InitCurrencyMap(mapCostCurrency);

        // 计算需要消费的金额
        int nRes = CMall::Instance().AddBuyGoods(mapCostCurrency, pMallGoods, 0, bUseCoupon);
        if (nRes != 0)
        {
            nError = GameMsg_S2C_RequestBuyVipRes::eBuyVipErrorType_NoVIPCard;
            strError = "BuyVipError_NoVIPCard";

            WriteLog(LOGLEVEL_ERROR, "[VIP]role[%u] buy vip[%d] error: NoVIPCard",
                m_pRoleAttr->GetRoleID(), (int)nVIPCardID);
        }
        // 账号余额是否足够
        else if (pMallComp->CanBill(mapCostCurrency) != 0
            || pMallComp->FinishBill(mapCostCurrency, EFinishBillFrom_Buy) != 0)
        {
            nError = GameMsg_S2C_RequestBuyVipRes::eBuyVipErrorType_NotEnoughBalance;
            strError = "BuyVipError_NotEnoughBalance";

            WriteLog(LOGLEVEL_ERROR, "[VIP]role[%u] buy vip[%d] error: NotEnoughBalance",
                m_pRoleAttr->GetRoleID(), (int)nVIPCardID);
        }
        else
        {
            // 添加VIP效果(gene)
            list<GeneInfo *> &listGene = pItemInfo->m_listGene;
            for (list<GeneInfo *>::iterator it = listGene.begin(); it != listGene.end(); ++it)
            {
                GeneInfo *pGene = (*it);
                if (pGene != NULL)
                {
                    bool bSuccess = pGeneComp->AddGene(pGene);
                    if (!bSuccess)
                    {
                        WriteLog(LOGLEVEL_ERROR, "[VIP]role[%u] buy vip[%d] error: add gene %d failed",
                            m_pRoleAttr->GetRoleID(), (int)nVIPCardID, (int)pGene->m_nGeneID);
                    }
                }
            }

            nError = 0;
            WriteLog(LOGLEVEL_DEBUG, "[VIP]role[%u] buy vip[%d] successfully",
                m_pRoleAttr->GetRoleID(), (int)nVIPCardID);
        }
    }

    // 消息响应
    GameMsg_S2C_RequestBuyVipRes response;
    response.m_nError = (unsigned char)nError;
    if (nError != 0)
    {
        response.m_strError = strError;
    }

    SendPlayerMsg(&response);
}

void CEntityVIPComponent::OnSendOpenVipFrom(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_SendOpenVipFrom &rSendFrom = dynamic_cast<GameMsg_C2S_SendOpenVipFrom &>(rMsg);

    m_listOpenVipFromType = rSendFrom.m_listOpenVipFromType;
}

void CEntityVIPComponent::OnUpdateVIPInfo(unsigned int nAddExp, unsigned int nAddTime)
{
    if (m_VIPInfo.m_nOpenTime == 0)
    {
        m_VIPInfo.m_nOpenTime = (unsigned int)time(NULL);
        LogVipOpen();
    }

    if (m_VIPInfo.m_nExpiredTime <= (long long)time(NULL))
    {
        // VIP开通后，回传给客户端
        SendOpenVipFrom();
    }

    ChangeVIPInfo(nAddTime, nAddExp, ERoleVipExpCause_AddByCard);
}

void CEntityVIPComponent::ChangeVIPInfo(unsigned int nAddTime, unsigned int nAddExp, ERoleVipExpCause eCause)
{
    bool bIsOpenAction = false;
    bool bIsExpiredToday = m_VIPInfo.IsExpiredToday();
    if (nAddTime > 0)
    {
        int nNow = (int)time(NULL);
        if (m_VIPInfo.m_nExpiredTime < (long long)nNow)
        {
            m_VIPInfo.m_nExpiredTime = (long long)nNow + (long long)nAddTime;
            bIsOpenAction = true;
        }
        else
        {
            m_VIPInfo.m_nExpiredTime += (long long)nAddTime;
        }

        LogVipDuration(nAddTime);
    }

    int nOldLevel = m_VIPInfo.m_nLevel;
    if (nAddExp > 0)
    {
        m_VIPInfo.m_nExp += nAddExp;
        LogVipExp(eCause, nAddExp);

        while (m_VIPInfo.m_nExp >= (int)CVIPDataMgr::Instance().GetExpLevelUpNeed(m_VIPInfo.m_nLevel)
            && m_VIPInfo.m_nLevel < CVIPDataMgr::Instance().GetMaxLevel())
        {
            int nExpNeed = CVIPDataMgr::Instance().GetExpLevelUpNeed(m_VIPInfo.m_nLevel);
            m_VIPInfo.m_nExp -= nExpNeed;
            ++m_VIPInfo.m_nLevel;
            SendVIPLevelUpReward();
            VIPLevelUpAnnounce(m_VIPInfo.m_nLevel);

            LogVipExp(ERoleVipExpCause_DelByLevelUp, -nExpNeed);
        }

        if (m_VIPInfo.m_nLevel > nOldLevel)
        {
            LevelUpRewardNotification(m_VIPInfo.m_nLevel);
            LogVipLevelUp(m_VIPInfo.m_nLevel);
            m_pRoleItem->UpdateStorageCapacity(m_VIPInfo.m_nLevel);
        }
    }

    if (nAddTime > 0 || nAddExp > 0)
    {
        m_VIPInfo.m_bValid = true;
        OnVIPInfoChanged(nOldLevel, m_VIPInfo.m_nLevel);
        NotifyClientUpdateVIPInfo();
        UpdateVIPInfoToDB();

        // 开启vip
        if (bIsOpenAction)
        {
            m_pRoleCheckIn->ReissueRewardByVipOpen(m_VIPInfo.m_nLevel);
        }
        else if (IsVIP() && m_VIPInfo.m_nLevel > nOldLevel)
        {
            m_pRoleCheckIn->ReissueRewardByVipLevelChange(nOldLevel, m_VIPInfo.m_nLevel);
        }

        unsigned short nOldExtraChance = 0, nNewExtraChance = 0;
        const CVIPLevelPrivInfo *pPrivData = NULL;

        if (nOldLevel != 0)
        {
            pPrivData = CVIPDataMgr::Instance().GetVIPPrivInfo(nOldLevel);
            if (pPrivData != NULL)
            {
                nOldExtraChance = pPrivData->m_nDanceGroupChallengeExtraChance;

            }
        }
        if (m_VIPInfo.m_nLevel != 0)
        {
            pPrivData = CVIPDataMgr::Instance().GetVIPPrivInfo(m_VIPInfo.m_nLevel);
            if (pPrivData != NULL)
            {
                nNewExtraChance = pPrivData->m_nDanceGroupChallengeExtraChance;
            }
        }

        // 通知观察者
        CommonParam param;
        param.SetParam(std::string("medal"), std::string("vip"));
        param.SetParam(std::string("vip_level"), m_VIPInfo.m_nLevel);
        if (nOldExtraChance < nNewExtraChance)
        {
            param.SetParam("add_challenge_chance", nNewExtraChance - nOldExtraChance);
        }
        else if (bIsOpenAction && !bIsExpiredToday)
        {
            param.SetParam("add_challenge_chance", nOldExtraChance);
        }
        NotifyAll(param);
    }
}

bool CEntityVIPComponent::IsVIP()
{
    return m_VIPInfo.IsVIP();
}

void CEntityVIPComponent::NotifyClientUpdateVIPInfo()
{
    GameMsg_S2C_UpdateVIPInfo updateInfoMsg;

    updateInfoMsg.m_updateInfo.m_bVIP = IsVIP();
    updateInfoMsg.m_updateInfo.m_nLevel = (unsigned short)VIPLevel();
    updateInfoMsg.m_updateInfo.m_nExp = m_VIPInfo.m_nExp;
    updateInfoMsg.m_updateInfo.m_nExpiredTime = m_VIPInfo.m_nExpiredTime;
    updateInfoMsg.m_updateInfo.m_bCanGetDailyReward = m_VIPInfo.CanGetDailyReward();

    SendPlayerMsg(&updateInfoMsg);
}

void CEntityVIPComponent::NotifyGroupUpdateVIPInfo()
{

}

int CEntityVIPComponent::VIPLevel()
{
    return m_VIPInfo.m_nLevel;
}

int CEntityVIPComponent::VIPExp()
{
    return m_VIPInfo.m_nExp;
}

void CEntityVIPComponent::SetVIPConsumedPt(int nConsumedPt)
{
    m_VIPInfo.SetConsumedPt(nConsumedPt);
}

void CEntityVIPComponent::ChangeConsumedPt(int nValue)
{
    if (nValue != 0) {
        __int64 nConsumedPt = m_VIPInfo.ConsumedPt() + nValue;
        nConsumedPt = nConsumedPt > 0 ? nConsumedPt : 0;
        m_VIPInfo.SetConsumedPt((int)nConsumedPt);
        if (m_pRoleAttr != NULL) {
            WriteLog(LOGLEVEL_DEBUG, "--role[%d]changed vip consumedpt[%d]->[%d]", m_pRoleAttr->GetRoleID(), nValue, m_VIPInfo.ConsumedPt());
        }
    }
}

int CEntityVIPComponent::ConsumedPt()
{
    return m_VIPInfo.ConsumedPt();
}

void CEntityVIPComponent::SetExchangedpt(int nExchangedPt)
{
    m_VIPInfo.SetExchangedPt(nExchangedPt);
}

void CEntityVIPComponent::ChangeExchangedPt(int nValue)
{
    if (nValue != 0) {
        __int64 nExchangedPt = m_VIPInfo.ExchangedPt() + nValue;
        nExchangedPt = nExchangedPt > 0 ? nExchangedPt : 0;
        m_VIPInfo.SetExchangedPt((int)nExchangedPt);
        WriteLog(LOGLEVEL_DEBUG, "--role[%d] changed vip exchanged pt[%d]->[%d]", m_pRoleAttr->GetRoleID(), nValue, m_VIPInfo.ExchangedPt());
    }
}

int CEntityVIPComponent::ExchangedPt()
{
    return m_VIPInfo.ExchangedPt();
}

int CEntityVIPComponent::GetVIPMatchExpRateBouns()
{
    int nExpBonus = 0;
    if (m_VIPInfo.IsVIP()) {
        const CVIPLevelPrivInfo *pPrivData = CVIPDataMgr::Instance().GetVIPPrivInfo(m_VIPInfo.m_nLevel);
        if (pPrivData != NULL) {
            nExpBonus = pPrivData->m_nExpRateBonus;
        }
    }

    return nExpBonus;
}


void CEntityVIPComponent::OnVIPInfoChanged(int nOldVipLevel, int nNewVipLevel)
{
    UpdateVIPInfoToGroup();

    if (m_pRoleFriend != NULL)
    {
        m_pRoleFriend->UpdateAttributeToAllFriends();
        m_pRoleFriend->PushOnlineInfoToAllFriends();
    }

    GameMsg_S2C_NotifyRoleVIPInfoChanged msgNotify;
    msgNotify.m_nRoleID = m_pRoleAttr->GetRoleID();
    msgNotify.m_bIsVIP = m_VIPInfo.IsVIP();
    msgNotify.m_nVIPLevel = (unsigned short)m_VIPInfo.m_nLevel;

    if (m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL)
    {
        m_pRoleRoom->Room()->SendMsgToAll(&msgNotify, m_pRoleRoom);
    }

    if (m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL)
    {
        m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msgNotify, m_pRoleCouple);
    }

    if (m_pRolePhotoRoom != NULL && m_pRolePhotoRoom->PhotoRoom() != NULL)
    {
        m_pRolePhotoRoom->PhotoRoom()->SendMsgToAllBut(&msgNotify, m_pRolePhotoRoom);
    }

    if (m_pRoleAmuseRoom != NULL && m_pRoleAmuseRoom->AmuseRoom() != NULL)
    {
        m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll(&msgNotify, m_pRoleAmuseRoom);
    }

    if (m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL)
    {
        m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msgNotify);
    }

    if (m_pRoleDanceGroup != NULL)
    {
        m_pRoleDanceGroup->OnVIPChange();
    }
}

void CEntityVIPComponent::OnVIPExpired()
{
    m_pRoleItem->SetExpiredVIPEquipment();
    m_pRoleCheckIn->VipExpired();
}

void CEntityVIPComponent::SendOpenVipFrom()
{
    if (m_listOpenVipFromType.size() == 0)
        return;

    GameMsg_S2C_GetOpenVipFrom openFrom;
    openFrom.m_listOpenVipFromType = m_listOpenVipFromType;

    SendPlayerMsg(&openFrom);

    m_listOpenVipFromType.clear();
}

void CEntityVIPComponent::VIPBillExpBounus(int nPt)
{
    if (m_VIPInfo.IsVIP())
    {
        if (CVIPDataMgr::Instance().ConsumedPtExpBounsRate() > 0)
        {
            ChangeConsumedPt(nPt);
            int nExpBouns = ConsumedPt() / CVIPDataMgr::Instance().ConsumedPtExpBounsRate();
            if (nExpBouns > 0)
            {
                int nBounsPt = nExpBouns * CVIPDataMgr::Instance().ConsumedPtExpBounsRate();
                ChangeExchangedPt(nBounsPt);
                ChangeConsumedPt(nBounsPt * (-1));
                ChangeVIPInfo(0, nExpBouns, ERoleVipExpCause_AddByConsume);
                if (m_pRoleAttr != NULL)
                {
                    WriteLog(LOGLEVEL_DEBUG, "role[%u] Add vip exp[%d]->[%d]", m_pRoleAttr->GetRoleID(), nExpBouns, VIPExp());
                }
            }
        }
    }
}

unsigned short CEntityVIPComponent::GetDanceGroupChallengeExtraChance()
{
    int nLevel = 0;

    if (m_VIPInfo.IsVIP())
        nLevel = m_VIPInfo.m_nLevel;
    else if (m_VIPInfo.IsExpiredToday())
        nLevel = m_VIPInfo.m_nLevel;

    const CVIPLevelPrivInfo *pPrivData = NULL;
    if (nLevel != 0)
        pPrivData = CVIPDataMgr::Instance().GetVIPPrivInfo(nLevel);

    return (pPrivData == NULL ? 0 : pPrivData->m_nDanceGroupChallengeExtraChance);
}

void CEntityVIPComponent::CmdSetVIPLevel(int nVIPLevel)
{
    m_VIPInfo.m_nLevel = nVIPLevel;
}

void CEntityVIPComponent::OnVIPTest(int nPara1, int nPara2, int nPara3)
{
    int nIndex = nPara1;
    CSlotPeer slotPeer;
    switch (nIndex)
    {
    case 0:
    {
              GameMsg_C2S_GetVIPDailyReward msgDailyRewardRequest;
              OnGetVIPDailyReward(msgDailyRewardRequest, slotPeer);
    }
        break;
    case 1:
    {
              OnVIPExpired();
    }
        break;
    default:
        break;
    }
}

void CEntityVIPComponent::LogVipOpen()
{
    CRoleVipLog *pLog = new CRoleVipLog;
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_strRoleName = m_pRoleAttr->GetRoleName();
    pLog->m_eAction = CRoleVipLog::ERoleVipAction_OpenVip;

    LogRoleVip(pLog);
}

void CEntityVIPComponent::LogVipLevelUp(int nNewLevel)
{
    CRoleVipLog *pLog = new CRoleVipLog;
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_strRoleName = m_pRoleAttr->GetRoleName();
    pLog->m_eAction = CRoleVipLog::ERoleVipAction_LevelUp;
    pLog->m_nAddValue = nNewLevel;

    LogRoleVip(pLog);
}

void CEntityVIPComponent::LogVipExp(ERoleVipExpCause eCause, int nAddValue)
{
    CRoleVipLog *pLog = new CRoleVipLog;
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_strRoleName = m_pRoleAttr->GetRoleName();
    pLog->m_eAction = CRoleVipLog::ERoleVipAction_AddExp;
    pLog->m_eExpCause = eCause;
    pLog->m_nAddValue = nAddValue;

    LogRoleVip(pLog);
}

void CEntityVIPComponent::LogVipDuration(int nAddValue)
{
    CRoleVipLog *pLog = new CRoleVipLog;
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_strRoleName = m_pRoleAttr->GetRoleName();
    pLog->m_eAction = CRoleVipLog::ERoleVipAction_AddDuration;
    pLog->m_nAddValue = nAddValue;

    LogRoleVip(pLog);
}

void CEntityVIPComponent::LogRoleVip(ISQLLog *pLog)
{
    AddQuery(QUERY_LogRoleVip, 0, pLog);
}

void CEntityVIPComponent::PacketBuff(std::list<BuffData>& listBuff)
{
    int nExpRate = GetVIPMatchExpRateBouns();
    if (nExpRate > 0)
    {
        time_t tNow = time(NULL);
        BuffData buff;
        buff.m_nBuffType = EBuffType_Exp;
        buff.m_nBuffSrcType = EBuffSrcType_VIP;
        buff.m_nPercent = nExpRate;
        buff.m_nLeftTime = (int)(m_VIPInfo.m_nExpiredTime - (long long)tNow);
        listBuff.push_back(buff);
    }
}

