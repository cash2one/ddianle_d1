#include <set>
#include "../../socket/Windefine.h"
#include "GMQuery.h"
#include "../../socket/Ini.h"
#include "../../socket/Log.h"
#include "../../socket/Utility.h"
#include "../../datastructure/DataStruct_DB.h"

extern CIni g_Config;
//extern CIni g_Config;

#ifdef WIN32
#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#else
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#endif


CGMQueryMgr::CGMQueryMgr()
{
}

CGMQueryMgr::~CGMQueryMgr()
{
}

bool CGMQueryMgr::OpenDatabase()
{
	bool bRes = true;

	char szDatabaseServer[MAX_NAMEPASS];//Data base server name
	char szDatabase[MAX_NAMEPASS];//database name
	char szDatabaseUser[MAX_NAMEPASS];//Data base user name
	char szDatabasePassword[MAX_NAMEPASS];//data base user password
	unsigned int nPort = 0;

	if(bRes)
	{
		g_Config.GetString("GMdb", "server", szDatabaseServer, MAX_NAMEPASS, "");
		g_Config.GetString("GMdb", "database", szDatabase, MAX_NAMEPASS, "");
		g_Config.GetString("GMdb", "user", szDatabaseUser, MAX_NAMEPASS, "");
		g_Config.GetString("GMdb", "password", szDatabasePassword, MAX_NAMEPASS, "");
		nPort = g_Config.GetInt("GMdb", "port", 0);
		bRes = m_dbGM.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseServer, nPort);
		TRACE("Open nsytgmdb %s!\n", bRes ? "OK" : "Failed");
	}

    if(bRes)
    {
        g_Config.GetString("SlaveDb", "server", szDatabaseServer, MAX_NAMEPASS, "");
        g_Config.GetString("SlaveDb", "database", szDatabase, MAX_NAMEPASS, "");
        g_Config.GetString("SlaveDb", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("SlaveDb", "password", szDatabasePassword, MAX_NAMEPASS, "");
        nPort = g_Config.GetInt("SlaveDb", "port", 0);
        bRes = m_dbSlaveGM.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseServer, nPort);
        TRACE("Open GmSlaveDb %s!\n", bRes ? "OK" : "Failed");
    }

    if (bRes)
    {
        g_Config.GetString("dbSubLog", "server", szDatabaseServer, MAX_NAMEPASS, "");
        g_Config.GetString("dbSubLog", "database", szDatabase, MAX_NAMEPASS, "");
        g_Config.GetString("dbSubLog", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("dbSubLog", "password", szDatabasePassword, MAX_NAMEPASS, "");
        nPort = g_Config.GetInt("SlaveDb", "port", 3306);
        bRes = m_dbSubLog.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseServer, nPort);
        TRACE("Open SubLogDB %s!\n", bRes ? "OK" : "Failed");
    }

    
    char szUintBuff[MAX_NAMEPASS] = { 0 };
    if (bRes)
    { // use szDatabasePassword
        g_Config.GetString("RoleIDRange", "minRange", szUintBuff, MAX_NAMEPASS, "0");
        m_nMinRoleRange = (unsigned int)atoi64(szUintBuff);
        g_Config.GetString("RoleIDRange", "maxRange", szUintBuff, MAX_NAMEPASS, "0");
        m_nMaxRoleRange = (unsigned int)atoi64(szUintBuff);
    }

	return bRes;
}

