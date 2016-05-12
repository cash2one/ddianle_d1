#include <stdio.h>
#include <stdlib.h>
#include "../../socket/Log.h"

#include "../../socket/Formatter.h"
#include "../util/Split.h"


#include "GMDBInterface.h"

#ifdef WIN32
#pragma warning(disable :4996)
#endif // WIN32

GMDBInterface::GMDBInterface()
{
}

GMDBInterface::~GMDBInterface()
{
}

int GMDBInterface::GetGMAccInfo(GMAccountInfo &accountInfo)
{
    if (accountInfo.m_szName == NULL || accountInfo.m_szPasswd == NULL)
        return Query_Res_Fail;

    int nRes = Query_Res_Fail;
    char *end = sql;
    sprintf(end, "select account, passwd from account where name = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, accountInfo.m_szName, strlen(accountInfo.m_szName));
    strcat(end, "'");
    end += strlen(end);
    if (ExecSQL(sql, end - sql))
    {
        nRes = Query_Res_OK;
        row = FetchRow();
        if (row != NULL)
        {
            int nField = 0;
            accountInfo.m_nAccount = strtoul(row[nField++], NULL, 10);
            memcpy(accountInfo.m_szPasswd, row[nField++], GM_MAX_PASSWD_LEN);
        }
    }
    SafeFreeResult();

    return nRes;
}

int GMDBInterface::GetGMRoleInfo(CGMRoleInfoDB & roleInfo)
{
    int nRes = Query_Res_Fail;
    if (roleInfo.m_szName == NULL) {
        return Query_Res_Fail;
    }

    sprintf(sql, "select unix_timestamp(lastlogintime),priv from account where account = %u", roleInfo.m_nAccount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        if ((row = FetchRow()) != NULL)
        {
            if (row != NULL)
            {
                roleInfo.m_nLastLoginTime = strtoul(row[0], NULL, 10);
                roleInfo.m_nPriv = atoi(row[1]);
            }
        }
    }
    SafeFreeResult();

    if (nRes == Query_Res_OK)
    {
        LoadGrantedCmd(roleInfo.m_nAccount, roleInfo.m_listGrantedCmds);
        LoadRolePriv(roleInfo.m_nAccount, roleInfo.m_listPrivs);
        LoadRoleDisableCmds(roleInfo.m_nAccount, roleInfo.m_listDisabledCmds);
    }

    return nRes;
}

int GMDBInterface::InsertGrantedCmd(const char *pAccName, list<string> & listCmd, int & nErrno)
{
    int nRes = Query_Res_Fail;
    bool bRes = false;
    unsigned int nAccount = 0;

    char *end = sql;
    sprintf(end, "select account from account where name = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pAccName, strlen(pAccName));
    strcat(end, "'");
    end += strlen(end);
    bRes = ExecSQL();
    if (bRes)
    {
        if ((row = FetchRow()) != NULL)
        {
            nAccount = strtoul(row[0], NULL, 10);
            nErrno = 0;
        }
    }
    SafeFreeResult();

    if (bRes)
    {
        for (list<string>::iterator itr = listCmd.begin(); itr != listCmd.end(); ++itr)
        {
            end = sql;
            sprintf(sql, "insert into grantedcmd set account = %u, cmd = '", nAccount);
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, itr->c_str(), itr->size());
            strcat(end, "'");
            end += strlen(end);
            if (ExecSQL()) {
            }
        }
        nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}

int GMDBInterface::RemoveGrantedCmd(const char *pAccName, list<string> & listCmd, int & nErrno)
{
    int nRes = Query_Res_Fail;
    bool bRes = false;
    unsigned int nAccount = 0;

    char *end = sql;
    sprintf(end, "select account from account where name = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pAccName, strlen(pAccName));
    strcat(end, "'");
    end += strlen(end);
    bRes = ExecSQL();
    if (bRes)
    {
        if ((row = FetchRow()) != NULL)
        {
            nAccount = strtoul(row[0], NULL, 10);
            nErrno = 0;
        }
    }
    SafeFreeResult();

    if (bRes)
    {
        for (list<string>::iterator itr = listCmd.begin(); itr != listCmd.end(); ++itr)
        {
            end = sql;
            sprintf(end, "delete from grantedcmd where account = %u and cmd = '", nAccount);
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, itr->c_str(), itr->size());
            strcat(end, "'");
            end += strlen(end);
            if (ExecSQL()) {
                nRes = Query_Res_OK;
            }
        }
    }
    SafeFreeResult();

    return nRes;
}

int GMDBInterface::LoadGrantedCmd(unsigned int nAccount, list<string> & listCmd)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select cmd from grantedcmd where account = %u", nAccount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            string strCmd = row[nField++];
            listCmd.push_back(strCmd);
        }
    }
    SafeFreeResult();

    return nRes;
}

