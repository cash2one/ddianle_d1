#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseRoomManager.h"
#include "AmuseActionBase.h"
#include "AmuseRoomDataManager.h"
#include "AmuseEvent_AddExp.h"
#include "GameMsg_C2S_AmuseMsg.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../room/RoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../Pet/PetComponent.h"
#include "GameMsg_TimeEggMsg.h"
#include "AmuseAction_CreateTimeEgg.h"
#include "AmuseAction_OpenTimeEgg.h"
#include "AmuseEnum.h"
#include "../mall/GameMsg_S2G_TimeEgg.h"
#include "../mall/GameMsg_G2S_TimeEgg.h"
#include "../logic/LogicCirculator.h"
#include "../../datastructure/LogStructDef.h"
#include "GameMsg_ConstellationMatch.h"
#include "ConstellationMatchManager.h"
#include "ConstellationMatchDataManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../vip/VIPStruct.h"
#include "../vip/VIPDataMgr.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"
#include "../mall/EntityMallComponent.h"

#include <sstream>


#define AMUSEPLAYER_EVENT_STEP 1000
#define AMUSEPLAYER_INVITE_STEP 2000

#define AMUSEPLAYER_INVITE_DURATION 45000

#define CONSTELLATION_MATCH_ROUND_STEP 1500


CAmuseRoomComponent::CAmuseRoomComponent(void)
    : CEntityComponent(true, true)
    , m_bIsHost(false)
    , m_pAmuseRoom(NULL)
    , m_nRoomColor(0)
    , m_bRoomTop(false)
    , m_lRefreshInviteeTime(0)
    , m_lLastInviteTime(0)
    , m_lInterElapse(0)
    , m_lInviteStep(0)
    , m_lEventStep(0)
    , m_nEnterTime(0)
    , m_bHasEnterConstellationLobby(false)
    , m_nConstellationMatchCount(0)
    , m_nConstellationMatchTime(0)
    , m_lMatchRoundStep(0)
    , m_bConstellationMatch(false)
    , m_nMatchConstellation(-1)
    , m_pMatchCard(NULL)
    , m_nConstellationRoomID(0)
    , m_nConstellationRoomCreateTime(0)
    , m_nConstellationRoomLine(0)
    , m_pRoleNetCOM(NULL)
    , m_pRoleAttrCOM(NULL)
    , m_pRoleInfoCOM(NULL)
    , m_pRoleFriendCOM(NULL)
    , m_pRoleItemCOM(NULL)
    , m_pRoleGeneCOM(NULL)
    , m_pRoleVipCOM(NULL)
    , m_pRoleDanceGroupCOM(NULL)
    , m_pRoleStageRoomCOM(NULL)
    , m_pRoleCoupleCOM(NULL)
    , m_pRolePhotoRoomCOM(NULL)
    , m_pRolePet(NULL)
    , m_pRoleMedal( NULL )
    , m_pRoleHorse( NULL )
    , m_pRoleMall(NULL)
{
}

CAmuseRoomComponent::~CAmuseRoomComponent(void)
{
	for (std::map<int, TimeEggEntry*>::iterator it = m_mapEgg.begin();
		it != m_mapEgg.end(); ++it)
	{
		delete it->second;
	}
	m_mapEgg.clear();
} 

void CAmuseRoomComponent::Start()
{
	m_pRoleNetCOM = GetComponent<CRoleNet>();
	m_pRoleAttrCOM = GetComponent<CRoleAttribute>();
	m_pRoleInfoCOM = GetComponent<CPlayerInfo>();
	m_pRoleFriendCOM = GetComponent<CRoleFriend>();
	m_pRoleItemCOM = GetComponent<CRoleItem>();
	m_pRoleGeneCOM = GetComponent<CRoleGene>();
	m_pRoleVipCOM = GetComponent<CRoleVIP>();
	m_pRoleDanceGroupCOM = GetComponent<CRoleDanceGroup>();
	m_pRoleStageRoomCOM = GetComponent<CRoleRoom>();
	m_pRoleCoupleCOM = GetComponent<CRoleCouple>();
	m_pRolePhotoRoomCOM = GetComponent<CRolePhotoRoom>();
	m_pRolePet        = GetComponent<CRolePet>();
    m_pRoleMedal = GetComponent<CRoleMedal>();
    m_pRoleHorse = GetComponent<CRoleHorse>();
    m_pRoleMall = GetComponent<CRoleMall>();
}

void CAmuseRoomComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetAmuseRoomList );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_CreateAmuseRoom );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_TryEnterAmuseRoom );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_EnterAmuseRoom );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_QuitAmuseRoom );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_ChangeAmuseRoomInfo );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_AmuseKickPlayer );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_AmusePromoteHost );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_AmuseInvitePlayer );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_AmuseInviteeResponse );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_DoAmuseAction );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_AmuseApplyMatch );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_EnterConstellationLobby );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_QuitConstellationLobby );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_ConstellationMatch );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_EnterConstellationRoom );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_SendQuestionAnswer );

	RegMsgProcessor( MSG_C2S_GetAmuseRoomList, &CAmuseRoomComponent::OnGetAmuseRoomList );
	RegMsgProcessor( MSG_C2S_CreateAmuseRoom, &CAmuseRoomComponent::OnCreateAmuseRoom );
	RegMsgProcessor( MSG_C2S_TryEnterAmuseRoom, &CAmuseRoomComponent::OnTryEnterAmuseRoom );
	RegMsgProcessor( MSG_C2S_EnterAmuseRoom, &CAmuseRoomComponent::OnEnterAmuseRoom );
	RegMsgProcessor( MSG_C2S_QuitAmuseRoom, &CAmuseRoomComponent::OnQuitAmuseRoom );
	RegMsgProcessor( MSG_C2S_ChangeAmuseRoomInfo, &CAmuseRoomComponent::OnChangeAmuseRoomInfo );
	RegMsgProcessor( MSG_C2S_AmuseKickPlayer, &CAmuseRoomComponent::OnAmuseKickPlayer );
	RegMsgProcessor( MSG_C2S_AmusePromoteHost, &CAmuseRoomComponent::OnAmusePromoteHost );
	RegMsgProcessor( MSG_C2S_AmuseInvitePlayer, &CAmuseRoomComponent::OnAmuseInvitePlayer );
	RegMsgProcessor( MSG_C2S_AmuseInviteeResponse, &CAmuseRoomComponent::OnAmuseInviteeResponse );
	RegMsgProcessor( MSG_C2S_DoAmuseAction, &CAmuseRoomComponent::OnDoAmuseAction );
	RegMsgProcessor( MSG_C2S_AmuseApplyMatch, &CAmuseRoomComponent::OnAmuseApplyMatch );

	RegMsgCreatorAndProcessor(GameMsg_C2S_QueryTimeEggReward, &CAmuseRoomComponent::OnQueryTimeEggReward);
	RegMsgCreatorAndProcessor(GameMsg_G2S_OpenTimeEgg, &CAmuseRoomComponent::OnAutoOpenEgg);
	RegMsgProcessor( MSG_C2S_EnterConstellationLobby, &CAmuseRoomComponent::OnEnterConstellationLobby );
	RegMsgProcessor( MSG_C2S_QuitConstellationLobby, &CAmuseRoomComponent::OnQuitConstellationLobby );
	RegMsgProcessor( MSG_C2S_ConstellationMatch, &CAmuseRoomComponent::OnConstellationMatch );
	RegMsgProcessor( MSG_C2S_EnterConstellationRoom, &CAmuseRoomComponent::OnEnterConstellationRoom );
	RegMsgProcessor( MSG_C2S_SendQuestionAnswer, &CAmuseRoomComponent::OnSendQuestionAnswer );

	RegQueryProcessor(Query_CreateTimeEgg, &CAmuseRoomComponent::OnDBCreateTimeEgg);
	RegQueryProcessor(Query_GetTimeEggsByRoleID, &CAmuseRoomComponent::OnDBGetTimeEggsByRoleID);
}

bool CAmuseRoomComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
	unsigned int nNow = (unsigned int)time(NULL);
	m_bHasEnterConstellationLobby = pRoleInforDB->m_bHasEnterConstellationLobby;
	for (std::list<SConstellationCardInfo>::iterator it = pRoleInforDB->m_listCard.begin();
		it != pRoleInforDB->m_listCard.end(); ++it)
	{
		m_mapMatchCard[it->m_nIndex] = *it;
		if (nNow >= it->m_nCoolDownEndTime)
		{
			m_mapMatchCard[it->m_nIndex].m_nCoolDownTime = 0;
			m_mapMatchCard[it->m_nIndex].m_nCoolDownEndTime = 0;
		}
		else
		{
			m_mapMatchCard[it->m_nIndex].m_nCoolDownTime = 0;
		}
	}
	AddRandomCard();

	m_nConstellationMatchCount = pRoleInforDB->m_nConstellationMatchCount;
	m_nConstellationMatchTime = pRoleInforDB->m_nConstellationMatchTime;
	
	if (!IsCurrentRoundMatchTime(m_nConstellationMatchTime, nNow))
	{
		m_nConstellationMatchCount = 0;
	}

	m_nConstellationRoomID = pRoleInforDB->m_nCosntellationRoomID;
	m_nConstellationRoomCreateTime = pRoleInforDB->m_nCosntellationRoomCreateTime;
	m_nConstellationRoomLine = pRoleInforDB->m_nCosntellationRoomLine;

	return true;
}

bool CAmuseRoomComponent::IsCurrentRoundMatchTime(unsigned int nMatchTime, unsigned int nNow)
{
	return ConstellationMatchDataManager::Instance().IsInCurrentOpenTime(nMatchTime, nNow);
}

