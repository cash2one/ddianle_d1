
#include "GameMsg_BigMama.h"
#include "GameMsg_BigMamaServer.h"
#include "../../socket/Log.h"

#include "EntityDanceGroupComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../room/BigMamaRoom.h"
#include "../roomLib/RoomMarkInfo.h"
#include "../room/RoomBase.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "BigMamaConfig.h"
#include "BigMamaConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/Localization.h"
#include "../item/ItemProcess.h"
#include "../../socket/GameMsg_Map.h"
#include "../mail/MailMgr.h"

#include "EntityBigMama.h"

// unsigned int m_stampLastPrize;                  // ��һ����ȡ���˽�����ʱ���
// EBigMamaPrizeState m_prizeState;                // ���˵Ľ���״̬��ע�⣺����ֻ���ڻ���еĹ����в�����ȡ��
// unsigned int m_stampLastJoin;                   // �ϴβ���ʱ���
// bool m_bParticipant;                            // �Ƿ��ǲ�����
// 
// unsigned int m_nRoomID;                         // ���跿���ID
// unsigned char m_nBeatRound;                     // ����������
// unsigned int m_nTargetScore;                    // �����Ŀ�����

EntityBigMama::EntityBigMama(CEntityDanceGroupComponent* ptrDanceGroupComponent)
//     : m_bHasData(false)
    : m_ptrDanceGroupComponent(ptrDanceGroupComponent)
    , m_ptrAttributeComponent(NULL)
    , m_pRoleEntity(NULL)
    , m_stampLastPrize(0)
    , m_prizeState(EBigMamaPrizeState_NoPrize)
    , m_stampLastJoin(0)
    , m_bParticipant(false)
    , m_nRoomID(INVALID_ROOM_ID)
    , m_nBeatRound(0)
    , m_nTargetScore(0)
    , m_bReceiveGroupData(false)
{
}

EntityBigMama::~EntityBigMama()
{

}

void EntityBigMama::Init(CEntityAttributeComponent* pAttr, CRoleEntity* pRole)
{
    m_pRoleEntity = pRole;
    m_ptrAttributeComponent = pAttr;

    return;
}


// �����½ʱ���·���Ϣ
void EntityBigMama::OnLogin()
{ 
    // �·��������Ϣ
    {
        GameMsg_S2C_BigMamaIntroduce msg;
        msg.m_strIntroduce = g_BigMamaConfigManager.IntroductString();
        SendMsgToPlayer(msg);
    }

    // �����������
    CheckGuidePrizeOnLogin();


}

void EntityBigMama::OnLogout()
{ // �˳���ʱ�򣬹ر��������
    if (INVALID_ROOM_ID != m_nRoomID)
    {
        unsigned int nScore;
        CRoomMarkInfo markInfo;
        BigMamaRoomManager::Instance().End(m_nRoomID, m_pRoleEntity->GetRoleID(), nScore, markInfo);
    }
}

void EntityBigMama::CreateFromDB(ROLEINFO_DB *pRoleInforDB)
{
    m_stampLastPrize = pRoleInforDB->m_bigMamaRoleInfo.m_nPrizeStamp;
    m_stampLastJoin = pRoleInforDB->m_bigMamaRoleInfo.m_nJoinStamp;
    return;
}

bool EntityBigMama::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    pRoleInfoCache->m_bigMamaRoleInfo.m_nPrizeStamp = m_stampLastPrize;
    pRoleInfoCache->m_bigMamaRoleInfo.m_nJoinStamp = m_stampLastJoin;

    return true;
}


