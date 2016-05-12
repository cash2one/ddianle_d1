
#include "../group/Global.h"
#include "../util/CommonFunc.h"
#include "../../socket/GameMsg_Map.h"
#include "../chatGrp/WChat.h"
#include "GameMsg_BigMamaServer.h"
#include "DanceGroupProcessor.h"
#include "BigMamaGroup.h"
#include "../libServerFrame/QueryManager.h"
#include "BigMamaConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../group/GroupSystemSetting.h"

extern CQueryManager* g_pQueryMgr;

BigMamaGroup::BigMamaGroup()
    : GameMsg_Processor(true, true)
    , m_nNextCheckTime(0)
{
    time_t now = time(NULL);
    m_bPrevOpened = IsActive(now);
    m_nNextCheckTime = (unsigned int)CalcNextCheckTime(now); 
}

BigMamaGroup::~BigMamaGroup()
{

}

BigMamaGroup& BigMamaGroup::Instance()
{
    static BigMamaGroup s;
    return s;
}

bool BigMamaGroup::Initialize()
{
    RegisterMsgMap();
    // 加载数据库
    g_pQueryMgr->AddQuery(QUERY_DanceGroup_LoadBigMamaGroupInfo, 0, &m_mapBigMamaInfo); // 把数据全部加载进来吧。

    return true;
}

void BigMamaGroup::RegisterMsgMap()
{
    RegQueryProcessor(QUERY_DanceGroup_LoadBigMamaGroupInfo, &BigMamaGroup::OnLoadBigMamaGroupInfo);
}

void BigMamaGroup::RegisterMsgMapDelay()
{
    RegMsgCreatorAndProcessor(GameMsg_S2G_BigMamaStageInfoRequest, &BigMamaGroup::OnBigMamaStageInfoRequest);
    RegMsgCreatorAndProcessor(GameMsg_S2G_BigMamaFightRequest, &BigMamaGroup::OnS2GBigMamaFightRequest);
    RegMsgCreatorAndProcessor(GameMsg_S2G_BigMamaGetRewardRequest, &BigMamaGroup::OnS2GBigMamaGetRewardRequest);
    RegMsgCreatorAndProcessor(GameMsg_S2G_BigMamaBeatSucess, &BigMamaGroup::OnBigMamaBeatSucess);
}

void BigMamaGroup::OnLoadBigMamaGroupInfo(QueryBase &rQuery)
{
    RegisterMsgMapDelay();
}


void BigMamaGroup::Update(time_t now)
{
    UpdateBigMamaState(now);
    if (m_bPrevOpened)
        UpdateAnnouncement(now);
    
    return;
}

void BigMamaGroup::UpdateBigMamaState(time_t now)
{
    if (m_nNextCheckTime > (unsigned int)now)
        return;

    m_nNextCheckTime = (unsigned int)CalcNextCheckTime(now); // 计算下一次的检测时间

    // 如果之前是开启状态，本次检测又出现一个开启状态，则一定要填充一个关闭的同步消息
    bool openNow = IsActive(now);
    
    if (m_bPrevOpened && openNow)
    {
        GameMsg_G2S_BigMamaStateNotify msg; // 驱动
        msg.m_nState = 0;
        BroadcastMsg2GameServer(&msg);
    }
    m_bPrevOpened = openNow;

    GameMsg_G2S_BigMamaStateNotify msg; // 驱动
    msg.m_nState = openNow ? 1 : 0;
    if (1 == msg.m_nState)
    {
        std::pair<unsigned int, unsigned int> t = GetStartAndEnd(now);
        msg.m_nStartTime = t.first;
        msg.m_nEndTime = t.second;
    }
    BroadcastMsg2GameServer(&msg);

    if (m_bPrevOpened) // 强制发送一次
        Announcement(now);

    return;
}

void BigMamaGroup::UpdateAnnouncement(time_t now)
{
    if (m_bPrevOpened && m_nNextAnnounceTime < (unsigned int)now)
        Announcement(now);
}


