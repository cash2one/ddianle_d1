#include "EntityPhotoRoomComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../room/RoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Pet/PetComponent.h"
#include "../Medal/EntityMedalComponent.h"
#include "PhotoRoom.h"
#include "PhotoRoomDataStruct.h"
#include "../../socket/GameMsg_Map.h"
#include "PhotoRoomManager.h"
#include "../../socket/GameMsg_Map.h"
#include "PhotoRoomMsgDef.h"
#include "GameMsg_C2S_PhotoRoom.h"
#include "GameMsg_S2C_PhotoRoom.h"
#include "PhotoRoomLuaDataManager.h"


CEntityPhotoRoomComponent::CEntityPhotoRoomComponent()
    : CEntityComponent(true,true)
    , m_pPhotoRoom(NULL)
    , m_bHost(false)
    , m_ePlayerState(EPhotoRoomPlayerState_Free)
    , m_nLastInviteTime(0)
    , m_pRoleAttr(NULL)
    , m_pRoleNet(NULL)
    , m_pRoleItem(NULL)
    , m_pRoleRoom(NULL)
    , m_pRoleCouple(NULL)
    , m_pRoleGene(NULL)
    , m_pRoleDanceGroup(NULL)
    , m_pRoleFriend(NULL)
    , m_pRoleChat(NULL)
    , m_pRoleVIP(NULL)
    , m_pRoleAmuseRoom(NULL)
    , m_pRolePet(NULL)
    , m_pRoleMedal( NULL )
{
}

CEntityPhotoRoomComponent::~CEntityPhotoRoomComponent()
{
}

void CEntityPhotoRoomComponent::Start()
{
	m_pRoleAttr = GetComponent<CRoleAttribute>();
	m_pRoleNet = GetComponent<CRoleNet>();
	m_pRoleItem = GetComponent<CRoleItem>();
	m_pRoleRoom = GetComponent<CRoleRoom>();
	m_pRoleCouple = GetComponent<CRoleCouple>();
	m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
	m_pRoleGene = GetComponent<CRoleGene>();
	m_pRoleFriend = GetComponent<CRoleFriend>();
	m_pRoleChat = GetComponent<CRoleChat>();
	m_pRoleVIP = GetComponent<CRoleVIP>();
	m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
	m_pRolePet        = GetComponent<CRolePet>();
    m_pRoleMedal = GetComponent<CRoleMedal>();
}

void CEntityPhotoRoomComponent::OnEvent( CComponentEvent& refEvent )
{

}

bool CEntityPhotoRoomComponent::CreateFromDB( ROLEINFO_DB* pRoleInforDB )
{
	return true;
}

bool CEntityPhotoRoomComponent::PacketToDB( ROLEINFO_DB *pRoleInforDB ) const
{
	return true;
}

void CEntityPhotoRoomComponent::OnUpdate( const unsigned long &nTimeElapsed )
{
	std::map<unsigned int, CInvitPhotoRoomInfo>::iterator itr = m_mapInvitedRoom.begin(); 
	while (itr != m_mapInvitedRoom.end()) {
		itr->second.m_nTimeRemain -= nTimeElapsed;
		if (itr->second.m_nTimeRemain <= 0) {
			m_mapInvitedRoom.erase(itr++);
		} else {
			++itr;
		}
	}
}

const char* CEntityPhotoRoomComponent::SerializeName()
{
	return "PhotoStudio";
}

void CEntityPhotoRoomComponent::SerializeComponent( CParamPool &IOBuff )
{

}

void CEntityPhotoRoomComponent::OnLogin()
{

}

void CEntityPhotoRoomComponent::OnLogout()
{
	CPhotoRoomManager::Instance().PlayerLogout(*this);
}

void CEntityPhotoRoomComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_TryEnterMyPhotoRoom);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CreatePhotoRoom);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuitPhotoRoom);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ClosePhotoRoom);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InvitePhotoRoomPlayer);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PhotoRoomInviteeResponse);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_KickPhotoRoomPlayer);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangePhotoRoomBG);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangePhotoRoomAction);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangePhotoRoomPlayerState);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangePhotoRoomState);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMyPhotoRoomInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RenewPhotoRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PressPhotoButton);

	RegMsgProcessor(MSG_C2S_TryEnterMyPhotoRoom, &CEntityPhotoRoomComponent::OnTryEnterMyPhotoRoom);
	RegMsgProcessor(MSG_C2S_CreatePhotoRoom, &CEntityPhotoRoomComponent::OnCreatePhotoRoom);
	RegMsgProcessor(MSG_C2S_QuitPhotoRoom, &CEntityPhotoRoomComponent::OnQuitPhotoRoom);
	RegMsgProcessor(MSG_C2S_ClosePhotoRoom, &CEntityPhotoRoomComponent::OnCloseRoom);
	RegMsgProcessor(MSG_C2S_InvitePhotoRoomPlayer, &CEntityPhotoRoomComponent::OnInvitedPhotoRoomPlayer);
	RegMsgProcessor(MSG_C2S_PhotoRoomInviteeResponse, &CEntityPhotoRoomComponent::OnInviteeResponse);
	RegMsgProcessor(MSG_C2S_KickPhotoRoomPlayer, &CEntityPhotoRoomComponent::OnKickPhotoRoomPlayer);
	RegMsgProcessor(MSG_C2S_ChangePhotoRoomBG, &CEntityPhotoRoomComponent::OnChangePhotoRoomBG);
	RegMsgProcessor(MSG_C2S_ChangePhotoRoomAction, &CEntityPhotoRoomComponent::OnChangePhotoRoomAction);
	RegMsgProcessor(MSG_C2S_ChangeRoomPlayerState, &CEntityPhotoRoomComponent::OnChangePhotoRoomPlayerState);
	RegMsgProcessor(MSG_C2S_ChangePhotoRoomState, &CEntityPhotoRoomComponent::OnChangePhotoRoomState);
	RegMsgProcessor(MSG_C2S_GetMyPhotoRoomInfo, &CEntityPhotoRoomComponent::OnGetMyPhotoRoomInfo);
	RegMsgProcessor(MSG_C2S_RenewPhotoRoom, &CEntityPhotoRoomComponent::OnRenewPhotoRoom);
    RegMsgProcessor(MSG_C2S_PressPhotoButtonAction, &CEntityPhotoRoomComponent::OnPressPhotoButton);
}

CEntityAttributeComponent* CEntityPhotoRoomComponent::RoleAttr() const
{
	return m_pRoleAttr;
}

CEntityItemComponent* CEntityPhotoRoomComponent::RoleItem() const
{
	return m_pRoleItem;
}

CRoomComponent* CEntityPhotoRoomComponent::RoleRoom() const
{
	return m_pRoleRoom;
}

CEntityCoupleComponent* CEntityPhotoRoomComponent::RoleCouple() const
{
	return m_pRoleCouple;
}

CEntityFriendComponent* CEntityPhotoRoomComponent::RoleFriend() const
{
	return m_pRoleFriend;
}

CEntityChatComponent* CEntityPhotoRoomComponent::RoleChat() const
{
	return m_pRoleChat;
}

CAmuseRoomComponent* CEntityPhotoRoomComponent::RoleAmuseRoom() const
{
	return m_pRoleAmuseRoom;
}

CEntityDanceGroupComponent  * CEntityPhotoRoomComponent::RoleDanceGroup() const
{
    return m_pRoleDanceGroup;
}

