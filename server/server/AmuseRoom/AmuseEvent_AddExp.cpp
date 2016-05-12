#include "AmuseEvent_AddExp.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../../datastructure/LogStructDef.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseEvent_AddExp::CAmuseEvent_AddExp(CAmuseRoomComponent* pAmusePlayer, bool bLoopEvent, int nEventTime, int nAddExp)
:CAmuseEvent_Base(eAmuseEvent_AddExp, bLoopEvent, nEventTime)
,m_pOwnerPlayer(pAmusePlayer)
,m_nAddExp(nAddExp)
{
}

CAmuseEvent_AddExp::~CAmuseEvent_AddExp(void)
{
}

CAmuseEventParam_Base* CAmuseEvent_AddExp::ToParam(int nNowTime)
{
	return NULL;
}

void CAmuseEvent_AddExp::GoStart(int nNowTime)
{
	int nExp = m_pOwnerPlayer->AttrCOM()->AddRateExp( m_nAddExp, CRoleExpLog::EAddExpCause_AmuseRoom, 0 );

	if ( nExp > 0 )
	{
		CAmuseEventParam_AddExp* pAddExpParam = new CAmuseEventParam_AddExp();
		pAddExpParam->m_nPlayerID = m_pOwnerPlayer->RoleID();
		pAddExpParam->m_nAddExp = nExp;

		GameMsg_S2C_AmuseEventNotice msgNotice;
		msgNotice.m_pEventParam = pAddExpParam;
		m_pOwnerPlayer->AmuseRoom()->SendMsgToAll( &msgNotice );
	}

	m_bEverStart = true;
}

//file end

