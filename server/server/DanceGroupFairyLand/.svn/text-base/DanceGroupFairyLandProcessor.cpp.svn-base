#include "../libServerFrame/QueryManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/Macro_Define.h"
#include "../group/GroupSystemSetting.h"
#include "DanceGroupFairyLandProcessor.h"
#include "../vip/VIPDataMgr.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../util/CommonFunc.h"
#include "DanceGroupFairyLandCfgMgr.h"
#include "GameMsg_FairyLand.h"
#include "../group/GroupServerCirculator.h"
#include "../chatGrp/WChat.h"
#include "../mail/WordMailManager.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../group/Global.h"
#include "../share/GameMsg_Forward.h"
#include "DanceGroupFairyLandTimeManager.h"

extern CQueryManager* g_pQueryMgr;

enum 
{
    eYesterdayRank = 1,
    eTodayRank,
    eBlessRank,
};


struct FairyLandReward
{
    unsigned int nRoleID;
    CBonus bonus;
};

CDanceGroupFairyLandProcessor::CDanceGroupFairyLandProcessor()
    : GameMsg_Processor(true, true)
    , m_nPrvCheckTime(0)
    , m_bIsFairyLandActive(false)
    , m_nNextSendPassedRewardTime(0)
    , m_nNextSendStorageBoxTime(0)
    , m_nNextAnnouncementTime(0)
    , m_bLoadComplete(false)

{
}

CDanceGroupFairyLandProcessor::~CDanceGroupFairyLandProcessor()
{

}

//////////////////////////////////////////////////////////////////////////
//Base Func
//////////////////////////////////////////////////////////////////////////
void CDanceGroupFairyLandProcessor::Initialize()
{
    if (!IsFairyLandOpening())
        return;

    RegisterMessage();

    SharedPtr<FairyLandLoadAllDB> sharedInfo(new FairyLandLoadAllDB);
    sharedInfo->m_pGInfo = &m_mapGroupInfo;
    sharedInfo->m_pGPInfo = &m_mapGroupIndexPcInfo;

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandLoadAllInfo, sharedInfo);

    return;
}

void CDanceGroupFairyLandProcessor::RegisterMessage()
{
    // 未开启，或者未加载
    if (!IsFairyLandOpening())
    {
        return;
    }

    RegQueryProcessor(QUERY_DanceGroupFairyLandClearAllPassedReward, &CDanceGroupFairyLandProcessor::OnClearAllPassedRewardCB);
    RegQueryProcessor(QUERY_DanceGroupFairyLandClearAllStorageBoxReward, &CDanceGroupFairyLandProcessor::OnClearAllStorageBoxRewardCB);
    RegQueryProcessor(QUERY_DanceGroupFairyLandLoadAllInfo, &CDanceGroupFairyLandProcessor::OnLoadAllInfo);
    return;
}

void CDanceGroupFairyLandProcessor::RegisterMessageDealy()
{
    // 未开启，或者未加载
    if (!IsFairyLandOpening())
    {
        return;
    }

    RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandCheckIsInYesterdatRank, &CDanceGroupFairyLandProcessor::OnCheckIsHaveYesterdayReward);
    RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandStorageboxInfo, &CDanceGroupFairyLandProcessor::OnFairyLandStorageBoxInfo);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairyLandRankInfo, &CDanceGroupFairyLandProcessor::OnFairyLandRankInfo);
    RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandGetYesterdayRankReward, &CDanceGroupFairyLandProcessor::OnFairyLandSendYesterDayReward);
    RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandOpenBless, &CDanceGroupFairyLandProcessor::OnFairyLandOpenBless);
    RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandBless, &CDanceGroupFairyLandProcessor::OnFairyLandBless);
}

void CDanceGroupFairyLandProcessor::OnUpdate(time_t tNow)
{
    if (!IsFairyLandOpening())
        return; // 未开启的情况下不检测

    if (!m_bLoadComplete)
        return;

    FairyLandTimeManager::Instance().OnUpdate(tNow);

    // 清理过期数据
    NewDayProcess(tNow);
    // 检测活动开启状态
    ActiveTimeProcess(tNow);
    // 公告
    if (tNow >= m_nNextAnnouncementTime)
    {
        AnnouncementTimeProcess(tNow);
        m_nNextAnnouncementTime = NextAnnouncementTime(tNow); // 
    }
    // 发放通关奖励
    if (tNow >= m_nNextSendPassedRewardTime)
    {
        PassedRewardTimeProcess(tNow);
        m_nNextSendPassedRewardTime = NextPassRewardTime(tNow); 
    }
    // 发放储物箱奖励
    if (tNow >= m_nNextSendStorageBoxTime)
    {
        StorageBoxRewardTimeProcess(tNow);
        m_nNextSendStorageBoxTime = NextStorageBoxRewardTime(tNow);
    }
    m_nPrvCheckTime = tNow;
}

/// <summary>
/// 时间处理
/// <summary>
void CDanceGroupFairyLandProcessor::NewDayProcess(time_t tNow)
{ // 先清理内存，然后清理数据库
    if ( !IsSameDay(m_nPrvCheckTime, tNow) )
    {
        NewDayClear();
    }
}

void CDanceGroupFairyLandProcessor::ActiveTimeProcess(time_t tNow)
{
    if (IsActive(tNow) == m_bIsFairyLandActive)
        return;

    m_bIsFairyLandActive = IsActive(tNow);
    // 记录开启时间

    GameMsg_S2C_FairyLandStateNotify notifyMsg;
    if (m_bIsFairyLandActive)
    { // 开启了，
        notifyMsg.m_nState = GameMsg_S2C_FairyLandStateNotify::EOpen;
        notifyMsg.m_nCurrentChapter = GetDefaultChapter(); // 新开始的时候，所有帮会都是默认的第一关
        notifyMsg.m_nTotalScore = CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(GetDefaultChapter());
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateLastStartTime, (int)time(NULL));
    }
    else
    { // 关闭了
        notifyMsg.m_nState = GameMsg_S2C_FairyLandStateNotify::EClose;
    }

    CGroupPlayerManager::Instance().SendMsgToAll(notifyMsg);
    return;
}

void CDanceGroupFairyLandProcessor::PassedRewardTimeProcess(time_t tNow)
{
    SendAllPassedReward(); // 直接发了
}

void CDanceGroupFairyLandProcessor::AnnouncementTimeProcess(time_t tNow)
{
    CAnnounceBase ann;
    ann.m_bShowInChatWindow = true;
    ann.m_bIsAdminChat = false;
    ann.m_strContent = CDanceGroupFairyLandCfgMgr::Instance().GetEndAnnounceMentContext();
    CWChatMgr::Instance().SendAllAnnouncement(ann);
}

void CDanceGroupFairyLandProcessor::StorageBoxRewardTimeProcess(time_t tNow)
{
    SendAllStorageBoxReward();
}

time_t CDanceGroupFairyLandProcessor::NextPassRewardTime(time_t tNow)
{
    int nSec = CDanceGroupFairyLandCfgMgr::Instance().GetNeedRewardTimeSecond();
    int zero = GetTodayStartTime(tNow);

    return tNow - zero >= nSec ? (zero + DAY_PRE_SECONDS + nSec) : (zero + nSec);
}

