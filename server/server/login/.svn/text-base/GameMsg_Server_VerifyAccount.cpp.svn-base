#include "GameMsg_Server_VerifyAccount.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2A_VerifyAccount::GameMsg_S2A_VerifyAccount()
    : GameMsg_Base( MSG_S2A_VerifyAccount )
    , m_nSlot( 0 )
    , m_nInstallerID( 0 )
    , m_nVersionType( 0 )
{
}

GameMsg_S2A_VerifyAccount::~GameMsg_S2A_VerifyAccount()
{
}

bool GameMsg_S2A_VerifyAccount::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nSlot << m_nInstallerID << m_nVersionType;

    return true;
}

bool GameMsg_S2A_VerifyAccount::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nSlot >> m_nInstallerID >> m_nVersionType;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_A2S_VerifyAccountRes::GameMsg_A2S_VerifyAccountRes()
    : GameMsg_Base( MSG_A2S_VerifyAccountRes )
    , m_bVerifyingPassed( false )
    , m_nSlot( 0 )
    , m_nInstallerID( 0 )
    , m_nVersionType( 0 )
{
}

GameMsg_A2S_VerifyAccountRes::~GameMsg_A2S_VerifyAccountRes()
{
}

bool GameMsg_A2S_VerifyAccountRes::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_bVerifyingPassed << m_nSlot << m_nInstallerID << m_nVersionType;

    return true;
}

bool GameMsg_A2S_VerifyAccountRes::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_bVerifyingPassed >> m_nSlot >> m_nInstallerID >> m_nVersionType;

    return true;
}

