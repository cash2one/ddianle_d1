#include "EntityAttributeComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../datastructure/LogicInitData.h"
#include "../login/GameMsg_Player_CreateRoleRes.h"
#include "../quest/EntityQuestComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../item/EntityItemComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Rank/RankComponent.h"

#include "../mail/MailMgr.h"
#include "../Money/MoneyMgr.h"
#include "../activity/LevelUpRewardInfoMgr.h"
#include "../item/ItemProcess.h"

#include "../Notification/NotificationEvent.h"
#include "../Notification/NotificationComponent.h"

#include "../../socket/ParamPool.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../item/ItemMsgDefine.h"
#include "../logic/LogicCirculator.h"
#include "../payment/PaymentInfoMgr.h"
#include "../Location/LocationMgr.h"

#include "../player/GameMsg_S2G_UpdateAttrInfo.h"
#include "GameMsg_C2S_ChangeRoleName.h"
#include "GameMsg_C2S_ChangeRoleSignature.h"
#include "../activity/GameMsg_LevelUpReward.h"
#include "GameMsg_CompleteGuide.h"
#include "../share/GameMsg_S2A_AddMCoinByCmd.h"

#include "GameMsg_C2S_ChangeBirthday.h"
#include "GameMsg_S2C_ChangeBirthdayRes.h"
#include "../activity/GameMsg_Activity.h"

#include "GameMsg_S2C_ChangeRoleNameResult.h"
#include "GameMsg_S2C_ChangeRoleSignatureResult.h"
#include "../chat/GameMsg_S2C_Chat.h"
#include "../login/GameMsg_Payment.h"
#include "../activity/GlobalActivityData.h"
#include "../../datastructure/Localization.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../payment/PaymentInfoMgr.h"
#include "../anti-addiction/EntityAntiAddictionComponent.h"
#include "guidereward.h"

#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../../datastructure/datainfor/ServerConfig.h"
#include "../activity/EntityActivityComponent.h"
#include "../mall/MallDefine.h" // for experiencecard item.
#include "../mall/MallDataMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../mall/CurrencyType.h"
#include "../util/NameDeny.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../roomLib/GameMsgNewRoom.h"

extern int g_nLine;

#include <time.h>

extern CLogicCirculator* g_pCirculator;
#define TODO todo...

CEntityAttributeComponent::CEntityAttributeComponent(void)
:CEntityComponent(true, true)
{
    m_nPID = 0;
    memset(m_szRoleName, 0, MAX_NAME_LEN);
    m_nSex = ESexType_Male;
    m_nMoney = 0;
    m_nExp = 0;
    m_nLevel = 1;
    m_nHot = 0;
    m_nConstellation = 0;
    m_nSkinColor = 1;
    m_nPriv = 0;
    m_nRoleCreateTime = 0;
    m_nLastLoginTime = 0;
    m_nLastLogoutTime = 0;
    m_bTodayFirstLogin = false;
    m_nTimeOnline = 0;
    m_nUpdateDelay = 0;
    m_nQuestDone = 0;
    m_bCompleteTeachGuide = false;
    m_nCompleteProcGuide = 0;
    m_bReceivedInvitationReward = false;
    m_bCompleteAnimation = false;
    m_pFirstPaymentInfoLastNotify = NULL;

    for (unsigned char i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; i++)
    {
        m_anBeiBaoKuozhan[i] = 0;
    }

    m_nBadgeGridNum = BADGE_BAG_BEGIN_INDEX;

    m_strBirthday.clear();
    memset(m_szSignature, 0, MAX_SIGNATURE_LEN);

    m_pRoleNet = NULL;
    m_pRoleQuest = NULL;
    m_pRoleAch = NULL;
    m_pRoleItem = NULL;
    m_pRoleMail = NULL;
    m_pRoleFriend = NULL;
    m_pRoleRoom = NULL;
    m_pDanceGroup = NULL;
    m_pRoleCouple = NULL;
    m_pRoleVIP = NULL;
    m_pNotification = NULL;
    m_pRoleAntiAddiction = NULL;
    m_pRoleQuestNew = NULL;
    m_pPhotoRoomCOM = NULL;
    m_pAmuseRoomCOM = NULL;
    m_pRoleActivity = NULL;
    m_pRankComponent = NULL;

    m_nMoneyGain = 100;
    m_nExpGain = 100;
    m_nPreGain = 100;
    m_nIgnoreMis = 0;

    m_nRoleLocation = Zone_None;

    m_bCompleteFirstPaymentTag = false;
    m_bCanSendFirstPaymentReward = false;
    m_nLastRstMoneyTime = 0;
    m_nMallLastMoneyRstRemindtime = 0;
    m_nMailLastMoneyRstRemindTime = 0;

    m_nLastChangeBirthdayTime = 0;

    memset(m_szInvitationCode, 0, MAX_INVITATION_CODE_LEN);
    m_nClothGroupId = 1;
    m_nTransformId = 0;
    m_bReceivedProcGuideReward = false;

    m_bKeyDataModified = false;

    m_nNotGetLevelUpVIPRewardLevel = 0;
    m_guideData = 0;
}

CEntityAttributeComponent::~CEntityAttributeComponent(void)
{
}

void CEntityAttributeComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoleName);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoleSignature);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CompleteTeachGuide);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CompleteProcGuide);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SelectGuideFace);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CompleteAnimation);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeBirthday);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReciveGuideExperienceCard);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_FixedChargeReq);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestGuideFinish);

    RegMsgProcessor(MSG_C2S_ChangeRoleName, &CEntityAttributeComponent::OnChangeName);
    RegMsgProcessor(MSG_C2S_ChangeRoleSignature, &CEntityAttributeComponent::OnChangeSignature);
    RegMsgProcessor(MSG_C2S_CompletedTeachGuide, &CEntityAttributeComponent::OnCompleteTeachGuide);
    RegMsgProcessor(MSG_C2S_CompletedProcGuide, &CEntityAttributeComponent::OnCompleteProcGuide);
    RegMsgProcessor(MSG_C2S_SelectGuideFace, &CEntityAttributeComponent::OnSelectGuideFace);
    RegMsgProcessor(MSG_C2S_CompleteAnimation, &CEntityAttributeComponent::OnCompleteAnimation);
    RegMsgProcessor(MSG_C2S_ChangeBirthday, &CEntityAttributeComponent::OnChangeBirthday);
    RegMsgProcessor(MSG_C2S_ReciveGuideExperienceCard, &CEntityAttributeComponent::OnReciveGuideExperienceCard);
    RegMsgProcessor(MSG_C2S_RequestGuideFinish, &CEntityAttributeComponent::OnReciveGuideFinish);
    RegMsgProcessor(MSG_C2S_FixedchargeReq, &CEntityAttributeComponent::OnFixedChargeReq);

    // DB
    RegQueryProcessor(QUERY_ChangeRoleName, &CEntityAttributeComponent::OnChangeRoleNameCallBack);
    RegQueryProcessor(QUERY_Bill_ChongZhi, &CEntityAttributeComponent::OnChargeCallBack);
    RegQueryProcessor(QUERY_AddCompleteTeachGuideEvent, &CEntityAttributeComponent::OnCompleteTeachGuideCallBack);
    RegQueryProcessor(QUERY_AddCompleteProcGuideEvent, &CEntityAttributeComponent::OnCompleteProcGuideCallBack);
    RegQueryProcessor(QUERY_AddCompleteAnimation, &CEntityAttributeComponent::OnCompleteProcAnimationCallBack);
    RegQueryProcessor(QUERY_ClearFirstPaymentInfo, &CEntityAttributeComponent::OnClearFirstPaymentInfoCallBack);
}

void CEntityAttributeComponent::Start()
{
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleQuest = GetComponent<CRoleQuest>();
    m_pRoleAch = GetComponent<CRoleAchievement>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleMail = GetComponent<CRoleMail>();
    m_pRoleFriend = GetComponent<CRoleFriend>();
    m_pRoleRoom = GetComponent<CRoleRoom>();
    m_pDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRoleVIP = GetComponent<CRoleVIP>();
    m_pNotification = GetComponent<CRoleNotification>();
    m_pRoleAntiAddiction = GetComponent<CRoleAntiAddiction>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
    m_pPhotoRoomCOM = GetComponent<CRolePhotoRoom>();
    m_pAmuseRoomCOM = GetComponent<CRoleAmuseRoom>();
    m_pRoleActivity = GetComponent<CRoleActivity>();
    m_pRankComponent = GetComponent<CRoleRank>();
}

unsigned short CEntityAttributeComponent::PID() const
{
    return m_nPID;
}

const char *CEntityAttributeComponent::GetRoleName() const
{
    return m_szRoleName;
}

void CEntityAttributeComponent::SetRoleName(const char *pNewName)
{
    SetRoleName(pNewName, true);
}

void CEntityAttributeComponent::OnNameChanged(const char* pNewName)
{
    NotifyGroupRoleNameChange();

    GameMsg_S2C_ChangeRoleNameResult msg(CHANGEROLENAME_SUCCESS);
    msg.m_nRoleID = m_nRoleID;
    msg.m_strNewName = pNewName;

    if (m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL)
    {
        m_pRoleRoom->Room()->SendMsgToAll(&msg, m_pRoleRoom);
    }
    if (m_pAmuseRoomCOM != NULL && m_pAmuseRoomCOM->AmuseRoom() != NULL)
    {
        m_pAmuseRoomCOM->AmuseRoom()->SendMsgToAll(&msg, m_pAmuseRoomCOM);
    }
    if (m_pPhotoRoomCOM != NULL && m_pPhotoRoomCOM->PhotoRoom() != NULL)
    {
        m_pPhotoRoomCOM->PhotoRoom()->SendMsgToAllBut(&msg, m_pPhotoRoomCOM);
    }
    if (m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL)
    {
        m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msg, m_pRoleCouple);
    }
    if (m_pDanceGroup != NULL && m_pDanceGroup->GetCeremonyRoom() != NULL)
    {
        m_pDanceGroup->GetCeremonyRoom()->RoomBroadcast(msg);
    }

    SendPlayerMsg(&msg);

    if (m_pRoleFriend != NULL)
    {
        m_pRoleFriend->UpdateAttributeToAllFriends();
        m_pRoleFriend->PushOnlineInfoToAllFriends();
    }
    if (m_pDanceGroup)
    {
        m_pDanceGroup->OnRoleNameChanged();
    }
}

