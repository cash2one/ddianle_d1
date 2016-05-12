#include "GameMsg_LevelUpReward.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_LevelUpRewardInfo::GameMsg_S2C_LevelUpRewardInfo()
    : GameMsg_Base(MSG_S2C_GetLevelUpRewardInfoRes)
    , m_nCurrLevel(0)
    , m_nCurrLevelMoney(0)
    , m_nCurrLevelBindMCoin(0)
    , m_nCurrLevelVIPMoney(0)
    , m_nCurrLevelVIPBindMCoin(0)
{
}

GameMsg_S2C_LevelUpRewardInfo::~GameMsg_S2C_LevelUpRewardInfo()
{

}

bool GameMsg_S2C_LevelUpRewardInfo::doEncode(CParamPool & ioBuff)
{
    ioBuff.AddUShort(m_nCurrLevel);

    ioBuff.AddInt(m_nCurrLevelMoney);
    ioBuff.AddInt(m_nCurrLevelBindMCoin);
    ioBuff.AddUShort((unsigned short)m_listCurrLevelReward.size());
    for (std::list< CItem >::iterator itr = m_listCurrLevelReward.begin();
        itr != m_listCurrLevelReward.end(); ++itr)
    {
        itr->doEncode(ioBuff);
    }

    ioBuff.AddInt(m_nCurrLevelVIPMoney);
    ioBuff.AddInt(m_nCurrLevelVIPBindMCoin);
    ioBuff.AddUShort((unsigned short)m_listCurrLevelVIPReward.size());
    for (std::list< CItem >::iterator itr = m_listCurrLevelVIPReward.begin();
        itr != m_listCurrLevelVIPReward.end(); ++itr)
    {
        itr->doEncode(ioBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_UpgradeInstaller::GameMsg_S2C_UpgradeInstaller()
    :GameMsg_Base(MSG_S2C_UpgradeInstaller)
{
}

GameMsg_S2C_UpgradeInstaller::~GameMsg_S2C_UpgradeInstaller()
{
}

bool GameMsg_S2C_UpgradeInstaller::doEncode(CParamPool & IOBuff)
{
    return true;
}