void CEntityPhotoRoomComponent::OnGetMyPhotoRoomInfo(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnGetMyPhotoRoomInfo(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnCreatePhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnCreatePhotoRoom(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnTryEnterMyPhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnTryEnterPhotoRoom(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnQuitPhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnQuitPhotoRoom(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnCloseRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnClosePhotoRoom(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnInvitedPhotoRoomPlayer(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnInvitedPlayer(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnInviteeResponse(GameMsg_Base & rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnInviteeResponse(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnKickPhotoRoomPlayer(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnKickPhotoRoomPlayer(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnChangePhotoRoomPlayerState(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnChangePhotoRoomPlayerState(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnChangePhotoRoomState(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnChangePhotoRoomState(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnChangePhotoRoomBG(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnPlayerChangeRoomBG(rMsg, *this);
}

void CEntityPhotoRoomComponent::OnChangePhotoRoomAction(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnPlayerChangeRoomAction(rMsg, *this);	
}

void CEntityPhotoRoomComponent::OnRenewPhotoRoom(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	CPhotoRoomManager::Instance().OnRenewPhotoRoom(rMsg, *this);	
}

void CEntityPhotoRoomComponent::OnPressPhotoButton(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
    InsertActionLog(CRolePhotoRoomLog::ERolePhotoRoomAction_UseButton);
}

void CEntityPhotoRoomComponent::SetPhotoRoom(CPhotoRoom *pRoom)
{
	m_pPhotoRoom = pRoom;
}

CPhotoRoom *CEntityPhotoRoomComponent::PhotoRoom()
{
	return m_pPhotoRoom;
}

void CEntityPhotoRoomComponent::SetHost(bool bHost)
{
	m_bHost = bHost;
}

bool CEntityPhotoRoomComponent::IsHost()
{
	return m_bHost;
}

void CEntityPhotoRoomComponent::SetPlayerState(EPhotoRoomPlayerState eState)
{
	m_ePlayerState = eState;
}

EPhotoRoomPlayerState CEntityPhotoRoomComponent::PlayerState()
{
	return m_ePlayerState;
}

void CEntityPhotoRoomComponent::SetLastInviteTime( int nInviteTime )
{
	m_nLastInviteTime = nInviteTime;
}

int CEntityPhotoRoomComponent::LastInvitedTime()
{
	return m_nLastInviteTime;
}

void CEntityPhotoRoomComponent::InPhotoRoom(CPhotoRoom *pRoom)
{
	if (pRoom != NULL && m_pRoleAttr != NULL) {
		m_pPhotoRoom = pRoom;
		m_bHost = pRoom->HostID() == m_pRoleAttr->GetRoleID();
		m_ePlayerState = EPhotoRoomPlayerState_Free;
	}
}

void CEntityPhotoRoomComponent::OutPhotoRoom()
{
	m_pPhotoRoom = NULL;
	m_bHost = false;
	m_ePlayerState = EPhotoRoomPlayerState_Free;
}

void CEntityPhotoRoomComponent::EncodePhotoRoomPlayerInfo(CPhotoRoomPlayerInfo & rPhotoRoomPlayerInfo)
{
	if (m_pRoleAttr != NULL && m_pRoleDanceGroup != NULL) {
		rPhotoRoomPlayerInfo.m_nRoleID = m_pRoleAttr->GetRoleID();
		rPhotoRoomPlayerInfo.m_strRoleName = m_pRoleAttr->GetRoleName();
		rPhotoRoomPlayerInfo.m_eRoleSex = m_pRoleAttr->GetSex();
		rPhotoRoomPlayerInfo.m_nRoleSkin = m_pRoleAttr->GetColor();
		rPhotoRoomPlayerInfo.m_bIsHost = m_bHost;
		rPhotoRoomPlayerInfo.m_ePlayerState = m_ePlayerState;
		rPhotoRoomPlayerInfo.m_nTransFormID = (unsigned short)m_pRoleAttr->GetTransformId();
		if (m_pRoleVIP != NULL) {
			rPhotoRoomPlayerInfo.m_bIsVIP = m_pRoleVIP->IsVIP();
			rPhotoRoomPlayerInfo.m_nVIPLevel = (unsigned short)m_pRoleVIP->VIPLevel();
		}
		CDanceGroupInfo *pDanceGroup = m_pRoleDanceGroup->GetDanceGroupInfo();
		if (pDanceGroup != NULL) {
			rPhotoRoomPlayerInfo.m_strDanceGroupName = pDanceGroup->m_DGBaseInfo.m_strGroupName;
			rPhotoRoomPlayerInfo.m_nDanceGroupBadge = pDanceGroup->m_DGBaseInfo.m_nBadge;
			rPhotoRoomPlayerInfo.m_nDanceGroupEffect = pDanceGroup->m_DGBaseInfo.m_nEffect;
			CDanceGroupMember * pDGMember = pDanceGroup->GetGroupMember(m_pRoleAttr->GetRoleID());
			if (pDGMember != NULL) {
				rPhotoRoomPlayerInfo.m_nGroupTitle = (char)pDGMember->m_nTitle;
			}
		}
		rPhotoRoomPlayerInfo.m_pRoleItem = m_pRoleItem;
		rPhotoRoomPlayerInfo.m_pRoleGene = m_pRoleGene;
		rPhotoRoomPlayerInfo.m_pRolePet = m_pRolePet;
        rPhotoRoomPlayerInfo.m_pRoleMedal = m_pRoleMedal;
	}
}

bool CEntityPhotoRoomComponent::TryBeInvited(unsigned int nRoomID, int nRoomCreateTime)
{
	std::map<unsigned int, CInvitPhotoRoomInfo>::iterator it = m_mapInvitedRoom.find( nRoomID );
	if ( it != m_mapInvitedRoom.end() ) {
		if ( it->second.m_nRoomCreatedTime != nRoomCreateTime ) {
			it->second.m_nRoomCreatedTime = nRoomCreateTime;
			it->second.m_nTimeRemain = CPhotoRoomLuaDataManager::Instance().InvitePlayerInterval() * 1000;
		} else {
			return false;
		}
	} else {
		CInvitPhotoRoomInfo invitedInfo( nRoomID, nRoomCreateTime, CPhotoRoomLuaDataManager::Instance().InviteeResponseExpiredTime() * 1000 );
		m_mapInvitedRoom.insert( std::make_pair( nRoomID, invitedInfo ) );
	}

	return true;
}

void CEntityPhotoRoomComponent::InvitedFinished()
{
	m_mapInvitedRoom.clear();
}

void CEntityPhotoRoomComponent::InvitedOver(unsigned int nRoomID)
{

	m_mapInvitedRoom.erase(nRoomID);
}

bool CEntityPhotoRoomComponent::IsInvited(unsigned int nRoomID, int nCreatedTime)
{
	if (nRoomID > 0 && nCreatedTime > 0) {
		for (std::map<unsigned int, CInvitPhotoRoomInfo >::iterator itr = m_mapInvitedRoom.begin(); itr != m_mapInvitedRoom.end(); ++itr) {
			if (itr->second.m_nRoomID == nRoomID && itr->second.m_nRoomCreatedTime == nCreatedTime) {
				return true;
			}
		}
	}

	return false;
}

void CEntityPhotoRoomComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pRoleNet != NULL && pMsg != NULL) {
		m_pRoleNet->SendPlayerMsg(pMsg);
	}
}

void CEntityPhotoRoomComponent::OnPhotoRoomTest(int nPara1, int nPara2, int nPara3)
{
	int nIndex = -1;
	nIndex = nPara1;
	CSlotPeer slotPeer;
	switch (nIndex)
	{
	case 0:
		{
			GameMsg_C2S_CreatePhotoRoom msgCreate;
			msgCreate.m_nRoomTheme = (unsigned char)nPara2;
			CPhotoRoomManager::Instance().OnCreatePhotoRoom( msgCreate, *this );
		}
		break;
	case 1:
		{
			GameMsg_C2S_ClosePhotoRoom msgClose;
			CPhotoRoomManager::Instance().OnClosePhotoRoom(msgClose, *this);
		}
		break;
	case 2:
		{
			GameMsg_C2S_KickPhotoRoomPlayer msgKick;
			msgKick.m_nBeKickedID = 178;
			CPhotoRoomManager::Instance().OnKickPhotoRoomPlayer(msgKick, *this);
		}
	case 3:
		{
			GameMsg_C2S_ChangePhotoRoomBG msgChangeBG;
			msgChangeBG.m_nBGID = (unsigned char)nPara2;
			CPhotoRoomManager::Instance().OnPlayerChangeRoomBG(msgChangeBG, *this);
		}
		break;
	case 4:
		{
			GameMsg_C2S_ChangePhotoRoomAction msgChangeAction;
			msgChangeAction.m_nActionID = (unsigned char)nPara2;
			CPhotoRoomManager::Instance().OnPlayerChangeRoomAction(msgChangeAction, *this);
		}
		break;
	case 5:
		{
			GameMsg_C2S_ChangePhotoRoomPlayerState msgChangPlayerState;
			msgChangPlayerState.m_nState = (unsigned char)nPara2;
			CPhotoRoomManager::Instance().OnChangePhotoRoomPlayerState(msgChangPlayerState, *this);
		}
		break;
	case 6:
		{
			GameMsg_C2S_ChangePhotoRoomState msgChangeRoomState;
			msgChangeRoomState.m_nRoomState = (unsigned char)nPara2;
			CPhotoRoomManager::Instance().OnChangePhotoRoomState(msgChangeRoomState, *this);
		}
		break;
	default:
		break;
	}

}

void CEntityPhotoRoomComponent::InsertActionLog(CRolePhotoRoomLog::EPhotoRoomAction eAction)
{
    CRolePhotoRoomLog * pLog = new CRolePhotoRoomLog();
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_eAction = eAction;

    AddQuery(QUERY_LogRolePhotoRoom, 0, pLog);
}





