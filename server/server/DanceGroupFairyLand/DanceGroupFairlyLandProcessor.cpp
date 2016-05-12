#include "../libServerFrame/QueryManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/Macro_Define.h"
#include "../group/GroupSystemSetting.h"
#include "DanceGroupFairlyLandProcessor.h"
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
#include "DanceGroupFairlyLandTimeManager.h"

extern CQueryManager* g_pQueryMgr;

enum 
{
    eYesterdayRank = 1,
    eTodayRank,
    eBlessRank,
};


struct FairlyLandReward
{
    unsigned int nRoleID;
    CBonus bonus;
};

CDanceGroupFairlyLandProcessor::CDanceGroupFairlyLandProcessor()
    : GameMsg_Processor(true, true)
    , m_nPrvCheckTime(0)
    , m_bIsFairlyLandActive(false)
    , m_bLoadComplete(false)
    , m_nNextSendPassedRewardTime(0)
    , m_nNextSendStorageBoxTime(0)
    , m_nNextAnnouncementTime(0)

{

}

CDanceGroupFairlyLandProcessor::~CDanceGroupFairlyLandProcessor()
{

}

//////////////////////////////////////////////////////////////////////////
//Base Func
//////////////////////////////////////////////////////////////////////////
void CDanceGroupFairlyLandProcessor::Initialize()
{
    if (!IsFairlyLandOpening())
        return;

    RegisterMessage();

    SharedPtr<FairlyLandLoadAllDB> sharedInfo(new FairlyLandLoadAllDB);
    sharedInfo->m_pGInfo = &m_mapGroupInfo;
    sharedInfo->m_pGPInfo = &m_mapGroupIndexPcInfo;

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandLoadAllInfo, sharedInfo);

    return;
}

void CDanceGroupFairlyLandProcessor::RegisterMessage()
{
    // 未开启，或者未加载
    if (!IsFairlyLandOpening())
    {
        return;
    }

    RegQueryProcessor(QUERY_DanceGroupFairlyLandClearAllPassedReward, &CDanceGroupFairlyLandProcessor::OnClearAllPassedRewardCB);
    RegQueryProcessor(QUERY_DanceGroupFairlyLandClearAllStorageBoxReward, &CDanceGroupFairlyLandProcessor::OnClearAllStorageBoxRewardCB);
    RegQueryProcessor(QUERY_DanceGroupFairlyLandLoadAllInfo, &CDanceGroupFairlyLandProcessor::OnLoadAllInfo);

    return;
}

void CDanceGroupFairlyLandProcessor::RegisterMessageDealy()
{
    // 未开启，或者未加载
    if (!IsFairlyLandOpening())
    {
        return;
    }

    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLandCheckIsInYesterdatRank, &CDanceGroupFairlyLandProcessor::OnCheckIsHaveYesterdayReward);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLand_GroupInfo, &CDanceGroupFairlyLandProcessor::OnFairlyLandGroupInfo);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLandStorageboxInfo, &CDanceGroupFairlyLandProcessor::OnFairlyLandStorageBoxInfo);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLandRankInfo, &CDanceGroupFairlyLandProcessor::OnFairlyLandRankInfo);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLandGetYesterdayRankReward, &CDanceGroupFairlyLandProcessor::OnFairlyLandSendYesterDayReward);
    RegMsgCreatorAndProcessor(GameMsg_S2G_FairlyLandBless, &CDanceGroupFairlyLandProcessor::OnFairlyLandBless);
}

void CDanceGroupFairlyLandProcessor::OnUpdate(time_t tNow)
{
    if (!IsFairlyLandOpening())
        return; // 未开启的情况下不检测

    if (!m_bLoadComplete)
        return;

    FairlyLandTimeManager::Instance().OnUpdate(tNow);

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
void CDanceGroupFairlyLandProcessor::NewDayProcess(time_t tNow)
{ // 先清理内存，然后清理数据库
    if ( !IsSameDay(m_nPrvCheckTime, tNow) )
    {
        NewDayClear();
    }
}

void CDanceGroupFairlyLandProcessor::ActiveTimeProcess(time_t tNow)
{
    time_t zero = GetTodayStartTime(tNow);

    bool old = m_bIsFairlyLandActive;

    int nTodayTotalSecond = ((unsigned int)tNow - (unsigned int)zero);
    if (nTodayTotalSecond > CDanceGroupFairlyLandCfgMgr::Instance().GetStartTimeSecond() &&
        nTodayTotalSecond < CDanceGroupFairlyLandCfgMgr::Instance().GetEndTimeSecond() )
    {
        m_bIsFairlyLandActive = true;
    }
    else
    {
        m_bIsFairlyLandActive = false;
    }

    // 记录开启时间
    if ( old == false
        && m_bIsFairlyLandActive == true)
    {
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateLastStartTime, (int)time(NULL));
    }
}

void CDanceGroupFairlyLandProcessor::PassedRewardTimeProcess(time_t tNow)
{
    SendAllPassedReward(); // 直接发了
}

void CDanceGroupFairlyLandProcessor::AnnouncementTimeProcess(time_t tNow)
{
    CAnnounceBase ann;
    ann.m_bShowInChatWindow = true;
    ann.m_bIsAdminChat = false;
    ann.m_strContent = CDanceGroupFairlyLandCfgMgr::Instance().GetEndAnnounceMentContext();
    CWChatMgr::Instance().SendAllAnnouncement(ann);
}

void CDanceGroupFairlyLandProcessor::StorageBoxRewardTimeProcess(time_t tNow)
{
    SendAllStorageBoxReward();
}

time_t CDanceGroupFairlyLandProcessor::NextPassRewardTime(time_t tNow)
{
    int nSec = CDanceGroupFairlyLandCfgMgr::Instance().GetNeedRewardTimeSecond();
    int zero = GetTodayStartTime(tNow);

    return tNow - zero >= nSec ? (zero + DAY_PRE_SECONDS + nSec) : (zero + nSec);
}

