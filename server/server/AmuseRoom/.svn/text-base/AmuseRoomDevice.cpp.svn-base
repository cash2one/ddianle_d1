#include "AmuseRoomDevice.h"
#include "AmuseEnum.h"
#include "AmuseRoomDataManager.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseRoom.h"
#include "AmuseRoomComponent.h"

CAmuseRoomDevice::CAmuseRoomDevice(CAmuseRoom* pAmuseRoom)
:m_pOwnerRoom(pAmuseRoom)
,m_nDeviceID(0)
,m_nDeviceCapacity(0)
,m_nInviteType(eAmuseInvite_None)
,m_nDeviceType(AmuseRoomDeviceType_Normal)
{
}

CAmuseRoomDevice::~CAmuseRoomDevice()
{
}

int CAmuseRoomDevice::DeviceID() const
{
	return m_nDeviceID;
}

int CAmuseRoomDevice::InviteType() const
{
	return m_nInviteType;
}

bool CAmuseRoomDevice::ReachCapacity() const
{
	if ( m_setDeviceUsers.size() < (size_t)m_nDeviceCapacity )
	{
		return false;
	}

	return true;
}

bool CAmuseRoomDevice::HasUser() const
{
	if ( m_setDeviceUsers.empty() )
	{
		return false;
	}

	return true;
}

bool CAmuseRoomDevice::HasUser(unsigned int nUserID) const
{
	if ( m_setDeviceUsers.find( nUserID ) == m_setDeviceUsers.end() )
	{
		return false;
	}

	return true;
}

void CAmuseRoomDevice::UserIn(unsigned int nPlayerID)
{
	if ( m_setDeviceUsers.size() < (size_t)m_nDeviceCapacity )
	{
		m_setDeviceUsers.insert( nPlayerID );
	}
}

void CAmuseRoomDevice::UserOut(unsigned int nPlayerID)
{
	m_setDeviceUsers.erase( nPlayerID );
}

void CAmuseRoomDevice::EncodeDeviceInfo(CAmuseRoomDeviceInfo& rDeviceInfo)
{
	rDeviceInfo.m_nDeviceID = (char)m_nDeviceID;
	rDeviceInfo.m_nInviteType = (char)m_nInviteType;
}

void CAmuseRoomDevice::DecodeDeviceInfo(CAmuseDeviceData& rDeviceData)
{
	m_nDeviceID = rDeviceData.m_nDeviceID;
	m_nDeviceCapacity = rDeviceData.m_nCapacity;
	m_nInviteType = rDeviceData.m_nInviteType;
}

void CAmuseRoomDevice::GetPlayers( std::vector<CAmuseRoomComponent*>& players )
{
	for(DeviceUserSet::iterator it = m_setDeviceUsers.begin(); 
		it != m_setDeviceUsers.end(); ++it)
	{
		CAmuseRoomComponent* p = m_pOwnerRoom->GetAmusePlayer(*it);
		if ( p != NULL )
		{
			players.push_back(p);
		}
	}
}

void CAmuseRoomDevice::GetPlayers( std::vector<unsigned int>& players )
{
	players.insert(players.end(), m_setDeviceUsers.begin(), m_setDeviceUsers.end());
}

CAmuseRoomComponent* CAmuseRoomDevice::GetOtherPlayer( unsigned int nRoleID )
{
	for(DeviceUserSet::iterator it = m_setDeviceUsers.begin(); 
		it != m_setDeviceUsers.end(); ++it)
	{
		if (*it != nRoleID)
		{
			return m_pOwnerRoom->GetAmusePlayer(*it);
		}
	}
	return NULL;
}

AmuseRoomDeviceType CAmuseRoomDevice::GetDeviceType()
{
	return m_nDeviceType;
}

void CAmuseRoomDevice::SetDeviceType( AmuseRoomDeviceType type )
{
	m_nDeviceType = type;
}

void CAmuseRoomDevice::SendMsgToAll( GameMsg_Base* pMsg, CAmuseRoomComponent* pExceptPlayer /*= NULL*/ )
{
	std::vector<CAmuseRoomComponent*> players;
	GetPlayers(players);
	std::vector<CAmuseRoomComponent*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
	{
		if ( *it && *it != pExceptPlayer )
		{
			(*it)->SendPlayerMsg(pMsg);
		}
	}
}

bool CAmuseRoomDevice::IsInDevice(int nRoleId)
{
    if( m_setDeviceUsers.find(nRoleId) != m_setDeviceUsers.end() )
    {
        return true;
    }
    return false;
}

//file end

