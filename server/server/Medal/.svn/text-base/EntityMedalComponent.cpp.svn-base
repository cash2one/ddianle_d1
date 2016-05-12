#include "EntityMedalComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../mall/EntityMallComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../../socket/GameMsg_Map.h"
#include "../logic/EntityComponentEventID.h"
#include "GameMsg_Medal.h"
#include "MedalCfgMgr.h"
#include "MedalConditionFactory.h"
#include "../Rank/GameMsg_Rank_SG.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../util/CommonFunc.h"
#include "../Rank/RankComponent.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../roomLib/GameMsgNewRoom.h"

CEntityMedalComponent::CEntityMedalComponent()
    : CEntityComponent(true,true)
    , m_pNetComponent( NULL )
    , m_pAttrComponent( NULL )
    , m_pItemComponent( NULL )
    , m_pRoomComponent( NULL )
    , m_pCoupleComponent( NULL )
    , m_pPhotoRoomComponent( NULL )
    , m_pAmuseRoomComponent( NULL )
    , m_pDanceGroupComponent( NULL )
    , m_pRankComponent( NULL )
    , m_nSceneID(0)
    , m_nMedalWearScore(0)
    , m_nMedalCumulativeScore(0)
    , m_nInterval(0)
{
    m_bIsOpen = CSystemSettingMgr::Instance().IsFunctionOpen( EFunctionSwitch_Medal );
    m_bIsRankOpen = CSystemSettingMgr::Instance().IsFunctionOpen((unsigned short)EFunctionSwitch_Rank);
}

CEntityMedalComponent::~CEntityMedalComponent()
{
    std::map<int, MedalUnitDB*>::iterator it = m_mapAllMedal.begin();
    for (; m_mapAllMedal.end() != it; ++it)
    {
        delete it->second;
    }
    m_mapAllMedal.clear();


    std::map<int, IMedalCondition*>::iterator condit = m_mapAllCondition.begin();
    for (; m_mapAllCondition.end() != condit; ++condit)
    {
        delete condit->second;
    }
    m_mapAllCondition.clear();
}

void CEntityMedalComponent::Start()
{
    m_pNetComponent = GetComponent<CRoleNet>();
    m_pAttrComponent = GetComponent<CRoleAttribute>();
    m_pItemComponent = GetComponent<CRoleItem>();
    m_pRoomComponent = GetComponent<CRoleRoom>();
    m_pCoupleComponent = GetComponent<CRoleCouple>();
    m_pPhotoRoomComponent = GetComponent<CRolePhotoRoom>();
    m_pAmuseRoomComponent = GetComponent<CRoleAmuseRoom>();
    m_pDanceGroupComponent = GetComponent<CRoleDanceGroup>();
    m_pRankComponent = GetComponent<CRoleRank>();

    m_pAttrComponent->AddSubscriber(this);
    m_pItemComponent->AddSubscriber(this);
    GetComponent<CRoleMall>()->AddSubscriber(this);
    GetComponent<CRoleVIP>()->AddSubscriber(this);
    
}

bool CEntityMedalComponent::CreateFromDB(ROLEINFO_DB* p_RoleInfoDB)
{
    //积分初始化
    m_nMedalWearScore = p_RoleInfoDB->m_nMedalWearScore;
    m_nMedalCumulativeScore = p_RoleInfoDB->m_nMedalCumulativeScore;

    //CMedalUnitDB Map 初始化
    for (std::map<int ,MedalUnitDB>::const_iterator itMedal = p_RoleInfoDB->m_mapMedalUnit.begin();
        itMedal != p_RoleInfoDB->m_mapMedalUnit.end(); ++itMedal)
    {
        MedalUnitDB* pMedal = new MedalUnitDB( itMedal->second );
        if (NULL != pMedal)
        {
            m_mapAllMedal[pMedal->getID()] = pMedal;
            InsertMedalIDToStateTypeList(pMedal->getID(), (EMedalState)pMedal->getCurState() );

            //show状态 同时要加到activity中
            if (pMedal->getCurState() == eMedalState_Show)
            {
                InsertMedalIDToStateTypeList(pMedal->getID(), eMedalState_Activated);
            }
        }
    }

    //IMedalCondition Map 初始化
    for (std::map<int ,MedalConditionDB>::const_iterator itCond = p_RoleInfoDB->m_mapMedalCondition.begin(); 
        itCond != p_RoleInfoDB->m_mapMedalCondition.end(); ++itCond)
    {
        /*
        特殊处理 Cond只能从数据库获得数据，并不能产生对应类型的process()
        重新生成 pRealCond
        */
        const MedalConditionDB &cond = itCond->second;

        int nCondType = CMedalCfgMgr::Instance().GetProgressType( cond.GetID() );
        IMedalCondition* pRealCond = CMedalConditionFactory::CreateMedalCondition( nCondType, Entity() );
        if ( NULL == pRealCond )
        {
            continue;
        }

        pRealCond->SetConditionID( cond.GetID() );
        pRealCond->SetCurProgress( cond.GetCurProgress() );
        pRealCond->SetAreadyReward( cond.GetAreadyReward() );

        m_mapAllCondition[pRealCond->GetConditionID()] = pRealCond;
        //InsertConditionIDToTypeList( pRealCond->GetConditionID(), nCondType );
    }

    if (m_nMedalWearScore == 0)
    {
        m_nMedalWearScore = CalculateWearMedalScore(eMedalState_Activated);
    }

    if (m_nMedalCumulativeScore == 0)
    {
        m_nMedalCumulativeScore = CalculateWearMedalScore(eMedalState_Activated);
        m_nMedalCumulativeScore += CalculateWearMedalScore(eMedalState_Invalid);
        m_nMedalCumulativeScore += CalculateWearMedalScore(eMedalState_Show);
    }

    return true;
}

bool CEntityMedalComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    //由于Gm工具不支持缓存 不启用
//     if ( pRoleInfoCache == NULL )
//         return false;
// 
//     for ( std::map<int, MedalUnitDB *>::const_iterator unitItr = m_mapAllMedal.begin();
//         unitItr != m_mapAllMedal.end(); ++unitItr )
//     {
//         if ( unitItr->second == NULL )
//             continue;
// 
//         pRoleInfoCache->m_mapMedalUnit.insert( std::make_pair( unitItr->first, *unitItr->second ) );
//     }
// 
//     for ( std::map<int, IMedalCondition *>::const_iterator condItr = m_mapAllCondition.begin();
//         condItr != m_mapAllCondition.end(); ++condItr )
//     {
//         const IMedalCondition *pCond = condItr->second;
//         if ( pCond == NULL )
//             continue;
// 
//         MedalConditionDB cond( pCond->GetConditionID(), pCond->GetCurProgress(), pCond->GetAreadyReward() );
//         pRoleInfoCache->m_mapMedalCondition.insert( std::make_pair( condItr->first, cond ) );
//     }
// 
//     pRoleInfoCache->m_nMedalWearScore = m_nMedalWearScore;
//     pRoleInfoCache->m_nMedalCumulativeScore= m_nMedalCumulativeScore;
    return true;
}