time_t CDanceGroupFairlyLandProcessor::NextStorageBoxRewardTime(time_t tNow)
{
    int nTargetSec = CDanceGroupFairlyLandCfgMgr::Instance().GetNeedStorageBoxRewardTimeSecond();
    int nTargetWeek = CDanceGroupFairlyLandCfgMgr::Instance().GetStorageBoxRewardCycleWeek();

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

time_t CDanceGroupFairlyLandProcessor::NextAnnouncementTime(time_t tNow)
{
    int nTargetSec = CDanceGroupFairlyLandCfgMgr::Instance().GetNeedAnnounMentTimeSecond();
    int zero = GetTodayStartTime(tNow);

    return tNow - zero >= nTargetSec ? (zero + DAY_PRE_SECONDS + nTargetSec) : (zero + nTargetSec);
}


void CDanceGroupFairlyLandProcessor::NewDayClear()
{
    RankToYesterDay();
    for (GroupInfoMapIt it = m_mapGroupInfo.begin(); it != m_mapGroupInfo.end(); ++it)
    {
        it->second.SetCurChapterLvl(0);
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

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyNewDay);
}


/// <summary>
/// Func
/// <summary>
void CDanceGroupFairlyLandProcessor::SendJoinReward(int nGroupId, int nRoleid, int nChapter)
{
    GameMsg_G2S_FairlyLandJoinAndRandomReward msg;
    msg.m_nChapter = nChapter;
    msg.m_nIsSendJoin = true;
    CGroupRoleEntity::SendPlayerMsgByRoleID(msg, nRoleid);

}

void CDanceGroupFairlyLandProcessor::SendAllPassedReward()
{
    // 清理内存，并且把数据打包出去
    std::list<FairlyLandReward> *lstRewards = new std::list<FairlyLandReward>;

    for (GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.begin(); GPit != m_mapGroupIndexPcInfo.end(); ++GPit)
    {
        GroupInfoMapIt Git = m_mapGroupInfo.find(GPit->first);
        if (m_mapGroupInfo.end() == Git)
            continue; // 下一个舞团

        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            FairlyLandReward reward;
            reward.nRoleID = Pit->first;
            
            FairlyLandChapterReward passReward;
            for (std::set<int>::iterator iter = Pit->second.m_setPassRewardFalg.begin(); iter != Pit->second.m_setPassRewardFalg.end(); ++iter)
            {
                if (*iter >= Git->second.GetCurChapterLvl()) // 当前关卡，还没有通
                    continue;

                FairlyLandChapterReward tempReward;
                GetChapterPassReward(tempReward, *iter, Pit->second.m_nSex);
                passReward += tempReward;
            }

            //发奖励
            for (std::list<RewardItem>::iterator RIt = passReward.m_listItems.begin(); RIt != passReward.m_listItems.end(); ++RIt)
            {
                reward.bonus.m_listItems.push_back(CItem(RIt->m_nItemId, (unsigned short)RIt->m_nCount, RIt->m_nDuration, false));
            }

            reward.bonus.m_nMoney = passReward.m_nMoney;
            reward.bonus.m_nBindMCoin = passReward.m_nBindMB;

            //清除标志位
            Pit->second.m_setPassRewardFalg.clear();
            Pit->second.m_setJoinRewardFalg.clear();
//             Pit->second.ClearPassedChapterReward(); // 清除掉这些数据

            if (!reward.bonus.m_listItems.empty() || reward.bonus.m_nMoney != 0 || reward.bonus.m_nBindMCoin != 0)
            {
                lstRewards->push_back(reward);
            }
        }
    }

    SharedPtr<std::list<FairlyLandReward> > sharedLstReward(lstRewards);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandClearAllPassedReward, sharedLstReward);

}

void CDanceGroupFairlyLandProcessor::SendAllStorageBoxReward()
{
    SharedPtr<std::list<FairlyLandReward> > shareLstReward(new std::list<FairlyLandReward>);
    for (GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.begin(); GPit != m_mapGroupIndexPcInfo.end(); ++GPit)
    {
        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            FairlyLandReward reward;
            reward.nRoleID = Pit->first;
            //发奖励
            std::list<RewardItem> listReward;
            Pit->second.GetStorageBoxItems(listReward);
            for (std::list<RewardItem>::iterator RIt = listReward.begin();
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

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandClearAllStorageBoxReward, shareLstReward);
}

void CDanceGroupFairlyLandProcessor::AddStroageBox(int GID, int Rid, int chapter)
{
    DanceGroupFairlyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(GID, Rid);
    if (NULL == pPersonalInfo)
        return;

    FairlyLandChapterReward reward;
    reward.m_nBindMB = CDanceGroupFairlyLandCfgMgr::Instance().GetPassRewardBindMB(chapter);
    reward.m_nMoney = CDanceGroupFairlyLandCfgMgr::Instance().GetPassRewardMoney(chapter);
    std::list<CRewardUnit> listReward;
    CDanceGroupFairlyLandCfgMgr::Instance().GetStroageBoxReward(chapter, listReward);

    for (std::list<CRewardUnit>::iterator it = listReward.begin(); it != listReward.end(); ++it)
    {
        RewardItem i;
        i.m_nItemId = it->m_nItemId;
        i.m_nDuration = it->m_nTime;
        i.m_nCount = it->m_nCount;
        pPersonalInfo->AddStorageBoxItem(i);
    }
    std::string * pStorageBox = new std::string(pPersonalInfo->StorageBoxToJson());
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateStorageBox, Rid, pStorageBox);
}

void CDanceGroupFairlyLandProcessor::OnRoleLogin(CGroupRoleEntity &groupRole)
{
    if (!IsFairlyLandOpening() || !m_bLoadComplete)
        return;

    if (groupRole.GetDanceGroupID() == 0)
        return; // 没有舞团

    CheckRoleGroup(groupRole.GetRoleID(), groupRole.GetDanceGroupID());

    SendFairlyLandMsg(groupRole);
    return;
}

