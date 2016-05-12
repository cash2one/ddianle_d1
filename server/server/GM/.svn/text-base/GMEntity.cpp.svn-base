#include <algorithm>
#include "GMEntity.h"
#include "GMCirculator.h"
#include "priv/PrivMgr.h"

GMEntity::GMEntity()
:m_nAccount(0)
, m_nSessionID(0)
, m_nSlot(0)
{
    memset(m_szName, 0, sizeof(char)* GM_MAX_NAME_LEN);
}

GMEntity::~GMEntity()
{
}

void GMEntity::SendMsgToGM(GameMsg_Base *pMsg)
{
    g_pGMCirculator->SendMsgToGM(pMsg, m_nSlot);
}

void GMEntity::SendMsgToGroup(GameMsg_Base *pMsg)
{
    pMsg->SessionID = m_nSessionID;
    g_pGMCirculator->SendMsgToGroup(pMsg);
}

void GMEntity::CreateFromDB(CGMRoleInfoDB & roleDB)
{
    memcpy(m_szName, roleDB.m_szName, GM_MAX_NAME_LEN - 1);
    m_nAccount = roleDB.m_nAccount;
    m_nSessionID = roleDB.m_nSessionID;
    m_nSlot = roleDB.m_nSlot;
    m_listPriv = roleDB.m_listPrivs;
    m_listGrantedCmds = roleDB.m_listGrantedCmds;
    m_listDisabledCmds = roleDB.m_listDisabledCmds;

    m_listAllCmds = m_listGrantedCmds;
    for (list<unsigned int>::iterator itr = m_listPriv.begin(); itr != m_listPriv.end(); ++itr)
    {
        const list<string> *pListCmd = CPrivMgr::Instance().GetCmdsByPriv(*itr);
        if (pListCmd != NULL)
        {
            for (list<string>::const_iterator it = pListCmd->begin(); it != pListCmd->end(); ++it)
                m_listAllCmds.push_back(*it);
        }
    }
    for (list<string>::iterator itr = m_listDisabledCmds.begin(); itr != m_listDisabledCmds.end(); ++itr)
    {
        list<string>::iterator disabledItr = find(m_listAllCmds.begin(), m_listAllCmds.end(), *itr);
        if (disabledItr != m_listAllCmds.end())
            m_listAllCmds.erase(disabledItr);
    }
}

char *GMEntity::GetName()
{
    return m_szName;
}

void GMEntity::SetAccount(unsigned int nAccount)
{
    m_nAccount = nAccount;
}

unsigned int GMEntity::GetAccount()
{
    return m_nAccount;
}

void GMEntity::SetSession(__int64 nSession)
{
    m_nSessionID = nSession;
}

__int64 GMEntity::GetSession()
{
    return m_nSessionID;
}

void GMEntity::SetSlot(unsigned short nSlot)
{
    m_nSlot = nSlot;
}

unsigned short GMEntity::GetSlot()
{
    return m_nSlot;
}

bool GMEntity::HasCmdPriv(string & strCmd)
{
    return (find(m_listAllCmds.begin(), m_listAllCmds.end(), strCmd)) != m_listAllCmds.end();
}

