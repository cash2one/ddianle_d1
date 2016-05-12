#include "GameMsg_SyncSetting.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"


GameMsg_A2S_SyncChargeSetting::GameMsg_A2S_SyncChargeSetting(void)
    : _baseClass(MSG_A2S_SyncChargeSetting)
    , m_bEnableRecharge(false)
    , m_bEnableFirstRechargeReward(false)
{
}

GameMsg_A2S_SyncChargeSetting::~GameMsg_A2S_SyncChargeSetting(void)
{
}

bool GameMsg_A2S_SyncChargeSetting::doEncode( CParamPool& IoBuff )
{
    IoBuff.AddBool(m_bEnableRecharge);
    IoBuff.AddBool(m_bEnableFirstRechargeReward);

    return true;
}

bool GameMsg_A2S_SyncChargeSetting::doDecode( CParamPool& IoBuff )
{
    m_bEnableRecharge = IoBuff.GetBool();
    m_bEnableFirstRechargeReward = IoBuff.GetBool();

    return true;
}

