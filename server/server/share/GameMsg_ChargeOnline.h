#ifndef __GAMEMSG_CHARGEONLINE_H__
#define __GAMEMSG_CHARGEONLINE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////
class GameMsg_Bill2Account_ChargeByAccountID : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Bill2Account_ChargeByAccountID);
    GameMsg_Bill2Account_ChargeByAccountID();
    ~GameMsg_Bill2Account_ChargeByAccountID();

public:
    virtual bool doDecode(CParamPool& ioBuff);

public:
    unsigned short nPid;            // 平台ID
    std::string strOrderId;         // 订单号
    int nVersionType;               // 付费版本还是免费版本(iOS用，安卓全是免费版)
    unsigned int nAmount;           // 充值直接得到的M币数量，不算赠送的，可能会比nAllAmount少
    unsigned int nAllAmount;        // 总共获得的M币数量
    unsigned int nCurrency;         // 真实货币金额
    std::string strDeviceID;        // 设备编号
    unsigned char nCurrencyType;    // 真实货币类型
    unsigned char bIsValidPayment;  // 是否是合法支付
    unsigned int nChargeTime;       // 充值时间
    bool bIsRetryOrder;             // 是否是掉单后重发的订单
    unsigned char nRetryTimes;      // 掉单后重试次数
    std::string strPartnerOrderID;  // 平台订单号
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_Bill2Account_ChargeByPUID : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Bill2Account_ChargeByPUID);
    GameMsg_Bill2Account_ChargeByPUID();
    ~GameMsg_Bill2Account_ChargeByPUID();

public:
    virtual bool doDecode(CParamPool& ioBuff);

public:
    unsigned short nPid;
    std::string strOrderId;
    int nVersionType;
    unsigned int nAmount;
    unsigned int nAllAmount;
    unsigned int nCurrency;
    std::string strPUID;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_A2S_ChargeNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_A2S_ChargeNotify);
    GameMsg_A2S_ChargeNotify();
    ~GameMsg_A2S_ChargeNotify();

public:
    bool doEncode(CParamPool & ioBuff);
    bool doDecode(CParamPool& ioBuff);

public:
    unsigned int nRoleID;           // 角色ID
    std::string strOrderId;         // 订单号
    unsigned int nAmount;           // 充值所得M币数量
    unsigned int nChargeTime;       // 充值时间
    bool bIsRetryOrder;             // 是否是掉单后重发的订单
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_A2S_ChargeReward : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_A2S_ChargeReward);
    GameMsg_A2S_ChargeReward();
    ~GameMsg_A2S_ChargeReward();

public:
    virtual bool doEncode(CParamPool& ioBuff);
    virtual bool doDecode(CParamPool& ioBuff);

public:
    unsigned int m_nRoleID;
    stringw m_strRoleName;
    int m_nChargeRewardType;
    int m_nRewardBindMCoin;
    int m_nRewardMoney;
    MailInfo m_mailInfo;
    int m_nVersionType;
};

//////////////////////////////////////////////////////////////////////////
// 定额首冲赠送绑定MB
class GameMsg_A2S_FixedChargeReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_A2S_FixedChargeReward);
	GameMsg_A2S_FixedChargeReward();
	~GameMsg_A2S_FixedChargeReward();

public:
	void Set(unsigned int roleID, unsigned int nAmount, unsigned int nBindMCoinUpdate);
	virtual bool doEncode(CParamPool& ioBuff);
	virtual bool doDecode(CParamPool& ioBuff);

public:
	unsigned int m_nRoleID;
	unsigned int m_nAmount; // 当前充值的金额
	unsigned int m_nBindMCoinUpdate; // 更新的bindMB
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_A2S_SendMCoinOnline : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_A2S_SendMCoinOnline);
    GameMsg_A2S_SendMCoinOnline();
    ~GameMsg_A2S_SendMCoinOnline();

public:
    bool doEncode(CParamPool &ioBuff);
    bool doDecode(CParamPool &ioBuff);

public:
    unsigned int m_nAccount;
    int m_nMCoin;
    int m_nBindMCoin;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_A2S_RedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_A2S_RedEnvelope);
    GameMsg_A2S_RedEnvelope();
    ~GameMsg_A2S_RedEnvelope();

public:
    bool doEncode(CParamPool &ioBuff);
    bool doDecode(CParamPool &ioBuff);

public:
    unsigned int        m_nRoleID;
    int                 m_nMCoin;
};


#endif