int GMDBInterface::InsertRolePriv(unsigned int nAccount, list<unsigned int> & listPriv)
{
    for (list<unsigned int>::iterator itr = listPriv.begin(); itr != listPriv.end(); ++itr)
    {
        sprintf(sql, "insert into privs set account = %u, priv = %u", nAccount, *itr);
        if (!ExecSQL())
        {
            WriteLog(LOGLEVEL_ERROR, "InsertRolePriv failed:account = %d, priv = %u", nAccount, *itr);
        }
    }
    SafeFreeResult();

    return Query_Res_OK;
}

int GMDBInterface::LoadRolePriv(unsigned int nAccount, list<unsigned int> & listPriv)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select priv from privs where account = %u", nAccount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nPriv = strtoul(row[nField++], NULL, 10);
            listPriv.push_back(nPriv);
        }
    }
    SafeFreeResult();

    return nRes;
}

int GMDBInterface::InsertRoleDisabledCmds(unsigned int nAccount, list<string> & listCmd)
{
    for (list<string>::iterator itr = listCmd.begin(); itr != listCmd.end(); ++itr)
    {
        char *end = sql;
        sprintf(sql, "insert into disabledcmd set account = %u, cmd = '", nAccount);
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, itr->c_str(), itr->size());
        strcat(end, "'");
        end += strlen(end);
        if (!ExecSQL())
        {
            WriteLog(LOGLEVEL_ERROR, "InsertRoleDisabledCmds failed:account = %u, cmd = %s", nAccount, itr->c_str());
        }
    }
    SafeFreeResult();

    return Query_Res_OK;
}

int GMDBInterface::LoadRoleDisableCmds(unsigned int nAccount, list<string> & listCmd)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select cmd from disabledcmd where account = %u", nAccount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            string strCmd = row[nField++];
            listCmd.push_back(strCmd);
        }
    }
    SafeFreeResult();

    return nRes;
}

int GMDBInterface::InsertSysMailLog(CGMSysMailLog & sysMailLog)
{
    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(sql, "insert into sysmaillog set gmaccount = %u, puid = '%s', itemtype = %u, itemcnt = %u, validtime = %u, money = %u, sendtime = from_unixtime(%u), mailtitle = '",
        sysMailLog.m_nGMAccount, sysMailLog.m_strPUID.c_str(), sysMailLog.m_nItemType, sysMailLog.m_nItemCount,
        sysMailLog.m_nValidTime, sysMailLog.m_nMoney, sysMailLog.m_nSendTime);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, sysMailLog.m_strMailTitle.c_str(), sysMailLog.m_strMailTitle.size());
    strcat(end, "', mailcontent = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, sysMailLog.m_strMailContent.c_str(), sysMailLog.m_strMailContent.size());
    strcat(end, "', rolename = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, sysMailLog.m_strRoleName.c_str(), sysMailLog.m_strRoleName.size());
    strcat(end, "'");
    end += strlen(end);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int GMDBInterface::AddAnnouce(CAnnounceInfo & record)
{
    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(sql, "insert into announce set pid = %d, issyschat = %d, isadminchat = %d,starttime = %u, endtime = %u, sendinterval = %d, lastsendtime = %u, state = %d, content = '",
        record.m_nPID, record.m_bIsSysChat == false ? 0 : 1, record.m_bIsAdminChat == false ? 0 : 1, record.m_nStartTime, record.m_nEndTime, record.m_nSendInterval, record.m_nLastSendTime, record.m_nState);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, record.m_strContent.c_str(), record.m_strContent.size());
    strcat(end, "'");

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        record.m_nID = (unsigned int)mysql_insert_id(&m_mysqlConnection);
    }

    return nRes;
}