void CAmuseRoomComponent::OnUpdate(const unsigned long& lTimeElapsed)
{
	m_lInterElapse += lTimeElapsed;
	m_lInviteStep += lTimeElapsed;
	m_lEventStep += lTimeElapsed;

	if ( m_lInviteStep >= AMUSEPLAYER_INVITE_STEP )
	{
		m_lInviteStep -= AMUSEPLAYER_INVITE_STEP;

		for ( AmuseInviteInfoMap::iterator it = m_mapAmuseInvited.begin(); it != m_mapAmuseInvited.end(); )
		{
			it->second.m_lInviteLeftTime -= lTimeElapsed;

			if ( it->second.m_lInviteLeftTime <= 0 )
			{
				m_mapAmuseInvited.erase( it++ );
			}
			else
			{
				++it;
			}
		}
	}

	if ( m_lEventStep >= AMUSEPLAYER_EVENT_STEP )
	{
		m_lEventStep -= AMUSEPLAYER_EVENT_STEP;

		while ( !m_AllPlayerEvents.empty() )
		{
			AmusePlayerEventMap::iterator it = m_AllPlayerEvents.begin();
			unsigned long lStartTime = it->first;

			if ( lStartTime <= m_lInterElapse )
			{
				CAmuseEvent_Base* pAmuseEvent = it->second;
				m_AllPlayerEvents.erase( it );

				if ( pAmuseEvent != NULL )
				{
					if ( pAmuseEvent->IsLoop() || !pAmuseEvent->EverStart() )
					{
						int nNowTime = (int)time( NULL );
						pAmuseEvent->GoStart( nNowTime );

						lStartTime += pAmuseEvent->EventTime();
						m_AllPlayerEvents.insert( std::make_pair( lStartTime, pAmuseEvent ) );
					}
					else
					{
						pAmuseEvent->GoEnd();

						delete pAmuseEvent;
						pAmuseEvent = NULL;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	m_lMatchRoundStep += lTimeElapsed;
	if (m_lMatchRoundStep >= CONSTELLATION_MATCH_ROUND_STEP)
	{
		if (!ConstellationMatchDataManager::Instance().IsOpen())
		{
			m_mapMatchCard.clear();
			m_nConstellationMatchCount = 0;
			m_allSentQuestion.clear();
		}
		m_lMatchRoundStep -= CONSTELLATION_MATCH_ROUND_STEP;
	}
}

void CAmuseRoomComponent::ShuffleCard()
{
	if (m_mapMatchCard.empty() && ConstellationMatchDataManager::Instance().IsOpen())
	{
		std::vector<int> vecMatchConstellation;
		ConstellationMatchDataManager::Instance().RandomMatchConstellation(m_pRoleAttrCOM->m_nConstellation, vecMatchConstellation);
		for (size_t i = 0; i < vecMatchConstellation.size(); i++)
		{
			SConstellationCardInfo card;
			card.m_nIndex = (unsigned short)i;
			card.m_nConstellation = (unsigned char)vecMatchConstellation[i];

			m_mapMatchCard[(unsigned short)i] = card;

			int *pIndex = new int;
			unsigned int *pCoolDownEndTime = new unsigned int;
			*pIndex = card.m_nIndex;
			*pCoolDownEndTime = 0;
			AddQuery(Query_Constellation_UpdateCardInfo, m_pRoleAttrCOM->GetRoleID(),
				(void *)pIndex, card.m_nConstellation, (void *)pCoolDownEndTime);
		}

		// random card
		AddRandomCard();
	}
}

void CAmuseRoomComponent::AddRandomCard()
{
	if (!m_mapMatchCard.empty())
	{
		SConstellationCardInfo card;
		card.m_nIndex = (unsigned short)m_mapMatchCard.size();
		card.m_nConstellation = MAX_CONSTELLATION_COUNT;
		m_mapMatchCard[card.m_nIndex] = card;
	}
}

void CAmuseRoomComponent::OnLogin()
{
	QueryRoleTimeEggEntry* pInfo = new QueryRoleTimeEggEntry();
	pInfo->m_nRoleID = m_pRoleAttrCOM->GetRoleID();
	CDelHelper delHelper(pInfo);
	AddQuery(Query_GetTimeEggsByRoleID, 0, pInfo, 0, NULL, &delHelper);

	ConstellationMatchManager::Instance().OnLogin(*this);
}

void CAmuseRoomComponent::OnLogout()
{
	CAmuseRoomManager::Instance().OnAmusePlayerLogout( *this );
}

void CAmuseRoomComponent::SendPlayerMsg(GameMsg_Base* pMsg)
{
	if ( m_pRoleNetCOM != NULL )
	{
		m_pRoleNetCOM->SendPlayerMsg( pMsg );
	}
}

unsigned int CAmuseRoomComponent::RoleID() const
{
	return m_pRoleAttrCOM->GetRoleID();
}

CAmuseRoom* CAmuseRoomComponent::AmuseRoom() const
{
	return m_pAmuseRoom;
}

unsigned int CAmuseRoomComponent::AmuseRoomID() const
{
	int nAmuseRoomID = 0;
	if ( m_pAmuseRoom != NULL )
	{
		nAmuseRoomID = m_pAmuseRoom->RoomID();
	}

	return nAmuseRoomID;
}

void CAmuseRoomComponent::SetHost(bool bHost)
{
	m_bIsHost = bHost;
}

bool CAmuseRoomComponent::IsHost() const
{
	return m_bIsHost;
}

void CAmuseRoomComponent::InAmuseRoom(CAmuseRoom* pAmuseRoom, bool bHost)
{
	m_bIsHost = bHost;
	m_pAmuseRoom = pAmuseRoom;
	m_nEnterTime = (unsigned int)time(NULL);

	AmuseInvitedOver();
	_LoadAmusePlayerEvent();
	_LogRoleAmuse(CRoleAmuseLog::EAction_Enter, pAmuseRoom->RoomScene(), 0);
}

void CAmuseRoomComponent::OutAmuseRoom()
{
	unsigned int nExitTime = (unsigned int)time(NULL);
	unsigned int nStayTime = nExitTime - m_nEnterTime;

	AmuseActionOver();
	_ReleaseAmusePlayerEvent();
	_LogRoleAmuse(CRoleAmuseLog::EAction_Exit, m_pAmuseRoom->RoomScene(), nStayTime);

	m_bIsHost = false;
	m_pAmuseRoom = NULL;

    m_pRoleHorse->LeaveRoomHorseTask();
}

void CAmuseRoomComponent::SetRoomColor(int nColor)
{
	//if ( nColor >= RoomColor_None && nColor < RoomColor_Max )
	//{
		m_nRoomColor = nColor;
	//}
}

int CAmuseRoomComponent::GetRoomColor()
{
	return m_nRoomColor;	
}

void CAmuseRoomComponent::SetRoomTop(bool bTop)
{
	m_bRoomTop = bTop;
}

bool CAmuseRoomComponent::GetRoomTop()
{
	return m_bRoomTop;
}

unsigned long CAmuseRoomComponent::RefreshInviteeTime() const
{
	return m_lRefreshInviteeTime;
}

void CAmuseRoomComponent::SetRefreshInviteeTime(unsigned long lTime)
{
	m_lRefreshInviteeTime = lTime;
}

unsigned long CAmuseRoomComponent::LastInviteTime() const
{
	return m_lLastInviteTime;
}

void CAmuseRoomComponent::SetLastInviteTime(unsigned long lTime)
{
	m_lLastInviteTime = lTime;
}

bool CAmuseRoomComponent::IsAmuseInvited(unsigned int nRoomID, unsigned long lRoomCreateTime) const
{
	AmuseInviteInfoMap::const_iterator it = m_mapAmuseInvited.find( nRoomID );
	if ( it != m_mapAmuseInvited.end() && it->second.m_lRoomCreateTime == lRoomCreateTime )
	{
		return true;
	}

	return false;
}

bool CAmuseRoomComponent::RecvAmuseInvited(unsigned int nRoomID, unsigned long lRoomCreateTime)
{
	bool bRes = false;

	if ( m_pRoleInfoCOM->AcceptRoomInvitation() )
	{
		AmuseInviteInfoMap::iterator it = m_mapAmuseInvited.find( nRoomID );
		if ( it != m_mapAmuseInvited.end() )
		{
			if ( it->second.m_lRoomCreateTime != lRoomCreateTime )
			{
				it->second.m_lRoomCreateTime = lRoomCreateTime;
				it->second.m_lInviteLeftTime = AMUSEPLAYER_INVITE_DURATION;

				bRes = true;
			}
		}
		else
		{
			SInvitedAmuseRoomInfo invitedInfo( nRoomID, lRoomCreateTime, AMUSEPLAYER_INVITE_DURATION );
			m_mapAmuseInvited.insert( std::make_pair( nRoomID, invitedInfo ) );

			bRes = true;
		}
	}

	return bRes;
}


void CAmuseRoomComponent::AmuseInvitedOver()
{
	m_mapAmuseInvited.clear();
}

void CAmuseRoomComponent::AmuseInvitedOver(unsigned int nAmuseRoomID)
{
	m_mapAmuseInvited.erase( nAmuseRoomID );
}

void CAmuseRoomComponent::DoAmuseAction(CAmuseAction_Base* pAmuseAction)
{
	if ( pAmuseAction != NULL )
	{
		std::list<CAmuseAction_Base*> listNewActions;
		if ( pAmuseAction->GoProcess( listNewActions ) )
		{
			AddAmuseAction( pAmuseAction );
		}
		else
		{
			pAmuseAction->GoTerminate();

			delete pAmuseAction;
			pAmuseAction = NULL;
		}

		for ( std::list<CAmuseAction_Base*>::iterator it = listNewActions.begin(); it != listNewActions.end(); ++it )
		{
			(*it)->ActPlayer()->DoAmuseAction( *it );
		}
	}
}

AmuseActionByTypeMap& CAmuseRoomComponent::AllAmuseActions()
{
	return m_mapAmuseActions;
}

CAmuseAction_Base* CAmuseRoomComponent::UniqueAmuseAction(int nActionType)
{
	CAmuseAction_Base* pAmuseAction = NULL;

	AmuseActionByTypeMap::iterator it_Type = m_mapAmuseActions.find( nActionType );
	if ( it_Type != m_mapAmuseActions.end() )
	{
		if ( it_Type->second.size() == 1 )
		{
			AmuseActionByIDMap::iterator it_Action = it_Type->second.begin();
			pAmuseAction = it_Action->second;
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Action not unique, type: %d", nActionType );
		}
	}

	return pAmuseAction;
}

void CAmuseRoomComponent::AddAmuseAction(CAmuseAction_Base* pAmuseAction)
{
	if ( pAmuseAction != NULL )
	{
		AmuseActionByTypeMap::iterator it_Type = m_mapAmuseActions.find( pAmuseAction->ActionType() );
		if ( it_Type != m_mapAmuseActions.end() )
		{
			if ( pAmuseAction->IsUnique() )
			{
				for ( AmuseActionByIDMap::iterator it_Action = it_Type->second.begin(); it_Action != it_Type->second.end(); ++it_Action )
				{
					it_Action->second->GoTerminate();

					delete it_Action->second;
					it_Action->second = NULL;
				}

				it_Type->second.clear();
			}

			it_Type->second.insert( std::make_pair( pAmuseAction->ActionID(), pAmuseAction ) );
		}
		else
		{
			AmuseActionByIDMap singleActions;
			singleActions.insert( std::make_pair( pAmuseAction->ActionID(), pAmuseAction ) );

			m_mapAmuseActions.insert( std::make_pair( pAmuseAction->ActionType(), singleActions ) );
		}
	}
}

void CAmuseRoomComponent::AmuseActionOver(CAmuseAction_Base* pAmuseAction)
{
	if ( pAmuseAction != NULL )
	{
		AmuseActionByTypeMap::iterator it_Type = m_mapAmuseActions.find( pAmuseAction->ActionType() );
		if ( it_Type != m_mapAmuseActions.end() )
		{
			AmuseActionByIDMap::iterator it_Action = it_Type->second.find( pAmuseAction->ActionID() );
			if ( it_Action != it_Type->second.end())
			{
				it_Action->second->GoTerminate();

				delete it_Action->second;
				it_Action->second = NULL;

				it_Type->second.erase( it_Action );
				if ( it_Type->second.empty() )
				{
					m_mapAmuseActions.erase( it_Type );
				}
			}
		}
	}
}

void CAmuseRoomComponent::AmuseActionOver()
{
	for ( AmuseActionByTypeMap::iterator it_Type = m_mapAmuseActions.begin(); it_Type != m_mapAmuseActions.end(); ++it_Type )
	{
		for ( AmuseActionByIDMap::iterator it_Action = it_Type->second.begin(); it_Action != it_Type->second.end(); ++it_Action )
		{
			it_Action->second->GoTerminate();

			delete it_Action->second;
			it_Action->second = NULL;
		}

		it_Type->second.clear();
	}

	m_mapAmuseActions.clear();
}

void CAmuseRoomComponent::EncodePlayerInfo(CAmuseRoomPlayerInfo& rPlayerInfo) const
{
	rPlayerInfo.m_nRoleID = m_pRoleAttrCOM->GetRoleID();
	rPlayerInfo.m_strRoleName = m_pRoleAttrCOM->GetRoleName();
	rPlayerInfo.m_bIsHost = m_bIsHost;
	rPlayerInfo.m_nRoleSex = m_pRoleAttrCOM->GetSex();
	rPlayerInfo.m_nRoleSkin = m_pRoleAttrCOM->GetColor();

	CDanceGroupInfo* pDanceGroupInfo = m_pRoleDanceGroupCOM->GetDanceGroupInfo();
	if ( pDanceGroupInfo != NULL )
	{
		rPlayerInfo.m_strDanceGroupName = pDanceGroupInfo->m_DGBaseInfo.m_strGroupName;
		rPlayerInfo.m_nDanceGroupBadgeId = pDanceGroupInfo->m_DGBaseInfo.m_nBadge;
		rPlayerInfo.m_nDanceGroupEffectId = pDanceGroupInfo->m_DGBaseInfo.m_nEffect;

		CDanceGroupMember* pDanceGroupMember = pDanceGroupInfo->GetGroupMember( m_pRoleAttrCOM->GetRoleID() );
		if ( pDanceGroupMember != NULL )
		{
			rPlayerInfo.m_nDanceGroupTitle = pDanceGroupMember->m_nTitle;
		}
	}

	rPlayerInfo.m_bIsVIP = m_pRoleVipCOM->IsVIP();
	rPlayerInfo.m_nVIPLevel = (unsigned short)m_pRoleVipCOM->VIPLevel();

	rPlayerInfo.m_nMoveType = m_pRoleInfoCOM->m_position.m_nMoveType;
	rPlayerInfo.m_nTransFormID = (unsigned short)m_pRoleAttrCOM->GetTransformId();
	rPlayerInfo.m_fOrient = m_pRoleInfoCOM->m_position.m_fOrient;
	rPlayerInfo.m_fPosX = m_pRoleInfoCOM->m_position.m_fPosX;
	rPlayerInfo.m_fPosY = m_pRoleInfoCOM->m_position.m_fPosY;
	rPlayerInfo.m_fPosZ = m_pRoleInfoCOM->m_position.m_fPosZ;

	rPlayerInfo.m_pItemCOM = m_pRoleItemCOM;
	rPlayerInfo.m_pGeneCOM = m_pRoleGeneCOM;
	rPlayerInfo.m_pPetCOM = m_pRolePet;
    rPlayerInfo.m_pMedalCOM = m_pRoleMedal;
    rPlayerInfo.m_pHorseCOM = m_pRoleHorse;
}

CEntityNetComponent* CAmuseRoomComponent::NetCOM() const
{
	return m_pRoleNetCOM;
}

CEntityAttributeComponent* CAmuseRoomComponent::AttrCOM() const
{
	return m_pRoleAttrCOM;
}

CPlayerInfoComponent* CAmuseRoomComponent::InfoCOM()const
{
	return m_pRoleInfoCOM;
}

CEntityFriendComponent* CAmuseRoomComponent::FriendCOM()const
{
	return m_pRoleFriendCOM;
}

CEntityVIPComponent* CAmuseRoomComponent::VipCOM() const
{
	return m_pRoleVipCOM;
}

CRoomComponent* CAmuseRoomComponent::StageRoomCOM() const
{
	return m_pRoleStageRoomCOM;
}

CEntityCoupleComponent* CAmuseRoomComponent::CoupleCOM() const
{
	return m_pRoleCoupleCOM;
}

CEntityPhotoRoomComponent* CAmuseRoomComponent::PhotoRoomCOM() const
{
	return m_pRolePhotoRoomCOM;
}

CEntityItemComponent* CAmuseRoomComponent::ItemCOM() const
{
	return m_pRoleItemCOM;
}

CEntityDanceGroupComponent *CAmuseRoomComponent::RoleDanceGroup() const
{
    return m_pRoleDanceGroupCOM;
}

CEntityMallComponent * CAmuseRoomComponent::RoleMall() const
{
    return m_pRoleMall;
}

void CAmuseRoomComponent::OnGetAmuseRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseGetRoomList( rMsg, *this );
}

void CAmuseRoomComponent::OnCreateAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseCreateRoom( rMsg, *this );
}

void CAmuseRoomComponent::OnQuitAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseQuitRoom( rMsg, *this );
}

void CAmuseRoomComponent::OnTryEnterAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseTryEnter( rMsg, *this );
}

void CAmuseRoomComponent::OnEnterAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseEnterPlayer( rMsg, *this );
}

