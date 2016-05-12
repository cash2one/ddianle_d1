#include "GameMsg_VIP.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2C_VIPLevelUpReward::GameMsg_S2C_VIPLevelUpReward()
:GameMsg_Base(MSG_S2C_LevelUpReward)
{
}

GameMsg_S2C_VIPLevelUpReward::~GameMsg_S2C_VIPLevelUpReward()
{
}

bool GameMsg_S2C_VIPLevelUpReward::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort(m_nVIPLevel);

	return true;
}

/////////////////////////////////////////////////////
GameMsg_C2S_GetVIPDailyReward::GameMsg_C2S_GetVIPDailyReward()
:GameMsg_Base(MSG_C2S_GetVIPDailyReward)
{

}

GameMsg_C2S_GetVIPDailyReward::~GameMsg_C2S_GetVIPDailyReward()
{

}

bool GameMsg_C2S_GetVIPDailyReward::doDecode( CParamPool &IOBuff )
{
	return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_GetVIPDailyRewardSuc::GameMsg_S2C_GetVIPDailyRewardSuc()
:GameMsg_Base(MSG_S2C_GetVIPDailyRewardSuc)
,m_nDailyExpBonus(0)
,m_nBindMBill(0)
{

}

GameMsg_S2C_GetVIPDailyRewardSuc::~GameMsg_S2C_GetVIPDailyRewardSuc()
{

}

bool GameMsg_S2C_GetVIPDailyRewardSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddInt(m_nDailyExpBonus);
    IOBuff.AddInt(m_nBindMBill);

    IOBuff.AddUShort((unsigned short)m_listItemReward.size());
    std::list<CItem>::iterator it = m_listItemReward.begin();
    for (; it != m_listItemReward.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
	return true;
}

//////////////////////////////////////////////////////////
GameMsg_S2C_UpdateVIPInfo::GameMsg_S2C_UpdateVIPInfo():
GameMsg_Base(MSG_S2C_UpdateVIPInfo)
{
}

GameMsg_S2C_UpdateVIPInfo::~GameMsg_S2C_UpdateVIPInfo()
{
}

bool GameMsg_S2C_UpdateVIPInfo::doEncode(CParamPool &IOBuff)
{
	m_updateInfo.doEncode(IOBuff);

	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RequestBuyVip::GameMsg_C2S_RequestBuyVip()
    : GameMsg_Base(MSG_C2S_RequestBuyVip)
    , m_nVIPCardID(0)
{

}

GameMsg_C2S_RequestBuyVip::~GameMsg_C2S_RequestBuyVip()
{

}

bool GameMsg_C2S_RequestBuyVip::doDecode(CParamPool &ioBuff)
{
    m_nVIPCardID = ioBuff.GetUInt();
    return true;
}

/////////////////////////////////////////////////////
GameMsg_S2C_RequestBuyVipRes::GameMsg_S2C_RequestBuyVipRes()
    : GameMsg_Base(MSG_S2C_RequestBuyVipRes)
    , m_nError(0)
{

}

GameMsg_S2C_RequestBuyVipRes::~GameMsg_S2C_RequestBuyVipRes()
{

}

bool GameMsg_S2C_RequestBuyVipRes::doEncode(CParamPool & ioBuff)
{
    ioBuff.AddUChar(m_nError);
    ioBuff.AddStringW(m_strError.c_str());
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_SendOpenVipFrom::GameMsg_C2S_SendOpenVipFrom()
    : GameMsg_Base(MSG_C2S_SendOpenVipFrom)
{
}

bool GameMsg_C2S_SendOpenVipFrom::doDecode(CParamPool &IOBuff)
{
    unsigned short count = IOBuff.GetUShort();

    for (unsigned short i = 0; i < count; ++i)
    {
        m_listOpenVipFromType.push_back(IOBuff.GetUChar() );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetOpenVipFrom::GameMsg_S2C_GetOpenVipFrom()
    : GameMsg_Base(MSG_S2C_GetOpenVipFrom)
{
}

bool GameMsg_S2C_GetOpenVipFrom::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listOpenVipFromType.size() );
    for (std::list<unsigned char>::const_iterator itr = m_listOpenVipFromType.begin(); 
        itr != m_listOpenVipFromType.end(); ++itr)
    {
        IOBuff.AddUChar(*itr);
    }

    return true;
}