int GMDBInterface::UpdateAnnouce(CAnnounceInfo & record)
{
    int nRes = Query_Res_Fail;

    char *end = sql;

    sprintf(end, "update announce set lastsendtime = %d, state = %d where id = %u", record.m_nLastSendTime, record.m_nState, record.m_nID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int GMDBInterface::RemoveAnnounce(CAnnounceInfo & record)
{
    int nRes = Query_Res_Fail;

#define ANNOUNCE_FINISHED 2
    sprintf(sql, "update announce set state = %d where id = %u", ANNOUNCE_FINISHED, record.m_nID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int GMDBInterface::LoadAnnounce(std::map< unsigned int, CAnnounceInfo*> & mapAnnounce)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select id, pid, issyschat, isadminchat, starttime, endtime, content, sendinterval, lastsendtime, state from announce where state >= 0 and state < 2");

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CAnnounceInfo *pRecord = new CAnnounceInfo;
            if (pRecord != NULL)
            {
                pRecord->m_nID = strtoul(row[nField++], NULL, 10);
                pRecord->m_nPID = (unsigned short)atoi(row[nField++]);
                pRecord->m_bIsSysChat = atoi(row[nField++]) == 1;
                pRecord->m_bIsAdminChat = atoi(row[nField++]) == 1;
                pRecord->m_nStartTime = strtoul(row[nField++], NULL, 10);
                pRecord->m_nEndTime = strtoul(row[nField++], NULL, 10);
                pRecord->m_strContent = row[nField++];
                pRecord->m_nSendInterval = atoi(row[nField++]);
                pRecord->m_nLastSendTime = strtoul(row[nField++], NULL, 10);
                pRecord->m_nState = (CAnnounceInfo::EAnnounceState)(char)atoi(row[nField]);

                mapAnnounce.insert(make_pair(pRecord->m_nID, pRecord));
            }
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}

int GMDBInterface::AddUser(const char* szName, const char* szPassword)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "insert into account set name = '%s' , passwd = '%s' , lastlogintime = 0 , priv = 0 ", szName, szPassword);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int GMDBInterface::LoadUsers(std::vector<CGMRoleInfoDB>* pRoleList)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select account , name , passwd , priv from account");
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        if (pRoleList != NULL)
        {
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                CGMRoleInfoDB role;
                role.m_nAccount = atoi(row[nField++]);
                strncpy(role.m_szName, row[nField++], GM_MAX_NAME_LEN);
                strncpy(role.m_szPassword, row[nField++], GM_MAX_PASSWD_LEN);
                role.m_nPriv = atoi(row[nField++]);

                pRoleList->push_back(role);
            }
        }
    }
    return nRes;
}