time_t CDanceGroupFairyLandProcessor::NextStorageBoxRewardTime(time_t tNow)
{
    int nTargetSec = CDanceGroupFairyLandCfgMgr::Instance().GetNeedStorageBoxRewardTimeSecond();
    int nTargetWeek = CDanceGroupFairyLandCfgMgr::Instance().GetStorageBoxRewardCycleWeek();

    int nowWeek = GetWeekDay(tNow);
    int zero = GetTodayStartTime(tNow);

    if (nTargetWeek * WEEK_PRE_SECONDS + nTargetSec > nowWeek * WEEK_PRE_SECONDS + (tNow - zero))
    { // 目标周期还在后边：
        return zero + (nTargetWeek - nowWeek)*DAY_PRE_SECONDS + nTargetSec;
    }
    else
    { // 目标周期已过，或者正好是当前时间
        return zero + (nTargetWeek + 7 - nowWeek)*DAY_PRE_SECONDS + nTargetSec;
    }
}

time_t CDanceGroupFairyLandProcessor::NextAnnouncementTime(time_t tNow)
{
    int nTargetSec = CDanceGroupFairyLandCfgMgr::Instance().GetNeedAnnounMentTimeSecond();
    int zero = GetTodayStartTime(tNow);

    return tNow - zero >= nTargetSec ? (zero + DAY_PRE_SECONDS + nTargetSec) : (zero + nTargetSec);
}


void CDanceGroupFairyLandProcessor::NewDayClear()
{
    for (GroupInfoMapIt it = m_mapGroupInfo.begin(); it != m_mapGroupInfo.end(); ++it)
    {
        std::list<FairyLandRankCell> rank;
        it->second.GetScoreTodayRank(rank);
        it->second.SetYestedayRank(rank);
        it->second.ClearTodayRank();

        it->second.SetCurChapterLvl(GetDefaultChapter());
        it->second.SetTotalScore(0);
        it->second.ClearBlessRank();
    }

    for (GroupPersonalMapIt GPIt = m_mapGroupIndexPcInfo.begin(); GPIt != m_mapGroupIndexPcInfo.end(); ++GPIt)
    {
        for (PersonalInfoMapIt PIt = GPIt->second.begin(); PIt != GPIt->second.end(); ++PIt)
        {
            PIt->second.TotalScoreOnNewDay();
            PIt->second.ClearBlassTimes();
            PIt->second.ClearLastChallenge();
//            PIt->second.m_setPassRewardFalg.clear();
//             PIt->second.m_setJoinRewardFalg.clear();
        }
    }

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyNewDay, GetDefaultChapter());
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateSystemStamp, time(NULL), NULL, SYSTEM_TABLE_ID_FAIRYLAND_DAY_CLEAN); // 设置为当前时间
}


/// <summary>
/// Func
/// <summary>
void CDanceGroupFairyLandProcessor::SendJoinReward(int nGroupId, int nRoleid, int nChapter)
{
    GameMsg_G2S_FairyLandJoinAndRandomReward msg;
    msg.m_nChapter = nChapter;
    msg.m_nIsSendJoin = true;
    CGroupRoleEntity::SendPlayerMsgByRoleID(msg, nRoleid);

}

void CDanceGroupFairyLandProcessor::SendAllPassedReward()
{
    // 清理内存，并且把数据打包出去
    SharedPtr<std::list<FairyLandReward> > sharedLstReward(new std::list<FairyLandReward>);

    for (GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.begin(); GPit != m_mapGroupIndexPcInfo.end(); ++GPit)
    {
        GroupInfoMapIt Git = m_mapGroupInfo.find(GPit->first);
        if (m_mapGroupInfo.end() == Git)
            continue; // 下一个舞团

        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            FairyLandReward reward;
            reward.nRoleID = Pit->first;

            //发奖励
            for (std::list<FairyLandRewardItem>::iterator RIt = Pit->second.m_PassedChapterReward.m_listItems.begin(); 
                RIt != Pit->second.m_PassedChapterReward.m_listItems.end(); ++RIt)
            {
                reward.bonus.m_listItems.push_back(CItem(RIt->m_nItemId, (unsigned short)RIt->m_nCount, RIt->m_nDuration, false));
            }

            reward.bonus.m_nMoney = Pit->second.m_PassedChapterReward.m_nMoney;
            reward.bonus.m_nBindMCoin = Pit->second.m_PassedChapterReward.m_nBindMB;

            //清除标志位
//             Pit->second.m_setPassRewardFalg.clear();
            Pit->second.m_setJoinRewardFalg.clear();
            Pit->second.ClearPassedChapterReward(); // 清除掉这些数据

            if (!reward.bonus.m_listItems.empty() || reward.bonus.m_nMoney != 0 || reward.bonus.m_nBindMCoin != 0)
            {
                sharedLstReward->push_back(reward);
            }
        }
    }

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandClearAllPassedReward, sharedLstReward);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateSystemStamp, time(NULL), NULL, SYSTEM_TABLE_ID_FAIRYLAND_PASSREWARD);

}

void CDanceGroupFairyLandProcessor::SendAllStorageBoxReward()
{
    SharedPtr<std::list<FairyLandReward> > shareLstReward(new std::list<FairyLandReward>);
    for (GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.begin(); GPit != m_mapGroupIndexPcInfo.end(); ++GPit)
    {
        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            FairyLandReward reward;
            reward.nRoleID = Pit->first;
            //发奖励
            std::list<FairyLandRewardItem> listReward;
            Pit->second.GetStorageBoxItems(listReward);
            for (std::list<FairyLandRewardItem>::iterator RIt = listReward.begin();
                RIt != listReward.end(); ++RIt)
            {
                reward.bonus.m_listItems.push_back(CItem(RIt->m_nItemId, RIt->m_nCount, RIt->m_nDuration, false));
            }

            if (!reward.bonus.m_listItems.empty())
            {
                shareLstReward->push_back(reward);
            }
            //清除数据
            Pit->second.ClearStorageBoxItems();
        }
    }

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandClearAllStorageBoxReward, shareLstReward);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateSystemStamp, time(NULL), NULL, SYSTEM_TABLE_ID_FAIRYLAND_STORGEREWARD);
}

void CDanceGroupFairyLandProcessor::AddStroageBox(int GID, int Rid, int chapter)
{
    DanceGroupFairyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(GID, Rid);
    if (NULL == pPersonalInfo)
        return;

    FairyLandChapterReward reward;
    reward.m_nBindMB = CDanceGroupFairyLandCfgMgr::Instance().GetPassRewardBindMB(chapter);
    reward.m_nMoney = CDanceGroupFairyLandCfgMgr::Instance().GetPassRewardMoney(chapter);
    std::list<CRewardUnit> listReward;
    CDanceGroupFairyLandCfgMgr::Instance().GetStroageBoxReward(chapter, listReward);

    for (std::list<CRewardUnit>::iterator it = listReward.begin(); it != listReward.end(); ++it)
    {
        FairyLandRewardItem i;
        i.m_nItemId = it->m_nItemId;
        i.m_nDuration = it->m_nTime;
        i.m_nCount = it->m_nCount;
        pPersonalInfo->AddStorageBoxItem(i);
    }
    std::string * pStorageBox = new std::string(pPersonalInfo->StorageBoxToJson());
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateStorageBox, Rid, pStorageBox);
}

