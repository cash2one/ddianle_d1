#ifndef __GAMEMSG_VIP_H__
#define	__GAMEMSG_VIP_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2C_VIPLevelUpReward : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_VIPLevelUpReward();
	~GameMsg_S2C_VIPLevelUpReward();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	unsigned short m_nVIPLevel;
};

//////////////////////////////////////////
class GameMsg_C2S_GetVIPDailyReward:
	public GameMsg_Base	
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetVIPDailyReward);
	GameMsg_C2S_GetVIPDailyReward();
	~GameMsg_C2S_GetVIPDailyReward();
public:
	bool doDecode(CParamPool &IOBuff);
};

////////////////////////////////////////
class GameMsg_S2C_GetVIPDailyRewardSuc : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetVIPDailyRewardSuc();
	~GameMsg_S2C_GetVIPDailyRewardSuc();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	int m_nDailyExpBonus;
    int m_nBindMBill;
    std::list<CItem>    m_listItemReward;
};

class GameMsg_S2C_UpdateVIPInfo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateVIPInfo();
	~GameMsg_S2C_UpdateVIPInfo();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	CVIPUpdateInfo	m_updateInfo;
};



//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_RequestBuyVip:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestBuyVip);
    GameMsg_C2S_RequestBuyVip();
    ~GameMsg_C2S_RequestBuyVip();

public:
    bool doDecode(CParamPool &ioBuff);

public:
    itemtype_t m_nVIPCardID;    // ѡ���VIP�����ͣ��ܿ����꿨��
};

////////////////////////////////////////
class GameMsg_S2C_RequestBuyVipRes : 
    public GameMsg_Base
{
public:
    enum eBuyVipErrorType
    {
        eBuyVipErrorType_OK = 0,
        eBuyVipErrorType_NoVIPCard = 1,         // û�и������͵�VIP��
        eBuyVipErrorType_NotEnoughBalance = 2,  // ����
    };

public:
    GameMsg_S2C_RequestBuyVipRes();
    ~GameMsg_S2C_RequestBuyVipRes();

public:
    bool doEncode(CParamPool &ioBuff);

public:
    unsigned char m_nError;         // ����ö�٣�0Ϊ�ɹ�
    std::string m_strError;         // �մ���ʾ�ɹ�
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_SendOpenVipFrom : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendOpenVipFrom);
    GameMsg_C2S_SendOpenVipFrom();
    ~GameMsg_C2S_SendOpenVipFrom() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    std::list<unsigned char>    m_listOpenVipFromType;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetOpenVipFrom : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetOpenVipFrom);
    GameMsg_S2C_GetOpenVipFrom();
    ~GameMsg_S2C_GetOpenVipFrom() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<unsigned char>    m_listOpenVipFromType;
};

#endif