void CEntityMedalComponent::OnLogin()
{
    //初始化配置
    InitMedalConfig();

    for (std::map<int, IMedalCondition*>::iterator MedalConditIt = m_mapAllCondition.begin();
        MedalConditIt != m_mapAllCondition.end(); ++ MedalConditIt)
    {
        MedalConditIt->second->check();
    }

    //发送登陆同步消息
    GameMsg_S2C_MedalInit initMsg;
    std::map<int, list<int> >::iterator showIt = m_mapStateTypeMedal.find(eMedalState_Show);
    if (showIt != m_mapStateTypeMedal.end())
    {
        for (std::list<int>::iterator it = showIt->second.begin();
            it != showIt->second.end(); ++it)
        {
            GameMsg_S2C_MedalInit::perShowMedal perInfo;
            perInfo.m_MedalID = *it;
            CMedalCfgMgr::Instance().GetMedalIcon(perInfo.m_MedalID, perInfo.m_strIcon);

            initMsg.m_listPerShowMedal.push_back(perInfo);
        }
    }

    m_pNetComponent->SendPlayerMsg(&initMsg);

    GetMedalActivityInfo();
    OnLoginCheckPushMsg();
}

void CEntityMedalComponent::OnLogout()
{
    //MedalScoreDB *pMedalScore = new MedalScoreDB();
    //pMedalScore->m_nMedalCumulativeScore = m_nMedalCumulativeScore;

    ////m_nMedalWearScore = CalculateWearMedalScore(eMedalState_Activated);
    //pMedalScore->m_nMedalWearScore = m_nMedalWearScore;
    //AddQuery(QUERY_Medal_UpdateScore, m_pAttrComponent->GetRoleID(), pMedalScore);
}

void CEntityMedalComponent::OnNotify(IParam &param)
{
    std::string strType;
    if ( !param.GetParam( "medal", strType ) )
        return;

    for (std::map<int, IMedalCondition*>::iterator CondIt = m_mapAllCondition.begin();
        CondIt != m_mapAllCondition.end() ;++CondIt)
    {
        int nBeforeProgress = CondIt->second->GetCurProgress();
        CondIt->second->process(param);
        //比较进度 变更即存储
        if (nBeforeProgress != CondIt->second->GetCurProgress() )
        {
            MedalConditionDB *pCond = new MedalConditionDB( CondIt->second->GetConditionID(),
                CondIt->second->GetCurProgress(), CondIt->second->GetAreadyReward() );
            AddQuery( QUERY_Medal_UpdateProgress, m_pAttrComponent->GetRoleID(), pCond );
            CheckPushMsgTask( *(CondIt->second) );
        }
    }
}

void CEntityMedalComponent::RegComponentNetMsgMap()
{
    if (!m_bIsOpen)
    {
        return;
    }

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMedalInfo);
    GAMEMSG_REGISTERCREATOR(GameMSg_C2S_GetConditionInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MedalHandBook);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Medal_Exchange_Info);
    GAMEMSG_REGISTERCREATOR(GameMSg_C2S_Medal_Show);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Medal_Exchange);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Medal_ActivityInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Medal_GetConditionReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRankAllDataForMedal);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRankAllData);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMedalScore);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Medal_GetScoreDescribe);

    RegMsgProcessor(MSG_C2S_GET_MEDALINFO, &CEntityMedalComponent::OnGetMedalInfo);
    RegMsgProcessor(MSG_C2S_GET_CONDITIONINFO, &CEntityMedalComponent::OnGetConditionInfo);
    RegMsgProcessor(MSG_C2S_MEDALHANDBOOK, &CEntityMedalComponent::OnGetMedalHandBook);
    RegMsgProcessor(MSG_C2S_MEDAL_EXCHANGE_INFO, &CEntityMedalComponent::OnGetMedalExchangeInfo);
    RegMsgProcessor(MSG_C2S_MEDAL_SHOW, &CEntityMedalComponent::OnSetMedalShow);
    RegMsgProcessor(MSG_C2S_MEDAL_EXCHANGE, &CEntityMedalComponent::OnGetMedalExchange);
    RegMsgProcessor(MSG_C2S_MEDAL_ACT_INFO, &CEntityMedalComponent::OnGetMedalActivityInfo);
    RegMsgProcessor(MSG_C2S_MEDAL_CONDITION_REWARD, &CEntityMedalComponent::OnGetMedalConditionReward);
    RegMsgProcessor(MSG_G2S_Rank_GetAllRankDataResultForMedal, &CEntityMedalComponent::OnGroupRankDataResultForMedal);
    RegMsgProcessor(MSG_G2S_Rank_GetAllRankDataResult, &CEntityMedalComponent::OnGroupRankDataResult);
    RegMsgProcessor(MSG_C2S_GetMedalScore, &CEntityMedalComponent::OnGetMedalScore);
    RegMsgProcessor(MSG_C2S_MEDALSCORE_DESCRIBE, &CEntityMedalComponent::OnGetMedalScoreDescribe);

}

void CEntityMedalComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
    if (!m_bIsRankOpen)
    {
        return;
    }

    m_nInterval += nTimeElapsed;
    if (m_nInterval >= UPDATE_INTERVAL)
    {
        m_nInterval = 0;
    }
    else
    {
        return;
    }

    //发送给group收集排行榜
    GameMsg_S2G_GetRankAllData S2Gmsg;
    S2Gmsg.m_nRoleId = m_pAttrComponent->GetRoleID();
    S2Gmsg.m_GroupId = m_pDanceGroupComponent->GetDanceGroupID();
    S2Gmsg.m_IntimacyId = m_pCoupleComponent->PairID();
    m_pNetComponent->Send2GroupServer((GameMsg_Base *)&S2Gmsg);
}

void CEntityMedalComponent::GetSortMedal(std::list<int> &listMedalID)
{
    std::vector<std::pair<int, int> > sortList; // key: priority value: medal id

    // prepare to order
    for (std::list<int>::iterator it = listMedalID.begin(); listMedalID.end() != it; ++it)
    {
        int nMedalId = *it;

        std::pair<int, int> element;
        element.first = CMedalCfgMgr::Instance().GetMedalSortPriority(nMedalId);
        element.second = nMedalId;
        sortList.push_back(element);
    }

    // sort by priority
    std::sort(sortList.begin(), sortList.end());

    // prepare output
    listMedalID.clear();
    for (int i = (int)sortList.size() - 1; i >= 0; --i)
    {
        std::pair<int, int> &element = sortList[i];
        listMedalID.push_back(element.second);
    }
}

/************************************************************************/
/* FUNC                                                                     */
/************************************************************************/

int CEntityMedalComponent::CalculateWearMedalScore(int nState)
{
    int nScore = 0;
    std::map<int, list<int> >::iterator actIt = m_mapStateTypeMedal.find(nState);
    if (actIt != m_mapStateTypeMedal.end())
    {
        for (list<int>::iterator it = actIt->second.begin(); it != actIt->second.end(); ++it)
        {
            nScore += CMedalCfgMgr::Instance().GetMedalScore(*it);
        }
    }
    return nScore;
}

void CEntityMedalComponent::OnLoginCheckPushMsg()
{
    for (std::map<int, IMedalCondition*>::iterator CondIt = m_mapAllCondition.begin();
        CondIt != m_mapAllCondition.end(); ++CondIt)
    {
        CheckPushMsgTask(*(CondIt->second));
    }
}