short CEntityAttributeComponent::GetAge() const
{
    short nAge = BirthdayToAge(m_strBirthday.c_str());
    return nAge;
}

void CEntityAttributeComponent::SetBirthday(const std::string &rBirthday)
{
    m_strBirthday = rBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

void CEntityAttributeComponent::SetInvitationCode(const char *pszInvitationCode)
{
    memcpy(m_szInvitationCode, pszInvitationCode, MAX_INVITATION_CODE_LEN - 1);
    m_szInvitationCode[MAX_INVITATION_CODE_LEN - 1] = '\0';
}

const char *CEntityAttributeComponent::GetInvitationCode()
{
    return m_szInvitationCode;
}

void CEntityAttributeComponent::SetMoney(int nValue)
{
    m_nMoney = nValue;

    GameMsg_S2C_RefreshMoney msg;
    msg.nMoney = m_nMoney;
    SendPlayerMsg(&msg);

    NotifyGroupMoneyChange();
}

void CEntityAttributeComponent::ChangeMoney(int nValue, EChangeMoneyCause eChangeMoneyCause, int nAffixData, bool bSaveToDB, bool bChecAntiAddiction)
{
    if (nValue == 0)
        return;

#ifndef SERVER_IOS
    //防沉迷
    if (bChecAntiAddiction && m_pRoleAntiAddiction != NULL && m_pRoleAntiAddiction->IsActive() && nValue > 0)
    {
        int nEffect = m_pRoleAntiAddiction->GetAddicEffectByMoneyCause(eChangeMoneyCause);
        if (nEffect >= 0)
        {
            m_pRoleAntiAddiction->ReCalcEffectData((eAddicEffect)nEffect, nValue);
        }
        if (nValue == 0)
        {
            return;
        }
    }
#endif //SERVER_IOS

    //需要保证不能扣成负的
    __int64 nResult = m_nMoney;
    nResult += nValue;
    if (nResult < 0) {
        nResult = 0;
    }
    else if (nResult > MAX_MONEY_NUM)
    {
        nResult = MAX_MONEY_NUM;
    }

    // 同步服务器-客户端 金券数值
    SetMoney((int)nResult);

    // DB操作 and 日志
    if (bSaveToDB)
    {
        AddQuery(QUERY_UpdateRoleMoney, GetRoleID(), NULL, nValue);

        LogRoleChangeMoney(m_nRoleID, nValue, eChangeMoneyCause, m_nMoney);
    }
}

void CEntityAttributeComponent::SetHot(unsigned int nValue, int nCauseType, int nAffixData)
{
    m_nHot = nValue;

    GameMsg_S2C_RefreshHot msg;
    msg.nHot = m_nHot;
    SendPlayerMsg(&msg);

    if (m_pRoleQuest != NULL)
    {
        m_pRoleQuest->PushPlayerQuest();
        m_pRoleQuest->OnRoleAttrChange();
    }
}

void CEntityAttributeComponent::ModifyHot(int nValue, int nCauseType, int nAffixData)
{
    if (nValue != 0)
    {
        unsigned int nTemp = (int)m_nHot + nValue;
//         if (nTemp < 0)
//             nTemp = 0;

        SetHot(nTemp, nCauseType, nAffixData);
        m_pRoleAch->UpdateAchievement(AchievementType_HistoryHotValue);
    }
}

void CEntityAttributeComponent::SetOnlineTime(unsigned int nOnlineTime)
{
    m_nTimeOnline = nOnlineTime;
}


void CEntityAttributeComponent::SetLocation(int nLocation)
{
    if (nLocation > Zone_None && nLocation < Zone_Max)
    {
        m_nRoleLocation = nLocation;
    }
}

int CEntityAttributeComponent::GetLocation()
{
    return m_nRoleLocation;
}

unsigned short CEntityAttributeComponent::GetLevel() const
{
    return m_nLevel;
}

void CEntityAttributeComponent::SetLevel(unsigned short nValue, int nCauseType, int nAffixData)
{
    unsigned short nOldLevel = m_nLevel;
    m_nLevel = nValue;
    OnLevelUp(nOldLevel);
}

void CEntityAttributeComponent::OnLevelUp(unsigned short nOldLevel)
{
    NotifyGroupLevelChange();

    int nFuncID = 0;
    std::string strFuncName;
    CLevelUpInfoMgr::Instance().GetLevelUpRecommend(m_nLevel, nFuncID, strFuncName);

    GameMsg_S2C_RefreshLevel msg;
    msg.m_nLevel = m_nLevel;
    msg.m_nExp = m_nExp;
    msg.m_nFunctionID = nFuncID;

    SendPlayerMsg(&msg);

    //更新级别上升时需要更新的内容，如任务
    if (nOldLevel < m_nLevel)
    {
        if (m_pRoleQuest != NULL)
        {
            m_pRoleQuest->PushPlayerQuest();
            m_pRoleQuest->OnRoleAttrChange();
            m_pRoleQuestNew->OnAttributeChange();
        }

        if (m_pRoleAch != NULL)
        {
            m_pRoleAch->UpdateAchievement(AchievementType_Level);
        }
        // 如果是新手包，提示更新包
        _SendUpgradeInstallerMessage();
        m_bKeyDataModified = true;
    }

    if (m_pRoleFriend != NULL)
    {
        m_pRoleFriend->PushOnlineInfoToAllFriends();
    }
    if (m_pDanceGroup)
    {
        m_pDanceGroup->OnLevelUp();
    }

    if (m_pNotification)
    {
        CNotificationEvent event;
        event.m_nEventType = NOTIFICATION_LEVEL_UP;
        m_pNotification->AddEvent(event);
    }

    CommonParam param;
    param.SetParam(std::string("medal"), std::string("level"));
    param.SetParam(std::string("level"), (int)m_nLevel);

    NotifyAll(param);
}

unsigned int CEntityAttributeComponent::AddAbsExp(unsigned int nValue, CRoleExpLog::EAddExpCause eAddExpCause, int nAffixData)
{
    if (nValue == 0)
        return 0;

#ifndef SERVER_IOS
    //防沉迷
    if (m_pRoleAntiAddiction != NULL && m_pRoleAntiAddiction->IsActive() && nValue > 0)
    {
        int nEffect = m_pRoleAntiAddiction->GetAddicEffectByExpCause(eAddExpCause);
        if (nEffect >= 0)
        {
            m_pRoleAntiAddiction->ReCalcEffectData((eAddicEffect)nEffect, nValue);
        }
        if (nValue == 0)
        {
            return 0;
        }
    }
#endif //SERVER_IOS

    m_nExp += nValue;

    unsigned short nOldLevel = GetLevel();
    unsigned short nNewLevel = nOldLevel;

    while (m_nExp >= (unsigned int)CLogicInitData::Instance().GetExpNeed(nNewLevel)
        && nNewLevel < (unsigned short)CLogicInitData::Instance().GetMaxLevel())
    {
        m_nExp -= CLogicInitData::Instance().GetExpNeed(nNewLevel);
        ++nNewLevel;
    }

    if (nNewLevel != nOldLevel)
    {
        SetLevel(nNewLevel, eAddExpCause, nAffixData);
        _SendLevelUpReward(nOldLevel, nNewLevel);
    }
    else
    {
        GameMsg_S2C_RefreshExp msg;
        msg.nExp = m_nExp;
        SendPlayerMsg(&msg);
    }

    m_bKeyDataModified = true;
    if (nNewLevel != nOldLevel)
    {
        _LogAddExp(m_nRoleID, eAddExpCause, nValue, m_nLevel, m_nExp);
    }

    if (NULL != m_pRankComponent)
        m_pRankComponent->SubmitRankValue( ERankDataType_Exp, (int)nValue );

    return nValue;
}

unsigned int CEntityAttributeComponent::AddRateExp(unsigned int nValue, CRoleExpLog::EAddExpCause eAddExpCause, int nAffixData)
{
    int nOriExpGain = CLogicInitData::Instance().GetExpGainPer();
    int nExpRate = GetExpGain() + nOriExpGain + CGlobalActivityData::Instance().GetExtraExpRate() + m_pRoleVIP->GetVIPMatchExpRateBouns() - 100;

    unsigned int nExp = (nValue * nExpRate) / 100;
    nExp = AddAbsExp(nExp, eAddExpCause, nAffixData);

    return nExp;
}


int CEntityAttributeComponent::GetLevelExp() const
{
    if (m_nLevel < CLogicInitData::Instance().GetMaxLevel())
        return  CLogicInitData::Instance().GetExpNeed(m_nLevel);
    else
        return  CLogicInitData::Instance().GetExpNeed(m_nLevel - 1);
}

void CEntityAttributeComponent::SetPre(unsigned int nValue, int nCauseType, int nAffixData)
{
    m_nPre = nValue;

    //GameMsg_S2C_RefreshPre msg;
    //msg.nPre = m_nPre;
    //if (m_pNetComponent != NULL){
    //	m_pNetComponent->SendPlayerMsg(&msg);
    //}
}

void CEntityAttributeComponent::ModifyPre(int nValue, int nCauseType, int nAffixData)
{
    if (nValue == 0)
        return;

    //需要保证不能扣成负的
    unsigned int nTemp = (int)m_nPre + nValue;
//     if (nTemp < 0)
//         nTemp = 0;

    SetPre(nTemp, nCauseType, nAffixData);
}


void CEntityAttributeComponent::SetMoneyGain(unsigned short nValue)
{
    m_nMoneyGain = nValue;
    //GAMEMSG_S2C_RefreshMoneyGain msg(m_nMoneyGain);
    //SendGameMsg(&msg,&m_RoleTag);
}


void CEntityAttributeComponent::ModifyMoneyGain(int nValue)
{
    if (nValue != 0){
        unsigned short Temp = 0;
        if (nValue > 0){
            Temp = GetMoneyGain() + (unsigned short)nValue;
        }
        else{
            Temp = GetMoneyGain() > abs(nValue) ? (unsigned short)(GetMoneyGain() + nValue) : 0;
        }
        SetMoneyGain(Temp);
    }
}

void CEntityAttributeComponent::SetExpGain(unsigned short nValue)
{
    m_nExpGain = nValue;
    //GAMEMSG_S2C_RefreshExpGain msg(m_nExpGain);
    //SendGameMsg(&msg,&m_RoleTag);
}

void CEntityAttributeComponent::ModifyExpGain(int nValue)
{
    if (nValue != 0){
        unsigned short Temp = 0;
        if (nValue > 0){
            Temp = GetExpGain() + (unsigned short)nValue;
        }
        else{
            Temp = GetExpGain() > abs(nValue) ? (unsigned short)(GetExpGain() + nValue) : 0;
        }
        SetExpGain(Temp);
    }
}

void CEntityAttributeComponent::SetPreGain(unsigned short nValue)
{
    m_nPreGain = nValue;
    //GAMEMSG_S2C_RefreshPreGain msg(m_nPreGain);
    //SendGameMsg(&msg,&m_RoleTag);
}

void CEntityAttributeComponent::ModifyPreGain(int nValue)
{
    if (nValue != 0){
        unsigned short Temp = 0;
        if (nValue > 0){
            Temp = GetPreGain() + (unsigned short)nValue;
        }
        else{
            Temp = GetPreGain() > abs(nValue) ? (unsigned short)(GetPreGain() + nValue) : 0;
        }
        SetPreGain(Temp);
    }
}

void CEntityAttributeComponent::SetIgnoreMis(unsigned short nValue)
{
    m_nIgnoreMis = nValue;
    //GAMEMSG_S2C_RefreshIgnoreMis msg(m_nIgnoreMis);
    //SendGameMsg(&msg,&m_RoleTag);
}

void CEntityAttributeComponent::ModifyIgnoreMis(int nValue)
{
    if (nValue != 0){
        unsigned short Temp = 0;
        if (nValue > 0){
            Temp = GetIgnoreMis() + (unsigned short)nValue;
        }
        else{
            Temp = GetIgnoreMis() > abs(nValue) ? (unsigned short)(GetIgnoreMis() + nValue) : 0;
        }
        SetIgnoreMis(Temp);
    }
}


bool CEntityAttributeComponent::IsMarried()
{
    return m_pRoleCouple->IsMarried();
}

void CEntityAttributeComponent::ResendLevelUpVIPExtraReward(unsigned char nOpenVipFromType)
{
    CLevelRewardInfo vipRewardInfo;

    if (CLevelUpInfoMgr::Instance().IsVIPRewardLevel((unsigned short)m_nNotGetLevelUpVIPRewardLevel))
    {
        GameMsg_S2C_RequestVIPExtraRewardRes vipExtraRewardRes;
        vipExtraRewardRes.m_nOpenVipFromType = nOpenVipFromType;



        if (m_nNotGetLevelUpVIPRewardLevel == 0)
        {
            vipExtraRewardRes.m_nErrorType = ERequestVIPExtraRewardErrorType_AlreadyGot;
        }
        else
        {
            CLevelUpInfoMgr::Instance().GetVIPRewardInfo((unsigned short)m_nNotGetLevelUpVIPRewardLevel, vipRewardInfo);

            _SendLevelUpReward(vipRewardInfo);

            CReward reward;

            if (m_nSex == ESexType_Female)
                reward.m_listItems = vipRewardInfo.m_listFemaleItem;
            else
                reward.m_listItems = vipRewardInfo.m_listMaleItem;
            reward.m_nMoney = vipRewardInfo.m_nMoney;
            reward.m_nBindMCoin = vipRewardInfo.m_nBindMCoin;

            vipExtraRewardRes.m_listRewards.push_back(reward);

            m_nNotGetLevelUpVIPRewardLevel = 0;
        }

        SendPlayerMsg(&vipExtraRewardRes);
    }
}

bool CEntityAttributeComponent::CanBill(CurrencyMap &mapCurrency)
{
    if (mapCurrency.empty())
    {
        return false;
    }

    CurrencyMap::iterator itNeedPayMCoin = mapCurrency.find(EMallCurrencyType_MCoin);
    CurrencyMap::iterator itNeedPayBindMCoin = mapCurrency.find(EMallCurrencyType_BindMCoin);
    CurrencyMap::iterator itNeedPayMoney = mapCurrency.find(EMallCurrencyType_Money);

    unsigned int nNeedPoint = (itNeedPayMCoin != mapCurrency.end() ? itNeedPayMCoin->second : 0);
    unsigned int nNeedBindMCoin = (itNeedPayBindMCoin != mapCurrency.end() ? itNeedPayBindMCoin->second : 0);
    unsigned int nNeedMoney = (itNeedPayMoney != mapCurrency.end() ? itNeedPayMoney->second : 0);

    unsigned int nPoint = GetPoint();
    unsigned int nBindPoint = GetBindPoint();
    unsigned int nMoney = GetMoney();

    if (nMoney < nNeedMoney) // 金券不足用绑定M币补充支付
    {
        nNeedBindMCoin += (nNeedMoney - nMoney);
        nNeedMoney = nMoney;
    }
    if (nBindPoint < nNeedBindMCoin) // 绑定M币不足用M币补充支付
    {
        nNeedPoint += (nNeedBindMCoin - nBindPoint);
        nNeedBindMCoin = nBindPoint;
    }

    if (nPoint < nNeedPoint) // M币不足直接报错
    {
        return false;
    }

    return true;
}

void CEntityAttributeComponent::Bill(CurrencyMap &mapCurrency, EChangeBillCause eChangeBillCause,
    EChangeBindBillCause eChangeBindBillCause, EChangeMoneyCause eChangeMoneyCause)
{
    if (mapCurrency.empty())
    {
        return;
    }

    CurrencyMap::iterator itNeedPayMCoin = mapCurrency.find(EMallCurrencyType_MCoin);
    CurrencyMap::iterator itNeedPayBindMCoin = mapCurrency.find(EMallCurrencyType_BindMCoin);
    CurrencyMap::iterator itNeedPayMoney = mapCurrency.find(EMallCurrencyType_Money);

    int nNeedPoint = (itNeedPayMCoin != mapCurrency.end() ? itNeedPayMCoin->second : 0);
    int nNeedBindMCoin = (itNeedPayBindMCoin != mapCurrency.end() ? itNeedPayBindMCoin->second : 0);
    int nNeedMoney = (itNeedPayMoney != mapCurrency.end() ? itNeedPayMoney->second : 0);

    int nBindPoint = GetBindPoint();
    int nMoney = GetMoney();

    if (nMoney < nNeedMoney) // 金券不足用绑定M币补充支付
    {
        nNeedBindMCoin += (nNeedMoney - nMoney);
        nNeedMoney = nMoney;
    }
    if (nBindPoint < nNeedBindMCoin) // 绑定M币不足用M币补充支付
    {
        nNeedPoint += (nNeedBindMCoin - nBindPoint);
        nNeedBindMCoin = nBindPoint;
    }

    if (0 != nNeedPoint)
        ChangeBill(-nNeedPoint, eChangeBillCause);
    if (0 != nNeedBindMCoin)
        ChangeBindBill(-nNeedBindMCoin, eChangeBindBillCause);
    if (0 != nNeedMoney)
        ChangeMoney(-nNeedMoney, eChangeMoneyCause, 0);

    return;
}

void CEntityAttributeComponent::SetFixedChargeRewardDone(unsigned int amount)
{ // 完成一笔充值
    //	m_fixedCharges.insert(std::make_pair(amount, (unsigned int)time(NULL))); // 标记为已经完成
    m_fixedCharges[amount] = (unsigned int)time(NULL);

    SendFixedChargeMsg(true, PaymentInfoMgr::Instance().FixedChargeGiftCoin(amount));

    return;
}

void CEntityAttributeComponent::SendFixedChargeMsg(bool bIsFiexedCharge, unsigned int nGiftBindPoint)
{
    GameMsg_S2C_FixedchargeComplete complete;
    complete.m_bIsFixedCharge = bIsFiexedCharge;
    complete.m_nGiftBindPoint = nGiftBindPoint;

    unsigned int tmStart = 0, tmEnd = 0;
    complete.m_bOpened = PaymentInfoMgr::Instance().FixedChargeIsRun(time(NULL), &tmStart, &tmEnd);

    for (PaymentInfoMgr::FixedChargeGiftMCoinMap::iterator iter = PaymentInfoMgr::Instance().m_mapGiftFixedCharge.begin();
        iter != PaymentInfoMgr::Instance().m_mapGiftFixedCharge.end(); ++iter)
    {
        QuotaRechargeInfo rinfo;
        rinfo.m_strItem = PaymentInfoMgr::Instance().FixedChargeGiftItem(iter->first);
        rinfo.m_returnPtBind = iter->second; // binder point.
        //         rinfo.m_bOpenQuota = m_fixedCharges.find(iter->first) != m_fixedCharges.end();
        FixedCharges::iterator iterFc = m_fixedCharges.find(iter->first);
        if (iterFc == m_fixedCharges.end())
            rinfo.m_bOpenQuota = false; // 没有执行
        else if (iterFc->second > tmStart && iterFc->second < tmEnd)
            rinfo.m_bOpenQuota = true;
        else
            rinfo.m_bOpenQuota = false;

        complete.m_vecRechargeInfo.push_back(rinfo);
    }

    //     string strbuf;
    //     Formatter::Format(strbuf, "actopend:%s, fixcharge:time: %u, isF:%s,--", 
    //         complete.m_bOpened ? "y" : "n", (unsigned int)time(NULL), bIsFiexedCharge ? "y" : "n");
    // 
    //     for (int i = 0; i < complete.m_vecRechargeInfo.size(); ++i)
    //     {
    //         string buf1;
    //         Formatter::Format(buf1, "item:%s, iscomplete:%s--", complete.m_vecRechargeInfo[i].m_strItem.c_str(), 
    //             complete.m_vecRechargeInfo[i].m_bOpenQuota ? "y" : "n");
    //         strbuf += buf1;
    //     }
    // 
    //     WriteLog(LOGLEVEL_DEBUG, "%s", strbuf.c_str());

    SendPlayerMsg(&complete);
}

unsigned int CEntityAttributeComponent::ChangeBill(int nPoint, EChangeBillCause eChangeBillCause, bool bSaveToDB, bool bAddCumulativeSpend, bool bAddVipExp)
{
    if (nPoint == 0)
        return m_Bi.nPoint;

    //需要保证不能扣成负的
    if (nPoint < 0)
    {
        if (m_Bi.nPoint < (unsigned int)abs(nPoint))
        {
            nPoint = (m_Bi.nPoint)*(-1);
            m_Bi.nPoint = 0;
        }
        else
        {
            m_Bi.nPoint = m_Bi.nPoint - (unsigned int)abs(nPoint);
        }

        //扣除MBill通知
        CommonParam param;
        param.SetParam(std::string("delBill"), nPoint);
        NotifyAll(param);
    }
    else
    {
        m_Bi.nPoint = m_Bi.nPoint + (unsigned int)nPoint;
        m_Bi.nTotalPoint = m_Bi.nTotalPoint + (unsigned int)nPoint;
    }

    if (bSaveToDB)
    {

        AddQuery(QUERY_Bill_UpdatePt, nPoint, 0, GetAccount(), NULL);
        _LogRoleChangeBill(m_nRoleID, nPoint, eChangeBillCause, m_Bi.nPoint);

        // vip exp
        if (nPoint < 0 && bAddVipExp)
        {
            m_pRoleVIP->VIPBillExpBounus((-1)*nPoint);
        }

        if (nPoint < 0 && bAddCumulativeSpend)
        {
            AddQuery(QUERY_Insert_CumulativeSpendBill, GetRoleID(), NULL, (-1)*nPoint);
            m_pRoleActivity->AddCumulativeSpendBill((-1)*nPoint);

            if (NULL != m_pRankComponent)
            {
                int nChangeValue = -nPoint;
                m_pRankComponent->SubmitRankValue(ERankDataType_MCoin, nChangeValue);
            }
        }

    }

    GameMsg_S2C_RefreshPt msg;
    msg.m_nPoint = m_Bi.nPoint;
    msg.m_nTotalPoint = m_Bi.nTotalPoint;
    SendPlayerMsg(&msg);

    return m_Bi.nPoint;
}

bool CEntityAttributeComponent::IsEnoughCurrency(EMallCurrencyType eType, int nCurrencyValue)
{
    if (nCurrencyValue <= 0)
    {
        return false;
    }

    switch (eType)
    {
    case EMallCurrencyType_MCoin:
        return  (int)GetPoint() >= nCurrencyValue;
    case EMallCurrencyType_Money:
        return  GetMoney() >= nCurrencyValue;
    case EMallCurrencyType_BindMCoin:
        return  (int)GetBindPoint() >= nCurrencyValue;
    case EMallCurrencyType_Max:
    case EMallCurrencyType_None:
    default:
        break;
    }

    return false;
}

void CEntityAttributeComponent::DeductCurrency(EMallCurrencyType eType, int nMoney, const std::vector<int> & vecCause)
{
    if (nMoney <= 0 || vecCause.size() < (size_t)(EMallCurrencyType_Max - 1))
    {
        return;
    }

    switch (eType)
    {
    case EMallCurrencyType_MCoin:ChangeBill((-1)*nMoney, (EChangeBillCause)vecCause[eType - 1]); break;
    case EMallCurrencyType_Money:ChangeMoney((-1)*nMoney, (EChangeMoneyCause)vecCause[eType - 1], 0); break;
    case EMallCurrencyType_BindMCoin:ChangeBindBill((-1)*nMoney, (EChangeBindBillCause)vecCause[eType - 1]); break;
    case EMallCurrencyType_Max:
    case EMallCurrencyType_None:
    default:break;
    }
}

unsigned int CEntityAttributeComponent::ChangeBindBill(int nBindPoint, EChangeBindBillCause eCause, bool bSaveToDB, bool bAddCumulativeSpend)
{
    if (nBindPoint == 0)
        return m_Bi.nBindPoint;

    //需要保证不能扣成负的
    if (nBindPoint < 0)
    {
        if (m_Bi.nBindPoint < (unsigned int)abs(nBindPoint))
        {
            nBindPoint = (m_Bi.nBindPoint)*(-1);
            m_Bi.nBindPoint = 0;
        }
        else
        {
            m_Bi.nBindPoint = m_Bi.nBindPoint - (unsigned int)abs(nBindPoint);
        }
    }
    else
    {
        m_Bi.nBindPoint = m_Bi.nBindPoint + (unsigned int)nBindPoint;
    }

    if (bSaveToDB)
    {
        AddQuery(QUERY_Bill_UpdateBindPt, nBindPoint, 0, GetAccount(), NULL);
        _LogRoleChangeBindBill(m_nRoleID, nBindPoint, eCause, m_Bi.nBindPoint);

        if (nBindPoint < 0 && bAddCumulativeSpend)
        {
            AddQuery(QUERY_Insert_CumulativeSpendBindBill, GetRoleID(), NULL, (-1)*nBindPoint);
            m_pRoleActivity->AddCumulativeSpendBindBill((-1)*nBindPoint);
        }
    }

    GameMsg_S2C_RefreshBindPt msg;
    msg.nBindPoint = m_Bi.nBindPoint;
    SendPlayerMsg(&msg);

    return m_Bi.nBindPoint;
}

////nType  0表示游戏活动赠送  1表示游戏内任务奖励  2表示使用抵用券购物
//unsigned int CEntityAttributeComponent::AddChange(int nPoint,unsigned char nState)
//{
//	if(nPoint==0)
//		return m_Bi.nChange;
//
//	//需要保证不能扣成负的
//	if(nPoint < 0){
//		if(m_Bi.nChange < abs(nPoint)){
//			nPoint = (m_Bi.nChange)*(-1);
//			m_Bi.nChange = 0;
//		}
//		else{
//			m_Bi.nChange = m_Bi.nChange - (unsigned int)abs(nPoint);
//		}
//	}
//	else
//		m_Bi.nChange = m_Bi.nChange + (unsigned int)nPoint;
//
//	AddQuery(QUERY_Bill_UpdateChange,nPoint,0,GetAccount(),NULL);
//
//	GameMsg_S2C_RefreshChange msg;
//	msg.nChange = m_Bi.nChange;
//	SendPlayerMsg(&msg);

//	return m_Bi.nChange;
//}

void CEntityAttributeComponent::SetRoleName(const char *pNewName, bool bNotice)
{
    if (pNewName != NULL)
    {
        int nSize = strlen(pNewName) + 1;
        if (nSize > g_ServerConfig.m_MaxNameLimit - 1)
        {
            nSize = g_ServerConfig.m_MaxNameLimit - 1;
        }
        if (nSize > MAX_NAME_LEN)
        {
            nSize = MAX_NAME_LEN;
        }

        std::string strNewName(pNewName);
        NameDeny::Instance().FilterInvalidText(strNewName);

        Formatter(m_szRoleName, nSize) << strNewName;

        if (bNotice)
        {
            OnNameChanged(m_szRoleName);
        }
    }
}

bool CEntityAttributeComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    if (pRoleInforDB != NULL)
    {
        //防止数据库数据异常
        pRoleInforDB->m_szFamilyName[MAX_FAMILY_NAME - 1] = '\0';
        pRoleInforDB->m_szSignature[MAX_SIGNATURE_LEN - 1] = '\0';
        pRoleInforDB->m_szInvitationCode[MAX_INVITATION_CODE_LEN - 1] = '\0';

        SetRoleName(pRoleInforDB->GetName().c_str(), false);

        std::string strSignature(pRoleInforDB->m_szSignature);
        NameDeny::Instance().FilterInvalidText(strSignature);

        m_nPID = pRoleInforDB->m_nPID;
        m_nAccount = pRoleInforDB->nAccount;
        m_nRoleID = pRoleInforDB->m_nRoleID;
        m_nSex = pRoleInforDB->m_nSex;
        m_nMoney = pRoleInforDB->m_nMoney;
        if (m_nMoney < 0)
        {
            m_nMoney = 0;
        }
        else if (m_nMoney > MAX_MONEY_NUM)
        {
            m_nMoney = MAX_MONEY_NUM;
        }
        m_nExp = pRoleInforDB->m_nExp;
        m_nLevel = pRoleInforDB->m_nLevel;
        m_nHot = pRoleInforDB->m_nHot;
        m_nConstellation = pRoleInforDB->m_nConstellation;
        m_nSkinColor = pRoleInforDB->m_nSkinColor;
        m_nPriv = pRoleInforDB->m_nPriv;
        m_nRoleCreateTime = pRoleInforDB->m_nRoleCreateTime;
        m_nLastLoginTime = pRoleInforDB->m_nLastLoginTime;
        m_nLastLogoutTime = pRoleInforDB->m_nLastLogoutTime;
        m_nTimeOnline = pRoleInforDB->m_nTimeOnline;
        m_nQuestDone = pRoleInforDB->m_nQuestDone;
        m_bCompleteTeachGuide = pRoleInforDB->bCompleteTeachGuide;
        m_guideData = pRoleInforDB->m_nGuideData;
        m_nCompleteProcGuide = pRoleInforDB->nCompleteProcGuide;
        m_bCompleteAnimation = pRoleInforDB->bCompleteAnimation;
        m_bReceivedInvitationReward = pRoleInforDB->bReceivedInvitationReward;
        m_nLastRstMoneyTime = pRoleInforDB->m_nLastMoneyResetTime;
        m_nMailLastMoneyRstRemindTime = pRoleInforDB->m_nLastMoneyResetMailRemindTime;
        m_nMallLastMoneyRstRemindtime = pRoleInforDB->m_nLastMoneyResetMallRemindTime;
        m_nAccountCreateTime = pRoleInforDB->m_nAccountCreateTime;
        for (unsigned char i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; i++)
        {
            m_anBeiBaoKuozhan[i] = pRoleInforDB->m_anBeiBaoKuozhan[i];
        }

        m_nBadgeGridNum = pRoleInforDB->m_nBadgeGridNum;

        SetBirthday(pRoleInforDB->GetBirthday());
        Formatter(m_szSignature, MAX_SIGNATURE_LEN) << strSignature;
        memcpy(m_szInvitationCode, pRoleInforDB->m_szInvitationCode, MAX_INVITATION_CODE_LEN - 1);
        m_nClothGroupId = (pRoleInforDB->m_nClothGroupId > 0 ? pRoleInforDB->m_nClothGroupId : 1);

        m_Bi = pRoleInforDB->bi;
        if (m_nLevel < 1)
        {
            m_nLevel = 1;
        }

        unsigned int nNowTime = (unsigned int)time(NULL);
        if (!IsTimeIsToday((time_t)(m_nLastLoginTime)) && m_nLastLoginTime < nNowTime)
        {
            SetTodayFirstLoginTag(true);
        }

        m_nLastChangeBirthdayTime = pRoleInforDB->m_nLastChangeBirthdayTime;

        m_nLastLoginTime = nNowTime;
        //默认设置成未首充
        pRoleInforDB->bCompleteFirstPaymentTag = false;
        pRoleInforDB->bCanSendFirstPaymentReward = true;

        if (pRoleInforDB->m_nFirstPaymentTime > 0 && CSystemSettingMgr::Instance().CanFirstPaymentReward() &&
            PaymentInfoMgr::Instance().FirstChargeRewardIsOpen((Version_Type)pRoleInforDB->m_nVersionType, pRoleInforDB->m_nFirstPaymentTime))
        {
            pRoleInforDB->bCompleteFirstPaymentTag = true;
        }
        if (pRoleInforDB->m_nGetFirstPaymentRewardTime > 0 && pRoleInforDB->bCompleteFirstPaymentTag && pRoleInforDB->m_nGetFirstPaymentRewardTime >= pRoleInforDB->m_nFirstPaymentTime)
        {
            pRoleInforDB->bCanSendFirstPaymentReward = false;
        }
        m_bCompleteFirstPaymentTag = pRoleInforDB->bCompleteFirstPaymentTag;
        m_bCanSendFirstPaymentReward = pRoleInforDB->bCanSendFirstPaymentReward;
        m_bReceivedProcGuideReward = pRoleInforDB->m_bReceiveProcGuideReward;
        m_pFirstPaymentInfoLastNotify = PaymentInfoMgr::Instance().GetFirstPaymentInfo((Version_Type)pRoleInforDB->m_nVersionType, pRoleInforDB->m_nFirstPaymentTime);

        for (size_t i = 0; i < pRoleInforDB->m_fixedChargeRecord.vecRecords.size(); ++i)
        {
            const FixedChargeRecordItem &item = pRoleInforDB->m_fixedChargeRecord.vecRecords[i];
            m_fixedCharges.insert(std::make_pair(item.m_nMCoin, item.m_nChargeTime)); // 
        }

        return true;
    }

    return false;
}