void CDanceGroupFairlyLandProcessor::OnRoleQuitGroup(unsigned int nDanceGroup, unsigned int nRoleID)
{
    DelFairlyLandPersonal(nDanceGroup, nRoleID);


}

void CDanceGroupFairlyLandProcessor::OnDanceGroupDisband(unsigned int nDanceGroup)
{
    DelFairlyLandGroup(nDanceGroup);
}

void CDanceGroupFairlyLandProcessor::SendFairlyLandMsg(CGroupRoleEntity &groupRole)
{
    //     发送开关 /
    {
        GameMsg_S2C_FairlyLandStateNotify msg;
        msg.m_nState = m_bIsFairlyLandActive ? GameMsg_S2C_FairlyLandStateNotify::EOpen : GameMsg_S2C_FairlyLandStateNotify::EClose;
        if (m_bIsFairlyLandActive)
        {
            GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
            msg.m_nCurrentChapter = GetDefaultChapter();
            if (m_mapGroupInfo.end() != iter && iter->second.GetCurChapterLvl() != 0)
                msg.m_nCurrentChapter = iter->second.GetCurChapterLvl();
            msg.m_nTotalScore = CDanceGroupFairlyLandCfgMgr::Instance().GetChapterPassScore(msg.m_nCurrentChapter);
        }
        groupRole.SendPlayerMsg(&msg);
    }

    //         发送挑战次数
    if (m_bIsFairlyLandActive)
    {
        DanceGroupFairlyLandPersonalInfo *pPersionalInfo = GetPersonalInfo(groupRole.GetDanceGroupID(), groupRole.GetRoleID());
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairlyLandChanllegeTimesNotify msg;
        msg.m_nChanllegeTimes = 0;
        if (iter != m_mapGroupInfo.end() && NULL != pPersionalInfo && pPersionalInfo->GetLastChallengeChapterLvl() == iter->second.GetCurChapterLvl())
            msg.m_nChanllegeTimes = pPersionalInfo->GetCurChapterChallengeTimes();

        groupRole.SendPlayerMsg(&msg);
    }

    //         发送祝福
    {
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairlyLandBlessNotify msg;
        msg.m_nBelss = 0;
        if (iter != m_mapGroupInfo.end() && time(NULL) > iter->second.GetBlessBuffEndTimeStamp())
            msg.m_nBelss = CDanceGroupFairlyLandCfgMgr::Instance().GetBlessBuff();

        groupRole.SendPlayerMsg(&msg);
    }

    //         发送当前层进度
    {
        GroupInfoMapIt iter = m_mapGroupInfo.find(groupRole.GetDanceGroupID());
        GameMsg_S2C_FairlyLandChapterScoreNotify msg;
        msg.m_nScore = 0;
        if (m_mapGroupInfo.end() != iter)
            msg.m_nScore = iter->second.GetTotalScore(); // 当前进度

        groupRole.SendPlayerMsg(&msg);
    }

    return;
}


void CDanceGroupFairlyLandProcessor::CheckBlessStamp(time_t now, unsigned int nDanceGroupID)
{
    GroupInfoMapIt iter = m_mapGroupInfo.find(nDanceGroupID);
    if (iter == m_mapGroupInfo.end())
        return;

    if (iter->second.GetBlessBuffEndTimeStamp() <= now)
    { // 广播本舞团祝福停止的消息
        GameMsg_S2C_FairlyLandBlessNotify msg;
        msg.m_nBelss = 0; // 祝福已经停了。
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nDanceGroupID, 0, msg);
    }
}


bool CDanceGroupFairlyLandProcessor::CheckAllowReadyInFairlyLandRoom(int nGroupId, int nRoleid)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleid);
    if (NULL == pGroupRole)
        return false; // 

    DanceGroupFairlyLandPersonalInfo *personalInfo = GetPersonalInfo(nGroupId, nRoleid);
    if (NULL == personalInfo)
        return true; // 

    return personalInfo->GetCurChapterChallengeTimes() > CVIPDataMgr::Instance().GetDanceGroupFairlyLandExChance(pGroupRole->VIPLevel());
}

bool CDanceGroupFairlyLandProcessor::CheckAllowStart()
{
    return m_bIsFairlyLandActive;
}

bool CDanceGroupFairlyLandProcessor::CheckIsFinishedAllChapter(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() == GIt)
    {
        return false;
    }

    return (GIt->second.GetCurChapterLvl() > CDanceGroupFairlyLandCfgMgr::Instance().GetMaxChapterLevel());
}

void CDanceGroupFairlyLandProcessor::GroupToNextChapterlvl(int nGroupid)
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
    {
        if (Pit->second.GetLastChallengeChapterLvl() == nCurChapter && Pit->second.GetCurChapterChallengeTimes() == 0)
        {
            Pit->second.SetCurChapterChallengeTimes(0);
            g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateCurChapterChallengeTimes, Pit->second.GetRoleId(), NULL, 0);
        }
        if (Pit->second.CheckIsHaveJoinReward(nCurChapter))
        {
            RecordPassedReward(nCurChapter, Pit->second); // 
        }
    }

    {
        //通知所有人 到下关了
        GameMsg_S2C_FairlyLandUpdateChapterInfo msg;
        msg.m_nChapter = nCurChapter + 1;
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(GIt->first, 0, msg);
    }

    GIt->second.SetCurChapterLvl(nCurChapter + 1);
    GIt->second.SetTotalScore(0);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateCurChatper, nGroupid, NULL, nCurChapter + 1);

    return;
}


