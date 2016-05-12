#ifndef		__PRIVMGR_H__
#define		__PRIVMGR_H__

#include <string>
#include <list>
#include <map>
#include "../../../socket/GameMsg_Processor.h"

using namespace std;

class CGMQueryMgr;
class CGMRoleInfoDB;

class CPrivMgr  : public GameMsg_Processor
{
private:
	CPrivMgr();
	~CPrivMgr();
public:
	static CPrivMgr & Instance();
	void AddCmdInfo(string strCmd, unsigned int nPriv);
	const list<string> * GetCmdsByPriv(int nPriv);

	void LoadUsers();

	int GetUsers(std::map<std::string,int>& userMap);
	bool GetUser(std::string& szName,CGMRoleInfoDB* & pRoleInfo);

	bool GetUserPriv(std::string& szName,int* priv = NULL);

	void Init();

	bool HasPriv(string& szName,int priv);
	bool HasPriv(const char* szName,int priv);

	void SetUserPriv(string& szName,int priv);
	void SetUserPriv(const char* szName,int priv);

	void DeleteUser(const char* szName);

	std::string GetUserPassword(string& szName);
	std::string GetUserPassword(const char* szName);
	bool SetUserPassword(unsigned int nAccount,const char* newPassword);

private:
	void RegQuery();
	void LoadUsersCallback(QueryBase& query);

public:
	map< unsigned int, list<string> > m_mapCmd;
	std::map<std::string,CGMRoleInfoDB> m_UserMap;
};

#endif

