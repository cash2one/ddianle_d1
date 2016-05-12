#include "GameMsg_ChargeOnline.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"
#include "../../datastructure/Macro_Define.h"

//////////////////////////////////////////////////////////////////////////
GameMsg_Bill2Account_ChargeByAccountID::GameMsg_Bill2Account_ChargeByAccountID()
    : GameMsg_Base(MSG_SERVER_ChargeByAccountID)
{
    nPid = 0;
    strOrderId = "";
    nVersionType = 0;
    nAmount = 0;
    nAllAmount = 0;
    nCurrency = 0;
    strDeviceID = "";
    nCurrencyType = 0;
    bIsValidPayment = 0;
    nChargeTime = 0;
    bIsRetryOrder = false;
    nRetryTimes = 0;
    strPartnerOrderID = "";
}

GameMsg_Bill2Account_ChargeByAccountID::~GameMsg_Bill2Account_ChargeByAccountID()
{
}

bool GameMsg_Bill2Account_ChargeByAccountID::doDecode(CParamPool& ioBuff)
{
    nPid = ioBuff.GetUShort();
    ioBuff.GetStringW(strOrderId);
    nVersionType = ioBuff.GetInt();
    nAllAmount = ioBuff.GetUInt();
    nAmount = ioBuff.GetUInt();
    nCurrency = ioBuff.GetUInt();
    ioBuff.GetStringW(strDeviceID);
    nCurrencyType = ioBuff.GetUChar();
    bIsValidPayment = ioBuff.GetUChar();
    nChargeTime = ioBuff.GetUInt();
    bIsRetryOrder = ioBuff.GetUChar() != 0 ? true : false;
    nRetryTimes = ioBuff.GetUChar();
    ioBuff.GetStringW(strPartnerOrderID);

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_Bill2Account_ChargeByPUID::GameMsg_Bill2Account_ChargeByPUID()
    : GameMsg_Base(MSG_SERVER_ChargeByPUID)
{
    nPid = 0;
    strOrderId = "";
    nVersionType = 0;
    nAmount = 0;
    nAllAmount = 0;
    nCurrency = 0;
    strPUID = "";
}

GameMsg_Bill2Account_ChargeByPUID::~GameMsg_Bill2Account_ChargeByPUID()
{
}

bool GameMsg_Bill2Account_ChargeByPUID::doDecode(CParamPool& ioBuff)
{
    nPid = ioBuff.GetUShort();
    ioBuff.GetStringW(strOrderId);
    nVersionType = ioBuff.GetInt();
    nAllAmount = ioBuff.GetUInt();
    nAmount = ioBuff.GetUInt();
    nCurrency = ioBuff.GetUInt();
    ioBuff.GetStringW(strPUID);
    if (strPUID.size() > MAX_PUID_LEN)
    {
        strPUID.resize(MAX_PUID_LEN);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_A2S_ChargeNotify::GameMsg_A2S_ChargeNotify()
    : GameMsg_Base(MSG_A2S_ChargeNotify)
{
    nRoleID = 0;
    strOrderId = "";
    nAmount = 0;
    nChargeTime = 0;
    bIsRetryOrder = false;
}

GameMsg_A2S_ChargeNotify::~GameMsg_A2S_ChargeNotify()
{
}

bool GameMsg_A2S_ChargeNotify::doEncode(CParamPool &ioBuff)
{
    ioBuff.AddUInt(nRoleID);
    ioBuff.AddStringW(strOrderId.c_str());
    ioBuff.AddUInt(nAmount);
    ioBuff.AddUInt(nChargeTime);
    ioBuff.AddBool(bIsRetryOrder);

    return true;
}

bool GameMsg_A2S_ChargeNotify::doDecode(CParamPool& ioBuff)
{
    nRoleID = ioBuff.GetUInt();
    ioBuff.GetStringW(strOrderId);
    nAmount = ioBuff.GetUInt();
    nChargeTime = ioBuff.GetUInt();
    bIsRetryOrder = ioBuff.GetBool();

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_A2S_ChargeReward::GameMsg_A2S_ChargeReward()
    : GameMsg_Base(MSG_A2S_ChargeReward)
{
    m_nRoleID = 0;
    m_nChargeRewardType = 0;
    m_nRewardBindMCoin = 0;
    m_nRewardMoney = 0;
}

GameMsg_A2S_ChargeReward::~GameMsg_A2S_ChargeReward()
{

}

bool GameMsg_A2S_ChargeReward::doEncode(CParamPool& ioBuff)
{
    ioBuff << m_nRoleID << m_strRoleName << m_nChargeRewardType
        << m_nRewardBindMCoin << m_nRewardMoney << m_nVersionType;
    m_mailInfo.doEncodeToServer(ioBuff);
    return true;
}

bool GameMsg_A2S_ChargeReward::doDecode(CParamPool& ioBuff)
{
    ioBuff >> m_nRoleID >> m_strRoleName >> m_nChargeRewardType
        >> m_nRewardBindMCoin >> m_nRewardMoney >> m_nVersionType;
    m_mailInfo.doDecodeFromServer(ioBuff);
    return true;
}

//////////////////////////////////////////////////////////////////////////
// ¹Ì¶¨½ð¶îÊ×³å
GameMsg_A2S_FixedChargeReward::GameMsg_A2S_FixedChargeReward()
	: GameMsg_Base(MSG_A2S_FixedChargeReward)
	, m_nRoleID(0)
	, m_nAmount(0)
	, m_nBindMCoinUpdate(0)
{

}

GameMsg_A2S_FixedChargeReward::~GameMsg_A2S_FixedChargeReward()
{
	return ;
}

void GameMsg_A2S_FixedChargeReward::Set(unsigned int roleID, unsigned int nAmount, unsigned int nBindMCoinUpdate)
{
	m_nRoleID = roleID;
	m_nAmount = nAmount;
	m_nBindMCoinUpdate = nBindMCoinUpdate;
}


bool GameMsg_A2S_FixedChargeReward::doEncode(CParamPool& ioBuff)
{
	ioBuff << m_nRoleID << m_nAmount << m_nBindMCoinUpdate;
	return true;
}

bool GameMsg_A2S_FixedChargeReward::doDecode(CParamPool& ioBuff)
{
	ioBuff >> m_nRoleID >> m_nAmount >> m_nBindMCoinUpdate;
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_A2S_SendMCoinOnline::GameMsg_A2S_SendMCoinOnline()
    : GameMsg_Base(MSG_A2S_SendPrivMCoin)
{

}

GameMsg_A2S_SendMCoinOnline::~GameMsg_A2S_SendMCoinOnline()
{

}

bool GameMsg_A2S_SendMCoinOnline::doEncode(CParamPool &ioBuff)
{
    ioBuff.AddUInt(m_nAccount);
    ioBuff.AddInt(m_nMCoin);
    ioBuff.AddInt(m_nBindMCoin);

    return true;
}

bool GameMsg_A2S_SendMCoinOnline::doDecode(CParamPool &ioBuff)
{
    m_nAccount = ioBuff.GetUInt();
    m_nMCoin = ioBuff.GetInt();
    m_nBindMCoin = ioBuff.GetInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_A2S_RedEnvelope::GameMsg_A2S_RedEnvelope()
    : GameMsg_Base(MSG_A2S_RedEnvelope)
{

}

GameMsg_A2S_RedEnvelope::~GameMsg_A2S_RedEnvelope()
{

}

bool GameMsg_A2S_RedEnvelope::doEncode(CParamPool &ioBuff)
{
    ioBuff.AddUInt(m_nRoleID);
    ioBuff.AddInt(m_nMCoin);
    return true;
}

bool GameMsg_A2S_RedEnvelope::doDecode(CParamPool &ioBuff)
{
    m_nRoleID = ioBuff.GetUInt();
    m_nMCoin = ioBuff.GetInt();
    return true;
}

