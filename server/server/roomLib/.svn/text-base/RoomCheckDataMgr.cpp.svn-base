#include "RoomCheckDataMgr.h"
#include "../../socket/Log.h"


CRoomCheckDataMgr::CRoomCheckDataMgr()
{

}

CRoomCheckDataMgr::~CRoomCheckDataMgr()
{

}

CRoomCheckDataMgr& CRoomCheckDataMgr::Instance()
{
	static CRoomCheckDataMgr s_RoomCheckIns;
	return s_RoomCheckIns;
}

void CRoomCheckDataMgr::AddRoomCheckInfo(int nCheckMode, int nCheckTime, int nCheckNoMiss, int nCheckMark)
{
	if ( nCheckMode > MusicMode_None && nCheckMode < MusicMode_Max )
	{
		CRoomCheck rRoomCheck;
		rRoomCheck.m_nCheckTime = nCheckTime;
		rRoomCheck.m_nCheckNoMiss = nCheckNoMiss;
		rRoomCheck.m_nCheckMark = nCheckMark;

		m_arRoomCheck[nCheckMode] = rRoomCheck;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRoomCheckDataMgr::AddRoomCheckInfo. Mode error. nCheckMode=%d", nCheckMode );
	}
}

const CRoomCheck& CRoomCheckDataMgr::GetRoomCheck(int nCheckMode)
{
	if ( nCheckMode <= MusicMode_None || nCheckMode >= MusicMode_Max )
	{
		nCheckMode = MusicMode_None;
		WriteLog(LOGLEVEL_ERROR, "Error: CRoomCheckDataMgr::GetRoomCheck. Mode error. nCheckMode=%d", nCheckMode );
	}

	return m_arRoomCheck[nCheckMode];
}


