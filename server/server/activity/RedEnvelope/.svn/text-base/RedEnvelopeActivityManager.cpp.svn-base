#include "RedEnvelopeActivityManager.h"
#include "../../../socket/GameMsg_Map.h"
#include "../../../socket/GameMsg_Processor.h"
#include "../../share/GameMsg_ChargeOnline.h"
#include "../../../datastructure/RoleEntity.h"
#include "../../logic/PlayerManager.h"
#include "../../libServerFrame/QueryManager.h"
#include "RedEnvelope.h"
#include "GameMsg_CS_RedEnvelope.h"
#include "../../logic/LogicCirculator.h"
#include "../SystemActivityManager.h"
#include "RedEnvelopeConfigManager.h"
#include "GameMsg_GS_RedEnvelope.h"

extern CQueryManager* g_pQueryMgr;

RedEnvelopeActivity::RedEnvelopeActivity()
{
    SetActivityType(EActivity_RedEnvelope);
}

RedEnvelopeActivity::~RedEnvelopeActivity()
{

}

void RedEnvelopeActivity::Init(time_t tBeginTime, time_t tEndTime)
{
    CTimeActivity::Init(tBeginTime, tEndTime);
}

void RedEnvelopeActivity::OnActivityBegin()
{
    RedEnvelopeActivityManager::Instance().SetActivityOpen(true);
}

void RedEnvelopeActivity::OnActivityEnd()
{
    RedEnvelopeActivityManager::Instance().SetActivityOpen(false);
}

RedEnvelopeActivityManager::RedEnvelopeActivityManager()
    : GameMsg_Processor(true, true)
    , m_bAnonymity(false)
    , m_bActivityOpen(false)
{

}

RedEnvelopeActivityManager::~RedEnvelopeActivityManager()
{

}

RedEnvelopeActivityManager & RedEnvelopeActivityManager::Instance()
{
    static RedEnvelopeActivityManager s_RedEnvelopeProcessor;

    return s_RedEnvelopeProcessor; 
}

void RedEnvelopeActivityManager::Initialize()
{
    // add activity to manager
    InitializeActivity();

    // regis message
    RegisterMessage();
}

void RedEnvelopeActivityManager::InitializeActivity()
{
    unsigned int nActivityBeginTime = RedEnvelopeConfigManager::Instance().GetActivityBeginTime();
    unsigned int nActivityEndTime = RedEnvelopeConfigManager::Instance().GetActivityEndTime();

    RedEnvelopeActivity * pActivity = new RedEnvelopeActivity();
    pActivity->Init(nActivityBeginTime, nActivityEndTime);
    SetActivityOpen(pActivity->IsOpening());
    CSystemActivityManager::Instance().AddTimerActivity(pActivity);
}

void RedEnvelopeActivityManager::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_A2S_RedEnvelope);


    RegMsgProcessor(MSG_A2S_RedEnvelope, &RedEnvelopeActivityManager::OnPlayerCharge);
    RegQueryProcessor(QUERY_RedEnvelope_LoadRoleInfo, &RedEnvelopeActivityManager::OnLoadRoleInfo);
}

void RedEnvelopeActivityManager::OnPlayerCharge(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    bool bActivityOpen = RedEnvelopeConfigManager::Instance().IsInActivity((unsigned int)time(NULL));
    if (!bActivityOpen)
    {
        return;
    }

    GameMsg_A2S_RedEnvelope & a2smsg = (GameMsg_A2S_RedEnvelope&)msg;
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(a2smsg.m_nRoleID);
    if (pRoleEntity != NULL)
    {
        unsigned int nDanceGroupID = pRoleEntity->GetDanceGroupID();
        SyncChargeInfoToGroup(a2smsg.m_nRoleID, pRoleEntity->GetRoleName(), a2smsg.m_nMCoin, nDanceGroupID, IsAnonymity());
    }
    else
    {
        RedEnvelopeSenderInfo *pRoleInfo = new RedEnvelopeSenderInfo();
        pRoleInfo->m_nRoleID = a2smsg.m_nRoleID;
        g_pQueryMgr->AddQuery(QUERY_RedEnvelope_LoadRoleInfo, a2smsg.m_nMCoin, pRoleInfo);
    }
}

void RedEnvelopeActivityManager::OnLoadRoleInfo(QueryBase & query)
{
    int nMCoin = (int)query.m_nAttachment;
    RedEnvelopeSenderInfo * pRoleInfo = (RedEnvelopeSenderInfo *)query.m_pAttachment;
    if (pRoleInfo == NULL)
    {
        return;
    }

    SyncChargeInfoToGroup(pRoleInfo->m_nRoleID, pRoleInfo->m_strRoleName, nMCoin, pRoleInfo->m_nDanceGroupID, pRoleInfo->m_bIsAnonymity);
    delete pRoleInfo; pRoleInfo = NULL;
}

void RedEnvelopeActivityManager::SyncChargeInfoToGroup(unsigned int nRoleID, const std::string & strRoleName, int nMCoin,
            unsigned int nDanceGroupID, bool bAnonymity)
{
    GameMsg_S2G_CreateRedEnvelope syncmsg;
    syncmsg.m_nRoleID = nRoleID;
    syncmsg.m_strRoleName = strRoleName;
    syncmsg.m_nMCoin = nMCoin;
    syncmsg.m_nDanceGroupID = nDanceGroupID;
    syncmsg.m_bIsAnonymity = bAnonymity;
    ::SendMsg2GroupServer(&syncmsg);
}

void RedEnvelopeActivityManager::SetAnonymity(bool bAnonymity)
{
    m_bAnonymity = bAnonymity;
}

bool RedEnvelopeActivityManager::IsAnonymity()
{
    return m_bAnonymity;
}

bool RedEnvelopeActivityManager::IsActivityOpen()
{
    return m_bActivityOpen;
}

void RedEnvelopeActivityManager::SetActivityOpen(bool bIsOpen)
{
    m_bActivityOpen = bIsOpen;
}