void CDanceGroupFairyLandProcessor::OnRoleLogin(CGroupRoleEntity &groupRole)
{
    if (!IsFairyLandOpening() || !m_bLoadComplete)
        return;

    if (groupRole.GetDanceGroupID() == 0)
        return; // 没有舞团

    CheckRoleGroup(groupRole.GetRoleID(), groupRole.GetDanceGroupID());

    SendFairyLandMsg(groupRole);
    return;
}

void CDanceGroupFairyLandProcessor::OnRoleQuitGroup(unsigned int nDanceGroup, unsigned int nRoleID)
{
    DelFairyLandPersonal(nDanceGroup, nRoleID);


}

void CDanceGroupFairyLandProcessor::OnDanceGroupDisband(unsigned int nDanceGroup)
{
    DelFairyLandGroup(nDanceGroup);
}

void CDanceGroupFairyLandProcessor::SendFairyLandMsg(CGroupRoleEntity &groupRole)
{
    //     发送开关 /
    {
        GameMsg_S2C_FairyLandStateNotify msg;
        msg.m_nState = m_bIsFairyLandActive ? GameMsg_S2C_FairyLandStateNotify::EOpen : GameMsg_S2C_FairyLandStateNotify::EClose;
        if (m_bIsFairyLandActive)
        {
            GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
            msg.m_nCurrentChapter = GetDefaultChapter();
            if (m_mapGroupInfo.end() != iter && iter->second.GetCurChapterLvl() != 0)
                msg.m_nCurrentChapter = (unsigned char)iter->second.GetCurChapterLvl();
            msg.m_nTotalScore = CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(msg.m_nCurrentChapter);
        }
        groupRole.SendPlayerMsg(&msg);
    }

    //         发送挑战次数
    if (m_bIsFairyLandActive)
    {
        DanceGroupFairyLandPersonalInfo *pPersionalInfo = GetPersonalInfo(groupRole.GetDanceGroupID(), groupRole.GetRoleID());
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairyLandChanllegeTimesNotify msg;
        msg.m_nChanllegeTimes = 0;
        if (iter != m_mapGroupInfo.end() && NULL != pPersionalInfo)
            msg.m_nChanllegeTimes = (unsigned short)pPersionalInfo->GetCurChapterChallengeTimes(iter->second.GetCurChapterLvl());

        groupRole.SendPlayerMsg(&msg);
    }

    //         发送祝福
    {
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairyLandBlessNotify msg;
        msg.m_nBelss = 0;
        if (iter != m_mapGroupInfo.end() && time(NULL) < iter->second.GetBlessBuffEndTimeStamp())
            msg.m_nBelss = (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetBlessBuff();

        groupRole.SendPlayerMsg(&msg);
    }

    //         发送当前层进度
    if (m_bIsFairyLandActive)
    {
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairyLandChapterScoreNotify msg;
        msg.m_nScore = 0;
        if (m_mapGroupInfo.end() != iter)
            msg.m_nScore = iter->second.GetTotalScore(); // 当前进度

        groupRole.SendPlayerMsg(&msg);
    }

    return;
}


void CDanceGroupFairyLandProcessor::CheckBlessStamp(time_t now, unsigned int nDanceGroupID)
{
    GroupInfoMapIt iter = m_mapGroupInfo.find(nDanceGroupID);
    if (iter == m_mapGroupInfo.end())
        return;

    if (iter->second.GetBlessBuffEndTimeStamp() <= now)
    { // 广播本舞团祝福停止的消息
        GameMsg_S2C_FairyLandBlessNotify msg;
        msg.m_nBelss = 0; // 祝福已经停了。
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nDanceGroupID, 0, msg);
    }
}


bool CDanceGroupFairyLandProcessor::CheckAllowReadyInFairyLandRoom(int nGroupId, int nRoleid)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleid);
    if (NULL == pGroupRole)
        return false; // 

    DanceGroupFairyLandGroupInfo *pFGroupInfo = GetFairyLandGroupInfo(nGroupId);
    unsigned char nChapter = GetDefaultChapter();
    if (NULL != pFGroupInfo && pFGroupInfo->GetCurChapterLvl() != 0)
        nChapter = (unsigned char)pFGroupInfo->GetCurChapterLvl();

    DanceGroupFairyLandPersonalInfo *personalInfo = GetPersonalInfo(nGroupId, nRoleid);
    if (NULL == personalInfo)
        return true; // 

    return personalInfo->GetCurChapterChallengeTimes(nChapter) < CVIPDataMgr::Instance().GetDanceGroupFairyLandExChance(pGroupRole->IsVIP() ? pGroupRole->VIPLevel() : 0);
}

bool CDanceGroupFairyLandProcessor::CheckAllowStart()
{
    return m_bIsFairyLandActive;
}

bool CDanceGroupFairyLandProcessor::CheckIsFinishedAllChapter(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() == GIt)
    {
        return false;
    }

    return (GIt->second.GetCurChapterLvl() > CDanceGroupFairyLandCfgMgr::Instance().GetMaxChapterLevel());
}

void CDanceGroupFairyLandProcessor::GroupToNextChapterlvl(int nGroupid)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupid);
    if (m_mapGroupInfo.end() == GIt)
    {
        return;
    }

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupid);
    if (GPit == m_mapGroupIndexPcInfo.end())
    {
        return;
    }

    int nCurChapter = GIt->second.GetCurChapterLvl();
    for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
    { // 每打完一关，记录舞团内玩家的奖励，需要发奖励的时候，把奖励发送下去
        if (!Pit->second.CheckIsHaveJoinReward(nCurChapter))
            continue; // 

        FairyLandChapterReward tempReward;
        GetChapterPassReward(tempReward, nCurChapter, Pit->second.m_nSex);
        Pit->second.AddPassedChapterReward(tempReward);

        // 更新这个人的奖励数据
        SharedPtr<std::string> queryParam(new std::string(Pit->second.PassedRewardToJson()));
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdatePassedRewardRecord, queryParam, Pit->second.GetRoleId());
    }

    GIt->second.SetCurChapterLvl(nCurChapter + 1);
    GIt->second.SetTotalScore(0);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateCurChatper, nGroupid, NULL, nCurChapter + 1);

    return;
}


void CDanceGroupFairyLandProcessor::RecordChapterJoinFlag(int nChapter, int nGroupId, int nRoleId)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "role[%u] entity should not null");

    DanceGroupFairyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(nGroupId, nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pPersonalInfo, return, "don't get group[%u] personal[%u] info ", nGroupId, nRoleId);

    pPersonalInfo->m_nSex = pGroupRole->GetSex();
    pPersonalInfo->SetJoinRewardFlag(nChapter);
    std::string *pStrJoinFlag = new std::string;
    pPersonalInfo->RewardFlagToString(pPersonalInfo->m_nSex, pPersonalInfo->m_setJoinRewardFalg, *pStrJoinFlag);

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateJoinRewardFlag, nRoleId, pStrJoinFlag); // 表示已经参与过这个项目了
}