int GMDBInterface::SetUserPriv(CGMRoleInfoDB* pRole)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update account set priv = %d where name = '%s'", pRole->m_nPriv, pRole->m_szName);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int GMDBInterface::DelUser(CGMRoleInfoDB* pRole)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "delete from account where name = '%s'", pRole->m_szName);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int GMDBInterface::LoadRechargeRecord(IosForbidRechargeDB & request)
{
    sprintf(sql, "SELECT a.deviceid, COUNT(DISTINCT a.account), count(a.account),sum(a.amount) FROM nsytaccountdb.chargelog as a " 
        " WHERE UNIX_TIMESTAMP(a.dt) >= %u and UNIX_TIMESTAMP(a.dt) <= %u and a.deviceid is not null GROUP BY a.deviceid limit %u, %u;",
        request.m_nStartTime, request.m_nEndTime, request.m_nQueryBeginIndex, request.m_nQueryCount);

    if (!ExecSQL())
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        int field = 0;
        DeviceRechargeInfo device;
        device.m_strDeviceID = row[field++];
        device.m_nAccountCount = (unsigned int)atoi(row[field++]);
        device.m_nRechargeCount = (unsigned int)atoi(row[field++]);
        device.m_nAllAmount = (unsigned int)atoi(row[field++]);
        request.m_listRecharge.push_back(device);
    }

    return Query_Res_OK;
}

int GMDBInterface::WriteUserLockRecord(PlayerLockRequestDb &request)
{
    unsigned int nAccountID = 0;
    unsigned int nRoleID = 0;
    unsigned int nRoleLev = 0;
    std::string strRoleName;

    // 1.get account
    sprintf(sql, "SELECT account FROM nsytaccountdb.player WHERE pid=%d AND puid='%s'", request.m_nPid, request.m_strPuid.c_str());
    if (!ExecSQL())
        return Query_Res_Fail;
    if ((row = FetchRow()) == NULL)
        return Query_Res_Fail;
    nAccountID = atoi(row[0]);
    SafeFreeResult();

    // 2.get role info
    sprintf(sql, "SELECT roleid, rolename, level FROM nsytroledb.role WHERE account = %d", nAccountID);
    if (!ExecSQL())
        return Query_Res_Fail;
    if ((row = FetchRow()) == NULL)
        return Query_Res_Fail;
    nRoleID = atoi(row[0]);
    strRoleName = row[1];
    nRoleLev = atoi(row[2]);
    SafeFreeResult();

    // 3.write
    EscapeString(sql, strRoleName.c_str(), strRoleName.length());
    strRoleName = sql;
    sprintf(sql, "INSERT INTO nsytgmdb.lock_record(roleID, roleLevel, roleName, lockType, lockReason, isLock, operator, opertime) "
        "VALUE(%u, %d, '%s', %d, %d, %d, '%s', NOW())",
        nRoleID, nRoleLev, strRoleName.c_str(), request.m_nLockType, request.m_nLockReason,
        request.m_bIsLocked, request.m_strOperatorName.c_str());

    if (!ExecSQL())
        return Query_Res_Fail;
    SafeFreeResult();

    return Query_Res_OK;
}

int GMDBInterface::LoadUserLockRecord(PlayerLockQueryDb &request)
{
    sprintf(sql, "SELECT roleID, roleLevel, roleName, lockType, lockReason, operator, UNIX_TIMESTAMP(opertime) "
        "FROM lock_record WHERE opertime > FROM_UNIXTIME(%u) AND opertime < FROM_UNIXTIME(%u) AND isLock <> 0",
        request.m_nStartTime, request.m_nEndTime);

    if (!ExecSQL())
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        int field = 0;
        PlayerLockedItem item;
        item.m_nRoleID = atoi(row[field++]);
        item.m_nRoleLevel = (unsigned short)atoi(row[field++]);
        item.m_strRoleName = row[field++];
        item.m_nLockType = (unsigned short)atoi(row[field++]);
        item.m_nLockReason = (unsigned short)atoi(row[field++]);
        item.m_strOperatorName = row[field++];
        item.m_nLockTime = atoi(row[field++]);

        request.m_lstLockItem.push_back(item);
    }

    return Query_Res_OK;
}


