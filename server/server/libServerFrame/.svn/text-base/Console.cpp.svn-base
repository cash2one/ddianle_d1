#include "Console.h"

CommandPrivilegeManager::CommandPrivilegeManager()
{
}

CommandPrivilegeManager::~CommandPrivilegeManager()
{
}

bool CommandPrivilegeManager::AddCommandPrivilege(const std::string &strCommand, int nPriv)
{
    std::map<std::string, std::vector<unsigned int> >::iterator itr = m_mapCmdPrivilege.find(strCommand);
    if (itr != m_mapCmdPrivilege.end())
    {
        itr->second.push_back(nPriv);
    }
    else
    {
        std::vector<unsigned int> vecPriv;
        vecPriv.push_back(nPriv);
        m_mapCmdPrivilege.insert(make_pair(strCommand, vecPriv));
    }

    return true;
}

bool CommandPrivilegeManager::GetCommandPrivilege(const std::string &strCommand, std::vector<unsigned int> &vecPriv) const
{
    std::map<std::string, std::vector<unsigned int> >::const_iterator itr = m_mapCmdPrivilege.find(strCommand);
    if (itr != m_mapCmdPrivilege.end())
    {
        vecPriv = itr->second;
        return true;
    }

    return false;
}

bool CommandPrivilegeManager::HasCommandPrivilege(const std::string &strCommand, unsigned int nUserPriv) const
{
    std::vector<unsigned int> vecPrivilege;

    if (!GetCommandPrivilege(strCommand, vecPrivilege))
    {
        return false;
    }

    for (std::vector<unsigned int>::iterator it = vecPrivilege.begin(); it != vecPrivilege.end(); ++it)
    {
        unsigned int nPriv = *it;
        if ((nUserPriv & nPriv) == nPriv)
            return true;
    }

    return false;
}