bool CEntityAttributeComponent::PacketToDB(ROLEINFO_DB*pRoleInforDB) const
{
    if (pRoleInforDB != NULL)
    {
        pRoleInforDB->nAccount = m_nAccount;
        pRoleInforDB->m_nRoleID = m_nRoleID;
        pRoleInforDB->SetName(m_szRoleName);
        pRoleInforDB->m_nSex = (unsigned char)m_nSex;
        pRoleInforDB->m_nMoney = m_nMoney;
        pRoleInforDB->m_nExp = m_nExp;
        pRoleInforDB->m_nLevel = m_nLevel;
        pRoleInforDB->m_nHot = m_nHot;
        pRoleInforDB->m_nConstellation = m_nConstellation;
        pRoleInforDB->m_nSkinColor = m_nSkinColor;
        pRoleInforDB->m_nPriv = m_nPriv;
        pRoleInforDB->m_nLastLoginTime = m_nLastLoginTime;
        pRoleInforDB->m_nLastLogoutTime = m_nLastLogoutTime;
        pRoleInforDB->m_nTimeOnline = m_nTimeOnline;
        pRoleInforDB->m_nQuestDone = m_nQuestDone;
        pRoleInforDB->bCompleteTeachGuide = m_bCompleteTeachGuide;
        pRoleInforDB->nCompleteProcGuide = m_nCompleteProcGuide;
        pRoleInforDB->bCompleteAnimation = m_bCompleteAnimation;
        pRoleInforDB->bCompleteFirstPaymentTag = m_bCanSendFirstPaymentReward;
        pRoleInforDB->bReceivedInvitationReward = m_bReceivedInvitationReward;
        pRoleInforDB->m_bReceiveProcGuideReward = m_bReceivedProcGuideReward;
        for (unsigned char i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; i++)
        {
            pRoleInforDB->m_anBeiBaoKuozhan[i] = m_anBeiBaoKuozhan[i];
        }

        pRoleInforDB->m_nBadgeGridNum = m_nBadgeGridNum;

        pRoleInforDB->SetBirthday(m_strBirthday);
        memcpy(pRoleInforDB->m_szSignature, m_szSignature, MAX_SIGNATURE_LEN - 1);
        memcpy(pRoleInforDB->m_szInvitationCode, m_szInvitationCode, MAX_INVITATION_CODE_LEN - 1);
        pRoleInforDB->m_nClothGroupId = (m_nClothGroupId > 0 ? m_nClothGroupId : 1);

        pRoleInforDB->bi = m_Bi;

        pRoleInforDB->m_bKeyDataModified = m_bKeyDataModified;
        unsigned int nTimeNow = (unsigned int)time(NULL);
        static const unsigned int kSaveAttributeInterval = 300;//玩家登录超过5分钟时，在玩家退出时保存玩家基本属性，加这个条件是为了避免死锁
        if (nTimeNow - m_nLastLoginTime >= kSaveAttributeInterval)
        {
            pRoleInforDB->m_bKeyDataModified = true;
        }

        if (GetTodayFirstLoginTag()) // 当天首次登陆时，保存玩家信息，防止周一重复签到
        {
            pRoleInforDB->m_bKeyDataModified = true;
        }

        return true;
    }

    return false;
}

