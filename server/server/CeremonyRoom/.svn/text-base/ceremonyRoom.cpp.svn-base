#include "ceremonyRoom.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"
#include "GameMsg_CeremonyRoom.h"
#include "../dancegroup/DanceGroupMsgDef.h"
#include "../../datastructure/RoleEntity.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../logic/LogicCirculator.h"
#include "../vip/EntityVIPComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"
#include "../logic/LogicCirculator.h"
#include "../mall/CurrencyType.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../mall/MallDefine.h"
#include "../mail/MailMgr.h"
#include "../util/CommonFunc.h"

extern int g_nLine;
extern CLogicCirculator* g_pCirculator;

#define MISSED_NAME ("???")

CeremonyRoom::CeremonyRoom(unsigned int danceGroupID, CRoleEntity *pRole)
    : m_nGroupID(danceGroupID)
    , m_nCreater(pRole->GetRoleID())
    , m_crsState(ECeremonyState_Prepare)
    , m_nApplyTime((unsigned int)time(NULL))
    , m_nStartTime(0) 
{

    m_logRoomInfo.m_nCreatorID = m_nCreater;
    m_logRoomInfo.m_strCreatorName = pRole->GetRoleName();
    m_logRoomInfo.m_nCreatorVip = (unsigned char)pRole->GetVIPLevel();
    m_logRoomInfo.m_logTime = m_nApplyTime; // 

}

CeremonyRoom::~CeremonyRoom()
{

}

// 创建者操作
int CeremonyRoom::SetNewMember(unsigned int nRoleID, int seq, bool bNeedMengxin)
{
    // 验证房间状态:
    if (m_crsState != ECeremonyState_Prepare)
        return ECeremonyErrType_StateErr; // 

    // 在房间中
    CeremonyNormalMemberMap::iterator iter = m_mapNormalMember.find(nRoleID);
    if (iter == m_mapNormalMember.end())
        return ECeremonyErrType_StateErr;

    // 是小萌新
    CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return ECeremonyErrType_Internal, "error to get role[%u] ent.", nRoleID);
    CRoleDanceGroup *pGroup = pRole->GetComponent<CRoleDanceGroup>();
    ENSURE_WITH_LOG_CMD(NULL != pGroup, return ECeremonyErrType_Internal, "error to get role[%u] danceGroup ent.", nRoleID);
    CDanceGroupMember *pMember = pGroup->GetSelfMemberInfo(); // 取得自己的memberInfo.
    ENSURE_WITH_LOG_CMD(NULL != pMember, return ECeremonyErrType_Internal, "error to get role[%u] memberInfo.", nRoleID);

    if (bNeedMengxin && pMember->m_nProperty != EMemberProperty_MengXin)
        return ECeremonyErrType_NotMengxin;

    // 验证seq;
    if (seq < ECeremonyNewMemberSeq_Min || seq > ECeremonyNewMemberSeq_Max)
        return ECeremonyErrType_SequenceError;

    for (CeremonyNewMemberMap::iterator it = m_mapNewMember.begin(); it != m_mapNewMember.end(); ++it)
    {
        if (it->second.m_nSeq == seq)
            return ECeremonyErrType_SequenceRedumplicate;
    }

    // 直接添加这个新人的位置：
    FromNormalMemberToNewMember(nRoleID, seq);

    return ECeremonyErrType_OK;
}

int CeremonyRoom::CancelNewMember(unsigned int nRoleID)
{
    // 验证房间状态:
    if (m_crsState != ECeremonyState_Prepare)
        return ECeremonyErrType_StateErr; // 

    // 查证存在此人
    if (m_mapNewMember.find(nRoleID) == m_mapNewMember.end())
        return ECeremonyErrType_StateErr;

    //换位置
    FromNewMemberToNormalMember(nRoleID);

    return ECeremonyErrType_OK;
}

int CeremonyRoom::KickPerson(unsigned int nRoleID)
{
    if (m_crsState != ECeremonyState_Prepare)
        return ECeremonyErrType_StateErr; // 

    if (m_mapNewMember.find(nRoleID) != m_mapNewMember.end())
        m_mapNewMember.erase(nRoleID);

    if (m_mapNormalMember.find(nRoleID) != m_mapNormalMember.end())
        m_mapNormalMember.erase(nRoleID);

    return ECeremonyErrType_OK; // 直接成功
}

