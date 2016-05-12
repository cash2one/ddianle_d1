#ifndef __COMMAND_HANDLER_MANAGER_H__
#define __COMMAND_HANDLER_MANAGER_H__

#include <string>
#include <map>


class ICmdHandler;

class CCmdHanderManager
{
private:
    CCmdHanderManager();
    ~CCmdHanderManager();
public:
    static CCmdHanderManager &Instance();
public:
    void Init();
    void Add(ICmdHandler *pHandler);
    void Remove(const std::string & cmd);
    ICmdHandler *GetCmdHandler(const std::string &cmd);
private:
    std::map<std::string, ICmdHandler *> m_mapCmdHandler;
};

#endif //__COMMAND_HANDLER_MANAGER_H__

