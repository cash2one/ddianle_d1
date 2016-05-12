
#include "GameMsg_Server_Login.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

GameMsg_Server_Login::GameMsg_Server_Login(void)
    : GameMsg_Base( MSG_SERVER_Login )
    , m_nInstallerID( 0 )
    , m_nVersionType( 0 )
    , m_bSwitchLine( false )
{
}

GameMsg_Server_Login::~GameMsg_Server_Login(void)
{
}

bool GameMsg_Server_Login::doDecode( CParamPool& IoBuff )
{
    m_nInstallerID = IoBuff.GetUChar();
    m_nVersionType = IoBuff.GetUChar();
    m_bSwitchLine = IoBuff.GetBool();

    return true;
}

//end

