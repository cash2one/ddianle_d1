#include "GameServerLinkInfo.h"

#include "../../socket/ParamPool.h"
#include "../../datastructure/Macro_Define.h"


GameServerLinkInfo::GameServerLinkInfo()
{
    Reset();
}

void GameServerLinkInfo::Reset()
{
    m_nAddr = 0;
    m_nPort = 0;
    m_nGroup = INVALID_GROUP;
    m_nLine = INVALID_LINE;
    m_nSlot = 0;
    m_nClient = 0;
    m_strName.clear();
    m_nInstallerType = INSTALLER_INVALID;
    m_bEnableAudition = false;
}

bool GameServerLinkInfo::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nAddr << m_nPort << m_nGroup 
        << m_nLine << m_nSlot << m_nClient 
        << m_strName << m_nInstallerType 
        << m_bEnableAudition;

    return true;
}

bool GameServerLinkInfo::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nAddr >> m_nPort >> m_nGroup 
        >> m_nLine >> m_nSlot >> m_nClient 
        >> m_strName >> m_nInstallerType 
        >> m_bEnableAudition;

    return true;
}

