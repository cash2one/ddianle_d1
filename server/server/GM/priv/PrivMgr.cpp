#include "PrivMgr.h"
#include <vector>
#include "../../../socket/DBQuery_Map.h"
#include "../GMQueryType.h"
#include "../GMDataStruct.h"
#include "../GMQuery.h"

extern CGMQueryMgr*	g_pGMQueryMgr;


CPrivMgr::CPrivMgr()
	: GameMsg_Processor(false,true)
{
}

CPrivMgr::~CPrivMgr()
{
}

CPrivMgr & CPrivMgr::Instance()
{
	static CPrivMgr s_PrivMgr;
	return s_PrivMgr;
}

void CPrivMgr::Init()
{
	RegQuery();
	LoadUsers();
}

void CPrivMgr::RegQuery()
{
	RegQueryProcessor(Query_LoadUsers,&CPrivMgr::LoadUsersCallback);
}

void CPrivMgr::AddCmdInfo(string strCmd, unsigned int nPriv)
{
	map< unsigned int, list<string> >::iterator itr = m_mapCmd.find(nPriv);
	if (itr != m_mapCmd.end()) {
		itr->second.push_back(strCmd);
	} else {
		list<string> listString;
		listString.push_back(strCmd);
		m_mapCmd.insert(make_pair(nPriv, listString));
	}
}

const list<string> * CPrivMgr::GetCmdsByPriv(int nPriv)
{
	map< unsigned int, list<string> >::iterator itr = m_mapCmd.find(nPriv);
	if (itr != m_mapCmd.end()) {
		return &(itr->second);
	}

	return NULL;
}

int CPrivMgr::GetUsers(std::map<std::string,int>& userMap)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.begin();
	for (; it != m_UserMap.end() ; it++ )
	{
		userMap.insert(make_pair(it->first,it->second.m_nPriv));
	}
	return userMap.size();
}

void CPrivMgr::LoadUsers()
{
	std::vector<CGMRoleInfoDB>* pRoleVec = new std::vector<CGMRoleInfoDB>();
	g_pGMQueryMgr->AddQuery(Query_LoadUsers,0,pRoleVec,0,NULL,NULL);
}

bool CPrivMgr::GetUserPriv(std::string& szName,int* priv /* = NULL */)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.find(szName);
	if (it == m_UserMap.end())
	{
		return false;
	}
	if (priv != NULL)
	{
		*priv = it->second.m_nPriv;
	}
	return true;
}

void CPrivMgr::LoadUsersCallback(QueryBase& query)
{
	std::vector<CGMRoleInfoDB>* pRoleVec = (std::vector<CGMRoleInfoDB>*)query.m_pAttachment;
	if (query.m_nResult == 0)
	{
		m_UserMap.clear();
		std::vector<CGMRoleInfoDB>::iterator it = pRoleVec->begin();
		for( ; it != pRoleVec->end(); it++)
		{
			std::string userName = it->m_szName;
			m_UserMap.insert(make_pair(it->m_szName,*it));
		}
	}
	pRoleVec->clear();
	delete pRoleVec;
}

bool CPrivMgr::HasPriv(string& szName,int priv)
{
	int nUserPriv = 0;
	if (GetUserPriv(szName,&nUserPriv))
	{
		return (nUserPriv & priv) != 0;
	}
	return false;
}

bool CPrivMgr::HasPriv(const char* szName,int priv)
{
	std::string name = szName;
	return HasPriv(name,priv);
}

void CPrivMgr::SetUserPriv(const char* szName,int priv)
{
	std::string name = szName;
	SetUserPriv(name,priv);
}

void CPrivMgr::SetUserPriv(string& szName,int priv)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.find(szName);
	if (it != m_UserMap.end())
	{
		it->second.m_nPriv = priv;
	}
}

void CPrivMgr::DeleteUser(const char* szName)
{
	std::string name = szName;
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.find(name);
	if (it != m_UserMap.end())
	{
		m_UserMap.erase(it);
	}
}

std::string CPrivMgr::GetUserPassword(string& szName)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.find(szName);
	if (it != m_UserMap.end())
	{
		return it->second.m_szPassword;
	}
	else
	{
		return "";
	}
}

std::string CPrivMgr::GetUserPassword(const char* szName)
{
	std::string name = szName;
	return GetUserPassword(name);
}

bool CPrivMgr::SetUserPassword(unsigned int nAccount,const char* newPassword)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.begin();
	for (; it != m_UserMap.end() ; it++)
	{
		if (it->second.m_nAccount == nAccount)
		{
			strncpy(it->second.m_szPassword,newPassword,GM_MAX_PASSWD_LEN);
			return true;
		}
	}
	return false;
}

bool CPrivMgr::GetUser(std::string& szName,CGMRoleInfoDB* & pRoleInfo)
{
	std::map<std::string,CGMRoleInfoDB>::iterator it = m_UserMap.find(szName);
	if (it != m_UserMap.end())
	{
		pRoleInfo = &(it->second);
		return true;
	}
	return false;
}

