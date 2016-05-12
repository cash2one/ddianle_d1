/*
 * GameMsg_Server_VerifyAccount.h
 * Description: 
 * Copyright (C) 2016 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2016-3-15 10:24:33
 */

#ifndef __GAMEMSG_SERVER_VERIFYACCOUNT_H__
#define __GAMEMSG_SERVER_VERIFYACCOUNT_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_S2A_VerifyAccount : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2A_VerifyAccount );

public:
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned short  m_nSlot;
    unsigned char   m_nInstallerID;
    int             m_nVersionType;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_A2S_VerifyAccountRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_A2S_VerifyAccountRes );

public:
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    bool            m_bVerifyingPassed;
    unsigned short  m_nSlot;
    unsigned char   m_nInstallerID;
    int             m_nVersionType;
};

#endif // __GAMEMSG_SERVER_VERIFYACCOUNT_H__
