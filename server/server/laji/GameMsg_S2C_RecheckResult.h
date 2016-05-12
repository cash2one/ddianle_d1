#ifndef		__GAMEMSG_S2C_RECKECKRESULT_H__
#define		__GAMEMSG_S2C_RECKECKRESULT_H__ 

#include "../socket/GameMsg_Base.h"
#include "../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2C_RecheckResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RecheckResult(int nWeekDay = 0, unsigned char nResult = 1);
	~GameMsg_S2C_RecheckResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	int m_nWeekDay;
	CItem m_item;
	unsigned char m_nResult;	// 0 ³É¹¦ 1 Ê§°Ü
};

#endif
// end of file


