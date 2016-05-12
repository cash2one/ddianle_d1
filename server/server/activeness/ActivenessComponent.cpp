#include "ActivenessComponent.h"
#include "ActivenessDataManager.h"
#include "ActivenessService.h"
#include "GameMsg_Activeness.h"

#include "../../socket/GameMsg_Map.h"
#include "../util/CommonFunc.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/ItemProcess.h"
#include "../util/Split.h"

#include "../CheckIn/EntityCheckInComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../room/RoomComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../activity/EntityActivityComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../bet/BetComponent.h"

#include <memory>

ActivenessComponent::ActivenessComponent()
: CEntityComponent(true, true)
, m_pRoleAttr(NULL)
, m_pRoleNet(NULL)
, m_nActivenessScore(0)
, m_nUpdateTime(0)
{
}

ActivenessComponent::~ActivenessComponent()
{
}

void ActivenessComponent::Start()
{
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleNet = GetComponent<CRoleNet>();

    // 添加活跃度系统观察者
    GetComponent<CRoleActivity>()->AddSubscriber(this);
    GetComponent<CRoleCheckIn>()->AddSubscriber(this);
    GetComponent<CRoleDungeon>()->AddSubscriber(this);
    GetComponent<CRoleQuestNew>()->AddSubscriber(this);
    GetComponent<CRoleRoom>()->AddSubscriber(this);
    GetComponent<CRoleVIP>()->AddSubscriber(this);
    GetComponent<CRoleBet>()->AddSubscriber(this);
    GetComponent<CRoleItem>()->AddSubscriber(this);
}

bool ActivenessComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
    if (pRoleInfoDB != NULL)
    {
        RoleActivenessDB &activeness = pRoleInfoDB->m_activeness;

        m_nActivenessScore = activeness.m_nScore;
        m_nUpdateTime = activeness.m_nUpdateTime;

        // 只有使用过活跃度系统，才有相应的数据需要解析，要是没有使用过，就只做内存初始化
        if (m_nUpdateTime != 0)
        {
            SplitString(activeness.m_strRewardLevelScore, std::string("|"), m_setRewardLevelScore);
            Split(activeness.m_strTargetProgress, std::string("|"), std::string(","), m_mapTargetProgress);
            SplitString(activeness.m_strDanceMode, std::string(","), m_setFinishedDanceMode);
        }
    }

    return true;
}

bool ActivenessComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    pRoleInfoCache->m_activeness.m_nScore = m_nActivenessScore;
    pRoleInfoCache->m_activeness.m_nUpdateTime = m_nUpdateTime;

    if (0 != m_nUpdateTime)
    {
        ConcatToString(m_mapTargetProgress, std::string("|"), std::string(","), pRoleInfoCache->m_activeness.m_strTargetProgress);
        ConcatToString(m_setFinishedDanceMode, std::string(","), pRoleInfoCache->m_activeness.m_strDanceMode);
        ConcatToString(m_setRewardLevelScore, "|", pRoleInfoCache->m_activeness.m_strRewardLevelScore);
    }

    return true;
}


void ActivenessComponent::RegComponentNetMsgMap()
{
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetActivenessData, &ActivenessComponent::OnGetActiveness);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetActivenessReward, &ActivenessComponent::OnGetActivenessReward);
}

void ActivenessComponent::OnLogin()
{
    ResetActivinessData(time(NULL));
    SendActivenessInfo();
}

void ActivenessComponent::OnNotify(IParam &param)
{
    std::string strActiveModule;
    param.GetParam(std::string("activeness"), strActiveModule);
    int nTargetType = ActivenessTargetConfig::GetTypeFromName(strActiveModule);

    ActivenessTargetConfig  targetConfig;
    if (!ActivenessDataManager::Instance().GetTarget(nTargetType, targetConfig))
    {
        return;
    }

    // 根据当前时间充值活动数据
    ResetActivinessData(time(NULL));

    ActivenessTarget* pTarget = ActivenessTargetFactory::Instance().CreateTarget(targetConfig);
    if (pTarget == NULL)
    {
        return;
    }
    // 用于自动释放内存
    std::auto_ptr<ActivenessTarget> tempPtr(pTarget);

    // 设置数据
    pTarget->SetProgress(GetTargetProgress(nTargetType));
    if (nTargetType == ActivenessTargetConfig::EType_DanceMode)
    {
        ((ActivenessDanceModeTarget*)pTarget)->SetFinishedDanceMode(m_setFinishedDanceMode);
    }

    //目标是否完成
    if (pTarget->IsTargetComplete())
    {
        return;
    }

    // 更新目标进度
    if (pTarget->UpdateProgress(param))
    {
        // 更新component进度
        UpdateTarget(*pTarget);
    }

    // 更新活跃度
    if (pTarget->IsTargetComplete())
    {
        AddActiveness(pTarget->GetTargetScore(), nTargetType);

        if (CanGetReward())
        {
            // 重新通知前端
            SendActivenessInfo();
        }

        // 增加舞团试炼活力值
        CommonParam commonParam;
        commonParam.SetParam("dance_group_active", pTarget->GetTargetScore());
        NotifyAll(commonParam);
    }

    SaveActivinessDataToDb(); // 保存数据
}

