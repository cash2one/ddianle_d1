#ifndef __ROOM_CHECK_DATA_MGR_H__
#define __ROOM_CHECK_DATA_MGR_H__

#include "../../datastructure/Macro_Define.h"


class CRoomCheck
{
public:
	CRoomCheck()
	{
		m_nCheckTime = 0;
		m_nCheckNoMiss = 0;
		m_nCheckMark = 0;
	}

	~CRoomCheck() {}

public:
	int m_nCheckTime;
	int m_nCheckNoMiss;
	int m_nCheckMark;
};


class CRoomCheckDataMgr
{
protected:
	CRoomCheckDataMgr();
	~CRoomCheckDataMgr();

public:
	static CRoomCheckDataMgr& Instance();

	void AddRoomCheckInfo(int nCheckMode, int nCheckTime, int nCheckNoMiss, int nCheckMark);
	const CRoomCheck& GetRoomCheck(int nCheckMode );

private:
	CRoomCheck m_arRoomCheck[MusicMode_Max];
};


#endif