void CEntityMedalComponent::InitMedalConfig()
{
    /*
    //根据配置添加 CMedalUnitDB
    std::list<int> medalIDList;
    CMedalCfgMgr::Instance().GetAllMedalIDList(medalIDList);
    for (std::list<int>::iterator itMedal = medalIDList.begin();
        itMedal != medalIDList.end(); ++itMedal)
    {
        int nMedalId = *itMedal;
        if (FindMedal(nMedalId) == NULL)
        {
            CMedalUnitDB *pUnit = new CMedalUnitDB();
            pUnit->setID(nMedalId);
            pUnit->setGetTime(0);
            pUnit->setCurState(eMedalState_Underway);

            m_mapAllMedal.insert(std::make_pair(nMedalId, pUnit));
            InsertMedalIDToStateTypeList(pUnit->getID(), eMedalState_Underway);

            CMedalUnitDB *pDBunit = new CMedalUnitDB(*pUnit);
            AddQuery(QUERY_Medal_CreateMedal, m_pAttrComponent->GetRoleID(), pDBunit);
        }
    }
    */

    //根据配置添加 IMedalCondition
    std::list<int> conditionIDList;
    CMedalCfgMgr::Instance().GetAllProgressIDList(conditionIDList);
    for (std::list<int>::iterator itCond = conditionIDList.begin();
        itCond != conditionIDList.end(); ++itCond)
    {
        int nConditionID = *itCond;
        if (FindCondition(nConditionID) == NULL)
        {
            int nConditionType = CMedalCfgMgr::Instance().GetProgressType(nConditionID);
            IMedalCondition *pCond = CMedalConditionFactory::CreateMedalCondition(nConditionType, Entity());
            if (NULL == pCond)
            {
                continue;
            }

            pCond->SetConditionID(nConditionID);
            pCond->SetCurProgress(0);
            pCond->SetAreadyReward(0);

            m_mapAllCondition.insert(std::make_pair(nConditionID, pCond));

            //InsertConditionIDToTypeList(nConditionID, nConditionType);
        }
    }

    //去除不在配置中的条件,这些条件可能存在数据库中。
    for (std::map<int, IMedalCondition*>::iterator it = m_mapAllCondition.begin();
        it != m_mapAllCondition.end();)
    {
        std::list<int>::iterator findIt = std::find(conditionIDList.begin(), conditionIDList.end(), it->first);
        if (findIt == conditionIDList.end())
        {
            if ( it->second != NULL )
            {
                MedalConditionDB *pCondDB = new MedalConditionDB( it->second->GetConditionID(),
                    it->second->GetCurProgress(), it->second->GetAreadyReward() );
                AddQuery( QUERY_Medal_DeleteProgress, m_pAttrComponent->GetRoleID(), pCondDB );
            }

            //DeleteConditionFormTypeList(it->first);
            delete it->second, it->second = NULL;
            m_mapAllCondition.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

MedalUnitDB* CEntityMedalComponent::FindMedal(int nMedalID)
{
    std::map<int, MedalUnitDB*>::iterator it = m_mapAllMedal.find(nMedalID);
    if (m_mapAllMedal.end() == it)
    {
        return NULL;
    }
    return it->second;
}

IMedalCondition* CEntityMedalComponent::FindCondition(int nConditionID)
{
    std::map<int, IMedalCondition*>::iterator it = m_mapAllCondition.find(nConditionID);
    if (m_mapAllCondition.end() == it)
    {
        return NULL;
    }
    return it->second;
}

bool CEntityMedalComponent::CheckMedalHaveGot(int nMedalID)
{
    MedalUnitDB* result = FindMedal(nMedalID);
    if (NULL == result)
    {
        return false;
    }

    if (result->getCurState() < eMedalState_Activated)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//void CEntityMedalComponent::DeleteConditionFormTypeList(int nConditionId)
//{
//    int nType = CMedalCfgMgr::Instance().GetProgressType(nConditionId);
//    std::map<int, std::list<int> >::iterator itType = m_mapConditionType.find(nType);
//    if (itType != m_mapConditionType.end())
//    {
//        itType->second.remove(nConditionId);
//    }
//}

bool CEntityMedalComponent::ExchangeMedal(int nMedalID)
{
    int nMedalMoneyId = CMedalCfgMgr::Instance().GetMedalMoneyID();
    int haveCount = m_pItemComponent->GetItemCount(nMedalMoneyId, false, false);
    int needCount = CMedalCfgMgr::Instance().GetNeedMedalMoney(nMedalID);
    if (needCount <= haveCount)
    {
        if (!m_pItemComponent->RemoveItemByType(nMedalMoneyId, needCount, EItemAction_Del_Exchange_Medal))
        {
            return false;
        }

        if (SetMedalState(nMedalID, eMedalState_Activated))
        {
            LogMedal(nMedalID, CMedalLog::EMedalActivated, CMedalLog::EMedalExchage);
            return true;
        }
    }
    return false;
}

void CEntityMedalComponent::InsertMedalIDToStateTypeList(int nMedalID, EMedalState destState)
{
    std::map<int, std::list<int> >::iterator it = m_mapStateTypeMedal.find(destState);
    if (it == m_mapStateTypeMedal.end() )
    {
        std::list<int> list;
        list.push_back(nMedalID);
        m_mapStateTypeMedal.insert(std::make_pair(destState, list));
    }
    else
    {
        it->second.push_back(nMedalID);
    }
}

//void CEntityMedalComponent::InsertConditionIDToTypeList(int nConditID, int nType)
//{
//    std::map<int, list<int> >::iterator it = m_mapConditionType.find(nType);
//    if (it == m_mapConditionType.end())
//    {
//        std::list<int> list;
//        list.push_back(nConditID);
//        m_mapConditionType.insert(std::make_pair(nType, list));
//    }
//    else
//    {
//        it->second.push_back(nConditID);
//    }
//}

void CEntityMedalComponent::DelMedalIDFormStateTypeList(int nMedalID, EMedalState State)
{
    std::map<int, std::list<int> >::iterator actit = m_mapStateTypeMedal.find(State);
    if (actit != m_mapStateTypeMedal.end())
    {
        actit->second.remove(nMedalID);
        if (State == eMedalState_Activated)
        {
            ChangeMedalScore(-CMedalCfgMgr::Instance().GetMedalScore(nMedalID), 0);
        }
    }
}
bool CEntityMedalComponent::SetMedalState(int MedalId, int nDestMedalState, bool isNeedBoardcast/* = true*/)
{
    MedalUnitDB *pUnit = FindMedal(MedalId);
    if (NULL == pUnit)
    {

        if ( !CMedalCfgMgr::Instance().IsConfigContainMedal(MedalId) )
        {
            return false;
        }

        pUnit = new MedalUnitDB();
        pUnit->setID(MedalId);
        pUnit->setGetTime(0);
        pUnit->setCurState(eMedalState_Underway);

        m_mapAllMedal.insert(std::make_pair(MedalId, pUnit));
        InsertMedalIDToStateTypeList(pUnit->getID(), eMedalState_Underway);
    }

    int nMedalCurState = pUnit->getCurState();
    if (nDestMedalState == nMedalCurState)
    {
        return true;
    }

    int nMedalCurType = CMedalCfgMgr::Instance().GetMedalType(MedalId);

    if (nMedalCurState == eMedalState_Underway 
        && nDestMedalState == eMedalState_Activated)
    {
        std::map<int, std::list<int> >::iterator it = m_mapStateTypeMedal.find(eMedalState_Activated);
        if (it != m_mapStateTypeMedal.end() )
        {
            for(std::list<int>::iterator subit = it->second.begin();
                subit != it->second.end(); ++subit)
            {
                if (CMedalCfgMgr::Instance().GetMedalType(*subit) == nMedalCurType)
                {
                    SetMedalFromActivatedToInvalid(*subit);
                    break;
                }
            }
        }

        SetMedalFromUnderWayToActivated(MedalId);
    }
    else if (nMedalCurState == eMedalState_Activated
        && nDestMedalState == eMedalState_Show)
    {
        SetMedalFromActivatedToShow(MedalId);
    }
    else if (nMedalCurState == eMedalState_Activated 
        && nDestMedalState == eMedalState_Invalid)
    {
        SetMedalFromActivatedToInvalid(MedalId);
    }
    else if (nMedalCurState == eMedalState_Invalid 
        && nDestMedalState == eMedalState_Activated)
    {
        std::map<int, std::list<int> >::iterator actit = m_mapStateTypeMedal.find(eMedalState_Activated);
        if (actit != m_mapStateTypeMedal.end() )
        {
            for(std::list<int>::iterator subit = actit->second.begin();
                subit != actit->second.end(); ++subit)
            {
                if (CMedalCfgMgr::Instance().GetMedalType(*subit) == nMedalCurType)
                {
                    SetMedalFromActivatedToInvalid(*subit);
                    break;
                }
            }
        }

        SetMedalFromInvalidToActivated(MedalId);
    }
    else if (nMedalCurState == eMedalState_Show 
        && nDestMedalState == eMedalState_Activated)
    {
        setMedalFromShowToActiviated(MedalId);
    }
    else if (nMedalCurState == eMedalState_Show
        && nDestMedalState == eMedalState_Invalid)
    {
        SetMedalFormShowToInvalid(MedalId);
    }
    else
    {
        return false;
    }
    //激活要广播
    if (nDestMedalState == eMedalState_Activated && isNeedBoardcast)
    {
        BroadcastShowMedal( MedalId );
    }
    return true;
}

bool CEntityMedalComponent::CmdAddMedal(int nMedalID)
{
    bool IsSuccess = SetMedalState(nMedalID, eMedalState_Activated);
    if (IsSuccess)
    {
        GetMedalActivityInfo();
    }
    return IsSuccess;
}

void CEntityMedalComponent::SetMedalFromUnderWayToActivated(int nMedalID)
{
    MedalUnitDB* unit = FindMedal(nMedalID);
    if (unit == NULL)
    {
        return;
    }
    unit->setCurState(eMedalState_Activated);
    time_t now = time(NULL);
    unit->setGetTime( (int)now );

    UpdateMedalToDB( unit );

    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Underway);
    InsertMedalIDToStateTypeList(nMedalID, eMedalState_Activated);

    int nAddScore = CMedalCfgMgr::Instance().GetMedalScore(nMedalID);
    ChangeMedalScore(nAddScore, nAddScore);

    GameMsg_S2C_GetMedalScore retmsg;
    retmsg.m_nScore = m_nMedalCumulativeScore;
    m_pNetComponent->SendPlayerMsg(&retmsg);
}
void CEntityMedalComponent::SetMedalFromActivatedToShow(int nMedalID)
{
    const int nMaxShowMedal = 3;
    std::map<int, std::list<int> >::iterator showit = m_mapStateTypeMedal.find(eMedalState_Show);
    if (showit != m_mapStateTypeMedal.end())
    {
        if ((int)showit->second.size() >= nMaxShowMedal)
        {
            return;
        }
    }

    MedalUnitDB* unit = FindMedal(nMedalID);
    if (NULL == unit)
    {
        return;
    }
    unit->setCurState(eMedalState_Show);

    UpdateMedalToDB( unit );
    InsertMedalIDToStateTypeList(nMedalID , eMedalState_Show);
}
void CEntityMedalComponent::SetMedalFromActivatedToInvalid(int nMedalID)
{
    MedalUnitDB* unit = FindMedal(nMedalID);
    if (NULL == unit)
    {
        return;
    }
    unit->setCurState(eMedalState_Invalid);

    UpdateMedalToDB( unit );
    LogMedal(nMedalID, CMedalLog::EMedalInvalid);

    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Activated);
    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Show);
    InsertMedalIDToStateTypeList(nMedalID, eMedalState_Invalid);
}
void CEntityMedalComponent::setMedalFromShowToActiviated(int nMedalID)
{
    MedalUnitDB* unit = FindMedal(nMedalID);
    if (NULL == unit)
    {
        return;
    }
    unit->setCurState(eMedalState_Activated);

    UpdateMedalToDB( unit );
    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Show);
}

