#include <stdio.h>
#include <stdlib.h>
#include "QueryManager.h"
#include "../../socket/Ini.h"
#include "../../socket/Windefine.h"
#include "../../socket/Formatter.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include "Main.h"
#include "../Handbook/HandbookStruct.h"
#include "../activity/RedEnvelope/RedEnvelope.h"

#ifdef WIN32
#define millisecondSleep(milliseconds)	Sleep((milliseconds))
#else
#include <unistd.h>
#define millisecondSleep(milliseconds)	usleep(1000*(milliseconds))
#endif

#ifdef WIN32
#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#else
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#endif

extern CIni g_GameConfig;
extern CIni g_Config;
extern int g_nGroup;
extern int g_nLine;
extern int g_DbWarningProcessTime;
extern int g_DbWarningWaitTime;
extern int g_DbWarningQueueSize;

int g_nDBAccessDelay = 0;

#ifdef WIN32
#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#define WaitThread(x)	{WaitForSingleObject(x, INFINITE);CloseHandle(x);}
#define SignalEvent(x)	{SetEvent(m_hEvent##x);}
#else
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#define WaitThread(x)	pthread_join(x, NULL)
#define SignalEvent(x)	{pthread_mutex_lock(m_pMutex##x);pthread_cond_signal(m_pCond##x);pthread_mutex_unlock(m_pMutex##x);}
#endif


CQueryManager::CQueryManager()
    : CQueryManagerBase()
{
    m_bEnableBackupAccountDB = false;

#ifdef WIN32
    InitializeCriticalSection(&m_lockLogQuery);
    std::string strName;
    Formatter(strName) << "EventLogQuery" << (unsigned int)this << GetTickCount();
    m_hEventLogQuery = CreateEvent(NULL, FALSE, FALSE, strName.c_str());
#else
    pthread_mutex_init(&m_lockLogQuery, NULL);
    pthread_cond_init(&m_condLogQuery, NULL);
    m_pMutexLogQuery = &m_lockLogQuery;
    m_pCondLogQuery = &m_condLogQuery;
#endif
}

CQueryManager::~CQueryManager()
{
#ifdef WIN32
    CloseHandle(m_hEventLogQuery);
    DeleteCriticalSection(&m_lockLogQuery);
#else
    pthread_mutex_destroy(&m_lockLogQuery);
    pthread_cond_destroy(&m_condLogQuery);
    m_pMutexLogQuery = NULL;
    m_pCondLogQuery = NULL;
#endif
    while (m_apLogQueryQueue.size() > 0)
    {
        delete m_apLogQueryQueue.front();
        m_apLogQueryQueue.pop_front();
    }
}

bool CQueryManager::OpenDatabase(bool bOpenAccountDB, bool bOpenRoleDB, bool bOpenLogDB, bool bOpenSubLogDB)
{
    bool bRes = true;

    char szDatabaseServer[MAX_NAMEPASS];//Data base server name
    char szDatabase[MAX_NAMEPASS];//database name
    char szDatabaseUser[MAX_NAMEPASS];//Data base user name
    char szDatabasePassword[MAX_NAMEPASS];//data base user password
    unsigned int nPort = 0;

    if (bRes && bOpenAccountDB)
    {
        g_Config.GetString("dbAccount", "server", szDatabaseServer, MAX_NAMEPASS, "");
        g_Config.GetString("dbAccount", "database", szDatabase, MAX_NAMEPASS, "");
        g_Config.GetString("dbAccount", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("dbAccount", "password", szDatabasePassword, MAX_NAMEPASS, "");
        nPort = g_Config.GetInt("dbAccount", "port", 0);
        bRes = m_dbiAccount.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseServer, nPort);
        WriteLog(LOGLEVEL_DEBUG, "Open AccountDB %s\n", bRes ? "OK" : "Failed");
    }
    if (bRes && bOpenRoleDB)
    {
        char szSecion[MAX_NAMEPASS];
        Formatter(szSecion, sizeof(szSecion)) << "group" << g_nGroup;
        g_Config.GetString(szSecion, "dbRole", szDatabaseServer, MAX_NAMEPASS, "");
        g_Config.GetString("dbRole", "database", szDatabase, MAX_NAMEPASS, "");
        g_Config.GetString("dbRole", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("dbRole", "password", szDatabasePassword, MAX_NAMEPASS, "");
        nPort = g_Config.GetInt(szSecion, "dbRolePort", 0);
        bRes = m_dbiRole.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseServer, nPort);
        WriteLog(LOGLEVEL_DEBUG, "Open RoleDB %s\n", bRes ? "OK" : "Failed");
    }
    if (bRes && bOpenLogDB)
    {
        bRes = m_dbiLog.OpenDatabase(szDatabaseUser, szDatabasePassword, "nsytlogdb", szDatabaseServer, nPort);
        WriteLog(LOGLEVEL_DEBUG, "Open LogDB %s\n", bRes ? "OK" : "Failed");
    }
    if (bRes && bOpenSubLogDB)//陆浩霆
    {
        char szSecion[MAX_NAMEPASS];
        Formatter(szSecion, sizeof(szSecion)) << "group" << g_nGroup;
        g_Config.GetString(szSecion, "dbSubLog", szDatabaseServer, MAX_NAMEPASS, "");
        g_Config.GetString("dbSubLog", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("dbSubLog", "password", szDatabasePassword, MAX_NAMEPASS, "");
        bRes = m_dbiSubLog.OpenDatabase(szDatabaseUser, szDatabasePassword, "nsytsublogdb", szDatabaseServer, nPort);
        WriteLog(LOGLEVEL_DEBUG, "Open SubLogDB %s\n", bRes ? "OK" : "Failed");
    }

    if (bRes)
    {
        string sLogPath;
        string sLogFileName;
        g_Config.GetString(g_sSection, "logpath", sLogPath, g_sPath.c_str());
        g_Config.GetString(g_sSection, "querylogfile", sLogFileName, "logs/query");
        int nLogMode = LOGMODE_FILE | LOGMODE_CR | LOGMODE_TRUNCATE;
        if (sLogFileName[0] != '/')
            sLogFileName = (sLogPath + sLogFileName);

        if (g_nGroup >= 0 && g_nLine >= 0)
            Formatter::Format(sLogFileName, "%s%02d-%02d", sLogFileName.c_str(), g_nGroup, g_nLine);

        g_DBLog.SetLogMode(nLogMode, sLogFileName.c_str());

        m_timeRecord = time(NULL);
    }

    if (bRes)
    {
        m_nLotteryMode = g_Config.GetInt("AccountServer", "lotterymode", 0);
    }

    // 创建DB线程
    if (bRes && bOpenLogDB)
    {
#ifdef WIN32
        m_hThreadLogQuery = CreateThread(NULL, 0, LogQueryThreadProc, this, 0, NULL);
        if (m_hThreadLogQuery == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "Create Log Query Thread failed!");
            return false;
        }
#else
        int nResult = pthread_create(&m_hThreadLogQuery, NULL, LogQueryThreadProc, this);
        if (nResult != 0)
        {
            WriteLog(LOGLEVEL_ERROR, "Create Log Query Thread failed!");
            return false;
        }
#endif
    }



    return bRes;
}

bool CQueryManager::OpenBackupAccountDB()
{
    bool bRes = true;

    int nEnableBackup = g_Config.GetInt("BackupAccountDB", "enable", 0);
    if (nEnableBackup != 0)
    {
        m_bEnableBackupAccountDB = true;

        char szDatabaseHost[MAX_NAMEPASS];
        char szDatabase[MAX_NAMEPASS];
        char szDatabaseUser[MAX_NAMEPASS];//Data base user name
        char szDatabasePassword[MAX_NAMEPASS];//data base user password
        unsigned int nPort = 0;

        g_Config.GetString("BackupAccountDB", "backupAccountDB", szDatabaseHost, MAX_NAMEPASS, "");

        g_Config.GetString("dbAccount", "database", szDatabase, MAX_NAMEPASS, "");
        g_Config.GetString("dbAccount", "user", szDatabaseUser, MAX_NAMEPASS, "");
        g_Config.GetString("dbAccount", "password", szDatabasePassword, MAX_NAMEPASS, "");
        nPort = g_Config.GetInt("dbAccount", "port", 0);

        bRes = m_dbiBackupAccountDB.OpenDatabase(szDatabaseUser, szDatabasePassword, szDatabase, szDatabaseHost, nPort);
        WriteLog(LOGLEVEL_DEBUG, "Open Backup AccountDB %s", bRes ? "OK" : "Failed");
    }

    return bRes;
}

int CQueryManager::GetThreadState()
{
    return m_bRunning;//&&  time(NULL) - m_timeRecord < 8*60;	
}

bool CQueryManager::RecreateThread()
{
    m_bRunning = true;
#ifdef WIN32
    m_hThreadQuery = CreateThread(NULL, 0, QueryThreadProc, this, 0, NULL);
    if (m_hThreadQuery == NULL)
    {
        TRACE("Create Query Thread failed!\n");
        return false;
    }

#else
    m_bRunning = true;

    int nResult;
    nResult = pthread_create(&m_hThreadQuery, NULL, QueryThreadProc, this);
    if (nResult != 0)
    {
        TRACE("Create Query Thread failed!\n");
        return false;
    }

#endif
    return true;

}