void EntityBigMama::OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaInfoNotifyOnLogin *pMsg = (GameMsg_G2S_BigMamaInfoNotifyOnLogin *)pMsgBase;

    // 1��Ҫ��¼��Ϣ
    ConfigBrief().bOpened = pMsg->m_bOpened; // �Ƿ񿪷�
    if (pMsg->m_bOpened)
    {
        ConfigBrief().nStartTime = pMsg->m_nStartTime;  // ��¼��ʼʱ��ͽ���ʱ��
        ConfigBrief().nEndTime = pMsg->m_nEndTime;
    }

    // 2�����¸�������
    UpdateBigMamaPersonalData(ConfigBrief().bOpened, !pMsg->m_property.m_bBigMamaAllLose, ConfigBrief().nStartTime, ConfigBrief().nEndTime);

    // 3�������Ѿ�ͬ��
    m_bReceiveGroupData = true; // �Ѿ����յ���ͬ������

    // 4���ظ��ͻ��˵�½��Ϣ
    GameMsg_S2C_SyncBigMamaConfig retMsg;
    do 
    {
        if (ConfigBrief().bOpened)
        {
            retMsg.m_nCurState = (pMsg->m_property.m_bBigMamaAllLose ? EBigMamaState_Success : EBigMamaState_InProgress);
        }
        else
        {
            retMsg.m_nCurState = EBigMamaState_NotOpen;
            break; // 
        }

        retMsg.m_nBeaterType = pMsg->m_property.m_nBeaterType;
        retMsg.m_strRoleName = pMsg->m_property.m_strRoleName;
        retMsg.m_nRewardState = (unsigned char)m_prizeState; // ����״̬
//         retMsg.m_nBeaterType = (EBigMamaLastBeatType)101;
//         retMsg.m_nCurState = (EBigMamaState)222;
//         retMsg.m_strRoleName = "yyy";
//         retMsg.m_nRewardState = 111; // ����״̬

    } while (0);

    SendMsgToPlayer(retMsg);

    return;
}

void EntityBigMama::OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup *pMsg = (GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup *)pMsgBase;

    // 1��Ҫ��¼��Ϣ
    ConfigBrief().bOpened = pMsg->m_bOpened; // �Ƿ񿪷�
    if (pMsg->m_bOpened)
    {
        ConfigBrief().nStartTime = pMsg->m_nStartTime;  // ��¼��ʼʱ��ͽ���ʱ��
        ConfigBrief().nEndTime = pMsg->m_nEndTime;
    }

    // 2�����¸�������
    UpdateBigMamaPersonalData(ConfigBrief().bOpened, !pMsg->m_property.m_bBigMamaAllLose, ConfigBrief().nStartTime, ConfigBrief().nEndTime);

    // 3�������Ѿ�ͬ��
    m_bReceiveGroupData = true; // �Ѿ����յ���ͬ������

    return;
}


// ���ر仯
void EntityBigMama::OnG2SNoticeBigMamaState(GameMsg_Base* pMsgBase)
{ 
    GameMsg_G2S_BigMamaStateNotify *pMsg = (GameMsg_G2S_BigMamaStateNotify*)pMsgBase;

    UpdateBigMamaPersonalData(ConfigBrief().bOpened, true, ConfigBrief().nStartTime, ConfigBrief().nEndTime);

    GameMsg_S2C_NoticeBigMamaState msg;
    msg.m_nState = pMsg->m_nState;
    SendMsgToPlayer(msg); 

    return;
}

// ��ǰ�ؿ��仯
void EntityBigMama::OnG2SBroadBigMamaInfo(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaSyncStageInfo *pMsg = (GameMsg_G2S_BigMamaSyncStageInfo *)pMsgBase;

    UpdateBigMamaPersonalData(true, !pMsg->m_property.m_bBigMamaAllLose, ConfigBrief().nStartTime, ConfigBrief().nEndTime);

    GameMsg_S2C_SyncBigMamaStageInfo retMsg; // �ظ�����Ϣ
    retMsg.m_bOpened = true;
    retMsg.m_property = pMsg->m_property;

    SendMsgToPlayer(retMsg);

    return;
}

// ֪ͨ���˹ؿ��仯
void EntityBigMama::OnG2SBigMamaInfoNotify(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaStageInfoNotify *pMsg = (GameMsg_G2S_BigMamaStageInfoNotify *)pMsgBase; // �����ߵ��������ʧ�ܣ�
    
    GameMsg_S2C_SyncBigMamaStageInfo retMsg; // �ظ�����Ϣ
    retMsg.m_bOpened = (0 == pMsg->m_result);
    retMsg.m_property = pMsg->m_property;

    SendMsgToPlayer(retMsg);

    return;
}

