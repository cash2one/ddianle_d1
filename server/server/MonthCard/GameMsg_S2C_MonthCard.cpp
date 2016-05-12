#include	"GameMsg_S2C_MonthCard.h"
#include	"../share/ServerMsgDef.h"

int GetMonthCardState( unsigned int nOpenTime, unsigned int nOpenDays, unsigned int nRenewDays )
{
	unsigned int nCurrentTime = (unsigned int)time(NULL);

	if ( nOpenTime == 0 || nCurrentTime > (nOpenTime + nOpenDays ))
	{
		return 0;
	}
	else if ( nCurrentTime <= (nOpenTime + nOpenDays - nRenewDays))
	{
		return 1;
	}
	else if ( nCurrentTime <= (nOpenTime + nOpenDays ))
	{
		return 2;
	}

	return 0;
}
/////////////////////////////////////////////////////////////
GameMsg_S2C_GetMonthCardInfoResult::GameMsg_S2C_GetMonthCardInfoResult()
	:GameMsg_Base(MSG_S2C_GetMonthCardInfoResult)
	,m_nOpenDays(0)
	,m_nRenewDays(0)
	,m_nOpenTime(0)
	,m_bIsReward(false)
	,m_nBindMBill(0)
	,m_nMoney(0)
	,m_nPayBill(0)
	,m_nRewardBindBill(0)
{
}

GameMsg_S2C_GetMonthCardInfoResult::~GameMsg_S2C_GetMonthCardInfoResult()
{
}

bool GameMsg_S2C_GetMonthCardInfoResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar( (char)GetMonthCardState(m_nOpenTime, m_nOpenDays, m_nRenewDays ));
	IOBuff.AddUInt( m_nOpenTime );
	IOBuff.AddUInt( m_nOpenTime + m_nOpenDays  );
	IOBuff.AddUChar( m_bIsReward ? 1 : 0 );
	IOBuff.AddUShort( (unsigned short)m_listItem.size() );
	std::list<CItem>::iterator it = m_listItem.begin();
	for ( ; it != m_listItem.end(); ++it )
	{
		it->doEncode( IOBuff );
	}
	IOBuff.AddUInt( m_nBindMBill );
	IOBuff.AddUInt( m_nMoney );
	IOBuff.AddStringW( m_strDesc );
	IOBuff.AddUInt(m_nPayBill);
	IOBuff.AddUInt(m_nRewardBindBill);
	return true;
}


/////////////////////////////////////////////////////////////
GameMsg_S2C_GetMonthCardEveryDayRewardSuc::GameMsg_S2C_GetMonthCardEveryDayRewardSuc()
	:GameMsg_Base(MSG_S2C_GetMonthCardRewardSuc)
	,m_bIsReward(false)
{
}

GameMsg_S2C_GetMonthCardEveryDayRewardSuc::~GameMsg_S2C_GetMonthCardEveryDayRewardSuc()
{
}

bool GameMsg_S2C_GetMonthCardEveryDayRewardSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar( m_bIsReward ? 1 : 0 );
	return true;
}

/////////////////////////////////////////////////////////////
GameMsg_S2C_GetMonthCardEveryDayRewardFail::GameMsg_S2C_GetMonthCardEveryDayRewardFail()
	:GameMsg_Base(MSG_S2C_GetMonthCardRewardFail)
	,m_nError(0)
{
}

GameMsg_S2C_GetMonthCardEveryDayRewardFail::~GameMsg_S2C_GetMonthCardEveryDayRewardFail()
{
}

bool GameMsg_S2C_GetMonthCardEveryDayRewardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar( (char)m_nError );
	return true;
}

/////////////////////////////////////////////////////////////
GameMsg_S2C_OpenMonthCardSuc::GameMsg_S2C_OpenMonthCardSuc()
	:GameMsg_Base(MSG_S2C_OpenMonthCardSuc)
,m_nOpenDays(0)
,m_nRenewDays(0)
,m_nOpenTime(0)
,m_bIsReward(false)
,m_nBindMBill(0)
,m_nMoney(0)
{
}

GameMsg_S2C_OpenMonthCardSuc::~GameMsg_S2C_OpenMonthCardSuc()
{
}

bool GameMsg_S2C_OpenMonthCardSuc::doEncode(CParamPool &IOBuff)
{
 	IOBuff.AddChar( (char)GetMonthCardState(m_nOpenTime, m_nOpenDays, m_nRenewDays));
	IOBuff.AddUInt( m_nOpenTime );
	IOBuff.AddUInt( m_nOpenTime + m_nOpenDays );
	IOBuff.AddUChar( m_bIsReward ? 1 : 0 );
	IOBuff.AddUShort( (unsigned short)m_listItem.size() );
	std::list<CItem>::iterator it = m_listItem.begin();
	for ( ; it != m_listItem.end(); ++it )
	{
		it->doEncode( IOBuff );
	}
	IOBuff.AddUInt( m_nBindMBill );
	IOBuff.AddUInt( m_nMoney );

	return true;
}

/////////////////////////////////////////////////////////////
GameMsg_S2C_OpenMonthCardFail::GameMsg_S2C_OpenMonthCardFail()
	:GameMsg_Base(MSG_S2C_OpenMonthCardFail)
	,m_nError(0)
{
}

GameMsg_S2C_OpenMonthCardFail::~GameMsg_S2C_OpenMonthCardFail()
{
}

bool GameMsg_S2C_OpenMonthCardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar( (char)m_nError );
	return true;
}

/////////////////////////////////////////////////////////////
GameMsg_S2C_RenewMonthCardSuc::GameMsg_S2C_RenewMonthCardSuc()
	:GameMsg_Base(MSG_S2C_RenewMonthCardSuc)
	,m_nOpenDays(0)
	,m_nRenewDays(0)
	,m_nOpenTime(0)
	,m_bIsReward(false)
	,m_nBindMBill(0)
	,m_nMoney(0)
{
}

GameMsg_S2C_RenewMonthCardSuc::~GameMsg_S2C_RenewMonthCardSuc()
{
}

bool GameMsg_S2C_RenewMonthCardSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar( (char)GetMonthCardState(m_nOpenTime, m_nOpenDays, m_nRenewDays));
	IOBuff.AddUInt( m_nOpenTime );
	IOBuff.AddUInt( m_nOpenTime + m_nOpenDays );
	IOBuff.AddUShort( (unsigned short)m_listItem.size() );
	std::list<CItem>::iterator it = m_listItem.begin();
	for ( ; it != m_listItem.end(); ++it )
	{
		it->doEncode( IOBuff );
	}
	IOBuff.AddUInt( m_nBindMBill );
	IOBuff.AddUInt( m_nMoney );
	return true;
}

/////////////////////////////////////////////////////////////
GameMsg_S2C_RenewMonthCardFail::GameMsg_S2C_RenewMonthCardFail()
	:GameMsg_Base(MSG_S2C_RenewMonthCardFail)
	,m_nError(0)
{
}

GameMsg_S2C_RenewMonthCardFail::~GameMsg_S2C_RenewMonthCardFail()
{
}

bool GameMsg_S2C_RenewMonthCardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddChar((char)m_nError);
	return true;
}


