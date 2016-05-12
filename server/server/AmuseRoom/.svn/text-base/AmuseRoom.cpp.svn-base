#include "AmuseRoom.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoomDevice.h"
#include "AmuseRoomDataManager.h"
#include "AmuseEnum.h"
#include "AmuseActionBase.h"
#include "AmuseEventBase.h"
#include "AmuseEvent_StartCarrousel.h"
#include "AmuseEvent_StartBoat.h"
#include "../attribute/EntityAttributeComponent.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseRoomManager.h"
#include "../../datastructure/datainfor/GMDataInfo.h"


#define AMUSEROOM_EVENT_STEP 1000


CAmuseRoom::CAmuseRoom(unsigned int nRoomID, int nRoomTheme, int nRoomScene, const std::string& strRoomName, const std::string& strRoomPwd, unsigned long lCreateTime)
:m_nRoomID(nRoomID)
,m_strRoomName(strRoomName)
,m_strRoomPwd(strRoomPwd)
,m_nRoomTheme(nRoomTheme)
,m_nRoomScene(nRoomScene)
,m_nCapacity(0)
,m_lCreateTime(lCreateTime)
,m_lInternalTime(0)
,m_lEventStep(0)
,m_nActionID(0)
,m_nAmuseMaleCount(0)
,m_nAmuseFemaleCount(0)
,m_pAmuseHost(NULL)
{
	_LoadAmuseRoomEvent();
	_LoadAmuseRoomDevice();
}

CAmuseRoom::~CAmuseRoom(void)
{
	for ( AmuseRoomEventMap::iterator it_Event = m_AllAmuseEvents.begin(); it_Event != m_AllAmuseEvents.end(); ++it_Event )
	{
		delete it_Event->second;
		it_Event->second = NULL;
	}

	for ( AmuseRoomDeviceMap::iterator it_Device = m_AllAmuseDevices.begin(); it_Device != m_AllAmuseDevices.end(); ++it_Device )
	{
		delete it_Device->second;
		it_Device->second = NULL;
	}

	m_AllAmuseEvents.clear();
	m_AllAmuseDevices.clear();
}

