#include "GameMsg_Forward.h"

#include "../../socket/ParamPool.h"
#include "../../socket/GameMsg_Map.h"
#include "ServerMsgDef.h"

GameMsg_Forward::GameMsg_Forward()
: GameMsg_Base(MSG_S2S_Forward)
{
    m_nForwardMsgID = 0;
    m_forwardMsg = NULL;
    m_bDeleteMsg = false;
}

GameMsg_Forward::~GameMsg_Forward()
{
    if (m_forwardMsg != NULL && m_bDeleteMsg)
    {
        delete m_forwardMsg;
        m_forwardMsg = NULL;
    }
}

bool GameMsg_Forward::doEncode(CParamPool & IoBuff)
{
    IoBuff.AddUShort((unsigned short)m_RoleIDList.size());
    for (std::list<unsigned int>::iterator it = m_RoleIDList.begin(); it != m_RoleIDList.end(); ++it)
    {
        IoBuff.AddUInt(*it);
    }

    if (m_forwardMsg != NULL && m_nForwardMsgID > 0)
    {
        IoBuff.AddUShort(m_nForwardMsgID);
        m_forwardMsg->doEncode(IoBuff);
    }
    else
    {
        IoBuff.AddUShort(0);
    }

    return true;
}

bool GameMsg_Forward::doDecode(CParamPool & IoBuff)
{
    unsigned short nRoleCount = IoBuff.GetUShort();
    for (unsigned short i = 0; i < nRoleCount; i++)
    {
        m_RoleIDList.push_back(IoBuff.GetUInt());
    }

    m_nForwardMsgID = IoBuff.GetUShort();
    if (m_nForwardMsgID > 0)
    {
        if (m_forwardMsg != NULL && m_bDeleteMsg)
        {
            delete m_forwardMsg;
        }
        else
        {
            m_bDeleteMsg = true;
        }
        m_forwardMsg = GameMsg_Map::Instantce().CreateMsgFromType(m_nForwardMsgID);
        if (m_forwardMsg != NULL)
        {
            m_forwardMsg->doDecode(IoBuff);
        }
    }

    return true;
}