int GMDBInterface::LoadHarassInfo(HarassReqDb &harassDb, RoleHarassInfoAMap &harassMap, std::set<unsigned int> &setRoleID, unsigned int roleIdMin, unsigned int roleIdMax)
{
    EscapeString(sql, harassDb.m_strHarassInfo.c_str(), harassDb.m_strHarassInfo.length());
    std::string harassInfo = sql;

    std::string strSQL;
    Formatter::Format(strSQL, "SELECT a.role_id, COUNT(a.role_id) as rnum "
        "FROM log_RoleChat a "
        "WHERE a.time > %u AND a.time < %u AND a.channel <> 3 AND a.channel <> 4 AND a.role_id >= %lu AND a.role_id <= %lu "
        "AND a.content LIKE '%s' "
        "GROUP BY a.role_id ORDER BY rnum DESC",
        harassDb.m_nStartTime, harassDb.m_nEndTime, roleIdMin, roleIdMax, harassInfo.c_str());

    if (!ExecSQL(strSQL.c_str(), strSQL.length()))
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        RoleHarassInfo ri;
        int field = 0;
        ri.m_nRoleID = atoi(row[field++]);
        ri.m_nHarassNum = atoi(row[field++]);
        harassDb.m_lstRoleHarass.push_back(ri); // 
        harassMap.insert(std::make_pair(ri.m_nRoleID, &(*harassDb.m_lstRoleHarass.rbegin())));
    
        setRoleID.insert(ri.m_nRoleID);
    }

    return Query_Res_OK;
}

int GMDBInterface::LoadHarassRoleInfo(HarassReqDb &harassDb, RoleHarassInfoAMap &harassMap, std::set<unsigned int> &setRoleID)
{
    std::string strRoleID = "";
    ConcatToString(setRoleID, ",", strRoleID);

    std::string strSQL;

    Formatter::Format(strSQL, "SELECT c.roleid, c.level, c.rolename, b.account "
        "FROM nsytaccountdb.player b, nsytroledb.role c "
        "WHERE c.account = b.account AND "
        "b.blocktime = 0 AND c.roleid in (%s) ", strRoleID.c_str());

    if (!ExecSQL(strSQL.c_str(), strSQL.length()))
        return Query_Res_Fail;

    std::set<unsigned int> accuteRoleSet;
    while ((row = FetchRow()) != NULL)
    {
        unsigned int roleID = atoi(row[0]);

        RoleHarassInfoAMap::iterator iter = harassMap.find(roleID);
        if (iter == harassMap.end())
            continue;;

        iter->second->m_nRoleLev = (unsigned int)atoi(row[1]);
        iter->second->m_strRoleName = row[2];
        iter->second->m_bInited = true;
        //         iter->second->m_nRoleID

        accuteRoleSet.insert(roleID);
    }

    if (accuteRoleSet.empty())
    {
        harassDb.m_lstRoleHarass.clear();
        return Query_Res_OK;
    }

    ConcatToString(accuteRoleSet, ",", strRoleID);
    // load player locked num..
    Formatter::Format(strSQL, "SELECT roleID, COUNT(*) AS rnum FROM nsytgmdb.lock_record WHERE roleID in(%s) GROUP BY roleID", strRoleID.c_str());

    if (!ExecSQL(strSQL.c_str(), strSQL.length()))
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        unsigned int roleID = atoi(row[0]);
        int lockNum = atoi(row[1]);

        RoleHarassInfoAMap::iterator iter = harassMap.find(roleID);
        if (iter != harassMap.end())
            iter->second->m_nLockedTimes = lockNum;
    }

    // clear unint harass info.
    for (RoleHarassInfoLst::iterator iter = harassDb.m_lstRoleHarass.begin(); iter != harassDb.m_lstRoleHarass.end(); )
    {
        if (!iter->m_bInited)
            iter = harassDb.m_lstRoleHarass.erase(iter);
        else
            ++iter;
    }


    return Query_Res_OK;
}