void CAmuseRoomComponent::OnAmuseKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseKickPlayer( rMsg, *this );
}

void CAmuseRoomComponent::OnAmusePromoteHost(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmusePromoteHost( rMsg, *this );
}

void CAmuseRoomComponent::OnAmuseInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseInvitePlayer( rMsg, *this );
}

void CAmuseRoomComponent::OnAmuseInviteeResponse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseInviteeResponse( rMsg, *this );
}

void CAmuseRoomComponent::OnChangeAmuseRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnAmuseChangeRoomInfo( rMsg, *this );
}

void CAmuseRoomComponent::OnDoAmuseAction(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	CAmuseRoomManager::Instance().OnDoAmuseAction( rMsg, *this );
}

void CAmuseRoomComponent::UpdateAmusePlayerEvent(unsigned long lDelayTime, CAmuseEvent_Base* pAmuseEvent)
{
	if ( pAmuseEvent != NULL )
	{
		AmusePlayerEventMap::iterator it = m_AllPlayerEvents.begin();
		while ( it != m_AllPlayerEvents.end() )
		{
			CAmuseEvent_Base* pCurEvent = it->second;
			if ( pCurEvent->EventType() == pAmuseEvent->EventType() )
			{
				m_AllPlayerEvents.erase( it );

				delete pCurEvent;
				pCurEvent = NULL;

				break;
			}

			++it;
		}

		unsigned long lFirstTime = m_lInterElapse + lDelayTime;
		m_AllPlayerEvents.insert( std::make_pair( lFirstTime, pAmuseEvent ) );
	}
}