void CDanceGroupFairlyLandProcessor::RecordChapterJoinFlag(int nChapter, int nGroupId, int nRoleId)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "role[%u] entity should not null");

    DanceGroupFairlyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(nGroupId, nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pPersonalInfo, return, "don't get group[%u] personal[%u] info ", nGroupId, nRoleId);

    pPersonalInfo->m_nSex = pGroupRole->GetSex();
    pPersonalInfo->SetJoinRewardFlag(nChapter);
    std::string *pStrJoinFlag = new std::string;
    pPersonalInfo->RewardFlagToString(pPersonalInfo->m_nSex, pPersonalInfo->m_setJoinRewardFalg, *pStrJoinFlag);

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateJoinRewardFlag, nRoleId, pStrJoinFlag); // 表示已经参与过这个项目了
}


void CDanceGroupFairlyLandProcessor::RecordPassedReward(int nChapter, DanceGroupFairlyLandPersonalInfo &personalInfo)
{
    personalInfo.SetPassRewardFlag(nChapter);
    std::string *pStrPassFlag = new std::string;
    personalInfo.RewardFlagToString(personalInfo.m_nSex, personalInfo.m_setPassRewardFalg, *pStrPassFlag);

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdatePassedRewardFlag, personalInfo.GetRoleId(), pStrPassFlag);
}

void CDanceGroupFairlyLandProcessor::DanceOverScoreProcess(int nGroup, int nChapter, std::list<SDancerScore>& listScore)
{
    GroupInfoMapIt GIt = EnsureGetFairlyLandGroup(nGroup);

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroup);
    if (GPit == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairlyLandPersonalInfo> tmp;
        m_mapGroupIndexPcInfo.insert(std::make_pair(nGroup, tmp));
        GPit = m_mapGroupIndexPcInfo.find(nGroup);
    }

    int nBlessBuf = 0; // 祝福buff
    if (GIt->second.GetBlessBuffEndTimeStamp() > (unsigned int)time(NULL))
    {
        nBlessBuf = CDanceGroupFairlyLandCfgMgr::Instance().GetBlessBuff();
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

        DSIt->m_nVipBuf = CVIPDataMgr::Instance().GetDanceGroupFairlyLandVipBuf(pGroupRole->VIPLevel());
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
            DanceGroupFairlyLandPersonalInfo info;
            info.SetGroupId(nGroup);
            info.SetRoleId(DSIt->m_nRoleId);
            AddFairlyLandPersonal(nGroup, DSIt->m_nRoleId, info);
            PIt = GPit->second.find(DSIt->m_nRoleId);
        }

        //结算加成的分数
        int nScore = DSIt->m_nScore + (DSIt->m_nScore*(nTotalVipBuf + nBlessBuf)/100);
        nTotalScore += nScore;

        // 记录玩家总分（未加成）
        int nRoleid = PIt->first;
        PIt->second.AddTotalScore(DSIt->m_nScore);
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateTotalScore, DSIt->m_nRoleId, NULL, PIt->second.GetTotalScore());

        // 记录玩家挑战次数
        int challengetimes = PIt->second.GetCurChapterChallengeTimes() + 1;
        PIt->second.SetCurChapterChallengeTimes(challengetimes);
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateCurChapterChallengeTimes, DSIt->m_nRoleId, NULL, challengetimes);

        // 推送挑战次数：
        {
            GameMsg_S2C_FairlyLandChanllegeTimesNotify msg;
            msg.m_nChanllegeTimes = challengetimes;
            pGroupRole->SendPlayerMsg(&msg);
        }

        // 更新排名
        if (GIt->second.TryAddTodayRank(nRoleid, PIt->second.GetTotalScore(), pGroupRole->GetName()))
        {
            std::string *pStrRank = new std::string();
            *pStrRank = GIt->second.ScoreTodayRankToJson();
            g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateTodayRank, nGroup, pStrRank);
        }

        // 发放参与奖励
        SendJoinReward(nGroup, nRoleid, nChapter);

        // 记录通关奖励（秘境结束之后，统一发放）
        if (nChapter < GIt->second.GetCurChapterLvl() &&
            !(PIt->second.CheckIsHaveJoinReward(nChapter) ) )
        {
            RecordChapterJoinFlag(nChapter, nGroup, nRoleid);
        }


        if (nChapter != PIt->second.GetLastChallengeChapterLvl())
        {
            PIt->second.SetLastChallengeChapterLvl(nChapter);
            g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateLastChallengeChapterLvl, DSIt->m_nRoleId, NULL, nChapter);
        }

        // 舞团奖励
        if (DSIt->m_nScore != 0)
        {
            AddStroageBox(nGroup ,nRoleid, nChapter);
        }
    }

    TryAddFairlyLandGroupScore(nGroup, nChapter, nTotalScore);

    //广播舞蹈结束 发送消息
    GameMsg_S2C_FairlyLandDanceEndInfo msg;
    msg.m_nChapterScore = GIt->second.GetTotalScore();
    msg.m_nChapterTotalScore = CDanceGroupFairlyLandCfgMgr::Instance().GetChapterPassScore(GIt->second.GetCurChapterLvl());
    msg.m_nBlessBuf = nBlessBuf;
    msg.m_listScore = listScore;

    for (std::list<SDancerScore>::iterator DSIt = listScore.begin();
        DSIt != listScore.end(); ++DSIt)
    {
        CGroupRoleEntity::SendPlayerMsgByRoleID(msg, DSIt->m_nRoleId);
//        Forward(msg, DSIt->m_nRoleId);
    }
}

void CDanceGroupFairlyLandProcessor::RankToYesterDay()
{
    std::list<FairlyLandRankCell> rank;
    for (GroupInfoMapIt GIt = m_mapGroupInfo.begin();
        GIt != m_mapGroupInfo.end(); ++GIt)
    {
        GIt->second.GetScoreTodayRank(rank);
        GIt->second.SetYestedayRank(rank);
        GIt->second.ClearTodayRank();
    }
}