void CEntityAttributeComponent::SerializeComponent(CParamPool &IOBuff)
{
    unsigned int now = (unsigned int)time(NULL);

    //赋值给pCreateMsg
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_szRoleName);
    IOBuff.AddUChar((unsigned char)m_nSex);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddUInt(m_nExp);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUInt(m_nHot);

    IOBuff.AddStringW(m_strBirthday);
    IOBuff.AddUChar(m_nConstellation);
    IOBuff.AddStringW(m_szSignature);
    //IOBuff.AddStringW(m_szInvitationCode);
    IOBuff.AddUChar(m_nSkinColor);
    IOBuff.AddUInt(m_nHot);

    for (unsigned char i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; i++)
    {
        IOBuff.AddUChar(m_anBeiBaoKuozhan[i]);
    }

    IOBuff.AddUChar(m_nBadgeGridNum);

    IOBuff.AddUInt(m_Bi.nPoint);
    IOBuff.AddUInt(m_Bi.nTotalPoint);
    IOBuff.AddUInt(m_Bi.nBindPoint);
    IOBuff.AddUInt(m_Bi.nChange);
    IOBuff.AddBool(m_bCompleteTeachGuide);
    IOBuff.AddUChar(m_nCompleteProcGuide);
    IOBuff.AddBool(m_bCompleteAnimation);
    //IOBuff.AddBool(m_bReceivedInvitationReward);
    // Ligh add for guide data.
    IOBuff.AddUInt(m_guideData);

    IOBuff.AddInt(g_nLine);
    IOBuff.AddStringW(g_pCirculator->GetServerName().c_str());
    IOBuff.AddBool(g_pCirculator->IsEnableAudition());

    if (CSystemSettingMgr::Instance().CanFirstPaymentReward() &&
        PaymentInfoMgr::Instance().FirstChargeRewardIsOpen((Version_Type)m_pRoleNet->GetVerType(), (int)time(NULL)))
    {
        IOBuff.AddBool(m_bCompleteFirstPaymentTag);
    }
    else
    {
        IOBuff.AddBool(true);
    }
    IOBuff.AddInt(m_nTransformId);



    unsigned int resetTime = CSystemSettingMgr::Instance().GetBirthdayResetTime();
    bool canChangeBirthday = (resetTime <= now && m_nLastChangeBirthdayTime < resetTime);
    IOBuff.AddBool(canChangeBirthday);

    // 定额首冲信息下发
    //     bool canOpen = PaymentInfoMgr::Instance().CanOpenFirstCharge(now);
    //     IOBuff.AddBool(canOpen);
    //     if (canOpen)
    //     {
    //         IOBuff.AddUShort(PaymentInfoMgr::Instance().m_mapGiftFixedCharge.size());
    //         for (PaymentInfoMgr::FixedChargeGiftMCoinMap::iterator iter = PaymentInfoMgr::Instance().m_mapGiftFixedCharge.begin();
    //             iter != PaymentInfoMgr::Instance().m_mapGiftFixedCharge.end(); ++iter)
    //         {
    //             std::string strItem = PaymentInfoMgr::Instance().FixedChargeGiftItem(iter->first);
    //             iter->second; // binder point.
    //             bool bRecharged = m_fixedCharges.find(iter->first) != m_fixedCharges.end();
    // 
    //             IOBuff.AddStringW(strItem);
    //             IOBuff.AddUInt(iter->second);
    //             IOBuff.AddBool(bRecharged);
    //         }
    //     }
}