void CAmuseRoomComponent::_LoadAmusePlayerEvent()
{
	CAmuseExpData expData;
	if ( CAmuseRoomDataManager::Instance().GetExpData( m_pAmuseRoom->RoomScene(), expData ) )
	{
		CAmuseEvent_AddExp* pAddExpEvent = new CAmuseEvent_AddExp( this, expData.m_bIsLoop, expData.m_nInterval, expData.m_nAddExp );
		UpdateAmusePlayerEvent( pAddExpEvent->EventTime(), pAddExpEvent );
	}
}

void CAmuseRoomComponent::_ReleaseAmusePlayerEvent()
{
	for ( AmusePlayerEventMap::iterator it = m_AllPlayerEvents.begin(); it != m_AllPlayerEvents.end(); ++it )
	{
		it->second->GoEnd();

		delete it->second;
		it->second = NULL;
	}

	m_AllPlayerEvents.clear();
}

void CAmuseRoomComponent::OnAmuseApplyMatch( GameMsg_Base& rMsg, CSlotPeer& rSlotPeer )
{
	CAmuseRoomManager::Instance().OnAmuseApplyMatch( rMsg, *this );
}

void CAmuseRoomComponent::_LogRoleAmuse( int nAction, int nSceneID, int nStayTime )
{
	CRoleAmuseLog* pLog = new CRoleAmuseLog();
	pLog->m_nRoleID = AttrCOM()->GetRoleID();
	pLog->m_nAction = (CRoleAmuseLog::EAction)nAction;
	pLog->m_nSceneID = nSceneID;
	pLog->m_nStayTime = nStayTime;
	g_pQueryMgr->AddQuery(QUERY_LogRoleAmuse, 0, pLog);
}