void CDanceGroupFairlyLandProcessor::TryAddFairlyLandGroupScore(int nGroup, int nChapter, int nAddScore)
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

    if (GIt->second.GetTotalScore() > CDanceGroupFairlyLandCfgMgr::Instance().GetChapterPassScore(nChapter) )
    {
        GroupToNextChapterlvl(nGroup);

        GameMsg_S2C_FairlyLandCurrentChapterNotify msg;
        msg.m_nCurrentChapter = GIt->second.GetCurChapterLvl();
        msg.m_nTotalScore = CDanceGroupFairlyLandCfgMgr::Instance().GetChapterPassScore(msg.m_nCurrentChapter);
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nGroup, 0, msg);
    }
    else // 如果进入下一关，则可以省却发送进度变更消息
    {
        // 广播进度变更消息：
        GameMsg_S2C_FairlyLandChapterScoreNotify msg;
        msg.m_nScore = GIt->second.GetTotalScore() + nAddScore;
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(nGroup, 0, msg);
    }


    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateGroupScore, nGroup, NULL, GIt->second.GetTotalScore());
}

int CDanceGroupFairlyLandProcessor::GetCurChapterLvl(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return GIt->second.GetCurChapterLvl();
    }

    return GetDefaultChapter(); 
}

int CDanceGroupFairlyLandProcessor::GetCurMusic(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairlyLandCfgMgr::Instance().GetChapterMusicId(GIt->second.GetCurChapterLvl());
    }
    return 0;
}

int CDanceGroupFairlyLandProcessor::GetCurScene(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairlyLandCfgMgr::Instance().GetChapterSceneId(GIt->second.GetCurChapterLvl());
    }

    return 0;
}

int CDanceGroupFairlyLandProcessor::GetDifficultyLvl(int nGroupId)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != GIt)
    {
        return CDanceGroupFairlyLandCfgMgr::Instance().GetChapterDifficultyLevel(GIt->second.GetCurChapterLvl());
    }

    return 0;
}

CDanceGroupFairlyLandProcessor::GroupInfoMapIt CDanceGroupFairlyLandProcessor::EnsureGetFairlyLandGroup(unsigned int nGroupID)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupID);
    if (m_mapGroupInfo.end() == GIt)
    {
        DanceGroupFairlyLandGroupInfo info;
        info.SetGroupId(nGroupID);
        info.SetCurChapterLvl(GetDefaultChapter());
        AddFairlyLandGroup(info);
        GIt = m_mapGroupInfo.find(nGroupID);
    }
    return GIt;
}


void CDanceGroupFairlyLandProcessor::AddFairlyLandGroup(DanceGroupFairlyLandGroupInfo& Ginfo)
{
    GroupInfoMapIt GIt = m_mapGroupInfo.find(Ginfo.GetGroupId());
    if (GIt == m_mapGroupInfo.end())
    {
        m_mapGroupInfo.insert(std::make_pair(Ginfo.GetGroupId(), Ginfo));

        DanceGroupFairlyLandGroupInfo *pDbInfo = new DanceGroupFairlyLandGroupInfo(Ginfo);
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandAddGroupInfo, Ginfo.GetGroupId(), pDbInfo);
    }
}

void CDanceGroupFairlyLandProcessor::DelFairlyLandGroup(int nGroupId)
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
        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandDelGroupInfo, nGroupId);
    }
}

void CDanceGroupFairlyLandProcessor::AddFairlyLandPersonal(int nGroupId, int nRoleId, DanceGroupFairlyLandPersonalInfo &Pinfo)
{
    GroupPersonalMapIt PIt = m_mapGroupIndexPcInfo.find(nGroupId);
    if (PIt == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairlyLandPersonalInfo> info;
        info.insert(std::make_pair(nRoleId, Pinfo));
        m_mapGroupIndexPcInfo.insert(std::make_pair(nGroupId, info));
        UpdateRoleGroupInfo(nRoleId, nGroupId);
    }
    else
    {
        PIt->second.insert(std::make_pair(nRoleId, Pinfo));
        UpdateRoleGroupInfo(nRoleId, nGroupId);
    }
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandAddPersonalInfo, nGroupId, NULL, nRoleId);
}

void CDanceGroupFairlyLandProcessor::DelFairlyLandPersonal(int nGroupId, int nRoleId)
{
    GroupPersonalMapIt PIt = m_mapGroupIndexPcInfo.find(nGroupId);
    if (PIt != m_mapGroupIndexPcInfo.end())
    {
        PIt->second.erase(nRoleId);
        RemoveRoleGroupInfo(nRoleId);
    }

    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandDelPersonalInfo, nGroupId, NULL, nRoleId);
}

void CDanceGroupFairlyLandProcessor::GetRankList(int nGroupId, int nType, std::list<FairlyLandRankCell> &rRank)
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
// void CDanceGroupFairlyLandProcessor::TryAddTodayRank(int nGroupId, int nRoleId, int nSorce)
// {
//     GroupInfoMapIt GIt = m_mapGroupInfo.find(nGroupId);
//     if (m_mapGroupInfo.end() == GIt)
//     {
//         DanceGroupFairlyLandGroupInfo info;
//         info.SetGroupId(nGroupId);
//         AddFairlyLandGroup(info);
//     }
// 
//     GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
//     if (m_mapGroupIndexPcInfo.end() == GPit)
//     {
//         std::map<int, DanceGroupFairlyLandPersonalInfo> tmp;
//         m_mapGroupIndexPcInfo.insert(std::make_pair(nGroupId, tmp));
//     }
// 
//     PersonalInfoMapIt Pit = GPit->second.find(nRoleId);
//     if (GPit->second.end() == Pit)
//     {
//         DanceGroupFairlyLandPersonalInfo info;
//         info.SetGroupId(nGroupId);
//         info.SetRoleId(nRoleId);
//         AddFairlyLandPersonal(nGroupId, nRoleId, info);
//     }
// 
//     if (GIt->second.TryAddTodayRank(nRoleId, Pit->second.GetTotalScore()))
//     {
//         std::list<FairlyLandRankCell> *plist = new std::list<FairlyLandRankCell>();
//         GIt->second.GetScoreTodayRank(*plist);
//         g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateTodayRank, nGroupId, plist);
//     }
// }