// ����ؿ�
void EntityBigMama::OnRequestBigMamaStageInfo(GameMsg_Base *pMsgBase)
{
    ENSURE_WITH_LOG_CMD(m_bReceiveGroupData, return, "role[%u] not get group data.", m_ptrAttributeComponent->GetRoleID());
//    GameMsg_C2S_RequestBigMamaStageInfo *pMsg = (GameMsg_C2S_RequestBigMamaStageInfo *)pMsgBase;
    if (!ConfigBrief().bOpened)
    {
        GameMsg_S2C_SyncBigMamaStageInfo retMsg;
        retMsg.m_bOpened = false;
        SendMsgToPlayer(retMsg);
        return; // ��û����
    }

    if (0 == m_ptrDanceGroupComponent->GetDanceGroupID())
        return; // ����˻�û������

    // �� group ��������
    GameMsg_S2G_BigMamaStageInfoRequest sendMsg;
    sendMsg.m_nDanceGroupID = m_ptrDanceGroupComponent->GetDanceGroupID();
    SendMsgToGroup(sendMsg);

    return;
}

// ��������
void EntityBigMama::OnRequestFightBigMama(GameMsg_Base *pMsgBase)
{
    ENSURE_WITH_LOG_CMD(m_bReceiveGroupData, return, "role[%u] not get group data.", m_ptrAttributeComponent->GetRoleID());
    GameMsg_C2S_RequestFightBigMama *pMsg = (GameMsg_C2S_RequestFightBigMama *)pMsgBase;
    

    EFightBigMamaResult ret = EFightBigMamaResult_Sucess;
    do 
    {
        if (!ConfigBrief().bOpened)
        {
            ret = EFightBigMamaResult_ActiveNotOpen;
            break;
        }

        if (m_pRoleEntity->InSomeRoom())
        {
            ret = EFightBigMamaResult_InOtherRoom;
            break;
        }

        if (m_ptrDanceGroupComponent->GetDanceGroupID() == 0)
        {
            ret = EFightBigMamaResult_NoDanceGroup;
            break;
        }

        CDanceGroupMember *pMember = m_ptrDanceGroupComponent->GetSelfMemberInfo(); // ȡ���Լ���memberInfo.
        if (NULL == pMember)
        {
            ret = EFightBigMamaResult_InternalError;
            break;
        }

        if (pMember->m_nProperty == EMemberProperty_MengXin)
        {
            ret = EFightBigMamaResult_NotMengXin;
            break;
        }


        GameMsg_S2G_BigMamaFightRequest request;
        request.m_nDanceGroupID = m_ptrDanceGroupComponent->GetDanceGroupID();
        request.m_nRoleID = m_ptrAttributeComponent->GetRoleID();
        request.m_nPhoneOS = pMsg->m_nPhoneOS;
        SendMsgToGroup(request);
        return;

    } while (0);

    if (ret != EFightBigMamaResult_Sucess)
    {
        GameMsg_S2C_RequestFightBigMamaRes retMsg;
        retMsg.m_nErrorType = ret;

        SendMsgToPlayer(retMsg);
    }

    return;
}

void EntityBigMama::OnG2SBigMamaFightResult(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaFightResult *pMsg = (GameMsg_G2S_BigMamaFightResult *)pMsgBase;

    GameMsg_S2C_RequestFightBigMamaRes retMsg;
    do 
    {
        if (EFightBigMamaResult_Sucess != pMsg->m_nResult)
        {
            retMsg.m_nErrorType = pMsg->m_nResult;
            break;
        }

        std::string roomName = "BigMamaRoom";
        BigMamaStageInfo *stageInfo = g_BigMamaConfigManager.GetStageInfo((unsigned short)pMsg->m_nStageID);
        ENSURE_WITH_LOG_CMD(NULL != stageInfo, break, "miss bigmama stage[%d] info", pMsg->m_nStageID);

        unsigned int roomID = BigMamaRoomManager::Instance().CreateAndEnterRoom(m_ptrAttributeComponent->GetRoleID(),
            roomName, stageInfo->m_nSceneID, stageInfo->m_nMusicID, stageInfo->m_nModeID, stageInfo->m_nLevel);
        std::string strCheckKey;
        std::string strStage;
        if (!BigMamaRoomManager::Instance().Prepare(roomID, pMsg->m_nPhoneOS, retMsg.m_strCheckKey, retMsg.m_szStage))
        {
            retMsg.m_nErrorType = EFightBigMamaResult_LoadStageFailed;
            break;
        }

        m_nRoomID = roomID;
        m_nTargetScore = stageInfo->m_nTargetScore;
        m_nBeatRound = pMsg->m_nRound; // 

        retMsg.m_roomInfo.m_nScene = stageInfo->m_nSceneID;
        retMsg.m_roomInfo.m_nMusic = (unsigned short)stageInfo->m_nMusicID;
        retMsg.m_roomInfo.m_nMusicMode = stageInfo->m_nModeID;
        retMsg.m_roomInfo.m_nMusicLevel = stageInfo->m_nLevel;

        BigMamaPlayerInfo hostInfo;
        m_ptrDanceGroupComponent->EncodePlayerInfo(hostInfo);
        retMsg.m_roomInfo.m_listDancer.push_back(hostInfo);

        // �ݲ����bossInfo
        BigMamaPlayerInfo bossInfo;
        if (m_ptrDanceGroupComponent->EncodeBossInfo(bossInfo, stageInfo->m_nNpcID))
        {
            bossInfo.m_chRolePos = (char)retMsg.m_roomInfo.m_listDancer.size();
            retMsg.m_roomInfo.m_listDancer.push_back(bossInfo);
        }

//         m_nChallengeSaveStorey = m_nChallengeCurStorey;
//         m_nChallengeSaveStartTime = m_nChallengeStartTime;

        // �����Լ��Ѿ��μ��ˣ�
        m_stampLastJoin = (unsigned int)time(NULL);
        m_bParticipant = true; // �μ���
        m_ptrDanceGroupComponent->AddQuery(QUERY_DanceGroup_UpdateOrInsertBigMamaRoleInfo, m_stampLastPrize, NULL, m_stampLastJoin);

        LogRoleBigMamaRecord(m_ptrAttributeComponent->GetRoleID(), m_ptrDanceGroupComponent->GetDanceGroupID(), pMsg->m_nRound);
    } while (0);

    SendMsgToPlayer(retMsg);
    return;
}

