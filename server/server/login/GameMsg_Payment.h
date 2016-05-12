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

    std::string m_strItem;              // ��ֵ��ID��
    unsigned int m_returnPtBind;        // �����İ�MB
    bool m_bOpenQuota;                  // �Ƿ��Ѿ���ɸó�ֵ
};

class GameMsg_C2S_FixedChargeReq : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_FixedChargeReq);

	GameMsg_C2S_FixedChargeReq(void);
	~GameMsg_C2S_FixedChargeReq(void);

	virtual bool doDecode( CParamPool &IOBuff );
};

// �����׳����֪ͨ
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
// �ͻ��������ֵ����
class GameMsg_C2S_GetRechargeRewardsRules: public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_C2S_GetRechargeRewardsRules );

	GameMsg_C2S_GetRechargeRewardsRules( );
	~GameMsg_C2S_GetRechargeRewardsRules( );
public:
	virtual bool doDecode( CParamPool &IOBuff );
};
// ��������Ӧ����,���س�ֵ����
class GameMsg_S2C_GetRechargeRewardsRulesSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetRechargeRewardsRulesSuc( );
	~GameMsg_S2C_GetRechargeRewardsRulesSuc( );
public:
	// ��ֵ����
	std::vector<CChargePlan> m_vecChargePlan;
public:
	bool doEncode( CParamPool & IOBuff );
};
// �ͻ���ѯ�� ��ֵ�����Ƿ񿪷�
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
// ����������  ��ֵ���� �Ƿ񿪷� ��־
class GameMsg_S2C_RechargeRewardsEnableSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RechargeRewardsEnableSuc( );
	~GameMsg_S2C_RechargeRewardsEnableSuc( );
public:
	bool doEncode( CParamPool &IOBuff );
public:
	// �Ƿ񿪷�
	bool m_bRewardOPen;
};
// ���ͳ�ֵ�������
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
