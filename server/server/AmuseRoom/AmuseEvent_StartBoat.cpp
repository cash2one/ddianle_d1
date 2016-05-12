#include "AmuseEvent_StartBoat.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseEvent_StartBoat::CAmuseEvent_StartBoat(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime, int nBoatDock)
:CAmuseEvent_Base(eAmuseEvent_StartCarrousel, bLoopEvent, nEventTime)
,m_pOwnerRoom(pAmuseRoom)
,m_nStartTime(0)
,m_nBoatDock(nBoatDock)
{
}

CAmuseEvent_StartBoat::~CAmuseEvent_StartBoat(void)
{
}

CAmuseEventParam_Base* CAmuseEvent_StartBoat::ToParam(int nNowTime)
{
	CAmuseEventParam_StartBoat* pBoatParam = NULL;
	if ( m_nStartTime > 0 )
	{
		pBoatParam = new CAmuseEventParam_StartBoat();
		pBoatParam->m_nStartTime = nNowTime - m_nStartTime;
		pBoatParam->m_nDesDock = (char)m_nBoatDock;
	}

	return pBoatParam;
}

void CAmuseEvent_StartBoat::GoStart(int nNowTime)
{
	m_nStartTime = nNowTime;

	++m_nBoatDock;
	m_nBoatDock %= 2;

	CAmuseEventParam_StartBoat* pBoatParam = new CAmuseEventParam_StartBoat();
	pBoatParam->m_nDesDock = (char)m_nBoatDock;
	pBoatParam->m_nStartTime = 0;

	GameMsg_S2C_AmuseEventNotice msgNotice;
	msgNotice.m_pEventParam = pBoatParam;
	m_pOwnerRoom->SendMsgToAll( &msgNotice );

	m_bEverStart = true;
}

//file end