int GMDBInterface::LoadHarassChatInfo(HarassChatReqDb &harassChatReqDb)
{
    EscapeString(sql, harassChatReqDb.m_strHarassChat.c_str(), harassChatReqDb.m_strHarassChat.length());
    std::string strHarassEscape = sql;

    // SELECT channel, content, time FROM nsytlogdb.log_rolechat WHERE role_id = 0 AND time > 0 AND time < 100000;
    sprintf(sql, "SELECT channel, content, time "
        "FROM log_RoleChat "
        "WHERE role_id = %u AND time > %u AND time < %u AND channel <> 3 AND channel <> 4 AND content LIKE '%s' LIMIT 100",
        harassChatReqDb.m_nRoleID, harassChatReqDb.m_nStartTime, harassChatReqDb.m_nEndTime, strHarassEscape.c_str());

    if (!ExecSQL())
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        RoleChatInfo ri;
        ri.m_nChannel = atoi(row[0]);
        ri.m_strContent = row[1];
        ri.m_nChatTime = atoi(row[2]);
        harassChatReqDb.m_lstRoleChatInfo.push_back(ri);
    }

    return Query_Res_OK;
}

int GMDBInterface::LoadPlayerInfoForHarassChatInfo(HarassChatReqDb &harassChatReqDb)
{
    // 查找这个人的 puid 信息
    //    SELECT p.account, p.pid, p.puid FROM nsytroledb.role r, nsytaccountdb.player p 
    //     WHERE r.roleid = 1001 AND r.account=p.account;
    sprintf(sql, "SELECT p.account, p.pid, p.puid FROM nsytroledb.role r, nsytaccountdb.player p "
        "WHERE r.roleid = %u AND r.account=p.account", harassChatReqDb.m_nRoleID);

    if (!ExecSQL())
        return Query_Res_Fail;

    if ((row = FetchRow()) == NULL)
        return Query_Res_Fail;

    harassChatReqDb.m_nPlatformID = atoi(row[1]);
    harassChatReqDb.m_strPuid = row[2];

    return Query_Res_OK;
}


int GMDBInterface::WriteGMLog(unsigned int nAccount, int nAction, const char* param)
{
    int nRes = Query_Res_Fail;
    char *szBuff = new char[strlen(param) * 3 + 100];
    char *end = szBuff;
    sprintf(end, "insert into log_gmnew set account = %d , action = %d, time = %d , result = 0 ,param = '",
        nAccount, nAction, (int)time(NULL));
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, param, strlen(param));
    sprintf(end, "'");
    end += strlen(end);

    if (ExecSQL(szBuff, end - szBuff))
    {
        nRes = Query_Res_OK;
    }
    delete[] szBuff;
    return nRes;
}

int GMDBInterface::LoadGMLog(QueryLogDb *pQueryDb)
{
    sprintf(sql, "SELECT log_id, name, time, param FROM log_gmnew l, account a WHERE l.account = a.account AND time > %u AND time < %u AND action = %d", 
        pQueryDb->m_nStartTime, pQueryDb->m_nEndTime, (int)pQueryDb->logType);

    if (!ExecSQL())
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        QueryLogResult r;
        r.m_nLogID = atoi(row[0]);
        r.m_strGmaAccountName = row[1];
        r.m_nOperTime = atoi(row[2]);
        r.m_strOperParamJson = row[3];
        pQueryDb->m_vecLogs.push_back(r);
    }

    return Query_Res_OK;
}

int GMDBInterface::LoadGMLogItem(QueryLogItemDb *pQueryDb)
{
    sprintf(sql, "SELECT param FROM log_gmnew WHERE log_id = %u", pQueryDb->m_nLogID);

    if (!ExecSQL())
        return Query_Res_Fail;

    if ((row = FetchRow()) != NULL)
    {
        pQueryDb->jsonParam = row[0];
    }

    return Query_Res_OK;
}


int GMDBInterface::ChangePassword(unsigned int nAccount, const char* szNewPwd)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update account set passwd = '%s' where account = %d ", szNewPwd, nAccount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

