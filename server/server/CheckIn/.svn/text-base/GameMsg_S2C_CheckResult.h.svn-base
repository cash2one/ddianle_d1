#ifndef		__GAMEMSG_S2C_CHECKRESULT_H__
#define		__GAMEMSG_S2C_CHECKRESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2C_CheckInSuccess:
	public GameMsg_Base
{
public:
	GameMsg_S2C_CheckInSuccess();
	virtual ~GameMsg_S2C_CheckInSuccess();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nDayIndex;
	CItem m_ItemReward;
	unsigned int m_nMoney;
	unsigned int m_nBindCoin;
};


class GameMsg_S2C_CheckInFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_CheckInFail();
	virtual ~GameMsg_S2C_CheckInFail();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int	m_nFailFlag;
};

class GameMsg_S2C_CheckInMailRewardNotice:
    public GameMsg_Base
{
public:
    GameMsg_S2C_CheckInMailRewardNotice();
    virtual ~GameMsg_S2C_CheckInMailRewardNotice();

public:
    bool doEncode(CParamPool &IOBuff);

public:
  
};

#endif
// end of file