void BigMamaGroup::OnRoleLogin(CGroupRoleEntity &role)
{
    GameMsg_G2S_BigMamaInfoNotifyOnLogin msg;
    time_t now = time(NULL);
    msg.m_bOpened = IsActive(now);
    if (msg.m_bOpened)
    {
        std::pair<unsigned int, unsigned int> t = GetStartAndEnd(now);
        msg.m_nStartTime = t.first;
        msg.m_nEndTime = t.second;
    }
    
    DanceGroupBigMamaInfo *pBigMamaInfo = NULL;
    if (0 != role.m_nDanceGroupID)
        pBigMamaInfo = GetDanceGroupMamaInfo(role.m_nDanceGroupID);

    if (NULL != pBigMamaInfo && IsValidBigMamaInfo(*pBigMamaInfo, now))
    { 
        TransfromBigMamaInfoToStageInfo(*pBigMamaInfo, msg.m_property);
    }

    role.SendPlayerMsg(&msg); 

    return;
}

void BigMamaGroup::OnRoleJoinDanceGroup(CGroupRoleEntity &role)
{
    GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup msg;
    time_t now = time(NULL);
    msg.m_bOpened = IsActive(now);
    if (msg.m_bOpened)
    {
        std::pair<unsigned int, unsigned int> t = GetStartAndEnd(now);
        msg.m_nStartTime = t.first;
        msg.m_nEndTime = t.second;
    }

    DanceGroupBigMamaInfo *pBigMamaInfo = NULL;
    if (0 != role.m_nDanceGroupID)
        pBigMamaInfo = GetDanceGroupMamaInfo(role.m_nDanceGroupID);

    if (NULL != pBigMamaInfo && IsValidBigMamaInfo(*pBigMamaInfo, now))
    {
        TransfromBigMamaInfoToStageInfo(*pBigMamaInfo, msg.m_property);
    }

    role.SendPlayerMsg(&msg);

    return;
}


void BigMamaGroup::OnBigMamaStageInfoRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_BigMamaStageInfoRequest *pMsg = (GameMsg_S2G_BigMamaStageInfoRequest *)(&rMsgBase);

    // 如果存在已经生成的信息，直接回出去，不发广播；否则，新生成信息，然后仅发送广播，确保客户端可以得到消息
    // 出错的情况下，仅仅会消息给此客户端
    GameMsg_G2S_BigMamaSyncStageInfo msgToAll;
    GameMsg_G2S_BigMamaStageInfoNotify msgToOne;

    bool toOne = true;

    time_t now = time(NULL);

    do 
    {
        if (!IsActive(now))
        {
            msgToOne.m_result = 1; // 活动已经关闭
            break;
        }

        DanceGroupBigMamaInfo* pBigMamaInfo = GetDanceGroupMamaInfo(pMsg->m_nDanceGroupID);
        if (NULL == pBigMamaInfo)
        { 
            m_mapBigMamaInfo.insert(std::make_pair(pMsg->m_nDanceGroupID, DanceGroupBigMamaInfo()));
            pBigMamaInfo = GetDanceGroupMamaInfo(pMsg->m_nDanceGroupID);
            pBigMamaInfo->m_nDanceGroupID = pMsg->m_nDanceGroupID;
        }

        if (!IsValidBigMamaInfo(*pBigMamaInfo, now))
        { // 
            pBigMamaInfo->m_nStamp = (unsigned int)now;               // 时间戳
            pBigMamaInfo->m_nCurrentRound = 0;          // 第几波
            pBigMamaInfo->m_nState = EBigMamaActiveState_Running;           // 活动状态

            GenerateNewRound(*pBigMamaInfo);
            DanceGroupBigMamaInfo *pCopy = new DanceGroupBigMamaInfo;
            *pCopy = *pBigMamaInfo;
            g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateOrInsertBigMamaGroupInfo, 0, (void*)pCopy);

            toOne = false;
            TransfromBigMamaInfoToStageInfo(*pBigMamaInfo, msgToAll.m_property);
        }
        else
        {
            TransfromBigMamaInfoToStageInfo(*pBigMamaInfo, msgToOne.m_property);
        }

    } while (0);

    if (toOne)
    { // send to one.
        CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(pMsg->nAccount);
        if (pRole != NULL) {
            pRole->SendPlayerMsg(&msgToOne);
        }
    }
    else
    { // notify to all dancegroup member.
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(pMsg->m_nDanceGroupID, 0, msgToAll);
    }

    return;
}

