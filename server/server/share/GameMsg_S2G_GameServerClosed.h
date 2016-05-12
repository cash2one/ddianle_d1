/*
 * GameMsg_S2G_GameServerClosed.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-3-18 15:30:33
 */

#ifndef __GAMEMSG_S2G_GAMESERVERCLOSED_H__
#define __GAMEMSG_S2G_GAMESERVERCLOSED_H__

#include "../../socket/GameMsg_Base.h"

/**
* GameMsg_S2G_GameServerClosed:
* 
*/

class GameMsg_S2G_GameServerClosed : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GameServerClosed);
    GameMsg_S2G_GameServerClosed();
    virtual ~GameMsg_S2G_GameServerClosed() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);
};

#endif // __GAMEMSG_S2G_GAMESERVERCLOSED_H__