int CQueryManager::ProcessQuery(QueryBase* &pQuery)
{
    if (g_nDBAccessDelay >= 10)
    {
        millisecondSleep(g_nDBAccessDelay);
    }

    int nRes = 0;
    unsigned int nTickBegin = GetTickCount();
    switch (pQuery->m_nQueryType)
    {
    case QUERY_AccountLogin:
    {
        bool canCreateNew = true;
        nRes = m_dbiAccount.GetAccountInfo((LPACCOUNTINFO)pQuery->m_pAttachment, canCreateNew);

        if (!canCreateNew)
            pQuery->m_nAttachment = 1; // 标志获取角色失败
    }
        break;
    case QUERY_QuickCreateAccount:
    {
        CAutoAccount *pAutoAccount = (CAutoAccount *)pQuery->m_pAttachment;
        if (pAutoAccount != NULL){
            nRes = m_dbiAccount.UpdateAutoAccoutState(pAutoAccount->m_nAccountID, 0);
        }
    }
        break;
    case QUERY_CreateAccount:
    {
        // 没有使用多个AccountDB时，走原来的账号创建流程，使用多AccountDB时，需要在多个AccountDB
        // 上检查账号是否存在，并同时在多个AccountDB上创建账号 —— 方便后续平台系统合并账号数据
        AccountInfo *pAccount = (AccountInfo *)pQuery->m_pAttachment;
        if (!m_bEnableBackupAccountDB)
        {
            nRes = m_dbiAccount.CheckAndCreateAccount(pAccount);
        }
        else
        {
            int Res1 = m_dbiAccount.ExistAccountName(pAccount);
            int Res2 = m_dbiBackupAccountDB.ExistAccountName(pAccount);
            if (Res1 == Query_Res_OK && Res2 == Query_Res_OK)
            {
                Res1 = m_dbiAccount.CreateAccount(pAccount);
                if (Res1 == Query_Res_OK)
                {
                    AccountInfo backupAccount;
                    backupAccount = *pAccount;
                    backupAccount.nAccount = 0;
                    Res2 = m_dbiBackupAccountDB.CreateAccount(&backupAccount);
                    if (Res2 == Query_Res_OK)
                    {
                        Res1 = m_dbiAccount.CreateAccountBill(pAccount);
                        Res2 = m_dbiBackupAccountDB.CreateAccountBill(&backupAccount);
                    }
                }
            }

            if (Res1 == Query_Res_OK && Res2 == Query_Res_OK)
                nRes = Query_Res_OK;
            else
                nRes = Query_Res_Fail;
        }

        if (nRes == Query_Res_OK)
        {
            WriteLog(LOGLEVEL_DEBUG, "[ACCOUNT] create account=%s successed!", pAccount->szName);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "[ACCOUNT] create account=%s in double database failed!", pAccount->szName);
        }

    }
        break;
    case QUERY_CreateRole:
    {
        CreateRoleInfo *pRoleInfo = (CreateRoleInfo *)pQuery->m_pAttachment;
        int *pError = (int *)pQuery->m_pTag;
        if (pRoleInfo != NULL && pError != NULL) {
            nRes = m_dbiRole.CreateRole(pRoleInfo, *pError);
        }
    }
        break;
    case QUERY_CheckRoleName:
    {
        string *pStrRoleName = (string *)pQuery->m_pAttachment;

        if (pStrRoleName != NULL)
        {
            nRes = m_dbiRole.CheckRoleName(*pStrRoleName);
            //if (nRes == Query_Res_AlreadyExist)
            //{
            //	pQuery->m_nAttachment = 1;
            //}
        }
    }
        break;
    case QUERY_GetRegistedRoleCount:
        nRes = m_dbiRole.GetRegistedRoleCount(pQuery->m_nAttachment);
        break;
    case QUERY_GetRegistedPlayerCount:
    {
        nRes = m_dbiAccount.GetRegistedPlayerCount(pQuery->m_nAttachment);
    }
        break;
    case QUERY_GetBatchID64:
        nRes = m_dbiRole.GetBatchID64((__int64*)pQuery->m_pAttachment);
        break;
        //计费相关查询
    case QUERY_Bill_LoadInfo:
    {
        nRes = m_dbiAccount.LoadBillInfo((BILLINFO*)pQuery->m_pAttachment);
    }
        break;
    case QUERY_Bill_ChongZhi:
    {
        ChongZhiInfo * pChongZhiInfo = (ChongZhiInfo *)pQuery->m_pAttachment;
        if (pChongZhiInfo)
        {
            nRes = m_dbiAccount.UpdateBillPt(pChongZhiInfo);
        }
    }
        break;
    case QUERY_Bill_ChongZhiByPUID:
    {
        ChongZhiByPUIDInfo * pChongZhiInfo = (ChongZhiByPUIDInfo *)pQuery->m_pAttachment;
        if (pChongZhiInfo)
        {
            unsigned int nAccountID = 0;
            m_dbiAccount.GetAccountID(pChongZhiInfo->czInfo.nPID, pChongZhiInfo->strPUID, nAccountID);
            if (nAccountID == 0)
            {
                WriteLog(LOGLEVEL_ERROR, "ChongZhiByPUID. Account=0. PID=%u, PUID=%s, OrderID=%s, Type=%d, Amount=%u",
                    pChongZhiInfo->czInfo.nPID, pChongZhiInfo->strPUID.c_str(), pChongZhiInfo->czInfo.strOrderID.c_str(),
                    pChongZhiInfo->czInfo.nVersionType, pChongZhiInfo->czInfo.nAmount);
                pQuery->m_nResult = -1;
                nRes = -1;
            }
            else
            {
                pChongZhiInfo->czInfo.nAccount = nAccountID;
                nRes = m_dbiAccount.UpdateBillPt(&(pChongZhiInfo->czInfo));
            }
        }
    }
        break;
    case QUERY_Bill_UpdatePt:
    {
        nRes = m_dbiAccount.UpdateBillPt((unsigned int)pQuery->m_nTag, pQuery->m_nAttachment);
    }
        break;
    case QUERY_Bill_UpdateBindPt:
    {
        nRes = m_dbiAccount.UpdateBillBindPt((unsigned int)pQuery->m_nTag, pQuery->m_nAttachment);
    }
        break;
    case QUERY_Bill_UpdateChange:
    {
        //nRes = m_dbiAccount.UpdateBillChange((unsigned int)pQuery->m_nTag,pQuery->m_nAttachment);
    }
        break;
    case QUERY_Bill_UpdateChangeByUsername:
        //nRes = m_dbiAccount.UpdateBillChange((char*)pQuery->m_pAttachment, pQuery->m_nTag);
        //delete pQuery->m_pAttachment; pQuery->m_pAttachment = NULL;
        break;
    case QUERY_Mall_LoadInfo:
    {
        nRes = m_dbiRole.LoadMallInfo((list<MallGood*>*)pQuery->m_pAttachment, pQuery->m_nAttachment);
    }
        break;
    case QUERY_Mall_SaveLog:
    {
        nRes = m_dbiAccount.SaveMallLog((MALL_LOG *)pQuery->m_pAttachment);
        delete (MALL_LOG *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Mall_SigleItemLog:
    {
        std::vector<MallSigleItemLog> * pLog = (std::vector<MallSigleItemLog>*)pQuery->m_pAttachment;
        nRes = m_dbiAccount.SaveSingleItemLog(*pLog);
        delete (std::vector<MallSigleItemLog>*)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Mall_DesireNum:
    {
        nRes = m_dbiRole.SaveMallDesireNum((unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
    }
        break;
        // 	case QUERY_GetAccountInfo:
        // 		{
        // 			LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery->m_pAttachment;
        // 			nRes = m_dbiAccount.GetAccountInfo(pInfo);
        // 		}
        // 		break;
    case QUERY_UpdateAccountState:
    {
        LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery->m_pAttachment;
        nRes = m_dbiAccount.UpdateAccountState(pInfo);
        delete (LPACCOUNTINFO)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateAccountLockState:
    {
        LockAccountInfo* pInfo = (LockAccountInfo*)pQuery->m_pAttachment;
        if (pInfo)
        {
            unsigned int nAccountID = 0;
            m_dbiAccount.GetAccountID(pInfo->nPID, pInfo->strPUID, nAccountID);
            if (nAccountID == 0)
            {
                pQuery->m_nResult = -1;
                nRes = -1;
            }
            else
            {
                pInfo->nAccount = nAccountID;
                nRes = m_dbiAccount.UpdateAccountLockState(pInfo);
            }
        }
    }
        break;
    case QUERY_ResetAccountState:
    {
        nRes = m_dbiAccount.ResetAccountState((unsigned int)pQuery->m_nAttachment, (int)pQuery->m_nTag);
    }
        break;
    case QUERY_AccountLogout:
    {
        unsigned int nAccountID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nLoginTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiAccount.AccountLogout(nAccountID, nLoginTime);
    }
        break;
        // 	case QUERY_GetInviteAccountInfo:
        // 		{
        // 			LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery->m_pAttachment;
        // 			nRes = m_dbiAccount.GetAccountInfo(pInfo);
        // 		}
        // 		break;
    case QUERY_CheckAccountIsExist:
    {
        //LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery->m_pAttachment;
        //nRes = m_dbiAccount.GetAccountInfoByUUid(pInfo);
    }
        break;
    case QUERY_ACCOUNT_LoadAutoAccount:
    {
        list<CAutoAccount *> * pAutoAccountList = (list<CAutoAccount *> *)(pQuery->m_pAttachment);
        if (pAutoAccountList != NULL){
            nRes = m_dbiAccount.LoadAutoAccout(*pAutoAccountList);
        }
    }
        break;
    case QUERY_ACCOUNT_UpdateAutoAccountState:
    {
        nRes = m_dbiAccount.UpdateAutoAccoutState(pQuery->m_nAttachment, 0);
    }
        break;
    case QUERY_ACCOUNT_CreateNewAutoAccount:
    {
        list<CAutoAccount *> * pAutoAccountList = (list<CAutoAccount *> *)(pQuery->m_pAttachment);
        if (pAutoAccountList != NULL && pQuery->m_nAttachment > 0){
            nRes = m_dbiAccount.CreateNewRandomAccount(*pAutoAccountList, pQuery->m_nAttachment);
        }
    }
        break;
    case QUERY_ACCOUNT_LoadActivateCode:
    {
        map<unsigned int, unsigned char> *pMapActivateCode = (map<unsigned int, unsigned char> *)(pQuery->m_pAttachment);
        if (pMapActivateCode != NULL) {
            nRes = m_dbiAccount.LoadActivatedCode(*pMapActivateCode);
        }
    }
        break;
    case QUERY_ACCOUNT_ActivateCode:
    {
        unsigned int nCode = pQuery->m_nAttachment;
        AccountInfo *pInfo = (AccountInfo *)pQuery->m_pAttachment;

        if (pInfo != NULL) {
            nRes = m_dbiAccount.ActivateCode(nCode, pInfo);
        }
    }
        break;
    case QUERY_LoadCommands:
        nRes = m_dbiRole.LoadCommands((list< CMDINFO >*)pQuery->m_pAttachment);
        break;
    case QUERY_AddOnlineState:
    {
        nRes = m_dbiAccount.AddOnlineState(pQuery->m_nAttachment);
    }
        break;
    case QUERY_UpdateRole:
    {
        ROLEINFO_DB * pRoleInfoDB = (ROLEINFO_DB *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateRole(pRoleInfoDB);
        delete (ROLEINFO_DB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_MailGetRoleID:
    {
        Query<MailInfoDB> *rQuery = (Query<MailInfoDB> *)pQuery;
        if (NULL != rQuery && rQuery->IsValid() && NULL != rQuery->Get())
            nRes = m_dbiRole.MailGetRoleID(rQuery->Get());
    }
        break;
    case QUERY_LoadMailStrangerSetting:
    {
        Query<std::vector<unsigned int> > *pTmp = (Query<std::vector<unsigned int> > *)pQuery;
        if (NULL != pTmp && pTmp->IsValid() && NULL != pTmp->Get())
        {
            nRes = m_dbiRole.LoadMailStrangerSetting(*(pTmp->Get()));
        }
    }
        break;
    case QUERY_UpdateMailStrangerSetting:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        bool bReceive = (pQuery->m_nTag != 0);

        nRes = m_dbiRole.UpdateMailStrangerSetting(nRoleID, bReceive);
    }
        break;
    case QUERY_GetRoleInfo:
    {
        Query<ROLEINFO_DB> *pTmp = (Query<ROLEINFO_DB> *)pQuery;

        if ( pTmp != NULL && pTmp->IsValid() )
        {
            nRes = m_dbiRole.GetRoleInfo( pTmp->Get() );
        }
    }
        break;
    case QUERY_PlayerGetRoleInfo:
    {
        ROLEINFO_DB *pRoleInfo = (ROLEINFO_DB *)pQuery->m_pAttachment;
        nRes = m_dbiRole.GetRoleInfo(pRoleInfo);
    }
        break;
    case QUERY_ChangeRoleName:
    {
        nRes = m_dbiRole.UpdateRolenName((unsigned int)pQuery->m_nAttachment, (char *)pQuery->m_pAttachment);
        //delete (char *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_ChangeBirthday:
    {
        string *pNewBirthday = (string *)pQuery->m_pAttachment;
        unsigned int roleID = (unsigned int)pQuery->m_nAttachment;

        nRes = m_dbiRole.UpdateRoleBirthday(roleID, pNewBirthday);
        delete pNewBirthday, pQuery->m_pAttachment = pNewBirthday = NULL;
    }
        break;
    case QUERY_LoadOfflineRoleData:
    {
        nRes = m_dbiRole.LoadRoleOfflineData((unsigned int)pQuery->m_nAttachment, (RoleImageOfflineData *)pQuery->m_pAttachment);
    }
        break;
    case QUERY_LoadListOfflineRoleData:
    {
        std::set<unsigned int> * pSetRoleID = (std::set<unsigned int> *)pQuery->m_pAttachment;
        std::map<unsigned int, RoleImageOfflineData*> * pMapOfflineData = (std::map<unsigned int, RoleImageOfflineData*> *)pQuery->m_pTag;
        nRes = m_dbiRole.LoadRoleOfflineData(*pSetRoleID, *pMapOfflineData);
        delete pSetRoleID; pQuery->m_pAttachment = NULL;
        delete pMapOfflineData; pQuery->m_pTag = NULL;
    }
        break;
    case QUERY_LoadRoleOfflineTime:
    {
        unsigned int roleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int * pOfflineTime = (unsigned int*)pQuery->m_pAttachment;

        nRes = m_dbiRole.GetRoleOfflineTime(roleID, pOfflineTime);
    }
        break;
    case QUERY_LoadFriendOfflineTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        std::map<unsigned int, unsigned int*> * pFriendInfo = (std::map<unsigned int, unsigned int*> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.LoadFriendOfflineTime(nRoleID, pFriendInfo);
    }
        break;
    case QUERY_AddRoleItem:
    {
        nRes = m_dbiRole.AddRoleItem((unsigned int)pQuery->m_nTag, (DBItem*)pQuery->m_pAttachment);
        delete (DBItem*)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRoleItem:
    {
        nRes = m_dbiRole.UpdateRoleItem((unsigned int)pQuery->m_nTag, (DBItem*)pQuery->m_pAttachment);
        delete (DBItem*)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_AddRoleClothGroup:
    {
        nRes = m_dbiRole.AddRoleClothGroup((unsigned int)pQuery->m_nTag, (RoleClothGroup*)pQuery->m_pTag);
        delete (RoleClothGroup*)(pQuery->m_pTag), pQuery->m_pTag = NULL;
    }
        break;
    case QUERY_UpdateRoleClothGroup:
    {
        bool bUpdateItems = (pQuery->m_nAttachment != 0 ? true : false);
        nRes = m_dbiRole.UpdateRoleClothGroup((unsigned int)pQuery->m_nTag, (RoleClothGroup*)pQuery->m_pTag, bUpdateItems);
        delete (RoleClothGroup*)(pQuery->m_pTag), pQuery->m_pTag = NULL;
    }
        break;
    case QUERY_RemoveRoleItem:
        nRes = m_dbiRole.RemoveRoleItem(pQuery->m_nTag, (unsigned int)pQuery->m_nAttachment);
        break;
    case QUERY_RemoveDynamicBoxRelated:
        nRes = m_dbiRole.RemoveRoleDynamicBoxRelated((unsigned int)pQuery->m_nAttachment, pQuery->m_nTag);
        break;
    case QUERY_RemoveRoleItemByType:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTypeID = (unsigned int)pQuery->m_nTag;
        int *pCol = (int *)pQuery->m_pAttachment;
        if (pCol != NULL)
        {
            nRes = m_dbiRole.RemoveRoleItemByType(nRoleID, nTypeID, *pCol);
        }
        delete (int *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateCoolDown:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        CoolDownData *pCoolDown = (CoolDownData *)pQuery->m_pAttachment;
        if (pCoolDown != NULL) {
            m_dbiRole.UpdateCoolDown(nRoleID, *pCoolDown);
        }
        delete (CoolDownData *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateItemCount:
        nRes = m_dbiRole.UpdateItemCount((unsigned short)pQuery->m_nAttachment, pQuery->m_nTag);
        break;
    case QUERY_UpdateItemPosition:
    {
        nRes = m_dbiRole.UpdateItemPosition((ITEMINFO_Position*)pQuery->m_pAttachment);
        delete (ITEMINFO_Position *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Update2ItemPosition:
    {
        nRes = m_dbiRole.UpdateItemPosition2((ITEMINFO_Position2*)pQuery->m_pAttachment);
        delete (ITEMINFO_Position2 *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case  QUERY_UpdateItemOwner:
    {
        nRes = m_dbiRole.UpdateItemOwner((unsigned int)pQuery->m_nAttachment, pQuery->m_nTag);
    }
        break;
    case QUERY_InsertClothEffect:
    {
        ClothEffectDB * pCloth = (ClothEffectDB *)pQuery->m_pAttachment;
        if (pCloth != NULL)
        {
            nRes = m_dbiRole.InsertClothEffect(pCloth->m_nRoleID, pCloth->m_nItemID, pCloth->m_nEffectID);
            delete pCloth; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_UpdateClothEffect:
    {
        ClothEffectDB * pCloth = (ClothEffectDB *)pQuery->m_pAttachment;
        if (pCloth != NULL)
        {
            nRes = m_dbiRole.UpdateClothEffect(pCloth->m_nRoleID, pCloth->m_nItemID, pCloth->m_nEffectID);
            delete pCloth; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_InsertRoleEffect:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nEffectID = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.InsertRoleEffect(nRoleID, nEffectID);
    }
        break;
    case QUERY_InsertHandbookProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        ClothEffectHandbookDB * pHandbook = (ClothEffectHandbookDB *)pQuery->m_pAttachment;
        if (pHandbook != NULL)
        {
            nRes = m_dbiRole.InsertHandbookProgress(nRoleID, pHandbook);
            delete pHandbook, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_UpdateHandbookProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        ClothEffectHandbookDB * pHandbook = (ClothEffectHandbookDB *)pQuery->m_pAttachment;
        if (pHandbook != NULL)
        {
            nRes = m_dbiRole.UpdateHandbookProgress(nRoleID, pHandbook);
            delete pHandbook, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_InsertColorProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        ClothEffectHandbookDB * pColorProgress = (ClothEffectHandbookDB *)pQuery->m_pAttachment;
        if (pColorProgress != NULL)
        {
            nRes = m_dbiRole.InsertColorProgress(nRoleID, pColorProgress);
            delete pColorProgress, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_UpdateColorProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        ClothEffectHandbookDB * pColorProgress = (ClothEffectHandbookDB *)pQuery->m_pAttachment;
        if (pColorProgress != NULL)
        {
            nRes = m_dbiRole.UpdateColorProgress(nRoleID, pColorProgress);
            delete pColorProgress, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_InsertStorageCapacity:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        StorageCapacityRecord * pRecord = (StorageCapacityRecord *)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.InsertStorageCapacity(nRoleID, *pRecord);
            delete pRecord, pQuery->m_pAttachment = NULL;
        }
        break;
    }
    case QUERY_UpdateStorageCapacity:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        StorageCapacityRecord * pRecord = (StorageCapacityRecord *)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.UpdateStorageCapacity(nRoleID, *pRecord);
            delete pRecord, pQuery->m_pAttachment = NULL;
        }
        break;
    }
    case QUERY_UpdateRoleMoney:
    {
        nRes = m_dbiRole.UpdateRoleMoney((unsigned int)pQuery->m_nAttachment, (int)pQuery->m_nTag);
    }
        break;
    case QUERY_UpdateRoleMoneyByAccountId:
    {
        unsigned int nRoleID = 0;
        unsigned int nOldMoney = 0;
        nRes = m_dbiRole.UpdateRoleMoneyByAccountId((unsigned int)pQuery->m_nAttachment, (int)pQuery->m_nTag, nRoleID, nOldMoney);
        pQuery->m_nTag = nRoleID;
        pQuery->m_nAttachment = nOldMoney;
    }
        break;
    case QUERY_UpdateRoleBadgeNum:
    {
        nRes = m_dbiRole.UpdateRoleBadgeNum((unsigned int)pQuery->m_nTag, (unsigned char)pQuery->m_nAttachment);
    }
        break;
    case QUERY_UpdateRoleSignature:
    {
        std::string *pstrSignature = (std::string *)pQuery->m_pAttachment;
        if (pstrSignature != NULL)
        {
            nRes = m_dbiRole.UpdateRoleSignature((unsigned int)pQuery->m_nAttachment, pstrSignature->c_str());
        }
        delete pstrSignature, pQuery->m_pAttachment = pstrSignature = NULL;
    }
        break;
    case QUERY_AddRoleQuest:
    {
        nRes = m_dbiRole.AddRoleQuest((unsigned int)pQuery->m_nAttachment, (QUESTINFO *)pQuery->m_pAttachment);
    }
        break;
    case QUERY_UpdateTargetInfo:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;
        unsigned short nQuestID = (unsigned short)pQuery->m_nAttachment;
        TargetInfo *pQueryTargetInfo = (TargetInfo *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateQuestTargetEntry(nRoleID, nQuestID, pQueryTargetInfo);
        delete (TargetInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRoleQuest:
    {
        nRes = m_dbiRole.UpdateRoleQuest((unsigned int)pQuery->m_nAttachment, (QUESTINFO *)pQuery->m_pAttachment);
        delete (QUESTINFO *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateQuestInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        QUESTINFO *pQuestInfo = (QUESTINFO *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateQuestInfo(nRoleID, pQuestInfo);
        delete (QUESTINFO *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_RemoveRoleQuest:
        nRes = m_dbiRole.RemoveRoleQuest((unsigned int)pQuery->m_nAttachment, (unsigned short)pQuery->m_nTag);
        break;
    case QUERY_AddRoleQuestHistory:
    {
        nRes = m_dbiRole.AddRoleQuestHistory((unsigned int)pQuery->m_nAttachment, (short)pQuery->m_nTag, *((__int64 *)pQuery->m_pAttachment));
        delete (__int64 *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRoleQuestHistory:
    {
        nRes = m_dbiRole.UpdateRoleQuestHistory((unsigned int)pQuery->m_nAttachment, (short)pQuery->m_nTag, *((__int64 *)pQuery->m_pAttachment));
        delete (__int64 *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_RemoveRoleQuestHistory:
    {
        nRes = m_dbiRole.RemoveRoleQuestHistory((unsigned int)pQuery->m_nAttachment, (unsigned short)pQuery->m_nTag);
    }
        break;
    case QUERY_AddRepeatedQuestDone:
    {
        CRepeatedHistoryQuest *pInfo = (CRepeatedHistoryQuest *)pQuery->m_pAttachment;
        if (pInfo != NULL){
            nRes = m_dbiRole.AddRepeatedQuestDone((unsigned int)pQuery->m_nAttachment, *pInfo);
        }
        delete (CRepeatedHistoryQuest *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRepeatedQuestDone:
    {
        CRepeatedHistoryQuest *pInfo = (CRepeatedHistoryQuest *)pQuery->m_pAttachment;
        if (pInfo != NULL){
            nRes = m_dbiRole.UpdateRepeatedQuestDone((unsigned int)pQuery->m_nAttachment, *pInfo);
        }
        delete (CRepeatedHistoryQuest *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;

    case QUERY_GetBeAppliedPlayerInfo:
    {
        FriendInfo *pFriend = (FriendInfo *)pQuery->m_pAttachment;
        if (pFriend != NULL) {
            nRes = m_dbiRole.GetBeAppliedPlayerInfo(*pFriend);
        }
    }
        break;
    case QUERY_AddBlackListFriend:
    {
        int nRoleID = pQuery->m_nAttachment;
        FriendInfo *pFriend = (FriendInfo *)pQuery->m_pAttachment;
        if (pFriend != NULL) {
            nRes = m_dbiRole.AddBlackListFriend(nRoleID, *pFriend);
        }
    }
        break;
    case QUERY_AddRoleFriend:
    {
        nRes = m_dbiRole.AddRoleFriend((unsigned int)pQuery->m_nAttachment, ((FriendInfo*)pQuery->m_pAttachment)->m_nRoleID, (unsigned int)pQuery->m_nTag);
        //delete (FriendInfo*)pQuery->m_pAttachment;
        //pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_BecomeFriend:
    {
        FriendInfo *pFriendInfo = (FriendInfo *)pQuery->m_pAttachment;

        if (pFriendInfo != NULL){
            nRes = m_dbiRole.BeceomFriend(pQuery->m_nAttachment, pFriendInfo->m_nRoleID, pFriendInfo);
        }
    }
        break;
    case QUERY_ReworkFriendNotes:
    {
        FriendInfo *pFriendInfo = (FriendInfo *)pQuery->m_pAttachment;
        if (pFriendInfo != NULL)
        {
            nRes = m_dbiRole.ReworkFriendNotes((unsigned int)pQuery->m_nAttachment, pFriendInfo->m_nRoleID, pFriendInfo->m_strNotes);
            delete (FriendInfo *)pQuery->m_pAttachment; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_RemoveRoleFriend:
    {
        nRes = m_dbiRole.RemoveFriend((unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
    }
        break;
    case QUERY_AddBlackList:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        FriendInfo *pFriendInfo = (FriendInfo *)pQuery->m_pAttachment;
        if (pFriendInfo != NULL) {
            nRes = m_dbiRole.AddBlackList(nRoleID, *pFriendInfo);
        }
    }
        break;
    case QUERY_RemoveBlackList:
    {
        nRes = m_dbiRole.RemoveBlackList((unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
    }
        break;
        //case QUERY_RemoveRoleFriend:
        //	nRes = m_dbiRole.RemoveRoleFriend((unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
        //	break;
        //case QUERY_RemoveBlackList://函数同上,删除entry
        //	nRes = m_dbiRole.RemoveRoleFriend((unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
        //	break;
    case QUERY_UpdateFriendEntry:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        FriendInfo *pFriendInfo = (FriendInfo *)pQuery->m_pAttachment;
        unsigned char nType = (unsigned char)pQuery->m_nTag;

        if (pFriendInfo != NULL)
        {
            nRes = m_dbiRole.UpdateRoleFriend(nRoleID, pFriendInfo->m_nRoleID, nType, pFriendInfo->m_nCallTo);
        }
    }
        break;
    case QUERY_AddRoleGene:
    {
        nRes = m_dbiRole.AddRoleGene((unsigned int)pQuery->m_nAttachment, (GeneData *)pQuery->m_pAttachment);
        delete (GeneData *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DeleteRoleGene:
    {
        nRes = m_dbiRole.DeleteRoleGene((unsigned int)pQuery->m_nAttachment, (GeneData *)pQuery->m_pAttachment);
        delete (GeneData *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRoleGene:
    {
        nRes = m_dbiRole.UpdateRoleGene((unsigned int)pQuery->m_nAttachment, (GeneData *)pQuery->m_pAttachment);
        delete (GeneData *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateRoleBagExpand:
    {
        nRes = m_dbiRole.UpdateBagExpand((unsigned int)pQuery->m_nAttachment, (char *)pQuery->m_pAttachment);
        delete (char *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_LoadMail:
    {
        nRes = m_dbiRole.LoadRoleMail((unsigned int)pQuery->m_nTag, (mapXQMail *)pQuery->m_pAttachment);
    }
        break;
    case QUERY_AddMail:
    case QUERY_AddSysMail:
    {
        Query<MailInfo> *pTmp = dynamic_cast<Query<MailInfo> *>( pQuery );
        if ( pTmp != NULL && pTmp->IsValid() )
        {
            nRes = m_dbiRole.AddMail( pTmp->Get(), (unsigned int)pTmp->m_nAttachment );
        }
    }
        break;
    case QUERY_GetAllMailItem:
    {
        nRes = m_dbiRole.GetAllMailItem(pQuery->m_nTag);
    }
        break;
    case QUERY_GetMailMoney:
    {
        nRes = m_dbiRole.GetMailMoney(pQuery->m_nTag);
    }
        break;
    case QUERY_ReadMail:
    {
        nRes = m_dbiRole.ReadMail(pQuery->m_nTag);
    }
        break;
    case QUERY_DeleteMail:
    {
        nRes = m_dbiRole.DeleteMail(pQuery->m_nTag, pQuery->m_nAttachment);
    }
        break;
    case QUERY_AddAchievement:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CAchievementInfo *pInfo = (CAchievementInfo *)pQuery->m_pAttachment;
        nRes = m_dbiRole.AddAchievement(nRoleID, pInfo);
        delete (CAchievementInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_AddTitle:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CTitleInfo *pInfo = (CTitleInfo *)pQuery->m_pAttachment;
        nRes = m_dbiRole.AddTitle(nRoleID, pInfo);
        delete (CTitleInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateAchievementState:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned short * pAchievementID = (unsigned short *)(pQuery->m_pAttachment);
        unsigned char nState = (unsigned char)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateAchievementState(nRoleID, *pAchievementID, nState);
        delete (unsigned short *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_InsertCheckInInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CCheckInInfo *pCheckInfo = (CCheckInInfo  *)pQuery->m_pAttachment;
        nRes = m_dbiRole.InsertCheckIn(nRoleID, pCheckInfo);
        delete (CCheckInInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateCheckInInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;		// 角色ID
        CCheckInInfo *pCheckInfo = (CCheckInInfo  *)pQuery->m_pAttachment;	// checkin 更新用信息
        nRes = m_dbiRole.UpdateCheckIn(nRoleID, pCheckInfo);
        delete (CCheckInInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_AddCheckInGift:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned char nType = (unsigned char)pQuery->m_nTag;

        nRes = m_dbiRole.AddCheckInGift(nRoleID, nType);
    }
        break;
    case QUERY_RemoveCheckInGift:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned char nType = (unsigned char)pQuery->m_nTag;

        nRes = m_dbiRole.RemoveCheckInGift(nRoleID, nType);
    }
        break;
        //case QUERY_RemoveCheckInGiftByList:
        //	{
        //		unsigned int nRoleID = pQuery->m_nAttachment;
        //		list<unsigned char> *pListRemove = (list<unsigned char>*)pQuery->m_pAttachment;
        //		m_dbiRole.RemoveCheckInGiftByList(nRoleID, *pListRemove);
        //		delete pListRemove, pListRemove = NULL;
        //	}
        //	break;
    case QUERY_SaveVIPInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CRoleVIPInfo *pInfo = (CRoleVIPInfo *)pQuery->m_pAttachment;
        nRes = m_dbiRole.SaveVIPInfo(nRoleID, *pInfo);
        delete (CRoleVIPInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_SaveVIPConsumedPt:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nConsumedPt = (int)pQuery->m_nTag;
        nRes = m_dbiRole.SaveVIPConsumedPt(nRoleID, nConsumedPt);
    }
        break;
    case QUERY_Item_GetActivateGiftInfo:
    {
        CActivateGiftInfo *pGiftInfo = (CActivateGiftInfo *)pQuery->m_pAttachment;
        if (pGiftInfo != NULL) {
            nRes = m_dbiRole.GetActivateGiftInfo(pGiftInfo);
        }
    }
        break;
    case QUERY_Item_GetActivateGift:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CActivateGiftInfo *pGiftInfo = (CActivateGiftInfo *)pQuery->m_pAttachment;
        if (pGiftInfo != NULL) {
            nRes = m_dbiRole.ActivateGift(nRoleID, pGiftInfo);
        }
    }
        break;
    case QUERY_AddFirstPaymentInfo:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.AddFirstPaymentInfo(nAccount, nCount);
    }
        break;
    case QUERY_AddFirstPaymentInfoFeed:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.AddFirstPaymentInfoFeed(nAccount, nCount);
    }
        break;
    case QUERY_AddSendFirstPaymentReward:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.AddSendFirstPaymentReward(nAccount, nCount);
    }
        break;
    case QUERY_AddSendFirstPaymentRewardFeed:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.AddSendFirstPaymentRewardFeed(nAccount, nCount);
    }
        break;
    case QUERY_GetFirstPaymentReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nVersionType = (int)pQuery->m_nTag;
        int nCount = 0;
        int nRechargeTime = 0;
        nRes = m_dbiRole.LoadFirstPaymentInfo(nRoleID, nCount, nRechargeTime, nVersionType);

        pQuery->m_nTag = nRechargeTime;
        pQuery->m_nAttachment = nCount;
    }
        break;
    case QUERY_GetFirstPaymentRewardByAccountId:
    {
        RechargeRewardInfo* pRechargeRewardInfo = (RechargeRewardInfo*)pQuery->m_pAttachment;
        if (pRechargeRewardInfo != NULL)
        {
            if (pRechargeRewardInfo->pChongZhiInfo != NULL)
            {
                pRechargeRewardInfo->m_eVersionType = (Version_Type)pRechargeRewardInfo->pChongZhiInfo->nVersionType;
            }
            else if (pRechargeRewardInfo->pChongZhiByPUIDInfo != NULL)
            {
                pRechargeRewardInfo->m_eVersionType = (Version_Type)pRechargeRewardInfo->pChongZhiByPUIDInfo->czInfo.nVersionType;
                if (pRechargeRewardInfo->nAccountID == 0)
                {
                    m_dbiAccount.GetAccountID(pRechargeRewardInfo->pChongZhiByPUIDInfo->czInfo.nPID, pRechargeRewardInfo->pChongZhiByPUIDInfo->strPUID,
                        pRechargeRewardInfo->nAccountID);
                    pRechargeRewardInfo->pChongZhiByPUIDInfo->czInfo.nAccount = pRechargeRewardInfo->nAccountID;
                }
            }
            if (pRechargeRewardInfo->m_fixedChargeRecord.m_nMCoin != 0)
            {
                m_dbiRole.LoadFixedChargeReward(pRechargeRewardInfo->nAccountID,
                    pRechargeRewardInfo->m_fixedChargeRecord.m_nMCoin,
                    pRechargeRewardInfo); // 直接加在这个东西
            }
            nRes = m_dbiRole.LoadFirstPaymentRewardInfoByAccountId(*pRechargeRewardInfo);
        }
    }
        break;
    case QUERY_Bill_SaveChargeAction:
    {
        ChargeAction * pChargeAction = (ChargeAction*)pQuery->m_pAttachment;
        if (pChargeAction != NULL)
        {
            nRes = m_dbiRole.SaveChargeAction(*pChargeAction);
            delete (ChargeAction *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_AddCompleteTeachGuideEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.AddEventEntry(nRoleID, eEvent_CompleteTeachGuide, 1);
    }
        break;
    case QUERY_AddCompleteProcGuideEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateEventEntry(nRoleID, eEvent_CompleteProcGuide, nCount);
    }
        break;
    case QUERY_AddCompleteAnimation:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.AddEventEntry(nRoleID, eEvent_CompleteAnimation, 1);
    }
    case QUERY_UpdateCompleteProcGuideEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateEventEntry(nRoleID, eEvent_CompleteProcGuide, nCount);
    }
        break;
    case QUERY_AddOrUpdateEvent:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nTag;
        EventInfoDB* pEventInfoDB = (EventInfoDB*)pQuery->m_pAttachment;
        //如果需要，则按照账号id查找到角色id
        if (nAccount > 0)
        {
            nRes = m_dbiRole.GetRoleIDByAccountID(nAccount, pEventInfoDB->m_nRoleID);
        }
        if (pEventInfoDB->m_nRoleID > 0)
        {
            nRes = m_dbiRole.UpdateEventEntry4(pEventInfoDB->m_nRoleID, pEventInfoDB->m_nEventID, pEventInfoDB->m_nCount, pEventInfoDB->m_nTime);
        }
        delete pEventInfoDB;
        pEventInfoDB = NULL;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_UpdateFixedChargeReward:
    {
        FixedChargeRecordSingle *pRecord = (FixedChargeRecordSingle *)pQuery->m_pAttachment;
        if (NULL != pRecord)
        {
            nRes = m_dbiRole.UpdateFixedChargeReward(pRecord->m_nAccount, pRecord->m_nMCoin, pRecord->m_nChargeTime);

            delete pRecord;
            pRecord = NULL;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_LoadFixedChargeReward:
    {
        FixedChargeRecord *pRecord = (FixedChargeRecord*)pQuery->m_pAttachment;
        if (NULL != pRecord)
        {
            nRes = m_dbiRole.LoadFixedChargeRewardAll(pRecord->m_nAccount, *pRecord);
        }
    }
        break;
    case QUERY_ResetMoney:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nRestTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.ResetMoney(nRoleID, nRestTime);
    }
        break;
    case QUERY_UpdateResetMoneyTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nResetTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateMoneyResetTime(nRoleID, nResetTime);
    }
        break;
    case  QUERY_UpdateMoneyMailRemindTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nRemindTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateMoneyMailRemindTime(nRoleID, nRemindTime);
    }
        break;
    case  QUERY_UpdateMoneyMallRemindTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nRemindTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateMoneyMallRemindTime(nRoleID, nRemindTime);
    }
        break;
    case QUERY_UpdateOnlineRewardInfo:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        CKeepOnlineReward *pInfo = (CKeepOnlineReward *)pQuery->m_pAttachment;
        if (pInfo != NULL) {
            nRes = m_dbiRole.UpdateKeepOnlineRewardInfo(nRoleID, *pInfo);
        }

        delete (CKeepOnlineReward *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case GetRcvGMSysMailRoleInfo:
    {
        list<CRcvSysMailRoleInfo> *pListRoleInfo = (list<CRcvSysMailRoleInfo> *)(pQuery->m_pAttachment);
        if (pListRoleInfo != NULL) {
            nRes = m_dbiRole.GetRcvGMSysRoleInfo(*pListRoleInfo);
        }
    }
        break;
    case QUERY_QueryGiftByTimeRange:
    {
        CQueryGiftInfo *pInfo = (CQueryGiftInfo *)pQuery->m_pAttachment;

        if (pInfo != NULL) {
            nRes = m_dbiRole.QueryGiftByTimeRange(pInfo->m_nGiftID, pInfo->m_nActivatedNum, pInfo->m_strStartTime, pInfo->m_strEndTime);
        }
    }
        break;
    case QUERY_QueryMultiGift:
    {
        list<CQueryGiftState> *pListQueryCodes = (list<CQueryGiftState> *)pQuery->m_pAttachment;
        if (pListQueryCodes != NULL) {
            nRes = m_dbiRole.GetGiftCodesState(*pListQueryCodes);
        }
    }
        break;
    case QUERY_LogPlayerAction:
    case QUERY_LogRoleExp:
    case QUERY_LogRoleQuest:
    case QUERY_LogRoleAchievement:
    case QUERY_LogRoleDancing:
    case QUERY_LogRoleCheckIn:
    case QUERY_LogRoleSend:
    case QUERY_LogRoleItem:
    case QUERY_LogRoleChat:
    case QUERY_LogRoleAmuse:
    case QUERY_LogRoleTimeEgg:
    case QUERY_LogDanceGroup:
    case QUERY_LogRoleConstellationMatch:
    case QUERY_LogRoleVip:
    case QUERY_LogRoleMeetActivity:
    case QUERY_LogRoleAttrChange:
    case QUERY_LogRoleCouple:
    case QUERY_LogRoleDebris:
    case QUERY_LogRoleVisitRank:
    case QUERY_LogRoleAddFriends:
    case QUERY_LogRoleActiveness:
    case QUERY_LogRoleDungeon:
    case QUERY_LogRoleMall:
    case QUERY_LogRolePhotoRoom:
    case QUERY_LogRoleLuckDraw:
    case QUERY_LogRoleAudition:
    case QUERY_LogRoleLongacting:
    case QUERY_LogRoleCheckingMarkFailed:
    case QUERY_LogRoleClothEffect:
    case QUERY_LogRoleClothEffectProgress:
    case QUERY_LogRoleMedalLog:
    case QUERY_LogRoleRedEnvelopeLog:
    case QUERY_LogRoleMagicArray:
    case QUERY_LogRoleSwicthLine:
    case QUERY_LogRoleBigMamaDanceRecord:
    case QUERY_LogGroupBigMamaDanceRecord:
    case QUERY_LogPlayerType:
    {
        ISQLLog * pSQLLog = (ISQLLog *)pQuery->m_pAttachment;
        if (pSQLLog != NULL)
        {
            nRes = m_dbiSubLog.LogSQLLog(*pSQLLog);
        }
        delete (ISQLLog *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_LogRoleChangeBill:
    case QUERY_LogRoleChangeBindBill:
    case QUERY_LogRoleChangeMoney:
    {
        ISQLLog * pSQLLog = (ISQLLog *)pQuery->m_pAttachment;
        if (pSQLLog != NULL) {
            nRes = m_dbiLog.LogSQLLog(*pSQLLog);
        }
        delete (ISQLLog *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;

    case QUERY_Activity_LoadGift:
    {
        int nAccountID = pQuery->m_nAttachment;
        std::map< int, GiftDB > * pGiftMap = (std::map< int, GiftDB > *)pQuery->m_pAttachment;
        if (pGiftMap)
        {
            nRes = m_dbiAccount.LoadGift(nAccountID, *pGiftMap);
        }
    }
        break;
    case QUERY_Activity_GetGiftReward:
    {
        int nIndexID = pQuery->m_nAttachment;
        nRes = m_dbiAccount.GetTheGiftReward(nIndexID);
    }
        break;
    case QUERY_LoadCumulativeRechargeInfo:
    {
        unsigned int nAccount = (unsigned int)pQuery->m_nTag;
        unsigned int nRoleID = pQuery->m_nAttachment;
        std::vector<int> *pvecReward = (std::vector<int> *)pQuery->m_pAttachment;
        CCumulativeRechargeDB *pRechargeInfo = (CCumulativeRechargeDB *)pQuery->m_pTag;

        if (pvecReward != NULL && pRechargeInfo != NULL) {
            nRes = m_dbiRole.LoadCumulativeRechargeReward(nRoleID, pRechargeInfo->m_nRechargeStartTime, pRechargeInfo->m_nRechargeStopTime, *pvecReward);
            if (nRes == 0) {
                nRes = m_dbiAccount.LoadCumulativeRechargeInfo(nAccount, *pRechargeInfo);
            }
        }
    }
        break;
    case QUERY_LoadCumulativeSpendInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        std::vector<unsigned int> * pVecCumulativeSpend = (std::vector<unsigned int> *)pQuery->m_pAttachment;
        CCumulativeSpendDB * pSpendInfo = (CCumulativeSpendDB*)pQuery->m_pTag;
        if (pVecCumulativeSpend != NULL  && pSpendInfo != NULL)
        {
            nRes = m_dbiRole.LoadCumulativeSpendReward(nRoleID, *pVecCumulativeSpend);
            if (nRes == 0)
            {
                nRes = m_dbiRole.LoadCumulativeSpendInfo(nRoleID, *pSpendInfo);
            }
        }
        break;
    }
    case QUERY_LoadCumulativeSpendInfoForMedal:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        std::vector<unsigned int> * pVecCumulativeSpend = (std::vector<unsigned int> *)pQuery->m_pAttachment;
        CCumulativeSpendDB * pSpendInfo = (CCumulativeSpendDB*)pQuery->m_pTag;
        if (pVecCumulativeSpend != NULL  && pSpendInfo != NULL)
        {
            nRes = m_dbiRole.LoadCumulativeSpendReward(nRoleID, *pVecCumulativeSpend);
            if (nRes == 0)
            {
                nRes = m_dbiRole.LoadCumulativeSpendInfo(nRoleID, *pSpendInfo);
            }
        }
    }
        break;
    case QUERY_Insert_CumulativeSpendBill:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int nValue = (int)pQuery->m_nTag;
        nRes = m_dbiRole.InsertCumulativeSpendBill(nRoleID, nValue);
    }
        break;
    case QUERY_Insert_CumulativeSpendBindBill:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int nValue = (int)pQuery->m_nTag;
        nRes = m_dbiRole.InsertCumulativeSpendBindBill(nRoleID, nValue);
    }
        break;
    case QUERY_SaveCumulativeRecharge:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nLastReceivedTime = (unsigned int)pQuery->m_nTag;
        std::vector<int> * pVecReward = (std::vector<int> *)pQuery->m_pAttachment;
        if (pVecReward != NULL) {
            nRes = m_dbiRole.SaveCumulativeRecharge(nRoleID, nLastReceivedTime, *pVecReward);
        }
        delete (std::vector<int> *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Update_CumulativeReward:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nIndex = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.UpdateCumulativeSpendReward(nRoleID, nIndex);
    }
        break;
    case QUERY_Insert_CumulativeReward:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nIndex = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.InsertCumulativeSpendReward(nRoleID, nIndex);
    }
        break;
    case QUERY_LoadCumulativeSpendGotMedalReward:
    {
        std::vector<bool> *pGotReward = (std::vector<bool> *)pQuery->m_pAttachment;
        CumulativeSpendGotMedalDB *pInfo = (CumulativeSpendGotMedalDB *)pQuery->m_pTag;

        if (pGotReward != NULL && pInfo != NULL)
        {
            nRes = m_dbiRole.LoadCumulativeSpendGotMedalReward(*pInfo, *pGotReward);
        }

        delete pInfo, pQuery->m_pTag = pInfo = NULL;
    }
        break;
    case QUERY_InsertCumulativeSpendGotMedalReward:
    {
        unsigned char nIndex = (unsigned char)pQuery->m_nAttachment;
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.InsertCumulativeSpendGotMedalReward(nRoleID, nIndex);
    }
        break;
    case QUERY_LoadBuyItemGotMedalRewardedInfo:
    {
        BuyItemGotMedalRewardedInfoMap *pRewardedInfo = (BuyItemGotMedalRewardedInfoMap *)pQuery->m_pAttachment;
        BuyItemGotMedalDB *pInfo = (BuyItemGotMedalDB *)pQuery->m_pTag;

        if (pRewardedInfo != NULL && pInfo != NULL)
        {
            nRes = m_dbiRole.LoadBuyItemGotMedalRewardedInfo(*pInfo, *pRewardedInfo);
        }

        delete pInfo, pQuery->m_pTag = pInfo = NULL;
    }
        break;
    case QUERY_SetBuyItemGotMedalRewarded:
    {
        BuyItemGotMedalDB *pInfo = (BuyItemGotMedalDB *)pQuery->m_pAttachment;

        if (pInfo != NULL)
        {
            nRes = m_dbiRole.SetBuyItemGotMedalRewarded(*pInfo);
        }

        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_InsertBuyItemGotMedalInfo:
    {
        BuyItemGotMedalDB *pInfo = (BuyItemGotMedalDB *)pQuery->m_pAttachment;

        if (pInfo != NULL)
        {
            nRes = m_dbiRole.SaveBuyItemGotMedalInfo(true, *pInfo);
        }

        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_UpdateBuyItemGotMedalInfo:
    {
        BuyItemGotMedalDB *pInfo = (BuyItemGotMedalDB *)pQuery->m_pAttachment;

        if (pInfo != NULL)
        {
            nRes = m_dbiRole.SaveBuyItemGotMedalInfo(false, *pInfo);
        }

        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_LoadXmasFreeRewardInfo:
    {
        CXmasFreeRewardDB *pXmasFreeRewardDB = (CXmasFreeRewardDB *)pQuery->m_pAttachment;
        if (pXmasFreeRewardDB != NULL) {
            nRes = m_dbiRole.LoadXmaxFreeRewardInfo(*pXmasFreeRewardDB);
        }
    }
        break;
    case QUERY_SaveXmasFreeRewardInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nLastGetRewardTime = (unsigned int)pQuery->m_nTag;
        std::vector<int> * pVecReward = (std::vector<int> *)pQuery->m_pAttachment;
        if (pVecReward != NULL) {
            nRes = m_dbiRole.SaveXmasFreeRewardInfo(nRoleID, nLastGetRewardTime, *pVecReward);
        }
        delete (std::vector<int> *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_LoadFestivalWishInfo:
    {
        CFestivalWishInfoDB *pWishInfoDB = (CFestivalWishInfoDB *)pQuery->m_pAttachment;
        if (pWishInfoDB != NULL) {
            nRes = m_dbiRole.LoadFestivalWishInfo(*pWishInfoDB);
        }
    }
        break;
    case QUERY_SaveFestivalWishInfo:
    {
        CFestivalWishInfoDB *pWishInfoDB = (CFestivalWishInfoDB *)pQuery->m_pAttachment;
        if (pWishInfoDB != NULL) {
            nRes = m_dbiRole.SaveFestivalWishInfo(*pWishInfoDB);
        }
        delete (CFestivalWishInfoDB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_SaveFestivalBlessEntry:
    {
        CFestivalBlessInfoDB *pEntry = (CFestivalBlessInfoDB*)pQuery->m_pAttachment;
        if (pEntry != NULL) {
            nRes = m_dbiRole.AddBless(*pEntry);
        }
        delete (CFestivalBlessInfoDB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_SaveRoleFestivalBlessRewardInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        vector<int> *pVecReward = (vector<int> *)pQuery->m_pAttachment;
        unsigned int nLastGetRewardTime = (unsigned int)pQuery->m_nTag;
        if (pVecReward != NULL) {
            nRes = m_dbiRole.SaveRoleBlessActivityInfo(nRoleID, *pVecReward, nLastGetRewardTime);
        }
        delete (vector<int> *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_LoadRoleBlessActivityInfo:
    {
        CRoleBlessActivityDB *pBlessInfo = (CRoleBlessActivityDB *)pQuery->m_pAttachment;
        if (pBlessInfo != NULL) {
            nRes = m_dbiRole.LoadRoleBlessActivityInfo(pBlessInfo);
        }
    }
        break;
    case QUERY_AddProprietaryActivityReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.AddEventEntry(nRoleID, eEvent_ProprietaryActivityReward, 1);
    }
        break;
    case QUERY_DanceGroupLoadAllInfo:
    {
        if (pQuery->m_pAttachment != NULL)
        {
            CDanceGroupManager * pDGMgr = (CDanceGroupManager *)pQuery->m_pAttachment;
            nRes = m_dbiRole.LoadAllDanceGroup(*pDGMgr);
        }
    }
        break;
    case QUERY_RedEnvelope_LoadInfo:
    {
        std::map<unsigned int, RedEnvelope*> * pMapRedEnvelope = (std::map<unsigned int, RedEnvelope*>*)pQuery->m_pAttachment;
        if (pMapRedEnvelope != NULL)
        {
            nRes = m_dbiRole.LoadDanceGroupRedEnvelope(*pMapRedEnvelope, pQuery->m_nAttachment);
        }
    }
        break;
    case QUERY_RedEnvelope_LoadRoleInfo:
    {
        RedEnvelopeSenderInfo * pSenderInfo = (RedEnvelopeSenderInfo*)pQuery->m_pAttachment;
        if (pSenderInfo != NULL)
        {
            nRes = m_dbiRole.LoadRedEnvelopeSenderInfo(*pSenderInfo);
        }                                 
                                           
    }
        break;
    case QUERY_RedEnvelope_InsertNew:
    {
        RedEnvelope * pRedEnvelope = (RedEnvelope*)pQuery->m_pAttachment;
        if (pRedEnvelope != NULL)
        {
            nRes = m_dbiRole.InsertIntoRedEnvelope(*pRedEnvelope);
            delete pRedEnvelope; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_RedEnvelope_Update:
    {
        RedEnvelope * pRedEnvelope = (RedEnvelope*)pQuery->m_pAttachment;
        if (pRedEnvelope != NULL)
        {
            nRes = m_dbiRole.UpdateRedEnvelope(*pRedEnvelope);
            delete pRedEnvelope; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Load_FriendParcloseInfo:
    {
        if (pQuery->m_pAttachment != NULL)
        {
            std::vector<DungeonRankInfo> * pRankPlayers = (std::vector<DungeonRankInfo> *)pQuery->m_pAttachment;
            nRes = m_dbiRole.LoadFriendDungeonInfo((unsigned int)pQuery->m_nAttachment, pRankPlayers);
        }
    }
        break;
    case Query_Update_ParcloseInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MainlineParcloseRecord * pRecord = (MainlineParcloseRecord*)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.UpdateParcloseInfo(nRoleID, pRecord);
            delete (MainlineParcloseRecord*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Update_ThemeParcloseInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        ThemeParcloseRecord * pRecord = (ThemeParcloseRecord*)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.UpdateThemeParcloseInfo(nRoleID, pRecord);
            delete (ThemeParcloseRecord*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Update_FriendContactInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        FriendContactInfo * pFriendContactInfo = (FriendContactInfo *)pQuery->m_pAttachment;
        if (pFriendContactInfo != NULL)
        {
            nRes = m_dbiRole.UpdateFriendContactInfo(nRoleID, pFriendContactInfo);
        }
        delete (FriendContactInfo *)pQuery->m_pAttachment;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case Query_Update_Offline_FriendContactInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int * pContactInfo = (unsigned int *)pQuery->m_pAttachment;
        if (pContactInfo != NULL)
        {
            nRes = m_dbiRole.UpdateOfflineFriendContactInfo(nRoleID, *pContactInfo, *(pContactInfo + 1));
            delete[]pContactInfo; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Insert_FriendContactInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        FriendContactInfo * pFriendContactInfo = (FriendContactInfo *)pQuery->m_pAttachment;
        if (pFriendContactInfo != NULL)
        {
            nRes = m_dbiRole.InsertFriendContactInfo(nRoleID, pFriendContactInfo);
        }
        delete (FriendContactInfo *)pQuery->m_pAttachment;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case Query_Update_ClearGiveGetVitInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        nRes = m_dbiRole.BatchUpdateFriendContactInfo(nRoleID);
    }
        break;
    case Query_Update_ClearMailineQuickFinishNum:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        nRes = m_dbiRole.ClearMainlineQuickFinishNum(nRoleID);
    }
        break;
    case Query_Update_ClearThemeQuickFinishNum:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        nRes = m_dbiRole.ClearThemeQuickFinishNum(nRoleID);
    }
        break;
    case Query_Update_MagicLampInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MagicLamp * pInfo = (MagicLamp*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.UpateMagicLampInfo(nRoleID, pInfo);
            delete (MagicLamp*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Insert_MagicLampInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MagicLamp * pInfo = (MagicLamp*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.InsertMagicLampInfo(nRoleID, pInfo);
            delete (MagicLamp*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Insert_ParcloseInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MainlineParcloseRecord * pRecord = (MainlineParcloseRecord*)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.InsertParcloseInfo(nRoleID, pRecord);
            delete (MainlineParcloseRecord*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Insert_ThemeParcloseInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        ThemeParcloseRecord * pRecord = (ThemeParcloseRecord*)pQuery->m_pAttachment;
        if (pRecord != NULL)
        {
            nRes = m_dbiRole.InsertThemeParcloseInfo(nRoleID, pRecord);
            delete (ThemeParcloseRecord*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Update_MonthCardInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MonthCardDB * pInfo = (MonthCardDB*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.UpdateMonthCardInfo(nRoleID, pInfo);
            delete (MonthCardDB*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case Query_Insert_MonthCardInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        MonthCardDB * pInfo = (MonthCardDB*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.InsertMonthCardInfo(nRoleID, pInfo);
            delete (MonthCardDB*)pQuery->m_pAttachment;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case  Query_Insert_StarRewardIndex:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nIndex = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.InsertStarRewardIndex(nRoleID, nIndex);
    }
        break;
    case  Query_Update_DebrisInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int * pDebrisInfo = (unsigned int *)pQuery->m_pAttachment;
        if (pDebrisInfo != NULL)
        {
            nRes = m_dbiRole.UpdateDebrisInfo(nRoleID, *pDebrisInfo, *(pDebrisInfo + 1));
            delete[]pDebrisInfo; pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_DanceGroup_Create:
    {
        if (pQuery->m_pAttachment != NULL)
        {
            CDanceGroupInfo * pDGInfo = (CDanceGroupInfo *)pQuery->m_pAttachment;
            nRes = m_dbiRole.CreateDanceGroup(*pDGInfo, pQuery->m_nAttachment);
        }
    }
        break;
    case QUERY_DanceGroup_Remove:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        nRes = m_dbiRole.RemoveDanceGroup(nGroupID);
    }
        break;
    case QUERY_DanceGroup_UpdateDanceGroupBaseInfo:
    {
        CDanceGroupBaseInfo * pDGBaseInfo = (CDanceGroupBaseInfo *)pQuery->m_pAttachment;
        if (pDGBaseInfo != NULL)
        {
            nRes = m_dbiRole.UpdateDanceGroupInfo(*pDGBaseInfo);
        }
        delete (CDanceGroupBaseInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_AddDanceGroupMember:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        CDanceGroupMember * pMember = (CDanceGroupMember *)pQuery->m_pAttachment;
        if (pMember != NULL)
        {
            nRes = m_dbiRole.AddDanceGroupMember(nGroupID, pMember->m_nRoleID, pMember->m_nTitle, pMember->m_nProperty,
                pMember->m_nContribution, pMember->m_nInTime);
        }
        delete (CDanceGroupMember *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_UpdateMember:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        CDanceGroupMember * pMember = (CDanceGroupMember *)pQuery->m_pAttachment;
        if (pMember != NULL)
        {
            nRes = m_dbiRole.UpdateDanceGroupMember(nGroupID, pMember->m_nRoleID, pMember);
        }
        delete (CDanceGroupMember *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_UpdateMemberWithTime:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        CDanceGroupMember * pMember = (CDanceGroupMember *)pQuery->m_pAttachment;
        if (pMember != NULL)
        {
            nRes = m_dbiRole.UpdateDanceGroupMemberWithTime(nGroupID, pMember->m_nRoleID, pMember);
        }
        delete (CDanceGroupMember *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_DelDanceGroupMember:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.DelDanceGroupMember(nGroupID, nRoleID);
    }
        break;
    case QUERY_DanceGroup_UpdateRequestDanceGroup:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        CRequestDanceGroupInfo * pRequestDGInfo = (CRequestDanceGroupInfo *)pQuery->m_pAttachment;
        if (pRequestDGInfo != NULL)
        {
            nRes = m_dbiRole.UpdateRequestDanceGroup(nGroupID, pRequestDGInfo->m_nRoleID, pRequestDGInfo->m_nExpiredTime);
        }
        delete (CRequestDanceGroupInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_RemoveRequestDanceGroup:
    {
        unsigned int nGroupID = pQuery->m_nAttachment;
        std::list<unsigned int> *pListRoleID = (std::list<unsigned int> *)pQuery->m_pAttachment;

        nRes = m_dbiRole.RemoveRequestDanceGroup(nGroupID, pListRoleID);
        delete pListRoleID, pQuery->m_pAttachment = pListRoleID = NULL;
    }
        break;
    case QUERY_DanceGroup_ChangeDanceGroupName:
    {
        CChangeDanceGroupNameQuery *pChangeInfo = (CChangeDanceGroupNameQuery *)pQuery->m_pAttachment;
        if (pChangeInfo != NULL && pChangeInfo->m_nGroupID != 0 && !pChangeInfo->m_strNewName.empty()) {
            nRes = m_dbiRole.ChangeDanceGroupName(pChangeInfo->m_nGroupID, pChangeInfo->m_strNewName, pChangeInfo->m_nChangeTime, pQuery->m_nAttachment);
        }
    }
        break;
    case QUERY_DanceGroup_UpdateActivityInfo:
    {
        CDanceGroupActivityInfo *pUpdateDGBaseInfo = (CDanceGroupActivityInfo*)pQuery->m_pAttachment;
        if (pUpdateDGBaseInfo != NULL && pQuery->m_nAttachment != 0)
        {
            nRes = m_dbiRole.UpdateDanceGroupActivityInfo(pQuery->m_nAttachment, *pUpdateDGBaseInfo);
        }
        delete (CDanceGroupActivityInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_AddRecord:
    {
        CDanceGroupRecordsInfo *pRecrodInfo = (CDanceGroupRecordsInfo*)pQuery->m_pAttachment;
        if (pRecrodInfo != NULL && pQuery->m_nAttachment > 0)
        {
            nRes = m_dbiRole.AddDanceGroupRecord(pQuery->m_nAttachment, *pRecrodInfo);
        }
        delete (CDanceGroupRecordsInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_WriteQuitInfo:
    {
        DanceGroupQuitInfo *pQuitInfo = (DanceGroupQuitInfo *)pQuery->m_pAttachment;
        if (NULL != pQuitInfo)
            nRes = m_dbiRole.UpdateDanceGroupQuitInfo(pQuitInfo->m_nRoleID, pQuitInfo->m_nDanceGroupID, pQuitInfo->m_nQuitStamp);

        delete pQuitInfo;
        pQuitInfo = NULL;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroup_CeremonyLoadLog:
    {
        DanceGroupCeremonyLogMap *logMap = (DanceGroupCeremonyLogMap *)pQuery->m_pAttachment;
        if (NULL != logMap)
            nRes = m_dbiRole.CeremonyLoadLog(*logMap);
    }
        break;
    case QUERY_DanceGroup_CeremonyWriteLog:
    {
        DanceGroupCeremonyLog *groupLog = (DanceGroupCeremonyLog *)pQuery->m_pAttachment;
        unsigned int nGroupID = (unsigned int)pQuery->m_nAttachment;
        if (0 != nGroupID && NULL != groupLog)
            nRes = m_dbiRole.CeremonyWriteLog(*groupLog, nGroupID);
    }
        break;
    case QUERY_DanceGroup_LoadBigMamaGroupInfo:
    {
        DanceGroupBigMamaMap *pBigMamaMap = (DanceGroupBigMamaMap *)pQuery->m_pAttachment;
        if (NULL != pBigMamaMap)
        {
            nRes = m_dbiRole.LoadBigMamaGroupInfo(*pBigMamaMap);
        }
    }
        break;
    case QUERY_DanceGroup_UpdateOrInsertBigMamaGroupInfo:
    {
        DanceGroupBigMamaInfo *pBigMamaInfo = (DanceGroupBigMamaInfo *)pQuery->m_pAttachment;
        if (NULL != pBigMamaInfo)
        {
            nRes = m_dbiRole.UpdateOrInsertBigMamaGroupInfo(*pBigMamaInfo);
            delete pBigMamaInfo; pBigMamaInfo = NULL, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_DanceGroup_UpdateOrInsertBigMamaRoleInfo:
    {
        nRes = m_dbiRole.UpdateOrInsertBigMamaRoleInfo(pQuery->m_nRoleID, (unsigned int)pQuery->m_nAttachment, (unsigned int)pQuery->m_nTag);
    }
        break;
    case QUERY_External_SendMail:
    {
        ExternalSendMailInfo * pESMInfo = (ExternalSendMailInfo *)pQuery->m_pAttachment;
        if (pESMInfo)
        {
            nRes = m_dbiRole.ExternalSendMail(*pESMInfo);
        }
    }
        break;
    case QUERY_External_SendMailGift:
    {
        ExternalSendMailGiftInfo * pESMGInfo = (ExternalSendMailGiftInfo *)pQuery->m_pAttachment;
        if (pESMGInfo)
        {
            nRes = m_dbiRole.ExternalSendMailGift(*pESMGInfo);
        }
    }
        break;
    case QUERY_Player_SavePlayerSetting:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CPlayerSetting *pPlayerSetting = (CPlayerSetting *)pQuery->m_pAttachment;
        if (pPlayerSetting != NULL) {
            nRes = m_dbiRole.SavePlayerSetting(nRoleID, *pPlayerSetting, (int)pQuery->m_nTag);
        }
        delete (CPlayerSetting *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Couple_AddCouple:
    {
        CCoupleInfoDB *pCoupleInfo = (CCoupleInfoDB *)pQuery->m_pAttachment;
        if (pCoupleInfo != NULL) {
            nRes = m_dbiRole.AddCoupleInfo(*pCoupleInfo, pQuery->m_nAttachment);
        }
    }
        break;
    case QUERY_Couple_AddDivorceAgreement:
    {
        CDivorceAgreementInfo *pInfo = (CDivorceAgreementInfo *)pQuery->m_pAttachment;
        if (pInfo != NULL) {
            nRes = m_dbiRole.AddDivorceAgreementInfo(*pInfo, pQuery->m_nAttachment);
        }
    }
        break;
    case  QUERY_Couple_AddDivorceAction:
    {
        CDivorceAction * pAction = (CDivorceAction*)pQuery->m_pAttachment;
        if (pAction != NULL)
        {
            nRes = m_dbiRole.AddDivorceAction(*pAction);
        }
        delete pAction;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Couple_DelCouple:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        nRes = m_dbiRole.DelCoupleInfo(nPairID);
    }
        break;
    case QUERY_Couple_UpdateCoupleInfo:
    {
        CCoupleInfo *pCoupleInfo = (CCoupleInfo *)pQuery->m_pAttachment;
        if (pCoupleInfo != NULL) {
            nRes = m_dbiRole.UpdateCoupleMarriageDate(*pCoupleInfo);
        }
        delete (CCoupleInfo *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Couple_UpdateCoupleAgreementID:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nAgreementID = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateCoupleDivorceAgreementID(nRoleID, nAgreementID);
    }
        break;
    case QUERY_Couple_UpdateDivorceAgreementState:
    {
        unsigned int nAgreementID = (unsigned int)pQuery->m_nAttachment;
        int nState = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateDivorceAgreementState(nAgreementID, nState);
    }
        break;
    case QUERY_Couple_UpdateDivorceAgreement:
    {
        unsigned int nAgreementID = (unsigned int)pQuery->m_nAttachment;
        CDivorceAgreementInfo * pDivorceAgreementInfo = (CDivorceAgreementInfo*)pQuery->m_pAttachment;
        if (pDivorceAgreementInfo != NULL)
        {
            nRes = m_dbiRole.UpdateDivorceAgreement(nAgreementID, *pDivorceAgreementInfo);
        }
        delete (CDivorceAgreementInfo*)pQuery->m_pAttachment; pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Couple_UpdateIntimacy:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        int nValue = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateCoupleIntimacy(nPairID, nValue);
    }
        break;
    case QUERY_Couple_UpdateCoupleBlessingValue:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        int nValue = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateCoupleBlessingValue(nPairID, nValue);
    }
        break;
    case QUERY_Couple_AddBlessingRankInfo:
    {
        CBlessingValueRankInfo * pRankInfo = (CBlessingValueRankInfo *)pQuery->m_pAttachment;
        if (pRankInfo != NULL)
        {
            nRes = m_dbiRole.AddBlessingRankInfo(*pRankInfo);
        }

        //delete (BlessingRankInfoStruct *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Couple_DelBlessingRankInfo:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        nRes = m_dbiRole.DelBlessingRankInfo(nPairID);
    }
        break;
    case QUERY_Couple_UpdateBlessingRankValue:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        int nValue = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateBlessingRankValue(nPairID, nValue);
    }
        break;
    case QUERY_Couple_UpdateMarriageGiftDate:
    {
        unsigned int nPairID = pQuery->m_nAttachment;
        int nDate = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateMarriageGiftDate(nPairID, nDate);
    }
        break;
    case QUERY_Couple_LoadBlessingValueRank:
    {
        if (pQuery->m_pAttachment != NULL)
        {
            std::vector<CBlessingValueRankInfo> * prankmgr = (std::vector<CBlessingValueRankInfo> *)pQuery->m_pAttachment;
            nRes = m_dbiRole.LoadAllBlessingValueRank(prankmgr);
        }
    }
        break;
    case QUERY_Couple_LoadOfflineCoupleInfo:
    {
        if (pQuery->m_pAttachment != NULL)
        {
            std::map<unsigned int, COfflineCoupleInfo> * pOfflineCoupleInfo = (std::map<unsigned int, COfflineCoupleInfo> *)pQuery->m_pAttachment;
            nRes = m_dbiRole.LoadofflineCoupleInfo(pOfflineCoupleInfo);
        }
    }
        break;
    case QUERY_Insert_MagicArrayRecord:
        {
            unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
            MagicArrayRecord * pMagic = (MagicArrayRecord*)pQuery->m_pAttachment;
            if (pMagic != NULL)
            {
                nRes = m_dbiRole.InsertMagicArrayRecord(nRoleID, *pMagic);
                delete pMagic;pQuery->m_pAttachment = NULL;
            }
        }
        break;
    case QUERY_Insert_MagicArrayExchange:
        {
            unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
            MagicArrayExchangeRecord * pRecord = (MagicArrayExchangeRecord *)pQuery->m_pAttachment;
            if (pRecord != NULL)
            {
                nRes = m_dbiRole.InsertMagicArrayExchange(nRoleID, *pRecord);
                delete pRecord; pQuery->m_pAttachment = NULL;
            }
        }
        break;
    case QUERY_Update_MagicArrayExchange:
        {
            unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
            MagicArrayExchangeRecord * pRecord = (MagicArrayExchangeRecord *)pQuery->m_pAttachment;
            if (pRecord != NULL)
            {
                nRes = m_dbiRole.UpdateMagicArrayExchange(nRoleID, *pRecord);
                delete pRecord; pQuery->m_pAttachment = NULL;
            }
        }
        break;
    case QUERY_Update_MagicArrayRecord:
        {
            unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
            MagicArrayRecord * pMagic = (MagicArrayRecord*)pQuery->m_pAttachment;
            if (pMagic != NULL)
            {
                nRes = m_dbiRole.UpdateMagicArrayRecord(nRoleID, *pMagic);
                delete pMagic;pQuery->m_pAttachment = NULL;
            }
        }
        break;
    case QUERY_Puzzle_LoadPicture:
    {
        CRoleAllPuzzleDB* pRolePuzzleDB = (CRoleAllPuzzleDB *)pQuery->m_pAttachment;
        if (pRolePuzzleDB != NULL) {
            nRes = m_dbiRole.LoadRolePuzzle(*pRolePuzzleDB);
        }
    }
        break;

    case QUERY_Puzzle_InsertPicture:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        CRolePuzzleActivityDB* pPuzzleInfoDB = (CRolePuzzleActivityDB *)pQuery->m_pAttachment;
        if (pPuzzleInfoDB != NULL) {
            nRes = m_dbiRole.InsertRolePuzzle(nRoleID, *pPuzzleInfoDB);
        }
        delete (CRolePuzzleActivityDB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;

    case QUERY_Puzzle_UpdatePicture:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int nOriTime = (int)pQuery->m_nTag;
        CRolePuzzleActivityDB* pPuzzleInfoDB = (CRolePuzzleActivityDB *)pQuery->m_pAttachment;
        if (pPuzzleInfoDB != NULL) {
            nRes = m_dbiRole.UpdateRolePuzzle(nRoleID, nOriTime, *pPuzzleInfoDB);
        }
        delete (CRolePuzzleActivityDB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;

    case QUERY_Invitation_UpdateInvitationCode:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        char *pszInvitationCode = (char *)pQuery->m_pAttachment;
        if (pszInvitationCode != NULL)
        {
            nRes = m_dbiRole.UpdateRoleInvitationCode(nRoleID, pszInvitationCode);
        }
        delete (char *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Invitation_CheckInvitationCode:
    {
        char *pszInvitationCode = (char*)pQuery->m_pAttachment;
        if (pszInvitationCode != NULL)
        {
            int isValidInvitationCode = 0;
            nRes = m_dbiRole.CheckInvitationCode(pszInvitationCode, isValidInvitationCode);
            pQuery->m_nAttachment = isValidInvitationCode;
        }
    }
        break;
    case QUERY_Invitation_AddInvitation:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        char *pszInvitationCode = (char *)pQuery->m_pAttachment;
        if (pszInvitationCode)
        {
            nRes = m_dbiRole.AddInvitation(nRoleID, nTime, pszInvitationCode);
        }
        delete (char *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Invitation_GetInvitationInfo:
    {
        int nValidLevel = pQuery->m_nAttachment;
        unsigned int nStartTime = (unsigned int)pQuery->m_nTag;
        unsigned int nEndTime = *(unsigned int*)pQuery->m_pTag;
        CRoleAllInvitationInfoDB* pInvitationInfo = (CRoleAllInvitationInfoDB*)pQuery->m_pAttachment;
        if (pInvitationInfo != NULL)
        {
            nRes = m_dbiRole.GetInvitationInfo(nValidLevel, pInvitationInfo, nStartTime, nEndTime);
        }
    }
        break;
    case QUERY_Invitation_AddInviterReward:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int nLevel = (int)pQuery->m_nTag;
        nRes = m_dbiRole.AddInviterReward(nRoleID, nLevel);
    }
        break;
    case QUERY_Invitation_AddAcceptorReward:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        nRes = m_dbiRole.AddAcceptorReward(nRoleID);
    }
        break;
    case QUERY_VIP_UpdateOnlineAnnounceTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nAnnounceTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateVIPOnlineAnnounce(nRoleID, nAnnounceTime);
    }
        break;
    case QUERY_FresherActivity_AddReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nDay = (int)pQuery->m_nTag;
        nRes = m_dbiRole.AddFresherReward(nRoleID, nDay);
    }
        break;
    case QUERY_Rating_NoPushRating:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateNoPushRatingToday(nRoleID, nTime);
    }
        break;
    case QUERY_Rating_PushRatedLevelUp:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nLevel = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdatePushRatedLevelUp(nRoleID, nLevel);
    }
        break;
    case QUERY_Rating_PushRatedOnlineTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdatePushRatedOnlineTime(nRoleID, nTime);
    }
        break;
    case QUERY_Rating_PushRatedDailyQuest:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdatePushRatedDailyQuest(nRoleID, nTime);
    }
        break;
    case QUERY_Rating_PushRatedDailyShoping:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdatePushRatedDailyShoping(nRoleID, nTime);
    }
        break;
    case QUERY_Rating_PushRatedCount:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        RolePushRatingDB *pRating = (RolePushRatingDB *)pQuery->m_pAttachment;
        if (pRating != NULL)
        {
            nRes = m_dbiRole.UpdatePushRatedCount(nRoleID, pRating->nPushRatedCount, pRating->nPushRatedCountTime);
        }
        delete (RolePushRatingDB *)(pQuery->m_pAttachment), pQuery->m_pAttachment = NULL;
    }
        break;
    case Query_Security_SetSecurityQuiz:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        SecurityQuiz_DB *quiz = (SecurityQuiz_DB *)pQuery->m_pAttachment;
        if (quiz != NULL)
        {
            nRes = m_dbiRole.SetAccountSecurityQuiz(nRoleID, quiz);
        }
        delete (SecurityQuiz_DB *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
    }
        break;

    case Query_LiveRoom_GetDancerName:
    {
        CLiveRoomDancerDB *pDancer = (CLiveRoomDancerDB *)pQuery->m_pAttachment;
        if (pDancer != NULL)
        {
            nRes = m_dbiRole.GetDancerName(pDancer);
        }
    }
        break;
    case Query_LiveRoom_UpdateDancerName:
    {
        CLiveRoomDancerDB *pDancer = (CLiveRoomDancerDB *)pQuery->m_pAttachment;
        if (pDancer != NULL)
        {
            nRes = m_dbiRole.UpdateDancerName(pDancer);
        }
    }
        break;
    case Query_LoadCompensation:
    {
        CompensationInfoDB* pInfo = (CompensationInfoDB*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.LoadCompensationInfo(pInfo);
        }
    }
        break;
    case Query_InsertCompensation:
    {
        CompensationInfo* pInfo = (CompensationInfo*)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.InsertCompensationInfo(pInfo);
        }
    }
        break;
    case Query_InsertReceiveCompensation:
    {
        int nRoleID = (int)pQuery->m_nTag;
        int nCompensationID = pQuery->m_nAttachment;
        nRes = m_dbiRole.InsertRoleCompensation(nRoleID, nCompensationID);
    }
        break;
    case Query_LiveRoom_LoadPointAndHot:
    {
        unsigned int nStartTime = pQuery->m_nAttachment;
        unsigned int nEndTime = (unsigned int)pQuery->m_nTag;
        CAllLiveRoomScoreDB *pScore = (CAllLiveRoomScoreDB *)pQuery->m_pAttachment;
        if (pScore != NULL)
        {
            nRes = m_dbiRole.LoadLiveRoomPointAndHot(nStartTime, nEndTime, pScore);
        }
    }
        break;
    case Query_LiveRoom_GetPointAndHot:
    {
        unsigned int nStartTime = pQuery->m_nAttachment;
        unsigned int nEndTime = (unsigned int)pQuery->m_nTag;
        CLiveRoomScoreDB *pScore = (CLiveRoomScoreDB *)pQuery->m_pAttachment;
        if (pScore != NULL)
        {
            nRes = m_dbiRole.GetLiveRoomPointAndHot(nStartTime, nEndTime, pScore);
        }
    }
        break;
    case Query_LiveRoom_UpdateAuditionPoint:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nPoint = (int)pQuery->m_nTag;
        unsigned int nStartTime = (unsigned int)((unsigned long)pQuery->m_pAttachment);
        unsigned int nEndTime = (unsigned int)((unsigned long)pQuery->m_pTag);
        nRes = m_dbiRole.UpdateAuditionPoint(nRoleID, nPoint, nStartTime, nEndTime);
    }
        break;
    case Query_LiveRoom_UpdateDancerHot:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nHot = (int)pQuery->m_nTag;
        unsigned int nStartTime = (unsigned int)((unsigned long)pQuery->m_pAttachment);
        unsigned int nEndTime = (unsigned int)((unsigned long)pQuery->m_pTag);
        nRes = m_dbiRole.UpdateDancerHot(nRoleID, nHot, nStartTime, nEndTime);
    }
        break;
    case QUERY_FresherQuest_AddReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned char nEventID = (unsigned char)pQuery->m_nTag;
        unsigned short *pQuestID = (unsigned short *)pQuery->m_pAttachment;
        if (pQuestID != NULL)
        {
            nRes = m_dbiRole.AddFresherQuestReward(nRoleID, nEventID, *pQuestID);
            delete pQuestID, pQuestID = NULL;
        }
    }
        break;
    case QUERY_FresherQuest_BestMusicGrade:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned char nGrade = (unsigned char)pQuery->m_nTag;
        nRes = m_dbiRole.AddFresherQuestBestMusicGrade(nRoleID, nGrade);
    }
        break;
    case QUERY_Mall_AddMallBuyCount:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nBuyCount = (int)pQuery->m_nTag;
        nRes = m_dbiRole.AddMallBuyCount(nRoleID, nBuyCount);
    }
        break;
    case Query_CreateTimeEgg:
    {
        CreateTimeEggEntry *pEntry = (CreateTimeEggEntry *)pQuery->m_pAttachment;
        if (pEntry != NULL)
        {
            nRes = m_dbiRole.CreateTimeEgg(pEntry);
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }
        break;
    case Query_GetTimeEggsByRoleID:
    {
        QueryRoleTimeEggEntry *pInfo = (QueryRoleTimeEggEntry *)pQuery->m_pAttachment;
        if (pInfo != NULL)
        {
            nRes = m_dbiRole.GetTimeEggsByRoleID(pInfo);
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }
        break;
    case Query_OpenTimeEgg:
    {
        OpenTimeEggEntry *pEntry = (OpenTimeEggEntry *)pQuery->m_pAttachment;
        if (pEntry != NULL)
        {
            nRes = m_dbiRole.OpenTimeEgg(pEntry);
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }
        break;
    case Query_GetTimeEggs:
    {
        std::map<unsigned int, TimeEggEntry*>* pInfo = (std::map<unsigned int, TimeEggEntry*>*)pQuery->m_pAttachment;

        if (pInfo != NULL)
        {
            nRes = m_dbiRole.GetTimeEggs(pInfo, (unsigned int)pQuery->m_nAttachment);
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }
        break;

        //平台特权
    case Query_PlatformPrivilege_AddReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned short nRewardLevel = (unsigned short)pQuery->m_nTag;
        nRes = m_dbiRole.AddPlatformPrivilegeReward(nRoleID, nRewardLevel);
    }
        break;
    case QUERY_Add_Quest:
    {
        QUESTNEWINFO* pInfo = (QUESTNEWINFO*)pQuery->m_pAttachment;
        nRes = m_dbiRole.AddQuestNew(pInfo);
        delete pInfo, pInfo = NULL;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Update_Quest:
    {
        QUESTNEWINFO* pInfo = (QUESTNEWINFO*)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateQuestNew(pInfo);
        delete pInfo, pInfo = NULL;
        pQuery->m_pAttachment = NULL;
    }
        break;

    case Query_Constellation_EnterLobby:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        nRes = m_dbiRole.AddEnterConstellationLobbyEvent(nRoleID);
    }
        break;
    case Query_Constellation_UpdateCardInfo:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int *pIndex = (int *)pQuery->m_pAttachment;
        unsigned char nConstellation = (unsigned char)pQuery->m_nTag;
        unsigned int *pCoolDownEndTime = (unsigned int *)pQuery->m_pTag;
        if (pIndex != NULL && pCoolDownEndTime != NULL)
        {
            nRes = m_dbiRole.UpdateConstellationCardInfo(nRoleID, (unsigned short)*pIndex, nConstellation, *pCoolDownEndTime);
        }
        delete (int *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        delete (unsigned int *)pQuery->m_pTag, pQuery->m_pTag = NULL;
    }
        break;
    case Query_Constellation_MatchCount:
    {
        unsigned int nRoleID = pQuery->m_nAttachment;
        int nMatchCount = (int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateConstellationMatchCount(nRoleID, nMatchCount);
    }
        break;
    case Query_Constellation_UpdateMatchRoom:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int *pRoomID = (unsigned int *)pQuery->m_pAttachment;
        unsigned int nCreateTime = (unsigned int)pQuery->m_nTag;
        int *pLine = (int *)pQuery->m_pTag;
        if (pRoomID != NULL && pLine != NULL)
        {
            nRes = m_dbiRole.UpdateConstellationRoom(nRoleID, *pRoomID, nCreateTime, *pLine);
        }
        delete (unsigned int *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        delete (int *)pQuery->m_pTag, pQuery->m_pTag = NULL;
    }
        break;
    case QUERY_Add_DanceGroupReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nTime = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.AddDanceGroupReward(nRoleID, nTime);
    }
        break;
    case QUERY_Del_DanceGroupReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.DelDanceGroupReward(nRoleID);
    }
        break;
    case QUERY_ClearFirstPaymentInfo:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.ClearFirstPaymentInfo(nRoleID);
    }
        break;
    case QUERY_SaveSocialShare:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int* pUInt = (unsigned int*)&pQuery->m_nTag;
        unsigned int nTime = pUInt[0];
        int nUI = (int)pUInt[1];
        nRes = m_dbiRole.SaveSocialShare(nRoleID, nUI, nTime);
    }
        break;
    case QUERY_InsertProcGuideReward:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        nRes = m_dbiRole.AddEventEntry(nRoleID, eEvent_ReceivedProcGuideReward, 1);
    }
        break;
    case QUERY_UpdateMallSendItemEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCostBill = (unsigned int)pQuery->m_nTag;
        m_dbiRole.UpdateEventEntry4(nRoleID, eEvent_MallSendItemCostBill, nCostBill, (unsigned int)time(NULL));
    }
        break;
    case QUERY_SaveSpecialMall:
    {
        CSpecialMallInfoDB *pInfo = static_cast<CSpecialMallInfoDB *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.SaveSpecialMall(pInfo);
        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_SaveAuctionBid:
    {
        CAuctionBidInfoDB *pInfo = static_cast<CAuctionBidInfoDB *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.SaveAuctionBid(pInfo);
        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_SaveGroupBuyingCount:
    {
        CSaveGroupBuyingCount *pInfo = static_cast<CSaveGroupBuyingCount *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.SaveGroupBuyingCount(pInfo);
        delete pInfo, pQuery->m_pAttachment = pInfo = NULL;
    }
        break;
    case QUERY_SetAuctionStateSuccess:
    {
        CSetAuctionStateSuccessInfo *pInfo = static_cast<CSetAuctionStateSuccessInfo *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.ChangeAuctionState(-1, pInfo->m_uRoleID, pInfo->m_uIdx, (unsigned int)pInfo->m_tEndTime,
            EAuctionBidState_BidSuccess, pInfo->m_uBidPrice, pInfo->m_bSuccess);
    }
        break;
    case QUERY_AuctionReturnPoint:
    {
        CAuctionReturnPointDB *pInfo = static_cast<CAuctionReturnPointDB *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.ChangeAuctionState(pInfo->m_lLogID, pInfo->m_uRoleID, pInfo->m_uIdx, pInfo->m_nEndTime,
            EAuctionBidState_ReturnedMoney, pInfo->m_nPoint, pInfo->m_bSuccess);

        if (Query_Res_OK == nRes && pInfo->m_bSuccess)
        {
            nRes = m_dbiAccount.UpdateBillPt(pInfo->m_uAccountID, pInfo->m_nPoint);

            CRoleChangeBillLog changeBillLog;
            changeBillLog.m_nRoleID = pInfo->m_uRoleID;
            changeBillLog.m_nChangeValue = pInfo->m_nPoint;
            changeBillLog.m_eChangeBillCause = pInfo->m_eChangeBillCause;
            changeBillLog.m_nNewPT = pInfo->m_nNewPoint;

            nRes = m_dbiRole.LogSQLLog(changeBillLog);
        }
    }
        break;
    case QUERY_GetGroupBuyingCount:
    {
        std::list<CGetLimitedSalesCount> *pInfo = static_cast<std::list<CGetLimitedSalesCount> *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.GetGroupBuyingCount(pInfo);
    }
        break;
    case QUERY_GetLimitedEditionCount:
    {
        std::list<CGetLimitedSalesCount> *pInfo = static_cast<std::list<CGetLimitedSalesCount> *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.GetLimitedEditionCount(pInfo);
    }
        break;
    case QUERY_GetAuctionBidInfo:
    {
        std::map<unsigned short, CGetAuctionBidInfo> *pInfo = static_cast<std::map<unsigned short, CGetAuctionBidInfo> *>(pQuery->m_pAttachment);
        nRes = m_dbiRole.GetAuctionBidInfo(pInfo);
    }
        break;
    case QUERY_GMPrivRecharge:
    {
        std::vector<PlayerMoneyInfo>* pPlayerMoneyInfo = (std::vector<PlayerMoneyInfo>*)pQuery->m_pAttachment;
        std::vector<GMRechargeResult>* pPlayerResultList = (std::vector<GMRechargeResult>*)pQuery->m_pTag;
        nRes = m_dbiAccount.GMPrivSendMCoin(pPlayerMoneyInfo, pPlayerResultList);
    }
        break;
    case QUERY_Rank_GetRoleData:
    {
        RankGetRoleDataDB *pRoleDataDB = (RankGetRoleDataDB *)pQuery->m_pAttachment;

        nRes = m_dbiRole.GetRankRoleData(pRoleDataDB);
    }
        break;
    case QUERY_Rank_GetAllDatas:
    {
        RankGetAllDatasDB *pRoleDataDB = (RankGetAllDatasDB *)pQuery->m_pAttachment;

        nRes = m_dbiRole.GetRankAllDatas(pRoleDataDB);
    }
        break;
    case QUERY_Rank_SaveData:
    {
        RankSaveDataDB *pSaveDataDB = (RankSaveDataDB *)pQuery->m_pAttachment;

        nRes = m_dbiRole.SaveRankData( pSaveDataDB );

        delete pSaveDataDB, pQuery->m_pAttachment = pSaveDataDB = NULL;
    }
        break;
    case QUERY_Rank_UpdateDataState:
    {
        CRankUpdateDataStateDB *pUpdateDataStateDB = (CRankUpdateDataStateDB *)pQuery->m_pAttachment;

        nRes = m_dbiRole.UpdateRankDataState(pUpdateDataStateDB);

        delete pUpdateDataStateDB, pQuery->m_pAttachment = pUpdateDataStateDB = NULL;
    }
        break;
    case QUERY_Rank_SaveHistory:
    {
        RankSaveHistoryDB *pSaveHistoryDB = (RankSaveHistoryDB *)pQuery->m_pAttachment;
        if ( pSaveHistoryDB != NULL )
            nRes = m_dbiRole.SaveRankHistory( pSaveHistoryDB );

        delete pSaveHistoryDB, pQuery->m_pAttachment = pSaveHistoryDB = NULL;
    }
        break;
    case Query_System_LoadBlackDevice:
    {
        std::list<std::string> *pBlackList = (std::list<std::string> *)pQuery->m_pAttachment;
        if (pBlackList != NULL)
        {
            nRes = m_dbiAccount.LoadBlackDevice(*pBlackList);
        }
    }
        break;
    case QUERY_SendDeviceList:
    {
        std::vector<std::string>* pDeviceList = (std::vector<std::string>*)pQuery->m_pAttachment;
        std::string* pStrGM = (std::string*)pQuery->m_pTag;
        if (pDeviceList != NULL)
        {
            nRes = m_dbiRole.InsertBlackDevice(pDeviceList, pStrGM);
            delete (std::vector<std::string>*)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
            delete (std::string*)pQuery->m_pTag, pQuery->m_pTag = NULL;
        }
    }
        break;
    case QUERY_DeleteDeviceList:
    {
        std::vector<std::string>* pDeviceList = (std::vector<std::string>*)pQuery->m_pAttachment;
        if (pDeviceList != NULL)
        {
            nRes = m_dbiRole.DeleteBlackDevice(pDeviceList);
            delete (std::vector<std::string>*)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_ChangeAccountPassword:
    {
        std::string* pAccount = (std::string*)pQuery->m_pAttachment;
        std::string* pPassword = (std::string*)pQuery->m_pTag;
        if (pAccount != NULL && pPassword != NULL)
        {
            nRes = m_dbiRole.ChangeUserPassword(pAccount, pPassword);
        }
        if (pAccount != NULL)
        {
            delete (std::string*)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        }
        if (pPassword != NULL)
        {
            delete (std::string*)pQuery->m_pTag, pQuery->m_pTag = NULL;
        }
    }
        break;
    case QUERY_Recommend_UpdateCount:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.UpdateRecommendCount(nRoleID, nCount);
    }
        break;
    case QUERY_Recommend_UpdateResetTime:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.UpdateRecommendResetTime(nRoleID, nCount);
    }
        break;
    case QUERY_RedEnvelope_UpdateAnonymityEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nAnonymity = (unsigned int)pQuery->m_nTag;

        nRes = m_dbiRole.UpdateRedEnvelopeAnoymityEvent(nRoleID, nAnonymity);
    }
        break;
    case QUERY_Activeness_UpdateOrInsert:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        RoleActivenessDB *pActivness = (RoleActivenessDB *)pQuery->m_pAttachment;
        if (NULL != pActivness)
        {
            nRes = m_dbiRole.UpdateOrInsertActiveness(nRoleID, pActivness);
            delete pActivness;
            pActivness = NULL;
        }
    }
        break;
        //     case QUERY_Activeness_UpdateScore:
        //         {
        //             unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        //             int nScore = (int)pQuery->m_nTag;
        // 
        //             nRes = m_dbiRole.UpdateActivenessScore(nRoleID, nScore);
        //         }
        //         break;
        //     case QUERY_Activeness_UpdateTarget:
        //         {
        //             unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        //             std::string *pTargetProgress = (std::string *)pQuery->m_pAttachment;
        //             std::string *pDanceMode = (std::string *)pQuery->m_pTag;
        //             if (pTargetProgress != NULL && pDanceMode != NULL)
        //             {
        //                 nRes = m_dbiRole.UpdateActivenessTargetProgress(nRoleID, *pTargetProgress, *pDanceMode);
        //                 delete (std::string *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        //                 delete (std::string *)pQuery->m_pTag, pQuery->m_pTag = NULL;
        //             }
        //         }
        //         break;
        // 
        //     case QUERY_Activeness_Reward:
        //         {
        //             unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        //             std::string *pRewardLevelScore = (std::string *)pQuery->m_pAttachment;
        //             if (pRewardLevelScore != NULL)
        //             {
        //                 nRes = m_dbiRole.UpdateActivenessReward(nRoleID, *pRewardLevelScore);
        //                 delete (std::string *)pQuery->m_pAttachment, pQuery->m_pAttachment = NULL;
        //             }
        //         }
        //         break;

    case QUERY_PetEgg_LoadIncubator:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned short nLevel = 1;

        if (nRoleID != 0)
        {
            nRes = m_dbiRole.LoadPetIncubatorInfo(nRoleID, nLevel);

            pQuery->m_nAttachment = nLevel;
        }
    }
        break;
    case QUERY_PetEgg_UpdateIncubatorLevel:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;
        unsigned short nLevel = (unsigned short)pQuery->m_nAttachment;

        if (nRoleID != 0)
        {
            nRes = m_dbiRole.UpdatePetIncubatorLevel(nRoleID, nLevel);
        }
    }
        break;
    case QUERY_PetEgg_Load:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        PetEgg *pPetEgg = (PetEgg *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pPetEgg != NULL)
        {
            nRes = m_dbiRole.LoadPetEggInfo(nRoleID, *pPetEgg);
        }
    }
        break;
    case QUERY_PetEgg_Create:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        PetEgg *pPetEgg = (PetEgg *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pPetEgg != NULL)
        {
            nRes = m_dbiRole.CreatePetEggInfo(nRoleID, *pPetEgg);
        }
    }
        break;
    case QUERY_PetEgg_UpdateState:
    {
        unsigned int nEggID = (unsigned int)pQuery->m_nTag;
        unsigned char nState = (unsigned char)pQuery->m_nAttachment;

        if (nEggID != 0)
        {
            nRes = m_dbiRole.UpdatePetEggState(nEggID, nState);
        }
    }
        break;
    case QUERY_Pet_LoadExtendGridCount:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned short nCount = 0;

        if (nRoleID != 0)
        {
            nRes = m_dbiRole.LoadPetExtendGridCount(nRoleID, nCount);
            pQuery->m_nAttachment = nCount;
        }
    }
        break;
    case QUERY_Pet_UpdateExtendGridCount:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;
        unsigned short nCount = (unsigned short)pQuery->m_nAttachment;

        if (nRoleID != 0)
        {
            nRes = m_dbiRole.UpdatePetExtendGridCount(nRoleID, nCount);
        }
    }
        break;
    case QUERY_Pet_Create:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        Pet *pPet = (Pet *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pPet != NULL)
        {
            nRes = m_dbiRole.CreatePetInfo(nRoleID, *pPet);
        }
    }
        break;
    case QUERY_Pet_ChangeName:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nAttachment;
        std::string *pNewName = (std::string *)pQuery->m_pAttachment;

        if (nPetID != 0 && pNewName != NULL)
        {
            nRes = m_dbiRole.ChangePetName(nPetID, *pNewName);
        }

        delete pNewName, pQuery->m_pAttachment = pNewName = NULL;
    }
        break;
    case QUERY_Pet_UpdateState:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nTag;
        unsigned char nState = (unsigned char)pQuery->m_nAttachment;

        if (nPetID != 0)
        {
            nRes = m_dbiRole.UpdatePetState(nPetID, nState);
        }
    }
        break;
    case QUERY_Pet_UpdateQuality:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nTag;
        unsigned char nQualityType = (unsigned char)pQuery->m_nAttachment;

        if (nPetID != 0)
        {
            nRes = m_dbiRole.UpdatePetQuality(nPetID, nQualityType);
        }
    }
        break;
    case QUERY_Pet_UpdateEvolution:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nTag;
        unsigned char nEvolutionTimes = (unsigned char)pQuery->m_nAttachment;

        if (nPetID != 0)
        {
            nRes = m_dbiRole.UpdatePetEvolution(nPetID, nEvolutionTimes);
        }
    }
        break;
    case QUERY_Pet_UpdateEndurance:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nTag;
        unsigned short nEndurance = (unsigned short)pQuery->m_nAttachment;
        unsigned int *pRecoveryTime = (unsigned int *)pQuery->m_pAttachment;
        if (nPetID != 0 && pRecoveryTime != NULL)
        {
            nRes = m_dbiRole.UpdatePetEndurance(nPetID, nEndurance, *pRecoveryTime);
        }
        delete pRecoveryTime, pQuery->m_pAttachment = pRecoveryTime = NULL;
    }
        break;
    case QUERY_Pet_UpdateLevelAndExp:
    {
        unsigned int nPetID = (unsigned int)pQuery->m_nTag;
        unsigned short nLevel = (unsigned short)pQuery->m_nAttachment;
        unsigned int *pExp = (unsigned int *)pQuery->m_pAttachment;

        if (nPetID != 0 && pExp != NULL)
        {
            nRes = m_dbiRole.UpdatePetLevelAndExp(nPetID, nLevel, *pExp);
        }

        delete pExp, pQuery->m_pAttachment = pExp = NULL;
    }
        break;
    case QUERY_Pet_UpdateSkill:
    {
        Pet *pPet = (Pet *)pQuery->m_pAttachment;
        if (pPet != NULL)
        {
            nRes = m_dbiRole.UpdatePetSkill(*pPet);
        }
    }
        break;
    case QUERY_Pet_ResetSkill:
    {
        Pet *pPet = (Pet *)pQuery->m_pAttachment;
        if (pPet != NULL)
        {
            nRes = m_dbiRole.ResetPetSkill(*pPet);
        }
    }
        break;
    case QUERY_Pet_UpdateGuideStep:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nTag;
        unsigned short nCount = (unsigned short)pQuery->m_nAttachment;
        nRes = m_dbiRole.UpdateEventEntry4(nRoleID, eEvent_Pet_GuideEvent, nCount, (unsigned int)time(NULL));
    }
        break;
    case QUERY_InfoCenter_LoadAllInfo:
    {
        ENSURE_WITH_LOG_CMD(NULL != pQuery->m_pAttachment, break, "QUERY_InfoCenter_LoadAllInfo param err.");
        InfoCenterPtrMap *ptrMap = (InfoCenterPtrMap *)pQuery->m_pAttachment;

        nRes = m_dbiRole.InfoCenterLoadAllInfo(*ptrMap);
    }
        break;
    case QUERY_InfoCenter_AddInfo:
    {
        ENSURE_WITH_LOG_CMD(NULL != pQuery->m_pAttachment, break, "QUERY_InfoCenter_AddInfo param err.");
        InfoCenterItem *itemPtr = (InfoCenterItem *)pQuery->m_pAttachment;
        nRes = m_dbiRole.InfoCenterAddInfo(*itemPtr);
        pQuery->m_nTag = (nRes == Query_Res_OK ? 0 : 1);
    }
        break;
    case QUERY_InfoCenter_UpdateInfo:
    {
        ENSURE_WITH_LOG_CMD(NULL != pQuery->m_pAttachment, break, "QUERY_InfoCenter_UpdateInfo param err.");
        InfoCenterItem *itemPtr = (InfoCenterItem *)pQuery->m_pAttachment;
        nRes = m_dbiRole.InfoCenterUpdateInfo(*itemPtr);
    }
        break;
    case QUERY_InfoCenter_RemoveInfo:
    {
        nRes = m_dbiRole.InfoCenterRemoveInfo(pQuery->m_nAttachment);
    }
        break;
        // 	case QUERY_InfoCenter_LoadRoleReadInfo:
        // 		break;
    case QUERY_InfoCenter_UpdateRoleReadInfo:
    {
        std::pair<unsigned int, unsigned int> *p = (std::pair<unsigned int, unsigned int>*)pQuery->m_pAttachment;
        if (NULL != p)
        {
            nRes = m_dbiRole.InfoCenterUpdateRoleReadInfo(pQuery->m_nAttachment, p->first, p->second); // 更新这条记录
            delete p;
            p = NULL;
            pQuery->m_pAttachment = NULL;
        }
    }
        break;
    case QUERY_LongactingCumulativeRechagre_UpdateIndex:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nRewardIndex = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateLongActingRechargeIndex(nRoleID, nRewardIndex);
    }
        break;
    case QUERY_ExperienceCard_UpdateTime:
    {
        CExperiencedCardUseNum useNum;
        useNum.m_uRoleID = (unsigned int)pQuery->m_nRoleID;
        useNum.m_uUseNum = (unsigned short)pQuery->m_nAttachment;
        useNum.m_timeUpdate = (unsigned long)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateExperiencedTime(useNum);
    }
        break;

    case QUERY_ExperienceCard_AddExperiencedItem:
    {
        CExperiencedCardProduct usedProd((unsigned int)pQuery->m_nRoleID, (unsigned short)pQuery->m_nAttachment);

        nRes = m_dbiRole.InsertExperiencedProduct(usedProd);
    }
        break;
    case QUERY_UpdateGuideDataEvent:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nCount = (unsigned int)pQuery->m_nTag;
        nRes = m_dbiRole.UpdateEventEntry(nRoleID, eEvent_GuideData, nCount);
    }
        break;
    case QUERY_Medal_UpdateMedal:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        MedalUnitDB *pUnit = (MedalUnitDB *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pUnit != NULL)
        {
            nRes = m_dbiRole.UpdateMedal(nRoleID, *pUnit);
        }
        delete pUnit;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Medal_UpdateProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        MedalConditionDB *pCond = (MedalConditionDB *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pCond != NULL)
        {
            nRes = m_dbiRole.UpdateMedalCondition(nRoleID, *pCond);
        }

        delete pCond;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Medal_UpdateScore:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        MedalScoreDB *pMedalScore = (MedalScoreDB *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pMedalScore != NULL)
        {
            nRes = m_dbiRole.UpdateMedalScore(nRoleID, *pMedalScore);
        }

        delete pMedalScore;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Medal_DeleteProgress:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        MedalConditionDB *pCond = (MedalConditionDB *)pQuery->m_pAttachment;

        if (nRoleID != 0 && pCond != NULL)
        {
            nRes = m_dbiRole.DeleteMedalCondition(nRoleID, *pCond);
        }
        delete pCond;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Medal_DeleteVersionMedal:
    {
        std::list<int> *pidlist = (std::list<int> *)pQuery->m_pAttachment;
        if (pidlist != NULL)
        {
            nRes = m_dbiRole.DelVersionMedal(*pidlist);
        }

        delete pidlist;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Medal_AddMedalByGm:
    {
        int nRoleID = pQuery->m_nAttachment;
        std::list<int> *plistMedalID = (std::list<int> *)pQuery->m_pAttachment;
        int nMedalID = (int)pQuery->m_nTag;
        nRes = m_dbiRole.AddMedalByGm(nRoleID, nMedalID, plistMedalID);

        delete plistMedalID;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Handnbook_LoadGolbalCostumeData:
    {
        std::map<int, CHandbookUnit>* pMapCostume = (std::map<int, CHandbookUnit> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.LoadHandbookGlobalInfo(eHandbookType_Costume, *pMapCostume);
    }
        break;
    case QUERY_Handnbook_LoadGolbalEffectData:
    {
        std::map<int, CHandbookUnit>* pMapEffect = (std::map<int, CHandbookUnit> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.LoadHandbookGlobalInfo(eHandbookType_Effect, *pMapEffect);
    }
        break;
    case QUERY_Handnbook_SaveGlobalEffectData:
    {
        std::map<int, CHandbookUnit>* pMapEffect = (std::map<int, CHandbookUnit> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateHandbookGlobalInfo(eHandbookType_Effect, *pMapEffect);
        delete pMapEffect;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Handnbook_SaveGlobalCostumeData:
    {
        std::map<int, CHandbookUnit>* pMapCostume = (std::map<int, CHandbookUnit> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateHandbookGlobalInfo(eHandbookType_Costume, *pMapCostume);
        delete pMapCostume;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_Handnbook_UpdatePersonalData:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        std::map<int, std::string> *pDBMap = (std::map<int, std::string> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.UpdateHandbookPcInfo(nRoleID, *pDBMap);

//         for (std::map<int, std::string >::iterator mapit = pDBMap->begin();
//             mapit != pDBMap->end(); ++mapit)
//         {
//             for (std::list<CHandbookPersonalInfo *>::iterator listit = mapit->second.begin();
//                 listit != mapit->second.end(); ++listit)
//             {
//                 delete (*listit);
//             }
//         }
        pDBMap->clear();
        delete pDBMap;
        pDBMap = NULL;
    }
        break;
    case QUERY_Handnbook_InsertPersonalData:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        int nType = (int)pQuery->m_nTag;
        std::map<int, std::string> *pDBMap = (std::map<int, std::string> *)pQuery->m_pAttachment;
        nRes = m_dbiRole.InsertHandbookPcInfo(nRoleID, nType, *pDBMap);

//         for (std::map<int, std::list<CHandbookPersonalInfo *> >::iterator mapit = pDBMap->begin();
//             mapit != pDBMap->end(); ++mapit)
//         {
//             for (std::list<CHandbookPersonalInfo *>::iterator listit = mapit->second.begin();
//                 listit != mapit->second.end(); ++listit)
//             {
//                 delete (*listit);
//             }
//         }
        pDBMap->clear();
        delete pDBMap;
        pDBMap = NULL;
    }
        break;
    case QUERY_Desire_Insert:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        itemtype_t nItemType = (itemtype_t)pQuery->m_nTag;
        nRes = m_dbiRole.InsertDesiredItem(nRoleID, nItemType);
                                 
    }
        break;
    case QUERY_Desire_Delete:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        itemtype_t nItemType = (itemtype_t)pQuery->m_nTag;
        nRes = m_dbiRole.DeleteDesiredItem(nRoleID, nItemType);

    }
        break;
    case QUERY_AccountTran_CheckTranCode:
    {
        RebindAccountInfo *pRebindInfo = ((Query<RebindAccountInfo>*)pQuery)->Get();
        if (NULL != pRebindInfo)
        {
            m_dbiAccount.CheckRebindInfo(*pRebindInfo);
        }
    }
        break;
    case QUERY_AccountTran_UseTranCode:
    {
        RebindAccountInfo *pRebindInfo = ((Query<RebindAccountInfo>*)pQuery)->Get();
        if (NULL != pRebindInfo)
        {
            m_dbiAccount.UpdateAccountByTrancode(*pRebindInfo);
        }
    }
        break;

    case QUERY_DanceGroupChallenge_ResetChance:
    {
        Query<DanceGroupChallengeChanceEventDB> *pTmp = (Query<DanceGroupChallengeChanceEventDB> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.UpdateEventEntry4((*pTmp)->GetRoleID(),
                eEvent_DanceGroupChallengeChance, (*pTmp)->GetValue(),
                (*pTmp)->GetResetTime());
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateChance:
    {
        Query<DanceGroupChallengeChanceEventDB> *pTmp = (Query<DanceGroupChallengeChanceEventDB> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.UpdateEventEntry((*pTmp)->GetRoleID(),
                eEvent_DanceGroupChallengeChance, (*pTmp)->GetValue());
        }
    }
        break;
    case QUERY_DanceGroupChallenge_ResetBuyChance:
    {
        Query<DanceGroupChallengeChanceEventDB> *pTmp = (Query<DanceGroupChallengeChanceEventDB> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.UpdateEventEntry4((*pTmp)->GetRoleID(),
                eEvent_DanceGroupChallengeBuyChance, (*pTmp)->GetValue(),
                (*pTmp)->GetResetTime());
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateBuyChance:
    {
        Query<DanceGroupChallengeChanceEventDB> *pTmp = (Query<DanceGroupChallengeChanceEventDB> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.UpdateEventEntry((*pTmp)->GetRoleID(),
                eEvent_DanceGroupChallengeBuyChance, (*pTmp)->GetValue());
        }
    }
        break;
    case QUERY_DanceGroupChallenge_LoadRanking:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nStartTime = (unsigned int)pQuery->m_nTag;
        Query<std::map<int, DanceGroupChallengeRanking> > *pTmp = (Query<std::map<int, DanceGroupChallengeRanking> > *)pQuery;

        if (pTmp != NULL && pTmp->IsValid() && nRoleID != 0)
        {
            nRes = m_dbiRole.DanceGroupChallengeLoadRanking(nRoleID, nStartTime, **pTmp);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateRankingState:
    {
        unsigned int nRoleID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nStartTime = (unsigned int)pQuery->m_nTag;
        Query<std::map<int, DanceGroupChallengeRanking> > *pTmp = (Query<std::map<int, DanceGroupChallengeRanking> > *)pQuery;

        if (pTmp != NULL && pTmp->IsValid() && nRoleID != 0)
        {
            nRes = m_dbiRole.DanceGroupChallengeUpdateRankingState(nRoleID, nStartTime, **pTmp);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_RecordRanking:
    {
        Query<DanceGroupChallengeRecordRankingInfo> *pTmp = (Query<DanceGroupChallengeRecordRankingInfo> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.DanceGroupChallengeRecordRanking(**pTmp);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_LoadAllInfo:
    {
        unsigned int nStartTime = (unsigned int)pQuery->m_nAttachment;
        std::map<unsigned int, DanceGroupChallengeInfo *> *pInfo = (std::map<unsigned int, DanceGroupChallengeInfo *> *)pQuery->m_pAttachment;

        if (pInfo != NULL)
        {
            nRes = m_dbiRole.DanceGroupChallengeLoadAllInfo(nStartTime, *pInfo);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateActive:
    {
        unsigned int nGroupID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nActive = (unsigned int)pQuery->m_nTag;

        if (nGroupID != 0)
        {
            nRes = m_dbiRole.DanceGroupChallengeUpdateActive(nGroupID, nActive);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateContribution:
    {
        Query<DanceGroupChallengeUpdateRoleContribution> *pTmp = (Query<DanceGroupChallengeUpdateRoleContribution> *)pQuery;

        if (pTmp != NULL && pTmp->IsValid())
        {
            nRes = m_dbiRole.DanceGroupChallengeUpdateContribution(**pTmp);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_OpenStorey:
    {
        unsigned int nGroupID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nStartTime = (unsigned int)pQuery->m_nTag;
        const DanceGroupChallengeOpendStoreyInfo *pOpendInfo = (const DanceGroupChallengeOpendStoreyInfo *)pQuery->m_pAttachment;

        if (nGroupID != 0 && pOpendInfo != NULL)
        {
            // pOpendInfo不需要delete
            nRes = m_dbiRole.DanceGroupChallengeOpenStorey(nGroupID, nStartTime, *pOpendInfo);
        }
    }
        break;
    case QUERY_DanceGroupChallenge_UpdateStoreyScore:
    {
        unsigned int nGroupID = (unsigned int)pQuery->m_nAttachment;
        unsigned int nStartTime = (unsigned int)pQuery->m_nTag;
        const DanceGroupChallengeOpendStoreyInfo *pStoreyInfo = (const DanceGroupChallengeOpendStoreyInfo *)pQuery->m_pAttachment;

        if (nGroupID != 0 && pStoreyInfo != NULL)
        {
            // pStoreyInfo不需要delete
            nRes = m_dbiRole.DanceGroupChallengeUpdateStoreyScore(nGroupID, nStartTime, *pStoreyInfo);
        }
    }
        break;

    case QUERY_DanceGroupFairyLandLoadAllInfo:
    {
        Query<FairyLandLoadAllDB> *info = (Query<FairyLandLoadAllDB> *)pQuery;
        if (NULL != info && info->IsValid() && (*info)->m_pGInfo != NULL && (*info)->m_pGPInfo != NULL)
        {
            nRes = m_dbiRole.DanceGroupFairyLandLoadAllInfo(*(*info)->m_pGInfo, *(*info)->m_pGPInfo, (*info)->m_nNewDayStamp, (*info)->m_nPassRewardStamp, (*info)->m_nStorgeRewardStamp);
        }
    }
        break;
// 
//     case QUERY_DanceGroupFairyLandLoadLastStartTime:
//     {
//         int nLastBeginTime = 0;
//         nRes = m_dbiRole.DanceGroupFairyLandLoadLastBeginTime(nLastBeginTime);
//         pQuery->m_nAttachment = nLastBeginTime;
//     }
//         break;

    case QUERY_DanceGroupFairyLandUpdateLastStartTime:
    {
        int nLastBeginTime = (int)pQuery->m_nAttachment;
        m_dbiRole.DanceGroupFairyLandUpdateStampInfo(nLastBeginTime);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateSystemStamp:
    {
        int nTimeStamp = (int)pQuery->m_nAttachment;
        int nType = (int)pQuery->m_nTag;
        m_dbiRole.DanceGroupFairyLandUpdateStampInfo(nType, nTimeStamp);
    }
        break;

    case QUERY_DanceGroupFairyLandDelGroupInfo:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        nRes = m_dbiRole.DanceGroupFairyLandDelGroupInfo(nGroupId);
    }
        break;

    case QUERY_DanceGroupFairyLandAddGroupInfo:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        DanceGroupFairyLandGroupInfo *pGInfo = (DanceGroupFairyLandGroupInfo *)pQuery->m_pAttachment;
        nRes = m_dbiRole.DanceGroupFairyLandAddGroupInfo(nGroupId, *pGInfo);

        delete pGInfo;
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroupFairyLandDelPersonalInfo:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        int nRoleId = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandDelPersonalInfo(nGroupId, nRoleId);
    }
        break;
    case QUERY_DanceGroupFairyLandAddPersonalInfo:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        int nRoleId = (int)pQuery->m_nTag;

        nRes = m_dbiRole.DanceGroupFairyLandAddPersonalInfo(nGroupId, nRoleId);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateCurChatper:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        int nChapter = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandUpdateGroupChapter(nGroupId, nChapter);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateBlassRank:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        std::string *pRank = (std::string* )pQuery->m_pAttachment;
        if (NULL != pRank)
        {
            nRes = m_dbiRole.DanceGroupFairyLandUpdateBlassRank(nGroupId, *pRank);
        }
        SAFE_DELETE(pRank);
        pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateTodayRank:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        std::string *pRank = (std::string*)pQuery->m_pAttachment;
        nRes = m_dbiRole.DanceGroupFairyLandUpdateTodayRank(nGroupId, *pRank);
        SAFE_DELETE(pRank);
        pQuery->m_pAttachment = NULL;

    }
        break;
    case QUERY_DacneGroupFairyLandUpdateBlessInfo:
    {
        Query<FairyLandLastBlessInfoDB> *blessInfo = (Query<FairyLandLastBlessInfoDB> *)pQuery;
        if (NULL != blessInfo && blessInfo->IsValid())
            nRes = m_dbiRole.DanceGroupFairyLandUpdateBlassEndTime((*blessInfo)->m_nGroupID, (*blessInfo)->m_nBlessEndTimeStamp, (*blessInfo)->m_nLastBlessRoleID, (*blessInfo)->m_strLastBlessRoleName);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateTotalScore:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        int nTotalScore = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandUpdatePersonalTotalScore(nRoleId, nTotalScore);
    }
        break;

    case QUERY_DanceGroupFairyLandClearYesterdayScore:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        nRes = m_dbiRole.DanceGroupFairyLandClearYesterdayScore(nRoleId);
    }
        break;
//     case QUERY_DanceGroupFairyLandUpdateLastChallengeChapterLvl:
//     {
//         int nRoleId = (int)pQuery->m_nAttachment;
//         int nChapterLv = (int)pQuery->m_nTag;
//         nRes = m_dbiRole.DanceGroupFairyLandUpdateLastChallengeLvl(nRoleId, nChapterLv);
//     }
//         break;
    case QUERY_DanceGroupFairyLandUpdateBlassTimes:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        int nTotalTimes = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandUpdatePersonaBlassTimes(nRoleId, nTotalTimes);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateCurChapterChallengeTimes:
    {
        Query<std::pair<int, int> > *qu = (Query<std::pair<int, int> > *)pQuery;
        if (NULL != qu && qu->IsValid())
            nRes = m_dbiRole.DanceGroupFairyLandUpdateChallengeTimes(qu->m_nAttachment, (*qu)->first, (*qu)->second);
    }
        break;
    case QUERY_DanceGroupFairyLandUpdateStorageBox:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        std::string *pStrBox = (std::string*)pQuery->m_pAttachment;
        if (NULL != pStrBox)
            nRes = m_dbiRole.DanceGroupFairyLandUpdateStotageBox(nRoleId, *pStrBox);
        SAFE_DELETE(pStrBox); pQuery->m_pAttachment = NULL;
    }
        break;

    case QUERY_DanceGroupFairyLandUpdateJoinRewardFlag:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        std::string *pJoinRewardFlag = (std::string *)pQuery->m_pAttachment;
        if (NULL != pJoinRewardFlag)
            nRes = m_dbiRole.DanceGroupFairyLandUpdateJoinRewardFlag(nRoleId, *pJoinRewardFlag);
        SAFE_DELETE(pJoinRewardFlag); pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroupFairyLandUpdatePassedRewardFlag:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        std::string *pPassedRewardFlag = (std::string *)pQuery->m_pAttachment;
        if (NULL != pPassedRewardFlag)
            nRes = m_dbiRole.DanceGroupFairyLandUpdatePassedRewardFlag(nRoleId, *pPassedRewardFlag);
        SAFE_DELETE(pPassedRewardFlag); pQuery->m_pAttachment = NULL;
    }
        break;
    case QUERY_DanceGroupFairyLandUpdatePassedRewardRecord:
    {
        int nRoleId = (int)pQuery->m_nAttachment;
        Query<std::string> *pStrJson = (Query<std::string> *)pQuery;
        if (NULL != pStrJson && pStrJson->IsValid())
        {
            nRes = m_dbiRole.DanceGroupFairyLandUpdatePassedRewardRecord(nRoleId, pStrJson->GetRef());
        }
    }
        break;
        
    case QUERY_DanceGroupFairyLandUpdateGroupScore:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        int nScore = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandUpdateGroupScore(nGroupId, nScore);
    }
        break;
    case QUERY_DacneGroupFairyLandUpdateBlessEndTime:
    {
        int nGroupId = (int)pQuery->m_nAttachment;
        int nEndTime = (int)pQuery->m_nTag;
        nRes = m_dbiRole.DanceGroupFairyLandUpdateBlessEndTime(nGroupId, nEndTime);
    }
        break;
    case QUERY_DanceGroupFairyLandClearAllPassedReward:
    {
        nRes = m_dbiRole.DanceGroupFairyLandClearAllPassedReward();
        if (nRes == Query_Res_OK)
            pQuery->m_nTag = 1;
    }
        break;
    case QUERY_DanceGroupFairyLandClearAllStorageBoxReward:
    {
        nRes = m_dbiRole.DanceGroupFairyLandClearAllStorageBoxReward();
        if (nRes == Query_Res_OK)
            pQuery->m_nTag = 1;
    }
        break;
    case QUERY_DanceGroupFairyNewDay:
    {
        nRes = m_dbiRole.DanceGroupFairyLandNewDay(pQuery->m_nAttachment);
    }
        break;

    default:
        break;
    }


    unsigned int nTickEnd = GetTickCount();

    // 查询处理耗时、排队耗时、总耗时
    unsigned int nProcessTime = nTickEnd - nTickBegin;
    unsigned int nQueueTime = nTickBegin - pQuery->m_nEnqueueTime;
    unsigned int nQueryTotalTime = nTickEnd - pQuery->m_nEnqueueTime;
    if (nProcessTime >= (unsigned int)g_DbWarningProcessTime || nQueueTime >= (unsigned int)g_DbWarningWaitTime)
    {
        WriteQueryExceptionLog(LOGLEVEL_WARNING, "query=%d roleid=%d processTime=%u queueTime=%u totalTime=%u\n",
            pQuery->m_nQueryType, pQuery->m_nRoleID, nProcessTime, nQueueTime, nQueryTotalTime);
    }

    WriteLogDB(LOGLEVEL_DEBUG, "query=%d roleid=%d processTime=%u queueTime=%u totalTime=%u\n",
        pQuery->m_nQueryType, pQuery->m_nRoleID, nProcessTime, nQueueTime, nQueryTotalTime);

    //10分钟记录一下查询队列的状况
    time_t nNow = time(NULL);
    if (nNow - m_timeRecord >= 600)
    {
        m_timeRecord = nNow;
        int nQueueSize = GetQueueSize();
        if (nQueueSize > 0)
        {
            WriteLogDB(LOGLEVEL_DEBUG, "QueryMgr[line=%d] QueueSize=%d\n", g_nLine, nQueueSize);
            if (nQueueSize >= g_DbWarningQueueSize)
            {
                WriteQueryExceptionLog(LOGLEVEL_WARNING, "QueryMgr[line=%d] QueueSize=%d\n", g_nLine, nQueueSize);
            }
        }
    }

    return nRes;
}

int CQueryManager::GetQueueSize()
{
    int nQueueSize = 0;
    Lock(&m_lockQuery);
    nQueueSize = (int)m_apQueryQueue.size();
    Unlock(&m_lockQuery);
    return nQueueSize;
}

bool CQueryManager::IsQueueEmpty()
{
    bool bEmpty = true;
    Lock(&m_lockQuery);
    bEmpty = m_apQueryQueue.empty();
    Unlock(&m_lockQuery);
    return bEmpty;
}


void CQueryManager::AddQuery(int nQueryType, int nAttachment, void* pAttachment, __int64 nTag, void* pTag, CDelHelper* pDelHelper)
{
    if (nQueryType >= QUERY_TYPE_LOG_MIN && nQueryType <= QUERY_TYPE_LOG_MAX)
    {
        QueryBase *pQuery = new QueryBase(nQueryType, nAttachment, pAttachment, nTag, pTag, pDelHelper);
        Lock(&m_lockLogQuery);
        pQuery->m_nEnqueueTime = GetTickCount();
        m_apLogQueryQueue.push_back(pQuery);
        Unlock(&m_lockLogQuery);

        SignalEvent(LogQuery);
    }
    else
    {
        CQueryManagerBase::AddQuery(nQueryType, nAttachment, pAttachment, nTag, pTag, pDelHelper);
    }
}

void CQueryManager::AddQuery(QueryBase *query)
{
    if (query->m_nQueryType >= QUERY_TYPE_LOG_MIN && query->m_nQueryType <= QUERY_TYPE_LOG_MAX)
    {
        Lock(&m_lockLogQuery);
        query->m_nEnqueueTime = GetTickCount();
        m_apLogQueryQueue.push_back(query);
        Unlock(&m_lockLogQuery);

        SignalEvent(LogQuery);
    }
    else
    {
        CQueryManagerBase::AddQuery(query);
    }
}

QueryBase* CQueryManager::GetLogQuery()
{
    QueryBase *pQuery = NULL;

    Lock(&m_lockLogQuery);
    if (m_apLogQueryQueue.size() > 0)
    {
        pQuery = m_apLogQueryQueue.front();
        m_apLogQueryQueue.pop_front();
    }

    Unlock(&m_lockLogQuery);
    return pQuery;
}

#ifdef WIN32
DWORD WINAPI CQueryManager::LogQueryThreadProc(void* pParam)
#else
#include <sys/time.h>
void* CQueryManager::LogQueryThreadProc(void* pParam)
#endif
{
    QueryBase* pQuery;
    int nRes;
    CQueryManager *pQM = (CQueryManager *)pParam;
#ifdef WIN32
#else
    timeval now;
    struct timezone tz;
    timespec timeout;
    pthread_mutex_t lockNothing;
    pthread_mutex_init(&lockNothing, NULL);
#endif

    while (pQM->m_bRunning)
    {
        if ((pQuery = pQM->GetLogQuery()) != NULL)
        {
            nRes = pQM->ProcessQuery(pQuery);
            if (pQuery)pQM->FinishQuery(pQuery, nRes);
        }
        else
        {
#ifdef WIN32
            WaitForSingleObject(pQM->m_hEventLogQuery, INFINITE);
#else
            gettimeofday(&now, &tz);
            timeout.tv_sec = now.tv_sec;
            timeout.tv_nsec = (now.tv_usec + 100000) << 10;

            pthread_mutex_lock(&lockNothing);
            pthread_cond_timedwait(&pQM->m_condLogQuery, &lockNothing, &timeout);
            pthread_mutex_unlock(&lockNothing);
#endif
        }
    }
#ifdef WIN32
#else
    pthread_mutex_destroy(&lockNothing);
#endif
    TRACE("LogQueryThreadProc return \n");
    return 0;
}

void CQueryManager::Release()
{
    CQueryManagerBase::Release();

    SignalEvent(LogQuery);
    WaitThread(m_hThreadLogQuery);
    m_hThreadLogQuery = 0;
}