void CEntityMedalComponent::SetMedalFormShowToInvalid(int nMedalID)
{
    MedalUnitDB* unit = FindMedal(nMedalID);
    if (NULL == unit)
    {
        return;
    }
    unit->setCurState(eMedalState_Invalid);

    UpdateMedalToDB( unit );
    LogMedal(nMedalID, CMedalLog::EMedalInvalid);

    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Show);
    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Activated);
    InsertMedalIDToStateTypeList(nMedalID, eMedalState_Invalid);
}
void CEntityMedalComponent::SetMedalFromInvalidToActivated(int nMedalID)
{
    MedalUnitDB* unit = FindMedal(nMedalID);
    if (NULL == unit)
    {
        return;
    }
    unit->setCurState(eMedalState_Activated);
    time_t now = time(NULL);
    unit->setGetTime( int(now) );

    UpdateMedalToDB( unit );
    DelMedalIDFormStateTypeList(nMedalID, eMedalState_Invalid);
    InsertMedalIDToStateTypeList(nMedalID, eMedalState_Activated);

    int nAddScore = CMedalCfgMgr::Instance().GetMedalScore(nMedalID);
    ChangeMedalScore(nAddScore, nAddScore);

    GameMsg_S2C_GetMedalScore retmsg;
    retmsg.m_nScore = m_nMedalCumulativeScore;
    m_pNetComponent->SendPlayerMsg(&retmsg);
}

void CEntityMedalComponent::SetMedalActivited(const std::list<int> & listmedalid ,CMedalLog::EMedalSource eMedalSource)
{
    GameMsg_S2C_Medal_GetConditionReward resultMsg;

    for (std::list<int>::const_iterator it = listmedalid.begin();
        it != listmedalid.end(); ++it)
    {
        SetMedalState(*it, eMedalState_Activated);
        resultMsg.m_nMedalID = *it;
        resultMsg.m_nRetCode = eMedalGetConditionRewardSuccess;
        resultMsg.m_nMedalType = CMedalCfgMgr::Instance().GetMedalType(*it);
        m_pNetComponent->SendPlayerMsg(&resultMsg);
        LogMedal(*it, CMedalLog::EMedalActivated, eMedalSource);
    }
}

void CEntityMedalComponent::SyncAllDisplayMedalsForOther( CParamPool &IOBuff )
{
    std::list<int> listMedalIDs;
    std::map<int, std::list<int> >::const_iterator mapItr = m_mapStateTypeMedal.find( eMedalState_Activated );

    if ( mapItr != m_mapStateTypeMedal.end() )
    {
        listMedalIDs = mapItr->second;
    }

    GetSortMedal( listMedalIDs );

    IOBuff.AddUShort( (unsigned short)listMedalIDs.size() );
    for ( std::list<int>::const_iterator itr = listMedalIDs.begin(); 
        itr != listMedalIDs.end(); ++itr )
    {
        MedalDisplayInfo displayInfo;
        displayInfo.m_nType = CMedalCfgMgr::Instance().GetMedalType( *itr );
        CMedalCfgMgr::Instance().GetMedalIcon( *itr, displayInfo.m_strIcon );

        displayInfo.doEncode( IOBuff );
    }
}