void CEntityAttributeComponent::_UpdateFirstPayment(unsigned int nTimeNow)
{
    Version_Type versionType = (Version_Type)m_pRoleNet->GetVerType();
    // 更新客户端首充/充值标志
    FirstPaymentInfo* pFirstPaymentInfoNow = PaymentInfoMgr::Instance().GetFirstPaymentInfo(versionType, nTimeNow);
    if (m_pFirstPaymentInfoLastNotify != pFirstPaymentInfoNow)
    {
        GameMsg_S2C_FirstPaymentStateChange msgIsOpen;
        msgIsOpen.m_bFirstPaymentRewardsIsOpen = (pFirstPaymentInfoNow != NULL);
        SendPlayerMsg(&msgIsOpen);

        m_pFirstPaymentInfoLastNotify = pFirstPaymentInfoNow;

        //首充时间段结束
        if (!msgIsOpen.m_bFirstPaymentRewardsIsOpen)
        {
            m_bCompleteFirstPaymentTag = false;
            m_bCanSendFirstPaymentReward = true;
        }
    }
}

void CEntityAttributeComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
    unsigned int tPre = m_nTimeOnline;
    bool bUpdated = false;
    m_nUpdateDelay += nTimeElapsed;
    if (m_nUpdateDelay >= ATTRIBUTE_UPDATE_DELAY)
    {
        unsigned int nRes = m_nUpdateDelay % 1000;
        m_nTimeOnline += (m_nUpdateDelay - nRes) / 1000;
        m_nUpdateDelay = nRes;
        bUpdated = true;
    }

    if ((m_nTimeOnline / SECONDSPERMINUTE) - (tPre / SECONDSPERMINUTE) >= 1 && bUpdated) // per minute
    {
        if (m_pRoleAch != NULL) //在线时长增长
        {
            m_pRoleAch->UpdateAchievement(AchievementType_HistoryTimeOnline);	// 自动遍历更新
        }
    }

    unsigned int nTimeNow = (unsigned int)time(NULL);
    unsigned int nCurrentMonthResetTime = CMoneyMgr::Instance().GetCurMonthMoneyRstTime(nTimeNow);
    if (m_nLastRstMoneyTime < nCurrentMonthResetTime && nTimeNow > nCurrentMonthResetTime)
    {
        WriteLog(LOGLEVEL_DEBUG, "RESET MONEY(OnUpdate): roleid=%u, lastResetTime=%u, nowTime=%u, "
            "currentMonthResetTime=%u", m_nRoleID, m_nLastRstMoneyTime, nTimeNow, nCurrentMonthResetTime);

        m_nLastRstMoneyTime = nTimeNow;
        ChangeMoney(-m_nMoney, EChangeMoneyCause_Monthly_Reset, 0);
        AddQuery(QUERY_UpdateResetMoneyTime, m_nRoleID, NULL, nTimeNow);
    }

    if (bUpdated) // 延迟更新
    {
        _UpdateFirstPayment(nTimeNow);
    }
}