/// <summary>
/// MsgCB
/// <summary>
void CDanceGroupFairlyLandProcessor::OnCheckIsHaveYesterdayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairlyLandCheckIsInYesterdatRank &rMsg = (GameMsg_S2G_FairlyLandCheckIsInYesterdatRank&)rMsgBase;
    int nYesterdayScore = 0;
    DanceGroupFairlyLandPersonalInfo *info = GetPersonalInfo(rMsg.m_nGroupId, rMsg.m_nRoleId);
    if (NULL != info)
        nYesterdayScore = info->GetYesterdayTotalScore();

    GameMsg_S2C_FairlyLandCheckIsInYesterdatRank msg;
    msg.m_nYesterdayScore = nYesterdayScore;
    CGroupRoleEntity::SendPlayerMsgByRoleID(msg, rMsg.m_nRoleId);
//     Forward(msg, rMsg.m_nRoleId);
}

void CDanceGroupFairlyLandProcessor::OnFairlyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{

    GameMsg_S2G_FairlyLand_GroupInfo &rMsg = (GameMsg_S2G_FairlyLand_GroupInfo&)rMsgBase;
    int nGroupId = rMsg.m_nGroupId;
    int nRoleId = rMsg.m_nRoleId;

    if (!m_bIsFairlyLandActive)
    {
        GameMsg_S2C_FairlyLandError errormsg;
        errormsg.m_nFlag = eFairylandClosed;
        CGroupRoleEntity::SendPlayerMsgByRoleID(errormsg, nRoleId);
        return;
    }

    GameMsg_S2C_FairlyLand_GroupInfo retmsg;
    GroupInfoMapIt it = m_mapGroupInfo.find(nGroupId);
    if (m_mapGroupInfo.end() != it)
    {
        if (it->second.GetBlessBuffEndTimeStamp() > (unsigned int ) time(NULL))
        {
            retmsg.m_nTotalScore = CDanceGroupFairlyLandCfgMgr::Instance().GetChapterPassScore(it->second.GetCurChapterLvl());
        }
        
        retmsg.m_nBlessBuff = CDanceGroupFairlyLandCfgMgr::Instance().GetBlessBuff();
        retmsg.m_nChapter = it->second.GetCurChapterLvl();
        retmsg.m_nScore = it->second.GetTotalScore();
    }
    else
    {
        //初始化为第一关
        retmsg.m_nChapter = GetDefaultChapter();
    }

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
    if (m_mapGroupIndexPcInfo.end() != GPit)
    {
        PersonalInfoMapIt Pit = GPit->second.find(nRoleId);
        if (GPit->second.end() != Pit)
        {
            retmsg.m_nChanllegeTimes = Pit->second.GetCurChapterChallengeTimes();
        }
    }

//    Forward(retmsg, nRoleId);
    CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, nRoleId);
}

void CDanceGroupFairlyLandProcessor::OnFairlyLandStorageBoxInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairlyLandStorageboxInfo msg = (GameMsg_S2G_FairlyLandStorageboxInfo&)rMsgBase;
    int nGroupId = msg.m_nGroupId;
    int nRoleId = msg.m_nRoleId;


    GameMsg_S2C_FairlyLandStorageboxInfo retmsg;
    int nTotalCount = 0;
    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(nGroupId);
    if (m_mapGroupIndexPcInfo.end() != GPit)
    {
        for (PersonalInfoMapIt Pit = GPit->second.begin(); Pit != GPit->second.end(); ++Pit)
        {
            std::list<RewardItem>  listItems;
            Pit->second.GetStorageBoxItems(listItems);
            for (std::list<RewardItem>::iterator it = listItems.begin(); it != listItems.end(); ++it)
            {
                nTotalCount += it->m_nCount;
            }
        }
    }
    retmsg.m_nTotalStorageBoxItemsCount = nTotalCount;
//    Forward(retmsg, nRoleId);
    CGroupRoleEntity::SendPlayerMsgByRoleID(retmsg, nRoleId);
}

void CDanceGroupFairlyLandProcessor::OnFairlyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairlyLandRankInfo &msg = (GameMsg_S2G_FairlyLandRankInfo&)rMsgBase;

    const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(msg.m_nGroupId);
    ENSURE_WITH_LOG_CMD(NULL != pDGInfo, return, "error to get account[%u] group[%d] info", msg.nAccount, msg.m_nGroupId);

    GameMsg_S2C_FairlyLandRankInfo retmsg;
    std::list<FairlyLandRankCell> list;
    std::list<SRankCell> msglist;
    retmsg.m_nType = msg.m_nType;

    GetRankList(msg.m_nGroupId, msg.m_nType, list);
    const int nMaxSize = 5; // 控制每次发送的个数

    for (std::list<FairlyLandRankCell>::iterator it = list.begin();
        it != list.end();)
    {
        msglist.clear();

        for (int tmp = 0; tmp >= nMaxSize; ++tmp)
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

void CDanceGroupFairlyLandProcessor::OnFairlyLandSendYesterDayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairlyLandGetYesterdayRankReward &msg = (GameMsg_S2G_FairlyLandGetYesterdayRankReward&)rMsgBase;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(msg.m_nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "error to get group role[%d] entity.", msg.m_nRoleId);

    DanceGroupFairlyLandPersonalInfo *pPersonalInfo = GetPersonalInfo(msg.m_nGroupId, msg.m_nRoleId);
    if (NULL == pPersonalInfo || pPersonalInfo->GetYesterdayTotalScore() == 0)
    {
        GameMsg_S2C_FairlyLandError ermsg;
        ermsg.m_nFlag = eNoRankReward;
        CGroupRoleEntity::SendPlayerMsgByRoleID(ermsg, msg.m_nRoleId);
        return; 
    }

    std::list<FairlyLandRankCell> listYesterdayRank;
    GetRankList(msg.m_nGroupId, eYesterdayRank, listYesterdayRank);
    bool isInRank = false;
    int nRank = 0;

    for (std::list<FairlyLandRankCell>::iterator it = listYesterdayRank.begin(); it != listYesterdayRank.end(); ++it)
    {
        nRank++;
        if (it->m_nRoleId == msg.m_nRoleId)
        {
            isInRank = true;
            break;
        }
    }

    const int NotZeroScoreRank = 21;
    if (!isInRank)
        nRank = NotZeroScoreRank;

    GameMsg_G2S_FairlyLandGetYesterdayRankReward Gmsg;
    Gmsg.m_nRewardId = nRank;
    pRoleEntity->SendPlayerMsg(&msg);

    pPersonalInfo->ClearYesterDaySocer();
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandClearYesterdayScore, pPersonalInfo->GetRoleId());

    return;
}

