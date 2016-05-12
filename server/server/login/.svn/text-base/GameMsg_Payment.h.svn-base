#include "../../socket/GameMsg_Base.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>
#include <string>

using namespace std;


class GameMsg_S2C_FinishedFirstPayment:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FinishedFirstPayment();
	~GameMsg_S2C_FinishedFirstPayment();
public:
	int m_nMoneyReward;
	int m_nMcoinReward;
	std::list<CItem> m_listItem;
public:
	bool doEncode(CParamPool & IOBuff);
};

class GameMsg_C2S_GetFirstPaymentPacketDtails : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFirstPaymentPacketDtails);

	GameMsg_C2S_GetFirstPaymentPacketDtails(void);
	~GameMsg_C2S_GetFirstPaymentPacketDtails(void);

	virtual bool doDecode( CParamPool &IOBuff );
};


class GameMsg_S2C_GetFirstPaymentPacketDetailsRes: public GameMsg_Base
{
public:
	GameMsg_S2C_GetFirstPaymentPacketDetailsRes( );
	~GameMsg_S2C_GetFirstPaymentPacketDetailsRes( );
public:
	std::vector<CChargePlan> m_vecChargePlan;
	std::list<CItem> m_listRewards;
public:
	bool doEncode( CParamPool & IOBuff );
};

class GameMsg_S2C_FirstPaymentStateChange: public GameMsg_Base
{
public:
	GameMsg_S2C_FirstPaymentStateChange();
	~GameMsg_S2C_FirstPaymentStateChange();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	bool m_bFirstPaymentRewardsIsOpen;
};


class QuotaRechargeInfo
{
public:
    QuotaRechargeInfo();
    ~QuotaRechargeInfo();

    std::string m_strItem;              // 充值的ID。
    unsigned int m_returnPtBind;        // 奖励的绑定MB
    bool m_bOpenQuota;                  // 是否已经完成该充值
};

class GameMsg_C2S_FixedChargeReq : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_FixedChargeReq);

	GameMsg_C2S_FixedChargeReq(void);
	~GameMsg_C2S_FixedChargeReq(void);

	virtual bool doDecode( CParamPool &IOBuff );
};

// 定额首冲完成通知
class GameMsg_S2C_FixedchargeComplete : public GameMsg_Base
{
public:
    GameMsg_S2C_FixedchargeComplete();
    ~GameMsg_S2C_FixedchargeComplete();

    virtual bool doEncode(CParamPool &IOBuff);

    bool m_bOpened;
    std::vector<QuotaRechargeInfo> m_vecRechargeInfo;
    bool m_bIsFixedCharge;
    unsigned int m_nGiftBindPoint;
};



//////////////////////////////////////////////////////////////////////////
// 客户端请求充值规则
class GameMsg_C2S_GetRechargeRewardsRules: public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_C2S_GetRechargeRewardsRules );

	GameMsg_C2S_GetRechargeRewardsRules( );
	~GameMsg_C2S_GetRechargeRewardsRules( );
public:
	virtual bool doDecode( CParamPool &IOBuff );
};
// 服务器响应请求,返回充值规则
class GameMsg_S2C_GetRechargeRewardsRulesSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetRechargeRewardsRulesSuc( );
	~GameMsg_S2C_GetRechargeRewardsRulesSuc( );
public:
	// 充值规则
	std::vector<CChargePlan> m_vecChargePlan;
public:
	bool doEncode( CParamPool & IOBuff );
};
// 客户端询问 充值奖励是否开放
class GameMsg_C2S_RechargeRewardEnable:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_C2S_RechargeRewardEnable );

	GameMsg_C2S_RechargeRewardEnable( );
	~GameMsg_C2S_RechargeRewardEnable( );
public:
	bool doDecode( CParamPool &IOBuff );
public:
};
// 服务器返回  充值奖励 是否开放 标志
class GameMsg_S2C_RechargeRewardsEnableSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RechargeRewardsEnableSuc( );
	~GameMsg_S2C_RechargeRewardsEnableSuc( );
public:
	bool doEncode( CParamPool &IOBuff );
public:
	// 是否开放
	bool m_bRewardOPen;
};
// 发送充值奖励完成
class GameMsg_S2C_SendRechargeRewardFinished:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendRechargeRewardFinished( );
	~GameMsg_S2C_SendRechargeRewardFinished( );
public:

public:
	bool doEncode( CParamPool & IOBuff );
};
