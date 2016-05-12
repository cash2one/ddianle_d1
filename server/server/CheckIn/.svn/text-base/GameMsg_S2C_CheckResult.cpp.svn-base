#include	"GameMsg_S2C_CheckResult.h"
#include	"../share/ServerMsgDef.h"

GameMsg_S2C_CheckInSuccess::GameMsg_S2C_CheckInSuccess()
:GameMsg_Base(MSG_S2C_CheckInSuccess)
,m_nDayIndex(0)
,m_nMoney(0)
,m_nBindCoin(0)
{
}

GameMsg_S2C_CheckInSuccess::~GameMsg_S2C_CheckInSuccess()
{
}

bool GameMsg_S2C_CheckInSuccess::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDayIndex);
	m_ItemReward.doEncode( IOBuff );
	IOBuff.AddUInt( m_nMoney );
	IOBuff.AddUInt( m_nBindCoin );
	return true;
}

GameMsg_S2C_CheckInFail::GameMsg_S2C_CheckInFail()
	:GameMsg_Base(MSG_S2C_CheckResultFail)
	,m_nFailFlag(0)
{
}

GameMsg_S2C_CheckInFail::~GameMsg_S2C_CheckInFail()
{
}

bool GameMsg_S2C_CheckInFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nFailFlag );
	return true;
}

GameMsg_S2C_CheckInMailRewardNotice::GameMsg_S2C_CheckInMailRewardNotice()
    :GameMsg_Base(MSG_S2C_CheckInRewardMailNotice)
{
}

GameMsg_S2C_CheckInMailRewardNotice::~GameMsg_S2C_CheckInMailRewardNotice()
{
}

bool GameMsg_S2C_CheckInMailRewardNotice::doEncode(CParamPool &IOBuff)
{
    return true;
}