void CAmuseRoomComponent::OnCreateTimeEggSuc( CreateTimeEggEntry* pInfo )
{
	TimeEggEntry* pEgg = new TimeEggEntry;
	*pEgg = pInfo->m_egg;
	for (vector<RoleTimeEggEntry>::iterator it = pInfo->m_roleinfo.begin();
		it != pInfo->m_roleinfo.end(); ++it)
	{
		if (it->m_nRoleID != m_pRoleAttrCOM->GetRoleID())
		{
			pEgg->m_nRoleID = it->m_nRoleID;
			break;
		}
	}
	AddTimeEgg(pEgg, true);
	_LogRoleTimeEgg(RoleID(), CRoleTimeEggLog::ETimeEggAction_Create, pEgg->m_nSceneID);
}

void CAmuseRoomComponent::OnOpenTimeEggSuc( unsigned int nEggID )
{
	bool bFound = false;
	unsigned int nSceneID = 0;
	for (std::map<int, TimeEggEntry*>::iterator it = m_mapEgg.begin();
		it != m_mapEgg.end(); ++it)
	{
		if (it->second->m_nEggID == nEggID)
		{
			nSceneID = it->second->m_nSceneID;
			delete it->second;
			m_mapEgg.erase(it);
			bFound = true;
			break;
		}
	}
	if ( bFound )
	{
		SendSynTimeEggs();
		_LogRoleTimeEgg(RoleID(), CRoleTimeEggLog::ETimeEggAction_Open, nSceneID);
	}
}

void CAmuseRoomComponent::OnDBCreateTimeEgg( QueryBase & refQuery )
{
	CreateTimeEggEntry* pInfo = (CreateTimeEggEntry*)refQuery.m_pAttachment;
	if (pInfo == NULL)
	{
		return;
	}

	if (pInfo->nError == ECreateTimeEggError_OK)
	{
		LOG_DEBUG << "Create time egg success: egg id(" << pInfo->m_egg.m_nEggID << ")" << std::endl;

		GameMsg_S2G_CreateTimeEgg msg2group;
		msg2group.m_nEggID = pInfo->m_egg.m_nEggID;
		SendMsg2GroupServer(&msg2group);

		CAmuseAction_CreateTimeEgg* pAction  = (CAmuseAction_CreateTimeEgg*)UniqueAmuseAction(eAmuseAction_CreateTimeEgg);
		if ( pAction )
		{
			pAction->OnCreateTimeEggReturn(pInfo->nError, pInfo->strError);
		}

		OnCreateTimeEggSuc(pInfo);

		TimeEggEntry* pEgg = GetTimeEggInCurScene();

		if (pEgg != NULL && m_pAmuseRoom != NULL)
		{
			CAmuseRoomComponent* pPartner = m_pAmuseRoom->GetAmusePlayer(pEgg->m_nRoleID);
			if (pPartner != NULL)
			{
				pPartner->OnCreateTimeEggSuc(pInfo);
			}
		}
	}
    else
    {
        LOG_ERROR << "Create time egg error: egg id(" << pInfo->m_egg.m_nEggID 
                  << "), error code(" << pInfo->nError 
                  << "), error string(" << pInfo->strError 
                  << ")" << std::endl;
    }

	delete pInfo;
	pInfo = NULL;
}

void CAmuseRoomComponent::OnDBGetTimeEggsByRoleID( QueryBase & refQuery )
{
	QueryRoleTimeEggEntry* pInfo = (QueryRoleTimeEggEntry*)refQuery.m_pAttachment;
	if (pInfo == NULL)
	{
		return;
	}
	for (vector<TimeEggEntry>::iterator it = pInfo->m_eggs.begin();
		it != pInfo->m_eggs.end(); ++it)
	{
		TimeEggEntry* pEgg = new TimeEggEntry;
		*pEgg = *it;
		AddTimeEgg(pEgg, false);
	}
	SendSynTimeEggs();
	delete pInfo;
	pInfo = NULL;
}

TimeEggEntry* CAmuseRoomComponent::GetTimeEggInCurScene()
{
	TimeEggEntry* pEntry = NULL;
	if (m_pAmuseRoom != NULL)
	{
		unsigned int nSceneID = m_pAmuseRoom->RoomScene();
		std::map<int, TimeEggEntry*>::iterator it = m_mapEgg.find(nSceneID);
		if (it != m_mapEgg.end())
		{
			pEntry = it->second;
		}
	}
	return pEntry;
}

void CAmuseRoomComponent::AddTimeEgg( TimeEggEntry* pEgg, bool bSyn )
{
	m_mapEgg[pEgg->m_nSceneID] = pEgg;

	if (bSyn)
	{
		SendSynTimeEggs();
	}
}

void CAmuseRoomComponent::SendSynTimeEggs()
{
	std::map<unsigned int, TimeEggEntry*> mapEggOrderByTime;
	for (std::map<int, TimeEggEntry*>::iterator it = m_mapEgg.begin();
		it != m_mapEgg.end(); ++it)
	{
		mapEggOrderByTime[it->second->m_nBeginTime] = it->second;
	}
	GameMsg_S2C_SynRoleTimeEggInfo msg;
	for (std::map<int, TimeEggEntry*>::iterator it = m_mapEgg.begin();
		it != m_mapEgg.end(); ++it)
	{
		msg.m_eggs.push_back(it->second);
	}
	TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
	msg.m_strHelpTitle = setting.m_strHelpTitle;
	msg.m_strHelpContent = setting.m_strHelpContent;
	SendPlayerMsg(&msg);
}

EggInviteType CAmuseRoomComponent::GetInviteTypeInCurScene()
{
	EggInviteType type = EggInviteType_Create;
	TimeEggEntry* egg = GetTimeEggInCurScene();
	if (egg)
	{
		type = EggInviteType_Open;
	}
	return type;
}

