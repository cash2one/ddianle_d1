#include "EntityHorseComponent.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "HorseCfg.h"


CEntityHorseComponent::CEntityHorseComponent()
: CEntityComponent(true, true)
, m_pItemComponent(NULL)
, m_pRoomComponent(NULL)
, m_pNetComponent(NULL)
, m_pCoupleComponent(NULL)
, m_pAmuseRoomComponent(NULL)
, m_pDanceGroupComponent(NULL)
, m_nCurHorseOwnerId(0)
, m_nCurHorseTypeId(0)
, m_nHorseSeatIndex(0)
, m_bIsOpen(false)
, m_nRoleID(0)
, m_nSenceId(0)
{
    m_bIsOpen = CSystemSettingMgr::Instance().IsFunctionOpen( EFunctionSwitch_Horse );
}

CEntityHorseComponent::~CEntityHorseComponent()
{

}

void CEntityHorseComponent::Start()
{
    m_pNetComponent = GetComponent<CRoleNet>();
    m_pItemComponent = GetComponent<CRoleItem>();
    m_pCoupleComponent = GetComponent<CRoleCouple>();
    m_pDanceGroupComponent = GetComponent<CRoleDanceGroup>();
    m_pAmuseRoomComponent = GetComponent<CRoleAmuseRoom>();
    m_pRoomComponent = GetComponent<CRoleRoom>();

    //监听道具过期
    m_pItemComponent->AddSubscriber(this);

}

bool CEntityHorseComponent::CreateFromDB(ROLEINFO_DB* p_RoleInfoDB)
{
    return true;
}

void CEntityHorseComponent::RegComponentNetMsgMap()
{
    if (!m_bIsOpen)
    {
        return;
    }

    RegMsgCreatorAndProcessor(GameMsg_C2S_MountHorse , &CEntityHorseComponent::OnMountHorse);
    RegMsgCreatorAndProcessor(GameMsg_C2S_UnmountHorse, &CEntityHorseComponent::OnUnMountHorse);
    RegMsgCreatorAndProcessor(GameMsg_C2S_SendHorseInvite, &CEntityHorseComponent::OnInvitePlayer);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RecvHorseInviteRet, &CEntityHorseComponent::OnRecvInviteRet);

}

void CEntityHorseComponent::OnNotify(IParam &param)
{
    int nHorseTypeId;
    if ( !param.GetParam( "ItemDueTime", nHorseTypeId ) )
    {
        return;
    }

    if(!IsMountSelfHorse())
    {
        return;
    }

    if(m_nCurHorseTypeId == nHorseTypeId)
    {
        UnMountHorse();
        GameMsg_S2C_MountHorseRet retMsg;
        retMsg.m_byFlag = eHorseItemTimeOut;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }
}

void CEntityHorseComponent::OnLogin()
{
    //设置自己坐骑Ownerid
    m_nRoleID = GetComponent<CRoleAttribute>()->m_nRoleID;
}

void CEntityHorseComponent::OnLogout()
{
    UnMountHorse();
}

//////////////////////////////////////////////////////////////////////////
//func
//////////////////////////////////////////////////////////////////////////
void CEntityHorseComponent::UnMountHorse()
{
    //坐在自己的坐骑上 ，坐骑上乘客处理
    if (IsMountSelfHorse()
        && m_MapPassengers.size() > 1)
    {
        for (std::map<int, CHorseSeatInfo>::iterator it = m_MapPassengers.begin();
            it != m_MapPassengers.end(); ++it)
        {
            if (it->second.m_nRoleID == m_nRoleID)
            {
                continue;
            }

            CRoleEntity * pOtherRole = CPlayerManager::Instance().GetEntityByRoleID( it->second.m_nRoleID);
            if (NULL == pOtherRole)
            {
                continue;
            }

            CEntityHorseComponent *pOtherCOM = pOtherRole->GetComponent<CRoleHorse>();
            if (pOtherCOM != NULL)
            {
                pOtherCOM->UnMountFromOtherHorse();
            }
        }
    }

    //坐在自己的坐骑上 ，自己的处理
    if (IsMountSelfHorse() )
    {
        m_nHorseSeatIndex = 0;
        m_nCurHorseTypeId = 0;
        m_nCurHorseOwnerId = 0;
        m_MapPassengers.clear();
    }
    else //在别人坐骑上的处理
    {
        CRoleEntity * pHorseOwnerRole = CPlayerManager::Instance().GetEntityByRoleID(m_nCurHorseOwnerId);
        if (NULL != pHorseOwnerRole)
        {
            CEntityHorseComponent *pHorseOwnerCOM = pHorseOwnerRole->GetComponent<CRoleHorse>();
            if (pHorseOwnerCOM != NULL)
            {
                pHorseOwnerCOM->PassagerLeave(m_nHorseSeatIndex);
            }
        }
        m_nHorseSeatIndex = 0;
        m_nCurHorseTypeId = 0;
        m_nCurHorseOwnerId = 0;
    }

    BroadcastHorse();
}