void CEntityAttributeComponent::OnLogin()
{
    unsigned int nTimeNow = time(NULL);
    CMoneyMgr &rMoneyMgr = CMoneyMgr::Instance();

    if (m_nLastRstMoneyTime == 0)
    {
        m_nLastRstMoneyTime = rMoneyMgr.GetCurMonthMoneyRstTime( nTimeNow );
        AddQuery(QUERY_UpdateResetMoneyTime, m_nRoleID, NULL, m_nLastRstMoneyTime);
    }
    else
    {
        unsigned int nCompareTime = rMoneyMgr.GetCompareRstTime( m_nLastRstMoneyTime );
        if (nTimeNow >= nCompareTime)
        {
            WriteLog(LOGLEVEL_DEBUG, "RESET MONEY(OnLogin): roleid=%u, lastResetTime=%u, nowTime=%u, "
                "compareTime=%u", m_nRoleID, m_nLastRstMoneyTime, nTimeNow, nCompareTime);

            m_nLastRstMoneyTime = nTimeNow;
            ChangeMoney(-m_nMoney, EChangeMoneyCause_Monthly_Reset, 0);
            AddQuery(QUERY_UpdateResetMoneyTime, m_nRoleID, NULL, nTimeNow);
        }
    }

    if ( nTimeNow >= rMoneyMgr.GetCurMonthRemindTime( nTimeNow )
        && nTimeNow < rMoneyMgr.GetNextMonthMoneyRstTime( nTimeNow )
        && m_nMailLastMoneyRstRemindTime < rMoneyMgr.GetCurMonthRemindTime( nTimeNow ) )
    {
        m_nMailLastMoneyRstRemindTime = nTimeNow;
        if (m_pRoleMail != NULL)
        {
            CMailMgr::Instance().SendSystemMailToPlayer( m_nRoleID, EMailType_MoneyReset, rMoneyMgr.MailTitle(), rMoneyMgr.MailContent() );
        }
        AddQuery(QUERY_UpdateMoneyMailRemindTime, m_nRoleID, NULL, nTimeNow);
    }
}

void  CEntityAttributeComponent::OnLogout()
{
    CLocationMgr::Instance().PlayerLogout(m_nRoleLocation, m_nRoleID);
}

void CEntityAttributeComponent::OnEvent(CComponentEvent& refEvent)
{

}


void CEntityAttributeComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNet != NULL && pMsg != NULL)
    {
        m_pRoleNet->SendPlayerMsg(pMsg);
    }
}

void CEntityAttributeComponent::SendGroupMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNet != NULL && pMsg != NULL)
    {
        m_pRoleNet->Send2GroupServer(pMsg);
    }
}

void CEntityAttributeComponent::TodayFirstLogin()//第一次登入完。
{
}

void CEntityAttributeComponent::SetBadgeGridNum(unsigned char nNum)
{
    m_nBadgeGridNum = nNum;
    AddQuery(QUERY_UpdateRoleBadgeNum, m_nBadgeGridNum, NULL, GetRoleID());
}

void CEntityAttributeComponent::SetGroupUpdateAttrInfo(CWorldRoleUpdateAttrInfo &updateAttrInfo)
{
    updateAttrInfo.m_nRoleID = GetRoleID();
    memcpy(updateAttrInfo.m_szRoleName, GetRoleName(), g_ServerConfig.m_MaxNameLimit - 1);
    updateAttrInfo.m_nMoney = GetMoney();
    updateAttrInfo.m_nLevel = GetLevel();
}

void CEntityAttributeComponent::SendGroupUpdateAttrInfo(CWorldRoleUpdateAttrInfo &updateAttrInfo)
{
    GameMsg_S2G_UpdateAttrInfo updateMsg = GameMsg_S2G_UpdateAttrInfo(updateAttrInfo);
    SendPlayerMsg(&updateMsg);
}

void CEntityAttributeComponent::UpdateGroupAttrInfo()
{
    CWorldRoleUpdateAttrInfo updateAttrInfo;
    SetGroupUpdateAttrInfo(updateAttrInfo);
    SendGroupUpdateAttrInfo(updateAttrInfo);
}

void CEntityAttributeComponent::OnChangeName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeRoleName *pChangeMsg = (GameMsg_C2S_ChangeRoleName *)&msg;
    if (!NameDeny::Instance().IsValidText(pChangeMsg->m_szNewName))
    {
        GameMsg_S2C_ChangeRoleNameResult resultMsg(CHANGEROLENAME_INVADROLENAME);
        resultMsg.m_nRoleID = m_nRoleID;
        SendPlayerMsg(&resultMsg);
    }
    else if (m_pRoleItem->IsItemInCoolDown(ITEMID_CHANGENAMECARD))
    {
        GameMsg_S2C_ChangeRoleNameResult resultMsg(CHANGEROLENAME_CD);
        resultMsg.m_nRoleID = m_nRoleID;
        SendPlayerMsg(&resultMsg);	// 修改昵称CD，客户端提示需要修改
    }
    else if (m_pRoleItem->SearchItem(ITEMID_CHANGENAMECARD) != NULL)
    {
        char * szNewName = new char[MAX_NAME_LEN];
        memset(szNewName, 0, MAX_NAME_LEN);
        memcpy(szNewName, pChangeMsg->m_szNewName, g_ServerConfig.m_MaxNameLimit - 1);
        CDelHelper delHelper(szNewName, MAX_NAME_LEN);
        AddQuery(QUERY_ChangeRoleName, m_nRoleID, szNewName, 0, NULL, &delHelper);
    }
}

void CEntityAttributeComponent::OnChangeRoleNameCallBack(QueryBase & query)
{
    char *pNewName = (char *)query.m_pAttachment;

    if (query.m_nResult != 0)
    {
        GameMsg_S2C_ChangeRoleNameResult resultMsg(CHANGEROLENAME_DUPILICATED);
        resultMsg.m_nRoleID = m_nRoleID;
        SendPlayerMsg(&resultMsg);
    }
    else
    {
        if (m_pRoleItem->RemoveItemByType(ITEMID_CHANGENAMECARD, 1, EItemAction_Del_UseItem))
        {
            SetRoleName(pNewName);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "扣除改名卡失败:roleid: %u , rolename: %s", GetRoleID(), GetRoleName());
        }
    }

    delete[] pNewName, pNewName = NULL;
}

void CEntityAttributeComponent::OnChargeCallBack(QueryBase & query)
{
    ChongZhiInfo * pInfo = (ChongZhiInfo *)query.m_pAttachment;
    if (query.m_nResult == 0)
    {
        m_Bi.nPoint += pInfo->nAmount;
        m_Bi.nTotalPoint += pInfo->nAmount;

        GameMsg_S2C_RefreshPt msg;
        msg.m_nPoint = m_Bi.nPoint;
        msg.m_nTotalPoint = m_Bi.nTotalPoint;
        SendPlayerMsg(&msg);
    }

    delete pInfo, pInfo = NULL;
}

void CEntityAttributeComponent::OnCompleteTeachGuideCallBack(QueryBase &query)
{
    if (query.m_nResult == 0) {
        m_bCompleteTeachGuide = true;
        GameMsg_S2C_CompleteGuideResult sendMsg;
        SendPlayerMsg(&sendMsg);
    }
}

void CEntityAttributeComponent::OnCompleteProcGuideCallBack(QueryBase &query)
{
    if (query.m_nResult == 0) {
        GameMsg_S2C_CompleteProcGuideResult sendMsg;
        SendPlayerMsg(&sendMsg);
    }
}
void CEntityAttributeComponent::OnClearFirstPaymentInfoCallBack(QueryBase& query)
{
    if (query.m_nResult == Query_Res_OK)
    {
        WriteLog(LOGLEVEL_DEBUG, "[INFO]-[@ OnClearFirstPaymentInfoCallBack]-[ ClearFirstPaymentInfo Query_Res_OK ]");
        m_bCompleteFirstPaymentTag = false;
        m_bCanSendFirstPaymentReward = true;

        GameMsg_S2C_FirstPaymentStateChange msgIsOpen;
        msgIsOpen.m_bFirstPaymentRewardsIsOpen = true;
        SendPlayerMsg(&msgIsOpen);
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR]-[@ OnClearFirstPaymentInfoCallBack]-[ ClearFirstPaymentInfo Query_Res_Fail ]");
    }
}

void CEntityAttributeComponent::OnCompleteProcAnimationCallBack(QueryBase& query)
{
    if (query.m_nResult == 0)
    {
        m_bCompleteAnimation = true;
        GameMsg_S2C_CompleteAnimationResult msg;
        SendPlayerMsg(&msg);
    }
}