// 
// void CDanceGroupFairyLandProcessor::RecordPassedReward(int nChapter, DanceGroupFairyLandPersonalInfo &personalInfo)
// {
//     personalInfo.SetPassRewardFlag(nChapter);
//     std::string *pStrPassFlag = new std::string;
//     personalInfo.RewardFlagToString(personalInfo.m_nSex, personalInfo.m_setPassRewardFalg, *pStrPassFlag);
// 
//     g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdatePassedRewardFlag, personalInfo.GetRoleId(), pStrPassFlag);
// }

void CDanceGroupFairyLandProcessor::DanceOverScoreProcess(int nGroup, int nChapter, std::list<SDancerScore>& listScore)
{
    GroupInfoMapIt GIt = EnsureGetFairyLandGroup(nGroup);

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroup);
    if (GPit == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairyLandPersonalInfo> tmp;
        m_mapGroupIndexPcInfo.insert(std::make_pair(nGroup, tmp));
        GPit = m_mapGroupIndexPcInfo.find(nGroup);
    }

    int nBlessBuf = 0; // 祝福buff
    if (GIt->second.GetBlessBuffEndTimeStamp() > (unsigned int)time(NULL))
    {
        nBlessBuf = CDanceGroupFairyLandCfgMgr::Instance().GetBlessBuff();
    }

    int nTotalVipBuf = 0;
    for (std::list<SDancerScore>::iterator DSIt = listScore.begin();
        DSIt != listScore.end(); ++DSIt)
    {
        CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(DSIt->m_nRoleId);
        if (NULL == pGroupRole)
            continue;

        if (!pGroupRole->IsVIP())
            continue;

        DSIt->m_nVipBuf = CVIPDataMgr::Instance().GetDanceGroupFairyLandVipBuf(pGroupRole->VIPLevel());
        nTotalVipBuf += DSIt->m_nVipBuf;
    }

    int nTotalScore = 0;
    // 计算总分，和统计排行榜
    for (std::list<SDancerScore>::iterator DSIt = listScore.begin();
        DSIt != listScore.end(); ++DSIt)
    {
        CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(DSIt->m_nRoleId);
        if (NULL == pGroupRole)
            continue;

        PersonalInfoMapIt PIt = GPit->second.find(DSIt->m_nRoleId);
        if (PIt == GPit->second.end())
        {
            DanceGroupFairyLandPersonalInfo info;
            info.SetGroupId(nGroup);
            info.SetRoleId(DSIt->m_nRoleId);
            AddFairyLandPersonal(nGroup, DSIt->m_nRoleId, info);
            PIt = GPit->second.find(DSIt->m_nRoleId);
        }

        //结算加成的分数
        int nScore = DSIt->m_nScore + (DSIt->m_nScore*(nTotalVipBuf + nBlessBuf)/100);
        nTotalScore += nScore;

        // 记录玩家总分（未加成）
        int nRoleid = PIt->first;
        PIt->second.AddTotalScore(DSIt->m_nScore);
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateTotalScore, DSIt->m_nRoleId, NULL, PIt->second.GetTotalScore());

        // 记录玩家挑战次数
        {
            int challengetimes = PIt->second.GetCurChapterChallengeTimes(nChapter) + 1;
            PIt->second.SetCurChapterChallengeTimes(challengetimes);
            PIt->second.SetLastChallengeChapterLvl(nChapter); // 最后一次挑战的是这个关卡
            SharedPtr<std::pair<int, int> > queryParam(new std::pair<int, int>);
            queryParam->first = nChapter;
            queryParam->second = challengetimes;
            g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateCurChapterChallengeTimes, queryParam, DSIt->m_nRoleId);
        }

        // 推送挑战次数：客户端需要的是当前关卡的次数，不是当前房间的关卡次数
        {
            GameMsg_S2C_FairyLandChanllegeTimesNotify msg;
            msg.m_nChanllegeTimes = (unsigned short)PIt->second.GetCurChapterChallengeTimes(GIt->second.GetCurChapterLvl()); // 这个应该用group的通知：
            pGroupRole->SendPlayerMsg(&msg);
        }

        // 更新排名
        if (GIt->second.TryAddTodayRank(nRoleid, PIt->second.GetTotalScore(), pGroupRole->GetName()))
        {
            std::string *pStrRank = new std::string();
            *pStrRank = GIt->second.ScoreTodayRankToJson();
            g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateTodayRank, nGroup, pStrRank);
        }

        // 发放参与奖励
        SendJoinReward(nGroup, nRoleid, nChapter);

        // 记录通关奖励（秘境结束之后，统一发放）
        if (nChapter <= GIt->second.GetCurChapterLvl() &&
            !(PIt->second.CheckIsHaveJoinReward(nChapter) ) )
        {
            RecordChapterJoinFlag(nChapter, nGroup, nRoleid);
        }

        // 舞团奖励
        if (DSIt->m_nScore != 0)
        {
            AddStroageBox(nGroup ,nRoleid, nChapter);
        }
    }

    TryAddFairyLandGroupScore(nGroup, nChapter, nTotalScore);

    //广播舞蹈结束 发送消息
    GameMsg_S2C_FairyLandDanceEndInfo msg;
    msg.m_nChapterTotalScore = CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(nChapter);
    msg.m_nChapterScore = (nChapter < GIt->second.GetCurChapterLvl() ? msg.m_nChapterTotalScore : GIt->second.GetTotalScore());
    msg.m_nBlessBuf = nBlessBuf;
    msg.m_listScore = listScore;

    for (std::list<SDancerScore>::iterator DSIt = listScore.begin();
        DSIt != listScore.end(); ++DSIt)
    {
        CGroupRoleEntity::SendPlayerMsgByRoleID(msg, DSIt->m_nRoleId);
//        Forward(msg, DSIt->m_nRoleId);
    }
}

void CDanceGroupFairyLandProcessor::TryAddFairyLandGroupScore(int nGroup, int nChapter, int nAddScore)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroup);
    if (m_mapGroupInfo.end() == GIt)
    {
        return;
    }

    if (GIt->second.GetCurChapterLvl() != nChapter)
    {
        return;
    }


    GIt->second.SetTotalScore(GIt->second.GetTotalScore() + nAddScore);

    if (GIt->second.GetTotalScore() > CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(nChapter) )
    {
        GroupToNextChapterlvl(nGroup);

        GameMsg_S2C_FairyLandCurrentChapterNotify msg;
        msg.m_nCurrentChapter = (unsigned char)GIt->second.GetCurChapterLvl();
        msg.m_nTotalScore = CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(msg.m_nCurrentChapter);
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nGroup, 0, msg);
    }
    else // 如果进入下一关，则可以省却发送进度变更消息
    {
        // 广播进度变更消息：
        GameMsg_S2C_FairyLandChapterScoreNotify msg;
        msg.m_nScore = GIt->second.GetTotalScore();
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nGroup, 0, msg);
    }


    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateGroupScore, nGroup, NULL, GIt->second.GetTotalScore());
}