void CEntityHorseComponent::UnMountFromOtherHorse()
{
    m_nCurHorseOwnerId = 0;
    m_nHorseSeatIndex = 0;
    m_nCurHorseTypeId= 0;
    BroadcastHorse();
}

void CEntityHorseComponent::MountSelfHorse(int HorseType)
{
    m_nCurHorseTypeId = HorseType;
    m_nCurHorseOwnerId = m_nRoleID;
    m_nHorseSeatIndex = 0;

    CHorseSeatInfo info;
    info.m_nRoleID = m_nRoleID;
    info.m_nSeatIndex = 0;
    m_MapPassengers.insert(std::make_pair(m_nHorseSeatIndex ,info) );

    BroadcastHorse();
}

bool CEntityHorseComponent::CheckIsInTransForm()
{
    if (m_pItemComponent == NULL)
    {
        return false;
    }

    if (m_pItemComponent->IsInTransForm())
    {
        return true;
    }
    return false;
}

bool CEntityHorseComponent::CheckIsInDevice()
{
    if (m_pAmuseRoomComponent == NULL)
    {
        return false;
    }

    return m_pAmuseRoomComponent->CheckIsInDevice();
}

bool CEntityHorseComponent::CheckIsSameRoomPlayer(int nInvateRoleID)
{
    if (m_pAmuseRoomComponent->AmuseRoom() != NULL)
    {
        if (NULL == m_pAmuseRoomComponent->AmuseRoom()->GetAmusePlayer(nInvateRoleID) )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (m_pCoupleComponent->WeddingRoom() != NULL)
    {
        if (NULL == m_pCoupleComponent->WeddingRoom()->GetWeddingPlayer(nInvateRoleID) )
        {
            return false;
        } 
        else
        {
            return true;
        }
    }
    else if(m_pDanceGroupComponent->GetCeremonyRoom() != NULL)
    {
        if (NULL == m_pAmuseRoomComponent->AmuseRoom()->GetAmusePlayer(nInvateRoleID) )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

bool CEntityHorseComponent::CheckRoleAllowInvite(int nBeInvitedRoleId)
{
    CRoleEntity * pSendInviter = CPlayerManager::Instance().GetEntityByRoleID(nBeInvitedRoleId);
    if (NULL == pSendInviter)
    {
        return false;
    }

    CEntityHorseComponent *pSendInviterHorseCOM = pSendInviter->GetComponent<CRoleHorse>();
    if (NULL == pSendInviterHorseCOM)
    {
        return false;
    }

    if ( pSendInviterHorseCOM->IsMountHorse() )
    {
        return false;
    }

    return true;
}

bool CEntityHorseComponent::CheckAllowHorse()
{
    if ((m_pRoomComponent->Room() != NULL 
         && !m_pRoomComponent->Room()->HasStart() ) //在大厅等待房间
        ||m_pAmuseRoomComponent->AmuseRoom() != NULL
        ||m_pCoupleComponent->WeddingRoom() != NULL
        ||m_pDanceGroupComponent->GetCeremonyRoom() != NULL
        || (m_pRoomComponent->IsInGroupRoom()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CEntityHorseComponent::CheckRoomAllowInvite()
{
    if( m_pAmuseRoomComponent->AmuseRoom() != NULL
        ||m_pCoupleComponent->WeddingRoom() != NULL
        ||m_pDanceGroupComponent->GetCeremonyRoom() != NULL)
    {
        return true;
    }
    return false;
}

bool CEntityHorseComponent::CheckAllowHorseMorePassager()
{
    if ((int)m_MapPassengers.size() >= CHorseCfgMgr::Instance().GetMaxPassager(m_nCurHorseTypeId) )
    {
        return false;
    }
    return true;
}

bool CEntityHorseComponent::IsMountSelfHorse()
{
    if (m_nCurHorseTypeId == 0)
    {
        return false;
    }

    if (m_nCurHorseOwnerId == m_nRoleID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CEntityHorseComponent::IsMountHorse()
{
    if (m_nCurHorseTypeId == 0)
    {
        return false;
    }
    return true;
}

void CEntityHorseComponent::BroadcastHorse()
{
    GameMsg_S2C_SyncHorseData syncMsg;
    syncMsg.m_nHorseOwnerId = m_nCurHorseOwnerId;
    syncMsg.m_nHorseSeatIdx = m_nHorseSeatIndex;
    syncMsg.m_nHorseTypeId = m_nCurHorseTypeId;
    syncMsg.m_nSelfRoleId = m_nRoleID;
    syncMsg.m_nSceneId = m_nSenceId;

    ((CRoleEntity*)Entity())->SendMsg2GroupServer(&syncMsg); // group sever ，准备广播

    if ( m_pRoomComponent != NULL && m_pRoomComponent->Room() != NULL )
    {
        m_pRoomComponent->Room()->SendMsgToAll( &syncMsg);
    }
    if ( m_pCoupleComponent != NULL && m_pCoupleComponent->WeddingRoom() != NULL )
    {
        m_pCoupleComponent->WeddingRoom()->SendMsgToAll( &syncMsg );
    }
    if ( m_pAmuseRoomComponent != NULL && m_pAmuseRoomComponent->AmuseRoom() != NULL )
    {
         m_pAmuseRoomComponent->AmuseRoom()->SendMsgToAll( &syncMsg );
    }
    if ( m_pDanceGroupComponent != NULL && m_pDanceGroupComponent->GetCeremonyRoom() != NULL )
    {
        m_pDanceGroupComponent->GetCeremonyRoom()->RoomBroadcast( syncMsg );
    }
}

void CEntityHorseComponent::LeaveRoomHorseTask()
{
    UnMountHorse();
}

void CEntityHorseComponent::PassagerLeave(int nSeatIndex)
{
    std::map<int, CHorseSeatInfo>::iterator it = m_MapPassengers.find(nSeatIndex);
    if (it != m_MapPassengers.end())
    {
        m_MapPassengers.erase(it);
    }
}

bool CEntityHorseComponent::AddNewPassager(CEntityHorseComponent* other)
{
    if (!IsMountSelfHorse())
    {
        return false;
    }

    if (!CheckAllowHorseMorePassager())
    {
        return false;
    }

    int maxPassager = CHorseCfgMgr::Instance().GetMaxPassager(m_nCurHorseTypeId);

    bool isfind = false;
    int freeSeatIdx= 0;
    //找到一个空闲位置
    for (int i = 0 ; i < maxPassager ; ++i)
    {
        for (std::map<int, CHorseSeatInfo>::iterator it = m_MapPassengers.begin();
            it != m_MapPassengers.end(); ++it)
        {
            if (it->second.m_nSeatIndex == i)
            {
                isfind = false;
                break;
            }

            isfind = true;
        }

        if (isfind)
        {
            freeSeatIdx = i;
            break;
        }
    }

    CHorseSeatInfo info;
    info.m_nRoleID = other->m_nRoleID;
    info.m_nSeatIndex = freeSeatIdx;
    m_MapPassengers.insert( std::make_pair(freeSeatIdx, info) );

    other->m_nCurHorseOwnerId = m_nRoleID;
    other->m_nCurHorseTypeId = m_nCurHorseTypeId;
    other->m_nHorseSeatIndex = freeSeatIdx;
    other->BroadcastHorse();

    return true;
}

void CEntityHorseComponent::BeInvitedHorseSucTask(CEntityHorseComponent* HorseOwner, CEntityHorseComponent* BeInviter)
{
    if (!HorseOwner->CheckAllowHorseMorePassager())
    {
        return;
    }

    if (!HorseOwner->AddNewPassager(BeInviter) )
    {
        return;
    }
}

//////////////////////////////////////////////////////////////////////////
//msg
//////////////////////////////////////////////////////////////////////////
void CEntityHorseComponent::OnMountHorse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_MountHorse &clientMsg = (GameMsg_C2S_MountHorse&)rMsg;
    std::set<int> allhorseId;

    GameMsg_S2C_MountHorseRet retMsg;

    if (!CheckAllowHorse())
    {
        retMsg.m_byFlag = eHorseUnknow;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }

    m_nSenceId = clientMsg.m_nSceneId;//客户端需要的数据 只做记录
    m_pItemComponent->GetAllHorseTypeId(allhorseId);
    if (allhorseId.find(clientMsg.m_nHorseTypeID) == allhorseId.end())
    {
        retMsg.m_byFlag = eHorseItemTimeOut;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }

    //如果坐在坐骑上 卸下坐骑
    if (IsMountHorse())
    {
        UnMountHorse();
    }

    if (CheckIsInTransForm())
    {
        retMsg.m_byFlag = eHorseInTransForm;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }

    if (CheckIsInDevice())
    {
        retMsg.m_byFlag = eHorseInDevice;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }
    
    int nCurHorseTypeId = CHorseCfgMgr::Instance().GetTypeId(clientMsg.m_nHorseTypeID);
    if (nCurHorseTypeId == 0)
    {
        retMsg.m_byFlag = eHorseUnknow;
        m_pNetComponent->SendPlayerMsg(&retMsg);
        return;
    }
    
    MountSelfHorse(nCurHorseTypeId);
    retMsg.m_byFlag = eHorseSuc;
    m_pNetComponent->SendPlayerMsg(&retMsg);
}

void CEntityHorseComponent::OnUnMountHorse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_UnmountHorse &clientMsg = (GameMsg_C2S_UnmountHorse&)rMsg;
    m_nSenceId = clientMsg.m_nSceneId;

    UnMountHorse();
    GameMsg_S2C_UnmountHorseRet msg;
    msg.m_byFlag = eHorseSuc;
    m_pNetComponent->SendPlayerMsg(&msg);
}

void CEntityHorseComponent::OnInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (!IsMountSelfHorse())
    {
        return;
    }

    //更具配置 检测坐骑是否可以坐更多人
    if ( !CheckAllowHorseMorePassager() )
    {
        GameMsg_S2C_SendHorseInviteRet msg;
        msg.m_byFlag = eInviteHorseFull;
        m_pNetComponent->SendPlayerMsg(&msg);
        return;
    }

    //不允许邀请
    if( !CheckRoomAllowInvite() )
    {
        GameMsg_S2C_SendHorseInviteRet msg;
        msg.m_byFlag = eHorseInviteNotAllow;
        m_pNetComponent->SendPlayerMsg(&msg);
        return;
    }

    if ( CheckIsInTransForm() )
    {
        GameMsg_S2C_SendHorseInviteRet msg;
        msg.m_byFlag = eHorseInTransForm;
        m_pNetComponent->SendPlayerMsg(&msg);
        return;
    }

    if ( CheckIsInDevice() )
    {
        GameMsg_S2C_SendHorseInviteRet msg;
        msg.m_byFlag = eHorseInDevice;
        m_pNetComponent->SendPlayerMsg(&msg);
        return;
    }

    GameMsg_C2S_SendHorseInvite *pClientMsg = (GameMsg_C2S_SendHorseInvite *)&rMsg;
    m_nSenceId = pClientMsg->m_nSceneId;

    //邀请了 不可以邀请的玩家
    for (std::list<unsigned int >::iterator InviterIt = pClientMsg->m_ListInviteID.begin();
        InviterIt != pClientMsg->m_ListInviteID.end(); ++InviterIt)
    {
        if (!CheckIsSameRoomPlayer(*InviterIt))
        {
            GameMsg_S2C_SendHorseInviteRet msg;
            msg.m_byFlag = eHorseInviteLegal;
            m_pNetComponent->SendPlayerMsg(&msg);
            return;
        }

        if (!CheckRoleAllowInvite(*InviterIt))
        {
            GameMsg_S2C_SendHorseInviteRet msg;
            msg.m_byFlag = eHorseInviteNotAllow;
            m_pNetComponent->SendPlayerMsg(&msg);
            return;
        }

        CRoleEntity * pInvite = CPlayerManager::Instance().GetEntityByRoleID( *InviterIt );
        if (NULL == pInvite)
        {
            GameMsg_S2C_SendHorseInviteRet msg;
            msg.m_byFlag = eHorseInviteLegal;
            m_pNetComponent->SendPlayerMsg(&msg);
            return;
        }

        CEntityHorseComponent *pBeInvitedHorseCOM = pInvite->GetComponent<CRoleHorse>();
        if (NULL != pBeInvitedHorseCOM)
        {
            if (pBeInvitedHorseCOM->CheckIsInDevice())
            {
                continue;
            }

            if (pBeInvitedHorseCOM->CheckIsInTransForm())
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        CEntityAttributeComponent *pAttrCOM = GetComponent<CRoleAttribute>();
        if (pAttrCOM == NULL)
        {
            GameMsg_S2C_SendHorseInviteRet msg;
            msg.m_byFlag = eHorseUnknow;
            m_pNetComponent->SendPlayerMsg(&msg);
            return;
        }

        GameMsg_S2C_RecvHorseInvite InviteMsg;
        InviteMsg.m_nInviteSendRoleId = m_nRoleID;
        InviteMsg.m_nHorseType = m_nCurHorseTypeId;
        InviteMsg.m_strInviterName = pAttrCOM->GetRoleName();
        InviteMsg.m_nSceneId = m_nSenceId;
        pInvite->SendPlayerMsg(&InviteMsg);
    }
}

void CEntityHorseComponent::OnRecvInviteRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_RecvHorseInviteRet *pClient = (GameMsg_C2S_RecvHorseInviteRet *)&rMsg;
    m_nSenceId = pClient->m_nSenceId;
    if(pClient->m_bIsAccept)
    {
        if (!CheckIsSameRoomPlayer(pClient->m_nInviteSendRoleId))
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eHorseInviteLeave;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        CRoleEntity * pSendInviter = CPlayerManager::Instance().GetEntityByRoleID( pClient->m_nInviteSendRoleId );
        if (NULL == pSendInviter)
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eHorseInviteLeave;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        CEntityHorseComponent *pSendInviterHorseCOM = pSendInviter->GetComponent<CRoleHorse>();
        if (NULL == pSendInviterHorseCOM )
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eHorseUnknow;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        if (!pSendInviterHorseCOM->IsMountSelfHorse())
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eInviteHorseChange;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        if ((int)pClient->m_nHorseType != pSendInviterHorseCOM->m_nCurHorseTypeId)
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eInviteHorseChange;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        //邀请发起者坐骑是否还可以坐更多的人
        if (!pSendInviterHorseCOM->CheckAllowHorseMorePassager())
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eInviteHorseFull;
            m_pNetComponent->SendPlayerMsg(&msg);
            return ;
        }

        if ( IsMountHorse() )
        {
            GameMsg_S2C_RecvHorseInviteRetRes msg;
            msg.m_nFlag = eHorseInviteHorseing;
            m_pNetComponent->SendPlayerMsg(&msg);
            return;
        }

        GameMsg_S2C_SendHorseInviteRet msg;
        msg.m_byFlag = eHorseInviteAccept;
        pSendInviter->SendPlayerMsg(&msg);

        GameMsg_S2C_RecvHorseInviteRetRes Resmsg;
        Resmsg.m_nFlag = eHorseSuc;
        m_pNetComponent->SendPlayerMsg(&Resmsg);

        BeInvitedHorseSucTask(pSendInviterHorseCOM, this);
    }
}

void CEntityHorseComponent::SyncAllDisplayHorse2Other( CParamPool &IOBuff )
{
    int nCurHorseTypeId;
    int nCurHorseOwnerId;
    int nHorseSeatIndex;
    GetHorseInfo(nCurHorseTypeId, nCurHorseOwnerId, nHorseSeatIndex);

    IOBuff.AddInt(nCurHorseTypeId);
    IOBuff.AddInt(nCurHorseOwnerId);
    IOBuff.AddInt(nHorseSeatIndex);

    return;
}

void CEntityHorseComponent::GetHorseInfo(int &nCurHorseTypeId, int &nCurHorseOwnerId, int &nHorseSeatIndex)
{
    if (IsMountHorse())
    {
        nCurHorseTypeId = m_nCurHorseTypeId;
        nCurHorseOwnerId = m_nCurHorseOwnerId;
        nHorseSeatIndex = m_nHorseSeatIndex;
    }
    else
    {
        nCurHorseTypeId = 0;
        nCurHorseOwnerId = 0;
        nHorseSeatIndex = 0;
    }
}
