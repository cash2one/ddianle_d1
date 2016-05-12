#include "GameMsgCacheEnt.h"
#include "../share/ServerMsgDef.h"

GameMsg_PlayerCacheTransferStart::GameMsg_PlayerCacheTransferStart()
:GameMsg_Base(MSG_PlayerCacheTransferStart)
, m_nAccountID(0)
{
}

GameMsg_PlayerCacheTransferStart::~GameMsg_PlayerCacheTransferStart()
{

}

bool GameMsg_PlayerCacheTransferStart::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nAccountID);
	return true;
}

bool GameMsg_PlayerCacheTransferStart::doDecode(CParamPool &IOBuff)
{
    m_nAccountID = IOBuff.GetUInt();
    return true;
}


GameMsg_PlayerCacheTransfer::GameMsg_PlayerCacheTransfer()
    :GameMsg_Base(MSG_PlayerCacheTransfer)
    , m_nAccountID(0)
    , m_nKey(0)
    , m_nAllDataSize(0)
    , m_nThisSize(0)
    , m_pData(NULL)
    , m_bDeleteBuff(false)
{
}

GameMsg_PlayerCacheTransfer::~GameMsg_PlayerCacheTransfer()
{
    if (m_bDeleteBuff)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
}

bool GameMsg_PlayerCacheTransfer::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nAccountID);
    IOBuff.AddUShort(m_nKey);
    IOBuff.AddUInt(m_nAllDataSize);
    IOBuff.AddUInt(m_nThisSize);
    if (0 != m_nThisSize)
    {
        IOBuff.AddMemory(m_pData, (unsigned short)m_nThisSize);
    }

    return true;
}

bool GameMsg_PlayerCacheTransfer::doDecode(CParamPool &IOBuff)
{
    m_nAccountID = IOBuff.GetUInt();
    m_nKey = IOBuff.GetUShort();
    m_nAllDataSize = IOBuff.GetUInt();
    m_nThisSize = IOBuff.GetUInt();
    if (0 != m_nThisSize)
    {
        m_pData = new char[m_nThisSize];
        IOBuff.GetMemory(m_pData, (unsigned short)m_nThisSize);
        m_bDeleteBuff = true;
    }

    return true;
}


GameMsg_PlayerCacheTransferEnd::GameMsg_PlayerCacheTransferEnd()
    :GameMsg_Base(MSG_PlayerCacheTransferEnd)
    , m_nAccountID(0)
{
}

GameMsg_PlayerCacheTransferEnd::~GameMsg_PlayerCacheTransferEnd()
{

}

bool GameMsg_PlayerCacheTransferEnd::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nAccountID);
    return true;
}

bool GameMsg_PlayerCacheTransferEnd::doDecode(CParamPool &IOBuff)
{
    m_nAccountID = IOBuff.GetUInt();
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2A_RequestPlayerCache::GameMsg_S2A_RequestPlayerCache()
    : GameMsg_Base( MSG_S2A_RequestPlayerCache )
{
}

GameMsg_S2A_RequestPlayerCache::~GameMsg_S2A_RequestPlayerCache()
{
}

bool GameMsg_S2A_RequestPlayerCache::doEncode( CParamPool &IOBuff )
{
    return true;
}

bool GameMsg_S2A_RequestPlayerCache::doDecode( CParamPool &IOBuff )
{
    return true;
}

GameMsg_A2S_RequestPlayerCacheRes::GameMsg_A2S_RequestPlayerCacheRes()
    : GameMsg_Base( MSG_A2S_RequestPlayerCacheRes )
    , m_bCacheSuccess( false )
    , m_bHasCache( false )
{
}

GameMsg_A2S_RequestPlayerCacheRes::~GameMsg_A2S_RequestPlayerCacheRes()
{
}

bool GameMsg_A2S_RequestPlayerCacheRes::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_bCacheSuccess << m_bHasCache;

    return true;
}

bool GameMsg_A2S_RequestPlayerCacheRes::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_bCacheSuccess >> m_bHasCache;

    return true;
}