int CeremonyRoom::Play(std::vector<CeremonyRookieInfo> &vecRookieInfo)
{
    if (m_crsState != ECeremonyState_Prepare)
        return ECeremonyErrType_StateErr; // 

    // 检测是否存在指定新人
    if (m_mapNewMember.empty())
        return ECeremonyErrType_NoNewMan;

    if (m_mapNormalMember.find(m_nCreater) == m_mapNormalMember.end())
        return ECeremonyErrType_CreatorNotInRoom;

    m_nStartTime = (unsigned int)time(NULL);

    m_crsState = ECeremonyState_Start; // 开启了。 
    // 状态发生改变，通知到group, group 再广播到所有的线
    GameMsg_S2G_CeremonyRoomStateChange brod;
    brod.m_msg.m_nDanceGroupID = m_nGroupID;
    brod.m_msg.m_nNewState = m_crsState;
    SendMsg2GroupServer(&brod);
    // 新人属性发生改变：
    GameMsg_S2G_CeremonyRoomNewMember newmemberNotify;
    newmemberNotify.m_nGroupID = m_nGroupID;
    for (CeremonyNewMemberMap::iterator iter = m_mapNewMember.begin(); iter != m_mapNewMember.end(); ++iter)
    {
        newmemberNotify.m_vecNewMember.push_back(iter->second.m_nRoleID); // 
    }
    SendMsg2GroupServer(&newmemberNotify); //

    // 记录下来所有人的信息用于后续日志：
    for (CeremonyNormalMemberMap::iterator iter = m_mapNormalMember.begin(); iter != m_mapNormalMember.end(); ++iter)
    {
        CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(iter->second.m_nRoleID);
        ENSURE_WITH_LOG_CMD(NULL != pRole, continue, "error to get role[%u] entity info on start ceremony", iter->second.m_nRoleID);
        CeremonyMemberLogInfo info;
        info.m_nRoleID = iter->second.m_nRoleID;
        info.m_strName = pRole->GetRoleName(); // 丢失了这个名字
        info.m_nVipLev = (unsigned char)pRole->GetVIPLevel();
        m_mapMemberLogInfo.insert(std::make_pair(info.m_nRoleID, info));
    }


    // 记录下来所有新人的信息
    // fill newer info.
    for (CeremonyNewMemberMap::iterator iter = m_mapNewMember.begin(); iter != m_mapNewMember.end(); ++iter)
    {
        CeremonyMemberLogInfo *voterLogInfo = GetMemberLogInfo(iter->first);
        iter->second.m_logs.m_newerInfo.m_nRoleID = iter->first;
        iter->second.m_logs.m_newerInfo.m_nVipLv = (NULL != voterLogInfo ? voterLogInfo->m_nVipLev : 0);
        iter->second.m_logs.m_newerInfo.m_strRoleName = (NULL != voterLogInfo ? voterLogInfo->m_strName : MISSED_NAME);

        CeremonyRookieInfo rookieInfo;
        rookieInfo.m_nRoleID = iter->first;
        rookieInfo.m_nRookieIndex = iter->second.m_nSeq;
        rookieInfo.m_strIntruduction =  GetIntroduce(iter->first);
        vecRookieInfo.push_back(rookieInfo);
    }


    // 保存这些人，用来做点赞处理
    m_mapNormalMemberAtStart.clear();
    m_mapNewMemberAtStart.clear();
    m_mapNormalMemberAtStart.insert(m_mapNormalMember.begin(), m_mapNormalMember.end());
    m_mapNewMemberAtStart.insert(m_mapNewMember.begin(), m_mapNewMember.end());

    return ECeremonyErrType_OK;
}