void CEntityAttributeComponent::OnChangeSignature(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeRoleSignature *pChangeMsg = (GameMsg_C2S_ChangeRoleSignature *)&msg;
    GameMsg_S2C_ChangeRoleSignatureResult resultMsg;
    if (pChangeMsg != NULL)
    {
        std::string *pstrSignature = new std::string(pChangeMsg->m_szNewSignature);

        NameDeny::Instance().FilterInvalidText(*pstrSignature);

        Formatter(m_szSignature, g_ServerConfig.m_SignatureLength) << *pstrSignature;

        CDelHelper delHelper(pstrSignature);
        AddQuery(QUERY_UpdateRoleSignature, m_nRoleID, pstrSignature, 0, NULL, &delHelper);

        resultMsg.m_nResult = CHANGEROLESIGNATURE_SUCCESS;
        memcpy(resultMsg.m_szNewSignature, m_szSignature, g_ServerConfig.m_SignatureLength);
    }
    SendPlayerMsg(&resultMsg);
}

void CEntityAttributeComponent::OnCompleteTeachGuide(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_CompleteTeachGuide *pMsg = (GameMsg_C2S_CompleteTeachGuide *)&msg;
    if (pMsg != NULL && !m_bCompleteTeachGuide) {
        AddQuery(QUERY_AddCompleteTeachGuideEvent, GetRoleID());
    }
}

void CEntityAttributeComponent::OnCompleteAnimation(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (!m_bCompleteAnimation)
    {
        AddQuery(QUERY_AddCompleteAnimation, GetRoleID());
    }
}

void CEntityAttributeComponent::OnCompleteProcGuide(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_CompleteProcGuide &rMsg = (GameMsg_C2S_CompleteProcGuide &)msg;
    int nProcStep = rMsg.m_chProcStep;

    if (nProcStep > m_nCompleteProcGuide && nProcStep < MAX_GUIDE_STEP)
    {
        m_nCompleteProcGuide = (char)nProcStep;

        AddQuery(QUERY_AddCompleteProcGuideEvent, GetRoleID(), NULL, nProcStep);
    }

    //if (!m_bCompleteProcGuide) {
    //	m_bCompleteProcGuide = true;
    //	AddQuery(QUERY_AddCompleteProcGuideEvent, GetRoleID(), NULL, nProcStep);
    //}
    //else {
    //	AddQuery(QUERY_UpdateCompleteProcGuideEvent, GetRoleID(), NULL, nProcStep);
    //}
}

void CEntityAttributeComponent::OnChangeRoleSignatureCallBack(QueryBase & query)
{
}

void CEntityAttributeComponent::OnChangeBirthday(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeBirthday *pChangeMsg = (GameMsg_C2S_ChangeBirthday *)&msg;

    if (NULL != pChangeMsg)
    {
        int nYear = atoi(pChangeMsg->m_szYear);
        int nMonth = atoi(pChangeMsg->m_szMonth);
        int nDay = atoi(pChangeMsg->m_szDay);
        unsigned int now = (unsigned int)time(NULL);
        unsigned int resetTime = CSystemSettingMgr::Instance().GetBirthdayResetTime();

        GameMsg_S2C_ChangeBirthdayRes resultMsg(CHANGEBIRTHDAY_SUCCESS);

        if (!IsValidBirthDay(nYear, nMonth, nDay))
        {
            resultMsg.m_nResult = CHANGEBIRTHDAY_INVALID_BIRTHDAY;

            LOG_ERROR << "Change birthday error: roleid: " << m_nRoleID
                << ", new birthday: " << nYear << "-" << nMonth << "-" << nDay
                << ", result: CHANGEBIRTHDAY_INVALID_BIRTHDAY" << endl;
        }
        else if (!(resetTime <= now && m_nLastChangeBirthdayTime < resetTime) && m_nLastChangeBirthdayTime != 0)
        {
            resultMsg.m_nResult = CHANGEBIRTHDAY_CANNOT_CHANGE;

            LOG_ERROR << "Change birthday error: roleid: " << m_nRoleID
                << ", new birthday: " << nYear << "-" << nMonth << "-" << nDay
                << ", result: CHANGEBIRTHDAY_CANNOT_CHANGE" << endl;
        }
        else
        {
            int nBirthday = nYear * 10000 + nMonth * 100 + nDay;
            ostringstream oss;

            oss << nBirthday;

            string *pStrBirthday = new string(oss.str());

            if (NULL == pStrBirthday)
            {
                LOG_ERROR << "Can not create string instance." << std::endl;

                return;
            }
            else if (*pStrBirthday == m_strBirthday)
            {
                delete pStrBirthday;
                pStrBirthday = NULL;

                resultMsg.m_nResult = CHANGEBIRTHDAY_IS_SAME;

                LOG_WARNING << "Change birthday warning: roleid: " << m_nRoleID
                    << ", new birthday: " << nYear << "-" << nMonth << "-" << nDay
                    << ", result: CHANGEBIRTHDAY_IS_SAME" << endl;
            }
            else
            {
                LogRoleAttrChange(m_nRoleID, m_strBirthday, ERoleAttrChangeCause_ChangeBirthday, *pStrBirthday);

                SetBirthday(*pStrBirthday);
                resultMsg.SetBirthday(*pStrBirthday);

                m_nConstellation = (unsigned char)checkConstellation(nMonth, nDay);
                resultMsg.m_nConstellation = m_nConstellation;

                m_nLastChangeBirthdayTime = now;

                AddQuery(QUERY_ChangeBirthday, m_nRoleID, pStrBirthday, 0, NULL);
                NotifyGroupRoleAgeChange();
            }
        }

        SendPlayerMsg(&resultMsg);
    }
}

bool CEntityAttributeComponent::IsGuideFinish(EGuideBit eGuide)
{
    return (m_guideData & eGuide) != 0;
}

void CEntityAttributeComponent::SetGuideFinish(EGuideBit eGuide)
{
    m_guideData |= eGuide;
}

void CEntityAttributeComponent::FlushGuidData()
{
    AddQuery(QUERY_UpdateGuideDataEvent, GetRoleID(), NULL, m_guideData);
}

void CEntityAttributeComponent::OnReciveGuideFinish(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestGuideFinish & finishmsg = (GameMsg_C2S_RequestGuideFinish&)msg;

    if (!IsGuideFinish((EGuideBit)finishmsg.m_nGuideEnum))
    {
        SetGuideFinish((EGuideBit)finishmsg.m_nGuideEnum);
        AddQuery(QUERY_UpdateGuideDataEvent, GetRoleID(), NULL, m_guideData);
    }

    GameMsg_S2C_RequestGuideFinishResult resultmsg;
    resultmsg.m_nGuideData = m_guideData;
    SendPlayerMsg(&resultmsg);
}

void CEntityAttributeComponent::OnReciveGuideExperienceCard(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (NULL == m_pRoleItem)
    {
        WriteLog(LOGLEVEL_ERROR, "error to get item Conponenet.");
        return;
    }

    GameMsg_S2C_ReciveGuideExperienceCardRes retMsg;

    do
    {
        if ((m_guideData & EGuideBit_RecivedExperienceCard) != 0)
        {
            WriteLog(LOGLEVEL_ERROR, "role[%u] has recived experineced card.", GetRoleID());
            retMsg.m_nRes = 1; // 已经领取
            break;
        }

        ItemConfig* config = ConfigManager::Instance().GetItemConfigManager().GetByID(CMallDataMgr::Instance().m_experienceCardItemType);
        if (NULL == config)
        {
            WriteLog(LOGLEVEL_ERROR, "error to get config of experience card");
            break;
        }

        std::list<CItem> rewardItem;
        rewardItem.push_back(CItem(CMallDataMgr::Instance().m_experienceCardItemType, 1, config->m_nDueTime, false, 0));
        if (!CItemProcess::AddItems(*(CRoleEntity*)Entity(), rewardItem, EItemAction_Add_ProcGuide, 0, true))
        {
            WriteLog(LOGLEVEL_ERROR, "role[%u] add ExperienceCard failed..", GetRoleID());
            return;
        }

        m_guideData |= EGuideBit_RecivedExperienceCard; // 代表已经使用过体验卡

        AddQuery(QUERY_UpdateGuideDataEvent, GetRoleID(), NULL, m_guideData);

    } while (0);

    SendPlayerMsg(&retMsg);

    return;
}

void CEntityAttributeComponent::OnFixedChargeReq(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendFixedChargeMsg(false, 0);

    return;
}


void CEntityAttributeComponent::NotifyGroupRoleNameChange()
{
    GameMsg_S2G_UpdateRoleName updateMsg(GetRoleID(), GetRoleName());
    SendGroupMsg(&updateMsg);
}

void CEntityAttributeComponent::NotifyGroupLevelChange()
{
    GameMsg_S2G_UpdateLevel updateMsg(GetRoleID(), GetLevel());
    SendGroupMsg(&updateMsg);
}

void CEntityAttributeComponent::NotifyGroupMoneyChange()
{
    GameMsg_S2G_UpdateMoney updateMsg(GetRoleID(), GetMoney());
    SendGroupMsg(&updateMsg);
}

void CEntityAttributeComponent::NotifyGroupRoleAgeChange()
{
    GameMsg_S2G_UpdateRoleAge updateMsg(GetRoleID(), GetAge(), m_nConstellation);
    SendGroupMsg(&updateMsg);
}