void CAmuseRoom::OnUpdate(unsigned long lMgrTimeNow, unsigned long lTimeElapsed)
{
	m_lInternalTime+= lTimeElapsed;
	m_lEventStep += lTimeElapsed;

	if ( m_lEventStep >= AMUSEROOM_EVENT_STEP )
	{
		m_lEventStep -= AMUSEROOM_EVENT_STEP;

		while ( !m_AllAmuseEvents.empty() )
		{
			AmuseRoomEventMap::iterator it = m_AllAmuseEvents.begin();
			unsigned long lStartTime = it->first;

			if ( lStartTime <= m_lInternalTime )
			{
				CAmuseEvent_Base* pAmuseEvent = it->second;
				m_AllAmuseEvents.erase( it );

				if ( pAmuseEvent != NULL )
				{
					if ( pAmuseEvent->IsLoop() || !pAmuseEvent->EverStart() )
					{
						int nNowTime = (int)time( NULL );
						pAmuseEvent->GoStart( nNowTime );

						lStartTime += pAmuseEvent->EventTime();
						m_AllAmuseEvents.insert( std::make_pair( lStartTime, pAmuseEvent ) );
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
}

unsigned int CAmuseRoom::RoomID() const
{
	return m_nRoomID;
}

const std::string& CAmuseRoom::RoomName() const
{
	return m_strRoomName;
}

const std::string& CAmuseRoom::RoomPwd() const
{
	return m_strRoomPwd;
}

void CAmuseRoom::ChangeRoomInfo(const char* pRoomName, const char* pRoomPwd)
{
	m_strRoomName.assign( pRoomName );
	m_strRoomPwd.assign( pRoomPwd );
}

int CAmuseRoom::RoomTheme() const
{
	return m_nRoomTheme;
}

int CAmuseRoom::RoomScene() const
{
	return m_nRoomScene;
}

int CAmuseRoom::RoomCapacity() const
{
	return m_nCapacity;
}

void CAmuseRoom::SetRoomCapacity(int nCapcity)
{
	m_nCapacity = nCapcity;
}

bool CAmuseRoom::HasPwd() const
{
	return !m_strRoomPwd.empty();
}

bool CAmuseRoom::PwdPassed(const char* pRoomPwd) const
{
	if ( HasPwd() && pRoomPwd != NULL && m_strRoomPwd.compare( pRoomPwd ) != 0 )
	{
		return false;
	}

	return true;
}

unsigned long CAmuseRoom::CreateTime() const
{
	return m_lCreateTime;
}

int CAmuseRoom::ActionID()
{
	int nAction = m_nActionID;
	++m_nActionID;

	return nAction;
}

CAmuseRoomComponent* CAmuseRoom::AllocHost()
{
	if ( !m_AllAmusePlayerID.empty() )
	{
		unsigned int nHostID = m_AllAmusePlayerID.front();
		m_pAmuseHost = GetAmusePlayer( nHostID );
	}
	else
	{
		m_pAmuseHost = NULL;
	}

	if ( m_pAmuseHost != NULL )
	{
		m_pAmuseHost->SetHost( true );
	}

	return m_pAmuseHost;
}

CAmuseRoomComponent* CAmuseRoom::RoomHost()
{
	return m_pAmuseHost;
}

void CAmuseRoom::AssignHost(CAmuseRoomComponent* pAmuseHost)
{
	if ( m_pAmuseHost != NULL )
	{
		m_pAmuseHost->SetHost( false );
		m_pAmuseHost = NULL;
	}

	if ( pAmuseHost != NULL )
	{
		m_pAmuseHost = pAmuseHost;
		m_pAmuseHost->SetHost( true );
	}
}

int CAmuseRoom::PlayerCount() const
{
	int nPlayerCount = m_AllAmusePlayers.size();
	return nPlayerCount;
}

CAmuseRoomComponent* CAmuseRoom::GetAmusePlayer()
{
	CAmuseRoomComponent* pAmusePlayer = NULL;

	if ( !m_AllAmusePlayers.empty() )
	{
		AmusePlayerMap::iterator it = m_AllAmusePlayers.begin();
		pAmusePlayer = it->second;
	}

	return pAmusePlayer;
}

CAmuseRoomComponent* CAmuseRoom::GetAmusePlayer(unsigned int nPlayerID)
{
	CAmuseRoomComponent* pAmusePlayer = NULL;

	AmusePlayerMap::iterator it = m_AllAmusePlayers.find( nPlayerID );
	if ( it != m_AllAmusePlayers.end() )
	{
		pAmusePlayer = it->second;
	}

	return pAmusePlayer;
}

CAmuseRoomDevice* CAmuseRoom::GetRoomDevice(int nDeviceID)
{
	CAmuseRoomDevice* pAmuseDevice = NULL;

	AmuseRoomDeviceMap::iterator it = m_AllAmuseDevices.find( nDeviceID );
	if ( it != m_AllAmuseDevices.end() )
	{
		pAmuseDevice = it->second;
	}

	return pAmuseDevice;
}

int CAmuseRoom::MaleCount() const
{
	return m_nAmuseMaleCount;
}

int CAmuseRoom::FemaleCount() const
{
	return m_nAmuseFemaleCount;
}

int CAmuseRoom::RoomColor() const
{
	int nRoomColor = 0;

	if ( m_pAmuseHost != NULL )
	{
		nRoomColor = m_pAmuseHost->GetRoomColor();
	}

	return nRoomColor;
}

bool CAmuseRoom::IsTopRoom() const
{
	bool bRoomTop = false;

	if ( m_pAmuseHost != NULL )
	{
		bRoomTop = m_pAmuseHost->GetRoomTop();
	}

	return bRoomTop;
}

bool CAmuseRoom::PlayerIn(CAmuseRoomComponent& rAmusePlayer, bool bHost)
{
	bool bRes = false;

	if ( m_AllAmusePlayers.find( rAmusePlayer.RoleID() ) == m_AllAmusePlayers.end() )
	{
		m_AllAmusePlayers.insert( std::make_pair( rAmusePlayer.RoleID(), &rAmusePlayer ) );
		m_AllAmusePlayerID.push_back( rAmusePlayer.RoleID() );

		rAmusePlayer.InAmuseRoom( this, bHost );

		if ( bHost )
		{
			m_pAmuseHost = &rAmusePlayer;
		}

		if ( rAmusePlayer.AttrCOM()->GetSex() == ESexType_Male )
		{
			++m_nAmuseMaleCount;
		}
		else
		{
			++m_nAmuseFemaleCount;
		}

		bRes = true;
	}

	return bRes;
}

CAmuseRoomComponent* CAmuseRoom::PlayerOut(unsigned int nPlayerID)
{
	CAmuseRoomComponent* pAmusePlayer = NULL;

	if ( nPlayerID > 0 )
	{
		AmusePlayerMap::iterator it_Player = m_AllAmusePlayers.find( nPlayerID );
		if ( it_Player != m_AllAmusePlayers.end() )
		{
			pAmusePlayer = it_Player->second;
			m_AllAmusePlayers.erase( it_Player );

			for ( AmusePlayerIDList::iterator it_ID = m_AllAmusePlayerID.begin(); it_ID != m_AllAmusePlayerID.end(); ++it_ID )
			{
				if ( *it_ID == nPlayerID )
				{
					m_AllAmusePlayerID.erase( it_ID );
					break;
				}
			}
		}

		if ( pAmusePlayer != NULL )
		{
			pAmusePlayer->OutAmuseRoom();

			if ( m_pAmuseHost == pAmusePlayer )
			{
				m_pAmuseHost = NULL;
			}

			if ( pAmusePlayer->AttrCOM()->GetSex() == ESexType_Male )
			{
				--m_nAmuseMaleCount;
			}
			else
			{
				--m_nAmuseFemaleCount;
			}
		}
	}

	return pAmusePlayer;
}

void CAmuseRoom::EncodeBriefInfo(CAmuseRoomBriefInfo& rBriefInfo)
{
	rBriefInfo.m_nRoomID = m_nRoomID;
	rBriefInfo.m_strRoomName = m_strRoomName;
	rBriefInfo.m_bHasPwd = HasPwd();
	rBriefInfo.m_chRoomTheme = (char)RoomTheme();
	rBriefInfo.m_chRoomScene = (char)RoomScene();
	rBriefInfo.m_chRoomColor = (char)RoomColor();
	rBriefInfo.m_bIsTop = IsTopRoom();

	int nPlayerIndex = 0;
	for ( AmusePlayerIDList::iterator it = m_AllAmusePlayerID.begin(); it != m_AllAmusePlayerID.end(); ++it )
	{
		if ( nPlayerIndex < MAX_AMUSEROOM_PLAYER_COUNT )
		{
			CAmuseRoomComponent* pAmusePlayer = GetAmusePlayer( *it );
			if ( pAmusePlayer != NULL )
			{
				rBriefInfo.m_szPlayerSex[nPlayerIndex] = pAmusePlayer->AttrCOM()->GetSex();
			}

			++nPlayerIndex;
		}
		else
		{
			break;
		}
	}
}

void CAmuseRoom::EncodeWholeInfo(CAmuseRoomWholeInfo& rWholeInfo, bool bHost)
{
	int nNowTime = (int)time( NULL );

	rWholeInfo.m_nRoomID = m_nRoomID;
	rWholeInfo.m_strRoomName = m_strRoomName;
	rWholeInfo.m_strRoomPwd = ( bHost ? m_strRoomPwd : "" );
	rWholeInfo.m_bHasPwd = !m_strRoomPwd.empty();
	rWholeInfo.m_bIsHost = bHost;
	rWholeInfo.m_chRoomTheme = (char)RoomTheme();
	rWholeInfo.m_chRoomScene = (char)RoomScene();
	rWholeInfo.m_chRoomCapacity = (char)RoomCapacity();
	if (IsConstellationRoom())
	{
		rWholeInfo.m_chRoomType = eAmuseRoomType_ConstellationRoom;
	}
	else
	{
		rWholeInfo.m_chRoomType = eAmuseRoomType_AmuseRoom;
	}

	AmuseActionByIDMap allRoomActions;

	for ( AmusePlayerIDList::iterator it_ID = m_AllAmusePlayerID.begin(); it_ID != m_AllAmusePlayerID.end(); ++it_ID )
	{
		AmusePlayerMap::iterator it_Player = m_AllAmusePlayers.find( *it_ID );
		if ( it_Player != m_AllAmusePlayers.end() )
		{
			CAmuseRoomPlayerInfo playerInfo;
			it_Player->second->EncodePlayerInfo( playerInfo );

			rWholeInfo.m_lstAmusePlayer.push_back( playerInfo );

			AmuseActionByTypeMap& playerAmuseActions = it_Player->second->AllAmuseActions();
			for ( AmuseActionByTypeMap::iterator it_Type = playerAmuseActions.begin(); it_Type != playerAmuseActions.end(); ++it_Type )
			{
				for ( AmuseActionByIDMap::iterator it_Action = it_Type->second.begin(); it_Action != it_Type->second.end(); ++it_Action )
				{
					if ( it_Action->second != NULL && it_Action->second->NeedSync() )
					{
						allRoomActions.insert( std::make_pair( it_Action->first, it_Action->second ) );
					}
				}
			}
		}
	}

	for ( AmuseRoomDeviceMap::iterator it_Device = m_AllAmuseDevices.begin(); it_Device != m_AllAmuseDevices.end(); ++it_Device )
	{
		CAmuseRoomDeviceInfo deviceInfo;
		it_Device->second->EncodeDeviceInfo( deviceInfo );

		rWholeInfo.m_lstAmuseDevice.push_back( deviceInfo );
	}

	for ( AmuseActionByIDMap::iterator it = allRoomActions.begin(); it != allRoomActions.end(); ++it )
	{
		CAmuseActionParam_Base* pActionParam = it->second->ToParam( nNowTime );
		if ( pActionParam != NULL )
		{
			rWholeInfo.m_lstPlayerAction.push_back( pActionParam );
		}
	}

	for ( AmuseRoomEventMap::iterator it_Event = m_AllAmuseEvents.begin(); it_Event != m_AllAmuseEvents.end(); ++it_Event )
	{
		CAmuseEventParam_Base* pEventParam = it_Event->second->ToParam( nNowTime );
		if ( pEventParam != NULL )
		{
			rWholeInfo.m_lstRoomEvent.push_back( pEventParam );
		}
	}
}

void CAmuseRoom::SendMsgToAll(GameMsg_Base* pMsg, CAmuseRoomComponent* pExceptPlayer)
{
	if ( pExceptPlayer == NULL )
	{
		for ( AmusePlayerMap::iterator it = m_AllAmusePlayers.begin(); it != m_AllAmusePlayers.end(); ++it )
		{
			if ( it->second != NULL )
			{
				it->second->SendPlayerMsg( pMsg );
			}
		}
	}
	else
	{
		for ( AmusePlayerMap::iterator it = m_AllAmusePlayers.begin(); it != m_AllAmusePlayers.end(); ++it )
		{
			if ( it->second != NULL && it->second != pExceptPlayer )
			{
				it->second->SendPlayerMsg( pMsg );
			}
		}
	}
}

void CAmuseRoom::SendMsgToAll( GameMsg_Base* pMsg, const std::set<CAmuseRoomComponent*>& setExceptPlayer )
{
	if ( setExceptPlayer.empty() )
	{
		for ( AmusePlayerMap::iterator it = m_AllAmusePlayers.begin(); it != m_AllAmusePlayers.end(); ++it )
		{
			if ( it->second != NULL )
			{
				it->second->SendPlayerMsg( pMsg );
			}
		}
	}
	else
	{
		for ( AmusePlayerMap::iterator it = m_AllAmusePlayers.begin(); it != m_AllAmusePlayers.end(); ++it )
		{
			if ( it->second != NULL && setExceptPlayer.find(it->second) == setExceptPlayer.end() )
			{
				it->second->SendPlayerMsg( pMsg );
			}
		}
	}
}

void CAmuseRoom::UpdateAmuseRoomEvent(unsigned long lDelayTime, CAmuseEvent_Base* pAmuseEvent)
{
	if ( pAmuseEvent != NULL )
	{
		AmuseRoomEventMap::iterator it = m_AllAmuseEvents.begin();
		while ( it != m_AllAmuseEvents.end() )
		{
			CAmuseEvent_Base* pCurEvent = it->second;
			if ( pCurEvent->EventType() == pAmuseEvent->EventType() )
			{
				m_AllAmuseEvents.erase( it );

				delete pCurEvent;
				pCurEvent = NULL;

				break;
			}

			++it;
		}

		unsigned long lFirstTime = m_lInternalTime + lDelayTime;
		m_AllAmuseEvents.insert( std::make_pair( lFirstTime, pAmuseEvent ) );
	}
}

void CAmuseRoom::_LoadAmuseRoomEvent()
{
	CAmuseCarrouselData carrouselData;
	if ( CAmuseRoomDataManager::Instance().GetCarrouselData( m_nRoomScene, carrouselData ) )
	{
		CAmuseEvent_StartCarrousel* pCarrouselEvent = new CAmuseEvent_StartCarrousel( this, carrouselData.m_bIsLoop, carrouselData.m_nInterval, carrouselData.m_vecStartSong );
		UpdateAmuseRoomEvent( pCarrouselEvent->EventTime(), pCarrouselEvent );
	}

	CAmuseBoatData boatData;
	if ( CAmuseRoomDataManager::Instance().GetBoatData( m_nRoomScene, boatData ) )
	{
		CAmuseEvent_StartBoat* pBoatEvent = new CAmuseEvent_StartBoat( this, boatData.m_bIsLoop, boatData.m_nInterval, boatData.m_nBoatDock );
		UpdateAmuseRoomEvent( pBoatEvent->EventTime(), pBoatEvent );
	}
}

void CAmuseRoom::_LoadAmuseRoomDevice()
{
	AmuseDeviceDataMap sceneDeviceData = CAmuseRoomDataManager::Instance().GetDeviceData( m_nRoomScene );
	for ( AmuseDeviceDataMap::iterator it = sceneDeviceData.begin(); it != sceneDeviceData.end(); ++it )
	{
		CAmuseRoomDevice* pAmuseDevice = new CAmuseRoomDevice( this );
		pAmuseDevice->DecodeDeviceInfo( it->second );
		if (CAmuseRoomDataManager::Instance().GetEggData(m_nRoomScene, pAmuseDevice->DeviceID()))
		{
			pAmuseDevice->SetDeviceType(AmuseRoomDeviceType_TimeEgg);
		}
		m_AllAmuseDevices.insert( std::make_pair( pAmuseDevice->DeviceID(), pAmuseDevice ) );
	}
}

bool CAmuseRoom::IsConstellationRoom() const
{
	return CAmuseRoomManager::Instance().IsConstellationRoom(m_nRoomID);
}

void CAmuseRoom::ToGMRoomInfo(GMRoomInfo& gmRoom)
{
	gmRoom.m_RoomID = (unsigned short)RoomID();
	gmRoom.m_RoomName = RoomName();
	gmRoom.m_RoomPassword = RoomPwd();

	AmusePlayerMap::iterator it = m_AllAmusePlayers.begin();
	for (; it != m_AllAmusePlayers.end() ; it++)
	{
		GMRoomPlayerInfo roomPlayerInfo;
		CAmuseRoomComponent* pRoomCom = it->second;
		roomPlayerInfo.m_playerID = pRoomCom->RoleID();
		roomPlayerInfo.m_playerName = pRoomCom->AttrCOM()->m_szRoleName;
		gmRoom.AddPlayerInfo(roomPlayerInfo);
	}
}

bool CAmuseRoom::CheckIsInDevice(int nRoleId)
{
    for (AmuseRoomDeviceMap::iterator it_Device = m_AllAmuseDevices.begin(); it_Device != m_AllAmuseDevices.end(); ++it_Device)
    {
       if ( it_Device->second->IsInDevice(nRoleId) )
       {
           return true;
       }
    }
    return false;
}

//file end