// 
int CeremonyRoom::EnterRoom(unsigned int nRoleID, CeremonyRoomInfo& roomInfo, CeremonyPlayerShowInfo &selfInfo)
{
    // 进入者是创建者或者其他人：
    if (!HasMember(nRoleID))
    { 
        CeremonyNormalMember normalMember;
        normalMember.m_nRoleID = nRoleID;
        m_mapNormalMember.insert(std::make_pair(nRoleID, normalMember));        
    }
    
// 
//     CRoleEntity *pRoleEnt = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
//     ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, return false, "error to get role[%u] ent", nRoleID);
//     CPlayerInfo *pRoleInfo = pRoleEnt->GetComponent<CPlayerInfo>();
//     pRoleInfo->m_position = pos;
//     pRoleInfo->m_position.m_ESceneID = ESceneMove_DanceGroupCeremonyRoom; // 进入了

    // 填充房间信息和进入者的信息
    GetRoomInfo(roomInfo);
    GetRoomPlayerShowInfo(nRoleID, selfInfo);

    return ECeremonyErrType_OK; 
}

int CeremonyRoom::LeaveRoom(unsigned int nRoleID)
{
    if (!HasMember(nRoleID))
        return ECeremonyErrType_NotInRoom;

    // 依次的删除这些信息
    if (IsNewMember(nRoleID))
    {
        m_mapNewMember.erase(nRoleID);
    }
    
    if (IsNormalMember(nRoleID))
    {
        m_mapNormalMember.erase(nRoleID);
    }
    
    if (IsCreator(nRoleID))
    {
        // do nothing.
    }
//     else
//     {
//         return ECeremonyErrType_NotInRoom; 
//     }

    CRoleEntity * pEntity = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
    if (NULL != pEntity)
    {
        CEntityHorseComponent *pHorseCOM = pEntity->GetComponent<CRoleHorse>();
        if (NULL != pHorseCOM)
        {
            pHorseCOM->LeaveRoomHorseTask();
        }
    }

    return ECeremonyErrType_OK;
}

void CeremonyRoom::CloseRoom()
{
    for (std::map<unsigned int, CeremonyNormalMember>::iterator it = m_mapNormalMember.begin();
        it != m_mapNormalMember.end(); ++it)
    {
        CRoleEntity * pEntity = CPlayerManager::Instance().GetEntityByRoleID(it->first);
        if (NULL != pEntity)
        {
            CEntityHorseComponent *pHorseCOM = pEntity->GetComponent<CRoleHorse>();
            if (NULL != pHorseCOM)
            {
                pHorseCOM->LeaveRoomHorseTask();
            }
        }
    }
}

int CeremonyRoom::SaveIntroduce(unsigned int nRoleID, std::string strIntroduce)
{
    SetIntroduct(nRoleID, strIntroduce);
    return ECeremonyErrType_OK;
}

unsigned char CeremonyRoom::APraiseB(unsigned int nRoleA, unsigned int nRoleNewer, unsigned char nNewerIndex, int count, int &nNewCount)
{
    if (m_crsState != ECeremonyState_Start)
        return ECeremonyErrType_StateErr;

    CRoleEntity *pRoleEnt = CPlayerManager::Instance().GetEntityByRoleID(nRoleA);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, return ECeremonyErrType_Internal, "error to get role[%u] entity", nRoleA);

    CRoleAttribute *pRoleAttr = pRoleEnt->GetComponent<CRoleAttribute>();
    ENSURE_WITH_LOG_CMD(NULL != pRoleAttr, return ECeremonyErrType_Internal, "error to get role[%u] attr ent", nRoleA);

    // A 是否在房间中
    CeremonyNormalMember *pMemberA = GetNormalMemberAtStart(nRoleA);
    ENSURE_WITH_LOG_CMD(NULL != pMemberA, return ECeremonyErrType_NotInRoom, "normal[%u] praise newer[%u] miss normal", nRoleA, nRoleNewer);

    // b是否在房间中
    CeremonyNewMember *pMemberNewer = GetNewerMemberAtStart(nRoleNewer);
    ENSURE_WITH_LOG_CMD(NULL != pMemberNewer, return ECeremonyErrType_NotNewMan, "normal[%u] praise newer[%u] miss newer", nRoleA, nRoleNewer);
    if (nNewerIndex != pMemberNewer->m_nSeq)
        return ECeremonyErrType_SequenceErr;

    // 扣钱结束
    int uNeedMoney = 0;

    int lastFreeCount = 0;
    if (pMemberA->m_nPraiseNum < CDanceGroupDataMgr::Instance().m_nCeremonyFreePraiseTimes)
        lastFreeCount = CDanceGroupDataMgr::Instance().m_nCeremonyFreePraiseTimes - pMemberA->m_nPraiseNum;
    if (lastFreeCount < count)
    {
        uNeedMoney = (count - lastFreeCount) * CDanceGroupDataMgr::Instance().m_nCeremonyPraiseCostNum;
    }

    if (uNeedMoney > 0)
    {
        BUILD_CURRENCY_MAP_ONETYPE(currencyMap, CDanceGroupDataMgr::Instance().m_nCeremonyPraiseCostType, uNeedMoney);
        if (pRoleAttr->CanBill(currencyMap))
            pRoleAttr->Bill(currencyMap, EChangeBillCause_CeremonyParise, EChangeBindBillCause_CeremonyParise, EChangeMoneyCause_CeremonyParise);
        else
            return ECeremonyErrType_ResNotEnough;
    }

    // 更新点赞次数
    pMemberA->m_nPraiseNum += (unsigned short)count; // 
    pMemberNewer->m_nPraisedNum += (unsigned short)count;
    nNewCount = pMemberNewer->m_nPraisedNum; // 


    // 日志记录
    CeremonyLogVoterInfo voterInfo;
    CeremonyMemberLogInfo *voterLogInfo = GetMemberLogInfo(nRoleA);
    if (NULL != voterLogInfo)
        voterInfo.Set(voterLogInfo->m_nRoleID, voterLogInfo->m_strName, voterLogInfo->m_nVipLev, (unsigned short)count, (unsigned int)time(NULL));
    else
        voterInfo.Set(pMemberA->m_nRoleID, MISSED_NAME, 0, (unsigned short)count, (unsigned int)time(NULL));

    pMemberNewer->m_logs.AddVoterInfo(voterInfo);

    return ECeremonyErrType_OK;
}