void CEntityAttributeComponent::_SendLevelUpReward(unsigned short nOldLevel, unsigned short nNewLevel)
{
    if (m_pRoleItem == NULL)
        return;

    CLevelRewardInfo rewardInfo;
    CLevelRewardInfo vipRewardInfo;

    // 发奖励
    int nCurLevel = nOldLevel + 1;
    while (nCurLevel <= nNewLevel)
    {
        if (CLevelUpInfoMgr::Instance().IsRewardLevel((unsigned short)nCurLevel))
        {
            CLevelUpInfoMgr::Instance().GetRewardInfo((unsigned short)nCurLevel, rewardInfo);
            _SendLevelUpReward(rewardInfo);
        }

        if (CLevelUpInfoMgr::Instance().IsVIPRewardLevel((unsigned short)nCurLevel))
        {
            CLevelUpInfoMgr::Instance().GetVIPRewardInfo((unsigned short)nCurLevel, vipRewardInfo);

            if (m_pRoleVIP != NULL && m_pRoleVIP->IsVIP())
                _SendLevelUpReward(vipRewardInfo);
        }

        ++nCurLevel;
    }

    if (m_pRoleVIP == NULL || !m_pRoleVIP->IsVIP())
        m_nNotGetLevelUpVIPRewardLevel = nNewLevel;

    // 通知客户端，每升级就提示，方便客户端处理
    GameMsg_S2C_LevelUpRewardInfo msgResult;
    msgResult.m_nCurrLevel = nNewLevel;

    msgResult.m_nCurrLevelMoney = rewardInfo.m_nMoney;
    msgResult.m_nCurrLevelBindMCoin = rewardInfo.m_nBindMCoin;
    msgResult.m_listCurrLevelReward =
        (m_nSex == ESexType_Male ? rewardInfo.m_listMaleItem : rewardInfo.m_listFemaleItem);

    msgResult.m_nCurrLevelVIPMoney = vipRewardInfo.m_nMoney;
    msgResult.m_nCurrLevelVIPBindMCoin = vipRewardInfo.m_nBindMCoin;
    msgResult.m_listCurrLevelVIPReward =
        (m_nSex == ESexType_Male ? vipRewardInfo.m_listMaleItem : vipRewardInfo.m_listFemaleItem);

    // 现在客户端不需要下一级的奖励信息，如果需要加回来，就得再测试一下 -- 2015-04-25
    //unsigned short nNextRewardLevel = CLevelUpInfoMgr::Instance().GetNextRewardLevel(nNewLevel);
    //msgResult.m_nNextLevel = nNextRewardLevel;
    //if (nNextRewardLevel > nNewLevel)
    //{
    //    CLevelRewardInfo nextRewardInfo;
    //    CLevelRewardInfo nextVIPRewardInfo;
    //    CLevelUpInfoMgr::Instance().GetRewardInfo(nNextRewardLevel, nextRewardInfo);
    //    CLevelUpInfoMgr::Instance().GetVIPRewardInfo(nNextRewardLevel, nextVIPRewardInfo);

    //    msgResult.m_nNextLevelMoney = nextRewardInfo.m_nMoney;
    //    msgResult.m_nNextLevelBindMCoin = nextRewardInfo.m_nBindMCoin;
    //    msgResult.m_listNextLevelReward = 
    //        (m_nSex == SexType_Male ? nextRewardInfo.m_listMaleItem : nextRewardInfo.m_listFemaleItem);

    //    msgResult.m_nNextLevelVIPMoney = nextVIPRewardInfo.m_nMoney;
    //    msgResult.m_nNextLevelVIPBindMCoin = nextVIPRewardInfo.m_nBindMCoin;
    //    msgResult.m_listNextLevelVIPReward = 
    //        (m_nSex == SexType_Male ? nextVIPRewardInfo.m_listMaleItem : nextVIPRewardInfo.m_listFemaleItem);
    //}
    SendPlayerMsg(&msgResult);
}

void CEntityAttributeComponent::_SendLevelUpReward(const CLevelRewardInfo &rRewardInfo)
{
    // 发奖励（包括金券、绑定M币和奖励物品）
    if (rRewardInfo.m_nMoney > 0)
    {
        ChangeMoney(rRewardInfo.m_nMoney, EChangeMoneyCause_Add_LevelUpReward, 0);
    }

    if (rRewardInfo.m_nBindMCoin > 0)
    {
        ChangeBindBill(rRewardInfo.m_nBindMCoin, EChangeBindBillCause_Add_LevelUpReward);
    }

    std::list<CItem> listReward;
    if (m_nSex == ESexType_Male)
    {
        listReward = rRewardInfo.m_listMaleItem;
    }
    else if (m_nSex == ESexType_Female)
    {
        listReward = rRewardInfo.m_listFemaleItem;
    }

    if (listReward.size() > 0)
    {
        std::string strMailSender(CLocalization::Instance().GetString("Level_Up_Reward_Mail_Sender"));
        std::string strMailTitle(CLocalization::Instance().GetString("Level_Up_Reward_Mail_Title"));
        std::string strMailContent(CLocalization::Instance().GetString("Level_Up_Reward_Mail_Content"));
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_LevelUpReward, 0, true,
            EMailType_LevelUp_Reward, strMailSender, strMailTitle, strMailContent);
    }
}

//角色添加经验日志记录
void CEntityAttributeComponent::_LogAddExp(unsigned int nRoleID, CRoleExpLog::EAddExpCause eAddExpCause, unsigned int nAddExp, unsigned int nNewLevel, unsigned int nNewExp)
{
    CRoleExpLog * pRoleExpLog = new CRoleExpLog();
    pRoleExpLog->m_nRoleID = nRoleID;
    pRoleExpLog->m_eAddExpCause = eAddExpCause;
    pRoleExpLog->m_nAddExp = nAddExp;
    pRoleExpLog->m_nNewLevel = nNewLevel;
    pRoleExpLog->m_nNewExp = nNewExp;

    AddQuery(QUERY_LogRoleExp, 0, pRoleExpLog);
}

//GM命令添加M币
void CEntityAttributeComponent::AddMCoinByCmd(unsigned int nValue)
{
    GameMsg_S2A_AddMCoinByCmd s2aMsg;
    s2aMsg.nAccount = m_nAccount;
    s2aMsg.nAmount = nValue;
    SendMsg2AccountServer(&s2aMsg);
}

//GM命令添加绑定M币
void CEntityAttributeComponent::AddBindMCoinByCmd(unsigned int nValue)
{
    ChangeBindBill(nValue, EChangeBindBillCause_Add_GMCommand);
}

//角色改变M币日志
void CEntityAttributeComponent::_LogRoleChangeBill(unsigned int nRoleID, int nChangeValue, EChangeBillCause eChangeBillCause, unsigned int nNewPT)
{
    CRoleChangeBillLog * pLog = new CRoleChangeBillLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_nChangeValue = nChangeValue;
    pLog->m_eChangeBillCause = eChangeBillCause;
    pLog->m_nNewPT = nNewPT;
    AddQuery(QUERY_LogRoleChangeBill, 0, pLog);
}

//角色改变绑定M币日志
void CEntityAttributeComponent::_LogRoleChangeBindBill(unsigned int nRoleID, int nChangeValue, EChangeBindBillCause eCause, unsigned int nNewBindPT)
{
    CRoleChangeBindBillLog * pLog = new CRoleChangeBindBillLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_nChangeValue = nChangeValue;
    pLog->m_eChangeBindBillCause = eCause;
    pLog->m_nNewBindPT = nNewBindPT;
    AddQuery(QUERY_LogRoleChangeBindBill, 0, pLog);
}

//角色改变金券日志
void CEntityAttributeComponent::LogRoleChangeMoney(unsigned int nRoleID, int nChangeValue, EChangeMoneyCause eChangeMoneyCause, int nNewMoney)
{
    CRoleChangeMoneyLog * pLog = new CRoleChangeMoneyLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_nChangeValue = nChangeValue;
    pLog->m_eChangeMoneyCause = eChangeMoneyCause;
    pLog->m_nNewMoney = nNewMoney;
    g_pQueryMgr->AddQuery(QUERY_LogRoleChangeMoney, 0, pLog);
}

void CEntityAttributeComponent::LogRoleAttrChange(unsigned int nRoleID, const std::string &strOldAttrValue, ERoleAttrChangeCause eChangeCause, const std::string &strNewAttrValue)
{
    CRoleAttrChangeLog *pLog = new CRoleAttrChangeLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eChangeCause = eChangeCause;
    pLog->m_strOldAttrValue = strOldAttrValue;
    pLog->m_strNewAttrValue = strNewAttrValue;
    AddQuery(QUERY_LogRoleAttrChange, 0, pLog);
}

bool CEntityAttributeComponent::CanBeVIPKick()
{
    int ntime = time(NULL) - m_nAccountCreateTime;
    if (ntime < 172800)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CEntityAttributeComponent::_SendUpgradeInstallerMessage()
{
    bool isFresherLine = (g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ? true : false);
    if (isFresherLine && m_nLevel >= CSystemSettingMgr::Instance().UpgradeInstallerLevel())
    {
        GameMsg_S2C_UpgradeInstaller msg;
        SendPlayerMsg(&msg);
    }
}

void CEntityAttributeComponent::OnSelectGuideFace(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_SelectGuideFace& sMsg = (GameMsg_C2S_SelectGuideFace&)msg;
    std::list<CItem> items;
    GuideRewardManager& gMgr = GuideRewardManager::Instance();
    int nStep = 0;
    if (m_bCompleteAnimation)
    {
        nStep = 1;
    }
    bool bRet = gMgr.GetGuideReward(nStep, m_nSex, sMsg.m_Face, items);
    if (bRet && !m_bReceivedProcGuideReward)
    {
        //CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), items, EItemAction_Add_LevelUpReward, 0, true, EMailType_LevelUp_Reward,gMgr.MailTitle(),gMgr.MailContent() );
        bool bAdd = CItemProcess::AddItems(*(CRoleEntity*)Entity(), items, EItemAction_Add_ProcGuide, 0, false);
        if (bAdd)
        {
            //写入领取奖励标记到数据库
            m_bReceivedProcGuideReward = true;
            AddQuery(QUERY_InsertProcGuideReward, m_nRoleID, NULL, 0, NULL, NULL);

            if (m_pRoleItem != NULL)
            {
                std::list<CItem>::iterator itItem = items.begin();
                for (; itItem != items.end(); itItem++)
                {
                    ItemBase* pItemFind = m_pRoleItem->SearchItem(itItem->m_nItemType, true);
                    if (pItemFind != NULL)
                    {
                        m_pRoleItem->OnEquip((ItemEquip*)pItemFind, true);
                    }
                    else
                    {
                        WriteLog(LOGLEVEL_ERROR, "OnSelectGuideFace Error!,SearchItem return false , item type = %d .\n", (int)itItem->m_nItemType);
                    }
                }
            }
        }
    }
    GameMsg_S2C_SelectGuideFaceRet retMsg;
    retMsg.SetItems(items);
    retMsg.m_Result = (unsigned char)bRet;
    SendPlayerMsg(&retMsg);
}

int CEntityAttributeComponent::GetTransformId() const
{
    return m_nTransformId;
}

void CEntityAttributeComponent::SetTransformIdAndNotify(int val)
{
    if (m_nTransformId != val)
    {
        m_nTransformId = val;

        GameMsg_S2C_RefreshTransform msg;
        msg.nTransformId = val;
        SendPlayerMsg(&msg);

        GameMsg_S2G_TransFromIDUpdate sgMsg;
        sgMsg.m_nTransformId = val;
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }
}