void BigMamaGroup::OnS2GBigMamaFightRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_BigMamaFightRequest *pMsg = (GameMsg_S2G_BigMamaFightRequest *)(&rMsgBase);

    time_t now = time(NULL);
    GameMsg_G2S_BigMamaFightResult retMsg;
    retMsg.m_nPhoneOS = pMsg->m_nPhoneOS;
    retMsg.m_nResult = EFightBigMamaResult_Sucess;
    do 
    {
        // 活动状态：
        if (!IsActive(now))
        {
            retMsg.m_nResult = EFightBigMamaResult_ActiveNotOpen;
            break;
        }

        DanceGroupBigMamaInfo *pBigMamaInfo = GetDanceGroupMamaInfo(pMsg->m_nDanceGroupID);
        if (NULL == pBigMamaInfo || !IsSamePeriod(pBigMamaInfo->m_nStamp, now) || pBigMamaInfo->m_nCurrentRound == 0)
        {
            retMsg.m_nResult = EFightBigMamaResult_LoadStageFailed;
            break; // 
        }

        if (pBigMamaInfo->m_nState != EBigMamaActiveState_Running)
        {
            retMsg.m_nResult = EFightBigMamaResult_FightOver;
            break; 
        }

        retMsg.m_nStageID = pBigMamaInfo->m_nCurrentStageID; // 关卡
        retMsg.m_nRound = pBigMamaInfo->m_nCurrentRound;
        // 下发战斗信息
    } while (0);

    SendMsgToGameServerPlayer(pMsg->m_nRoleID, retMsg);
    return;
}

void BigMamaGroup::OnS2GBigMamaGetRewardRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_BigMamaGetRewardRequest *pMsg = (GameMsg_S2G_BigMamaGetRewardRequest *)(&rMsgBase);

    time_t now = time(NULL);
    GameMsg_G2S_BigMamaGetRewardResult retMsg;
    retMsg.m_nErrorType = EBigMamaRewardResult_Sucess;
    do 
    {
        if (!IsActive(now))
        { 
            retMsg.m_nErrorType = EBigMamaRewardResult_ActiviteClosed;
            break;
        }

        DanceGroupBigMamaInfo *pBigMamaInfo = GetDanceGroupMamaInfo(pMsg->m_nDanceGroupID);
        if (NULL == pBigMamaInfo || !IsSamePeriod(pBigMamaInfo->m_nStamp, now) || pBigMamaInfo->m_nCurrentRound == 0
            || pBigMamaInfo->m_nState != EBigMamaActiveState_AllBeat)
        {
            retMsg.m_nErrorType = EBigMamaRewardResult_NoPrize;
            break; // 
        }

        if (pBigMamaInfo->m_nBeaterType == EBigMamaLastBeatType_XiaoLian)
        {
            retMsg.m_rewardType = (unsigned char)EBigMamaRewardsType_XiaoLian;
        }
        else if (pBigMamaInfo->m_nBeaterType == EBigMamaLastBeatType_XiaoRuan)
        {
            retMsg.m_rewardType = (unsigned char)EBigMamaRewardsType_XiaoRuan;
        }
        else if (pBigMamaInfo->m_nBeaterType == EBigMamaLastBeatType_GroupMember)
        {
            if (pMsg->m_nRoleID == pBigMamaInfo->m_nBeaterRoleID)
                retMsg.m_rewardType = (unsigned char)EBigMamaRewardsType_Killer;
            else if (pMsg->m_bJoined)
                retMsg.m_rewardType = (unsigned char)EBigMamaRewardsType_Join;
            else
                retMsg.m_rewardType = (unsigned char)EBigMamaRewardsType_UnJoin;
        }
    } while (0);

    SendMsgToGameServerPlayer(pMsg->m_nRoleID, retMsg);

    return;
}