int CDanceGroupFairyLandProcessor::GetCurChapterLvl(int nGroupId)
{
    DanceGroupFairyLandGroupInfo *pFGroupInfo = GetFairyLandGroupInfo(nGroupId);
    if (NULL == pFGroupInfo || pFGroupInfo->GetCurChapterLvl() == 0)
        return GetDefaultChapter();

    return pFGroupInfo->GetCurChapterLvl(); 
}

int CDanceGroupFairyLandProcessor::GetCurMusic(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairyLandCfgMgr::Instance().GetChapterMusicId(GIt->second.GetCurChapterLvl());
    }
    return 0;
}

int CDanceGroupFairyLandProcessor::GetCurScene(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairyLandCfgMgr::Instance().GetChapterSceneId(GIt->second.GetCurChapterLvl());
    }

    return 0;
}

int CDanceGroupFairyLandProcessor::GetDifficultyLvl(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairyLandCfgMgr::Instance().GetChapterDifficultyLevel(GIt->second.GetCurChapterLvl());
    }

    return 0;
}

CDanceGroupFairyLandProcessor::GroupInfoMapIt CDanceGroupFairyLandProcessor::EnsureGetFairyLandGroup(unsigned int nGroupID)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupID);
    if (m_mapGroupInfo.end() == GIt)
    {
        DanceGroupFairyLandGroupInfo info;
        info.SetGroupId(nGroupID);
        info.SetCurChapterLvl(GetDefaultChapter());
        AddFairyLandGroup(info);
        GIt = m_mapGroupInfo.find(nGroupID);
    }
    return GIt;
}


void CDanceGroupFairyLandProcessor::AddFairyLandGroup(DanceGroupFairyLandGroupInfo& Ginfo)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(Ginfo.GetGroupId());
    if (GIt == m_mapGroupInfo.end())
    {
        m_mapGroupInfo.insert(std::make_pair(Ginfo.GetGroupId(), Ginfo));

        DanceGroupFairyLandGroupInfo *pDbInfo = new DanceGroupFairyLandGroupInfo(Ginfo);
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandAddGroupInfo, Ginfo.GetGroupId(), pDbInfo);
    }
}

void CDanceGroupFairyLandProcessor::DelFairyLandGroup(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (GIt != m_mapGroupInfo.end())
    {
        GroupPersonalMapIt PIt = m_mapGroupIndexPcInfo.find(nGroupId);
        if (PIt != m_mapGroupIndexPcInfo.end())
        {
            PIt->second.erase(nGroupId);
        }
        m_mapGroupInfo.erase(nGroupId);
        //这个Query删除Group表 和Perosonal表GroupId == nGroupId 的数据
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandDelGroupInfo, nGroupId);
    }
}

void CDanceGroupFairyLandProcessor::AddFairyLandPersonal(int nGroupId, int nRoleId, DanceGroupFairyLandPersonalInfo &Pinfo)
{
    GroupPersonalMapIt PIt = m_mapGroupIndexPcInfo.find(nGroupId);
    if (PIt == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairyLandPersonalInfo> info;
        info.insert(std::make_pair(nRoleId, Pinfo));
        m_mapGroupIndexPcInfo.insert(std::make_pair(nGroupId, info));
        UpdateRoleGroupInfo(nRoleId, nGroupId);
    }
    else
    {
        PIt->second.insert(std::make_pair(nRoleId, Pinfo));
        UpdateRoleGroupInfo(nRoleId, nGroupId);
    }
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandAddPersonalInfo, nGroupId, NULL, nRoleId);
}

void CDanceGroupFairyLandProcessor::DelFairyLandPersonal(int nGroupId, int nRoleId)
{
    GroupPersonalMapIt PIt = m_mapGroupIndexPcInfo.find(nGroupId);
    if (PIt != m_mapGroupIndexPcInfo.end())
    {
        PIt->second.erase(nRoleId);
        RemoveRoleGroupInfo(nRoleId);
    }

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandDelPersonalInfo, nGroupId, NULL, nRoleId);
}

void CDanceGroupFairyLandProcessor::GetRankList(int nGroupId, int nType, std::list<FairyLandRankCell> &rRank)
{
    rRank.clear();

    GroupInfoMapIt it = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != it)
    {
        switch (nType)
        {
        case eYesterdayRank:
        {
            it->second.GetScoreYesterdayRank(rRank);
        }
            break;
        case eTodayRank:
        {
            it->second.GetScoreTodayRank(rRank);
        }
            break;
        case eBlessRank:
        {
            it->second.GetBlessRank(rRank);
        }
            break;
        default:
            break;
        }
    }
}
// void CDanceGroupFairyLandProcessor::TryAddTodayRank(int nGroupId, int nRoleId, int nSorce)
// {
//     GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
//     if (m_mapGroupInfo.end() == GIt)
//     {
//         DanceGroupFairyLandGroupInfo info;
//         info.SetGroupId(nGroupId);
//         AddFairyLandGroup(info);
//     }
// 
//     GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
//     if (m_mapGroupIndexPcInfo.end() == GPit)
//     {
//         std::map<int, DanceGroupFairyLandPersonalInfo> tmp;
//         m_mapGroupIndexPcInfo.insert(std::make_pair(nGroupId, tmp));
//     }
// 
//     PersonalInfoMapIt Pit = GPit->second.find(nRoleId);
//     if (GPit->second.end() == Pit)
//     {
//         DanceGroupFairyLandPersonalInfo info;
//         info.SetGroupId(nGroupId);
//         info.SetRoleId(nRoleId);
//         AddFairyLandPersonal(nGroupId, nRoleId, info);
//     }
// 
//     if (GIt->second.TryAddTodayRank(nRoleId, Pit->second.GetTotalScore()))
//     {
//         std::list<FairyLandRankCell> *plist = new std::list<FairyLandRankCell>();
//         GIt->second.GetScoreTodayRank(*plist);
//         g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateTodayRank, nGroupId, plist);
//     }
// }

/// <summary>
/// MsgCB
/// <summary>
void CDanceGroupFairyLandProcessor::OnCheckIsHaveYesterdayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    
//    GameMsg_C2S_FairyLandCheckIsInYesterdatRank &rMsg = (GameMsg_C2S_FairyLandCheckIsInYesterdatRank&)rMsgBase;
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsgBase.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get account[%u] group entity.", rMsgBase.nAccount);

    int nYesterdayScore = 0;
    DanceGroupFairyLandPersonalInfo *info = GetPersonalInfo(pGroupRole->GetDanceGroupID(), pGroupRole->GetRoleID());
    if (NULL != info)
        nYesterdayScore = info->GetYesterdayTotalScore();

    GameMsg_S2C_FairyLandCheckIsInYesterdatRank msg;
    msg.m_nYesterdayScore = nYesterdayScore;
    pGroupRole->SendPlayerMsg(&msg);
