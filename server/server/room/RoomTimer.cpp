#include "RoomTimer.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../friend/GameMsg_C2S_Friend.h"
#include "Room.h"
#include "RoomManager.h"
#include "../logic/PlayerManager.h"
#include "../friend/EntityFriendComponent.h"
#include "../../socket/GameMsg_Map.h"

CBeginToSelectPartnerTimer::CBeginToSelectPartnerTimer()
{
	m_nTimerType = ETimerType_BeginToSelectPartner;
}

CBeginToSelectPartnerTimer::~CBeginToSelectPartnerTimer()
{

}

void CBeginToSelectPartnerTimer::OnEvent()
{
	CRoom* pRoom = CRoomManager::Instance().FindRoom((unsigned int)m_nTimerID);
	if (pRoom)
	{
		pRoom->BeginToSelectPartner();
	}
}

CEndToSelectPartnerTimer::CEndToSelectPartnerTimer()
{
	m_nTimerType = ETimerType_EndToSelectPartner;
}

CEndToSelectPartnerTimer::~CEndToSelectPartnerTimer()
{

}

void CEndToSelectPartnerTimer::OnEvent()
{
	CRoom* pRoom = CRoomManager::Instance().FindRoom((unsigned int)m_nTimerID);
	if (pRoom)
	{
		pRoom->EndToSelectPartner();

		const SelectedMap& mp = pRoom->GetSelectedMap();
		GameMsg_S2C_EndToSelectPartner msg;
		msg.m_vecGroup.resize(mp.size());
		size_t i = 0;
		for (SelectedMap::const_iterator it = mp.begin(); it != mp.end(); ++it)
		{
			GameMsg_S2C_EndToSelectPartner::Sweethearts& elem = msg.m_vecGroup[i++];
			elem.m_nGroupID = it->first;
			elem.m_bSelectEachOther = it->second.m_bSelectEachOther;
			elem.m_vecRoleID = it->second.m_vecRoleId;
		}
		pRoom->SendMsgToAll(&msg);
	}
}