void CDanceGroupFairlyLandProcessor::OnFairlyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_FairlyLandBless& msg = (GameMsg_S2G_FairlyLandBless&)rMsgBase;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(msg.m_nRoleId);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "error to get group role[%d] entity on group bless.", msg.m_nRoleId);

    GroupInfoMapIt GIt = EnsureGetFairlyLandGroup(msg.m_nGroupId);

    if (GIt->second.GetBlessBuffEndTimeStamp() < time(NULL))
    { // 之前是没有祝福的：
        GameMsg_S2C_FairlyLandBlessNotify msg;
        msg.m_nBelss = CDanceGroupFairlyLandCfgMgr::Instance().GetBlessBuff();
        pRoleEntity->SendPlayerMsg(&msg);
    }

    GIt->second.AddBlessTime(CDanceGroupFairlyLandCfgMgr::Instance().GetBlessAddTime());

    SharedPtr<FairlyLandLastBlessInfoDB> blessInfo(new FairlyLandLastBlessInfoDB(msg.m_nGroupId, pRoleEntity->GetRoleID(), pRoleEntity->GetName(), GIt->second.GetBlessBuffEndTimeStamp()));
    g_pQueryMgr->AddQuery(QUERY_DacneGroupFairlyLandUpdateBlessInfo, blessInfo);

    FairlyLandTimeManager::Instance().AddTimer(GIt->second.GetBlessBuffEndTimeStamp(), GIt->first); // 时间管理

    GroupPersonalMapIt GPit = m_mapGroupIndexPcInfo.find(msg.m_nGroupId);
    if (GPit == m_mapGroupIndexPcInfo.end())
    {
        std::map<int, DanceGroupFairlyLandPersonalInfo> tmp;
        m_mapGroupIndexPcInfo.insert(std::make_pair(msg.m_nGroupId, tmp));
        GPit = m_mapGroupIndexPcInfo.find(msg.m_nGroupId);
    }

    PersonalInfoMapIt PIt = GPit->second.find(msg.m_nRoleId);
    if (PIt == GPit->second.end())
    {
        DanceGroupFairlyLandPersonalInfo info;
        info.SetGroupId(msg.m_nGroupId);
        info.SetRoleId(msg.m_nRoleId);
        AddFairlyLandPersonal(msg.m_nGroupId, msg.m_nRoleId, info);
        PIt = GPit->second.find(msg.m_nRoleId);
    }

    PIt->second.AddBlassTimes(1);
    g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateBlassTimes, msg.m_nRoleId, NULL, PIt->second.GetBlassTimes());
    if (GIt->second.TryAddBlessRank(msg.m_nRoleId, PIt->second.GetBlassTimes(),pRoleEntity->GetName()))
    {
        std::string *pRankString = new std::string();
        *pRankString = GIt->second.BlessRankToJson();

        g_pQueryMgr->AddQuery(QUERY_DanceGroupFairlyLandUpdateBlassRank, msg.m_nGroupId, pRankString);
    }

    GameMsg_S2C_FairlyLandBless G2Cmsg;
    G2Cmsg.m_nFlag = eSuc;
//     Forward(G2Cmsg, msg.m_nRoleId);
    CGroupRoleEntity::SendPlayerMsgByRoleID(G2Cmsg, msg.m_nRoleId);
}
/// <summary>
/// DBCallBack
/// <summary>
void CDanceGroupFairlyLandProcessor::OnLoadAllInfo(QueryBase &rQuery)
{
    WriteLog(LOGLEVEL_DEBUG, "Dance Group FairlyLand Info Load Complete.");

    Query<FairlyLandLoadAllDB> &info = (Query<FairlyLandLoadAllDB> &)rQuery;
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
                FairlyLandTimeManager::Instance().AddTimer(iter->second.GetBlessBuffEndTimeStamp(), iter->first);
            }
        }
    }

    // 这之前都不能注册消息，也不能处理数据
    RegisterMessageDealy(); //

    m_bLoadComplete = true;

    m_nPrvCheckTime = now; // 从现在开始检测
    m_nNextAnnouncementTime = NextAnnouncementTime(now);
    m_nNextSendPassedRewardTime = NextStorageBoxRewardTime(now);
    m_nNextSendPassedRewardTime = NextPassRewardTime(now);
}

DanceGroupFairlyLandPersonalInfo *CDanceGroupFairlyLandProcessor::GetPersonalInfo(unsigned int nGroupID, unsigned int nRoleID)
{
    GroupPersonalMapIt iterGroup = m_mapGroupIndexPcInfo.find(nGroupID);
    if (m_mapGroupIndexPcInfo.end() == iterGroup)
        return NULL;

    PersonalInfoMapIt iterPerson = iterGroup->second.find(nRoleID);
    if (iterGroup->second.end() == iterPerson)
        return NULL;

    return &(iterPerson->second);
}

int CDanceGroupFairlyLandProcessor::GetDefaultChapter() const
{
    return 1; // 每天从第一章开始
}

