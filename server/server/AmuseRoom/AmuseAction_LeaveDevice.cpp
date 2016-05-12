#include "AmuseAction_LeaveDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseRoomDevice.h"


CAmuseAction_LeaveDevice::CAmuseAction_LeaveDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_LeaveDevice, true, false)
{
}

CAmuseAction_LeaveDevice::~CAmuseAction_LeaveDevice(void)
{
}

bool CAmuseAction_LeaveDevice::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_LeaveDevice )
	{
		return true;
	}

	return false;
}

bool CAmuseAction_LeaveDevice::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice );
	if ( pUseDeviceAction != NULL )
	{
		CAmuseRoomDevice* pRoomDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice(pUseDeviceAction->m_nDeviceID);
		if ( pRoomDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg )
		{
			vector<int> vecActionType;
			vecActionType.push_back(eAmuseAction_GetTimeEggGiftList);
			vecActionType.push_back(eAmuseAction_CommitTimeEggInfo);
			vecActionType.push_back(eAmuseAction_CreateTimeEgg);
			vecActionType.push_back(eAmuseAction_OpenTimeEgg);
			for (vector<int>::iterator it = vecActionType.begin();
				it != vecActionType.end(); ++it)
			{
				CAmuseAction_Base* pAction = m_pActPlayer->UniqueAmuseAction( *it );
				if ( pAction )
				{
					m_pActPlayer->AmuseActionOver( pAction );
				}
			}
		}

		m_pActPlayer->AmuseActionOver( pUseDeviceAction );

		CAmuseActionParam_LeaveDevice* pLeaveDeviceParam = new CAmuseActionParam_LeaveDevice();
		pLeaveDeviceParam->m_nPlayerID = m_pActPlayer->RoleID();

		GameMsg_S2C_DoAmuseActionSuc msgResponse;
		msgResponse.m_pActionParam = pLeaveDeviceParam;
		m_pActPlayer->AmuseRoom()->SendMsgToAll( &msgResponse );

		m_bSuccess = true;
	}

	return false;
}

//file end

