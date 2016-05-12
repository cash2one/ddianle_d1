#ifndef		__GAMEMSG_S2C_GETCHECKININFORESULT_H__
#define		__GAMEMSG_S2C_GETCHECKININFORESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2C_GetCheckInInfoResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCheckInInfoResult();
	~GameMsg_S2C_GetCheckInInfoResult();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	char			m_cSex;
	CCheckInInfo	m_CheckInInfo;

};

#endif


// end of file