//     Forward(msg, rMsg.m_nRoleId);
}

// void CDanceGroupFairyLandProcessor::OnFairyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
// {
// 
//     GameMsg_S2G_FairyLand_GroupInfo &rMsg = (GameMsg_S2G_FairyLand_GroupInfo&)rMsgBase;
//     int nGroupId = rMsg.m_nGroupId;
//     int nRoleId = rMsg.m_nRoleId;
// 
//     if (!m_bIsFairyLandActive)
//     {
// //         GameMsg_S2C_FairyLandError errormsg;
// //         errormsg.m_nFlag = eFairylandClosed;
// //         CGroupRoleEntity::SendPlayerMsgByRoleID(errormsg, nRoleId);
//         return;
//     }
// 
//     GameMsg_S2C_FairyLand_GroupInfo retmsg;
//     GroupInfoMapIt it = m_mapGroupInfo.find(nGroupId);
//     if (m_mapGroupInfo.end() != it)
//     {
//         if (it->second.GetBlessBuffEndTimeStamp() > (unsigned int ) time(NULL))
//         {
//             retmsg.m_nTotalScore = CDanceGroupFairyLandCfgMgr::Instance().GetChapterPassScore(it->second.GetCurChapterLvl());
//         }
//         
//         retmsg.m_nBlessBuff = CDanceGroupFairyLandCfgMgr::Instance().GetBlessBuff();
//         retmsg.m_nChapter = it->second.GetCurChapterLvl();
//         retmsg.m_nScore = it->second.GetTotalScore();
//     }
//     else
//     {
//         //初始化为第一关
//         retmsg.m_nChapter = GetDefaultChapter();
//     }
// 
//     GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
//     if (m_mapGroupIndexPcInfo.end() != GPit)
//     {
//         PersonalInfoMapIt Pit = GPit->second.find(nRoleId);
//         if (GPit->second.end() != Pit)
//         {
//             retmsg.m_nChanllegeTimes = Pit->second.GetCurChapterChallengeTimes();
//         }
//     }
// 
// //    Forward(retmsg, nRoleId);
//     CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, nRoleId);
// }

void CDanceGroupFairyLandProcessor::OnFairyLandStorageBoxInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
//     GameMsg_C2S_FairyLandStorageboxInfo msg = (GameMsg_C2S_FairyLandStorageboxInfo&)rMsgBase;
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsgBase.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get account[%u] group entity.", rMsgBase.nAccount);
    int nGroupId = pGroupRole->GetDanceGroupID();
    int nRoleId = pGroupRole->GetRoleID();

    GameMsg_S2C_FairyLandStorageboxInfo retmsg;
    int nTotalCount = 0;
    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
    if (m_mapGroupIndexPcInfo.end() != GPit)
    {
        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            std::list<FairyLandRewardItem>  listItems;
            Pit->second.GetStorageBoxItems(listItems);
            for (std::list<FairyLandRewardItem>::iterator it = listItems.begin(); it != listItems.end(); ++it)
            {
                nTotalCount += it->m_nCount;
            }
        }
    }
    retmsg.m_nTotalStorageBoxItemsCount = nTotalCount;
//    Forward(retmsg, nRoleId);
    CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, nRoleId);
}

void CDanceGroupFairyLandProcessor::OnFairyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairyLandRankInfo &msg = (GameMsg_S2G_FairyLandRankInfo&)rMsgBase;

    const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(msg.m_nGroupId);
    ENSURE_WITH_LOG_CMD(NULL != pDGInfo, return, "error to get account[%u] group[%d] info", msg.nAccount, msg.m_nGroupId);

    GameMsg_S2C_FairyLandRankInfo retmsg;
    std::list<FairyLandRankCell> list;
    std::list<SRankCell> msglist;
    retmsg.m_nType = msg.m_nType;

    GetRankList(msg.m_nGroupId, msg.m_nType, list);
    const int nMaxSize = 5; // 控制每次发送的个数

    for (std::list<FairyLandRankCell>::iterator it = list.begin();
        it != list.end();)
    {
        msglist.clear();

        for (int tmp = 0; tmp <= nMaxSize; ++tmp)
        {
            SRankCell cell;
            cell.m_nRoleId = it->m_nRoleId;
            cell.m_nValue = it->m_nValue;
            cell.m_strName = it->m_strName;
            msglist.push_back(cell);

            ++it;

            if (it == list.end())
            {
                retmsg.m_bIsComplete = true;
                break;
            }
        }

        retmsg.m_listRank = msglist;
//         Forward(retmsg, msg.m_nRoleId);
        CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, msg.m_nRoleId);
    }

    if (!retmsg.m_bIsComplete)
    { // 为空的时候：
        retmsg.m_bIsComplete = true;
        CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, msg.m_nRoleId);
    }

    return;
}

void CDanceGroupFairyLandProcessor::OnFairyLandSendYesterDayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_FairyLandGetYesterdayRankReward &msg = (GameMsg_C2S_FairyLandGetYesterdayRankReward&)rMsgBase;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "error to get group account[%u] entity.", msg.nAccount);

    DanceGroupFairyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(pRoleEntity->GetDanceGroupID(), pRoleEntity->GetRoleID());
    if (NULL == pPersonalInfo || pPersonalInfo->GetYesterdayTotalScore() == 0)
    {
        return; 
    }

    std::list<FairyLandRankCell> listYesterdayRank;
    GetRankList(pRoleEntity->GetDanceGroupID(), eYesterdayRank, listYesterdayRank);
    bool isInRank = false;
    int nRank = 0;

    for (std::list<FairyLandRankCell>::iterator it = listYesterdayRank.begin(); it != listYesterdayRank.end(); ++it)
    {
        nRank++;
        if (it->m_nRoleId == (int)pRoleEntity->GetRoleID())
        {
            isInRank = true;
            break;
        }
    }

    const int NotZeroScoreRank = 21;
    if (!isInRank)
        nRank = NotZeroScoreRank;

    GameMsg_G2S_FairyLandGetYesterdayRankReward Gmsg;
    Gmsg.m_nRewardId = nRank;
    pRoleEntity->SendPlayerMsg(&Gmsg);

    pPersonalInfo->ClearYesterDaySocer();
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandClearYesterdayScore, pPersonalInfo->GetRoleId());

    return;
}

void CDanceGroupFairyLandProcessor::OnFairyLandOpenBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsgBase.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsgBase.nAccount);


    GameMsg_S2C_FairyLandOpenBless retMsg;
    retMsg.m_nRetCode = GameMsg_S2C_FairyLandOpenBless::Sucess;
    retMsg.m_nLeftTime = 0;
    retMsg.m_strLastBlessRole = "";

    do 
    {
        if (0 == pGroupRole->GetDanceGroupID())
        {
            retMsg.m_nRetCode = GameMsg_S2C_FairyLandOpenBless::NOGroup;
            break;
        }

        GroupInfoMapIt it = m_mapGroupInfo.find(pGroupRole->GetDanceGroupID());
        if (it == m_mapGroupInfo.end())
        {
            break;
        }

        time_t now = time(NULL);
        if (it->second.GetBlessBuffEndTimeStamp() > now)
        {
            retMsg.m_nLeftTime = it->second.GetBlessBuffEndTimeStamp() - now;
            retMsg.m_strLastBlessRole = it->second.GetLastBlessRoleName();
        }

    } while (0);

    pGroupRole->SendPlayerMsg(&retMsg);

    return;
}