void ActivenessComponent::UpdateTarget(const ActivenessTarget & target)
{
    if (target.GetTargetType() == ActivenessTargetConfig::EType_DanceMode)
    {
        UpdateDanceModeProgress(target);
    }

    m_mapTargetProgress[target.GetTargetType()] = target.GetProgress();

    SaveActivenessLog(CRoleActivenessLog::EActivenessAction_FinishTarget, target.GetTargetType(), target.GetProgress());
}

void ActivenessComponent::UpdateDanceModeProgress(const ActivenessTarget & target)
{
    const ActivenessDanceModeTarget & danceModeTarget = (ActivenessDanceModeTarget&)target;
    danceModeTarget.GetFinisheDanceMode(m_setFinishedDanceMode);
}

void ActivenessComponent::AddActiveness(int nAddScore, int nTargetType)
{
    m_nActivenessScore += nAddScore;
}

// 客户端请求进度
void ActivenessComponent::OnGetActiveness(GameMsg_Base &msg, CSlotPeer &slot)
{
    ResetActivinessData(time(NULL));
    SendActivenessInfo();
}

void ActivenessComponent::SendActivenessInfo()
{
    GameMsg_S2C_GetActivenessDataRes response;
    response.m_eSex = (ESexType)m_pRoleAttr->GetSex();
    response.m_nActiveness = m_nActivenessScore;

    std::list<ActivenessRewardConfig> listTempRewardConfig;
    ActivenessDataManager::Instance().GetTargetRewardConfigList(listTempRewardConfig);
    int nRewardIndex = 0;
    for (std::list<ActivenessRewardConfig>::iterator it = listTempRewardConfig.begin(); it != listTempRewardConfig.end(); ++it, ++nRewardIndex)
    {
        ActivenessReward reward(*it);
        reward.m_nRewardIndex = nRewardIndex;
        reward.m_bReceived = HasReceivedReward(reward.m_ActivenessRewardConfig.GetRequireScore());
        response.m_listRewardState.push_back(reward);
    }

    std::list<ActivenessTargetConfig> listTempTarget;
    ActivenessDataManager::Instance().GetTargetConfigList(listTempTarget);
    for (std::list<ActivenessTargetConfig>::iterator it = listTempTarget.begin();
        it != listTempTarget.end(); ++it)
    {
        ActivenessTarget targetProgress(*it);
        targetProgress.SetProgress(m_mapTargetProgress[it->m_nTargetType]);
        response.m_listTargetProgress.push_back(targetProgress);
    }

    m_pRoleNet->SendPlayerMsg(&response);
}

bool ActivenessComponent::HasReceivedReward(int nRequireScore)
{
    return (m_setRewardLevelScore.find(nRequireScore) != m_setRewardLevelScore.end());
}

int ActivenessComponent::GetTargetProgress(int nTargetType)
{
    int nProgress = 0;
    std::map<int, int>::const_iterator it = m_mapTargetProgress.find(nTargetType);
    if (it != m_mapTargetProgress.end())
    {
        nProgress = it->second;
    }

    return nProgress;
}

bool ActivenessComponent::CanGetReward()
{
    std::list<ActivenessRewardConfig> listRewardConfig;
    ActivenessDataManager::Instance().GetTargetRewardConfigList(listRewardConfig);
    std::list<ActivenessRewardConfig>::iterator it = listRewardConfig.begin();
    for (; it != listRewardConfig.end(); ++it)
    {
        if (m_nActivenessScore >= it->m_nRequireScore && !HasReceivedReward(it->m_nRequireScore))
        {
            return true;
        }
    }

    return false;
}