void EntityBigMama::OnReadyRoom()
{
    if (m_nRoomID == INVALID_ROOM_ID)
        return;

    BigMamaRoomManager::Instance().Start(m_nRoomID); // 

    return;
}


// ������ȡ
void EntityBigMama::OnRequestGetBigMamaReward(GameMsg_Base *pMsgBase)
{
    ENSURE_WITH_LOG_CMD(m_bReceiveGroupData, return, "role[%u] not get group data.", m_ptrAttributeComponent->GetRoleID());
//    GameMsg_C2S_RequestGetBigMamaReward *pMsg = (GameMsg_C2S_RequestGetBigMamaReward *)pMsgBase;

    EBigMamaRewardResult ret = EBigMamaRewardResult_Sucess;
    do 
    {
        if (!ConfigBrief().bOpened)
        {
            ret = EBigMamaRewardResult_ActiviteClosed;
            break;
        }

        if (m_prizeState == EBigMamaPrizeState_NoPrize)
        {
            ret = EBigMamaRewardResult_NoPrize;
            break;
        }

        if (m_prizeState == EBigMamaPrizeState_ReceivePrize)
        {
            ret = EBigMamaRewardResult_HasRecived;
            break;
        }

        CDanceGroupMember *pMember = m_ptrDanceGroupComponent->GetSelfMemberInfo(); // ȡ���Լ���memberInfo.
        if (NULL == pMember)
        {
            ret = EBigMamaRewardResult_InternalError;
            break;
        }

        if (pMember->m_nProperty == EMemberProperty_MengXin)
        {
            ret = EBigMamaRewardResult_NotMengXin;
            break;
        }

        GameMsg_S2G_BigMamaGetRewardRequest request;
        request.m_nDanceGroupID = m_ptrDanceGroupComponent->GetDanceGroupID();
        request.m_nRoleID = m_ptrAttributeComponent->GetRoleID();
        request.m_bJoined = m_bParticipant; 
        SendMsgToGroup(request);

        return;// 

    } while (0);
 
    if (ret != EBigMamaRewardResult_Sucess)
    { //
        GameMsg_S2C_RequestGetBigMamaRewardRes retMsg;
        retMsg.m_nErrorType = ret;
        retMsg.m_bIsGuideReward = false;
        SendMsgToPlayer(retMsg);
    }

    return;
}