void CeremonyRoom::GetRoomInfo(CeremonyRoomInfo& roomInfo)
{
    roomInfo.m_nCeremonyState = m_crsState; // 仪式状态
    if (ECeremonyState_Prepare == m_crsState)
    {
        unsigned int now = (unsigned int)time(NULL);
        unsigned int escapeTime = 0;
        if (now > m_nApplyTime)
            escapeTime = now - m_nApplyTime;
        escapeTime = escapeTime; // 转化为
        roomInfo.m_nCeremontyCD = 0;
        if (CDanceGroupDataMgr::Instance().m_nCeremonyApplyDuration > escapeTime)
            roomInfo.m_nCeremontyCD = CDanceGroupDataMgr::Instance().m_nCeremonyApplyDuration - escapeTime;
    }

    std::vector<unsigned int> allPlayers;
    AllMember(allPlayers);

    for (size_t i = 0; i < allPlayers.size(); ++i)
    {
        CeremonyPlayerShowInfo cpsi;
        if (GetRoomPlayerShowInfo(allPlayers[i], cpsi))
        {
            roomInfo.m_vecCeremonyPlayers.push_back(cpsi);
        }
    }

    return ;
}

bool CeremonyRoom::GetRoomPlayerShowInfo(unsigned int nRoleID, CeremonyPlayerShowInfo &info)
{
    if (IsCreator(nRoleID))
    {
        info.m_nPlayerType = ECeremonyPlayerType_Applicant;
    }
    else if (IsNewMember(nRoleID))
    {
        info.m_nPlayerType = ECeremonyPlayerType_Rookie;
        CeremonyNewMemberMap::iterator iter = m_mapNewMember.find(nRoleID);
        info.m_nRookieIndex = iter->second.m_nSeq;    
    }
    else if (IsNormalMember(nRoleID))
    {
        info.m_nPlayerType = ECeremonyPlayerType_None;
    }
    else
    {
        ENSURE_WITH_LOG_CMD(false, return false, "error role[%u] to get player show info..", nRoleID);
    }
    
    CRoleEntity *pRoleEnt = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, return false, "error to get role[%u] ent", nRoleID);
    CRoleAttribute *pRoleAttr = pRoleEnt->GetComponent<CRoleAttribute>();
    ENSURE_WITH_LOG_CMD(NULL != pRoleAttr, return false, "error to get role[%u] attr component", nRoleID);
    CRoleVIP *pRoleVip = pRoleEnt->GetComponent<CRoleVIP>();
    ENSURE_WITH_LOG_CMD(NULL != pRoleVip, return false, "error to get role[%u] vip component", nRoleID);
    CPlayerInfo *pRoleInfo = pRoleEnt->GetComponent<CPlayerInfo>();
    ENSURE_WITH_LOG_CMD(NULL != pRoleInfo, return false, "error to get role[%u] info component", nRoleID);
    CRoleDanceGroup *pRoleDance = pRoleEnt->GetComponent<CRoleDanceGroup>();
    if (NULL != pRoleDance)
    {
        CDanceGroupMember* pMember = pRoleDance->GetSelfMemberInfo();
        CDanceGroupInfo* pDanceGroupInfo = pRoleDance->GetDanceGroupInfo();
        ENSURE_WITH_LOG_CMD(NULL != pMember && NULL != pDanceGroupInfo, return false, "erro to get role dancegroup self member info.", nRoleID);
        /// 玩家舞团职位-团长 副团长 团务 正式成员 见习成员
        info.m_nDanceGroupPos = pMember->m_nTitle;
        /// 玩家舞团属性-创始人 老团员 小萌新
        info.m_nDanceGroupProperty = pMember->m_nProperty;
        info.m_nDanceGroupBadgeId = pDanceGroupInfo->m_DGBaseInfo.m_nBadge;
        info.m_nDanceGroupEffectId = pDanceGroupInfo->m_DGBaseInfo.m_nEffect;
        info.m_strDanceGroupName = pDanceGroupInfo->m_DGBaseInfo.m_strGroupName;
    }

    info.m_nPlayerID = pRoleEnt->GetRoleID();
    info.m_strPlayerName = pRoleAttr->GetRoleName();
    info.m_nPlayerSex = pRoleAttr->GetSex();
    info.m_bIsVIP = pRoleVip->IsVIP();
    info.m_nVIPLevel = (unsigned short)pRoleVip->VIPLevel();

    /// 玩家移动类型-行走 飞行
    info.m_nMoveType = pRoleInfo->m_position.m_nMoveType;
    /// 玩家朝向
    info.m_fOrient = pRoleInfo->m_position.m_fOrient;
    /// 玩家坐标x
    info.m_fPosX = pRoleInfo->m_position.m_fPosX;
    /// 玩家坐标y
    info.m_fPosY = pRoleInfo->m_position.m_fPosY;
    /// 玩家坐标z
    info.m_fPosZ = pRoleInfo->m_position.m_fPosZ;
    /// 玩家肤色
    info.m_nPlayerSkin = pRoleAttr->GetColor();
    /// 玩家变身形象ID
    info.m_nTransformId = (unsigned short)pRoleAttr->GetTransformId();
    
    info.m_pItemCOM = pRoleEnt->GetComponent<CRoleItem>();
    info.m_pGeneCOM = pRoleEnt->GetComponent<CRoleGene>();
    info.m_pPetCOM = pRoleEnt->GetComponent<CRolePet>();
    info.m_pMedalCOM = pRoleEnt->GetComponent<CRoleMedal>();
    info.m_pHorseCOM = pRoleEnt->GetComponent<CRoleHorse>();
    return true;
}