void BigMamaGroup::OnBigMamaBeatSucess(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2G_BigMamaBeatSucess *pMsg = (GameMsg_S2G_BigMamaBeatSucess *)(&rMsgBase);

    time_t now = time(NULL);
    do 
    {
        if (!IsActive(now))
            break; // 活动已经结束

        DanceGroupBigMamaInfo* pBigMamaInfo = GetDanceGroupMamaInfo(pMsg->m_nDanceGroupID);
        if (NULL == pBigMamaInfo)
            break; // 没有开始

        if (EBigMamaActiveState_Running != pBigMamaInfo->m_nState)
            break; // 已经打完了

        if (pMsg->m_nRound != pBigMamaInfo->m_nCurrentRound)
            break; // 当前轮次不匹配

        LogDanceGroupBigMamaRecord(pMsg->m_nDanceGroupID, pMsg->m_nRound, CGroupBigMamaDanceRecord::EBeatType_Role, pMsg->m_nRoleID);

        if (pBigMamaInfo->m_nCurrentRound >= g_BigMamaConfigManager.MaxRound())
        { // 成功击杀完毕
//            pBigMamaInfo->m_nCurrentRound++;
            pBigMamaInfo->m_nState = EBigMamaActiveState_AllBeat;
            pBigMamaInfo->m_nBeaterType = EBigMamaLastBeatType_GroupMember;
            pBigMamaInfo->m_nBeaterRoleID = pMsg->m_nRoleID;
            CGroupRoleEntity* roleEnt = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
            if (NULL != roleEnt)
            {
                pBigMamaInfo->m_strBeaterName = roleEnt->GetName();
            }
        }
        else
        {
            GenerateNewRound(*pBigMamaInfo);
        }

        DanceGroupBigMamaInfo *pCopy = new DanceGroupBigMamaInfo;
        *pCopy = *pBigMamaInfo;
        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateOrInsertBigMamaGroupInfo, 0, (void*)pCopy);

        GameMsg_G2S_BigMamaSyncStageInfo msg;
        TransfromBigMamaInfoToStageInfo(*pBigMamaInfo, msg.m_property);

        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(pMsg->m_nDanceGroupID, 0, msg); // 发出去这个消息
    } while (0);

    return;
}


void BigMamaGroup::GenerateNewRound(DanceGroupBigMamaInfo &bigMamaInfo)
{
    const CDanceGroupInfo* pDanceGroup = CDanceGroupProcessor::Instance().GetDanceGroupInfo(bigMamaInfo.m_nDanceGroupID);
    unsigned short lev = 0;
    if (NULL != pDanceGroup)
    {
        lev = pDanceGroup->m_DGBaseInfo.m_nLevel;
    }
    ENSURE_WITH_LOG_CMD(NULL != pDanceGroup, , "dance[%u] group not find...", bigMamaInfo.m_nDanceGroupID);

    unsigned int npcID = 0;
    BigMamaStageInfo* rankStage = g_BigMamaConfigManager.RankStage(lev, npcID);

    if (NULL != rankStage)
    { // new stage.
        bigMamaInfo.m_nCurrentStageID = rankStage->m_nStageID;
        bigMamaInfo.m_nCurrentRound++;
        bigMamaInfo.m_nState = EBigMamaActiveState_Running;
    }
    else
    { // 结束
        bigMamaInfo.m_nCurrentRound++;
        bigMamaInfo.m_nState = EBigMamaActiveState_AllBeat;
        bigMamaInfo.m_nBeaterType = (g_BigMamaConfigManager.XiaoLianID() == npcID ? EBigMamaLastBeatType_XiaoLian : EBigMamaLastBeatType_XiaoRuan);
        int nLogBeatType = (g_BigMamaConfigManager.XiaoLianID() == npcID ? CGroupBigMamaDanceRecord::EBeatType_XiaoLian : CGroupBigMamaDanceRecord::EBeatType_XiaoRuan);
        LogDanceGroupBigMamaRecord(bigMamaInfo.m_nDanceGroupID, bigMamaInfo.m_nCurrentRound, nLogBeatType, 0);
    }

    return;
}

void BigMamaGroup::LogDanceGroupBigMamaRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole)
{
    g_pQueryMgr->AddQuery(QUERY_LogGroupBigMamaDanceRecord, 0, new CGroupBigMamaDanceRecord(nDanceGroupID, nDanceRound, nBeatType, nBeatRole));
}