void EntityBigMama::OnG2SBigMamaGetRewardResult(GameMsg_Base* pMsgBase)
{
    GameMsg_G2S_BigMamaGetRewardResult *pMsg = (GameMsg_G2S_BigMamaGetRewardResult *)pMsgBase;
    
    GameMsg_S2C_RequestGetBigMamaRewardRes retMsg;
    retMsg.m_nErrorType = EBigMamaRewardResult_Sucess;
    retMsg.m_bIsGuideReward = false;
    do
    {
        if (pMsg->m_nErrorType != EBigMamaRewardResult_Sucess)
        {
            retMsg.m_nErrorType = pMsg->m_nErrorType;
            break;
        }

        BigMamaReward *pReward = g_BigMamaConfigManager.GetBigMamaReward((EBigMamaRewardsType)pMsg->m_rewardType);
        ENSURE_WITH_LOG_CMD(NULL != pReward, return, "error to get bigmama reward[%d]", (int)pMsg->m_rewardType);

        BigMamaRewardToReward(m_ptrAttributeComponent->GetSex() == ESexType_Male, pReward, retMsg.m_RewardInfo);
        retMsg.m_nDanceGroupContribution = pReward->m_nDanceGroupContribution;

        m_prizeState = EBigMamaPrizeState_ReceivePrize; // ��Ϊ�Ѿ���ȡ������״̬��
        m_stampLastPrize = (unsigned int)time(NULL);
        m_ptrDanceGroupComponent->AddQuery(QUERY_DanceGroup_UpdateOrInsertBigMamaRoleInfo, m_stampLastPrize, 0, m_stampLastJoin);

        AddReward(retMsg.m_RewardInfo, pReward->m_nDanceGroupContribution);

    } while (0);

    SendMsgToPlayer(retMsg); // ���ͳ�ȥ�ˡ�

    return;
}


// ��ɴ�������
void EntityBigMama::OnCompleteBigMamaGuide(GameMsg_Base *pMsgBase)
{
    ENSURE_WITH_LOG_CMD(NULL != m_ptrAttributeComponent, return, "big mama miss attribute component ptr");

    if (m_ptrAttributeComponent->IsGuideFinish(EGuideBit_BigMamaComplete))
        return;
    
    m_ptrAttributeComponent->SetGuideFinish(EGuideBit_BigMamaComplete); // ������ɣ�Ȼ��ˢ�����ݿ�
//    g_pQueryMgr->AddQuery(QUERY_UpdateGuideDataEvent, GetRoleID(), NULL, m_guideData);
    m_ptrAttributeComponent->FlushGuidData(); // ���������д��
}

// ������轱��
void EntityBigMama::OnRequestBigMamaGuideReward(GameMsg_Base *pMsgBase)
{
    GameMsg_C2S_RequestBigMamaGuideReward *pMsg = (GameMsg_C2S_RequestBigMamaGuideReward *)pMsgBase;

    GameMsg_S2C_RequestGetBigMamaRewardRes retMsg;
    retMsg.m_bIsGuideReward = true;
    
    do 
    {
        if (m_ptrAttributeComponent->IsGuideFinish(EGuideBit_BigMamaRecivePrize))
        {
            retMsg.m_nErrorType = EBigMamaRewardResult_HasRecivedGuidePrize;
            break;
        }

        FillGuidePrize(m_ptrAttributeComponent->GetSex() == ESexType_Male, retMsg.m_RewardInfo);
        m_ptrAttributeComponent->SetGuideFinish(EGuideBit_BigMamaRecivePrize);
        m_ptrAttributeComponent->FlushGuidData();
        
        if (pMsg->m_nRewardType == 0)
            AddReward(retMsg.m_RewardInfo, 0);
        else
        {
            MailReward(retMsg.m_RewardInfo.m_listItems);
        }

        retMsg.m_nErrorType = EBigMamaRewardResult_Sucess;
    } while (0);

    SendMsgToPlayer(retMsg);

    return;
}

// ��ǰ�Ƿ������跿��
unsigned int EntityBigMama::BigMamaRoomID() const
{
    return m_nRoomID;
}