bool CAmuseRoomComponent::CheckInviteTypeInCurScene( EggInviteType type, unsigned int nInviteeID, string& strError )
{
	TimeEggEntry* egg = GetTimeEggInCurScene();
	if (type == EggInviteType_Create)
	{
		if (egg != NULL)
		{
			strError = "DoAmuseAction_AlreadyExistEgg";
			return false;
		}
	}
	else if (type == EggInviteType_Open)
	{
		if (egg == NULL)
		{
			strError = "DoAmuseAction_NotExistEgg";
			return false;
		}
		unsigned int nNow = (unsigned int)time(NULL);
		if (egg->m_nEndTime > nNow)
		{
			strError = "DoAmuseAction_OpenTimeEggNotTime";
			return false;
		}
		else if (egg->m_nRoleID != nInviteeID)
		{
			strError = "DoAmuseAction_NotRightPartner";
			return false;
		}
	}
	return true;
}

bool CAmuseRoomComponent::CheckIsInDevice()
{
    if (m_pAmuseRoom == NULL)
    {
        return false;
    }

    return m_pAmuseRoom->CheckIsInDevice(m_pRoleAttrCOM->GetRoleID() );
}

void CAmuseRoomComponent::OnQueryTimeEggReward( GameMsg_Base& rMsg, CSlotPeer& rSlotPeer )
{
	ESexType sex = (ESexType)AttrCOM()->GetSex();
	GameMsg_S2C_QueryTimeEggReward msg;
	TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
	if ( sex == ESexType_Male )
	{
		msg.m_listTogetherReward = setting.m_listTogetherMaleItem;
		msg.m_listReward = setting.m_listMaleItem;
	}
	else if ( sex == ESexType_Female )
	{
		msg.m_listTogetherReward = setting.m_listTogetherFemaleItem;
		msg.m_listReward = setting.m_listFemaleItem;
	}
	msg.m_nFixedMoneyReward = setting.m_nFixedMoneyReward;
	msg.m_nExtraMoneyReward = setting.m_nExtraMoneyReward;
	SendPlayerMsg(&msg);
}

void CAmuseRoomComponent::OnAutoOpenEgg( GameMsg_Base& rMsg, CSlotPeer& rSlotPeer )
{
	GameMsg_G2S_OpenTimeEgg& msg = (GameMsg_G2S_OpenTimeEgg&)rMsg;
	OnOpenTimeEggSuc(msg.m_nEggID);
}

void CAmuseRoomComponent::_LogRoleTimeEgg( unsigned int nRoleID, int nAction, int nScene )
{
	CRoleTimeEggLog* pLog = new CRoleTimeEggLog();
	pLog->m_nRoleID = nRoleID;
	pLog->m_eEggAction = (CRoleTimeEggLog::ETimeEggAction)(nAction);
	pLog->m_nSceneID = nScene;
	g_pQueryMgr->AddQuery(QUERY_LogRoleTimeEgg, 0, pLog);
}

void CAmuseRoomComponent::OnEnterConstellationLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	ConstellationMatchManager::Instance().OnEnterConstellationLobby(rMsg, *this);
}

void CAmuseRoomComponent::OnQuitConstellationLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	ConstellationMatchManager::Instance().OnQuitConstellationLobby(rMsg, *this);
}

void CAmuseRoomComponent::OnConstellationMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	ConstellationMatchManager::Instance().OnMatch(rMsg, *this);
}

void CAmuseRoomComponent::OnEnterConstellationRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	ConstellationMatchManager::Instance().OnEnterConstellationRoom(rMsg, *this);
}

void CAmuseRoomComponent::OnSendQuestionAnswer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
	GameMsg_C2S_SendQuestionAnswer *pMsg = (GameMsg_C2S_SendQuestionAnswer *)&rMsg;

	std::vector<ConstellationConfig> vecAllQuestion;
	ConfigManager::Instance().GetConstellationMgr().AllQuestion(vecAllQuestion);

	std::stringstream ss;
	int i = 0;
	ss << "roleid=" << AttrCOM()->GetRoleID();
	for (std::list<unsigned char>::iterator it = pMsg->m_listAnswer.begin(); it != pMsg->m_listAnswer.end(); ++it)
	{
		size_t nQuestion = 0;
		if (nQuestion < m_Question.size())
		{
			nQuestion = m_Question[i++];
		}
		
		if (nQuestion < vecAllQuestion.size())
		{
			ConstellationConfig &question = vecAllQuestion[nQuestion];
			if (*it < question.m_vecAnswer.size()) 
			{
				ss << " question: " << question.m_strQuestion.c_str() 
				   << " answer: " << question.m_vecAnswer[*it].c_str() << "\n";
			}
		}
	}
	WriteLog(LOGLEVEL_DEBUG, ss.str().c_str());
}

bool CAmuseRoomComponent::HasEnterConstellationLobby() const
{
	return m_bHasEnterConstellationLobby;
}

void CAmuseRoomComponent::SetEnterConstellationLobby()
{
	m_bHasEnterConstellationLobby = true;
	AddQuery(Query_Constellation_EnterLobby, m_pRoleAttrCOM->GetRoleID());
}

int CAmuseRoomComponent::MaxConstellationMatchCount() const
{
	int nCount = ConstellationMatchDataManager::Instance().m_nPriorityCardMaxUseTimes;
	if (m_pRoleVipCOM != NULL)
	{
		if (m_pRoleVipCOM->IsVIP())
		{
			const CVIPLevelPrivInfo *pPrivInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(m_pRoleVipCOM->VIPLevel());
			if (pPrivInfo != NULL && pPrivInfo->m_nConstellationMatchCount > 0)
			{
				nCount += pPrivInfo->m_nConstellationMatchCount;
			}
		}
	}
	return nCount;
}

int CAmuseRoomComponent::ConstellationMatchCount() const
{
	return m_nConstellationMatchCount;
}

