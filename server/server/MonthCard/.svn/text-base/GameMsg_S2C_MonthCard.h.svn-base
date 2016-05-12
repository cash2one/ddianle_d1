#ifndef		__GAMEMSG_S2C_MONTHCARD_H__
#define		__GAMEMSG_S2C_MONTHCARD_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

int GetMonthCardState( unsigned int nOpenTime, unsigned int nOpenDays, unsigned int nRenewDays );
///////////////////////////////////////////
class GameMsg_S2C_GetMonthCardInfoResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetMonthCardInfoResult();
	~GameMsg_S2C_GetMonthCardInfoResult();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int	m_nOpenDays;
	unsigned int	m_nRenewDays;
	unsigned int	m_nOpenTime;
	bool			m_bIsReward;
	unsigned int	m_nBindMBill;
	std::list<CItem> m_listItem;
	unsigned int	m_nMoney;
	std::string		m_strDesc;
	unsigned int	m_nPayBill;
	unsigned int	m_nRewardBindBill;
};

///////////////////////////////////////////
class GameMsg_S2C_GetMonthCardEveryDayRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetMonthCardEveryDayRewardSuc();
	~GameMsg_S2C_GetMonthCardEveryDayRewardSuc();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	bool m_bIsReward;
};

///////////////////////////////////////////
class GameMsg_S2C_GetMonthCardEveryDayRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetMonthCardEveryDayRewardFail();
	~GameMsg_S2C_GetMonthCardEveryDayRewardFail();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	int m_nError;
};

///////////////////////////////////////////
class GameMsg_S2C_OpenMonthCardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_OpenMonthCardSuc();
	~GameMsg_S2C_OpenMonthCardSuc();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int	m_nOpenDays;
	unsigned int	m_nRenewDays;
	unsigned int	m_nOpenTime;
	bool			m_bIsReward;
	unsigned int	m_nBindMBill;
	std::list<CItem> m_listItem;
	unsigned int	m_nMoney;
};

///////////////////////////////////////////
class GameMsg_S2C_OpenMonthCardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_OpenMonthCardFail();
	~GameMsg_S2C_OpenMonthCardFail();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	int	m_nError;
};

///////////////////////////////////////////
class GameMsg_S2C_RenewMonthCardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RenewMonthCardSuc();
	~GameMsg_S2C_RenewMonthCardSuc();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int	m_nOpenDays;
	unsigned int	m_nRenewDays;
	unsigned int	m_nOpenTime;
	bool			m_bIsReward;
	unsigned int	m_nBindMBill;
	std::list<CItem> m_listItem;
	unsigned int	m_nMoney;
};

///////////////////////////////////////////
class GameMsg_S2C_RenewMonthCardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RenewMonthCardFail();
	~GameMsg_S2C_RenewMonthCardFail();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	int m_nError;
};

#endif
// end of file