void CEntityMedalComponent::EncodePlayerActivityMedals( std::list<PlayerMedal> &rInfos )
{
    std::list<int> listMedalIDs;
    std::map<int, std::list<int> >::const_iterator mapItr = m_mapStateTypeMedal.find( eMedalState_Activated );

    if ( mapItr != m_mapStateTypeMedal.end() )
    {
        listMedalIDs = mapItr->second;
    }

    GetSortMedal( listMedalIDs );

    for ( std::list<int>::const_iterator itr = listMedalIDs.begin(); itr != listMedalIDs.end(); ++itr )
    {
        PlayerMedal info;
        info.m_nID = *itr;
        info.m_nType = CMedalCfgMgr::Instance().GetMedalType( *itr );

        rInfos.push_back( info );
    }
}

void CEntityMedalComponent::EncodePlayerShowMedals( std::list<PlayerMedal> &rInfos )
{
    std::list<int> listMedalIDs;
    std::map<int, std::list<int> >::const_iterator mapItr = m_mapStateTypeMedal.find( eMedalState_Show );

    if ( mapItr != m_mapStateTypeMedal.end() )
    {
        listMedalIDs = mapItr->second;
    }

    GetSortMedal( listMedalIDs );

    for ( std::list<int>::const_iterator itr = listMedalIDs.begin(); itr != listMedalIDs.end(); ++itr )
    {
        PlayerMedal info;
        info.m_nID = *itr;
        info.m_nType = CMedalCfgMgr::Instance().GetMedalType( *itr );

        rInfos.push_back( info );
    }
}

int CEntityMedalComponent::EncodePlayerMedalScore()
{
    return m_nMedalCumulativeScore;
}

void CEntityMedalComponent::BroadcastShowMedal( int nMedalID )
{
    if ( m_pAttrComponent == NULL )
        return;

    { // 广播更新 group
        GameMsg_S2G_AddMedals sgMsg;
        sgMsg.m_nMedalID = nMedalID;
        sgMsg.m_nSceneID = m_nSceneID;
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }

    GameMsg_S2C_SyncMedalInfoToOthers syncMsg;
    syncMsg.m_nSceneID = m_nSceneID;
    syncMsg.m_nRoleID = m_pAttrComponent->GetRoleID();
    syncMsg.m_displayInfo.m_nType = CMedalCfgMgr::Instance().GetMedalType( nMedalID );
    CMedalCfgMgr::Instance().GetMedalIcon( nMedalID, syncMsg.m_displayInfo.m_strIcon );

    if ( m_pRoomComponent != NULL && m_pRoomComponent->Room() != NULL )
    {
        m_pRoomComponent->Room()->SendMsgToAll( &syncMsg, m_pRoomComponent );
    }
    if ( m_pCoupleComponent != NULL && m_pCoupleComponent->WeddingRoom() != NULL )
    {
        m_pCoupleComponent->WeddingRoom()->SendMsgToAllBut( &syncMsg, m_pCoupleComponent );
    }
    if ( m_pPhotoRoomComponent != NULL && m_pPhotoRoomComponent->PhotoRoom() != NULL )
    {
        m_pPhotoRoomComponent->PhotoRoom()->SendMsgToAllBut( &syncMsg, m_pPhotoRoomComponent );
    }
    if ( m_pAmuseRoomComponent != NULL && m_pAmuseRoomComponent->AmuseRoom() != NULL )
    {
        m_pAmuseRoomComponent->AmuseRoom()->SendMsgToAll( &syncMsg, m_pAmuseRoomComponent );
    }
    if ( m_pDanceGroupComponent != NULL && m_pDanceGroupComponent->GetCeremonyRoom() != NULL )
    {
        m_pDanceGroupComponent->GetCeremonyRoom()->RoomBroadcastExcept( syncMsg.m_nRoleID, syncMsg );
    }
}

void CEntityMedalComponent::GetActivityMedals(std::list<int> &listMedalIDs)
{
    std::map<int, std::list<int> >::const_iterator mapItr = m_mapStateTypeMedal.find(eMedalState_Activated);

    if (mapItr != m_mapStateTypeMedal.end())
    {
        listMedalIDs = mapItr->second;
    }

    GetSortMedal(listMedalIDs);

    return;
}


void CEntityMedalComponent::RankConditionDo(IMedalCondition *medalcondit , const RankData &rankData)
{
    CommonParam param;
    int nType = CMedalCfgMgr::Instance().GetProgressType(medalcondit->GetConditionID() );
    switch(nType)
    {
    case EMedalProgressType_BuyRank:
        param.SetParam("mcoinrank",rankData.m_RankMCoin);
        break;
    case EMedalProgressType_LevelRank:
        param.SetParam("exprank", rankData.m_RankExp);
        break;
    case EMedalProgressType_TrumpetRank:
        param.SetParam("hornrank", rankData.m_RankHorn);
        break;
    case EMedalProgressType_HonourDanceGroupRank:
        param.SetParam("grouhonorrank", rankData.m_RankGroupHonor);
        break;
    case EMedalProgressType_ManWifeRank:
        param.SetParam("intimacyrank", rankData.m_RankIntimacy);
        break;
    case EMedalProgressType_MedalScoreRank:
        param.SetParam("medalrank", rankData.m_RankMedalScore);
        break;
    case EMedalProgressType_ClothEffectRank:
        param.SetParam("clotheffectrank", rankData.m_RankClothEffect);
        break;
    case EMedalProgressType_EquipCountRank:
        param.SetParam("equipcountrank", rankData.m_RankEquipCount);
        break;
    default:
        break;
    }

    if (medalcondit != NULL)
    {
        medalcondit->process(param);
    }
}

void CEntityMedalComponent::getAllRankCondition(std::list<int > & result)
{
    for (std::map<int, IMedalCondition* >::iterator it = m_mapAllCondition.begin()
        ; it != m_mapAllCondition.end(); ++it)
    {
        int nCondType = CMedalCfgMgr::Instance().GetProgressType(it->first);
        switch (nCondType)
        {
        case EMedalProgressType_BuyRank:
        case EMedalProgressType_TrumpetRank:
        case EMedalProgressType_LevelRank:
        case EMedalProgressType_HonourDanceGroupRank:
        case EMedalProgressType_ManWifeRank:
        case EMedalProgressType_MedalScoreRank:
        case EMedalProgressType_ClothEffectRank:
        case EMedalProgressType_EquipCountRank:
            result.push_back(it->first);
            break;
        default:
            break;
        }
    }
}

void CEntityMedalComponent::CheckPushMsgTask(IMedalCondition &Condition)
{
    unsigned int now = (unsigned int)time(NULL);
    int nConditionId = Condition.GetConditionID();
    if (
        !(now > CMedalCfgMgr::Instance().GetAllowGetStartTime(nConditionId)
        && now < CMedalCfgMgr::Instance().GetAllowGetEndTime(nConditionId)
        && CheckIsVisibility(CMedalCfgMgr::Instance().GetVisibilityIndex(nConditionId) ) )
        )
    {
        return;
    }

    int nTotalProgress = CMedalCfgMgr::Instance().GetTotalProgress(Condition.GetConditionID());
    if (Condition.CheckIsAllowGet(nTotalProgress))
    {
        int medalid = CMedalCfgMgr::Instance().GetRewardMedal(Condition.GetConditionID());
        GameMsg_S2C_PushCompleteMedal msg;
        msg.m_setAllMedalId.insert(medalid);
        m_pNetComponent->SendPlayerMsg(&msg);
    }
}