time_t BigMamaGroup::CalcNextCheckTime(time_t now)
{
    std::pair<unsigned int, unsigned int> t = GetStartAndEnd(now);

    if (now < (time_t)t.first)
        return t.first;                      // 下一个检测时间设置为当天的开始时间
    else if (now <= (time_t)t.second)
        return t.second;                        // 下一个检测时间设置为当天的结束时间
    else
        return t.first + 24 * 60 * 60;       // 下一个检测时间设置为明天的开始时间
}

bool BigMamaGroup::IsActive(time_t now)
{
    std::pair<unsigned int, unsigned int> ret = GetStartAndEnd(now);
    return now >= (time_t)ret.first && now < (time_t)ret.second;
}

std::pair<unsigned int, unsigned int> BigMamaGroup::GetStartAndEnd(time_t now)
{
    int sec = GetHourMinuteSecond(now);
    unsigned int s = (unsigned int)(now - sec + g_BigMamaConfigManager.StartTime());
    unsigned int e = (unsigned int)(now - sec + g_BigMamaConfigManager.EndTime());
    
    return std::make_pair(s, e);
}

bool BigMamaGroup::IsValidBigMamaInfo(const DanceGroupBigMamaInfo& mamInfo, time_t now)
{
    return IsSamePeriod(mamInfo.m_nStamp, now);
}

bool BigMamaGroup::IsSamePeriod(time_t t1, time_t t2)
{
    std::pair<unsigned int, unsigned int> t = GetStartAndEnd(t2);
    return t1 >= (time_t)t.first && t1 < (time_t)t.second;
}

DanceGroupBigMamaInfo* BigMamaGroup::GetDanceGroupMamaInfo(unsigned int nDanceGroupID)
{
    DanceGroupBigMamaMap::iterator iter = m_mapBigMamaInfo.find(nDanceGroupID);

    if (iter == m_mapBigMamaInfo.end())
        return NULL;

    return &iter->second;
}


void BigMamaGroup::TransfromBigMamaInfoToStageInfo(const DanceGroupBigMamaInfo &bigMamaInfo, BigMamaProperty &bigMamaStageInfo)
{
    bigMamaStageInfo.m_bBigMamaAllLose = (bigMamaInfo.m_nState == EBigMamaActiveState_AllBeat ? true : false);
    bigMamaStageInfo.m_nCurRound = bigMamaInfo.m_nCurrentRound;

    if (bigMamaInfo.m_nState == EBigMamaActiveState_AllBeat)
    {
        bigMamaStageInfo.m_nBeaterType = bigMamaInfo.m_nBeaterType;
        bigMamaStageInfo.m_strRoleName = bigMamaInfo.m_strBeaterName;
    }
    else
    {
        BigMamaStageInfo* pStageInfo = g_BigMamaConfigManager.GetStageInfo((unsigned short)bigMamaInfo.m_nCurrentStageID);
        ENSURE_WITH_LOG_CMD(NULL != pStageInfo, return, "miss big mama stage[%u] Info", bigMamaInfo.m_nCurrentStageID);

        bigMamaStageInfo.m_nCurMode = pStageInfo->m_nModeID;
        bigMamaStageInfo.m_nTargetScore = pStageInfo->m_nTargetScore;
    }

    return;
}

void BigMamaGroup::SendMsgToGameServerPlayer(unsigned int nRoleID, GameMsg_Base &msg)
{
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    if (pRole != NULL) {
        pRole->SendPlayerMsg(&msg);
    }
}

void BigMamaGroup::Announcement(time_t now)
{
    if (!CGroupSystemSetting::Instance().IsFunctionOpen(EFunctionSwitch_BigMama))
        return;

    //bool m_bShowInChatWindow;
    //bool m_bIsAdminChat;
    std::string m_strContent;

    CAnnounceBase ann;
    ann.m_bShowInChatWindow = false;
    ann.m_bIsAdminChat = true;
    ann.m_strContent = g_BigMamaConfigManager.AnnouncementString();
    CWChatMgr::Instance().SendAllAnnouncement(ann);


    m_nNextAnnounceTime = (unsigned int)now + g_BigMamaConfigManager.AnnouncementSpace();
}