void CeremonyRoom::RoomBroadcastExcept(unsigned int nRoleID, GameMsg_Base &msg)
{
    std::vector<unsigned int> vecAllMember;
    AllMember(vecAllMember);

    for (size_t i = 0; i < vecAllMember.size(); ++i)
    {
        if (vecAllMember[i] == nRoleID)
            continue;

        CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(vecAllMember[i]);
        if (NULL == pRole)
        {
            continue; 
        }

        pRole->SendPlayerMsg(&msg); // 广播这个消息
    }
}
// 广播所有人
void CeremonyRoom::RoomBroadcast(GameMsg_Base &msg)
{
    RoomBroadcastExcept(0, msg); 
    return ;
}

unsigned int CeremonyRoom::GetCreator()
{
    return m_nCreater;
}

unsigned int CeremonyRoom::GetApplyTime()
{
    return m_nApplyTime;
}
unsigned int CeremonyRoom::GetStartTime()
{
    return m_nStartTime;
}
ECeremonyState CeremonyRoom::GetState()
{
    return m_crsState;
}

void CeremonyRoom::ComitRoomLog()
{
    static unsigned int logSeq = 0;

    GameMsg_S2G_CeremonyRoomLogsStart startMsg;
    startMsg.m_nLine = g_nLine;
    startMsg.m_nLogSeq = logSeq;
    startMsg.m_nGroupID = m_nGroupID; //
    startMsg.m_roomInfo = m_logRoomInfo;
    SendMsg2GroupServer(&startMsg);

    for (CeremonyNewMemberMap::iterator iter = m_mapNewMemberAtStart.begin(); iter != m_mapNewMemberAtStart.end(); ++iter)
    { // 记录每一个人的信息
        GameMsg_S2G_CeremonyRoomLogsItem itemMsg;
        itemMsg.m_nLine = g_nLine;
        itemMsg.m_nLogSeq = logSeq;
        itemMsg.m_logItem = iter->second.m_logs;

        SendMsg2GroupServer(&itemMsg);
    }


    GameMsg_S2G_CeremonyRoomLogsEnd endMsg;
    endMsg.m_nLine = g_nLine;
    endMsg.m_nLogSeq = logSeq;
    SendMsg2GroupServer(&endMsg);

    logSeq++;

    return ;
}

