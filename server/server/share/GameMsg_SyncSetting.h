#ifndef __GameMsg_SyncSetting_H__
#define __GameMsg_SyncSetting_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_A2S_SyncChargeSetting : public GameMsg_Base
{
    typedef GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_A2S_SyncChargeSetting);
    GameMsg_A2S_SyncChargeSetting(void);
    ~GameMsg_A2S_SyncChargeSetting(void);

    virtual bool doEncode(CParamPool & ioBuff);
    virtual bool doDecode(CParamPool & ioBuff);

public:
    bool m_bEnableRecharge;
    bool m_bEnableFirstRechargeReward;
};

#endif

