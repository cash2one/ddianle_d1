/*
*       记录玩家是否为游客
*/

#ifndef __GameMsg_C2S_TouristSignInfo_H__
#define __GameMsg_C2S_TouristSignInfo_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_C2S_TouristSignInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_TouristSignInfo);
    GameMsg_C2S_TouristSignInfo(void);
    virtual ~GameMsg_C2S_TouristSignInfo(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    int m_nUserType;
};

#endif //__GameMsg_C2S_TouristSignInfo_H__