bool PraiseSort::Great(const PraiseSort& s1, const PraiseSort &s2)
{
    if (s1.nPraise > s2.nPraise)
        return true;
    if (s1.nPraise == s2.nPraise)
        return s1.nSeq < s2.nSeq;

    return false;
}


void CeremonyRoom::MailRoomPrize()
{
    if (m_mapNewMemberAtStart.empty())
        return ;

    // 发送新人奖励
    std::vector<PraiseSort> rankRoles;
    for (CeremonyNewMemberMap::iterator iter = m_mapNewMemberAtStart.begin(); iter != m_mapNewMemberAtStart.end(); ++iter)
    {
        PraiseSort p;
        p.nRoleID = iter->second.m_nRoleID;
        p.nPraise = iter->second.m_nPraisedNum;
        p.nSeq = iter->second.m_nSeq;
        rankRoles.push_back(p);
    }
    std::sort(rankRoles.begin(), rankRoles.end(), PraiseSort::Great);

    for (size_t i = 0; i < rankRoles.size(); ++i)
    {
        CeremonyNewerPrize* prize = CDanceGroupDataMgr::Instance().GetCeremonyNewerPrize(i);
        ENSURE_WITH_LOG_CMD(NULL != prize, continue, "error to get seq[%d] prize", i);

        std::list< CItem > itemList;
        TransferItemList(prize->m_strItem.c_str(), itemList);
        CMailMgr::Instance().SendMailToPlayer( rankRoles[i].nRoleID, EMailType_CeremonyNewer,
            prize->m_strSender, prize->m_strMailTitle, prize->m_strContext, itemList );
    }

    // 发送参与者奖励， 根据发起点赞的总人数发送奖励
    do 
    {
        unsigned int nSumPraiser = 0; // 发起点赞的总人数：
        for (CeremonyNormalMemberMap::iterator iter = m_mapNormalMemberAtStart.begin(); iter != m_mapNormalMemberAtStart.end(); ++iter)
        {
            if (iter->second.m_nPraiseNum > 0)
                nSumPraiser ++;
        }

        CeremonyNormalPrize *normalPrize = CDanceGroupDataMgr::Instance().GetCeremonyNormalPrize(nSumPraiser);
        ENSURE_WITH_LOG_CMD(NULL != normalPrize, break, "error to get ceremony normal prize of num:[%u]", nSumPraiser);

        for (CeremonyNormalMemberMap::iterator iter = m_mapNormalMemberAtStart.begin(); iter != m_mapNormalMemberAtStart.end(); ++iter)
        {
            if (0 == iter->second.m_nPraiseNum)
                continue; // 没点赞的人不拿奖励

            if (IsNewMemberAtStart(iter->second.m_nRoleID))
                continue; // 新人不拿这个奖励

            std::list< CItem > itemList;
            TransferItemList(normalPrize->m_strItem.c_str(), itemList);
            CMailMgr::Instance().SendMailToPlayer( iter->second.m_nRoleID, EMailType_CeremonyNormalMember,
                normalPrize->m_strSender, normalPrize->m_strMailTitle, normalPrize->m_strContext, itemList );
        }
        
    } while (0);

    return ;
}



