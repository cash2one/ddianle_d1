#include "AmuseEvent_StartCarrousel.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseEvent_StartCarrousel::CAmuseEvent_StartCarrousel(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime, std::vector<std::string>& vecStartSongs)
:CAmuseEvent_Base(eAmuseEvent_StartCarrousel, bLoopEvent, nEventTime)
,m_pOwnerRoom(pAmuseRoom)
,m_nStartTime(0)
{
	m_vecStartSongs = vecStartSongs;
}

CAmuseEvent_StartCarrousel::~CAmuseEvent_StartCarrousel(void)
{
}

CAmuseEventParam_Base* CAmuseEvent_StartCarrousel::ToParam(int nNowTime)
{
	CAmuseEventParam_StartCarrousel* pCarrouselParam = NULL;
	if ( m_nStartTime > 0 )
	{
		pCarrouselParam = new CAmuseEventParam_StartCarrousel();
		pCarrouselParam->m_nStartTime = nNowTime - m_nStartTime;
		pCarrouselParam->m_strStartSong = m_strStartSong;
	}

	return pCarrouselParam;
}

void CAmuseEvent_StartCarrousel::GoStart(int nNowTime)
{
	std::string strStartSong = "";
	if ( !m_vecStartSongs.empty() )
	{
		int nIndex = Random( 0, m_vecStartSongs.size() - 1 );
		strStartSong = m_vecStartSongs[nIndex];
	}

	m_nStartTime = nNowTime;

	CAmuseEventParam_StartCarrousel* pCarrouselParam = new CAmuseEventParam_StartCarrousel();
	pCarrouselParam->m_nStartTime = 0;
	pCarrouselParam->m_strStartSong = strStartSong;

	GameMsg_S2C_AmuseEventNotice msgNotice;
	msgNotice.m_pEventParam = pCarrouselParam;
	m_pOwnerRoom->SendMsgToAll( &msgNotice );

	m_bEverStart = true;
}

//file end

