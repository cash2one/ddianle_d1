/*
 * GameMsg_GameServerLinkInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-11-10 15:25:31
 */

#ifndef __GAMEMSG_GAMESERVERLINKINFO_H__
#define __GAMEMSG_GAMESERVERLINKINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "GameServerLinkInfo.h"

class GameMsg_S2G_GameServerLinkInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_GameServerLinkInfo)

    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    GameServerLinkInfo  m_linkInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GameServerLinkInfoResult : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_GameServerLinkInfoResult)

    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned char m_nServerType;
};

#endif // __GAMEMSG_GAMESERVERLINKINFO_H__