int CGMQueryMgr::ProcessQuery(QueryBase* &pQuery)
{
	if (pQuery == NULL) {
		return 0;
	}

	int nRes = 0;
	switch (pQuery->m_nQueryType)
	{
	case Query_GMCreateAccount:
		{
			WriteLog(LOGLEVEL_DEBUG, "Hi, this is GM Query:Query_GMCreateAccount manager process function!\n");
		}
		break;
	case Query_GetGMAccInfo:
		{
			GMAccountInfo	*pAccountInfo = (GMAccountInfo	*)pQuery->m_pAttachment;
			if (pQuery != NULL) {
				nRes = m_dbGM.GetGMAccInfo(*pAccountInfo);	
			}
		}
		break;
	case Query_GetGMRoleInfo:
		{
			CGMRoleInfoDB *pRoleDB = (CGMRoleInfoDB *)pQuery->m_pAttachment;	
			if (pRoleDB != NULL) {
				nRes = m_dbGM.GetGMRoleInfo(*pRoleDB);
			}
		}
		break;
	case Query_InsertGrantedCmd:
		{
			char *pName = (char *)pQuery->m_pAttachment;
			list<string> *pListCmd = (list<string> *)pQuery->m_pTag;

			if (pName != NULL && pListCmd != NULL) {
				nRes = m_dbGM.InsertGrantedCmd(pName, *pListCmd, pQuery->m_nAttachment);
			}
		}
		break;
	case Query_RemoveGrantedCmd:
		{
			char *pName = (char *)pQuery->m_pAttachment;
			list<string> *pListCmd = (list<string> *)pQuery->m_pTag;

			if (pName != NULL && pListCmd != NULL) {
				nRes = m_dbGM.RemoveGrantedCmd(pName, *pListCmd, pQuery->m_nAttachment);
			}
		}
		break;
	case Query_InsertSysMailLog:
		{
			CGMSysMailLog *pSysLog = (CGMSysMailLog *)pQuery->m_pAttachment;
			if (pSysLog != NULL ) {
				nRes = m_dbGM.InsertSysMailLog(*pSysLog);
			}
		}
		break;
	case Query_AddAnnounce:
		{
			CAnnounceInfo *pRecord = (CAnnounceInfo *)pQuery->m_pAttachment;
			if (pRecord != NULL) {
				nRes = m_dbGM.AddAnnouce(*pRecord);
			}
		}
		break;
	case Query_UpdateAnnouce:
		{
			CAnnounceInfo *pRecord = (CAnnounceInfo *)pQuery->m_pAttachment;
			if (pRecord != NULL) {
				nRes = m_dbGM.UpdateAnnouce(*pRecord);
				delete pRecord, pRecord = NULL;
			}
		}
		break;
	case Query_RemoveAnnouce:
		{
			CAnnounceInfo *pRecord = (CAnnounceInfo *)pQuery->m_pAttachment;
			if (pRecord != NULL) {
				nRes = m_dbGM.RemoveAnnounce(*pRecord);
				delete pRecord, pRecord = NULL;
			}
		}
		break;
	case Query_LoadAnnounce:
		{

			std::map< unsigned int,  CAnnounceInfo * >* pMapAnnouce = (std::map< unsigned int,  CAnnounceInfo * > *)pQuery->m_pAttachment;
			if (pMapAnnouce != NULL) {
				nRes = m_dbGM.LoadAnnounce(*pMapAnnouce);
			}
		}
		break;
	case Query_AddUser:
		{
			CGMRoleInfoDB* pRole = (CGMRoleInfoDB*) pQuery->m_pAttachment;
			nRes = m_dbGM.AddUser(pRole->m_szName,pRole->m_szPassword);
		}
		break;
	case Query_LoadUsers:
		{
			std::vector<CGMRoleInfoDB>* pRoleVec = (std::vector<CGMRoleInfoDB>*)pQuery->m_pAttachment;
			nRes = m_dbGM.LoadUsers(pRoleVec);
		}
		break;
	case Query_SetPriv:
		{
			CGMRoleInfoDB* pRole = (CGMRoleInfoDB*)pQuery->m_pAttachment;
			nRes = m_dbGM.SetUserPriv(pRole);
		}
		break;
	case Query_DelUser:
		{
			CGMRoleInfoDB* pRole = (CGMRoleInfoDB*)pQuery->m_pAttachment;
			nRes = m_dbGM.DelUser(pRole);
		}
		break;
	case Query_Log:
		{
			unsigned int nAccount = (unsigned int)pQuery->m_nTag;
			int nAction = pQuery->m_nAttachment;
			char* szParam = (char*)pQuery->m_pAttachment;
			if (szParam != NULL)
			{
				nRes = m_dbGM.WriteGMLog(nAccount,nAction,szParam);
				delete szParam,szParam = NULL;
			}
		}
		break;
    case Query_LogLoad:
        { // ¼ÓÔØGMÈÕÖ¾
            QueryLogDb* pParam = (QueryLogDb*)pQuery->m_pAttachment;
            if (NULL != pParam)
            {
                nRes = m_dbGM.LoadGMLog(pParam);
            }
        }
        break;
    case Query_LogItemLoad:
    {
        QueryLogItemDb *pParam = (QueryLogItemDb *)pQuery->m_pAttachment;
        if (NULL != pParam)
        {
            nRes = m_dbGM.LoadGMLogItem(pParam);
        }
        break;
    }
	case Query_ChangePassword:
		{
			unsigned int nAccount = (unsigned int)pQuery->m_nAttachment;
			const char* szNewPassword = (const char* )pQuery->m_pAttachment;
			nRes = m_dbGM.ChangePassword(nAccount,szNewPassword);
		}
		break;

    case Query_WriteLockPlayerRecord:
        {
            PlayerLockRequestDb* request = (PlayerLockRequestDb*)pQuery->m_pAttachment;
            if (NULL != request)
            {
                nRes = m_dbGM.WriteUserLockRecord(*request);
                delete request;
                request = NULL;
                pQuery->m_pAttachment = NULL;
            }
        }
        break;
    case Query_PlayerLockQuery:
        {
            PlayerLockQueryDb *request = (PlayerLockQueryDb *)pQuery->m_pAttachment;
            if (NULL != request)
            {
                nRes = m_dbGM.LoadUserLockRecord(*request);
            }
        }
        break;
    case Query_RechagreRecordList:
        {
            IosForbidRechargeDB * request = (IosForbidRechargeDB *)pQuery->m_pAttachment;
            if (request != NULL)
            {
                nRes = m_dbGM.LoadRechargeRecord(*request);
            }
        }
        break;
    case Query_HarassPlayers:
        {
            HarassReqDb *harassDb = (HarassReqDb*)pQuery->m_pAttachment;
            if (NULL != harassDb)
            {
                RoleHarassInfoAMap filterMap;
                std::set<unsigned int> roleSet;
                nRes = m_dbSubLog.LoadHarassInfo(*harassDb, filterMap, roleSet, m_nMinRoleRange, m_nMaxRoleRange);
                if (nRes == Query_Res_OK && !roleSet.empty())
                {
                    nRes = m_dbSlaveGM.LoadHarassRoleInfo(*harassDb, filterMap, roleSet);
                }
            }
        }
        break;
    case Query_HarassPlayerChat:
        {
            HarassChatReqDb *harassChat = (HarassChatReqDb*)pQuery->m_pAttachment;
            if (NULL != harassChat)
            {
                nRes = m_dbSubLog.LoadHarassChatInfo(*harassChat);
                if (nRes == Query_Res_OK)
                    nRes = m_dbSlaveGM.LoadPlayerInfoForHarassChatInfo(*harassChat);
            }
        }
        break;

	default:
		break;
	}

	return nRes;
}

int CGMQueryMgr::GetQueueSize()
{
	int nQueueSize = 0;
	Lock(&m_lockQuery);
	nQueueSize = (int)m_apQueryQueue.size();
	Unlock(&m_lockQuery);
	return nQueueSize;
}

bool CGMQueryMgr::GetThreadState()
{
	return m_bRunning;
}

bool CGMQueryMgr::RecreateThread()
{
	m_bRunning = true;
#ifdef WIN32
	m_hThreadQuery = CreateThread(NULL, 0, QueryThreadProc, this, 0, NULL);
	if(m_hThreadQuery == NULL)
	{
		TRACE("GM serverCreate Query Thread failed!\n");
		return false;
	}

#else
	m_bRunning = true;

	int nResult;
	nResult = pthread_create(&m_hThreadQuery, NULL, QueryThreadProc, this);
	if (nResult != 0)
	{
		TRACE("GM server Create Query Thread failed!\n");
		return false;
	}
#endif
	return true;
}