void CEntityMedalComponent::LogMedal(int MedalID, CMedalLog::EMedalAction eAction, CMedalLog::EMedalSource eMedalSource /* = CMedalLog::EMedalUnKonw */, int ConditionID /* = 0 */)
{
    CMedalLog *pLog = new CMedalLog();
    pLog->m_nRoleID = m_pAttrComponent->GetRoleID();
    pLog->m_nMedalID = MedalID;
    pLog->m_nConditionID = ConditionID;
    pLog->m_eAction = eAction;
    pLog->m_eActivity = eMedalSource;
    pLog->m_nTimeStamp = (int)time(NULL);
    AddQuery(QUERY_LogRoleMedalLog, 0, pLog);
}

void CEntityMedalComponent::ChangeMedalScore(int nWearChange, int nCumulativeChange)
{
    m_nMedalWearScore += nWearChange;
    m_nMedalCumulativeScore += nCumulativeChange;

    CommonParam param;
    param.SetParam(std::string("medal"), std::string("medal_score"));
    param.SetParam(std::string("medal_score"), m_nMedalCumulativeScore);
    OnNotify(param);
    
    if (m_pRankComponent != NULL)
    {
        m_pRankComponent->SubmitRankValue(ERankDataType_Medal, nCumulativeChange);
    }

    if (nWearChange != 0 || nCumulativeChange != 0)
    {
        MedalScoreDB *pMedalScore = new MedalScoreDB();
        pMedalScore->m_nMedalCumulativeScore = m_nMedalCumulativeScore;
        pMedalScore->m_nMedalWearScore = m_nMedalWearScore;
        AddQuery(QUERY_Medal_UpdateScore, m_pAttrComponent->GetRoleID(), pMedalScore);
    }
}

/************************************************************************/
/* MSG                                                                     */
/************************************************************************/

void CEntityMedalComponent::OnGetMedalInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_GetMedalInfo *pClientMsg = (GameMsg_C2S_GetMedalInfo *)&rMsg;
    if (pClientMsg == NULL)
    {
        return;
    }

    GameMsg_S2C_GetMedalInfo resultMsg;
    GameMsg_S2C_GetMedalInfo::PerMedalInfo perInfo;

    for (size_t i = 0; i < pClientMsg->m_vecMedalID.size(); ++i)
    {
        int nMedalID = pClientMsg->m_vecMedalID[i];

        perInfo.nMedalID = nMedalID;
        perInfo.nType = CMedalCfgMgr::Instance().GetMedalType(nMedalID);
        perInfo.nNeedMedalMoney = CMedalCfgMgr::Instance().GetNeedMedalMoney(nMedalID);
        CMedalCfgMgr::Instance().GetMedalName(nMedalID, perInfo.strName);
        CMedalCfgMgr::Instance().GetMedalIcon(nMedalID, perInfo.strIcon);
        CMedalCfgMgr::Instance().GetMedalDescribe(nMedalID, perInfo.strDescribe);

        perInfo.nState = CMedalCfgMgr::Instance().IsMedalForevery(nMedalID) ? 1 : 0;

        //std::map<int, MedalUnitDB* >::iterator medalIt = m_mapAllMedal.find(nMedalID);
        //if (medalIt != m_mapAllMedal.end())
        //{
        //    if (CMedalCfgMgr::Instance().IsMedalForevery(nMedalID) )
        //    {
        //        perInfo.nState = 1;
        //    }
        //    else
        //    {
        //        perInfo.nState = 0;
        //    }
        //}
        //else
        //{
        //    perInfo.nState = 0;
        //}

        resultMsg.m_listPerMedalInfo.push_back(perInfo);
    }

    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGetConditionInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if ( m_bIsRankOpen )
    {
        GameMsg_S2G_GetRankAllDataForMedal S2Gmsg;
        S2Gmsg.m_nRoleId = m_pAttrComponent->GetRoleID();
        S2Gmsg.m_GroupId = m_pDanceGroupComponent->GetDanceGroupID();
        S2Gmsg.m_IntimacyId = m_pCoupleComponent->PairID();
        m_pNetComponent->Send2GroupServer((GameMsg_Base *)&S2Gmsg);
    }
    else
    {
        std::list<int> listConditionIDSort;

        unsigned int now = (unsigned int)time(NULL);

        for (std::map<int, IMedalCondition*>::iterator itCond = m_mapAllCondition.begin();
            itCond != m_mapAllCondition.end(); ++itCond)
        {
            if (now > CMedalCfgMgr::Instance().GetAllowGetStartTime(itCond->first) 
                && now < CMedalCfgMgr::Instance().GetAllowGetEndTime(itCond->first) 
                && CheckIsVisibility(CMedalCfgMgr::Instance().GetVisibilityIndex(itCond->first) ) )
            {
                listConditionIDSort.push_back(itCond->first);
            }
        }

        GameMSg_S2C_GetConditionInfo resultMsg;
        for (std::list<int>::iterator condit = listConditionIDSort.begin();
            condit!= listConditionIDSort.end() ; ++condit)
        {
            int nConditionID = *condit;
            IMedalCondition *condition = FindCondition(nConditionID);
            if (NULL != condition)
            {
                GameMSg_S2C_GetConditionInfo::PerConditionInfo perInfo;

                perInfo.nCondititonID = nConditionID;
                perInfo.nCurProgress = condition->GetCurProgress();
                perInfo.nTotalProtgress = CMedalCfgMgr::Instance().GetTotalProgress(nConditionID);
                int nRewardMedal = CMedalCfgMgr::Instance().GetRewardMedal(nConditionID);
                perInfo.nRewardMedalID = nRewardMedal;
                perInfo.isHaveGot = (unsigned char)condition->GetAreadyReward();
                perInfo.nTypeID = CMedalCfgMgr::Instance().GetProgressType(nConditionID);
                CMedalCfgMgr::Instance().GetSpecialParams(nConditionID, perInfo.m_listDestID);

                resultMsg.m_listPerConditionInfo.push_back(perInfo);
            }
        }

        m_pNetComponent->SendPlayerMsg(&resultMsg);
    }
}