bool EntityBigMama::EndRoom()
{
    if (INVALID_ROOM_ID == m_nRoomID)
        return false;

    unsigned int score = 0;
    CRoomMarkInfo mark;
    bool bSuccess = BigMamaRoomManager::Instance().End(m_nRoomID, m_ptrAttributeComponent->GetRoleID(), score, mark);
//    score = 200000;
    m_nRoomID = INVALID_ROOM_ID;

    if (!bSuccess)
    {
        return true;
    }

    if (score >= m_nTargetScore)
    { // ���������Ϣ��
        GameMsg_S2G_BigMamaBeatSucess msg;
        msg.m_nDanceGroupID = m_ptrDanceGroupComponent->GetDanceGroupID();
        msg.m_nRoleID = m_ptrAttributeComponent->GetRoleID();
        msg.m_nRound = m_nBeatRound;
        SendMsgToGroup(msg); // 
    } // �ȴ�ȷ��

    GameMsg_S2C_EndRoomSuc msgResponse;
    const int nPos = 0;
    msgResponse.m_szDancerPlace[nPos] = 1;              // ������λ��0
    msgResponse.m_szDancerGrade[nPos] = (char)mark.Grade();
    msgResponse.m_szDancerMark[nPos] = mark.Mark();
    msgResponse.m_szDancerExp[nPos] = mark.Exp();
    msgResponse.m_szDancerPerfect[nPos] = mark.TotalRankCount(RoomRoundRank_Perfect);
    msgResponse.m_szDancerCool[nPos] = mark.TotalRankCount(RoomRoundRank_Cool);
    msgResponse.m_szDancerGood[nPos] = mark.TotalRankCount(RoomRoundRank_Good);
    msgResponse.m_szDancerBad[nPos] = mark.TotalRankCount(RoomRoundRank_Bad);
    msgResponse.m_szDancerMiss[nPos] = mark.TotalRankCount(RoomRoundRank_Miss);
    msgResponse.m_szDancerSpecial[nPos] = mark.MaxSpecialCount();
    SendMsgToPlayer(msgResponse);

    return bSuccess;
}

void EntityBigMama::LogRoleBigMamaRecord(unsigned int nRoleID, unsigned int nDanceGroupID, int nDanceRound)
{
    g_pQueryMgr->AddQuery(QUERY_LogRoleBigMamaDanceRecord, 0, new CRoleBigMamaDanceRecord(nRoleID, nDanceGroupID, nDanceRound));
}


void EntityBigMama::CheckGuidePrizeOnLogin()
{
    ENSURE_WITH_LOG_CMD(NULL != m_ptrAttributeComponent, return, "miss attr component on bigmama");

    if (m_ptrAttributeComponent->IsGuideFinish(EGuideBit_BigMamaComplete)
        && !m_ptrAttributeComponent->IsGuideFinish(EGuideBit_BigMamaRecivePrize))
    {
        CReward reward;
        FillGuidePrize(m_ptrAttributeComponent->GetSex() == ESexType_Male, reward);
        m_ptrAttributeComponent->SetGuideFinish(EGuideBit_BigMamaRecivePrize);
        m_ptrAttributeComponent->FlushGuidData();

        MailReward(reward.m_listItems);
    }
}


void EntityBigMama::UpdateBigMamaPersonalData(bool bOpend, bool bRunning, time_t nStartTime, time_t nEndTime)
{
    if (bOpend)
    {
        if (m_stampLastPrize < (unsigned int)nStartTime || m_stampLastPrize > (unsigned int)nEndTime)
        { // ��һ���콱�Ƿǻʱ��
            m_prizeState = (bRunning ? EBigMamaPrizeState_NoPrize : EBigMamaPrizeState_HasPrize);
        } 
        else
        {
            m_prizeState = EBigMamaPrizeState_ReceivePrize; // �Ѿ��콱��
        }

        m_bParticipant = (m_stampLastJoin >= (unsigned int)nStartTime && m_stampLastJoin <= (unsigned int)nEndTime); // ����״̬���ϵģ�ֱ������Ϊδ����״̬
    }
    else
    { // �δ������
        m_prizeState = EBigMamaPrizeState_NoPrize; // û�н���
        m_bParticipant = false; // δ����
    }

    return;
}

void EntityBigMama::SendMsgToPlayer(GameMsg_Base &msg)
{
    m_ptrDanceGroupComponent->SendPlayerMsg(msg);
    return;
}

void EntityBigMama::SendMsgToGroup(GameMsg_Base &msg)
{
    m_ptrDanceGroupComponent->Send2GroupMsg(msg);
    return;
}


void EntityBigMama::FillGuidePrize(bool isMale, CReward &reward)
{
    if (isMale)
        reward.m_listItems.assign(g_BigMamaConfigManager.GetGuideMaleItem().begin(), g_BigMamaConfigManager.GetGuideMaleItem().end());
    else
        reward.m_listItems.assign(g_BigMamaConfigManager.GetGuideFemaleItem().begin(), g_BigMamaConfigManager.GetGuideFemaleItem().end());

    return;
}

