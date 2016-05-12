#ifndef		__GMENTITY_H__
#define		__GMENTITY_H__

#include <list>
#include "GMDefine.h"
#include "GMDataStruct.h"

class GameMsg_Base;

using namespace std;

class GMEntity
{
public:
    GMEntity();
    ~GMEntity();
public:
    void SendMsgToGM(GameMsg_Base *pMsg);
    void SendMsgToGroup(GameMsg_Base *pMsg);
public:
    void CreateFromDB(CGMRoleInfoDB & roleDB);
    char *GetName();

    void SetAccount(unsigned int nAccount);
    unsigned int GetAccount();

    void SetSession(__int64 nSession);
    __int64 GetSession();

    void SetSlot(unsigned short nSlot);
    unsigned short GetSlot();

    bool HasCmdPriv(string & strCmd);
private:
    char m_szName[GM_MAX_NAME_LEN];
    unsigned int	m_nAccount;
    __int64			m_nSessionID;
    unsigned short	m_nSlot;
    list<unsigned int>		m_listPriv;
    list<string>	m_listGrantedCmds;
    list<string>	m_listAllCmds;
    list<string>	m_listDisabledCmds;
};

#endif

