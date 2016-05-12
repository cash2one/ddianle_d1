#include "AmuseAction_OpenTimeEgg.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "GameMsg_TimeEggMsg.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "AmuseRoomDataManager.h"
#include "AmuseRoomDevice.h"
#include "../mall/EntityMallComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/ItemProcess.h"
#include "../mail/MailMgr.h"
#include "../../datastructure/Localization.h"
#include "../../socket/Formatter.h"
#include <sstream>

CAmuseAction_OpenTimeEgg::CAmuseAction_OpenTimeEgg(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_OpenTimeEgg, true, true)
{
	m_chOpenType = 0;
}

CAmuseAction_OpenTimeEgg::~CAmuseAction_OpenTimeEgg(void)
{

}

bool CAmuseAction_OpenTimeEgg::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_OpenTimeEgg )
	{
		CAmuseActionParam_OpenTimeEgg* pOpenTimeEgg = (CAmuseActionParam_OpenTimeEgg*)pActionParam;
		m_chOpenType = pOpenTimeEgg->m_chOpenType;
		return true;
	}

	return false;
}

bool CAmuseAction_OpenTimeEgg::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	unsigned int nNow = (unsigned int)time(NULL);
	CAmuseRoomDevice* pAmuseDevice = NULL;
	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
	if (pUseDeviceAction)
	{
		pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( pUseDeviceAction->m_nDeviceID );
	}
	TimeEggEntry* pEgg = m_pActPlayer->GetTimeEggInCurScene();
	CAmuseAction_OpenTimeEgg* pOpenAction = (CAmuseAction_OpenTimeEgg*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_CreateTimeEgg ));
	if ( pOpenAction || pEgg == NULL || pEgg->m_nDestroyTime < nNow )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyOpenTimeEgg";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( pUseDeviceAction == NULL || pAmuseDevice == NULL ||
		pAmuseDevice->GetDeviceType() != AmuseRoomDeviceType_TimeEgg )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NotInRightDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( nNow < pEgg->m_nEndTime )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_OpenTimeEggNotTime";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( m_chOpenType != OpenEggType_OpenAlone && m_chOpenType != OpenEggType_Together )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NoOpenType";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	if ( m_chOpenType == OpenEggType_Together )
	{
		CAmuseRoomComponent* pPartnerCOM = pAmuseDevice->GetOtherPlayer(m_pActPlayer->AttrCOM()->GetRoleID());
		if ( pPartnerCOM == NULL )
		{
			GameMsg_S2C_DoAmuseActionFail msgResponse;
			msgResponse.m_strError = "DoAmuseAction_NoPartner";
			m_pActPlayer->SendPlayerMsg( &msgResponse );
			return false;
		}
		else if ( pPartnerCOM->AttrCOM()->GetRoleID() != pEgg->m_nRoleID )
		{
			GameMsg_S2C_DoAmuseActionFail msgResponse;
			msgResponse.m_strError = "DoAmuseAction_NoRightPartner";
			m_pActPlayer->SendPlayerMsg( &msgResponse );
			return false;
		}
	}
	OpenTimeEggEntry* pOpenTimeEggEntry = new OpenTimeEggEntry();
	pOpenTimeEggEntry->m_nEggID = pEgg->m_nEggID;
	pOpenTimeEggEntry->m_nOpenRoleID = m_pActPlayer->AttrCOM()->GetRoleID();
	pOpenTimeEggEntry->m_nOpenType = (OpenEggType)m_chOpenType;

	DoOpenTimeEgg(pOpenTimeEggEntry);
	m_bSuccess = true;
	return true;
}

void CAmuseAction_OpenTimeEgg::DoOpenTimeEgg(OpenTimeEggEntry* pOpenTimeEggEntry)
{
	//Êý¾Ý¿âÇëÇó
	CDelHelper delHelper(pOpenTimeEggEntry);
	m_pActPlayer->AddQuery(Query_OpenTimeEgg, 0, pOpenTimeEggEntry, 0, NULL, &delHelper);
}

//file end