void EntityBigMama::AddReward(CReward &reward, unsigned int nDancegroupContribution)
{
//     std::list<CItem>    m_listItems;    // ��Ʒ
//     int                 m_nMoney;       // ��ȯ
//     int                 m_nBindMCoin;   // ��M��
    std::string strTitle = CLocalization::Instance().GetString("BigMamaBagNotEnoughMailTitle");
    std::string strContext = CLocalization::Instance().GetString("BigMamaBagNotEnoughMailContext");

    CItemProcess::AddOrMailItems(*m_pRoleEntity, reward.m_listItems, EItemAction_Add_BigMamaReward,
        0, true, EMailType_BigMamaReward, strTitle, strContext);

    if (0 != reward.m_nMoney)
    {
        m_ptrAttributeComponent->ChangeMoney(reward.m_nMoney, EChangeMoneyCause_Add_BigMama, 0);
    }
    if (0 != reward.m_nBindMCoin)
    {
        m_ptrAttributeComponent->ChangeBindBill(reward.m_nBindMCoin, EChangeBindBillCause_Add_BigMama);
    }
    if (0 != nDancegroupContribution)
    {
        m_ptrDanceGroupComponent->ChangeDanceGroupContribution(nDancegroupContribution);
    }

    return;
}

void EntityBigMama::MailReward(std::list<CItem> &items)
{
    std::string strTitle = CLocalization::Instance().GetString("BigMamaGuideMailTitle");
    std::string strContext = CLocalization::Instance().GetString("BigMamaGuideMailContext");

//    CItemProcess::MailItems(m_ptrAttributeComponent->GetRoleID(), items, EMailType_BigMamaReward, strTitle, strContext);
    CMailMgr::Instance().SendSystemMailToPlayer(m_ptrAttributeComponent->GetRoleID(), EMailType_BigMamaReward, strTitle, strContext, items);
    return; // �ʼ��·�����
}

void EntityBigMama::BigMamaRewardToReward(bool isMale, BigMamaReward* pReward, CReward &reward)
{
    if (isMale)
        reward.m_listItems.assign(pReward->m_vecMaleItem.begin(), pReward->m_vecMaleItem.end());
    else
        reward.m_listItems.assign(pReward->m_vecFemaleItem.begin(), pReward->m_vecFemaleItem.end());

    reward.m_nBindMCoin = pReward->m_nBindCoin;
    reward.m_nMoney = pReward->m_nMoney;

    return;
}

BigMamaConfigBrief& EntityBigMama::ConfigBrief()
{
    return EntityBigMamaManager::Instance().GetCfg();
}



//////////////////////////////////////////////////////////////////////////
// EntityBigMamaManager

EntityBigMamaManager::EntityBigMamaManager()
    : GameMsg_Processor(true, false)
{

}

EntityBigMamaManager::~EntityBigMamaManager()
{}

EntityBigMamaManager& EntityBigMamaManager::Instance()
{
    static EntityBigMamaManager s;
    return s;
}

void EntityBigMamaManager::Initialize()
{
    RegisterMessage();
}


void EntityBigMamaManager::RegisterMessage()
{
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaStateNotify, &EntityBigMamaManager::OnG2SNoticeBigMamaState);
}

void EntityBigMamaManager::OnG2SNoticeBigMamaState(GameMsg_Base &msgBase, CSlotPeer & SlotPeer)
{
    GameMsg_G2S_BigMamaStateNotify &rMsg = (GameMsg_G2S_BigMamaStateNotify&)msgBase;

    m_cfgBrief.bOpened = (rMsg.m_nState == 0 ? false : true);
    m_cfgBrief.nStartTime = rMsg.m_nStartTime;
    m_cfgBrief.nEndTime = rMsg.m_nEndTime;

    std::list<CRoleEntity*> lstRole;
    CPlayerManager::Instance().GetAllPlayers(lstRole);
    for (std::list<CRoleEntity*>::iterator iter = lstRole.begin(); iter != lstRole.end(); ++iter)
    {
        CRoleEntity *ent = *iter;
        if (NULL == ent)
            continue;

        CEntityDanceGroupComponent* pDance = ent->GetComponent<CRoleDanceGroup>();
        pDance->OnG2SNoticeBigMamaState(rMsg, SlotPeer);
    }

    return;
}

BigMamaConfigBrief& EntityBigMamaManager::GetCfg()
{
    return m_cfgBrief;
}
