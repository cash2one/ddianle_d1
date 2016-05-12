#ifndef	__GAMEMSG_LEVELUPREWARDINFO_H__
#define __GAMEMSG_LEVELUPREWARDINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"


//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_LevelUpRewardInfo:
    public GameMsg_Base
{
public:
    GameMsg_S2C_LevelUpRewardInfo();
    ~GameMsg_S2C_LevelUpRewardInfo();

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    unsigned short m_nCurrLevel;

    // 当前等级的普通玩家奖励
    int m_nCurrLevelMoney;
    int m_nCurrLevelBindMCoin;
    std::list<CItem> m_listCurrLevelReward;
    // 当前等级的VIP玩家奖励
    int m_nCurrLevelVIPMoney;
    int m_nCurrLevelVIPBindMCoin;
    std::list<CItem> m_listCurrLevelVIPReward;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_UpgradeInstaller:
    public GameMsg_Base
{
public:
    GameMsg_S2C_UpgradeInstaller();
    ~GameMsg_S2C_UpgradeInstaller();
public:
    virtual bool doEncode(CParamPool & IOBuff);
};

#endif

