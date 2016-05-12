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
    unsigned short nPid;            // ƽ̨ID
    std::string strOrderId;         // ������
    int nVersionType;               // ���Ѱ汾������Ѱ汾(iOS�ã���׿ȫ����Ѱ�)
    unsigned int nAmount;           // ��ֱֵ�ӵõ���M���������������͵ģ����ܻ��nAllAmount��
    unsigned int nAllAmount;        // �ܹ���õ�M������
    unsigned int nCurrency;         // ��ʵ���ҽ��
    std::string strDeviceID;        // �豸���
    unsigned char nCurrencyType;    // ��ʵ��������
    unsigned char bIsValidPayment;  // �Ƿ��ǺϷ�֧��
    unsigned int nChargeTime;       // ��ֵʱ��
    bool bIsRetryOrder;             // �Ƿ��ǵ������ط��Ķ���
    unsigned char nRetryTimes;      // ���������Դ���
    std::string strPartnerOrderID;  // ƽ̨������
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
    unsigned int nRoleID;           // ��ɫID
    std::string strOrderId;         // ������
    unsigned int nAmount;           // ��ֵ����M������
    unsigned int nChargeTime;       // ��ֵʱ��
    bool bIsRetryOrder;             // �Ƿ��ǵ������ط��Ķ���
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
// �����׳����Ͱ�MB
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
	unsigned int m_nAmount; // ��ǰ��ֵ�Ľ��
	unsigned int m_nBindMCoinUpdate; // ���µ�bindMB
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

