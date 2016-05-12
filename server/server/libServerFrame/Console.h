#ifndef __COMMAND_PRIVILEGE_MANAGER_H__
#define __COMMAND_PRIVILEGE_MANAGER_H__

#include <string>
#include <vector>
#include <map>

class CommandPrivilegeManager
{
public:
    CommandPrivilegeManager();
    virtual ~CommandPrivilegeManager();

public:
    // 添加命令的执行执行权限
    bool AddCommandPrivilege(const std::string &strCommand, int nPriv);

    // 查询命令的执行权限
    bool GetCommandPrivilege(const std::string &strCommand, std::vector<unsigned int> &vecPriv) const;

    // 指定的权限是否可以执行指定的命令
    bool HasCommandPrivilege(const std::string &strCommand, unsigned int nUserPriv) const;

protected:
    // 命令权限列表: key-> cmd name, value-> privilege vector, 只要具备vector中任意一个权限,就可以执行该命令
    std::map<std::string, std::vector<unsigned int> > m_mapCmdPrivilege;
};

#endif //__COMMAND_PRIVILEGE_MANAGER_H__

