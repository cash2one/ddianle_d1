#ifndef		__GAMEMSG_CACHEENT_H_
#define		__GAMEMSG_CACHEENT_H_

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_PlayerCacheTransferStart :
	public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_PlayerCacheTransferStart);
    GameMsg_PlayerCacheTransferStart();
    ~GameMsg_PlayerCacheTransferStart();
public:
	bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nAccountID;
};


class GameMsg_PlayerCacheTransfer :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_PlayerCacheTransfer);
    GameMsg_PlayerCacheTransfer();
    ~GameMsg_PlayerCacheTransfer();
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
public:

    unsigned int m_nAccountID;
    unsigned short m_nKey;
    unsigned int m_nAllDataSize;
    unsigned int m_nThisSize;
    char *m_pData; // 

    bool m_bDeleteBuff;
};

class GameMsg_PlayerCacheTransferEnd :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_PlayerCacheTransferEnd);
    GameMsg_PlayerCacheTransferEnd();
    ~GameMsg_PlayerCacheTransferEnd();
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
public:

    unsigned int m_nAccountID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2A_RequestPlayerCache : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2A_RequestPlayerCache );

public:
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );
};

class GameMsg_A2S_RequestPlayerCacheRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_A2S_RequestPlayerCacheRes );

public:
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    bool            m_bCacheSuccess;
    bool            m_bHasCache;
};

#endif

