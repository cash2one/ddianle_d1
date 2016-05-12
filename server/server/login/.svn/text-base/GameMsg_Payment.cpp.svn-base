#include "GameMsg_Payment.h"

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FinishedFirstPayment::GameMsg_S2C_FinishedFirstPayment( ) :
GameMsg_Base( MSG_S2C_FinishedFirstPayment )
, m_nMoneyReward( 0 )
, m_nMcoinReward( 0 )
{
}

GameMsg_S2C_FinishedFirstPayment::~GameMsg_S2C_FinishedFirstPayment()
{

}

bool GameMsg_S2C_FinishedFirstPayment::doEncode(CParamPool & IOBuff)		
{
	IOBuff.AddInt( m_nMoneyReward );
	IOBuff.AddInt( m_nMcoinReward );
	IOBuff.AddUShort( (unsigned short)m_listItem.size( ) );
	for (std::list<CItem>::iterator itr = m_listItem.begin(); itr != m_listItem.end(); ++itr)
	{
		itr->doEncode(IOBuff);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetFirstPaymentPacketDtails::GameMsg_C2S_GetFirstPaymentPacketDtails(void)
:GameMsg_Base(MSG_C2S_GetFirstPaymentRewards)
{
}

GameMsg_C2S_GetFirstPaymentPacketDtails::~GameMsg_C2S_GetFirstPaymentPacketDtails(void)
{
}

bool GameMsg_C2S_GetFirstPaymentPacketDtails::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetFirstPaymentPacketDetailsRes::GameMsg_S2C_GetFirstPaymentPacketDetailsRes( )
:GameMsg_Base( MSG_S2C_GetFirstPaymentRewardsRes )
{
}

GameMsg_S2C_GetFirstPaymentPacketDetailsRes::~GameMsg_S2C_GetFirstPaymentPacketDetailsRes( )
{

}

bool GameMsg_S2C_GetFirstPaymentPacketDetailsRes::doEncode( CParamPool & IOBuff )
{
	// 首次充值	金券\Mcoin奖励规则
	IOBuff.AddUShort( (unsigned short)m_vecChargePlan.size( ) );
	for( vector<CChargePlan>::iterator itr = m_vecChargePlan.begin( ); itr != m_vecChargePlan.end( ); ++itr )
	{
		itr->doEncode( IOBuff );
	}

	// 道具奖励
	IOBuff.AddUShort( (unsigned short)m_listRewards.size( ) );
	for( list<CItem>::iterator itr = m_listRewards.begin( ); itr != m_listRewards.end( ); ++itr )
	{
		itr->doEncode( IOBuff );
	}

	return true;
}

GameMsg_S2C_FirstPaymentStateChange::GameMsg_S2C_FirstPaymentStateChange():
GameMsg_Base( MSG_S2C_FirstPaymentStateChange ),
m_bFirstPaymentRewardsIsOpen(false)
{
}

GameMsg_S2C_FirstPaymentStateChange::~GameMsg_S2C_FirstPaymentStateChange()
{
}

bool GameMsg_S2C_FirstPaymentStateChange::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddBool( m_bFirstPaymentRewardsIsOpen );

	return true;
}




QuotaRechargeInfo::QuotaRechargeInfo()
    : m_returnPtBind(0)
    , m_bOpenQuota(false)
{

}

QuotaRechargeInfo::~QuotaRechargeInfo()
{

}

GameMsg_C2S_FixedChargeReq::GameMsg_C2S_FixedChargeReq(void)
    : GameMsg_Base(MSG_C2S_FixedchargeReq)
{

}

GameMsg_C2S_FixedChargeReq::~GameMsg_C2S_FixedChargeReq(void)
{

}

bool GameMsg_C2S_FixedChargeReq::doDecode( CParamPool &IOBuff )
{
    return true;
}


GameMsg_S2C_FixedchargeComplete::GameMsg_S2C_FixedchargeComplete()
    : GameMsg_Base(MSG_S2C_FixedchargeComplete)
    , m_bOpened(false)
    , m_bIsFixedCharge(false)
    , m_nGiftBindPoint(0)
{

}

GameMsg_S2C_FixedchargeComplete::~GameMsg_S2C_FixedchargeComplete()
{

}

bool GameMsg_S2C_FixedchargeComplete::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bOpened);
    if (!m_bOpened)
        return true;

    IOBuff.AddUShort((unsigned short)m_vecRechargeInfo.size());

    for (size_t i = 0; i < m_vecRechargeInfo.size(); ++i)
    {
        IOBuff.AddStringW(m_vecRechargeInfo[i].m_strItem);
        IOBuff.AddUInt(m_vecRechargeInfo[i].m_returnPtBind);
        IOBuff.AddBool(m_vecRechargeInfo[i].m_bOpenQuota);
    }

    IOBuff.AddBool(m_bIsFixedCharge);
    if (m_bIsFixedCharge)
        IOBuff.AddUInt(m_nGiftBindPoint);

    return true;
}



//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetRechargeRewardsRules::GameMsg_C2S_GetRechargeRewardsRules( void )
:GameMsg_Base( MSG_C2S_GetRechargeRewardsRules )
{
}

GameMsg_C2S_GetRechargeRewardsRules::~GameMsg_C2S_GetRechargeRewardsRules( void  )
{
}

bool GameMsg_C2S_GetRechargeRewardsRules::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_GetRechargeRewardsRulesSuc::GameMsg_S2C_GetRechargeRewardsRulesSuc( )
:GameMsg_Base( MSG_S2C_GetRechargeRewardsRulesSuc )
{
}

GameMsg_S2C_GetRechargeRewardsRulesSuc::~GameMsg_S2C_GetRechargeRewardsRulesSuc( )
{
}

bool GameMsg_S2C_GetRechargeRewardsRulesSuc::doEncode( CParamPool & IOBuff )
{
	// 规则数
	IOBuff.AddUShort( (unsigned short)m_vecChargePlan.size( ) );
	// 具体规则
	for( vector<CChargePlan>::iterator itr = m_vecChargePlan.begin( ); itr != m_vecChargePlan.end( ); ++itr )
	{
		itr->doEncode( IOBuff );
	}
	return true;
}

GameMsg_S2C_RechargeRewardsEnableSuc::GameMsg_S2C_RechargeRewardsEnableSuc( ) :
GameMsg_Base( MSG_S2C_RechargeRewardsEnableSuc ),
m_bRewardOPen(false)
{
}
GameMsg_S2C_RechargeRewardsEnableSuc::~GameMsg_S2C_RechargeRewardsEnableSuc( )
{
}
bool GameMsg_S2C_RechargeRewardsEnableSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddBool( m_bRewardOPen );
	return true;
}

GameMsg_C2S_RechargeRewardEnable::GameMsg_C2S_RechargeRewardEnable( void ) :
GameMsg_Base( MSG_C2S_RechargeRewardEnable )
{
}
GameMsg_C2S_RechargeRewardEnable::~GameMsg_C2S_RechargeRewardEnable( void )
{
}
bool GameMsg_C2S_RechargeRewardEnable::doDecode( CParamPool &IOBuff )
{
	return true;
}
// 处理充值奖励
GameMsg_S2C_SendRechargeRewardFinished::GameMsg_S2C_SendRechargeRewardFinished( ) :
GameMsg_Base( MSG_S2C_SendRechargeRewardFinished )
{
}
GameMsg_S2C_SendRechargeRewardFinished::~GameMsg_S2C_SendRechargeRewardFinished( )
{
}
bool GameMsg_S2C_SendRechargeRewardFinished::doEncode( CParamPool & IOBuff )
{
	return true;
}