bool CeremonyRoom::HasMember(unsigned int nRoleID)
{
    return IsNormalMember(nRoleID); // 包含所有人
}


void CeremonyRoom::FromNormalMemberToNewMember(unsigned int nRoleID, unsigned seq)
{
    CeremonyNewMember newMember;
    newMember.m_nRoleID = nRoleID;
    newMember.m_nSeq = (unsigned char)seq;

    m_mapNewMember.insert(std::make_pair(nRoleID, newMember)); // 调整了这个新人的位置

    return ;
}

void CeremonyRoom::FromNewMemberToNormalMember(unsigned int nRoleID)
{
    m_mapNewMember.erase(nRoleID);

    return ;
}



bool CeremonyRoom::IsCreator(unsigned nRoleID)
{
    return m_nCreater == nRoleID;
}

bool CeremonyRoom::IsNewMember(unsigned nRoleID)
{
    return m_mapNewMember.find(nRoleID) != m_mapNewMember.end();
}

bool CeremonyRoom::IsNormalMember(unsigned nRoleID)
{
    return m_mapNormalMember.find(nRoleID) != m_mapNormalMember.end();
}

bool CeremonyRoom::IsNewMemberAtStart(unsigned nRoleID)
{
    return m_mapNewMemberAtStart.find(nRoleID) != m_mapNewMemberAtStart.end();
}

bool CeremonyRoom::IsNormalMemberAtStart(unsigned nRoleID)
{
    return m_mapNormalMemberAtStart.find(nRoleID) != m_mapNormalMemberAtStart.end();
}

bool CeremonyRoom::IsRoomStart()
{
    return m_crsState == ECeremonyState_Start; // 如果已经开始，
}


void CeremonyRoom::AllMember(std::vector<unsigned int> &allMember)
{
    for (CeremonyNormalMemberMap::iterator iter = m_mapNormalMember.begin(); iter != m_mapNormalMember.end(); ++iter)
        allMember.push_back(iter->second.m_nRoleID);

    return ;
}



CeremonyNormalMember* CeremonyRoom::GetNormalMember(unsigned int nRoleID)
{
    CeremonyNormalMemberMap::iterator iter = m_mapNormalMember.find(nRoleID);
    if (m_mapNormalMember.end() != iter)
        return &(iter->second);

    return NULL;
}

CeremonyNewMember* CeremonyRoom::GetNewerMember(unsigned int nRoleID)
{
    CeremonyNewMemberMap::iterator iter = m_mapNewMember.find(nRoleID);
    if (m_mapNewMember.end() != iter)
        return &(iter->second);

    return NULL;
}

CeremonyNormalMember* CeremonyRoom::GetNormalMemberAtStart(unsigned int nRoleID)
{
    CeremonyNormalMemberMap::iterator iter = m_mapNormalMemberAtStart.find(nRoleID);
    if (m_mapNormalMemberAtStart.end() != iter)
        return &(iter->second);

    return NULL;
}

CeremonyNewMember* CeremonyRoom::GetNewerMemberAtStart(unsigned int nRoleID)
{
    CeremonyNewMemberMap::iterator iter = m_mapNewMemberAtStart.find(nRoleID);
    if (m_mapNewMemberAtStart.end() != iter)
        return &(iter->second);

    return NULL;
}


CeremonyMemberLogInfo *CeremonyRoom::GetMemberLogInfo(unsigned int nRoleID)
{
    CeremonyMemberLogInfoMap::iterator iter = m_mapMemberLogInfo.find(nRoleID);
    if (m_mapMemberLogInfo.end() != iter)
        return &(iter->second);

    return NULL;
}

std::string CeremonyRoom::GetIntroduce(unsigned int roleID)
{
    StringMap::iterator iter = m_mapIntroduce.find(roleID);
    if (iter == m_mapIntroduce.end())
        return "";

    return iter->second;
}

void CeremonyRoom::SetIntroduct(unsigned int roleID, std::string strIntroduce)
{
    m_mapIntroduce[roleID] = strIntroduce;

    return ;
}