void CDanceGroupFairlyLandProcessor::GetChapterPassReward(FairlyLandChapterReward &reward, int nChapter, unsigned char sex)
{
    reward.m_nBindMB = CDanceGroupFairlyLandCfgMgr::Instance().GetPassRewardBindMB(nChapter);
    reward.m_nMoney = CDanceGroupFairlyLandCfgMgr::Instance().GetPassRewardMoney(nChapter);
    std::list < CRewardUnit > listReward;

    CDanceGroupFairlyLandCfgMgr::Instance().GetPassRewardUnit(nChapter, sex, listReward);

    for (std::list<CRewardUnit>::iterator it = listReward.begin(); it != listReward.end(); ++it)
    {
        RewardItem i;
        i.m_nItemId = it->m_nItemId;
        i.m_nDuration = it->m_nTime;
        i.m_nCount = it->m_nCount;
        reward.m_listItems.push_back(i);
    }

    return;
}


bool CDanceGroupFairlyLandProcessor::IsFairlyLandOpening() const
{
    return CGroupSystemSetting::Instance().IsFunctionOpen(EFunctionSwitch_DanceGroupFairlyLand);
}

bool CDanceGroupFairlyLandProcessor::NeedDoLastDayClear(time_t nLastTime, time_t now)
{ // 不是同一天的数据，则需要清理
    return !IsSameDay(nLastTime, now);
}

bool CDanceGroupFairlyLandProcessor::NeedDoLastPassRewardSend(time_t nLastTime, time_t now)
{
    return now >= NextPassRewardTime(nLastTime);
}

bool CDanceGroupFairlyLandProcessor::NeedDoLastStorgeBoxSend(time_t nLastTime, time_t now)
{
    return now >= NextStorageBoxRewardTime(nLastTime); // 时间已经超出了最后应该发送的时间
}

void CDanceGroupFairlyLandProcessor::CheckRoleGroup(unsigned int nRoleID, unsigned int nGroupID)
{
    std::map<unsigned int, unsigned int>::iterator iter = m_mapRoleGroup.find(nRoleID);
    if (m_mapRoleGroup.end() == iter)
        return;

    if (iter->second == nGroupID)
        return;

    // 不匹配：
    WriteLog(LOGLEVEL_ERROR, "error role[%u], group[%u, %u] not right.", iter->second, nGroupID);

    DelFairlyLandPersonal(iter->second, nRoleID);
}

void CDanceGroupFairlyLandProcessor::UpdateRoleGroupInfo(unsigned int nRoleID, unsigned int nGroupID)
{
    m_mapRoleGroup[nRoleID] = nGroupID; // 如果有老的，直接覆盖掉
}

void CDanceGroupFairlyLandProcessor::RemoveRoleGroupInfo(unsigned int nRoleID)
{
    m_mapRoleGroup.erase(nRoleID);
    return;
}

void CDanceGroupFairlyLandProcessor::ConstuctRoleGroupMap()
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


void CDanceGroupFairlyLandProcessor::OnClearAllPassedRewardCB(QueryBase &rQuery)
{
    Query<std::list<FairlyLandReward> > &lstRewards = (Query<std::list<FairlyLandReward> >&)rQuery;

    //DB过关奖励数据删除的回调
    if (rQuery.m_nTag == TRUE && lstRewards.IsValid())
    {
        std::string strSender = CDanceGroupFairlyLandCfgMgr::Instance().GetMailSender();
        std::string strTitle = CDanceGroupFairlyLandCfgMgr::Instance().GetMailTitle();
        std::string strContext = CDanceGroupFairlyLandCfgMgr::Instance().GetMailContext();

        for (std::list<FairlyLandReward>::iterator iter = lstRewards->begin(); iter != lstRewards->end(); ++iter)
        {
            //发奖励
            if (iter->bonus.m_nMoney != 0
                || iter->bonus.m_nBindMCoin != 0)
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairlyLand,
                    strSender, strTitle, strContext, iter->bonus.m_nMoney, iter->bonus.m_nBindMCoin);
            }

            if (!iter->bonus.m_listItems.empty())
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairlyLand, strSender, strTitle, strContext, iter->bonus.m_listItems);
            }
        }
    }
    else
    {
        WriteLog(LOGLEVEL_SYSTEM, "Dance Group FairlyLand SendReward Unfinished, Because DBReward Data Clear Fail!");
    }
}

void CDanceGroupFairlyLandProcessor::OnClearAllStorageBoxRewardCB(QueryBase &rQuery)
{
    Query<std::list<FairlyLandReward> > &lstRewards = (Query<std::list<FairlyLandReward> >&)rQuery;
    //DB储物箱奖励数据删除的回调
    if (rQuery.m_nTag == TRUE && lstRewards.IsValid())
    {
        std::string strSender = CDanceGroupFairlyLandCfgMgr::Instance().GetMailSender();
        std::string strTitle = CDanceGroupFairlyLandCfgMgr::Instance().GetMailTitle();
        std::string strContext = CDanceGroupFairlyLandCfgMgr::Instance().GetMailStorageBoxContext();

        std::list<CItem> listPassRewardItemFormat;

        for (std::list<FairlyLandReward>::iterator iter = lstRewards->begin(); iter != lstRewards->end(); ++iter)
        {
            if (iter->bonus.m_nMoney != 0
                || iter->bonus.m_nBindMCoin != 0)
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairlyLand,
                    strSender, strTitle, strContext, iter->bonus.m_nMoney, iter->bonus.m_nBindMCoin);
            }

            if (!iter->bonus.m_listItems.empty())
            {
                CWMailManager::Instance().SendMailToPlayer(iter->nRoleID, EMailType_DanceGroupFairlyLand, strSender, strTitle, strContext, iter->bonus.m_listItems);
            }
        }
    }
    else
    {
        WriteLog(LOGLEVEL_SYSTEM, "Dance Group FairlyLand SendStorageBoxReward Unfinished, Because DBReward Data Clear Fail!");
    }
}