void CAmuseRoomComponent::EncodeConstellationCardInfo(std::list<SConstellationCardInfo> &listCard)
{
	for (std::map<unsigned short, SConstellationCardInfo>::iterator it = m_mapMatchCard.begin();
		it != m_mapMatchCard.end(); ++it)
	{
		unsigned int nNow = (unsigned int)time(NULL);
		SConstellationCardInfo &card = it->second;
		
		if (card.m_nConstellation < MAX_CONSTELLATION_COUNT)
		{
			if (card.m_nCoolDownEndTime <= nNow)
			{
				card.m_nCoolDownTime = 0;
				card.m_nCoolDownEndTime = 0;
			}
			else
			{
				card.m_nCoolDownTime = nNow - card.m_nCoolDownEndTime;
			}
		}
		listCard.push_back(card);
	}
}

SConstellationCardInfo *CAmuseRoomComponent::GetConstellationCard(unsigned short nIndex)
{
	if (m_mapMatchCard.find(nIndex) != m_mapMatchCard.end())
	{
		SConstellationCardInfo &card = m_mapMatchCard[nIndex];
		return &card;
	}
	return NULL;
}

bool CAmuseRoomComponent::ConstellationMatch(unsigned short nIndex)
{
	bool bRes = false;
	SConstellationCardInfo *pCard = NULL;

	if (m_nConstellationMatchCount < MaxConstellationMatchCount())
	{
		if (m_mapMatchCard.find(nIndex) != m_mapMatchCard.end())
		{
			pCard = &m_mapMatchCard[nIndex];

			unsigned int nNow = (unsigned int)time(NULL);
			if (pCard->m_nConstellation == MAX_CONSTELLATION_COUNT)
			{
				bRes = true;
			}
			else if (pCard->m_nConstellation < MAX_CONSTELLATION_COUNT
				&& pCard->m_nCoolDownEndTime <= nNow)
			{
				bRes = true;
			}
		}
	}

	if (bRes)
	{
		m_nConstellationMatchTime = (unsigned int)time(NULL);
		m_pMatchCard = pCard;

		m_bConstellationMatch = true;
		m_nMatchConstellation = pCard->m_nConstellation;
	}
	else
	{
		m_pMatchCard = NULL;
	}

	return bRes;
}

void CAmuseRoomComponent::OnConstellationMatchSuc()
{
	if (m_pMatchCard != NULL && m_pMatchCard->m_nConstellation != MAX_CONSTELLATION_COUNT)
	{
		m_pMatchCard->m_nCoolDownTime = ConstellationMatchDataManager::Instance().m_nPriorityCardCoolDown;
		m_pMatchCard->m_nCoolDownEndTime = (unsigned int)time(NULL) + m_pMatchCard->m_nCoolDownTime;

		int *pCardIndex = new int;
		unsigned int *pCoolDownEndTime = new unsigned int;
		*pCardIndex = m_pMatchCard->m_nIndex;
		*pCoolDownEndTime = m_pMatchCard->m_nCoolDownEndTime;
		AddQuery(Query_Constellation_UpdateCardInfo, m_pRoleAttrCOM->GetRoleID(),(void *)pCardIndex, 
			m_pMatchCard->m_nConstellation, (void *)pCoolDownEndTime);

		m_pMatchCard = NULL;
	}

	++m_nConstellationMatchCount;
	AddQuery(Query_Constellation_MatchCount, m_pRoleAttrCOM->GetRoleID(), NULL, m_nConstellationMatchCount);
}

void CAmuseRoomComponent::CancelConstellationMatch()
{
	m_bConstellationMatch = false;
	m_nMatchConstellation = -1;
}

int CAmuseRoomComponent::MatchedConstellation() const
{
	return m_nMatchConstellation;
}

unsigned int CAmuseRoomComponent::ConstellationRoomID() const
{
	return m_nConstellationRoomID;
}

unsigned int CAmuseRoomComponent::ConstellationRoomCreateTime() const
{
	return m_nConstellationRoomCreateTime;
}

int CAmuseRoomComponent::ConstellationRoomLine() const
{
	return m_nConstellationRoomLine;
}

void CAmuseRoomComponent::SetConstellationRoom(unsigned int nRoomID, unsigned int nCreateTime, int nLine)
{
	m_nConstellationRoomID = nRoomID;
	m_nConstellationRoomCreateTime = nCreateTime;
	m_nConstellationRoomLine = nLine;

	unsigned int *pRoomID = new unsigned int;
	int *pLine = new int;
	*pRoomID = nRoomID;
	*pLine = nLine;

	AddQuery(Query_Constellation_UpdateMatchRoom, m_pRoleAttrCOM->GetRoleID(), (void *)pRoomID, nCreateTime, (void *)pLine);
}

void CAmuseRoomComponent::RandomQuestion(std::vector<ConstellationConfig> &vecQuestion)
{
	vecQuestion.clear();
	m_Question.clear();

	std::vector<ConstellationConfig> vecAllQuestion;
	ConfigManager::Instance().GetConstellationMgr().AllQuestion(vecAllQuestion);

	std::vector<int> vecRandomQuestion;

	for (size_t i = 0; i < vecAllQuestion.size(); i++)
	{
		if (m_allSentQuestion.find((int)i) == m_allSentQuestion.end())
		{
			vecRandomQuestion.push_back(i);
		}
	}

	const static int kMaxConstellationQuestionCount = 3;
	for (int i = 0; i < kMaxConstellationQuestionCount; i++)
	{
		int nQuestion = 0;
		if (vecRandomQuestion.size() > 0)
		{
			int nIndex = Random(0, (int)vecRandomQuestion.size() - 1);
			nQuestion = vecRandomQuestion[nIndex];
			vecRandomQuestion.erase(vecRandomQuestion.begin() + nIndex);
			m_allSentQuestion.insert(nQuestion);
		}
		else
		{
			int nIndex = Random(0, (int)m_allSentQuestion.size() - 1);
			std::set<int>::iterator it = m_allSentQuestion.begin();
			std::advance(it, nIndex);
			nQuestion = *it;

			WriteLog(LOGLEVEL_WARNING, "[WARNING] Constellation match: not enough question");
		}

		if (nQuestion < (int)vecAllQuestion.size())
		{
			vecQuestion.push_back(vecAllQuestion[nQuestion]);
			m_Question.push_back(nQuestion);
		}
	}
}

//file end