void CDanceGroupFairyLandProcessor::OnFairyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_FairyLandBless& msg = (GameMsg_C2S_FairyLandBless&)rMsgBase; 

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "error to get group account[%d] entity on group bless.", msg.nAccount);
    ENSURE_WITH_LOG_CMD(0 != pRoleEntity->GetDanceGroupID(), return, "role[%u] should should has group.", pRoleEntity->GetRoleID());

    GroupInfoMapIt GIt = EnsureGetFairyLandGroup(pRoleEntity->GetDanceGroupID());

    if (GIt->second.GetBlessBuffEndTimeStamp() < time(NULL))
    { // 之前是没有祝福的：
        GameMsg_S2C_FairyLandBlessNotify msg;
        msg.m_nBelss = (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetBlessBuff();
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(pRoleEntity->GetDanceGroupID(), 0, msg);
    }

    GIt->second.AddBlessTime(CDanceGroupFairyLandCfgMgr::Instance().GetBlessAddTime());
    GIt->second.SetLastBlessRole(pRoleEntity->GetRoleID(), pRoleEntity->GetName());
    SharedPtr<FairyLandLastBlessInfoDB> blessInfo(new FairyLandLastBlessInfoDB(pRoleEntity->GetDanceGroupID(), pRoleEntity->GetRoleID(), pRoleEntity->GetName(), GIt->second.GetBlessBuffEndTimeStamp()));
    g_pQueryMgr->AddQuery(QUERY_DacneGroupFairyLandUpdateBlessInfo, blessInfo);

    FairyLandTimeManager::Instance().AddTimer(GIt->second.GetBlessBuffEndTimeStamp(), GIt->first); // 时间管理

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(pRoleEntity->GetDanceGroupID());
    if (GPit == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairyLandPersonalInfo> tmp;
        m_mapGroupIndexPcInfo.insert(std::make_pair(pRoleEntity->GetDanceGroupID(), tmp));
        GPit = m_mapGroupIndexPcInfo.find(pRoleEntity->GetDanceGroupID());
    }

    PersonalInfoMapIt PIt = GPit->second.find(pRoleEntity->GetRoleID());
    if (PIt == GPit->second.end())
    {
        DanceGroupFairyLandPersonalInfo info;
        info.SetGroupId(pRoleEntity->GetDanceGroupID());
        info.SetRoleId(pRoleEntity->GetRoleID());
        AddFairyLandPersonal(pRoleEntity->GetDanceGroupID(), pRoleEntity->GetRoleID(), info);
        PIt = GPit->second.find(pRoleEntity->GetRoleID());
    }

    PIt->second.AddBlassTimes(1);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateBlassTimes, pRoleEntity->GetRoleID(), NULL, PIt->second.GetBlassTimes());
    if (GIt->second.TryAddBlessRank(pRoleEntity->GetRoleID(), PIt->second.GetBlassTimes(), pRoleEntity->GetName()))
    {
        std::string *pRankString = new std::string();
        *pRankString = GIt->second.BlessRankToJson();

        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateBlassRank, pRoleEntity->GetDanceGroupID(), pRankString);
    }

    GameMsg_S2C_FairyLandBless G2Cmsg;
    G2Cmsg.m_nFlag = GameMsg_S2C_FairyLandBless::eSuc;
//     Forward(G2Cmsg, msg.m_nRoleId);
    CGroupRoleEntity::SendPlayerMsgByRoleID(G2Cmsg, pRoleEntity->GetRoleID());
}
/// <summary>
/// DBCallBack
/// <summary>
void CDanceGroupFairyLandProcessor::OnLoadAllInfo(QueryBase &rQuery)
{
    WriteLog(LOGLEVEL_DEBUG, "Dance Group FairyLand Info Load Complete.");

    Query<FairyLandLoadAllDB> &info = (Query<FairyLandLoadAllDB> &)rQuery;
    ENSURE_WITH_LOG_CMD(info.IsValid(), return, "load all info cb should not has unvalid query ptr");

    ConstuctRoleGroupMap();

// 检测： 三部操作并非同步执行，因此需要留下三个标记
    // 清理操作没有执行，则执行
    time_t now = time(NULL);
    // 三个时间戳分别记录最后一期活动的举行时间，每次发完奖励之后需要把这三个时间戳清0
    time_t nDayClearStamp = info->m_nNewDayStamp;
    time_t nPassRewardStamp = info->m_nPassRewardStamp;
    time_t nStorgeBoxStamp = info->m_nStorgeRewardStamp;

    if (NeedDoLastDayClear(nDayClearStamp, now))
    {
        NewDayClear();
    }

    // 通关奖励如果没发，则发送
    if (NeedDoLastPassRewardSend(nPassRewardStamp, now))
    {
        SendAllPassedReward();
    }
    // 储物箱奖励如果没发，则发送
    if (NeedDoLastStorgeBoxSend(nStorgeBoxStamp, now))
    {
        SendAllStorageBoxReward();
    }

    { // 添加祝福时间管理器
        for (GroupInfoMapIt iter = m_mapGroupInfo.begin(); iter != m_mapGroupInfo.end(); ++iter)
        {
            if (iter->second.GetBlessBuffEndTimeStamp() > now)
            {
                FairyLandTimeManager::Instance().AddTimer(iter->second.GetBlessBuffEndTimeStamp(), iter->first);
            }
        }
    }

    // 这之前都不能注册消息，也不能处理数据
    RegisterMessageDealy(); //

    m_bLoadComplete = true;

    m_nPrvCheckTime = now; // 从现在开始检测
    m_nNextAnnouncementTime = NextAnnouncementTime(now);
    m_nNextSendStorageBoxTime = NextStorageBoxRewardTime(now);
    m_nNextSendPassedRewardTime = NextPassRewardTime(now);
    m_bIsFairyLandActive = IsActive(now); // 初始化状态

    if (m_bIsFairyLandActive)
    { // 如果是活动时间，则记录本次活动时间
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairyLandUpdateLastStartTime, (int)time(NULL));
    }

    return;
}

DanceGroupFairyLandPersonalInfo *CDanceGroupFairyLandProcessor::GetPersonalInfo(unsigned int nGroupID, unsigned int nRoleID)
{
    GroupPersonalMapIt iterGroup = m_mapGroupIndexPcInfo.find(nGroupID);
    if (m_mapGroupIndexPcInfo.end() == iterGroup)
        return NULL;

    PersonalInfoMapIt iterPerson = iterGroup->second.find(nRoleID);
    if (iterGroup->second.end() == iterPerson)
        return NULL;

    return &(iterPerson->second);
}