void ActivenessComponent::OnGetActivenessReward(GameMsg_Base &msg, CSlotPeer &slot)
{
    ResetActivinessData(time(NULL));

    std::string strError = "";

    int nRewardIndex = ((GameMsg_C2S_GetActivenessReward &)msg).m_nRewardIndex;

    ActivenessRewardConfig reward;

    if (!ActivenessDataManager::Instance().IsOpen())
    {
        strError = "GetActivenssReward_Closed";
    }
    else if (!ActivenessDataManager::Instance().GetReward(nRewardIndex, reward))
    {
        strError = "GetActivenssReward_InvalidParam";
    }
    else if (HasReceivedReward(reward.m_nRequireScore))
    {
        strError = "GetActivenssReward_Received";
    }
    else if (m_nActivenessScore < reward.m_nRequireScore)
    {
        strError = "GetActivenssReward_NotEnoughActiveness";
    }
    else
    {
        UpdateGetRewardRecord(reward.m_nRequireScore);

        SendReward(reward);
    }

    SaveActivinessDataToDb(); // 保存记录

    GameMsg_S2C_GetActivenessRewardRes response;
    response.m_nRewardIndex = nRewardIndex;
    response.m_bResult = strError.empty() ? true : false;
    response.m_strError = strError;
    m_pRoleNet->SendPlayerMsg(&response);

    if (!strError.empty())
    {
        LOG_ERROR << "get activeness error: roleid = " << m_pRoleAttr->GetRoleID()
            << " reward index = " << nRewardIndex
            << " have score = " << m_nActivenessScore
            << " error = " << strError << std::endl;
    }
}

void ActivenessComponent::ResetActivinessData(time_t nNow)
{
    if (IsSameDay(m_nUpdateTime, nNow))
        return; //

    m_nUpdateTime = (unsigned int)nNow;
    m_nActivenessScore = 0;
    m_setRewardLevelScore.clear();
    m_mapTargetProgress.clear();
    m_setFinishedDanceMode.clear();

    return;
}

void ActivenessComponent::SaveActivinessDataToDb()
{
    // 每次更新一整条数据到DB中
    RoleActivenessDB *pActivenessData = new RoleActivenessDB;

    // target_progress.
    ConcatToString(m_mapTargetProgress, std::string("|"), std::string(","), pActivenessData->m_strTargetProgress);
    ConcatToString(m_setFinishedDanceMode, std::string(","), pActivenessData->m_strDanceMode);

    // score.
    pActivenessData->m_nScore = m_nActivenessScore;
    pActivenessData->m_nUpdateTime = m_nUpdateTime;

    // reward statues.
    ConcatToString(m_setRewardLevelScore, "|", pActivenessData->m_strRewardLevelScore);

    AddQuery(QUERY_Activeness_UpdateOrInsert, m_pRoleAttr->GetRoleID(), pActivenessData);

    return;
}

void ActivenessComponent::UpdateGetRewardRecord(int nRewardScore)
{
    m_setRewardLevelScore.insert(nRewardScore);

    SaveActivenessLog(CRoleActivenessLog::EActivenessAction_Reward, 0, nRewardScore);
}

void ActivenessComponent::SendReward(ActivenessRewardConfig &reward)
{
    CReward *pFinalReward = NULL;
    if (m_pRoleAttr->GetSex() == (unsigned char)ESexType_Male)
    {
        pFinalReward = &reward.m_maleReward;
    }
    else
    {
        pFinalReward = &reward.m_femaleReward;
    }

    if (pFinalReward->m_nMoney > 0)
    {
        m_pRoleAttr->ChangeMoney(pFinalReward->m_nMoney, EChangeMoneyCause_Add_ActivenessReward, 0);
    }

    if (pFinalReward->m_nBindMCoin > 0)
    {
        m_pRoleAttr->ChangeBindBill(pFinalReward->m_nBindMCoin, EChangeBindBillCause_Add_ActivenessReward);
    }

    CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pFinalReward->m_listItems,
        EItemAction_Add_ActivenessReward, 0, true, EMailType_ActivenessReward,
        ActivenessDataManager::Instance().m_strMailSender, ActivenessDataManager::Instance().m_strMailTitle,
        ActivenessDataManager::Instance().m_strMailContent);
}

void ActivenessComponent::SaveActivenessLog(CRoleActivenessLog::EActivenessAction eAction, int nTargetType, int nScore)
{
    CRoleActivenessLog *pLog = new CRoleActivenessLog();
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_eAction = eAction;
    pLog->m_nTarget = nTargetType;
    pLog->m_nScore = nScore;
    AddQuery(QUERY_LogRoleActiveness, m_pRoleAttr->GetRoleID(), pLog);
}

