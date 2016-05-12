#include "GameMsg_C2S_TouristSignInfo.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

GameMsg_C2S_TouristSignInfo::GameMsg_C2S_TouristSignInfo()
    :GameMsg_Base(MSG_C2S_TouristSignInfo)
    , m_nUserType(0)
{

}

GameMsg_C2S_TouristSignInfo::~GameMsg_C2S_TouristSignInfo()
{

}

bool GameMsg_C2S_TouristSignInfo::doDecode(CParamPool &IOBuff)
{
    m_nUserType = (int)IOBuff.GetUChar();
    return true;
}