DanceGroupFairyLandGroupInfo *CDanceGroupFairyLandProcessor::GetFairyLandGroupInfo(unsigned int nGroupID)
{
    GroupInfoMapIt iter = m_mapGroupInfo.find(nGroupID);

    if (m_mapGroupInfo.end() == iter)
        return NULL;
         
    return &(iter->second);
}


unsigned char CDanceGroupFairyLandProcessor::GetDefaultChapter() const
{
    return 1; // 每天从第一章开始
}

void CDanceGroupFairyLandProcessor::GetChapterPassReward(FairyLandChapterReward &reward, int nChapter, unsigned char sex)
{
    reward.m_nBindMB = CDanceGroupFairyLandCfgMgr::Instance().GetPassRewardBindMB(nChapter);
    reward.m_nMoney = CDanceGroupFairyLandCfgMgr::Instance().GetPassRewardMoney(nChapter);
    std::list < CRewardUnit > listReward;

    CDanceGroupFairyLandCfgMgr::Instance().GetPassRewardUnit(nChapter, sex, listReward);

    for (std::list<CRewardUnit>::iterator it = listReward.begin(); it != listReward.end(); ++it)
    {
        FairyLandRewardItem i;
        i.m_nItemId = it->m_nItemId;
        i.m_nDuration = it->m_nTime;
        i.m_nCount = it->m_nCount;
        reward.m_listItems.push_back(i);
    }

    return;
}


bool CDanceGroupFairyLandProcessor::IsFairyLandOpening() const
{
    return CGroupSystemSetting::Instance().IsFunctionOpen(EFunctionSwitch_DanceGroupFairyLand);
}

bool CDanceGroupFairyLandProcessor::NeedDoLastDayClear(time_t nLastTime, time_t now)
{ // 不是同一天的数据，则需要清理
    return !IsSameDay(nLastTime, now);
}

bool CDanceGroupFairyLandProcessor::NeedDoLastPassRewardSend(time_t nLastTime, time_t now)
{
    return now >= NextPassRewardTime(nLastTime);
}

bool CDanceGroupFairyLandProcessor::NeedDoLastStorgeBoxSend(time_t nLastTime, time_t now)
{
    return now >= NextStorageBoxRewardTime(nLastTime); // 时间已经超出了最后应该发送的时间
}

void CDanceGroupFairyLandProcessor::CheckRoleGroup(unsigned int nRoleID, unsigned int nGroupID)
{
    std::map<unsigned int, unsigned int>::iterator iter = m_mapRoleGroup.find(nRoleID);
    if (m_mapRoleGroup.end() == iter)
        return;

    if (iter->second == nGroupID)
        return;

    // 不匹配：
    WriteLog(LOGLEVEL_ERROR, "error role[%u], group[%u, %u] not right.", iter->second, nGroupID);

    DelFairyLandPersonal(iter->second, nRoleID);
}

void CDanceGroupFairyLandProcessor::UpdateRoleGroupInfo(unsigned int nRoleID, unsigned int nGroupID)
{
    m_mapRoleGroup[nRoleID] = nGroupID; // 如果有老的，直接覆盖掉
}

void CDanceGroupFairyLandProcessor::RemoveRoleGroupInfo(unsigned int nRoleID)
{
    m_mapRoleGroup.erase(nRoleID);
    return;
}

void CDanceGroupFairyLandProcessor::ConstuctRoleGroupMap()
{
    for (GroupPersonalMapIt iterGroup = m_mapGroupIndexPcInfo.begin(); iterGroup != m_mapGroupIndexPcInfo.end(); ++iterGroup)
    {
        for (PersonalInfoMapIt iterePersonal = iterGroup->second.begin(); iterePersonal != iterGroup->second.end(); ++iterePersonal)
        {
            UpdateRoleGroupInfo(iterePersonal->first, iterGroup->first);
        }
    }

    return;
}


bool CDanceGroupFairyLandProcessor::IsActive(time_t tNow)
{
    time_t zero = GetTodayStartTime(tNow);

    int nTodayTotalSecond = ((unsigned int)tNow - (unsigned int)zero);
    return (nTodayTotalSecond > CDanceGroupFairyLandCfgMgr::Instance().GetStartTimeSecond() &&
        nTodayTotalSecond < CDanceGroupFairyLandCfgMgr::Instance().GetEndTimeSecond());
}


void CDanceGroupFairyLandProcessor::OnClearAllPassedRewardCB(QueryBase &rQuery)
{
    Query<std::list<FairyLandReward> > &lstRewards = (Query<std::list<FairyLandReward> >&)rQuery;

    //DB过关奖励数据删除的回调
    if (rQuery.m_nTag == TRUE && lstRewards.IsValid())
    {
        std::string strSender = CDanceGroupFairyLandCfgMgr::Instance().GetMailSender();
        std::string strTitle = CDanceGroupFairyLandCfgMgr::Instance().GetMailTitle();
        std::string strContext = CDanceGroupFairyLandCfgMgr::Instance().GetMailContext();

        for (std::list<FairyLandReward>::iterator iter = lstRewards->begin(); iter != lstRewards->end(); ++iter)
        {
            //发奖励
            if (iter->bonus.m_nMoney != 0
                || iter->bonus.m_nBindMCoin != 0)
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairyLand,
                    strSender, strTitle, strContext, iter->bonus.m_nMoney, iter->bonus.m_nBindMCoin);
            }

            if (!iter->bonus.m_listItems.empty())
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairyLand, strSender, strTitle, strContext, iter->bonus.m_listItems);
            }
        }
    }
    else
    {
        WriteLog(LOGLEVEL_SYSTEM, "Dance Group FairyLand SendReward Unfinished, Because DBReward Data Clear Fail!");
    }
}

void CDanceGroupFairyLandProcessor::OnClearAllStorageBoxRewardCB(QueryBase &rQuery)
{
    Query<std::list<FairyLandReward> > &lstRewards = (Query<std::list<FairyLandReward> >&)rQuery;
    //DB储物箱奖励数据删除的回调
    if (rQuery.m_nTag == TRUE && lstRewards.IsValid())
    {
        std::string strSender = CDanceGroupFairyLandCfgMgr::Instance().GetMailSender();
        std::string strTitle = CDanceGroupFairyLandCfgMgr::Instance().GetMailTitle();
        std::string strContext = CDanceGroupFairyLandCfgMgr::Instance().GetMailStorageBoxContext();

        std::list<CItem> listPassRewardItemFormat;

        for (std::list<FairyLandReward>::iterator iter = lstRewards->begin(); iter != lstRewards->end(); ++iter)
        {
            if (iter->bonus.m_nMoney != 0
                || iter->bonus.m_nBindMCoin != 0)
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairyLand,
                    strSender, strTitle, strContext, iter->bonus.m_nMoney, iter->bonus.m_nBindMCoin);
            }

            if (!iter->bonus.m_listItems.empty())
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairyLand, strSender, strTitle, strContext, iter->bonus.m_listItems);
            }
        }
    }
    else
    {
        WriteLog(LOGLEVEL_SYSTEM, "Dance Group FairyLand SendStorageBoxReward Unfinished, Because DBReward Data Clear Fail!");
    }
}