void CEntityMedalComponent::OnGroupRankDataResultForMedal(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_G2S_GetRankAllDataForMedal *pG2sMsg = (GameMsg_G2S_GetRankAllDataForMedal*)&rMsg;
    if (pG2sMsg == NULL)
    {
        return;
    }
    std::list<int> listConditionIDSort;

    unsigned int now = (unsigned int)time(NULL);

    for (std::map<int, IMedalCondition*>::iterator itCond = m_mapAllCondition.begin();
        itCond != m_mapAllCondition.end(); ++itCond)
    {
        if (now > CMedalCfgMgr::Instance().GetAllowGetStartTime(itCond->first) 
            && now < CMedalCfgMgr::Instance().GetAllowGetEndTime(itCond->first) 
            && CheckIsVisibility(CMedalCfgMgr::Instance().GetVisibilityIndex(itCond->first) ) )
        {
            listConditionIDSort.push_back(itCond->first);
        }
    }

    //特殊处理获取rank类型Condition
    std::list<int> rankList;
    getAllRankCondition(rankList);

    //装载数据
    GameMSg_S2C_GetConditionInfo resultMsg;
    RankData rankData;
    rankData.m_RankMCoin = pG2sMsg->m_RankMCoin;
    rankData.m_RankExp = pG2sMsg->m_RankExp;
    rankData.m_RankHorn = pG2sMsg->m_RankHorn;
    rankData.m_RankGroupHonor = pG2sMsg->m_RankGroupHonor;
    rankData.m_RankIntimacy = pG2sMsg->m_RankIntimacy;
    rankData.m_RankMedalScore = pG2sMsg->m_RankMedalScore;
    rankData.m_RankClothEffect = pG2sMsg->m_RankClothEffect;
    rankData.m_RankEquipCount = pG2sMsg->m_RankEquipCount;
    for (std::list<int>::iterator it = listConditionIDSort.begin();
        it != listConditionIDSort.end(); ++it)
    {
        int nConditionID = *it;
        std::list<int>::iterator isRankIt = std::find(rankList.begin(), rankList.end(), *it);
        if (rankList.end() != isRankIt) //找到排行榜类型
        {
            //装载数据
            IMedalCondition* medalcondit = FindCondition(*it);
            RankConditionDo(medalcondit, rankData);
        }

        IMedalCondition *condition = FindCondition(nConditionID);
        if (NULL != condition)
        {
            GameMSg_S2C_GetConditionInfo::PerConditionInfo perInfo;

            perInfo.nCondititonID = nConditionID;
            perInfo.nCurProgress = condition->GetCurProgress();
            perInfo.nTotalProtgress = CMedalCfgMgr::Instance().GetTotalProgress(nConditionID);
            int nRewardMedal = CMedalCfgMgr::Instance().GetRewardMedal(nConditionID);
            perInfo.nRewardMedalID = nRewardMedal;
            perInfo.isHaveGot = (unsigned char)condition->GetAreadyReward();
            perInfo.nTypeID = CMedalCfgMgr::Instance().GetProgressType(nConditionID);
            CMedalCfgMgr::Instance().GetSpecialParams(nConditionID, perInfo.m_listDestID);

            resultMsg.m_listPerConditionInfo.push_back(perInfo);
        }
    }

    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGroupRankDataResult(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_G2S_GetRankAllData *pG2sMsg = (GameMsg_G2S_GetRankAllData*)&rMsg;
    if (pG2sMsg == NULL)
    {
        return;
    }

    std::list<int> listConditionIDSort;
    unsigned int now = (unsigned int)time(NULL);
    for (std::map<int, IMedalCondition*>::iterator itCond = m_mapAllCondition.begin();
        itCond != m_mapAllCondition.end(); ++itCond)
    {
        if (now > CMedalCfgMgr::Instance().GetAllowGetStartTime(itCond->first) 
            && now < CMedalCfgMgr::Instance().GetAllowGetEndTime(itCond->first) 
            && CheckIsVisibility(CMedalCfgMgr::Instance().GetVisibilityIndex(itCond->first) ) )
        {
            listConditionIDSort.push_back(itCond->first);
        }
    }

    //特殊处理获取rank类型Condition
    std::list<int> rankList;
    getAllRankCondition(rankList);

    //装载数据
    GameMSg_S2C_GetConditionInfo resultMsg;
    RankData rankData;
    rankData.m_RankMCoin = pG2sMsg->m_RankMCoin;
    rankData.m_RankExp = pG2sMsg->m_RankExp;
    rankData.m_RankHorn = pG2sMsg->m_RankHorn;
    rankData.m_RankGroupHonor = pG2sMsg->m_RankGroupHonor;
    rankData.m_RankIntimacy = pG2sMsg->m_RankIntimacy;
    rankData.m_RankMedalScore = pG2sMsg->m_RankMedalScore;
    rankData.m_RankClothEffect = pG2sMsg->m_RankClothEffect;
    rankData.m_RankEquipCount = pG2sMsg->m_RankEquipCount;
    for (std::list<int>::iterator it = listConditionIDSort.begin();
        it != listConditionIDSort.end(); ++it)
    {
        int nConditionID = *it;
        std::list<int>::iterator isRankIt = std::find(rankList.begin(), rankList.end(), *it);
        if (rankList.end() != isRankIt) //找到排行榜类型
        {
            //装载数据
            IMedalCondition* medalcondit = FindCondition(nConditionID);
            RankConditionDo(medalcondit ,rankData);

            //检测是否满足领取条件
            if (medalcondit->GetAreadyReward() != 0)
            {
                continue;
            }

            int nTotalProgress = CMedalCfgMgr::Instance().GetTotalProgress(nConditionID);
            if (medalcondit->CheckIsAllowGet(nTotalProgress) )
            {
                m_setNeedPushRankConditionMedalId.insert(CMedalCfgMgr::Instance().GetRewardMedal(nConditionID) );
            }
        }
    }

    if (m_setNeedPushRankConditionMedalId.size() != 0)
    {
        GameMsg_S2C_PushCompleteMedal msg;
        for (std::set<int>::iterator it = m_setNeedPushRankConditionMedalId.begin();
            it != m_setNeedPushRankConditionMedalId.end(); ++it)
        {
            msg.m_setAllMedalId.insert(*it);
        }
        m_pNetComponent->SendPlayerMsg(&msg);
        //清除需要推送的数据
        m_setNeedPushRankConditionMedalId.clear();
    }
}

void CEntityMedalComponent::OnGetMedalHandBook(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    std::list<int> listMedalIDSort;
    CMedalCfgMgr::Instance().GetAllMedalIDList(listMedalIDSort);

//     for (std::map<int, MedalUnitDB*>::iterator it = m_mapAllMedal.begin();
//         it != m_mapAllMedal.end(); ++it)
//     {
//         listMedalIDSort.push_back(it->first);
//     }

    GetSortMedal(listMedalIDSort);

    GameMsg_S2C_MedalHandBook resultMsg;
    for (std::list<int>::iterator medaIdIt = listMedalIDSort.begin();
        listMedalIDSort.end() != medaIdIt; ++medaIdIt)
    {
        int nMedalId = *medaIdIt;

        GameMsg_S2C_MedalHandBook::PerMedalInfo perInfo;
        perInfo.m_nMedalId = nMedalId;
        perInfo.m_nType = CMedalCfgMgr::Instance().GetMedalType(nMedalId);
        perInfo.m_nDuration = CMedalCfgMgr::Instance().GetMedalDuration(nMedalId);
        resultMsg.m_listPerMedalInfo.push_back(perInfo);
    }
    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGetMedalExchangeInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    std::list<int> listMedalIDSort;
    for (std::map<int, MedalUnitDB*>::iterator it = m_mapAllMedal.begin();
        it != m_mapAllMedal.end(); ++it)
    {
        if (it->second->getCurState() == eMedalState_Invalid
            ||it->second->getCurState() == eMedalState_Activated
            ||it->second->getCurState() == eMedalState_Show)
        {
            listMedalIDSort.push_back(it->first);
        }
    }

    GetSortMedal(listMedalIDSort);

    GameMsg_S2C_Medal_Exchange_Info resultMsg;
    for (std::list<int>::iterator medaId_it = listMedalIDSort.begin();
        listMedalIDSort.end() != medaId_it; ++medaId_it)
    {
        GameMsg_S2C_Medal_Exchange_Info::PerExchangeInfo perinfo;
        perinfo.m_nMedalId = *medaId_it;
        perinfo.m_nPrice = CMedalCfgMgr::Instance().GetNeedMedalMoney(*medaId_it);
        resultMsg.m_listPerExchangeInfo.push_back(perinfo);
    }
    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnSetMedalShow(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    unsigned char nRetCode = eSetMedalShowSuccess;
    GameMSg_C2S_Medal_Show *pClientMsg = (GameMSg_C2S_Medal_Show *)&rMsg;
    if (pClientMsg == NULL)
    {
        return;
    }

    for (std::list<int>::iterator it = pClientMsg->m_listMedalID.begin();
        pClientMsg->m_listMedalID.end() != it; ++it)
    {
        MedalUnitDB *pUnit = FindMedal(*it);
        if (NULL == pUnit)
        {
            return;
        }

        if (pUnit->getCurState() == eMedalState_Underway ||
            pUnit->getCurState() == eMedalState_Invalid)
        {
            nRetCode = eSetMedalShowFail;
            break;
        }
    }

    if (nRetCode == eSetMedalShowSuccess)
    {
        for (std::map<int, MedalUnitDB* > ::iterator perIt = m_mapAllMedal.begin();
            perIt != m_mapAllMedal.end(); ++perIt)
        {
            if (perIt->second->getCurState() == eMedalState_Show)
            {
                SetMedalState(perIt->first, eMedalState_Activated, false);
            }
        }

        for (std::list<int>::iterator it = pClientMsg->m_listMedalID.begin();
            pClientMsg->m_listMedalID.end() != it; ++it)
        {
            SetMedalState(*it, eMedalState_Show);
        }
    }

    GameMSg_S2C_Medal_Show resultMsg ;
    resultMsg.m_nReCode = nRetCode;
    resultMsg.m_listPerShowID = pClientMsg->m_listMedalID;

    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGetMedalExchange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_Medal_Exchange *pClientMsg = (GameMsg_C2S_Medal_Exchange*)&rMsg;
    if (pClientMsg == NULL)
    {
        return;
    }

    GameMsg_S2C_Medal_Exchange resultMsg;

    m_nSceneID = pClientMsg->m_byScene;
    resultMsg.m_nRetCode = (eMedalExchangeRes)ExchangeMedal(pClientMsg->m_nMedalID);
    
    resultMsg.m_nMedalID = pClientMsg->m_nMedalID;
    resultMsg.m_nMedalType = CMedalCfgMgr::Instance().GetMedalType(pClientMsg->m_nMedalID);

    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGetMedalActivityInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GetMedalActivityInfo();
}

void CEntityMedalComponent::OnGetMedalScore(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_S2C_GetMedalScore retmsg;
    retmsg.m_nScore = m_nMedalCumulativeScore;
    m_pNetComponent->SendPlayerMsg(&retmsg);
}

void CEntityMedalComponent::OnGetMedalScoreDescribe(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_S2C_Medal_GetScoreDescribe retmsg;
    CMedalCfgMgr::Instance().GetMedalScoreDescribe(retmsg.m_strDescribe);
    m_pNetComponent->SendPlayerMsg(&retmsg);
}

void CEntityMedalComponent::GetMedalActivityInfo()
{
    GameMsg_S2C_Medal_ActivityInfo resultMsg;

    std::list<int> sortedlist;
    for (std::map<int,MedalUnitDB*>::iterator it = m_mapAllMedal.begin();
        it != m_mapAllMedal.end(); ++it)
    {
        if (it->second->getCurState() == eMedalState_Activated
            || it->second->getCurState() == eMedalState_Show)
        {
            sortedlist.push_back(it->first);
        }
    }

    GetSortMedal(sortedlist);

    for (std::list<int>::iterator it = sortedlist.begin();
        it != sortedlist.end(); ++it)
    {
        GameMsg_S2C_Medal_ActivityInfo::perShowInfo perinfo;
        perinfo.m_nID = *it;
        perinfo.m_nType = CMedalCfgMgr::Instance().GetMedalType(*it);
        CMedalCfgMgr::Instance().GetMedalIcon(*it, perinfo.m_strIcon);
        resultMsg.m_listPerInfo.push_back(perinfo);
    }
    m_pNetComponent->SendPlayerMsg(&resultMsg);
}

void CEntityMedalComponent::OnGetMedalConditionReward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    //初始化
    GameMsg_C2S_Medal_GetConditionReward *pClientMsg = (GameMsg_C2S_Medal_GetConditionReward *)&rMsg;
    if (NULL == pClientMsg)
    {
        return;
    }

    m_nSceneID = pClientMsg->m_bySceneid;
    GameMsg_S2C_Medal_GetConditionReward resultMsg;
    resultMsg.m_nRetCode = eMedalGetConditionRewardFail;
    resultMsg.m_nMedalID = 0;
    resultMsg.m_nMedalType = 0;

    int nDestConditionid = pClientMsg->m_nConditionid;
    IMedalCondition* condition = FindCondition(nDestConditionid);
    if (NULL == condition)
    {
        resultMsg.m_nRetCode = emedalGetConditionRewardNull;
        m_pNetComponent->SendPlayerMsg(&resultMsg);
        return;
    }
    else
    {
        //判断是否已经领取过
        if (condition->GetAreadyReward() != 0)
        {
            resultMsg.m_nRetCode = eMedalGetConditionRewardAlreadyGet;
            m_pNetComponent->SendPlayerMsg(&resultMsg);
            return;
        }
        int nConditionType = CMedalCfgMgr::Instance().GetProgressType(nDestConditionid);

        //判断是否可以获取
        int nTotalProgress = CMedalCfgMgr::Instance().GetTotalProgress(nDestConditionid);
        if (condition->CheckIsAllowGet(nTotalProgress) )
        {
            int nRewardMedalid = CMedalCfgMgr::Instance().GetRewardMedal(nDestConditionid);
            if (SetMedalState(nRewardMedalid, eMedalState_Activated))
            {
                resultMsg.m_nRetCode = eMedalGetConditionRewardSuccess;
                resultMsg.m_nMedalID = nRewardMedalid;
                resultMsg.m_nMedalType = CMedalCfgMgr::Instance().GetMedalType(nRewardMedalid);
                
                m_pNetComponent->SendPlayerMsg(&resultMsg);
                
                condition->SetAreadyReward(1);
                //存数据库
                MedalConditionDB *pCond = new MedalConditionDB( condition->GetConditionID(),
                    condition->GetCurProgress(), condition->GetAreadyReward() );
                AddQuery( QUERY_Medal_UpdateProgress, m_pAttrComponent->GetRoleID(), pCond );
                LogMedal(nRewardMedalid, CMedalLog::EMedalActivated, CMedalLog::EMedalCondition, condition->GetConditionID() );
                return;
            }
        }
        else    //vip失败的特殊处理
        {
            if ((!GetComponent<CRoleVIP>()->IsVIP() )
                && nConditionType == EMedalProgressType_VipLevel
                && condition->GetCurProgress() >= nTotalProgress)
            {
                resultMsg.m_nRetCode = eMedalGetConditionRewardVipTimeOut;
            }
        }
        m_pNetComponent->SendPlayerMsg(&resultMsg);
    }
}

bool CEntityMedalComponent::CheckIsVisibility(int nVisibility)
{
    if (nVisibility == ESexType_No)
    {
        return true;
    }

    if (nVisibility == m_pAttrComponent->m_nSex)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CEntityMedalComponent::UpdateMedalToDB( const MedalUnitDB *pMedalUnitDB )
{
    if ( pMedalUnitDB == NULL || m_pAttrComponent  == NULL )
        return;

    MedalUnitDB *unit = new MedalUnitDB( *pMedalUnitDB );
    AddQuery( QUERY_Medal_UpdateMedal, m_pAttrComponent->GetRoleID(), unit );
}

CEntityMedalComponent::RankData::RankData()
    : m_RankMCoin(0)
    , m_RankExp(0)
    , m_RankHorn(0)
    , m_RankGroupHonor(0)
    , m_RankIntimacy(0)
    , m_RankMedalScore(0)
    , m_RankClothEffect(0)
    , m_RankEquipCount(0)
{

}
