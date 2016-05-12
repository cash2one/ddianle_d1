/********************************************************************
created:	2010/10/12
author:		Mei jiuhua
FileName    dbinterface.cpp
Description interface for accessing mysql database
********************************************************************/

#include "../../socket/Formatter.h"
#include "DBInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/LogicInitData.h"
#include "../../socket/Md5.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"
#include "../payment/PaymentInfoMgr.h"
#include "../player/PlayerSettingDefine.h"
#include <algorithm>

#include <sstream>
#include "../Couple/CoupleManager.h"
#include "../../datastructure/datainfor/ItemConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../mall/CurrencyType.h"
#include "../Rank/RankDefine.h"
#include "../account/AccountConfigMgr.h"
#include "../../socket/Formatter.h"
#include "../GM/GameMsg_GM_share.h"
#include "../Pet/Pet.h"
#include "../Pet/PetEgg.h"

#include "../util/Split.h"
#include "../util/NameDeny.h"
#include "../Handbook/HandbookStruct.h"
#include "../activity/RedEnvelope/RedEnvelopeConfigManager.h"

//数据库表名的最大长度q

#define MAX_TABLE_NAME	20

// #ifndef DAY_PRE_SECONDS
// #define DAY_PRE_SECONDS     (24 * 60 * 60)
// #endif
// 
// #ifndef MONTH_PRE_SECONDS
// #define MONTH_PRE_SECONDS   (31 * DAY_PRE_SECONDS)
// #endif
// 

CDBInterface::CDBInterface()
{
}

CDBInterface::~CDBInterface()
{
}

int CDBInterface::GetRegistedRoleCount(int& nCount)
{
    sprintf(sql, "SELECT COUNT(roleid) rolecount FROM role");
    bool bRes = ExecSQL();
    if (bRes)
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            nCount = atoi(row[0]);
            TRACE("Registed role count = %u\n", nCount);
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::GetRegistedPlayerCount(int& nCount)
{
    sprintf(sql, "SELECT COUNT(account) usercount FROM player");
    bool bRes = ExecSQL();
    if (bRes)
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            nCount = atoi(row[0]);
            TRACE("Registed user count = %u\n", nCount);
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::GetBatchID64(__int64* pID)
{
    sprintf(sql, "INSERT INTO id64 VALUES()");
    bool bRes = ExecSQL();
    if (bRes)
    {
        *pID = mysql_insert_id(&m_mysqlConnection);
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::CheckAndCreateAccount(AccountInfo* pInfo)
{
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }

    int nRes = ExistAccountName(pInfo);
    if (nRes != Query_Res_OK)
    {
        return nRes;
    }

    nRes = CreateAccount(pInfo);
    if (nRes != Query_Res_OK)
    {
        return nRes;
    }

    nRes = CreateAccountBill(pInfo);
    return nRes;
}

int CDBInterface::ExistAccountName(LPACCOUNTINFO pInfo)
{
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }

    if (pInfo->nPID == PID_INTERNAL)
    {
        char *end = sql;
        sprintf(end, "SELECT account FROM player WHERE pid = %d AND username = '", (int)pInfo->nPID);
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pInfo->szName, strlen(pInfo->szName));
        strcat(end, "'");
        end += strlen(end);

        bool bRes = ExecSQL();
        if (bRes)
        {
            if (mysql_num_rows(result) > 0)
            {
                return Query_Res_Fail;
            }
        }
        SafeFreeResult();
    }

    return Query_Res_OK;
}

int CDBInterface::CreateAccount(AccountInfo * pInfo)
{
    if (pInfo == NULL || pInfo->nAccount > 0)
    {
        return Query_Res_Fail;
    }

    if (pInfo->nPID == PID_INTERNAL && (strlen(pInfo->szName) == 0 || strlen(pInfo->szPass) == 0))
    {
        return Query_Res_Fail;
    }

    char szSQLPUID[MAX_PUID_LEN] = { 0 };
    char szSQLName[MAX_NAME_LEN] = { 0 };
    char szSQLPass[MAX_PASS_LEN] = { 0 };
    char szSQLPhone[MAX_PHONE_LEN] = { 0 };

    CDBIMysql::EscapeString(szSQLPUID, pInfo->szPUID, strlen(pInfo->szPUID));
    CDBIMysql::EscapeString(szSQLName, pInfo->szName, strlen(pInfo->szName));
    CDBIMysql::EscapeString(szSQLPass, pInfo->szPass, strlen(pInfo->szPass));
    CDBIMysql::EscapeString(szSQLPhone, pInfo->szPhone, strlen(pInfo->szPhone));

    sprintf(sql, "INSERT INTO player set type = %d, pid = %d, puid = '%s', username = '%s', "
        " passwd = '%s', phone = '%s'", ACCOUNT_TYPE_MANUAL, pInfo->nPID, szSQLPUID, szSQLName,
        szSQLPass, szSQLPhone);

    bool bRes = ExecSQL();
    if (bRes)
    {
        pInfo->nAccount = (unsigned int)mysql_insert_id(&m_mysqlConnection);
        bRes = (pInfo->nAccount > 0);
    }

    SafeFreeResult();
    if (pInfo->nPID == PID_INTERNAL)
    {
        sprintf(sql, "update player set puid = md5(username) where account = %u", pInfo->nAccount);
        bRes = ExecSQL();
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::CreateAccountBill(LPACCOUNTINFO pInfo)
{
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }

    sprintf(sql, "INSERT INTO bill SET account = %u", pInfo->nAccount);
    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

unsigned int CDBInterface::GetAccountByAccountName(char *strAccountName, int nPID)
{
    unsigned int nAccount = 0;
    char *end = sql;
    sprintf(end, "SELECT account FROM player WHERE pid = %d, username = '", nPID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, strAccountName, strlen(strAccountName));
    strcat(end, "'");
    end += strlen(end);

    bool bRes = ExecSQL();//sql, (unsigned int) (end - sql)
    if (bRes)
    {
        if ((row = FetchRow()) != NULL)
        {
            nAccount = (unsigned int)atoi(row[0]);
        }
    }
    SafeFreeResult();
    return nAccount;
}

int CDBInterface::GetAccountInfo(LPACCOUNTINFO pInfo, bool &canCreate)
{
    if (pInfo->nAccount > 0)
    {
        sprintf(sql, "SELECT account, pid, puid, type, username, passwd, idcard, blocktime, state, lastlogintime, lastlogouttime, errors, phone FROM player WHERE account=%u", pInfo->nAccount);
    }
    else
    {
        if (pInfo->nPID == PID_INTERNAL)
        {
            char *end = sql;
            sprintf(end, "SELECT account, pid, puid, type, username, passwd, idcard, blocktime, state, lastlogintime, lastlogouttime, errors, phone FROM player WHERE pid = %d AND username = '", pInfo->nPID);
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, pInfo->szName, strlen(pInfo->szName));
            strcat(end, "'");
            end += strlen(end);
        }
        else
        {
            char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
            CDBIMysql::EscapeString(szSQLPUID, pInfo->szPUID, strlen(pInfo->szPUID));
            sprintf(sql, "SELECT  account, pid, puid, type, username, passwd, idcard, blocktime, state, lastlogintime, lastlogouttime, errors, phone FROM player WHERE pid = %d AND puid = '%s'", pInfo->nPID, szSQLPUID);
        }
    }

    bool bUpdatePhone = false;
    std::string strOldPhone = "";
    bool bRes = ExecSQL();
    if (bRes)
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            int nField = 0;
            pInfo->nAccount = strtoul(row[nField++], &pStrStop, 10);
            pInfo->nPID = (unsigned short)atoi(row[nField++]);
            strcpy(pInfo->szPUID, row[nField++]);
            pInfo->nType = atoi(row[nField++]);
            strcpy(pInfo->szName, row[nField++]);
            strncpy(pInfo->szPass, row[nField++], MAX_PASS_LEN - 1);
            strcpy(pInfo->szIDCard, row[nField++]);
            pInfo->nBlockTime = strtoul(row[nField++], &pStrStop, 10);
            pInfo->nBlockState = strtoul(row[nField++], &pStrStop, 10);
            pInfo->nLastLoginTime = strtoul(row[nField++], &pStrStop, 10);
            pInfo->nLastLogoutTime = strtoul(row[nField++], &pStrStop, 10);
            pInfo->nLoginError = strtoul(row[nField++], &pStrStop, 10);
            strOldPhone = row[nField++];
        }
        else if (pInfo->nPID != PID_INTERNAL && strlen(pInfo->szPUID) > 0)
        {
            // 检测是否禁止注册的平台：
            if (AccountConfigMgr::Instance().IsForbiddenRegPlatform(pInfo->nPID))
            {
                canCreate = false; // 
            }
            else
            {
                bRes = (CheckAndCreateAccount(pInfo) == Query_Res_OK ? true : false);
                bUpdatePhone = true;
            }
        }
    }
    SafeFreeResult();
    if (bRes && canCreate && bUpdatePhone)
    {
        UpdatePlayerPhone(pInfo->nAccount, pInfo->szPhone, strOldPhone);
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

// 获得账号ID
int CDBInterface::GetAccountID(unsigned short nPID, std::string strPUID, unsigned int & rnAccountID)
{
    char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLPUID, strPUID.c_str(), strPUID.length());
    rnAccountID = 0;
    sprintf(sql, "SELECT account FROM player WHERE pid = %d AND puid = '%s'", nPID, szSQLPUID);

    bool bRes = ExecSQL();
    if (bRes)
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            int nField = 0;
            rnAccountID = strtoul(row[nField++], &pStrStop, 10);
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePlayerPhone(unsigned int nAccount, std::string strNewPhone, std::string strOldPhone)
{
    if (strNewPhone.empty())
    {
        return Query_Res_OK;
    }
    if (strNewPhone == strOldPhone)
    {
        return Query_Res_OK;
    }
    char szSQLPhone[MAX_PHONE_LEN] = { 0 };
    CDBIMysql::EscapeString(szSQLPhone, strNewPhone.c_str(), strNewPhone.length());

    sprintf(sql, "UPDATE player SET phone='%s' WHERE account=%u", szSQLPhone, nAccount);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}
int CDBInterface::UpdateAccountState(LPACCOUNTINFO pInfo)
{
    sprintf(sql, "UPDATE player SET state=%u,blocktime=%u,lastlogintime=%u,errors=%u WHERE account=%u",
        pInfo->nBlockState, pInfo->nBlockTime, pInfo->nLastLoginTime, pInfo->nLoginError, pInfo->nAccount);
    bool bRes = ExecSQL();
    SafeFreeResult();

    //记录用户的登陆日志
    unsigned int nCurTime = (unsigned int)time(NULL);
    sprintf(sql, "INSERT INTO playerloginlog SET account=%u , logintime = %u, loginip='%s'",
        pInfo->nAccount, nCurTime, pInfo->szLoginIp);//IP从gate获得，所以没有CDBIMysql::EscapeString
    bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateAccountLockState(LockAccountInfo* pInfo)
{
    // 按位封停。
    // state 字段：
    // xxxxx0000x
    // 前边n位|封停类型(三位)|是否被封（1位）|在线类型（1位）
    pInfo->nBlockTime = 0;
    //先解锁
    unsigned int mask = 0xFFFFFFE1;
    sprintf(sql, "UPDATE player SET state=state & %u, blocktime=%u WHERE account=%u",
        mask, pInfo->nBlockTime, pInfo->nAccount);
    bool bRes = ExecSQL();
    SafeFreeResult();
    //锁定
    if (bRes && pInfo->nBlockType >= 0)
    {
        pInfo->nBlockTime = (unsigned int)time(NULL);
        mask = (pInfo->nBlockType << 2) + 0x2;//设置锁定标记和锁定类型
        sprintf(sql, "UPDATE player SET state=state | %u, blocktime=%u WHERE account=%u",
            mask, pInfo->nBlockTime, pInfo->nAccount);

        bRes = ExecSQL();
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::ResetAccountState(unsigned int nAccount, int nStateLogin)
{
    // 指定账号时，把指定账号改为上线或下线状态; 未指定账号时，则把全部账号改为下线状态
    if (nAccount > 0)
    {
        time_t timeNow = time(NULL);
        if (nStateLogin == 1)
        {
            sprintf(sql, "UPDATE player SET state = state | 1 WHERE account=%u", nAccount);
        }
        else
        {
            sprintf(sql, "UPDATE player SET state = state & 0xFFFFFFFE, lastlogouttime = %u WHERE account=%u",
                (unsigned int)timeNow, nAccount);
        }
    }
    else
    {
        strcpy(sql, "UPDATE player SET state = state & 0xFFFFFFFE");
    }
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}


int CDBInterface::AccountLogout(unsigned int nAccount, unsigned int nLoginTime)
{
    unsigned int nNowTime = (unsigned int)time(NULL);
    sprintf(sql, "INSERT INTO playerlogoutlog SET account=%u, logintime = %u, logouttime = %u, onlinetime=%d",
        nAccount, nLoginTime, nNowTime, (int)(nNowTime - nLoginTime));
    ExecSQL();
    SafeFreeResult();

    return ResetAccountState(nAccount, 0);
}

int CDBInterface::LoadBillInfo(BILLINFO* pBillInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT pt,bindpt,changes,totalpt,totalbindpt,totalgift FROM nsytaccountdb.bill WHERE account=%u", pBillInfo->nAccount);
    bool bRes = ExecSQL();
    if (bRes)
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            int nField = 0;
            pBillInfo->nPoint = (unsigned int)strtoul(row[nField++], NULL, 10);
            pBillInfo->nBindPoint = (unsigned int)strtoul(row[nField++], NULL, 10);
            pBillInfo->nChange = atoi(row[nField++]);
            pBillInfo->nTotalPoint = (unsigned int)strtoul(row[nField++], NULL, 10);
            pBillInfo->nTotalBindPoint = (unsigned int)strtoul(row[nField++], NULL, 10);
            pBillInfo->nTotalGift = atoi(row[nField++]);
            nRes = Query_Res_OK;
        }
        else//bill 表中没有该账号的信息
        {
            SafeFreeResult();
            sprintf(sql, "INSERT INTO bill SET account=%u", pBillInfo->nAccount);
            bRes = ExecSQL();
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    return nRes;
}

/*int CDBInterface::LoadLastLogoutTime(unsigned int nAccount, unsigned int & nLastLogoutTime)
{
int nRes = Query_Res_Fail;

sprintf(sql, "select lastlogouttime, pid from nsytaccountdb.player where account = %u", nAccount);
if (ExecSQL()) {
nRes = Query_Res_OK;
row = FetchRow();
if (row != NULL) {
int nFiled = 0;
rRoleDBInfo.nLastLogoutTime = strtoul(row[nFiled++],&pStrStop, 10);
rRoleDBInfo.m_nPID = (unsigned short)atoi(row[nFiled]);
}
}

SafeFreeResult();

return nRes;
}*/

int CDBInterface::UpdateBillPt(ChongZhiInfo * pChongZhiInfo)
{
    //检查是不是重复充值了
    char *end = sql;
#ifdef SERVER_IOS
    sprintf(end, "SELECT account FROM chargelog WHERE orderid = '");
#else
    sprintf(end, "SELECT account FROM chargelog WHERE account = %u AND orderid = '", pChongZhiInfo->nAccount);
#endif
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pChongZhiInfo->strOrderID.c_str(), pChongZhiInfo->strOrderID.length());
    strcat(end, "'");
    end += strlen(end);

    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            SafeFreeResult();
            WriteLog(LOGLEVEL_ERROR, "[CHARGE] error: duplicated order orderid=%s", pChongZhiInfo->strOrderID.c_str());
            return Query_Res_Fail;
        }
    }
    SafeFreeResult();

    //充值本金
    unsigned int nAmount = pChongZhiInfo->nAllAmount;

    //奖励绑定M币
    int nRewardBindPoint = pChongZhiInfo->nRechargeBindMCoinReward;
    if (nRewardBindPoint < 0)
    {
        nRewardBindPoint = 0;
    }

    unsigned int nRoleID = 0;
    unsigned int nMoney = 0;
    unsigned int nPoint = 0;
    unsigned int nBindPoint = 0;
    sprintf(sql, "SELECT nsytroledb.role.roleid, nsytroledb.role.money, bill.pt, bill.bindpt "
        "FROM bill LEFT JOIN nsytroledb.role ON nsytroledb.role.account = bill.account "
        "WHERE bill.account=%u ", pChongZhiInfo->nAccount);
    bool bRes = ExecSQL();
    if (bRes && (nAmount > 0 || nRewardBindPoint > 0 || pChongZhiInfo->m_nFixedChargeBindPoint > 0))
    {
        //如果bill表中有该账号信息
        bRes = false;
        if ((row = FetchRow()) != NULL && row[0] != NULL)
        {
            nRoleID = strtoul(row[0], NULL, 10);
            nMoney = atoi(row[1]);
            nPoint = atoi(row[2]);
            nBindPoint = atoi(row[3]);

            SafeFreeResult();
            sprintf(sql, "UPDATE bill SET pt=pt+%u,totalpt=totalpt+%u,bindpt=bindpt+%u,totalbindpt=totalbindpt+%u WHERE account=%u",
                nAmount, nAmount, nRewardBindPoint + pChongZhiInfo->m_nFixedChargeBindPoint,
                nRewardBindPoint + pChongZhiInfo->m_nFixedChargeBindPoint, pChongZhiInfo->nAccount);
            bRes = ExecSQL();
        }
        else
        {//如果没有,那看看player表中有没有,有的话补上
            SafeFreeResult();

            sprintf(sql, "SELECT nsytroledb.role.roleid,nsytroledb.role.money "
                "FROM player LEFT JOIN nsytroledb.role "
                "ON nsytroledb.role.account = player.account where player.account=%u", pChongZhiInfo->nAccount);
            bRes = ExecSQL();
            if (bRes)
            {
                bRes = false;
                if ((row = FetchRow()) != NULL && row[0] != NULL)
                {
                    nRoleID = atoi(row[0]);
                    nMoney = atoi(row[1]);

                    SafeFreeResult();
                    sprintf(sql, "INSERT INTO bill SET account=%u,pt=%u,bindpt=%u,totalpt=%u,totalbindpt=%u",
                        pChongZhiInfo->nAccount, nAmount, nRewardBindPoint + pChongZhiInfo->m_nFixedChargeBindPoint,
                        nAmount, nRewardBindPoint + pChongZhiInfo->m_nFixedChargeBindPoint);
                    bRes = ExecSQL();
                }
            }
        }
    }

    pChongZhiInfo->nRoleID = nRoleID;

    //bill更新成功
    if (bRes)
    {
        if (nAmount > 0)
        {
            CRoleChangeBillLog log;
            log.m_eChangeBillCause = pChongZhiInfo->eChangeBillCause;
            log.m_nChangeValue = nAmount;
            log.m_nNewPT = nPoint + log.m_nChangeValue;
            log.m_nRoleID = nRoleID;
            LogSQLLog(log);

            nPoint = log.m_nNewPT;
        }

        if (nRewardBindPoint > 0)
        {
            CRoleChangeBindBillLog log;
            log.m_eChangeBindBillCause = pChongZhiInfo->eChangeBindBillRewardCause;
            log.m_nChangeValue = nRewardBindPoint;
            log.m_nNewBindPT = nBindPoint + log.m_nChangeValue;
            log.m_nRoleID = nRoleID;
            LogSQLLog(log);

            nBindPoint = log.m_nNewBindPT;
        }

        if (pChongZhiInfo->m_nFixedChargeBindPoint > 0)
        {
            CRoleChangeBindBillLog log;
            log.m_eChangeBindBillCause = EChangeBindBillCause_FixedCharge;
            log.m_nChangeValue = pChongZhiInfo->m_nFixedChargeBindPoint;
            log.m_nNewBindPT = nBindPoint + log.m_nChangeValue; // 充值之后的金额
            log.m_nRoleID = nRoleID;
            LogSQLLog(log);

            nBindPoint = log.m_nNewBindPT;
        }
    }

    /*不在线时奖励的金券改成放入邮件，让玩家上线后自领，此设计的原因是玩家金券会每月清零，所以后面金券奖励的决断都加上了bOnline条件*/

    // 处理 金券奖励
    int nRewardMoney = pChongZhiInfo->nRechargeMoneyReward + pChongZhiInfo->nFeedVersionMoneyReward;
    if (nRewardMoney < 0)
    {
        nRewardMoney = 0;
    }

    if (bRes && nRewardMoney > 0 && pChongZhiInfo->bOnline)
    {
        bRes = UpdateRoleMoney(nRoleID, nRewardMoney) == Query_Res_OK ? true : false;
    }

    // 免费版本金券奖励日志
    if (bRes && pChongZhiInfo->nRechargeMoneyReward > 0 && pChongZhiInfo->bOnline)
    {
        CRoleChangeMoneyLog log;
        log.m_eChangeMoneyCause = pChongZhiInfo->eChangeMoneyRewardCause;
        log.m_nChangeValue = pChongZhiInfo->nRechargeMoneyReward;
        log.m_nNewMoney = nMoney + pChongZhiInfo->nRechargeMoneyReward;
        log.m_nRoleID = nRoleID;
        LogSQLLog(log);

        nMoney = log.m_nNewMoney;
    }

    // 收费版充值奖励金券日志
    if (bRes && pChongZhiInfo->nFeedVersionMoneyReward > 0 && pChongZhiInfo->bOnline)
    {
        CRoleChangeMoneyLog log;
        log.m_eChangeMoneyCause = EChangeMoneyCause_Add_ChargeReward;
        log.m_nChangeValue = pChongZhiInfo->nFeedVersionMoneyReward;
        log.m_nNewMoney = nMoney + pChongZhiInfo->nFeedVersionMoneyReward;
        log.m_nRoleID = nRoleID;
        LogSQLLog(log);

        nMoney = log.m_nNewMoney;
    }
    SafeFreeResult();

    //如果充值成功，则记log，这个很重要，对账需要用到此记录
    if (bRes)
    {
        end = sql;
        sprintf(end, "INSERT INTO chargelog SET account=%u,pid=%d,type=%d,pt=%u,amount=%u,rewardtype=%d,rewardpt=%u,rewardmoney=%u,orderid='",
            pChongZhiInfo->nAccount, pChongZhiInfo->nPID, pChongZhiInfo->nVersionType, pChongZhiInfo->nAllAmount, pChongZhiInfo->nAmount,
            (int)pChongZhiInfo->eChargeRewardType, nRewardBindPoint + pChongZhiInfo->m_nFixedChargeBindPoint, nRewardMoney);
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pChongZhiInfo->strOrderID.c_str(), pChongZhiInfo->strOrderID.length());
        sprintf(end, "', partner_orderid='");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pChongZhiInfo->strPartnerOrderID.c_str(), pChongZhiInfo->strPartnerOrderID.length());
        *end++ = '\'';
        *end++ = '\0';

        if (!pChongZhiInfo->strDeviceID.empty())
        {
            char strDeviceID[128] = { 0 };
            char strSubSql[160] = { 0 };
            CDBIMysql::EscapeString(strDeviceID, pChongZhiInfo->strDeviceID.c_str(), (unsigned long)pChongZhiInfo->strDeviceID.size());
            sprintf(strSubSql, ", deviceid='%s'", strDeviceID);

            strcat(sql, strSubSql);
        }
        TRACE(sql);

        bRes = ExecSQL(sql, (unsigned int)strlen(sql));
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::AddFirstPaymentInfo(unsigned int nAccount, unsigned int nCount)
{
    int nRes = Query_Res_Fail;
    if (nAccount > 0) {
        unsigned int nNow = (unsigned int)time(NULL);
        sprintf(sql, "call add_first_payment_info(%u, %u,%u)", nAccount, nCount, nNow);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}
int CDBInterface::AddFirstPaymentInfoFeed(unsigned int nAccount, unsigned int nCount)
{
    int nRes = Query_Res_Fail;
    if (nAccount > 0) {
        unsigned int nNow = (unsigned int)time(NULL);
        sprintf(sql, "call add_first_payment_info_feed(%u, %u, %u)", nAccount, nCount, nNow);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}
int CDBInterface::AddSendFirstPaymentReward(unsigned int nAccount, unsigned int nCount)
{
    int nRes = Query_Res_Fail;
    if (nAccount > 0) {
        unsigned int nNow = (unsigned int)time(NULL);
        sprintf(sql, "call add_send_payment_reward(%u, %u, %u)", nAccount, nCount, nNow);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}
int CDBInterface::AddSendFirstPaymentRewardFeed(unsigned int nAccount, unsigned int nCount)
{
    int nRes = Query_Res_Fail;
    if (nAccount > 0) {
        unsigned int nNow = (unsigned int)time(NULL);
        sprintf(sql, "call add_send_payment_reward_feed(%u, %u, %u)", nAccount, nCount, nNow);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::UpdateBillPt(unsigned int nAccount, int nPt)
{
    sprintf(sql, "call UpdatePt(%u, %d)", nAccount, nPt);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateBillBindPt(unsigned int nAccount, int nBindPt)
{
    sprintf(sql, "call UpdateBindPt(%u, %d)", nAccount, nBindPt);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateBillChange(unsigned int nAccount, int nPoint)
{
    sprintf(sql, "UPDATE bill SET changes=changes+%d WHERE account=%u", nPoint, nAccount);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::SaveChargeAction(const ChargeAction & action)
{
    char* end = sql;
    strFmt = "INSERT INTO nsytaccountdb.charge_action set puid = '%s', pid = %d, roleid = %u, action = %d, money = %d, orderid = '%s',time = %u";
    sprintf(end, strFmt.c_str(), action.m_strPUID.c_str(), (int)action.m_nPID, action.m_nRoleID, (int)action.m_nAction, action.m_nMoney, action.m_strOrderID.c_str(), (unsigned int)time(NULL));
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

//创建角色
int CDBInterface::CreateRole(CreateRoleInfo *pInfo, int & rError)
{
    // error 1表示该角色已经被创建
    // error 2表示该角色名字被占用
    // 详见 enum ECreateRoleError
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }
    int nRes = Query_Res_Fail;

    sprintf(sql, "select roleid from role where account = %u", pInfo->nAccount);
    if (ExecSQL()) {
        if (FetchRow() > 0) {
            nRes = Query_Res_OK;
            rError = 1;
        }
        else {
            SafeFreeResult();

            int nNowTime = (int)time(NULL);

            char* end = sql;
            sprintf(end, "INSERT INTO nsytroledb.role SET account=%u,sex = %u,color = %d, createtime = %d, rolename ='", pInfo->nAccount, pInfo->nSex, pInfo->nColor, nNowTime);
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, pInfo->szRoleName, strlen(pInfo->szRoleName));
            strcat(end, "',");
            end += strlen(end);

            sprintf(end, "signature = '");
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, pInfo->szSignature, strlen(pInfo->szSignature));
            strcat(end, "',");
            end += strlen(end);

            sprintf(end, "birthday = '");
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, pInfo->GetBirthday().c_str(), pInfo->GetBirthday().size());
            strcat(end, "'");
            end += strlen(end);

            if (ExecSQL(sql, (unsigned int)(end - sql))) {
                nRes = Query_Res_OK;
            }
            else {
                rError = 2;
            }
        }
    }


    SafeFreeResult();

    return nRes;
}
int CDBInterface::CheckRoleName(string &strRoleName)
{
    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(end, "SELECT rolename FROM role WHERE rolename = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, strRoleName.c_str(), strRoleName.length());
    strcat(end, "'");
    end += strlen(end);

    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));

    if (bRes)
    {
        if (FetchRow() != NULL)
        {
            nRes = Query_Res_AlreadyExist;;
        }
        else
        {
            nRes = Query_Res_OK;
        }
    }
    else
    {
        nRes = Query_Res_Fail;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadAutoAccout(list<CAutoAccount *> & listAutoAccount)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT username, account, passwd FROM randomaccount WHERE available = 1");
    if (ExecSQL()){
        while ((row = FetchRow()) != NULL){
            int nField = 0;
            CAutoAccount *pInfo = new CAutoAccount;
            strcpy(pInfo->m_szName, row[nField++]);
            pInfo->m_nAccountID = atoi(row[nField++]);
            strcpy(pInfo->m_szPasswd, row[nField]);
            listAutoAccount.push_back(pInfo);
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateAutoAccoutState(unsigned int nAccount, int nState)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "UPDATE randomaccount SET available = %d WHERE account = %d", nState, nAccount);

    if (ExecSQL()){
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::CreateNewRandomAccount(list<CAutoAccount *> &listAutoAccount, int nCount)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call newAccount(%d)", nCount);

    if (ExecSQL()){
        SafeFreeResult();
        nRes = LoadAutoAccout(listAutoAccount);
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadActivatedCode(map<unsigned int, unsigned char> & mapActivateCode)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select code, valid from activate_key");

    if (ExecSQL()) {
        while ((row = FetchRow()) != NULL) {
            int nField = 0;
            int nKey = atoi(row[nField++]);
            unsigned char nValid = (unsigned char)atoi(row[nField++]);
            mapActivateCode.insert(make_pair(nKey, nValid));
        }
    }

    return nRes;
}

int CDBInterface::ActivateCode(unsigned int nCode, AccountInfo *pInfo)
{
    int nRes = Query_Res_Fail;
    char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLPUID, pInfo->szPUID, strlen(pInfo->szPUID));
    sprintf(sql, "call ActivateCode(%u, '%s')", nCode, szSQLPUID);
    if (ExecSQL()) {
        SafeFreeResult();

        //这里必须要带上平台id，这里先救急改一下
        sprintf(sql, "select account, pid, puid, type, username, passwd, idcard, blocktime, state, lastlogintime, lastlogouttime, errors from player where pid = 2 and puid = '%s'", szSQLPUID);
        if (ExecSQL())
        {
            if (mysql_num_rows(result) > 0)
            {
                row = FetchRow();
                int nField = 0;
                pInfo->nAccount = strtoul(row[nField++], &pStrStop, 10);
                pInfo->nPID = (unsigned short)atoi(row[nField++]);
                strcpy(pInfo->szPUID, row[nField++]);
                pInfo->nType = atoi(row[nField++]);
                strcpy(pInfo->szName, row[nField++]);
                memcpy(pInfo->szPass, row[nField++], MAX_PASS_LEN - 1);
                strcpy(pInfo->szIDCard, row[nField++]);
                pInfo->nBlockTime = strtoul(row[nField++], &pStrStop, 10);
                pInfo->nBlockState = strtoul(row[nField++], &pStrStop, 10);
                pInfo->nLastLoginTime = strtoul(row[nField++], &pStrStop, 10);
                pInfo->nLastLogoutTime = strtoul(row[nField++], &pStrStop, 10);
                pInfo->nLoginError = strtoul(row[nField++], &pStrStop, 10);
            }
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();
    return nRes;
}



/*
int CDBInterface::CheckRoleNameExist(string *pStrRoleName, bool *pExist)
{
if (pStrRoleName == NULL)
{
return Query_Res_Fail;
}

*pExist = false;
char *end = sql;
sprintf(end,"SELECT rolename FROM role WHERE rolename = '");
end += strlen(end);
end += CDBIMysql::EscapeString(end,pStrRoleName->c_str(), pStrRoleName->size());
strcat(end, "'");
end += strlen(end);

bool bRes = ExecSQL(sql, (unsigned int)(end - sql));

if (FetchRow() != NULL)
{
*pExist = true;
}

SafeFreeResult();
return bRes ? Query_Res_OK : Query_Res_Fail;
}
*/

int CDBInterface::UpdateRoleMoney(unsigned int nRoleID, int nMoney)
{
    sprintf(sql, "call nsytroledb.updatemoney(%u, %d)", nRoleID, nMoney);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleMoneyByAccountId(unsigned int nAccountId, int nMoney, unsigned int& nRoleID, unsigned int& nOldMoney)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT roleid, money FROM nsytroledb.role WHERE account = %u;", nAccountId);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            nRoleID = atoi(row[0]);
            nOldMoney = atoi(row[1]);
        }
    }
    if (nRoleID > 0)
    {
        nRes = UpdateRoleMoney(nRoleID, nMoney);
    }

    return nRes;
}

int CDBInterface::UpdateRoleBadgeNum(unsigned int nRoleID, unsigned char nNum)
{
    sprintf(sql, "UPDATE role SET badgegridnum=%u WHERE roleid=%u", nNum, nRoleID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleSignature(unsigned int nRoleID, const char *szWords)
{
    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(sql, "UPDATE role SET signature = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, szWords, strlen(szWords));
    end += strlen(end);
    strcat(end, "'");
    end += strlen(end);
    sprintf(end, "where roleid = %u", nRoleID);

    if (ExecSQL()){
        nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::UpdateRolenName(unsigned int nRoleid, char * newrolename)
{
    bool bRes = false;
    if (strlen(newrolename) < MAX_NAME_LEN)
    {
        char* end = sql;
        sprintf(sql, "SELECT roleid FROM role WHERE rolename ='");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, newrolename, strlen(newrolename));
        strcat(end, "'"); end += strlen(end);

        bRes = ExecSQL(sql, (unsigned int)(end - sql));
        if (bRes)
        {
            bRes = (row = FetchRow()) == NULL;
            //如果新角色名不存在，则可以更改
            if (bRes)
            {
                SafeFreeResult();
                end = sql;
                sprintf(sql, "UPDATE role SET rolename = '");
                end += strlen(end);
                end += CDBIMysql::EscapeString(end, newrolename, strlen(newrolename));
                sprintf(end, "' WHERE roleid =%u", nRoleid);
                end += strlen(end);

                bRes = ExecSQL(sql, (unsigned int)(end - sql));
            }
        }
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRole(LPROLEINFO_DB pRole, int nMode)
{
    sprintf(sql, "UPDATE role "
        "   SET exp=%d, level=%d, lastlogintime=%u, onlinetime = %u, "
#ifndef SERVER_IOS
        "       addictiontime = %u, addictionofflinetime = %u, "
#endif //SERVER_IOS
        "       questdone = %d, clothgroup = %d WHERE roleid=%u",
        pRole->m_nExp, pRole->m_nLevel, pRole->m_nLastLoginTime, pRole->m_nTimeOnline,
#ifndef SERVER_IOS
        pRole->m_nAddictionTime, pRole->m_nAddictionOfflineTime,
#endif //SERVER_IOS
        pRole->m_nQuestDone, (int)pRole->m_nClothGroupId, pRole->m_nRoleID);

    bool bRes = true;
    if (pRole->m_bKeyDataModified)
    {
        bRes = ExecSQL(sql);
        SafeFreeResult();
    }

    if (!pRole->listItem.empty())
    {
        std::list<DBItem>::iterator it = pRole->listItem.begin();
        for (; it != pRole->listItem.end(); ++it)
        {
            sprintf(sql, "UPDATE item SET isquiped = %d WHERE itemid = %lld", it->m_bEquiped ? 1 : 0, it->m_nItemID);
            ExecSQL(sql);
            SafeFreeResult();
        }

    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::GetRoleOfflineTime(unsigned int nRoleID, unsigned int * pOfflineTime)
{
    if (pOfflineTime == NULL)
    {
        return false;
    }

    bool bRes = true;
    sprintf(sql, "SELECT lastlogintime, onlinetime FROM role WHERE roleid = %u", nRoleID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nLoginTime = strtoul(row[nField++], &pStrStop, 10);
            unsigned int nOnlineTime = strtoul(row[nField++], &pStrStop, 10);

            *pOfflineTime = nLoginTime + nOnlineTime;
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleBirthday(unsigned int nRoleID, const string *pStrNewBirthday)
{
    if (NULL == pStrNewBirthday || 0 == nRoleID)
        return Query_Res_Fail;

    int nRes = UpdateEventEntry4(nRoleID, eEvent_LastChangeBirthdayTime, 0, (unsigned int)time(NULL));

    if (Query_Res_OK == nRes)
    {
        char *end = sql;
        sprintf(sql, "UPDATE role SET birthday = '");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pStrNewBirthday->c_str(), pStrNewBirthday->size());
        sprintf(end, "' WHERE roleid = %u", nRoleID);

        return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
    }

    return nRes;
}

int CDBInterface::GetRoleInfo(LPROLEINFO_DB pRole, int nMode)
{
    unsigned int nLen = 0;
    if (pRole->nAccount > 0)
    {
        sprintf(sql,
            "SELECT a.account, a.roleid, a.rolename, a.sex, a.birthday, a.color, a.signature, "
            "       a.hot, a.pre, a.inte, a.apoint, a.money, a.exp, a.level, a.familyid, a.priv, "
            "       a.beibaokuozhan0, a.beibaokuozhan1, a.beibaokuozhan2, a.beibaokuozhan3, a.badgegridnum, "
            "       a.createtime, a.lastlogintime, a.onlinetime, "
#ifndef SERVER_IOS
            "       a.addictiontime, a.addictionofflinetime, "
#endif //SERVER_IOS
            "       a.questdone, a.dance_group_id, a.invitationcode, a.clothgroup, a.divorceagreementid,"
            "       b.pt, b.bindpt, b.changes, b.totalpt, b.totalbindpt, b.totalgift, "
            "       c.lastlogouttime,UNIX_TIMESTAMP(c.createtime),c.pid "
            "  FROM nsytroledb.role a, nsytaccountdb.bill b, nsytaccountdb.player c "
            " WHERE a.account = %u and b.account = a.account and c.account = a.account",
            pRole->nAccount);
    }
    else if (pRole->m_nRoleID > 0)
    {
        sprintf(sql,
            "SELECT a.account, a.roleid, a.rolename, a.sex, a.birthday, a.color, a.signature, "
            "       a.hot, a.pre, a.inte, a.apoint, a.money, a.exp, a.level, a.familyid, a.priv, "
            "       a.beibaokuozhan0, a.beibaokuozhan1, a.beibaokuozhan2, a.beibaokuozhan3, a.badgegridnum, "
            "       a.createtime, a.lastlogintime, a.onlinetime, "
#ifndef SERVER_IOS
            "       a.addictiontime, a.addictionofflinetime, "
#endif //SERVER_IOS
            "       a.questdone, a.dance_group_id, a.invitationcode, a.clothgroup, a.divorceagreementid,"
            "       b.pt, b.bindpt, b.changes, b.totalpt, b.totalbindpt, b.totalgift, "
            "       c.lastlogouttime, UNIX_TIMESTAMP(c.createtime), c.pid "
            "  FROM nsytroledb.role a, nsytaccountdb.bill b, nsytaccountdb.player c "
            " WHERE a.roleid = %u and b.account = a.account and c.account = a.account",
            pRole->m_nRoleID);
    }
    else
    {
        TRACE("Invalid query for roleinfo in CDBInterface::GetRoleInfo()\n");
        return Query_Res_InvalidParam;
    }

    bool bRes = ExecSQL(sql, nLen);
    int n = 0;
    if (bRes)
    {
        while (n < MAX_ROLE_IN_ACCOUNT && (row = FetchRow()) != NULL)
        {
            int nField = 0;
            pRole[n].nAccount = strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nRoleID = strtoul(row[nField++], &pStrStop, 10);
            if (NULL != row[nField])
            {
                pRole[n].SetName(row[nField]);
            }
            nField++;
            pRole[n].m_nSex = (unsigned char)atoi(row[nField++]);
            if (pRole[n].m_nSex != ESexType_Male && pRole[n].m_nSex != ESexType_Female)
            {
                pRole[n].m_nSex = ESexType_Female;
            }
            if (NULL != row[nField])
                pRole[n].SetBirthday(row[nField]);
            nField++;

            int nYear = 0;
            int nMonth = 0;
            int nDay = 0;
            GetBirthDay(atoi(pRole[n].GetBirthday().c_str()), nYear, nMonth, nDay);
            pRole[n].m_nConstellation = (unsigned char)checkConstellation(nMonth, nDay);
            pRole[n].m_nSkinColor = (unsigned char)atoi(row[nField++]);
            strcpy(pRole[n].m_szSignature, row[nField++]);

            pRole[n].m_nHot = strtoul(row[nField++], &pStrStop, 10);		//人气
            pRole[n].m_nPre = strtoul(row[nField++], &pStrStop, 10);		//声望
            pRole[n].m_nInt = strtoul(row[nField++], &pStrStop, 10);		//智力
            pRole[n].m_nAPt = strtoul(row[nField++], &pStrStop, 10);		//

            pRole[n].m_nMoney = strtoul(row[nField++], &pStrStop, 10);	//金钱
            pRole[n].m_nExp = strtoul(row[nField++], &pStrStop, 10);	//经验
            pRole[n].m_nLevel = (unsigned short)atoi(row[nField++]);	//等级

            pRole[n].m_nFamilyId = strtoul(row[nField++], &pStrStop, 10);	//公会ID
            pRole[n].m_nPriv = strtoul(row[nField++], &pStrStop, 10);		//角色权限

            for (unsigned char k = 0; k < 4; k++)
            {
                pRole[n].m_anBeiBaoKuozhan[k] = (unsigned char)atoi(row[nField++]);
            }
            pRole[n].m_nBadgeGridNum = (unsigned char)atoi(row[nField++]);

            pRole[n].m_nRoleCreateTime = strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nLastLoginTime = strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nTimeOnline = strtoul(row[nField++], &pStrStop, 10);

#ifndef SERVER_IOS
            pRole[n].m_nAddictionTime = strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nAddictionOfflineTime = strtoul(row[nField++], &pStrStop, 10);
#endif //SERVER_IOS

            pRole[n].m_nQuestDone = (unsigned short)atoi(row[nField++]);
            pRole[n].m_nDanceGroupID = strtoul(row[nField++], &pStrStop, 10);		//舞团编号
            strcpy(pRole[n].m_szInvitationCode, row[nField++]);
            pRole[n].m_nClothGroupId = (unsigned char)strtoul(row[nField++], &pStrStop, 10);		//服饰搭配id
            pRole[n].m_nDivorceAgreementID = (unsigned int)strtoul(row[nField++], &pStrStop, 10);	//离婚协议书ID

            pRole[n].bi.nPoint = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].bi.nBindPoint = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].bi.nChange = atoi(row[nField++]);
            pRole[n].bi.nTotalPoint = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].bi.nTotalBindPoint = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].bi.nTotalGift = atoi(row[nField++]);
            pRole[n].m_nLastLogoutTime = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nAccountCreateTime = (unsigned int)strtoul(row[nField++], &pStrStop, 10);
            pRole[n].m_nPID = (unsigned short)atoi(row[nField++]);
            n++;
        }
    }
    SafeFreeResult();


    for (int i = 0; i < n; i++)
    {
        if (nMode == MODE_RoleAll || (nMode & MODE_RoleEvent))
        {
            LoadEvent(pRole + i);
        }

        if (nMode == MODE_RoleAll || (nMode & MODE_RoleFriend))
        {
            LoadRoleFriend(pRole + i);
        }
        // VIP
        if (nMode == MODE_RoleAll || (nMode & MODE_RoleVIP))
        {
            LoadVIP(pRole + i);
        }
        if (nMode == MODE_RoleAll || (nMode & MODE_MoneyReset)) {
            LoadMoneyResetInfo(pRole + i);
        }
        if (nMode == MODE_RoleAll || (nMode & MODE_Mail)) {
            LoadRoleMail(pRole + i);
        }
        if (nMode == MODE_RoleAll || (nMode & MODE_Couple)) {
            LoadCoupleInfo(pRole + i);
            LoadDivorceAgreementInfo(pRole + i);
            LoadDivorceAction(pRole + i);
        }

        if (nMode == MODE_RoleAll || (nMode & MODE_Dungeon))
        {
            LoadFriendContactInfo(pRole + i);
        }

        if (MODE_RoleAll == nMode || (MODE_SpecialMall & nMode))
        {
            LoadLimitedEdition(pRole + i);
            LoadAuctionNotGiveInfo(pRole + i);
        }

        if (MODE_RoleAll == nMode || (MODE_LongactingRecharge & nMode))
        {
            LoadLongactingRecharge(pRole + i);
        }

        if (MODE_RoleAll == nMode)
        {
            //			if (PaymentInfoMgr::Instance().CanOpenFirstCharge(time(NULL)))
            LoadFixedChargeRewardAll(pRole->nAccount, pRole->m_fixedChargeRecord);
        }

        if (MODE_RoleAll == nMode)
        {
            LoadMedalMap(pRole[i].m_nRoleID, pRole[i].m_mapMedalUnit);
            LoadMedalScore(pRole[i].m_nRoleID, pRole[i].m_nMedalWearScore, pRole[i].m_nMedalCumulativeScore);
            LoadMedalCondition(pRole[i].m_nRoleID, pRole[i].m_mapMedalCondition);
        }
        
        if ( !pRole[i].IsFlagSet( ERoleInfoCacheQuestNew ) )
        {
            LoadRoleQuestNew( pRole + i );
        }

        if ( !pRole[i].IsFlagSet( ERoleInfoCacheItem ) )
        {
            LoadStorageCapacity( pRole + i );
            LoadRoleClothGroup( pRole + i );
            LoadRoleItem( pRole + i );
            LoadRoleCooldown( pRole + i );
            LoadRoleGene( pRole + i );
        }

        if ( !pRole[i].IsFlagSet( ERoleInfoCacheClothEffect ) )
        {
            LoadClothEffect( pRole + i );
            LoadRoleEffect( pRole + i );
            LoadHandbookProgress( pRole + i );
            LoadColorProgress( pRole + i );
        }

        if ( !pRole[i].IsFlagSet( ERoleInfoCacheSettings ) )
        {
            LoadPlayerSetting( pRole + i );
            LoadChekIn( pRole + i );
            LoadMonthCard( pRole + i );
        }

        if ( !pRole[i].IsFlagSet( ERoleInfoCacheActivity ) )
        {
            LoadSocialShare( pRole + i );
            LoadRolePuzzleOnLogin( pRole + i );
            LoadKeepOnlineRewardInfo( pRole + i );
            LoadGift( pRole->nAccount, pRole[i].m_mapGift );
        }

        if ( !pRole[i].IsFlagSet( ERoleInfoCacheOther ) )
        {
            LoadExperiencedProduct( pRole + i );
            LoadExperiencedTime( pRole + i );
            LoadMagicArrayRecord( pRole + i );
            LoadMagicArrayExchange( pRole + i );
            LoadHistoryFightResult( pRole );
            LoadTitle( pRole + i );
            LoadAchievement( pRole + i );
            InfoCenterLoadRoleReaded( pRole->m_nRoleID, pRole->m_vecInfoCenterReaded );
            LoadRoleActiveness( pRole[i].m_nRoleID, pRole[i].m_activeness );
            LoadDesiredItem( pRole + i );
            LoadPetsInfo( pRole[i].m_nRoleID, pRole[i].m_mapPet );
            LoadHandbookPcInfo( pRole[i].m_nRoleID, eHandbookType_Costume, pRole[i].m_MapCostumePersonalInfo );
            LoadHandbookPcInfo( pRole[i].m_nRoleID, eHandbookType_Effect, pRole[i].m_MapEffectPersonalInfo );
            LoadRoleCompensation( pRole + i );
            LoadMagicLamp( pRole + i );
            LoadMainlineParcloseList( pRole + i );
            LoadThemeParcloseList( pRole + i );
            LoadDungeonStarReward( pRole + i );
            LoadPlayerDebris( pRole + i );
            LoadBigMamaRoleInfo(pRole[i].m_nRoleID, pRole[i].m_bigMamaRoleInfo.m_nPrizeStamp, pRole[i].m_bigMamaRoleInfo.m_nJoinStamp);
        }

        WriteLog( LOGLEVEL_DEBUG, "[Cache]GetRoleInfo. Role = %u, cache flag string = %s", pRole[i].m_nRoleID, pRole[i].FlagsToString().c_str() );
    }

    return bRes ? (n > 0 ? Query_Res_OK : Query_Res_Empty) : Query_Res_Fail;
}

void CDBInterface::LoadItemInfoFromRow(DBItem* pItemInfo)
{
    int nField = 0;
    pItemInfo->m_nItemID = atoi64(row[nField++]);
    pItemInfo->m_nTypeID = (unsigned int)atoi(row[nField++]);
    pItemInfo->m_nColumn = (unsigned short)atoi(row[nField++]);
    pItemInfo->m_nIndex = (unsigned short)atoi(row[nField++]);
    pItemInfo->m_nCount = (unsigned short)atoi(row[nField++]);
    pItemInfo->m_nDynamicBoxRelatedValue = atoi(row[nField++]);
    pItemInfo->m_nUseTimes = (short)atoi(row[nField++]);
    pItemInfo->m_nDuration = atoi(row[nField++]);
    pItemInfo->m_bEquiped = atoi(row[nField++]) > 0 ? true : false;
}

int CDBInterface::LoadRoleItem(LPROLEINFO_DB pRole)
{
    bool bRes = true;

    pRole->listItem.clear();
    sprintf(sql, "SELECT itemid, typeid, col, idx, cnt, relatedvalue, usetimes, duration, isquiped FROM item WHERE roleid=%d and deletetime = 0", pRole->m_nRoleID);
    bRes = ExecSQL();
    if (bRes)
    {
        DBItem itemInfo;
        while ((row = FetchRow()) != NULL)
        {
            LoadItemInfoFromRow(&itemInfo);
            pRole->listItem.push_back(itemInfo);
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadClothEffect(LPROLEINFO_DB pRole)
{
    pRole->mapClothEffect.clear();
    sprintf(sql, "SELECT itemid, effectid FROM clotheffect WHERE roleid = %d", pRole->m_nRoleID);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            itemid_t nItemid = atoi64(row[nField++]);;
            unsigned int nEffectID = strtoul(row[nField++], &pStrStop, 10);

            pRole->mapClothEffect[nItemid] = nEffectID;
        }

        SafeFreeResult();
        return Query_Res_OK;
    }

    SafeFreeResult();
    return Query_Res_Fail;
}

int CDBInterface::LoadHandbookProgress(LPROLEINFO_DB pRole)
{
    bool bRes = true;

    pRole->mapHandbookProgress.clear();
    sprintf(sql, "SELECT suitid, clothpos, effectid from clotheffect_handbook_progress where roleid = %u", pRole->m_nRoleID);
    bRes = ExecSQL();
    if (bRes)
    {
        DBItem itemInfo;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned short nSuitID = (unsigned short)atoi(row[nField++]);
            unsigned char nClothType = (unsigned char)atoi( row[nField++] );
            std::string strEffectID = row[nField++];

            pRole->mapHandbookProgress[nSuitID].insert(make_pair(nClothType, strEffectID));
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadColorProgress(LPROLEINFO_DB pRole)
{
    bool bRes = true;

    pRole->mapColorProgress.clear();
    sprintf(sql, "SELECT suitid, color, clothpos from clotheffect_sevencolor_progress where roleid = %u", pRole->m_nRoleID);
    bRes = ExecSQL();
    if (bRes)
    {
        DBItem itemInfo;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned short nSuitID = (unsigned short)atoi(row[nField++]);
            unsigned char nColorType = (unsigned char)atoi(row[nField++]);
            std::string strClothPos = row[nField++];

            pRole->mapColorProgress[nSuitID].insert(make_pair(nColorType, strClothPos));
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadRoleEffect(LPROLEINFO_DB pRole)
{
    pRole->setRoleEffect.clear();
    sprintf(sql, "SELECT effectid FROM clotheffect_role WHERE roleid = %d", pRole->m_nRoleID);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nEffectID = strtoul(row[nField++], &pStrStop, 10);
            pRole->setRoleEffect.insert(nEffectID);
        }

        SafeFreeResult();
        return Query_Res_OK;
    }

    SafeFreeResult();
    return Query_Res_Fail;
}

int CDBInterface::InsertClothEffect(unsigned int nRoleID, itemid_t nItemID, unsigned int nEffectID)
{
    char* end = sql;
    strFmt = "INSERT INTO clotheffect set roleid = %u, itemid = %lld, effectid = %u";
    sprintf(end, strFmt.c_str(), nRoleID, nItemID, nEffectID);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateClothEffect(unsigned int nRoleID, itemid_t nItemID, unsigned int nEffectID)
{
    char* end = sql;
    strFmt = "UPDATE clotheffect SET effectid = %u WHERE itemid=%lld and roleid = %u";
    sprintf(end, strFmt.c_str(), nEffectID, nItemID, nRoleID);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::InsertRoleEffect(unsigned int nRoleID, unsigned int nEffectID)
{
    char* end = sql;
    strFmt = "INSERT INTO clotheffect_role set roleid = %u, effectid = %u";
    sprintf(end, strFmt.c_str(), nRoleID, nEffectID);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::InsertHandbookProgress(unsigned int nRoleID, ClothEffectHandbookDB * pHandbookProgress)
{
    if (pHandbookProgress == NULL)
    {
        return Query_Res_Fail;
    }

    char* end = sql;
    strFmt = "INSERT INTO clotheffect_handbook_progress set roleid = %u, suitid = %u, clothpos = %u, effectid = '%s'";
    sprintf(end, strFmt.c_str(), nRoleID, pHandbookProgress->m_nSuitID, pHandbookProgress->m_nParam1, pHandbookProgress->m_strParam2.c_str());
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateHandbookProgress(unsigned int nRoleID, ClothEffectHandbookDB * pHandbookProgress)
{
    if (pHandbookProgress == NULL)
    {
        return Query_Res_Fail;
    }

    char* end = sql;
    strFmt = "UPDATE clotheffect_handbook_progress SET effectid = '%s' WHERE roleid = %u and suitid = %u and clothpos = %u";
    sprintf(end, strFmt.c_str(), pHandbookProgress->m_strParam2.c_str(), nRoleID, pHandbookProgress->m_nSuitID, pHandbookProgress->m_nParam1);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateColorProgress(unsigned int nRoleID, ClothEffectHandbookDB * pColorProgress)
{
    if (pColorProgress == NULL)
    {
        return Query_Res_Fail;
    }

    char* end = sql;
    strFmt = "UPDATE clotheffect_sevencolor_progress SET clothpos = '%s' WHERE roleid = %u and suitid = %u and color = %u";
    sprintf(end, strFmt.c_str(), pColorProgress->m_strParam2.c_str(), nRoleID, pColorProgress->m_nSuitID, pColorProgress->m_nParam1);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::InsertColorProgress(unsigned int nRoleID, ClothEffectHandbookDB * pColorProgress)
{
    if (pColorProgress == NULL)
    {
        return Query_Res_Fail;
    }

    char* end = sql;
    strFmt = "INSERT INTO clotheffect_sevencolor_progress set roleid = %u, suitid = %u, color = %u, clothpos = '%s'";
    sprintf(end, strFmt.c_str(), nRoleID, pColorProgress->m_nSuitID, pColorProgress->m_nParam1, pColorProgress->m_strParam2.c_str());
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::InsertStorageCapacity(unsigned int nRoleID, const StorageCapacityRecord & record)
{
    char* end = sql;
    strFmt = "INSERT INTO storage_capacity set roleid = %u, viplevel = %d, buycount = %d";
    sprintf(end, strFmt.c_str(), nRoleID, record.m_nVipLevel, record.m_nAddCapacity);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadStorageCapacity(LPROLEINFO_DB pRole)
{
    bool bRes = true;
    pRole->m_StorageCapacity = StorageCapacityRecord();
    sprintf(sql, "SELECT viplevel, buycount from storage_capacity where roleid = %u", pRole->m_nRoleID);
    bRes = ExecSQL();
    if (bRes)
    {
        DBItem itemInfo;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            StorageCapacityRecord record;
            record.m_nVipLevel = (int)atoi(row[nField++]);
            record.m_nAddCapacity = (int)atoi(row[nField++]);

            pRole->m_StorageCapacity = record;
        }
    }
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateStorageCapacity(unsigned int nRoleID, const StorageCapacityRecord & record)
{
    char* end = sql;
    strFmt = "UPDATE storage_capacity SET viplevel = %d,buycount = %d where roleid = %u";
    sprintf(end, strFmt.c_str(), record.m_nVipLevel, record.m_nAddCapacity, nRoleID);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::AddRoleItem(unsigned int nRoleID, DBItem* pItem)
{
    char* end = sql;
    strFmt = "INSERT INTO item set roleid = %u, itemid = %lld, typeid = %u, col = %d, idx = %d, cnt = %d, relatedvalue = %d, usetimes = %d, duration =%d, isquiped = %d, deletetime = %u";
    sprintf(end, strFmt.c_str(), nRoleID, pItem->m_nItemID, pItem->m_nTypeID, pItem->m_nColumn, pItem->m_nIndex, pItem->m_nCount, pItem->m_nDynamicBoxRelatedValue, pItem->m_nUseTimes, pItem->m_nDuration, pItem->m_bEquiped, 0);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleItem(unsigned int nRoleID, DBItem* pItem)
{
    char* end = sql;
    strFmt = "UPDATE item SET roleid=%u,typeid=%u,idx=%d,col=%d,cnt=%d,usetimes=%d,duration=%d,isquiped=%d WHERE itemid=%lld";
    sprintf(end, strFmt.c_str(), nRoleID, pItem->m_nTypeID, pItem->m_nIndex, pItem->m_nColumn, pItem->m_nCount, pItem->m_nUseTimes, pItem->m_nDuration, pItem->m_bEquiped ? 1 : 0, pItem->m_nItemID);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::RemoveRoleItem(__int64 nItemID, unsigned int nDeleteTime)
{
    // 备份到删除表
    sprintf(sql, "INSERT INTO item_delete(roleid,itemid,typeid,col,idx,cnt,relatedvalue,usetimes,duration,isquiped,deletetime)"
        "SELECT roleid,itemid,typeid,col,idx,cnt,relatedvalue,usetimes,duration,isquiped, %u FROM item where itemid = %lld",
        nDeleteTime, nItemID);
    ExecSQL();

    sprintf(sql, "delete from item  where itemid = %lld", nItemID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::RemoveRoleItemByType(unsigned int nRoleID, unsigned int nTypeID, int nCol)
{
    // 备份到删除表
    sprintf(sql, "INSERT INTO item_delete(roleid,itemid,typeid,col,idx,cnt,relatedvalue,usetimes,duration,isquiped,deletetime)"
        "SELECT roleid,itemid,typeid,col,idx,cnt,relatedvalue,usetimes,duration,isquiped, %u FROM item where roleid = %u AND typeid = %u AND col = %d",
        (unsigned int)time(NULL), nRoleID, nTypeID, nCol);
    ExecSQL();

    sprintf(sql, "delete from item WHERE roleid=%u AND typeid = %u AND col = %d", nRoleID, nTypeID, nCol);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::RemoveRoleDynamicBoxRelated(unsigned int nRoleID, __int64 nitemID)
{
    sprintf(sql, "update item_dynamicbox set usetime = %d where itemid = %lld and roleid = %u", (unsigned int)time(NULL), nitemID, nRoleID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateItemCount(unsigned short nCount, __int64 nItemID)
{
    strFmt = "UPDATE item SET cnt=%d WHERE itemid=%lld";
    sprintf(sql, strFmt.c_str(), nCount, nItemID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateItemPosition(ITEMINFO_Position* pItem)
{
    strFmt = "UPDATE item SET idx=%d,col=%d WHERE itemid=%lld";
    sprintf(sql, strFmt.c_str(), pItem->nIndex, pItem->nColumn, pItem->nItemID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateItemPosition2(ITEMINFO_Position2* pItem)
{
    int nRes = UpdateItemPosition(pItem->aItemData);
    nRes = UpdateItemPosition(pItem->aItemData + 1);
    return nRes;
}

int CDBInterface::UpdateItemOwner(unsigned int nRoleID, __int64 nItemID)
{
    strFmt = "UPDATE item SET roleid=%u WHERE itemid=%lld";
    sprintf(sql, strFmt.c_str(), nRoleID, nItemID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::ActivateGift(unsigned int nRoleID, CActivateGiftInfo *pInfo)
{
    if (nRoleID == 0 || pInfo == NULL)
    {
        return Query_Res_Fail;
    }
    bool bRes = true;
    int nRes = Query_Res_Fail;

    if (pInfo->m_nGiftType != 0)
    {	// 领取过该类型礼包
        sprintf(sql, "SELECT roleid FROM giftcode WHERE roleid = %d AND gifttype = %u", nRoleID, pInfo->m_nGiftType);
        bRes = ExecSQL();
        if (bRes) {
            if ((row = FetchRow()) != NULL) {
                pInfo->m_nOwnRoleID = 1;
                return Query_Res_OK;
            }
        }
        else {
            return Query_Res_Fail;
        }
    }

    SafeFreeResult();
    char *end = sql;
    if (bRes) {	// 更新该激活码信息
        sprintf(end, "UPDATE giftcode SET roleid = %u, drawtime = %u WHERE code = '", nRoleID, (unsigned int)time(NULL));
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pInfo->m_strCode.c_str(), pInfo->m_strCode.size());
        strcat(end, "'");
        end += strlen(end);

        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddRoleClothGroup( unsigned int nRoleID, RoleClothGroup *pRoleClothGroup )
{
    if ( pRoleClothGroup == NULL || nRoleID == 0 )
        return Query_Res_Fail;

    std::string strAppend;
    Formatter formator( strAppend );

    for ( unsigned char nPos = eItemClothType_Hair; nPos <= eItemClothType_Suit; ++nPos )
    {
        formator << ", " << pRoleClothGroup->GetClothItemType( nPos );
    }

    sprintf( sql, "insert into clothgroup(roleid, groupid, groupname, hair, face, body, gloves, leg, cap, "
        "facial, shoulder, wing, lefthand, righthand, wrist, hip, socks, feet, skin, nothing, suit) "
        "values(%u, %u, '%s'%s)", nRoleID, pRoleClothGroup->GetGroupID(),
        CDBIMysql::EscapeString( pRoleClothGroup->GetGroupName() ).c_str(), strAppend.c_str() );

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleClothGroup( unsigned int nRoleID, RoleClothGroup* pRoleClothGroup, bool bUpdateItems )
{
    if ( pRoleClothGroup == NULL || nRoleID == 0 )
        return Query_Res_Fail;

    std::string strAppend;
    if ( bUpdateItems )
    {
        Formatter formator( strAppend );
        const int nMaxCount = 18;
        const char *szColumnName[nMaxCount] = { "hair", "face", "body", "gloves", "leg", "cap", "facial", "shoulder",
            "wing", "lefthand", "righthand", "wrist", "hip", "socks", "feet", "skin", "nothing", "suit" };
        for ( unsigned char nPos = eItemClothType_Hair; nPos <= eItemClothType_Suit && nPos < nMaxCount; ++nPos )
        {
            formator << ", " << szColumnName[nPos] << " = " << pRoleClothGroup->GetClothItemType( nPos );
        }
    }

    sprintf( sql, "update clothgroup set groupname = '%s'%s where roleid = %u and groupid = %u",
        CDBIMysql::EscapeString( pRoleClothGroup->GetGroupName() ).c_str(), strAppend.c_str(),
        nRoleID, pRoleClothGroup->GetGroupID() );

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadMagicArrayExchange(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_mapMagicArrayExchangeRecord = MagicArrayExchangeRecord();
    sprintf(sql, "SELECT groupid, refreshtime FROM magicarray_exchange WHERE roleid=%u", pRole->m_nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            MagicArrayExchangeRecord record;
            std::string strExchangeGroupID = row[nField++];
            Split(strExchangeGroupID, "|", ":", record.m_mapRefreshGroup);
            record.m_nRefreshTime = (unsigned int)atoi(row[nField++]);

            pRole->m_mapMagicArrayExchangeRecord = record;
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadMagicArrayRecord(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_mapMagicArrayRecord.clear();
    sprintf(sql, "SELECT accumulatedcount, magicarrayid, freerolltime, firstroll FROM magicarray_record WHERE roleid=%u", pRole->m_nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            MagicArrayRecord record;
            record.m_nAccumulatedCallCount = (unsigned int)atoi(row[nField++]);
            record.m_nMagicArrayID = (unsigned int)atoi(row[nField++]);
            record.m_nFreeCallTime = (unsigned int)atoi(row[nField++]);
            record.m_bHaveFirstCall = (unsigned int)atoi(row[nField++]) > 0 ? true : false;

            pRole->m_mapMagicArrayRecord[record.m_nMagicArrayID] = record;
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::InsertMagicArrayRecord(unsigned int nRoleID, const MagicArrayRecord & magic)
{
    sprintf(sql, "INSERT INTO magicarray_record set accumulatedcount = %u, magicarrayid = %u, freerolltime = %u, firstroll = %u, roleid=%u",
        magic.m_nAccumulatedCallCount, magic.m_nMagicArrayID, magic.m_nFreeCallTime, magic.m_bHaveFirstCall ? 1 : 0, nRoleID);

    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::InsertMagicArrayExchange(unsigned int nRoleID, const MagicArrayExchangeRecord & exchangeRoecord)
{
    std::string  strGroup;
    ConcatToString(exchangeRoecord.m_mapRefreshGroup, std::string("|"), std::string(":"), strGroup);

    sprintf(sql, "INSERT INTO magicarray_exchange set groupid = '%s', refreshtime = %u, roleid = %u", strGroup.c_str(), exchangeRoecord.m_nRefreshTime, nRoleID);
    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::UpdateMagicArrayExchange(unsigned int nRoleID, const MagicArrayExchangeRecord & exchangeRoecord)
{
    std::string  strGroup;
    ConcatToString(exchangeRoecord.m_mapRefreshGroup, std::string("|"), std::string(":"), strGroup);

    sprintf(sql, "UPDATE magicarray_exchange set  groupid = '%s', refreshtime = %u where roleid=%u", strGroup.c_str(), exchangeRoecord.m_nRefreshTime, nRoleID);

    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::UpdateMagicArrayRecord(unsigned int nRoleID, const MagicArrayRecord & magic)
{
    sprintf(sql, "UPDATE magicarray_record set accumulatedcount = %u, freerolltime = %u, firstroll = %u where roleid = %u and magicarrayid = %u", 
        magic.m_nAccumulatedCallCount, magic.m_nFreeCallTime, magic.m_bHaveFirstCall ? 1 : 0, nRoleID, magic.m_nMagicArrayID);

    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::GetActivateGiftInfo(CActivateGiftInfo *pGiftInfo)
{
    if (pGiftInfo == NULL) {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(end, "SELECT roleid, giftid, gifttype, itemid, itemcount, starttime, endtime, drawtime FROM giftcode WHERE code = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pGiftInfo->m_strCode.c_str(), pGiftInfo->m_strCode.size());
    strcat(end, "'");
    end += strlen(end);

    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            int nField = 0;
            pGiftInfo->m_nOwnRoleID = atoi(row[nField++]);
            pGiftInfo->m_nGiftID = atoi(row[nField++]);
            pGiftInfo->m_nGiftType = (unsigned int)atoi(row[nField++]);
            pGiftInfo->m_nItemID = strtoul(row[nField++], NULL, 10);
            pGiftInfo->m_nItemCount = atoi(row[nField++]);
            pGiftInfo->m_nStartTime = atoi(row[nField++]);
            pGiftInfo->m_nEndTime = atoi(row[nField++]);
            pGiftInfo->m_nDrawTime = atoi(row[nField]);

        }
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::UpdateCoolDown(unsigned int nRoleID, CoolDownData & CoolDown)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "call update_cooldown_data(%u, %u, %d)", nRoleID, CoolDown.m_nType, CoolDown.m_nDuration);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::SaveRoleCooldown(LPROLEINFO_DB pRole)
{
    bool bRes = true;
    sprintf(sql, "DELETE FROM cooldown WHERE roleid=%u", pRole->m_nRoleID);
    bRes = ExecSQL();
    SafeFreeResult();
    list< CoolDownData >::iterator it;
    for (it = pRole->listCoolDown.begin(); it != pRole->listCoolDown.end(); it++)
    {
        sprintf(sql, "INSERT INTO cooldown VALUES(%u,%d,%d)", pRole->m_nRoleID, it->m_nType, it->m_nDuration);
        bRes = ExecSQL();
        SafeFreeResult();
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadRoleCooldown(LPROLEINFO_DB pRole)
{
    pRole->listCoolDown.clear();
    sprintf(sql, "SELECT type, duration FROM cooldown WHERE roleid=%u", pRole->m_nRoleID);
    bool bRes = ExecSQL();
    int nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            CoolDownData cd;
            int nField = 0;
            cd.m_nType = (unsigned short)atoi(row[nField++]);
            cd.m_nDuration = atoi(row[nField++]);
            pRole->listCoolDown.push_back(cd);
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadRoleGene(LPROLEINFO_DB pRole)
{
    pRole->listGene.clear();
    sprintf(sql, "SELECT geneid, param1, param2, szparam FROM gene WHERE roleid=%u", pRole->m_nRoleID);
    bool bRes = ExecSQL();
    int nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            GeneData gene;
            int nField = 0;
            gene.nGeneId = (unsigned short)atoi(row[nField++]);
            gene.nParam1 = atoi(row[nField++]);
            gene.nParam2 = atoi(row[nField++]);
            strcpy(gene.szCmdParam, row[nField++]);
            pRole->listGene.push_back(gene);
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddRoleGene(unsigned int nRoleID, GeneData* pGeneData)
{
    char szSqlCmdParam[MAX_GENE_PARAM_LEN] = { 0 };
    CDBIMysql::EscapeString(szSqlCmdParam, pGeneData->szCmdParam, strlen(pGeneData->szCmdParam));
    sprintf(sql, "INSERT INTO gene VALUES(%u,%d,%d,%d,'%s')", nRoleID, pGeneData->nGeneId, pGeneData->nParam1, pGeneData->nParam2, szSqlCmdParam);

    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DeleteRoleGene(unsigned int nRoleID, GeneData* pGeneData)
{
    sprintf(sql, "delete FROM gene WHERE roleid=%u and geneid=%d", nRoleID, pGeneData->nGeneId);

    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleGene(unsigned int nRoleID, GeneData* pGeneData)
{
    sprintf(sql, "update gene set param1=%d,param2=%d where roleid=%u and geneid=%u", pGeneData->nParam1, pGeneData->nParam2, nRoleID, pGeneData->nGeneId);

    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateBagExpand(unsigned int nRoleID, char* BagExpand)
{
    sprintf(sql, "update role set beibaokuozhan0=%u,beibaokuozhan1=%u,beibaokuozhan2=%u,beibaokuozhan3=%u where roleid=%u ", BagExpand[0], BagExpand[1], BagExpand[2], BagExpand[3], nRoleID);

    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadRoleOfflineData(const std::set<unsigned int> & setRoleID, const std::map<unsigned int, RoleImageOfflineData*> & mapOfflineData)
{
    std::string strRoleID = "";
    ConcatToString(setRoleID, ",", strRoleID);

    WriteLog(LOGLEVEL_DEBUG, "Load offflinedata begin role count %d", (int)setRoleID.size());
   
    // 人物属性
    std::string strSQL;
    Formatter::Format( strSQL, "SELECT r.roleid, r.rolename,r.sex,r.color,r.level,v.level,v.expired_time FROM role r LEFT JOIN vip v ON v.roleid = r.roleid WHERE r.roleid in (%s)", strRoleID.c_str() );
    if (ExecSQL(strSQL.c_str(), strSQL.length()))
    {
        int nQueryCount = 0;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nRoleID = (unsigned int)atoi(row[nField++]);
            std::map<unsigned int, RoleImageOfflineData*>::const_iterator it = mapOfflineData.find(nRoleID);
            if (it != mapOfflineData.end())
            {
                it->second->m_nRoleID = nRoleID;
                it->second->m_strRoleName = row[nField++];
                it->second->m_nSex = (unsigned char)atoi(row[nField++]);
                it->second->m_nSkin = (unsigned char)atoi(row[nField++]);
                it->second->m_nLevel = (unsigned short)atoi(row[nField++]);
                it->second->m_nVIPLevel = (unsigned short)atoi(row[nField++]);
                it->second->m_bVIP = (atoi64(row[nField++]) > (long long)time(NULL) ? true : false);
                NameDeny::Instance().FilterInvalidText(it->second->m_strRoleName);

                nQueryCount++;
            }
        }

        WriteLog(LOGLEVEL_DEBUG, "Load offflinedata attr count %d", nQueryCount);
    }
    SafeFreeResult();

    // 服饰
    Formatter::Format( strSQL, "SELECT a.roleid, a.itemid, a.typeid, a.col, a.idx, a.cnt, a.usetimes, a.duration, a.isquiped FROM item a WHERE a.roleid in(%s) AND a.deletetime = 0 AND a.isquiped = 1", strRoleID.c_str() );
    //sprintf(sql, "SELECT a.roleid, a.itemid, a.typeid, a.col, a.idx, a.cnt, a.usetimes, a.duration, a.isquiped FROM item a WHERE a.roleid in(%s%s AND a.deletetime = 0 AND a.isquiped = 1", strRoleID.c_str(), ")");
    if (ExecSQL(strSQL.c_str(), strSQL.length()))
    {
        int nQueryCount = 0;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            DBItem item;
            unsigned int nRoleID = (unsigned int)atoi(row[nField++]);
            item.m_nItemID = atoi64(row[nField++]);
            item.m_nTypeID = (unsigned int)atoi(row[nField++]);
            item.m_nColumn = (unsigned short)atoi(row[nField++]);
            item.m_nIndex = (unsigned short)atoi(row[nField++]);
            item.m_nCount = (unsigned short)atoi(row[nField++]);
            item.m_nUseTimes = (short)atoi(row[nField++]);
            item.m_nDuration = atoi(row[nField++]);
            item.m_bEquiped = atoi(row[nField++]) > 0 ? true : false;

            if (item.m_bEquiped && item.m_nColumn == EItemColumn_ClothBag)
            {
                item.m_nColumn = EItemColumn_ClothOnPlayer;
            }

            std::map<unsigned int, RoleImageOfflineData*>::const_iterator it = mapOfflineData.find(nRoleID);
            if (it != mapOfflineData.end())
            {
                it->second->m_vecRoleItem.push_back(item);
                nQueryCount++;
            }
        }

        WriteLog(LOGLEVEL_DEBUG, "Load offflinedata cloth count %d", nQueryCount);
    }
    SafeFreeResult();

    // 宠物
    Formatter::Format( strSQL, "select roleid, petid, name, potential, level, exp, endurance, endurance_recovery_time, attributetype, qualitytype, evolutiontimes, "
        "pettypeid from pet where roleid in (%s) and state = %u", strRoleID.c_str(), (unsigned int)ePetState_Carrying );
//    sprintf(sql, "select roleid, petid, name, potential, level, exp, endurance, endurance_recovery_time, attributetype, qualitytype, evolutiontimes, "
//        "pettypeid from pet where roleid in (%s%s and state = %u", 
//        strRoleID.c_str(), ")", (unsigned int)ePetState_Carrying);

    if (ExecSQL(strSQL.c_str(), strSQL.length()))
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            std::map<unsigned int, RoleImageOfflineData*>::const_iterator it = mapOfflineData.find(nRoleID);
            if (it != mapOfflineData.end())
            {
                it->second->m_bHasCarriedPet = true;
                it->second->m_playerPet.m_nPetID = (unsigned int)strtoul(row[nField++], NULL, 10);
                it->second->m_playerPet.m_strName = row[nField++];
                it->second->m_playerPet.m_nPotentialID = (unsigned char)atoi(row[nField++]);
                it->second->m_playerPet.m_nLevel = (unsigned short)atoi(row[nField++]);
                it->second->m_playerPet.m_nExp = (unsigned int)strtoul(row[nField++], NULL, 10);
                it->second->m_playerPet.m_nEndurance = (unsigned short)atoi(row[nField++]);
                it->second->m_playerPet.m_nEnduranceRecoveryTime = (float)strtoul(row[nField++], NULL, 10);
                it->second->m_playerPet.m_nPetAttributeType = (unsigned char)atoi(row[nField++]);
                it->second->m_playerPet.m_nQualityType = (unsigned char)atoi(row[nField++]);
                it->second->m_playerPet.m_nEvolutionTimes = (unsigned char)atoi(row[nField++]);
                it->second->m_playerPet.m_nPetType = (unsigned short)atoi(row[nField++]);
                NameDeny::Instance().FilterInvalidText(it->second->m_playerPet.m_strName);
            }
        }
    }
    SafeFreeResult();

    // 衣服特效
    Formatter::Format( strSQL, "SELECT roleid, itemid, effectid from clotheffect WHERE roleid in (%s)", strRoleID.c_str() );
    //sprintf(sql, "SELECT roleid, itemid, effectid from clotheffect WHERE roleid in (%s%s", strRoleID.c_str(), ")");
    if (ExecSQL(strSQL.c_str(), strSQL.length()))
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            std::map<unsigned int, RoleImageOfflineData*>::const_iterator it = mapOfflineData.find(nRoleID);
            if (it != mapOfflineData.end())
            {
                itemid_t nItemID = atoi64(row[nField++]);
                unsigned int nEffectID = (unsigned int)atoi(row[nField++]);
                it->second->m_mapClothEffect[nItemID] = nEffectID;
            }
        }
    }
    SafeFreeResult();

    // 衣服特效七色彩虹
    Formatter::Format( strSQL, "SELECT roleID, suitid, color, clothpos from clotheffect_sevencolor_progress  WHERE roleid in (%s)", strRoleID.c_str() );
    //sprintf(sql, "SELECT roleID, suitid, color, clothpos from clotheffect_sevencolor_progress  WHERE roleid in (%s%s", strRoleID.c_str(), ")");
    if (ExecSQL(strSQL.c_str(), strSQL.length()))
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            std::map<unsigned int, RoleImageOfflineData*>::const_iterator it = mapOfflineData.find(nRoleID);
            if (it != mapOfflineData.end())
            {
                unsigned short nSuitID = (unsigned short)atoi(row[nField++]);
                unsigned short nColor = (unsigned short)atoi(row[nField++]);
                std::string strClothPos = row[nField++];

                std::set<int> setClothPos;
                Split(strClothPos, ":", setClothPos);

                std::set<EItemClothType> setClothPosConfig;
                ConfigManager::Instance().GetEffectConfigMgr().GetSuitClothPos(nSuitID, setClothPosConfig);

                if (setClothPos.size() == setClothPosConfig.size())
                {
                    it->second->m_mapClothEffectSevenColor[nSuitID].insert((ESevenColorType)nColor);
                }
            }
        }
    }
    SafeFreeResult();

    return true;
}

int CDBInterface::LoadRoleOfflineData(unsigned int nRoleID, RoleImageOfflineData * offLineData)
{
    sprintf(sql, "SELECT r.rolename,r.sex,r.color,r.level,v.level,v.expired_time FROM role r LEFT JOIN vip v ON v.roleid = r.roleid WHERE r.roleid = %u", nRoleID);
    bool bRes = ExecSQL();
    int nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        if (mysql_num_rows(result) > 0)
        {
            row = FetchRow();
            int nField = 0;
            offLineData->m_nRoleID = nRoleID;
            offLineData->m_strRoleName = row[nField++];
            offLineData->m_nSex = (unsigned char)atoi(row[nField++]);
            offLineData->m_nSkin = (unsigned char)atoi(row[nField++]);
            if (row[nField] != NULL)
            {
                offLineData->m_nLevel = (unsigned short)atoi(row[nField]);
            }
            ++nField;
            if (row[nField] != NULL)
            {
                offLineData->m_nVIPLevel = (unsigned short)atoi(row[nField]);
            }
            ++nField;
            if (row[nField] != NULL)
            {
                offLineData->m_bVIP = (atoi64(row[nField]) > (long long)time(NULL) ? true : false);
            }

            NameDeny::Instance().FilterInvalidText(offLineData->m_strRoleName);
        }
    }
    SafeFreeResult();

    if (!bRes)
    {
        return nRes;
    }

    sprintf(sql, "SELECT a.itemid, a.typeid, a.col, a.idx, a.cnt, a.usetimes, a.duration, a.isquiped FROM item a WHERE  a.roleid = %u AND a.deletetime = 0 AND a.isquiped = 1", nRoleID);
    bRes = ExecSQL();
    nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            DBItem item;
            item.m_nItemID = atoi64(row[nField++]);
            item.m_nTypeID = (unsigned int)atoi(row[nField++]);
            item.m_nColumn = (unsigned short)atoi(row[nField++]);
            item.m_nIndex = (unsigned short)atoi(row[nField++]);
            item.m_nCount = (unsigned short)atoi(row[nField++]);
            item.m_nUseTimes = (short)atoi(row[nField++]);
            item.m_nDuration = atoi(row[nField++]);
            item.m_bEquiped = atoi(row[nField++]) > 0 ? true : false;

            if (item.m_bEquiped && item.m_nColumn == EItemColumn_ClothBag)
            {
                item.m_nColumn = EItemColumn_ClothOnPlayer;
            }

            offLineData->m_vecRoleItem.push_back(item);

            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    if (!bRes)
    {
        return nRes;
    }

    sprintf(sql, "select petid, name, potential, level, exp, endurance, "
        "endurance_recovery_time, attributetype, qualitytype, evolutiontimes, "
        "pettypeid from pet where roleid = %u and state = %u",
        nRoleID, (unsigned int)ePetState_Carrying);

    if (ExecSQL())
    {
        nRes = Query_Res_Empty;

        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            offLineData->m_bHasCarriedPet = true;
            offLineData->m_playerPet.m_nPetID = (unsigned int)strtoul(row[nField++], NULL, 10);
            offLineData->m_playerPet.m_strName = row[nField++];
            offLineData->m_playerPet.m_nPotentialID = (unsigned char)atoi(row[nField++]);
            offLineData->m_playerPet.m_nLevel = (unsigned short)atoi(row[nField++]);
            offLineData->m_playerPet.m_nExp = (unsigned int)strtoul(row[nField++], NULL, 10);
            offLineData->m_playerPet.m_nEndurance = (unsigned short)atoi(row[nField++]);
            offLineData->m_playerPet.m_nEnduranceRecoveryTime = (float)strtoul(row[nField++], NULL, 10);
            offLineData->m_playerPet.m_nPetAttributeType = (unsigned char)atoi(row[nField++]);
            offLineData->m_playerPet.m_nQualityType = (unsigned char)atoi(row[nField++]);
            offLineData->m_playerPet.m_nEvolutionTimes = (unsigned char)atoi(row[nField++]);
            offLineData->m_playerPet.m_nPetType = (unsigned short)atoi(row[nField++]);

            NameDeny::Instance().FilterInvalidText(offLineData->m_playerPet.m_strName);
        }
    }
    else
    {
        nRes = Query_Res_Fail;
    }

    // 衣服特效
    sprintf(sql, "SELECT itemid, effectid from clotheffect WHERE roleid = %u", nRoleID);
    bRes = ExecSQL();
    nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            itemid_t nItemID = atoi64(row[nField++]);
            unsigned int nEffectID = (unsigned int)atoi(row[nField++]);

            offLineData->m_mapClothEffect[nItemID] = nEffectID;
        }
    }

    // 衣服特效七色彩虹
    sprintf(sql, "SELECT suitid, color, clothpos from clotheffect_sevencolor_progress  WHERE roleid = %u", nRoleID);
    bRes = ExecSQL();
    nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned short nSuitID = (unsigned short)atoi(row[nField++]);
            unsigned short nColor = (unsigned short)atoi(row[nField++]);
            std::string strClothPos = row[nField++];

            std::set<int> setClothPos;
            Split(strClothPos, ":", setClothPos);

            std::set<EItemClothType> setClothPosConfig;
            ConfigManager::Instance().GetEffectConfigMgr().GetSuitClothPos(nSuitID, setClothPosConfig);

            if (setClothPos.size() == setClothPosConfig.size())
            {
                offLineData->m_mapClothEffectSevenColor[nSuitID].insert((ESevenColorType)nColor);
            }
        }
    }

    return nRes;
}

int CDBInterface::LoadCommands(list< CMDINFO > *pCmdList)
{
    time_t timeNow = time(NULL);
    sprintf(sql, "SELECT UNIX_TIMESTAMP()");
    bool bRes = ExecSQL();
    if (bRes)
    {
        if ((row = FetchRow()) != NULL)
        {
            timeNow = 3 + atoi(row[0]);
        }
    }
    SafeFreeResult();

    sprintf(sql, "SELECT server, cmdid, cmdtime, param1, param2 FROM commands WHERE cmdtime <= %d ORDER BY cmdtime", (int)timeNow);
    bRes = ExecSQL();
    int nRes = bRes ? Query_Res_OK : Query_Res_Fail;
    if (bRes)
    {
        CMDINFO info;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            info.nServer = strtoul(row[nField++], &pStrStop, 10);
            info.nCmdID = strtoul(row[nField++], &pStrStop, 10);
            info.timeCmd = strtoul(row[nField++], &pStrStop, 10);
            info.nCmdParam = strtoul(row[nField++], &pStrStop, 10);
            strcpy(info.szCmdParam, row[nField++]);
            pCmdList->push_back(info);
        }

        SafeFreeResult();

        sprintf(sql, "DELETE FROM commands WHERE cmdtime <= %d", (int)timeNow);
        ExecSQL();
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadMallInfo(list<MallGood*>* goodslist, int &nVersion)
{
    int nRes = Query_Res_Fail;
    if (goodslist == NULL)
        return nRes;

    sprintf(sql, "SELECT typeid,state,discount,isvip, isprecious, isnew, ishot, iscoopon, isunique, paytype, changepercent, price,price1,price2,price3,unix_timestamp(sell_time0),unix_timestamp(sell_time1),unix_timestamp(discount_time0),unix_timestamp(discount_time1),idxhot,desirenum FROM mall order by idx");
    if (ExecSQL()){
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL){

            MallGood* pGoodsInfo = new MallGood;
            int nField = 0;
            pGoodsInfo->m_nType = (unsigned int)atoi(row[nField++]);
            pGoodsInfo->m_nState = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nDiscount = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nIsVip = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nIsPrecious = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nIsNew = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nHot = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nIsCoupon = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nUnique = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nPayType = (unsigned char)atoi(row[nField++]);
            pGoodsInfo->m_nUseChangePercent = (unsigned char)atoi(row[nField++]);

            pGoodsInfo->m_nPrice = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nPrice1 = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nPrice2 = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nPrice3 = strtoul(row[nField++], &pStrStop, 10);

            pGoodsInfo->m_nSell_time0 = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nSell_time1 = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nDiscount_time0 = strtoul(row[nField++], &pStrStop, 10);
            pGoodsInfo->m_nDiscount_time1 = strtoul(row[nField++], &pStrStop, 10);

            pGoodsInfo->m_nHotIdx = (unsigned short)atoi(row[nField++]);

            pGoodsInfo->m_nDesireCount = strtoul(row[nField++], &pStrStop, 10);

            goodslist->push_back(pGoodsInfo);
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::SaveMallLog(MALL_LOG* log)
{
    sprintf(sql, "INSERT INTO mall_log(roleid,costpt,costbindpt,costchange,costmoney,acttime,destsendid,renewitemid) values (%u,%u,%u,%u,%u,now(),%u,%lld)",
        log->m_nRoleID, log->m_nCostPoint, log->m_nCostBindPoint, log->m_nCostChange, log->m_nCostMoney, log->m_nDestSendRoleID, log->m_nItemID);

    bool bRes = ExecSQL();
    __int64 nOrderID = (__int64)mysql_insert_id(&m_mysqlConnection);
    SafeFreeResult();

    if (nOrderID > 0 && bRes)
    {
        for (MALL_LOG::VecGoodsPayInfo::iterator iter = log->m_vecGoodsPayInfo.begin();
            iter != log->m_vecGoodsPayInfo.end(); ++iter)
        {
            sprintf(sql, "INSERT INTO detail(orderid,typeid,selindex,moneyUsed,mCoinUsed,bindMCoinUsed,acttime,destsendid)"
                "values (%lld,%d,%d,%u,%u,%u,now(),%u)",
                nOrderID, iter->m_itemType, iter->m_nSelType, iter->m_uMoney, iter->m_uMCoin, iter->m_uBindMCoin, log->m_nDestSendRoleID);
            bRes = ExecSQL();
            SafeFreeResult();

        }
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::SaveSingleItemLog(const std::vector<MallSigleItemLog> & malllog)
{
    std::vector<MallSigleItemLog>::const_iterator it = malllog.begin();
    for (; it != malllog.end(); ++it)
    {
        sprintf(sql, "INSERT INTO mall_singleitem_log(roleid,itemtype,paytype,bill,bindbill,money,time) values (%u,%u,%u,%u,%u,%u,%u)",
            it->nRoleID, it->nItemType, it->nPayType, it->nPoint, it->nBindPoint, it->nMoney, (unsigned int)time(NULL));

        ExecSQL();
        SafeFreeResult();
    }

    return Query_Res_OK;
}

int CDBInterface::SaveMallDesireNum(unsigned int nType, unsigned int nAddNum)
{
    sprintf(sql, "UPDATE mall SET desirenum = desirenum + %u WHERE typeid = %u", nAddNum, nType);

    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadExperiencedProduct(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (NULL == pRole)
        return nRes;

    pRole->m_vecExperiencedProducts.clear();
    sprintf(sql, "SELECT productID FROM experienced_product WHERE roleID=%u", pRole->m_nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            pRole->m_vecExperiencedProducts.push_back((itemtype_t)atoi(row[0]));
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::InsertExperiencedProduct(CExperiencedCardProduct &prod)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "INSERT INTO experienced_product(roleID, productID) values(%u, %u)", prod.m_uRoleID, prod.m_uProductID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadExperiencedTime(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (NULL == pRole)
        return nRes;

    pRole->m_experiencedTimes = 0;
    pRole->m_experiencedStamp = 0;
    sprintf(sql, "SELECT usedTimes, UNIX_TIMESTAMP(updateTime) FROM experienced_time WHERE roleID=%u", pRole->m_nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_Empty;
        while ((row = FetchRow()) != NULL)
        {
            pRole->m_experiencedTimes = ((unsigned short)atoi(row[0]));
            pRole->m_experiencedStamp = strtoul(row[1], &pStrStop, 10);
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::UpdateExperiencedTime(CExperiencedCardUseNum &useNum)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "INSERT INTO experienced_time(roleID, usedTimes, updateTime) VALUES(%u, %d, from_unixtime(%u))"
        " ON DUPLICATE KEY UPDATE usedTimes=%d, updateTime=from_unixtime(%u)",
        useNum.m_uRoleID, (int)useNum.m_uUseNum, useNum.m_timeUpdate, (int)useNum.m_uUseNum, useNum.m_timeUpdate);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::AddRoleQuest(unsigned int nRoleID, QUESTINFO* pQuest)
{
    sprintf(sql, "INSERT INTO quest(roleid, questid, ttime, state) VALUES(%u, %d, %u, %d)", nRoleID, pQuest->m_nQuestID, pQuest->m_nTime, pQuest->m_nState);

    bool bRes = ExecSQL();
    SafeFreeResult();

    for (map<unsigned char, list<TargetInfo> >::iterator it = pQuest->m_mapTarget.begin(); it != pQuest->m_mapTarget.end(); it++)
    {
        for (list<TargetInfo>::iterator itTarget = it->second.begin(); itTarget != it->second.end(); itTarget++)
        {
            AddQuestTarget(nRoleID, pQuest->m_nQuestID, *itTarget);
        }
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::AddQuestTarget(unsigned int nRoleID, unsigned short nQuestID, TargetInfo &target)
{
    sprintf(sql, "INSERT INTO target(roleid, questid, type, idx, count) VALUES(%u, %d, %d, %d, %d)", nRoleID, nQuestID, target.m_nType, target.m_nIndex, target.m_nCount);
    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateQuestInfo(unsigned int nRoleID, QUESTINFO* pQuest)
{
    if (pQuest == NULL)
    {
        return Query_Res_Fail;
    }

    sprintf(sql, "UPDATE quest SET ttime = %u, state = %d WHERE roleid = %u AND questid = %d ", pQuest->m_nTime, pQuest->m_nState, nRoleID, pQuest->m_nQuestID);

    bool bRes = ExecSQL();

    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;

}

int CDBInterface::UpdateTargetInfo(unsigned int nRoleID, unsigned short nQuestID, map<unsigned char, list<TargetInfo> > &mapTarget)
{
    map< unsigned char, list<TargetInfo > >::iterator it;
    it = mapTarget.begin();
    for (; it != mapTarget.end(); it++)
    {
        list<TargetInfo>::iterator itTarget;
        itTarget = it->second.begin();
        for (; itTarget != it->second.end(); ++itTarget)
        {
            UpdateQuestTargetEntry(nRoleID, nQuestID, &(*itTarget));
        }
    }

    return Query_Res_OK;
}

int CDBInterface::UpdateRoleQuest(unsigned int nRoleID, QUESTINFO* pQuest)
{
    if (pQuest == NULL)
    {
        return Query_Res_Fail;
    }

    UpdateQuestInfo(nRoleID, pQuest);

    UpdateTargetInfo(nRoleID, pQuest->m_nQuestID, pQuest->m_mapTarget);

    return Query_Res_OK;
}

int CDBInterface::RemoveRoleQuest(unsigned int nRoleID, unsigned short nQuestID)
{
    sprintf(sql, "DELETE FROM quest WHERE roleid=%u AND questid=%d", nRoleID, nQuestID);
    bool bRes = ExecSQL();
    SafeFreeResult();

    if (bRes)
    {
        sprintf(sql, "DELETE FROM target WHERE roleid = %u AND questid = %d", nRoleID, nQuestID);
        bRes = ExecSQL();
        SafeFreeResult();		// 如果此处失败
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::AddRoleQuestHistory(unsigned int nRoleID, short nIndex, __int64 nRecord)
{
    sprintf(sql, "INSERT INTO questhistory VALUES(%u,%d,%lld)", nRoleID, nIndex, nRecord);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateRoleQuestHistory(unsigned int nRoleID, short nIndex, __int64 nRecord)
{
    sprintf(sql, "UPDATE questhistory SET record =%lld WHERE roleid = %d AND idx = %d", nRecord, nRoleID, nIndex);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::RemoveRoleQuestHistory(unsigned int nRoleID, unsigned short nQuestID)
{
    sprintf(sql, "DELETE FROM questhistory WHERE roleid=%u AND questid=%d", nRoleID, nQuestID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::AddRepeatedQuestDone(unsigned int nRoleID, CRepeatedHistoryQuest & info)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO repeated_quest_done(roleid, questid, ttime, cnt)  VALUE(%u, %u, %u, %u)", nRoleID, info.m_nQuestID, info.m_nTime, info.m_nCount);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateRepeatedQuestDone(unsigned nRoleID, CRepeatedHistoryQuest & info)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "UPDATE repeated_quest_done SET ttime = %u, cnt = %u WHERE roleid = %u AND questid = %u", info.m_nTime, info.m_nCount, nRoleID, info.m_nQuestID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }

    return nRes;
}

void CDBInterface::LoadQuestInfoFromRow(LPQUESTINFO pQuest)
{
    int nField = 0;
    pQuest->m_nQuestID = (unsigned short)atoi(row[nField++]);
    pQuest->m_nTime = strtoul(row[nField++], &pStrStop, 10);
    pQuest->m_nState = (unsigned char)atoi(row[nField]);
}
/*
int CDBInterface::LoadRoleQuest(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    std::map< int, QUESTINFO* > mapQuest;
    sprintf(sql, "SELECT questid, ttime, state FROM quest WHERE roleid=%u ORDER BY ttime", pRole->m_nRoleID);
    if (ExecSQL())
    {
        QUESTINFO info;
        while ((row = FetchRow()) != NULL)
        {
            LoadQuestInfoFromRow(&info);
            pRole->listQuest.push_back(info);
            mapQuest[info.m_nQuestID] = &(pRole->listQuest.back());
        }
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    if (nRes == Query_Res_OK) {

        nRes = Query_Res_Fail;
        sprintf(sql, "SELECT questid,type, idx, count FROM target WHERE roleid = %u", pRole->m_nRoleID);
        if (ExecSQL())
        {
            int nField = 0;
            unsigned short nQuestID;
            TargetInfo info;
            std::map< int, QUESTINFO* >::iterator it;
            while ((row = FetchRow()) != NULL)
            {
                nField = 0;
                nQuestID = (unsigned short)atoi(row[nField++]);
                info.m_nType = (unsigned char)atoi(row[nField++]);
                info.m_nIndex = (unsigned short)atoi(row[nField++]);
                info.m_nCount = (unsigned short)atoi(row[nField]);

                it = mapQuest.find(nQuestID);
                if (it != mapQuest.end())
                {
                    InsertTargetInto(it->second, &info);
                }
            }
            nRes = Query_Res_OK;
        }
    }

    if (nRes == Query_Res_OK){
        nRes = LoadQuestHistory(pRole->m_nRoleID, pRole->mapQuestHistory);
    }
    if (nRes == Query_Res_OK){
        nRes = LoadRepeatedQuest(pRole->m_nRoleID, pRole->listRepeatedQuest);
    }

    return nRes;
}

int CDBInterface::LoadRepeatedQuest(unsigned int nRoleID, list<CRepeatedHistoryQuest> &listHistory)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT questid, ttime, cnt FROM repeated_quest_done WHERE roleid = %u", nRoleID);
    if (ExecSQL()){
        while ((row = FetchRow()) != NULL){
            int nField = 0;
            CRepeatedHistoryQuest questInfo;
            questInfo.m_nQuestID = (unsigned short)atoi(row[nField++]);
            questInfo.m_nTime = (unsigned int)atoi(row[nField++]);
            questInfo.m_nCount = (unsigned int)atoi(row[nField]);
            listHistory.push_back(questInfo);
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadQuestHistory(unsigned int nRoleID, map<short, __int64> & mapQuestHistory)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT idx, record FROM questhistory WHERE roleid = %u", nRoleID);

    if (ExecSQL()){
        while ((row = FetchRow()) != NULL){
            int nField = 0;
            short idx = atoi(row[nField++]);
            __int64 record = atoi64(row[nField]);
            mapQuestHistory.insert(make_pair(idx, record));
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}*/

/*int CDBInterface::LoadQuestTarget(QUESTINFO *pQuest, unsigned int nRoleID, unsigned short nQeustID)
{
if (pQuest == NULL)
{
return Query_Res_Fail;
}

sprintf(sql, "SELECT type, idx, count FROM target WHERE roleid = %u AND  questid = %d", nRoleID, nQeustID);
bool bRes = ExecSQL();

if (bRes)
{
while ((row = FetchRow()) != NULL)
{
int nField = 0;
TargetInfo info;
info.m_nType = (unsigned char)atoi(row[nField++]);
info.m_nIndex = (unsigned short)atoi(row[nField++]);
info.m_nCount = (unsigned short)atoi(row[nField]);
InsertTargetInto(pQuest, &info);
}
}
SafeFreeResult();

return bRes ? Query_Res_OK : Query_Res_Fail;
}*/

void CDBInterface::InsertTargetInto(QUESTINFO *pQuest, TargetInfo *pInfo)
{
    if (pQuest != NULL && pInfo != NULL)
    {
        map<unsigned char, list<TargetInfo> >::iterator it;
        it = pQuest->m_mapTarget.find(pInfo->m_nType);
        if (it != pQuest->m_mapTarget.end())
        {
            list<TargetInfo> *listTarget = &(it->second);
            listTarget->push_back(*pInfo);
        }
        else
        {
            list<TargetInfo> listTarget;
            listTarget.push_back(*pInfo);
            pQuest->m_mapTarget.insert(make_pair(pInfo->m_nType, listTarget));
        }
    }
}

int CDBInterface::UpdateQuestTargetEntry(unsigned int nRoleID, unsigned short nQuestID, TargetInfo *pTarget)
{
    if (pTarget == NULL || nQuestID == 0 || nRoleID == 0){
        return Query_Res_Fail;
    }
    sprintf(sql, "UPDATE target SET count = %d WHERE roleid = %u AND questid = %d AND type = %d AND idx = %d", pTarget->m_nCount, nRoleID, nQuestID, pTarget->m_nType, pTarget->m_nIndex);
    bool bRes = ExecSQL();

    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

bool CDBInterface::IsValidCheckPacketType(unsigned char nPacketType)
{
    if (nPacketType == 3 || nPacketType == 5 || nPacketType == 7)
    {
        return true;
    }

    return false;
}

int CDBInterface::GetBeAppliedPlayerInfo(FriendInfo & beAppliedRole)
{
    int nRes = Query_Res_Fail;

    if (beAppliedRole.m_nRoleID > 0)
    {
        sprintf(sql, "select sex, rolename, level from role where roleid = %u", beAppliedRole.m_nRoleID);

        if (ExecSQL() && (row = FetchRow()) != NULL)
        {
            int nField = 0;

            beAppliedRole.m_nSex = (unsigned char)atoi(row[nField++]);
            beAppliedRole.SetName(row[nField++]);
            beAppliedRole.m_nLevel = (unsigned short)atoi(row[nField]);

            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::AddBlackListFriend(unsigned int nRoleID, FriendInfo & friendInfo)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0 && friendInfo.m_nRoleID > 0) {
        sprintf(sql, "select roleid from role where roleid = %u", friendInfo.m_nRoleID);// check whether player exists
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                SafeFreeResult();
                sprintf(sql, "call AddBlacklistFriend(%u, %u)", nRoleID, friendInfo.m_nRoleID);
                if (ExecSQL()) {
                    nRes = Query_Res_OK;
                }
            }
            SafeFreeResult();
        }
    }

    return nRes;
}

int CDBInterface::AddRoleFriend(unsigned int nRoleID, unsigned int nFriendID, unsigned int nType)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0 && nFriendID > 0)
    {
        sprintf(sql, "INSERT INTO friend(roleid,friendid,friendtype) VALUES(%u,%u,%u)", nRoleID, nFriendID, nType);
        char* end = sql + strlen(sql);
        if (ExecSQL(sql, (unsigned int)(end - sql)))
            nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::RemoveFriend(unsigned int nRoleID, unsigned int nFriendID)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0 && nFriendID > 0)
    {
        sprintf(sql, "call removefriend(%u, %u)", nRoleID, nFriendID);
        if (ExecSQL())
        {
            SafeFreeResult();
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::AddBlackList(unsigned int nRoleID, FriendInfo &friendInfo)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0 && friendInfo.m_nRoleID > 0)
    {
        sprintf(sql, "select roleid from role where roleid = %u", friendInfo.m_nRoleID);
        if (!ExecSQL() || (row = FetchRow()) == NULL)
        {
            return nRes;
        }

        sprintf(sql, "call addblacklist(%u, %u, %d)", nRoleID, friendInfo.m_nRoleID, friendInfo.m_nRelationType);
        if (!ExecSQL())
        {
            return nRes;
        }

        sprintf(sql, "select rolename, sex, level from role where roleid = %u", friendInfo.m_nRoleID);
        if (!ExecSQL() || (row = FetchRow()) == NULL)
        {
            return nRes;
        }

        int nIndex = 0;
        friendInfo.SetName(row[nIndex++]);
        friendInfo.m_nSex = (unsigned char)atoi(row[nIndex++]);
        friendInfo.m_nLevel = (unsigned short)atoi(row[nIndex]);

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::RemoveBlackList(unsigned int nRoleID, unsigned int nFriendID)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0 && nFriendID > 0)
    {
        sprintf(sql, "call removeblacklist(%u, %u)", nRoleID, nFriendID);
        if (ExecSQL())
            nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}


int CDBInterface::_BecomeFriend(unsigned int selfRoleID, unsigned int nFriendID, int nRelationType, int nCallBy, int nCallTo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select roleid from role where roleid = %u", nFriendID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            sprintf(sql, "call becomefriend(%u, %u, %d,  %d, %d)", selfRoleID, nFriendID, nRelationType, nCallBy, nCallTo);
            if (ExecSQL())
                nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::BeceomFriend(unsigned int nSelfID, unsigned int nFriendID, FriendInfo *pFriendInfo)
{
    if (pFriendInfo == NULL)
    {
        return Query_Res_Fail;
    }

    int nRes = _BecomeFriend(nSelfID, nFriendID, pFriendInfo->m_nRelationType, pFriendInfo->m_nCallBy, pFriendInfo->m_nCallTo);

    if (nRes == Query_Res_OK)
    {
        sprintf(sql, "select rolename, sex, level from role where roleid = %u", nFriendID);

        if (ExecSQL() && (row = FetchRow()) != NULL)
        {
            int nField = 0;

            pFriendInfo->SetName(row[nField++]);
            pFriendInfo->m_nSex = (unsigned char)atoi(row[nField++]);
            pFriendInfo->m_nLevel = (unsigned short)atoi(row[nField]);
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }

    return nRes;
}

int CDBInterface::RemoveRoleFriend(unsigned int nRoleID, unsigned int nFriendID)
{
    if (nRoleID > 0 && nFriendID > 0)
    {
        sprintf(sql, "DELETE FROM friend WHERE roleid=%u AND friendid=%u", nRoleID, nFriendID);
        bool bRes = ExecSQL();
        SafeFreeResult();
        return bRes ? Query_Res_OK : Query_Res_Fail;
    }

    return Query_Res_Fail;
}

int CDBInterface::UpdateRoleFriend(unsigned int nRoleID, unsigned int nFriendID, unsigned char nFriendType, int nFriendCalling)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0 && nFriendID > 0)
    {
        sprintf(sql, "UPDATE friend SET friendtype = %u, friendcalling = %d WHERE roleid = %u AND friendid = %u", nFriendType, nRoleID, nFriendID, nFriendCalling);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}


/*int CDBInterface::LoadFriendInfo(list<FriendInfo>& listFriendInfo)
{
int nRes = Query_Res_Fail;

for (list<FriendInfo>::iterator itr = listFriendInfo.begin(); itr != listFriendInfo.end(); itr++) {
sprintf(sql, "SELECT rolename, sex, level FROM role WHERE roleid = %u",itr->m_nRoleID);
if (ExecSQL()) {
while ((row = FetchRow()) != NULL) {
int nField = 0;
itr->m_strName = row[nField++];
itr->m_nSex = (unsigned char)atoi(row[nField++]);
itr->m_nLevel = (unsigned short)atoi(row[nField]);
}
}

SafeFreeResult();
sprintf(sql, "select level, expired_time from vip where roleid = %u", itr->m_nRoleID);
if (ExecSQL()) {
if ((row = FetchRow()) != NULL) {
int nField = 0;
itr->m_nVIPLevel = atoi(row[nField++]);
itr->m_bIsVIP = atoi64(row[nField]) > time(NULL);
}
nRes = Query_Res_OK;
}
SafeFreeResult();
}

return nRes;
}*/

void CDBInterface::LoadRoleFriend(LPROLEINFO_DB pRole)
{
    sprintf(sql, "SELECT a.blacklist,a.friendid,a.callby,a.callto,a.relationtype,a.notes,b.rolename, b.sex, b.level,c.level, c.expired_time FROM friend a LEFT JOIN vip c on c.roleid = a.friendid , role b WHERE a.roleid=%u and b.roleid = a.friendid", pRole->m_nRoleID);	// friend

    if (ExecSQL())
    {
        std::string strName;
        while ((row = FetchRow()) != NULL)
        {
            FriendInfo info;

            int nField = 0;
            int blacklist = atoi(row[nField++]);
            info.m_nRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            info.m_nCallBy = (unsigned char)atoi(row[nField++]);
            info.m_nCallTo = (unsigned char)atoi(row[nField++]);
            info.m_nRelationType = (unsigned char)atoi(row[nField++]);
            info.m_strNotes = row[nField++];
            strName = row[nField++];
            info.m_nSex = (unsigned char)atoi(row[nField++]);
            info.m_nLevel = (unsigned char)atoi(row[nField++]);
            if (row[nField] != NULL)
            {
                info.m_nVIPLevel = atoi(row[nField++]);
                info.m_bIsVIP = atoi64(row[nField++]) > (long long)time(NULL);
            }

            NameDeny::Instance().FilterInvalidText(strName);
            info.SetName(strName);

            if (blacklist == 0)
            {
                pRole->listFriend.push_back(info);
            }
            else
            {
                pRole->listBlackList.push_back(info);
            }
        }
    }

    SafeFreeResult();
}


int CDBInterface::LoadRoleMail(unsigned int nRoleID, mapXQMail * pXQMapIn)
{
    int nRes = Query_Res_Fail;
    if (pXQMapIn == NULL || nRoleID == 0)
        return nRes;

    //只读取15天之内的邮件
    unsigned int nBeginTime = (unsigned int)time(NULL) - 15 * 24 * 60 * 60;

    //收件箱
    sprintf(sql,
        "SELECT a.mailid, a.senderid, a.sendername, a.title, a.content, "
        "       a.money, a.bindmcoin, a.sendtime, a.readtime, a.appendixtag, a.type, "
        "       b.typeid, b.cnt, b.relatedvalue, b.validtime, b.state  "
        "  FROM mail a LEFT JOIN mailitem b ON b.mailid = a.mailid "
        " WHERE a.sendtime >= %u AND a.receiverid = %u AND a.deletetime = 0 "
        " ORDER BY sendtime DESC", // sendtime是mail表分区字段
        nBeginTime, nRoleID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            MailInfo* pInfo = new MailInfo();
            if (pInfo != NULL)
            {
                int nField = 0;

                pInfo->m_nMailID = atoi64(row[nField++]);
                pInfo->m_nSenderID = strtoul(row[nField++], &pStrStop, 10);
                if (NULL != row[nField])
                    pInfo->SetSenderName(row[nField]);
                nField++;
                if (NULL != row[nField])
                    pInfo->SetTitle(row[nField]);
                nField++;
                if (NULL != row[nField])
                    pInfo->SetContent(row[nField]);
                nField++;
                pInfo->m_nMoney = atoi(row[nField++]);
                if (pInfo->m_nMoney < 0)
                {
                    pInfo->m_nMoney = 0;
                }
                else if (pInfo->m_nMoney > MAX_MONEY_NUM)
                {
                    pInfo->m_nMoney = MAX_MONEY_NUM;
                }
                pInfo->m_nBindMCoin = atoi( row[nField++] );
                if ( pInfo->m_nBindMCoin < 0 )
                {
                    pInfo->m_nBindMCoin = 0;
                }
                else if ( pInfo->m_nBindMCoin > MAX_MONEY_NUM )
                {
                    pInfo->m_nBindMCoin = MAX_MONEY_NUM;
                }
                pInfo->m_nSendTime = strtoul(row[nField++], &pStrStop, 10);
                pInfo->m_bRead = strtoul(row[nField++], &pStrStop, 10) > 0 ? 1 : 0;
                pInfo->m_nHasItem = (unsigned char)atoi(row[nField++]);
                pInfo->m_nType = (unsigned short)atoi(row[nField++]);

                if (row[nField] != NULL)
                {
                    MailItem aItem;
                    aItem.m_nType = (unsigned int)atoi(row[nField++]);
                    if (aItem.m_nType != 0)
                    {
                        aItem.m_nCount = (unsigned short)atoi(row[nField++]);
                        aItem.m_nDynamicBoxRelatedValue = atoi(row[nField++]);
                        aItem.m_nValidTime = atoi(row[nField++]);
                        aItem.m_nState = (unsigned char)atoi(row[nField++]);
                    }
                    pInfo->m_mapItems.insert(mapMailItems::value_type(aItem.m_nType, aItem));
                }
                pXQMapIn->Add(pInfo->m_nMailID, pInfo);
            }
        }
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadRoleMail(LPROLEINFO_DB pRole)
{
    LoadRoleMail(pRole->m_nRoleID, &(pRole->mapMailBox));

    return Query_Res_OK;
}

int CDBInterface::AddMail(MailInfo * pMail, unsigned int nReceiverID)
{
    int nRes = Query_Res_Fail;
    if (pMail == NULL)
        return nRes;

    char* end = sql;
    sprintf(end, "INSERT INTO mail SET receiverid=%u,senderid=%u,type=%u,money=%d,bindmcoin=%d,sendtime=%u,appendixtag=%d,sendername ='",
        nReceiverID, pMail->m_nSenderID, pMail->m_nType, pMail->m_nMoney, pMail->m_nBindMCoin, pMail->m_nSendTime, pMail->m_nHasItem);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pMail->GetSenderName().c_str(), pMail->GetSenderName().size());
    strcat(end, "',");
    end += strlen(end);

    sprintf(end, "title = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pMail->GetTitle().c_str(), pMail->GetTitle().size());
    strcat(end, "',");
    end += strlen(end);

    sprintf(end, "content = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pMail->GetContent().c_str(), pMail->GetContent().size());
    strcat(end, "'");
    end += strlen(end);

    bool bRes = ExecSQL();
    __int64 nMailID = (__int64)mysql_insert_id(&m_mysqlConnection);
    pMail->m_nMailID = nMailID;
    SafeFreeResult();

    if (pMail->m_nHasItem > 0 && nMailID > 0 && bRes)
    {
        mapMailItems::iterator it = pMail->m_mapItems.begin();
        while (it != pMail->m_mapItems.end() && bRes)
        {
            MailItem & aItem = (it->second);
            if (aItem.m_nType > 0 && aItem.m_nCount > 0)
            {
                sprintf(sql, "INSERT INTO mailitem(mailid,typeid,cnt,relatedvalue,validtime,state) values (%lld,%u,%d,%d,%d,%d)", nMailID, aItem.m_nType, aItem.m_nCount, aItem.m_nDynamicBoxRelatedValue, aItem.m_nValidTime, aItem.m_nState);
                bRes = ExecSQL();
                SafeFreeResult();
            }

            it++;
        }
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::GetMailMoney(__int64 nMailID)
{
    int nRes = Query_Res_Fail;
    if (nMailID <= 0)
        return nRes;


    sprintf(sql, "UPDATE mail SET money=0, bindmcoin = 0 WHERE mailid = %lld", nMailID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::GetAllMailItem(__int64 nMailID)
{
    int nRes = Query_Res_Fail;
    if (nMailID <= 0)
        return nRes;


    sprintf(sql, "UPDATE mailitem SET state=1 WHERE mailid = %lld", nMailID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }
    SafeFreeResult();


    sprintf(sql, "UPDATE mail SET appendixtag = 0 WHERE mailid = %lld", nMailID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::ReadMail(__int64 nMailID)
{
    int nRes = Query_Res_Fail;
    if (nMailID <= 0)
        return nRes;

    sprintf(sql, "UPDATE mail SET readtime=%u WHERE mailid = %lld", (unsigned int)time(NULL), nMailID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::DeleteMail(__int64 nMailID, unsigned int nDeleteTime)
{
    int nRes = Query_Res_Fail;
    if (nMailID <= 0)
    {
        return nRes;
    }

    sprintf(sql, "UPDATE mail SET deletetime=%u WHERE mailid = %lld", nDeleteTime, nMailID);
    if (ExecSQL()){
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::MailGetRoleID(MailInfoDB * pMail_db)
{
    bool bRes = false;
    if (pMail_db != NULL && strlen(pMail_db->m_szReceiverName) < MAX_NAME_LEN)
    {
        char* end = sql;
        sprintf(sql, "SELECT roleid FROM role WHERE rolename ='");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pMail_db->m_szReceiverName, strlen(pMail_db->m_szReceiverName));
        strcat(end, "'");
        end += strlen(end);

        bRes = ExecSQL();
        if (bRes){
            row = FetchRow();
            if (row != NULL){
                pMail_db->m_nReceiverID = strtoul(row[0], &pStrStop, 10);
            }
        }
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadMailStrangerSetting(std::vector<unsigned int> vecNotReceiveRole)
{
    vecNotReceiveRole.reserve(2048); // 
    Formatter::Format(sql, sizeof(sql), "SELECT role_id FROM recive_stranger_role");

    bool bRes = ExecSQL();
    if (!bRes)
        return Query_Res_Fail; // 查找失败

    while (NULL != (row = FetchRow()))
    {
        vecNotReceiveRole.push_back(atoi(row[0])); // 填充进来
    }

    return Query_Res_OK;
}

// true: 删除记录, false: 添加记录
int CDBInterface::UpdateMailStrangerSetting(unsigned int nRoleID, bool bReceive)
{
    bool bRes = false;
    if (bReceive)
    {
        Formatter::Format(sql, sizeof(sql), "INSERT INTO recive_stranger_role(role_id) VALUE(%u)", nRoleID); //
    }
    else
    {
        Formatter::Format(sql, sizeof(sql), "DELETE FROM recive_stranger_role WHERE role_id = %u", nRoleID);
    }
    bRes = ExecSQL();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}


int CDBInterface::AddTitle(unsigned int nRoleID, CTitleInfo * pTitle)
{
    if (pTitle == NULL || pTitle->m_nTitleID <= 0)
    {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO title VALUE(%u, %u, %u)", nRoleID, pTitle->m_nTitleID, pTitle->m_nGainTitleTime);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;

}

//void CDBInterface::LoadEvent(LPROLEINFO_DB pRole)
//{
//LoadCompleteTeachGuide(pRole->nRoleID, pRole->bCompleteTeachGuide);
//LoadCompleteProcGuide(pRole->nRoleID, pRole->bCompleteProcGuide);
//LoadFinishFirstPaymentInfo(pRole->nRoleID, pRole->bCompleteFirstPaymentTag, pRole->bCanSendFirstPaymentReward);
//}

//int CDBInterface::LoadCompleteTeachGuide(unsigned int nRoleID, bool &bCompleteTeachGuide)
//{
//	int nRes = Query_Res_Fail;
//
//	if ( nRoleID > 0 ) {
//		sprintf(sql, "select roleid from event where roleid = %u and eventid = %d", nRoleID, eEvent_CompleteTeachGuide );
//		if (ExecSQL()) {
//			nRes = Query_Res_OK;
//			row = FetchRow();
//			if (row != NULL) {
//				bCompleteTeachGuide = true;
//			}
//		}
//	}
//
//	SafeFreeResult();
//	return nRes;
//}

//int CDBInterface::LoadCompleteProcGuide(unsigned int nRoleID, bool &bCompleteProcGuide)
//{
//	int nRes = Query_Res_Fail;
//
//	if ( nRoleID > 0 ) {
//		sprintf(sql, "select roleid from event where roleid = %u and eventid = %d", nRoleID, eEvent_CompleteProcGuide );
//		if (ExecSQL()) {
//			nRes = Query_Res_OK;
//			row = FetchRow();
//			if (row != NULL) {
//				bCompleteProcGuide = true;
//			}
//		}
//	}
//
//	SafeFreeResult();
//	return nRes;
//}

//int CDBInterface::LoadFinishFirstPaymentInfo(unsigned int nRoleID, bool &bFristPaymentTag, bool &bCanSendFirstPaymentReward)
//{
//	int nRes = Query_Res_Fail;
//
//	if (nRoleID > 0) {
//		sprintf(sql, "select count from nsytroledb.event where roleid = %u and eventid = %d", nRoleID, eEvent_FinishFirstPayment);
//		if (ExecSQL()) {
//			row = FetchRow();
//			if (row != NULL) {
//				bFristPaymentTag = true;
//			}
//		}
//		SafeFreeResult();
//
//		sprintf(sql, "select roleid from nsytroledb.event where roleid = %u and eventid = %d", nRoleID, eEvent_SentFirstPaymentReward);
//		if (ExecSQL()) {
//			row = FetchRow();
//			if (row == NULL) {
//				bCanSendFirstPaymentReward = true;
//			}
//		}
//		SafeFreeResult();
//	}
//
//	return Query_Res_OK;
//}
int CDBInterface::LoadEventRecord(EventInfoDB& eve)
{
    int nRes = Query_Res_Fail;

    if (eve.m_nRoleID > 0)
    {
        sprintf(sql, "select count,updatetime from event where roleid = %u and eventid = %d", eve.m_nRoleID, (int)eve.m_nEventID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            row = FetchRow();
            if (row != NULL)
            {
                eve.m_nCount = atoi(row[0]);
                eve.m_nTime = atoi(row[1]);
            }
            else
            {
                nRes = Query_Res_Empty;
            }
        }
        SafeFreeResult();
    }
    return nRes;
}

int CDBInterface::LoadFixedChargeReward(unsigned int account, unsigned int amount, RechargeRewardInfo* pRechargeRewardInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT UNIX_TIMESTAMP(stamp) FROM fixed_charge_reward WHERE account = %u AND amount = %u", account, amount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        row = FetchRow();
        if (row != NULL)
        {
            pRechargeRewardInfo->m_fixedChargeRecord.m_nChargeTime = atoi(row[0]);
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateFixedChargeReward(unsigned int account, unsigned int amount, unsigned int stamp)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "INSERT INTO fixed_charge_reward(account, amount, stamp) VALUE(%u, %u, FROM_UNIXTIME(%u)) "
        "ON DUPLICATE KEY UPDATE stamp = FROM_UNIXTIME(%u)", account, amount, stamp, stamp);
    if (!ExecSQL())
    {
        WriteLog("%s exec error.", sql);
    }
    else
    {
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadFixedChargeRewardAll(unsigned int account, FixedChargeRecord &record)
{
    int nRes = Query_Res_Fail;
    record.m_nAccount = account;

    sprintf(sql, "SELECT amount, UNIX_TIMESTAMP(stamp) FROM fixed_charge_reward WHERE account = %u", account);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            FixedChargeRecordItem item;
            item.m_nMCoin = atoi(row[0]);
            item.m_nChargeTime = atoi(row[1]);

            record.vecRecords.push_back(item);
        }
    }

    SafeFreeResult();
    return nRes;
}


int CDBInterface::LoadFirstPaymentInfo(unsigned int nRoleID, int & nCount, int & rRechargeTime, int nVersionType)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "select count,updatetime from event where roleid = %u and eventid = %d", nRoleID, GetChargeEventID(nVersionType));
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            row = FetchRow();
            if (row != NULL)
            {
                nCount = atoi(row[0]);
                rRechargeTime = atoi(row[1]);
            }
            else
            {
                nRes = Query_Res_Empty;
            }
        }
        SafeFreeResult();
    }
    return nRes;
}

int CDBInterface::LoadFirstPaymentRewardInfo(unsigned int nRoleID, int & nCount, int & rRechargeTime, int nVersionType)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "select count,updatetime from event where roleid = %u and eventid = %d", nRoleID, GetChargeSendRewardEventID(nVersionType));
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            row = FetchRow();
            if (row != NULL)
            {
                nCount = atoi(row[0]);
                rRechargeTime = atoi(row[1]);
            }
            else
            {
                nRes = Query_Res_Empty;
            }
        }
        SafeFreeResult();
    }
    return nRes;
}

int CDBInterface::LoadFirstPaymentRewardInfoByAccountId(RechargeRewardInfo& info)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT roleid, rolename, sex, money FROM nsytroledb.role WHERE account = %u;", info.nAccountID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            nRes = Query_Res_OK;

            info.nRoleID = atoi(row[0]);
            info.strRoleName = row[1];
            info.m_nSex = (unsigned char)atoi(row[2]);
            info.m_nMoney = atoi(row[3]);

            info.m_firstChargeEvent.m_nRoleID = info.nRoleID;
            info.m_firstChargeEvent.m_nEventID = (unsigned char)GetChargeEventID(info.m_eVersionType);
            info.m_firstChargeRewardEvent.m_nRoleID = info.nRoleID;
            info.m_firstChargeRewardEvent.m_nEventID = (unsigned char)GetChargeSendRewardEventID(info.m_eVersionType);
        }
    }
    if (info.nRoleID > 0)
    {
        nRes = LoadEventRecord(info.m_firstChargeEvent);
        nRes = LoadEventRecord(info.m_firstChargeRewardEvent);
    }

    return nRes;
}

void CDBInterface::UpdateAchievementData(ROLEINFO_DB * pRole)
{
    UpdateHistoryClothCount(pRole->m_nRoleID, pRole->nHistoryClothNum);
    UpdateHistoryBadageCount(pRole->m_nRoleID, pRole->nHistoryBadageNum);
    UpdateHistoryTaikoComboMusicCountEntry(pRole->m_nRoleID, pRole->nHistoryTaikoComboMusicNum);
    UpdateRoleFightResult(pRole);

    UpdateHistoryBigSpeakersUseCountInSevenDays(pRole->m_nRoleID, pRole->bigSpeakersUseNumWeekly.m_nCount, pRole->bigSpeakersUseNumWeekly.m_nLastEventTime);
    UpdateHistorySmallSpeakersUseCountInSevenDays(pRole->m_nRoleID, pRole->smallSpeakersUseNumWeekly.m_nCount, pRole->smallSpeakersUseNumWeekly.m_nLastEventTime);
    UpdaetHistorySendFlowerCountInSevenDays(pRole->m_nRoleID, pRole->sendFlowersNumInSevenWeekly.m_nCount, pRole->sendFlowersNumInSevenWeekly.m_nLastEventTime);

    UpdateHistoryWishItemCountEntry(pRole->m_nRoleID, pRole->nHistoryWishItemNum);
    UpdateHistoryGainWishItemCountEntry(pRole->m_nRoleID, pRole->nGainWishItemNum);
    UpdateHistoryUseBouquetCountEntry(pRole->m_nRoleID, pRole->nHistoryUseBouquetNum);
    UpdateHistoryAutoMatchWithOppositeSexCountEntry(pRole->m_nRoleID, pRole->nAutoMatchWithOppositeSexNum);
}

int CDBInterface::AddAchievement(unsigned int nRoleID, CAchievementInfo *pAchievement)
{
    int nRes = Query_Res_Fail;

    if (pAchievement != NULL && pAchievement->m_nAchievementID > 0)
    {
        sprintf(sql, "INSERT INTO achievement(roleid, achievementid, ttime, state) VALUE(%u, %u, %u, %u)", nRoleID, pAchievement->m_nAchievementID, pAchievement->m_nCompletedTime, pAchievement->m_nState);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

void CDBInterface::UpdateRoleFightResult(ROLEINFO_DB * pRole)
{
    map<int, map<int, CFightResult > >::iterator itrMap = pRole->mapFightResult.begin();

    for (; itrMap != pRole->mapFightResult.end(); itrMap++)
    {
        map<int, CFightResult >::iterator itr = itrMap->second.begin();
        for (; itr != itrMap->second.end(); itr++)
        {
            UpdateFightResult(pRole->m_nRoleID, itr->second);
        }
    }
}


int CDBInterface::AddFightResult(unsigned int nRoleID, CFightResult &fightResult)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "INSERT INTO fightresult(roleid, mode, difficulty, result0, result1, result2, result3, result4, result5, result6, score, conperfectnum) VALUE(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", nRoleID, fightResult.m_nModeID, fightResult.m_nDifficulty,
            fightResult.m_nResult[0], fightResult.m_nResult[1], fightResult.m_nResult[2], fightResult.m_nResult[3], fightResult.m_nResult[4], fightResult.m_nResult[5], fightResult.m_nResult[6],
            fightResult.m_nHistoryTotalScore, fightResult.m_nConPerfectNum);

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::UpdateFightResult(unsigned int nRoleID, CFightResult & fightResult)	// 如果新加入了别的模式，则会自动先添加
{
    int nRes = Query_Res_Fail;
    // 	sprintf(sql, "UPDATE mailitem SET state=%d WHERE mailid = %lld AND typeid=%d",pItem->m_nState,nMailID,pItem->m_nType);

    sprintf(sql, "SELECT roleid FROM fightresult WHERE roleid = %u AND mode = %d AND difficulty = %u", nRoleID, fightResult.m_nModeID, fightResult.m_nDifficulty);
    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
            SafeFreeResult();

            sprintf(sql, "UPDATE fightresult  SET result0 = %u, result1 = %u, result2 = %u, result3 = %u, result4 = %u, result5 = %u, result6 = %u, score = %u, conperfectnum = %u where roleid = %u and mode = %d and difficulty = %d",
                fightResult.m_nResult[0], fightResult.m_nResult[1], fightResult.m_nResult[2], fightResult.m_nResult[3], fightResult.m_nResult[4], fightResult.m_nResult[5], fightResult.m_nResult[6],
                fightResult.m_nHistoryTotalScore, fightResult.m_nConPerfectNum, nRoleID, fightResult.m_nModeID, fightResult.m_nDifficulty);

            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
            SafeFreeResult();
        }
        else
        {
            SafeFreeResult();
            AddFightResult(nRoleID, fightResult);
            SafeFreeResult();
        }
    }

    return nRes;
}

int CDBInterface::AddEventEntry(unsigned int nRoleID, unsigned char nEventID, unsigned int nCount)
{
    int nRes = Query_Res_Fail;

    if (nRoleID <= 0)
//        if (nRoleID <= 0 || nCount < 0)
    {
        return nRes;
    }

    unsigned int uNow = (unsigned int)time(NULL);

    sprintf(sql, "INSERT INTO event(roleid, eventid, count,updatetime) VALUE(%u, %d, %u,%u)", nRoleID, nEventID, nCount, uNow);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateEventEntry(unsigned nRoleID, unsigned char nEventID, unsigned nCount)
{
//    if (nRoleID <= 0 || nCount < 0)
    if (nRoleID <= 0)
    {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;

    sprintf(sql, "call update_event(%u, %u, %u)", nRoleID, nEventID, nCount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateEventEntry4(unsigned nRoleID, unsigned char nEventID, unsigned nCount, unsigned int nTime)
{
//    if (nRoleID <= 0 || nCount < 0)
    if (nRoleID <= 0)
    {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;

    sprintf(sql, "call update_event4(%u, %u, %u, %u)", nRoleID, nEventID, nCount, nTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::InfoCenterLoadAllInfo(InfoCenterPtrMap &ptrMap)
{
    Formatter::Format(sql, sizeof(sql), "SELECT id, title, context, href, infogroup, hidepid, icon, infotype, updatetime FROM infocenter_info");

    if (!ExecSQL())
    {
        WriteLog(LOGLEVEL_ERROR, "%s execute failed", sql);
        return Query_Res_Fail;
    }

    while ((row = FetchRow()) != NULL)
    {
        InfoCenterItem *itemPtr = new InfoCenterItem;

        unsigned short seq = 0;

        itemPtr->m_id = atoi(row[seq++]);
        itemPtr->m_strTitle = row[seq++];
        itemPtr->m_strContext = row[seq++];
        itemPtr->m_strHref = row[seq++];
        itemPtr->m_group = (unsigned char)atoi(row[seq++]);
        itemPtr->m_strHidPid = row[seq++];
        itemPtr->m_icon = row[seq++];
        itemPtr->m_type = (unsigned char)atoi(row[seq++]);
        itemPtr->m_updateTime = atoi(row[seq++]);

        ptrMap[itemPtr->m_id] = itemPtr;
    }

    SafeFreeResult();

    return Query_Res_OK;
}

int CDBInterface::InfoCenterAddInfo(InfoCenterItem &item)
{
    char escape[4 * 1024] = { 0 };
    std::string strEscapeTitle;
    std::string strEscapeContext;
    std::string strEscapeHref;
    std::string strEscapeIcon;
    CDBIMysql::EscapeString(escape, item.m_strTitle.c_str(), item.m_strTitle.length());
    strEscapeTitle = escape;
    CDBIMysql::EscapeString(escape, item.m_strContext.c_str(), item.m_strContext.length());
    strEscapeContext = escape;
    CDBIMysql::EscapeString(escape, item.m_strHref.c_str(), item.m_strHref.length());
    strEscapeHref = escape;
    CDBIMysql::EscapeString(escape, item.m_icon.c_str(), item.m_icon.length());
    strEscapeIcon = escape;

    Formatter::Format(sql, sizeof(sql),
        "INSERT INTO infocenter_info(title, context, href, infogroup, hidepid, icon, infotype, updatetime) "
        "VALUE('%s', '%s', '%s', %d, '%s', '%s', %d, %u)",
        strEscapeTitle.c_str(), strEscapeContext.c_str(), strEscapeHref.c_str(), item.m_group,
        item.m_strHidPid.c_str(), strEscapeIcon.c_str(), item.m_type, item.m_updateTime);

    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);
    SafeFreeResult();

    Formatter::Format(sql, sizeof(sql), "SELECT LAST_INSERT_ID()");
    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);

    if ((row = FetchRow()) != NULL)
    {
        item.m_id = atoi(row[0]);
    }

    return Query_Res_OK;
}

int CDBInterface::InfoCenterUpdateInfo(InfoCenterItem &item)
{
    char escape[4 * 1024] = { 0 };
    std::string strEscapeTitle;
    std::string strEscapeContext;
    std::string strEscapeHref;
    std::string strEscapeIcon;
    CDBIMysql::EscapeString(escape, item.m_strTitle.c_str(), item.m_strTitle.length());
    strEscapeTitle = escape;
    CDBIMysql::EscapeString(escape, item.m_strContext.c_str(), item.m_strContext.length());
    strEscapeContext = escape;
    CDBIMysql::EscapeString(escape, item.m_strHref.c_str(), item.m_strHref.length());
    strEscapeHref = escape;
    CDBIMysql::EscapeString(escape, item.m_icon.c_str(), item.m_icon.length());
    strEscapeIcon = escape;

    Formatter::Format(sql, sizeof(sql),
        "UPDATE infocenter_info SET "
        "title = '%s', context = '%s', href = '%s', "
        "infogroup = %d, hidepid = '%s', icon = '%s', "
        "infotype = %d, updatetime = %d WHERE id = %u",
        strEscapeTitle.c_str(), strEscapeContext.c_str(), strEscapeHref.c_str(), item.m_group,
        item.m_strHidPid.c_str(), strEscapeIcon.c_str(), item.m_type, item.m_updateTime, item.m_id);

    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);
    SafeFreeResult();

    return Query_Res_OK;
}

int CDBInterface::InfoCenterRemoveInfo(unsigned int id)
{
    Formatter::Format(sql, sizeof(sql), "DELETE FROM infocenter_info WHERE id = %u", id);

    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);
    SafeFreeResult();

    return Query_Res_OK;
}

int CDBInterface::InfoCenterUpdateRoleReadInfo(unsigned int roleID, unsigned int infoID, unsigned int uptime)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO infocenter_readed(roleID, infoID, readtime) "
        "VALUE(%u, %u, %u) ON DUPLICATE KEY UPDATE readtime = %u", roleID, infoID, uptime, uptime);

    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);
    SafeFreeResult();

    return Query_Res_OK;
}

int CDBInterface::InfoCenterLoadRoleReaded(unsigned int roleID, std::vector<std::pair<unsigned int, unsigned int> > &vecReaded)
{
    vecReaded.clear();
    Formatter::Format(sql, sizeof(sql), "SELECT infoID, readtime FROM infocenter_readed WHERE roleID = %u", roleID);

    ENSURE_WITH_LOG_CMD(ExecSQL(), return Query_Res_Fail, "%s execute failed", sql);

    while ((row = FetchRow()) != NULL)
    {
        vecReaded.push_back(std::make_pair<unsigned int, unsigned int>(atoi(row[0]), atoi(row[1])));
    }

    SafeFreeResult();

    return Query_Res_OK;
}


int CDBInterface::UpdateVIPOnlineAnnounce(unsigned int nRoleID, unsigned int nTime)
{
    return UpdateEventEntry(nRoleID, eEvent_VIPOnlineAnnounce, nTime);
}

int CDBInterface::AddHistoryTaikoComboMusicCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryComboMusic, nCount);
}

int CDBInterface::UpdateHistoryTaikoComboMusicCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryComboMusic, nCount);
}

int CDBInterface::AddHistoryWishItemCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryWishItem, nCount);
}

int CDBInterface::UpdateHistoryWishItemCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryWishItem, nCount);
}

int CDBInterface::AddHistoryGainWishItemCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryGainWishItem, nCount);
}

int CDBInterface::UpdateHistoryGainWishItemCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryGainWishItem, nCount);
}

int CDBInterface::AddHistoryUseBouquetCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryUseBoquet, nCount);
}

int CDBInterface::UpdateHistoryUseBouquetCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryUseBoquet, nCount);
}

int CDBInterface::AddHistoryClothCount(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryCloth, nCount);
}

int CDBInterface::UpdateHistoryClothCount(unsigned nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryCloth, nCount);
}

int CDBInterface::AddHistoryAutoMatchWithOppositeSexCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryAutoMatchWithOppositeSex, nCount);
}

int CDBInterface::UpdateHistoryAutoMatchWithOppositeSexCountEntry(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryAutoMatchWithOppositeSex, nCount);
}

int CDBInterface::AddHistoryBadageCount(unsigned nRoleID, unsigned int nCount)
{
    return AddEventEntry(nRoleID, eEvent_HistoryBadage, nCount);
}

int CDBInterface::UpdateHistoryBadageCount(unsigned nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_HistoryBadage, nCount);
}


int CDBInterface::LoadEvent(ROLEINFO_DB *pRole)
{
    if (pRole != NULL)
    {
        sprintf(sql, "SELECT eventid,count,updatetime FROM event WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                unsigned short eventid = (unsigned short)atoi(row[0]);
                unsigned int nValue = (unsigned int)atoi(row[1]);
                unsigned int nUpdateTime = (unsigned int)atoi(row[2]);
                SConstellationCardInfo card;
                switch (eventid)
                {
                case eEvent_HistoryComboMusic:
                    pRole->nHistoryTaikoComboMusicNum = (unsigned short)nValue;
                    break;
                case eEvent_HistoryCloth:
                    pRole->nHistoryClothNum = nValue;
                    break;
                case eEvent_HistoryWishItem:
                    pRole->nHistoryWishItemNum = nValue;
                    break;
                case eEvent_HistoryGainWishItem:
                    pRole->nGainWishItemNum = nValue;
                    break;
                case eEvent_HistoryUseBoquet:
                    pRole->nHistoryUseBouquetNum = nValue;
                    break;
                case eEvent_HistoryAutoMatchWithOppositeSex:
                    pRole->nAutoMatchWithOppositeSexNum = nValue;
                    break;
                case eEvent_HistoryBadage:
                    pRole->nHistoryBadageNum = nValue;
                    break;
                case eEvent_CompleteTeachGuide:
                    pRole->bCompleteTeachGuide = true;
                    break;
                case eEvent_FinishFirstPayment:
                {
                                                  pRole->bCompleteFirstPaymentTag = false;//此值会在外部通过nUpdateTime进行判断并重置
                                                  pRole->m_nFirstPaymentMoneyCount = nValue;
                                                  pRole->m_nFirstPaymentTime = nUpdateTime;
                }
                    break;
                case eEvent_SentFirstPaymentReward:
                {
                                                      pRole->bCanSendFirstPaymentReward = false;//此值会在外部通过nUpdateTime进行判断并重置
                                                      pRole->m_nGetFirstPaymentRewardTime = nUpdateTime;
                }
                    break;
                case eEvent_FinishFirstPaymentFeed:
                {
                                                      //pRole->bCompleteFirstPaymentTag = false;//此值会在外部通过nUpdateTime进行判断并重置
                                                      //pRole->m_nFirstPaymentMoneyCount = nValue;
                                                      //pRole->m_nFirstPaymentTime = nUpdateTime;
                }
                    break;
                case eEvent_SentFirstPaymentRewardFeed:
                {
                                                          //pRole->bCanSendFirstPaymentReward = false;//此值会在外部通过nUpdateTime进行判断并重置
                                                          //pRole->m_nGetFirstPaymentRewardTime = nUpdateTime;
                }
                    break;
                case eEvent_CompleteProcGuide:
                    pRole->nCompleteProcGuide = (unsigned char)nValue;
                    break;
                case eEvent_ReceivedInvitationReward:
                    pRole->bReceivedInvitationReward = true;
                    break;
                case eEvent_VIPOnlineAnnounce:
                    pRole->VIPInfo.m_nLastOnlineAnnounceTime = nValue;
                    break;
                case eEvent_NoPushRatingToday:
                    pRole->sPushRatingDB.nNoPushRatingTime = nValue;
                    break;
                case eEvent_PushRatedLevelUp:
                    pRole->sPushRatingDB.nPushRatedLevel = nValue;
                    break;
                case eEvent_PushRatedOnlineTime:
                    pRole->sPushRatingDB.nPushRatedOnlineTime = nValue;
                    break;
                case eEvent_PushRatedDailyQuest:
                    pRole->sPushRatingDB.nPushRatedDailyQuest = nValue;
                    break;
                case eEvent_PushRatedDailyShoping:
                    pRole->sPushRatingDB.nPushRatedDailyShoping = nValue;
                    break;
                case eEvent_PushRatedCount:
                    pRole->sPushRatingDB.nPushRatedCount = (int)nValue;
                    pRole->sPushRatingDB.nPushRatedCountTime = nUpdateTime;
                    break;
                case eEvent_CompleteAnimation:
                    pRole->bCompleteAnimation = true;
                    break;
                case eEvent_FresherReward_Day1:
                    pRole->m_setFresherReward.insert(1);
                    break;
                case eEvent_FresherReward_Day2:
                    pRole->m_setFresherReward.insert(2);
                    break;
                case eEvent_FresherReward_Day3:
                    pRole->m_setFresherReward.insert(3);
                    break;
                case eEvent_FresherReward_Day4:
                    pRole->m_setFresherReward.insert(4);
                    break;
                case eEvent_FresherReward_Day5:
                    pRole->m_setFresherReward.insert(5);
                    break;
                case eEvent_FresherReward_Day6:
                    pRole->m_setFresherReward.insert(6);
                    break;
                case eEvent_FresherReward_Day7:
                    pRole->m_setFresherReward.insert(7);
                    break;
                case eEvent_FresherReward_AllDay:
                    pRole->m_setFresherReward.insert(0);
                    break;
                case eEvent_FresherQuestReward_Day1:
                case eEvent_FresherQuestReward_Day2:
                case eEvent_FresherQuestReward_Day3:
                case eEvent_FresherQuestReward_Day4:
                case eEvent_FresherQuestReward_Day5:
                case eEvent_FresherQuestReward_Day6:
                case eEvent_FresherQuestReward_Day7:
                    pRole->m_setFresherQuestReward.insert((int)nValue);
                    break;
                case eEvent_FresherQuest_BestMusicGrade:
                    pRole->m_nFresherBestMusicGrade = (int)nValue;
                    break;
                case eEvent_MallBuyCount:
                    pRole->m_nMallBuyCount = (int)nValue;
                    break;

                    //UCVIP(Event表中存在该ID说明，特权已经领取过了,用1-5分别标识5个特权)
                case eEvent_UCVIP_Pri1:
                    pRole->setUCVIPReward.insert(1);
                    break;
                case eEvent_UCVIP_Pri2:
                    pRole->setUCVIPReward.insert(2);
                    break;
                case eEvent_UCVIP_Pri3:
                    pRole->setUCVIPReward.insert(3);
                    break;
                case eEvent_UCVIP_Pri4:
                    pRole->setUCVIPReward.insert(4);
                    break;
                case eEvent_UCVIP_Pri5:
                    pRole->setUCVIPReward.insert(5);
                    break;
                case eEvent_ConstellationMatch_EnterLobby:
                    pRole->m_bHasEnterConstellationLobby = nValue > 0 ? true : false;
                    break;
                case eEvent_ConstellationMatch_Card1:
                    card.m_nIndex = 0;
                    card.m_nConstellation = (unsigned char)nValue;
                    card.m_nCoolDownEndTime = nUpdateTime;
                    pRole->m_listCard.push_back(card);
                    break;
                case eEvent_ConstellationMatch_Card2:
                    card.m_nIndex = 1;
                    card.m_nConstellation = (unsigned char)nValue;
                    card.m_nCoolDownEndTime = nUpdateTime;
                    pRole->m_listCard.push_back(card);
                    break;
                case eEvent_ConstellationMatch_Count:
                    pRole->m_nConstellationMatchCount = nValue;
                    pRole->m_nConstellationMatchTime = nUpdateTime;
                    break;
                case eEvent_ConstellationMatch_RoomTime:
                    pRole->m_nCosntellationRoomID = nValue;
                    pRole->m_nCosntellationRoomCreateTime = nUpdateTime;
                    break;
                case eEvent_ConstellationMatch_RoomLine:
                    pRole->m_nCosntellationRoomLine = (int)nValue;
                    break;
                case eEvent_LastChangeBirthdayTime:
                    pRole->m_nLastChangeBirthdayTime = nUpdateTime;
                    break;
                case eEvent_DanceGroupFirstAddPowerReward:
                    pRole->m_bDanceGroupFirstAddPowerReward = true;
                    pRole->m_nDanceGroupFirstAddPowerRewardTime = nUpdateTime;
                    break;
                case eEvent_ProprietaryActivityReward:
                    pRole->m_bPropretaryActivityReward = true;
                    break;
                case eEvent_ReceivedProcGuideReward:
                    pRole->m_bReceiveProcGuideReward = true;
                    break;
                case eEvent_MallSendItemCostBill:
                    pRole->nMallSendItemCostBill = nValue;
                    pRole->nMallSendItemTime = nUpdateTime;
                    break;
                case eEvent_RecommendFriends:
                    pRole->m_nRecommendRefreshCount = nValue;
                    pRole->m_nLastRecommendResetTime = nUpdateTime;
                    break;
                case eEvent_Pet_GuideEvent:
                    pRole->m_nPetGuideStep = (int)nValue;
                    break;
                case eEvent_GuideData:
                    pRole->m_nGuideData = nValue;
                    break;
                case eEvent_DanceGroupChallengeChance:
                {
                                                         pRole->m_nDanceGroupChallengeChance = (unsigned short)nValue;
                                                         pRole->m_nDanceGroupChallengeChanceResetTime = nUpdateTime;
                }
                    break;
                case eEvent_DanceGroupChallengeBuyChance:
                {
                                                            pRole->m_nDanceGroupChallengeBuyChance = (unsigned short)nValue;
                                                            pRole->m_nDanceGroupChallengeBuyChanceResetTime = nUpdateTime;
                }
                    break;
                case eEvent_RedEnvelope_Anonymity:
                    pRole->m_bRedEnvelopeAnonymity = nValue > 0 ? true : false;
                    break;
                default:
                    break;
                }
            }
        }
        SafeFreeResult();

        sprintf(sql, "SELECT eventid,totalcount, lasteventtime FROM weeklyevent WHERE roleid = %u ", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                unsigned short eventid = (unsigned short)atoi(row[0]);
                unsigned int nCount = (unsigned int)atoi(row[1]);
                unsigned int nLastEventTime = (unsigned int)atoi(row[2]);
                switch (eventid)
                {
                case 0:
                    pRole->smallSpeakersUseNumWeekly.m_nCount = (unsigned short)nCount;
                    pRole->smallSpeakersUseNumWeekly.m_nLastEventTime = nLastEventTime;
                    break;
                case 1:
                    pRole->bigSpeakersUseNumWeekly.m_nCount = (unsigned short)nCount;
                    pRole->bigSpeakersUseNumWeekly.m_nLastEventTime = nLastEventTime;
                    break;
                case 2:
                    pRole->sendFlowersNumInSevenWeekly.m_nCount = (unsigned short)nCount;
                    pRole->sendFlowersNumInSevenWeekly.m_nLastEventTime = nLastEventTime;
                    break;
                default:
                    break;
                }
            }
        }
    }

    return Query_Res_OK;
}
//LoadHistoryClothCount(pRole);
//LoadHistoryWishItemCount(pRole);
//LoadGainWishItemCount(pRole);
//LoadHistoryUseBouquetCount(pRole);
//LoadHistoryBadageCount(pRole);

//LoadSmallSpeakersInSevenDays(pRole);
//LoadBigSpeakersInSevenDays(pRole);
//LoadFlowersSendInSevenDays(pRole);

//	return Query_Res_OK;
//}

//int CDBInterface::LoadHistoryClothCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, eEvent_HistoryCloth);
//
//		if(ExecSQL())
//		{
//			while( (row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nHistoryClothNum = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

//int CDBInterface::LoadHistoryWishItemCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, HistoryWishItem);
//
//		if (ExecSQL())
//		{
//			while ((row = FetchRow()) != NULL)
//		 {
//			 int nField = 0;
//			 pRole->nHistoryWishItemNum =  (unsigned int)atoi(row[nField]);
//		 }
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	 return nRes;
//}

//int CDBInterface::LoadGainWishItemCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, HistoryGainWishItem);
//
//		if (ExecSQL())
//		{
//			while ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nGainWishItemNum = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

//int CDBInterface::LoadSmallSpeakersInSevenDays(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT totalcount, lasteventtime FROM weeklyevent WHERE roleid = %u AND eventid = %d", pRole->nRoleID, 0);
//
//		if (ExecSQL())
//		{
//			if ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->smallSpeakersUseNumWeekly.m_nCount = (unsigned int)atoi(row[nField++]);
//				pRole->smallSpeakersUseNumWeekly.m_nLastEventTime = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

//int CDBInterface::LoadBigSpeakersInSevenDays(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT totalcount, lasteventtime FROM weeklyevent WHERE roleid = %u AND eventid = %d", pRole->nRoleID, 1);
//
//		if (ExecSQL())
//		{
//			if ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->bigSpeakersUseNumWeekly.m_nCount = (unsigned int)atoi(row[nField++]);
//				pRole->bigSpeakersUseNumWeekly.m_nLastEventTime = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

//int CDBInterface::LoadFlowersSendInSevenDays(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT totalcount, lasteventtime FROM weeklyevent  WHERE roleid = %u AND eventid = %d", pRole->nRoleID, 2);
//
//		if (ExecSQL())
//		{
//			if ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->sendFlowersNumInSevenWeekly.m_nCount = (unsigned int)atoi(row[nField++]);
//				pRole->sendFlowersNumInSevenWeekly.m_nLastEventTime = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}


//int CDBInterface::LoadHistoryUseBouquetCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, eEvent_HistoryUseBoquet);
//
//		if (ExecSQL())
//		{
//			while ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nHistoryUseBouquetNum = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();
//	}
//
//	return nRes;
//}

//int CDBInterface::LoadHistoryBadageCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, eEvent_HistoryBadage);
//
//		if (ExecSQL())
//		{
//			while ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nHistoryBadageNum =  (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

int CDBInterface::LoadTitle(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        pRole->listTitle.clear();
        sprintf(sql, "SELECT titleid, ttime FROM title WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                CTitleInfo titleInfo;
                titleInfo.m_nTitleID = (unsigned int)atoi(row[nField++]);
                titleInfo.m_nGainTitleTime = strtoul(row[nField++], &pStrStop, 10);;

                pRole->listTitle.push_back(titleInfo);
            }
            nRes = Query_Res_OK;
        }

        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::LoadAchievement(LPROLEINFO_DB	pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        pRole->listAchievement.clear();
        sprintf(sql, "SELECT achievementid, ttime, state FROM achievement WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                CAchievementInfo achievementInfo;
                achievementInfo.m_nAchievementID = (unsigned short)atoi(row[nField++]);;
                achievementInfo.m_nCompletedTime = strtoul(row[nField++], &pStrStop, 10);;
                achievementInfo.m_nState = (unsigned char)atoi(row[nField]);

                pRole->listAchievement.push_back(achievementInfo);
            }
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::UpdateAchievementState(unsigned int nRoleID, unsigned int nAchievementID, unsigned char nState)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0 && nAchievementID > 0)
    {
        sprintf(sql, "UPDATE achievement SET state = %u WHERE roleID = %u AND achievementid = %u ", nState, nRoleID, nAchievementID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

//int CDBInterface::LoadFightResult(LPROLEINFO_DB pRole)
//{
//	if (pRole == NULL)
//	{
//		return Query_Res_Fail;
//	}
//	//LoadHistoryTaikoComboMusicCount(pRole);
//	LoadHistoryFightResult(pRole);
//	//LoadHistoryAutoMatchWithOppositeSexCount(pRole);
//
//	return Query_Res_OK;
//}

//int CDBInterface::LoadHistoryTaikoComboMusicCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, eEvent_HistoryComboMusic);
//
//		if (ExecSQL())
//		{
//			if((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nHistoryTaikoComboMusicNum =  (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

int CDBInterface::LoadHistoryFightResult(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        pRole->mapFightResult.clear();
        sprintf(sql, "SELECT mode, difficulty, result0, result1, result2, result3, result4, result5, result6, score, conperfectnum FROM fightresult WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                CFightResult fightResult;
                fightResult.m_nModeID = (unsigned char)atoi(row[nField++]);
                fightResult.m_nDifficulty = (unsigned char)atoi(row[nField++]);
                fightResult.m_nResult[0] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[1] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[2] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[3] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[4] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[5] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nResult[6] = (unsigned int)atoi(row[nField++]);
                fightResult.m_nHistoryTotalScore = (unsigned int)atoi(row[nField++]);
                fightResult.m_nConPerfectNum = (unsigned int)atoi(row[nField++]);

                pRole->mapFightResult[fightResult.m_nModeID][fightResult.m_nDifficulty] = fightResult;
            }
            nRes = Query_Res_OK;
        }

        SafeFreeResult();
    }

    return nRes;
}

//int CDBInterface::LoadHistoryAutoMatchWithOppositeSexCount(LPROLEINFO_DB pRole)
//{
//	int nRes = Query_Res_Fail;
//
//	if (pRole != NULL)
//	{
//		sprintf(sql, "SELECT count FROM event WHERE roleid = %u AND eventid = %d", pRole->nRoleID, eEvent_HistoryAutoMatchWithOppositeSex);
//
//		if (ExecSQL())
//		{
//			while ((row = FetchRow()) != NULL)
//			{
//				int nField = 0;
//				pRole->nAutoMatchWithOppositeSexNum = (unsigned int)atoi(row[nField]);
//			}
//			nRes = Query_Res_OK;
//		}
//		SafeFreeResult();	
//	}
//
//	return nRes;
//}

int CDBInterface::AddHistoryWeeklyEventEntry(unsigned int nRoleID, unsigned char nEventID)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "INSERT INTO weeklyevent(roleid, eventid) VALUE(%u, %d)", nRoleID, nEventID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::UpdateHistoryWeeklyEventEntry(unsigned int nRoleID, unsigned char nEventID, unsigned int nCount, unsigned int nTime)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "call update_weekly_event(%u, %u, %u, %u)", nRoleID, nEventID, nCount, nTime);

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }

        SafeFreeResult();
    }

    return nRes;
}

// 一周内使用的小喇叭数量	
int CDBInterface::AddHistorySmallSpeakersUseCountInSevenDays(unsigned int nRoleID)
{
    return AddHistoryWeeklyEventEntry(nRoleID, HistoryWeeklySmallSpeaker);
}

int CDBInterface::UpdateHistorySmallSpeakersUseCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime)
{
    return UpdateHistoryWeeklyEventEntry(nRoleID, HistoryWeeklySmallSpeaker, nCount, nTime);
}

// 一周内使用的大喇叭数量
int CDBInterface::AddHistoryBigSpeakersUseCountInSevenDays(unsigned int nRoleID)
{
    return AddHistoryWeeklyEventEntry(nRoleID, HistoryWeeklyBigSpeaker);
}

int CDBInterface::UpdateHistoryBigSpeakersUseCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime)
{
    return UpdateHistoryWeeklyEventEntry(nRoleID, HistoryWeeklyBigSpeaker, nCount, nTime);
}

// 一周内鲜花的次数
int CDBInterface::AddHistorySendFlowerCountInSevenDays(unsigned int nRoleID)
{
    return AddHistoryWeeklyEventEntry(nRoleID, HistoryWeeklySentFlowers);
}

int CDBInterface::UpdaetHistorySendFlowerCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime)
{
    return UpdateHistoryWeeklyEventEntry(nRoleID, HistoryWeeklySentFlowers, nCount, nTime);
}


int CDBInterface::LoadChekIn(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        pRole->checkInInfo = CCheckInInfo();
        // 每周有且仅有一个条目需要更新
        sprintf(sql, "select dayindex, checkintime, vipouttimedayindex  from checkin1 where roleid = %u", pRole->m_nRoleID);
        if (ExecSQL())
        {
            if ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                pRole->checkInInfo.m_nDayIndex = (unsigned int)atoi(row[nField++]);
                pRole->checkInInfo.m_nCheckInTime = (unsigned int)atoi(row[nField++]);
                pRole->checkInInfo.m_nVipOutTimeDayIndex = (unsigned int)atoi(row[nField++]);
            }
            nRes = Query_Res_OK;
        }

        SafeFreeResult();
    }
    return nRes;
}

int CDBInterface::InsertCheckIn(unsigned int nRoleID, CCheckInInfo *pInfo)
{
    int nRes = Query_Res_Fail;

    if (nRoleID >0 && pInfo != NULL)
    {
        sprintf(sql, "INSERT INTO nsytroledb.checkin1( roleid, dayindex, checkintime, vipouttimedayindex )values(%u,%u,%u,%u)", nRoleID, pInfo->m_nDayIndex, pInfo->m_nCheckInTime, pInfo->m_nVipOutTimeDayIndex);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::UpdateCheckIn(unsigned int nRoleID, CCheckInInfo *pInfo)
{
    int nRes = Query_Res_Fail;

    if (nRoleID >0 && pInfo != NULL)
    {
        sprintf(sql, "select dayindex, checkintime from checkin1 where roleid = %u", nRoleID);
        if (ExecSQL())
        {
            if ((row = FetchRow()) == NULL)
            {
                sprintf(sql, "INSERT INTO nsytroledb.checkin1( roleid, dayindex, checkintime,vipouttimedayindex)values(%u,%u,%u,%u)", nRoleID, pInfo->m_nDayIndex, pInfo->m_nCheckInTime, pInfo->m_nVipOutTimeDayIndex);
            }
            else
            {
                sprintf(sql, "UPDATE nsytroledb.checkin1 SET dayindex = %u, checkintime = %u, vipouttimedayindex = %u WHERE roleid = %u", pInfo->m_nDayIndex, pInfo->m_nCheckInTime, pInfo->m_nVipOutTimeDayIndex, nRoleID);
            }

            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
            SafeFreeResult();
        }
    }

    return nRes;
}

int CDBInterface::AddCheckInGift(unsigned int nRoleID, unsigned char nPacketType)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0 && IsValidCheckPacketType(nPacketType))
    {
        sprintf(sql, "INSERT INTO nsytroledb.checkingift(roleid, type) VALUE(%u, %u)", nRoleID, nPacketType);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::RemoveCheckInGift(unsigned int nRoleID, unsigned char nPacketType)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0 && nPacketType > 0)
    {
        sprintf(sql, "DELETE FROM nsytroledb.checkingift WHERE roleid = %u AND type = %u", nRoleID, nPacketType);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }

        SafeFreeResult();
    }

    return nRes;
}
/*
int CDBInterface::LoadCheckInGift(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        sprintf(sql, "SELECT type FROM nsytroledb.checkingift WHERE roleid = %u", pRole->m_nRoleID);
        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL) {
                int nField = 0;
                unsigned char nPacket = (unsigned char)atoi(row[nField]);
                pRole->listCheckInGift.push_back(nPacket);
            }
        }

        nRes = Query_Res_OK;
        SafeFreeResult();
    }

    return nRes;
}*/

int CDBInterface::SaveVIPInfo(unsigned int nRoleID, CRoleVIPInfo &VIPInfo)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0)
    {
        sprintf(sql, "call save_vip(%u, %u, %u, %u, %lld, %u, %d, %d)",
            nRoleID, VIPInfo.m_nOpenTime, VIPInfo.m_nLevel, VIPInfo.m_nExp, VIPInfo.m_nExpiredTime,
            VIPInfo.m_nLastGetRewardTime, VIPInfo.m_nConsumedPt, VIPInfo.m_nExchangedPt);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::SaveVIPConsumedPt(unsigned int nRoleID, int nConsumedPt)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT expired_time FROM vip WHERE roleid = %u", nRoleID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            long long nExpiredTime = atoi64(row[nField++]);

            if (nExpiredTime > (long long)time(NULL))
            {
                SafeFreeResult();
                sprintf(sql, "update vip set consumed_pt = consumed_pt + %d WHERE roleid = %u", nConsumedPt, nRoleID);
                if (ExecSQL())
                {
                    nRes = Query_Res_OK;
                }
            }
            else
            {
                nRes = Query_Res_OK;
            }
        }
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadVIPInfo(unsigned int nRoleID, CRoleVIPInfo &VIPInfo)
{
    int nRes = Query_Res_Fail;

    if (nRoleID > 0)
    {
        sprintf(sql, "SELECT open_time, level, exp, expired_time, last_get_reward_time, consumed_pt, exchanged_pt FROM vip WHERE roleid = %u", nRoleID);
        if (ExecSQL())
        {
            if ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                VIPInfo.m_nOpenTime = strtoul(row[nField++], NULL, 10);
                VIPInfo.m_nLevel = (unsigned short)atoi(row[nField++]);
                VIPInfo.m_nExp = strtoul(row[nField++], NULL, 10);
                VIPInfo.m_nExpiredTime = atoi64(row[nField++]);
                VIPInfo.m_nLastGetRewardTime = strtoul(row[nField++], NULL, 10);
                VIPInfo.m_nConsumedPt = atoi(row[nField++]);
                VIPInfo.m_nExchangedPt = atoi(row[nField++]);
            }
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

void CDBInterface::LoadVIP(LPROLEINFO_DB pRole)
{
    if (pRole != NULL)
    {
        LoadVIPInfo(pRole->m_nRoleID, pRole->VIPInfo);
    }
}

int CDBInterface::LoadMoneyResetInfo(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (pRole != NULL) {
        sprintf(sql, "select resettime, mailremindtime, mallremindtime from moneyreset where roleid = %u", pRole->m_nRoleID);
        if (ExecSQL())
        {
            if ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                pRole->m_nLastMoneyResetTime = (unsigned int)atoi(row[nField++]);
                pRole->m_nLastMoneyResetMailRemindTime = (unsigned int)atoi(row[nField++]);
                pRole->m_nLastMoneyResetMallRemindTime = (unsigned int)atoi(row[nField]);
            }
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::ResetMoney(unsigned int nRoleID, unsigned int nResetTime)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0) {
        sprintf(sql, "call update_money_resetmoney(%u, %u)", nRoleID, nResetTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::UpdateMoneyResetTime(unsigned int nRoleID, unsigned int nResetTime)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0) {
        sprintf(sql, "call update_money_resettime(%u, %u)", nRoleID, nResetTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::UpdateMoneyMailRemindTime(unsigned int nRoleID, unsigned int nRemindTime)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0) {
        sprintf(sql, "call update_money_mailremindtime(%u, %u)", nRoleID, nRemindTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::UpdateMoneyMallRemindTime(unsigned int nRoleID, unsigned int nRemindTime)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0) {
        sprintf(sql, "call update_money_mallremindtime(%u, %u)", nRoleID, nRemindTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

//在线人数
int CDBInterface::AddOnlineState(int num)
{
    sprintf(sql, "Insert into onlinestate set curtotalplayercount = %d", num);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadKeepOnlineRewardInfo(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (pRole != NULL) {
        pRole->keepOnlineRewardInfo = CKeepOnlineReward();
        sprintf(sql, "select idx, cumulativetime, timingday, getreward from keeponlinereward where roleid = %u", pRole->m_nRoleID);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
            if ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                pRole->keepOnlineRewardInfo.m_nIndex = atoi(row[nField++]);
                pRole->keepOnlineRewardInfo.m_nCumulativeTime = atoi(row[nField++]);
                pRole->keepOnlineRewardInfo.m_nTimingDay = (unsigned int)atoi(row[nField++]);
                pRole->keepOnlineRewardInfo.m_bGotReward = (unsigned int)atoi(row[nField]) > 0 ? true : false;
            }
        }
    }

    return nRes;
}

int CDBInterface::UpdateKeepOnlineRewardInfo(unsigned int nRoleID, CKeepOnlineReward & info)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call update_keeponliereward(%u, %u, %u, %d, %u)", nRoleID, info.m_nIndex, info.m_nCumulativeTime, info.m_nTimingDay, info.m_bGotReward == true ? 1 : 0);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

// GM
int CDBInterface::GetRcvGMSysRoleInfo(list<CRcvSysMailRoleInfo> & listRoleInfo)
{
    int nRes = Query_Res_Fail;

    for (list<CRcvSysMailRoleInfo>::iterator itr = listRoleInfo.begin(); itr != listRoleInfo.end(); ++itr) {
        char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
        CDBIMysql::EscapeString(szSQLPUID, itr->m_strPUID.c_str(), itr->m_strPUID.length());

        sprintf(sql, "select account from nsytaccountdb.player where pid = %u AND puid = '%s'", itr->m_nPid, szSQLPUID);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
            row = FetchRow();
            if (row != NULL) {
                unsigned int nAccount = (unsigned int)atoi(row[0]);
                SafeFreeResult();
                if (nAccount != 0) {
                    nRes = Query_Res_Fail;
                    sprintf(sql, "select roleid, rolename from nsytroledb.role where account = %u", nAccount);
                    if (ExecSQL())
                    {
                        nRes = Query_Res_OK;
                        row = FetchRow();
                        if (row != NULL) {
                            int nFiled = 0;
                            itr->m_nRoleID = (unsigned int)atoi(row[nFiled++]);
                            itr->m_strRoleName = row[nFiled];
                        }
                        SafeFreeResult();
                    }
                }
            }
        }
    }

    return nRes;
}

int CDBInterface::QueryGiftByTimeRange(unsigned short nGiftID, int & nCount, string & strStartTime, string & strEndTime)
{
    bool bRes = false;
    char *end = sql;
    //sprintf(sql, "select count(*) from giftcode where giftid = %u and roleid > 0 and starttime >= unix_timestamp(' and endtime <= %u",nGiftID, nStartTime, nEndTime);
    sprintf(sql, "select count(*) from giftcode where giftid = %u and roleid > 0 and drawtime >= unix_timestamp('", nGiftID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, strStartTime.c_str(), strStartTime.size());
    strcat(end, "') and drawtime <= unix_timestamp('");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, strEndTime.c_str(), strEndTime.size());
    strcat(end, "')");
    end += strlen(end);

    bRes = ExecSQL();
    if (bRes) {
        row = FetchRow();
        if (row != NULL) {
            nCount = atoi(row[0]);
        }
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::GetGiftCodesState(list<CQueryGiftState> & listQueryCodes)
{
    for (list<CQueryGiftState>::iterator itr = listQueryCodes.begin(); itr != listQueryCodes.end(); ++itr)
    {
        char *end = sql;
        sprintf(end, "select giftid,roleid,starttime,endtime, drawtime, from_unixtime(drawtime) from giftcode where code = '");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, itr->m_strGiftCode.c_str(), strlen(itr->m_strGiftCode.c_str()));
        strcat(end, "'");
        end += strlen(end);

        if (ExecSQL())
        {
            row = FetchRow();
            if (row != NULL)
            {
                int nFiled = 0;
                itr->m_bValid = true;
                itr->m_nGiftID = (unsigned int)atoi(row[nFiled++]);
                itr->m_nRoleID = (unsigned int)atoi(row[nFiled++]);
                itr->m_nStartTime = (unsigned int)atoi(row[nFiled++]);
                itr->m_nEndTime = (unsigned int)atoi(row[nFiled++]);
                itr->m_nDrawTime = (unsigned int)atoi(row[nFiled++]);
                if (itr->m_nDrawTime > 0)
                {
                    itr->m_strDrawTime = row[nFiled];
                }
            }
        }
        SafeFreeResult();
    }

    return Query_Res_OK;
}

int CDBInterface::LogSQLLog(ISQLLog & rSQLLog)
{
    rSQLLog.ToSQL(sql, MAX_SQL_LEN);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadGift(int nAccountID, std::map< int, GiftDB > & rGiftMap)
{
    rGiftMap.clear();
    int nRes = Query_Res_Fail;
    time_t tNow = time(NULL);
    sprintf(sql, "SELECT a.id, a.gift_id, a.expired_time FROM nsytaccountdb.account_gift a "
        "LEFT JOIN nsytaccountdb.player p ON a.pid=p.pid AND a.puid=p.puid WHERE p.account=%d AND a.get_time=0 AND a.expired_time>%u",
        nAccountID, (unsigned int)tNow);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            int nIndexID = atoi(row[nField++]);
            GiftDB giftDB;
            giftDB.m_nGiftID = atoi(row[nField++]);
            giftDB.m_nExpiredTime = (unsigned int)atoi(row[nField++]);
            rGiftMap[nIndexID] = giftDB;
        }
    }
    nRes = Query_Res_OK;
    SafeFreeResult();
    return nRes;
}

int CDBInterface::GetTheGiftReward(int nIndexID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "UPDATE account_gift SET get_time=%u WHERE id=%d", (unsigned int)time(NULL), nIndexID);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::SaveCumulativeRecharge(unsigned int nRoleID, unsigned int nLastReceivedTime, std::vector<int> & rVecReward)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call save_cumulative_recharge(%u, %u, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
        nRoleID, nLastReceivedTime, rVecReward[0], rVecReward[1], rVecReward[2],
        rVecReward[3], rVecReward[4], rVecReward[5], rVecReward[6], rVecReward[7], rVecReward[8]);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadCumulativeRechargeReward(unsigned int nRoleID, unsigned int nRechargeStartTime, unsigned int nRechargeStopTime, std::vector<int> & rVecReward)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call load_cumulative_recharge_reward(%u, %u, %u, @level1, @level2, @level3, @level4, @level5, @level6, @level7, @level8, @level9)", nRoleID, nRechargeStartTime, nRechargeStopTime);
    if (ExecSQL()) {
        SafeFreeResult();
        sprintf(sql, "select @level1, @level2, @level3, @level4, @level5, @level6, @level7, @level8, @level9");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                if (row != NULL && *row != NULL) {
                    if (rVecReward.size() >= CUMULATIVE_REWARD_LEVEL) {
                        for (int i = 0; i < CUMULATIVE_REWARD_LEVEL; ++i) {
                            rVecReward[i] = atoi(row[i]);
                        }
                    }
                }
            }
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadCumulativeSpendReward(unsigned int nRoleID, std::vector<unsigned int> & rewardvec)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT level1,level2,level3,level4,level5,level6,level7,level8,level9 FROM cumulative_spend_reward WHERE roleid = %u ", nRoleID);
    if (ExecSQL())
    {
        if ((mysql_num_rows(result)) > 0)
        {
            row = FetchRow();

            int nField = 0;
            rewardvec[0] = (unsigned int)atoi(row[nField++]);
            rewardvec[1] = (unsigned int)atoi(row[nField++]);
            rewardvec[2] = (unsigned int)atoi(row[nField++]);
            rewardvec[3] = (unsigned int)atoi(row[nField++]);
            rewardvec[4] = (unsigned int)atoi(row[nField++]);
            rewardvec[5] = (unsigned int)atoi(row[nField++]);
            rewardvec[6] = (unsigned int)atoi(row[nField++]);
            rewardvec[7] = (unsigned int)atoi(row[nField++]);
            rewardvec[8] = (unsigned int)atoi(row[nField++]);
        }
    }

    nRes = Query_Res_OK;
    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadCumulativeSpendInfo(unsigned int nRoleID, CCumulativeSpendDB & spendinfo)
{
    sprintf(sql, "SELECT billtype,billvalue FROM cumulative_spend_log WHERE roleid = %u And time >= %u and time <= %u ",
        nRoleID, spendinfo.m_nSpendStartTime, spendinfo.m_nSpendStopTime);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            int nBillType = (int)(atoi(row[nField++]));
            if (nBillType == EMallCurrencyType_MCoin)
            {
                spendinfo.m_nBillNum += (int)(atoi(row[nField++]));
            }
            else
            {
                spendinfo.m_nBindBillNum += (int)(atoi(row[nField++]));
            }
        }

    }
    SafeFreeResult();

    return Query_Res_OK;
}

int CDBInterface::UpdateCumulativeSpendReward(unsigned int nRoleID, int nRewardIndex)
{
    switch (nRewardIndex)
    {
    case 1:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level1 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 2:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level2 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 3:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level3 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 4:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level4 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 5:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level5 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 6:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level6 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 7:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level7 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 8:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level8 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    case 9:
        sprintf(sql, "UPDATE cumulative_spend_reward SET level9 = %d WHERE roleid = %u", (unsigned int)time(NULL), nRoleID);
        break;
    }

    return  ExecSQL();
}

int CDBInterface::InsertCumulativeSpendReward(unsigned int nRoleID, int nRewardIndex)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "insert into cumulative_spend_reward set roleid = %u, level1 = %u,level2 = %u,level3 = %u,level4 = %u,level5 = %u,level6 = %u, level7 = %u, level8 = %u, level9 = %d ", nRoleID, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::InsertCumulativeSpendBill(unsigned int nRoleID, int nValue)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "insert into cumulative_spend_log set roleid = %u, billtype = %d, billvalue = %d, time = %u", nRoleID, EMallCurrencyType_MCoin, nValue, (unsigned int)time(NULL));
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::InsertCumulativeSpendBindBill(unsigned int nRoleID, int nValue)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "insert into cumulative_spend_log set roleid = %u, billtype = %d, billvalue = %d, time = %u", nRoleID, EMallCurrencyType_BindMCoin, nValue, (unsigned int)time(NULL));
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadCumulativeRechargeInfo(unsigned int nAccount, CCumulativeRechargeDB & rRechargeInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select sum(pt) from chargelog where unix_timestamp(dt) >= %u and unix_timestamp(dt) <= %u and account = %u",
        rRechargeInfo.m_nRechargeStartTime, rRechargeInfo.m_nRechargeStopTime, nAccount);

    if (ExecSQL()) {
        if (mysql_num_rows(result) > 0) {
            row = FetchRow();
            if (row != NULL && *row != NULL) {
                rRechargeInfo.m_nRechargeNum = atoi(row[0]);
            }
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadCumulativeSpendGotMedalReward(const CumulativeSpendGotMedalDB &rInfo, std::vector<bool> &rGotReward)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select position from cumulative_spend_medal_reward where roleid = %u and time >= %u and time <= %u",
        rInfo.m_nRoleID, rInfo.m_nOpenTime, rInfo.m_nCloseTime);

    if (ExecSQL())
    {
        unsigned char nIndex = 0;

        while ((row = FetchRow()) != NULL)
        { 
            nIndex = (unsigned char)atoi(row[0]);
            if (nIndex < (unsigned char)rGotReward.size())
            {
                rGotReward[nIndex] = true;
            }
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::InsertCumulativeSpendGotMedalReward(unsigned int nRoleID, unsigned char nIndex)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "insert into cumulative_spend_medal_reward set roleid = %u, position = %u, time = %u",
        nRoleID, (unsigned int)nIndex, (unsigned int)time(NULL));

    if (ExecSQL())
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::LoadBuyItemGotMedalRewardedInfo(const BuyItemGotMedalDB &rInfo, BuyItemGotMedalRewardedInfoMap &rRewardedInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select position, buynum, rewardtime from buy_item_medal_reward "
        "where roleid = %u and time >= %u and time <= %u",
        rInfo.m_nRoleID, rInfo.m_nOpenTime, rInfo.m_nCloseTime);

    if (ExecSQL())
    {
        int nField = 0;
        unsigned char nIndex = 0;
        unsigned int nRewardTime = 0;

        while ((row = FetchRow()) != NULL)
        {
            nField = 0;
            BuyItemGotMedalRewardedInfo rewarded;

            nIndex = (unsigned char)atoi(row[nField++]);
            rewarded.m_nBuyNum = (unsigned short)atoi(row[nField++]);
            nRewardTime = (unsigned int)strtoul(row[nField++], NULL, 10);

            if (nRewardTime != 0)
            {
                rewarded.m_bRewarded = true;
            }

            rRewardedInfo.insert(std::make_pair(nIndex, rewarded));
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::SetBuyItemGotMedalRewarded(const BuyItemGotMedalDB &rInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "update buy_item_medal_reward set rewardtime = %u "
        "where roleid = %u and time >= %u and time <= %u and position = %u",
        (unsigned int)time(NULL), rInfo.m_nRoleID, rInfo.m_nOpenTime,
        rInfo.m_nCloseTime, (unsigned int)rInfo.m_nIndex);

    if (ExecSQL())
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::SaveBuyItemGotMedalInfo(bool bInsert, const BuyItemGotMedalDB &rInfo)
{
    int nRes = Query_Res_Fail;

    if (bInsert)
    {
        sprintf(sql, "insert into buy_item_medal_reward set roleid = %u, position = %u, buynum = %u, time = %u",
            rInfo.m_nRoleID, (unsigned int)rInfo.m_nIndex, (unsigned int)rInfo.m_nBuyNum, (unsigned int)time(NULL));
    }
    else
    {
        sprintf(sql, "update buy_item_medal_reward set buynum = %u "
            "where roleid = %u and time >= %u and time <= %u and position = %u",
            (unsigned int)rInfo.m_nBuyNum, rInfo.m_nRoleID, rInfo.m_nOpenTime,
            rInfo.m_nCloseTime, (unsigned int)rInfo.m_nIndex);
    }

    if (ExecSQL())
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::SaveXmasFreeRewardInfo(unsigned int nRoleID, unsigned int nLastGetRewardTime, std::vector<int> & rVecReward)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call save_christmas_free_reward(%u, %u, %d, %d, %d, %d, %d)", nRoleID, nLastGetRewardTime, rVecReward[0], rVecReward[1], rVecReward[2], rVecReward[3], rVecReward[4]);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadXmaxFreeRewardInfo(CXmasFreeRewardDB &rXmasFreeRewardInfo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "call load_christmas_free_reward(%u, %u, %u, @last_get_reward_time, @level1, @level2, @level3, @level4, @level5)", rXmasFreeRewardInfo.m_nRoleID, rXmasFreeRewardInfo.m_nActivityStartTime, rXmasFreeRewardInfo.m_nActivityEndTime);
    if (ExecSQL()) {
        SafeFreeResult();
        sprintf(sql, "select @last_get_reward_time, @level1, @level2, @level3, @level4, @level5");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                if (row != NULL && *row != NULL) {
                    int nFiled = 0;
                    rXmasFreeRewardInfo.m_nLastGetRewardTime = strtoul(row[nFiled++], NULL, 10);
                    if (rXmasFreeRewardInfo.m_vecReward.size() >= XMAS_FREEREWARD_LEVEL) {
                        for (int i = 0; i < XMAS_FREEREWARD_LEVEL; ++i) {
                            rXmasFreeRewardInfo.m_vecReward[i] = atoi(row[nFiled++]);
                        }
                    }
                }
            }
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadRoleBlessActivityInfo(CRoleBlessActivityDB *pBlessInfoDB)
{
    int nRes = Query_Res_Fail;
    if (pBlessInfoDB != NULL) {
        sprintf(sql, "select count(*) from festival_bless where bless_time >= %u and bless_time <= %u and rcverid = %u", pBlessInfoDB->m_nBlessStartTime, pBlessInfoDB->m_nBlessEndTime, pBlessInfoDB->m_nRoleID);
        if (ExecSQL()) {
            row = FetchRow();
            if (row != NULL && *row != NULL) {
                pBlessInfoDB->m_nBeBlessedCount = strtoul(row[0], NULL, 10);
            }
            SafeFreeResult();
            sprintf(sql, "select a.rolename, a.roleid, b.words_index, b.bless_time from role a, festival_bless b where b.bless_time >= %u and b.bless_time <= %u and b.rcverid = %u and a.roleid = b.senderid",
                pBlessInfoDB->m_nBlessStartTime, pBlessInfoDB->m_nBlessEndTime, pBlessInfoDB->m_nRoleID);
            if (ExecSQL()) {
                while ((row = FetchRow()) != NULL) {
                    int nField = 0;
                    CFestivalBlessEntry entry;
                    entry.m_strSenderName = row[nField++];
                    entry.m_nSenderID = strtoul(row[nField++], NULL, 10);
                    entry.m_nWordsIndex = (unsigned short)atoi(row[nField++]);
                    entry.m_nBlessTime = strtoul(row[nField], NULL, 10);
                    pBlessInfoDB->m_listBlesses.push_front(entry);
                }
                SafeFreeResult();
                sprintf(sql, "call load_festival_bless_reward(%u, %u, %u, @level1, @level2, @level3, @level4, @level5, @level6, @level7, @level8, @level9, @level10)",
                    pBlessInfoDB->m_nRoleID, pBlessInfoDB->m_nBlessStartTime, pBlessInfoDB->m_nBlessEndTime);
                if (ExecSQL()) {
                    SafeFreeResult();
                    sprintf(sql, "select @level1, @level2, @level3, @level4, @level5, @level6, @level7, @level8, @level9, @level10");
                    if (ExecSQL()) {
                        if (mysql_num_rows(result) > 0) {
                            row = FetchRow();
                            if (row != NULL && *row != NULL) {
                                int nFiled = 0;
                                if (pBlessInfoDB->m_vecRewards.size() >= BLESS_REWARD_LEVEL) {
                                    for (int i = 0; i < BLESS_REWARD_LEVEL; ++i) {
                                        pBlessInfoDB->m_vecRewards[i] = atoi(row[nFiled++]);
                                    }
                                }
                            }
                            SafeFreeResult();
                        }
                        nRes = Query_Res_OK;
                    }
                }
            }
        }
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::SaveFestivalWishInfo(CFestivalWishInfoDB & rWishInfoDB)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "call save_festival_wish(%u, %u, %u)", rWishInfoDB.m_nRoleID, rWishInfoDB.m_nWishCount, rWishInfoDB.m_nLastGetRewardTime);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadFestivalWishInfo(CFestivalWishInfoDB & rWishInfoDB)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "call load_festival_wish(%u, %u, %u, @wish_count, @last_get_reward_time)", rWishInfoDB.m_nRoleID, rWishInfoDB.m_nActivityStartTime, rWishInfoDB.m_nActivityEndTime);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
        SafeFreeResult();
        sprintf(sql, "select @wish_count, @last_get_reward_time");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                if (row != NULL && *row != NULL) {
                    int nFiled = 0;
                    rWishInfoDB.m_nWishCount = atoi(row[nFiled++]);
                    rWishInfoDB.m_nLastGetRewardTime = strtoul(row[nFiled], NULL, 10);
                }
                nRes = Query_Res_OK;
                SafeFreeResult();
            }
        }
    }

    return nRes;
}

int CDBInterface::AddBless(CFestivalBlessInfoDB & blessEntryDB)
{
    int nRes = Query_Res_Fail;

    if (blessEntryDB.m_nSenderID > 0 && blessEntryDB.m_nRcverID > 0) {
        sprintf(sql, "insert into festival_bless set senderid = %u, rcverid = %d, words_index = %u, bless_time = %u", blessEntryDB.m_nSenderID, blessEntryDB.m_nRcverID, blessEntryDB.m_nWordsIndex, blessEntryDB.m_nBlessTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::SaveRoleBlessActivityInfo(unsigned int nRoleID, vector<int> &vecReward, unsigned int nLastGetRewardTime)
{
    int nRes = Query_Res_Fail;
    if (nRoleID > 0 && vecReward.size() >= BLESS_REWARD_LEVEL) {
        sprintf(sql, "call save_festival_bless_reward(%u, %u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
            nRoleID, nLastGetRewardTime, vecReward[0], vecReward[1], vecReward[2], vecReward[3], vecReward[4], vecReward[5], vecReward[6], vecReward[7], vecReward[8], vecReward[9]);
    }

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadRedEnvelopeSenderInfo(RedEnvelopeSenderInfo & sender)
{
    sprintf(sql, "SELECT a.rolename, a.dance_group_id, b.count from role as a left join event as b on b.roleid = a.roleid and b.eventid = %d where a.roleid = %d", (int)eEvent_RedEnvelope_Anonymity, sender.m_nRoleID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            sender.m_strRoleName = row[nField++];
            sender.m_nDanceGroupID = strtoul(row[nField++], &pStrStop, 10);
            sender.m_bIsAnonymity = strtoul(row[nField++], &pStrStop, 10) > 0 ? true : false;
        }

        return true;
    }

    return false;
}

// 加载舞团红包
int CDBInterface::LoadDanceGroupRedEnvelope(std::map<unsigned int, RedEnvelope*> & mapRedEnvelope, int & nMaxid)
{
    std::map<unsigned int, std::set<unsigned int> > mapRoleName;
    sprintf(sql, "SELECT redenvelopeid,sendroleid,dancegroupid,currencytype,currencyvalue,detail,getdetail,createtime,anonymity from redenvelope where expirationtime = 0");
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            RedEnvelope * pRedEnvelope = new RedEnvelope();
            int nField = 0;
            pRedEnvelope->m_nRedEnvelopeID = strtoul(row[nField++], &pStrStop, 10);
            pRedEnvelope->m_nSendRoleID = strtoul(row[nField++], &pStrStop, 10);
            pRedEnvelope->m_nDanceGroupID = strtoul(row[nField++], &pStrStop, 10);
            pRedEnvelope->m_nCurrencyType = strtoul(row[nField++], &pStrStop, 10);
            pRedEnvelope->m_nCurrencyValue = strtoul(row[nField++], &pStrStop, 10);
            mapRoleName[pRedEnvelope->m_nRedEnvelopeID].insert(pRedEnvelope->m_nSendRoleID);

            std::string strDetail = row[nField++];
            Split(strDetail, std::string("|"), std::string(":"), pRedEnvelope->m_mapDetail);

            std::string strGetDetail = row[nField++];
            std::list<std::string> listGetDetail;
            Split(strGetDetail, std::string("|"), listGetDetail);
            std::list<std::string>::iterator it = listGetDetail.begin();
            for (; it != listGetDetail.end(); ++it)
            {
                std::vector<unsigned int> listRoleInfo;
                Split(*it, std::string(":"), listRoleInfo);

                if (listRoleInfo.size() == 3)
                {
                    OpenRoleInfo getroleinfo;
                    getroleinfo.m_nRoleID = listRoleInfo[0];
                    getroleinfo.m_nOpenIndex = (unsigned int)listRoleInfo[1];
                    getroleinfo.m_nOpenTime = listRoleInfo[2];

                    std::map<int, int>::iterator itr = pRedEnvelope->m_mapDetail.find(getroleinfo.m_nOpenIndex);
                    if (itr != pRedEnvelope->m_mapDetail.end())
                    {
                        getroleinfo.m_nCurrencyValue = itr->second;
                    }
                    mapRoleName[pRedEnvelope->m_nRedEnvelopeID].insert(getroleinfo.m_nRoleID);
                    pRedEnvelope->m_mapOpenRole[getroleinfo.m_nRoleID] = getroleinfo;
                }
            }

            pRedEnvelope->m_nCreateTime = strtoul(row[nField++], &pStrStop, 10);
            pRedEnvelope->m_bIsAnonymity = strtoul(row[nField++], &pStrStop, 10) > 0 ? true : false;
            mapRedEnvelope.insert(make_pair(pRedEnvelope->m_nRedEnvelopeID, pRedEnvelope));
        }
    }

    std::map<unsigned int, std::set<unsigned int> >::iterator it = mapRoleName.begin();
    std::string strAnonymityName = RedEnvelopeConfigManager::Instance().GetAnonymityName();
    for (; it != mapRoleName.end(); ++it)
    {
        std::set<unsigned int>::iterator itr = it->second.begin();
        for (; itr != it->second.end(); ++itr)
        {
            sprintf(sql, "SELECT rolename from role where roleid = %u", *itr);
            if (ExecSQL() && (row = FetchRow()) != NULL)
            {
                int nField = 0;
                std::string strRoleName = row[nField++];

                std::map<unsigned int, RedEnvelope*>::iterator itred = mapRedEnvelope.find(it->first);
                if (itred != mapRedEnvelope.end() && itred->second != NULL)
                {
                    std::map<unsigned int, OpenRoleInfo>::iterator itt = itred->second->m_mapOpenRole.find(*itr);
                    if (itt != itred->second->m_mapOpenRole.end())
                    {
                        itt->second.m_strRoleName = strRoleName;
                    }

                    if (itred->second->m_nSendRoleID == *itr)
                    {
                        if (!itred->second->m_bIsAnonymity)
                        {
                            itred->second->m_strSendRoleName = strRoleName;
                        }
                        else
                        {

                            itred->second->m_strSendRoleName = strAnonymityName;
                        }
                    }
                }
            }
        }
    }

    sprintf(sql, "select max(redenvelopeid) from redenvelope");
    if (ExecSQL() && (row = FetchRow()) != NULL)
    {
        int nField = 0;
        nMaxid = strtoul(row[nField++], &pStrStop, 10);
    }

    return Query_Res_OK;
}

int CDBInterface::InsertIntoRedEnvelope(RedEnvelope & redenvelope)
{
    std::string strDetail;
    ::ConcatToString(redenvelope.m_mapDetail, std::string("|"), std::string(":"), strDetail);

    std::string strGetDetail;
    std::list<std::string> listGetDetail;
    redenvelope.GetRoleOpenIndex(listGetDetail);
    ::ConcatToString(listGetDetail, std::string("|"), strGetDetail);

    sprintf(sql, "insert into redenvelope set redenvelopeid = %u, sendroleid = %u, dancegroupid = %u, currencytype = %u, currencyvalue = %u, detail = '%s', getdetail = '%s',"
        "createtime = %u, anonymity = %u", redenvelope.m_nRedEnvelopeID, redenvelope.m_nSendRoleID, redenvelope.m_nDanceGroupID, redenvelope.m_nCurrencyType, redenvelope.m_nCurrencyValue,
        strDetail.c_str(), strGetDetail.c_str(), redenvelope.m_nCreateTime, redenvelope.m_bIsAnonymity ? 1 : 0);
    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::UpdateRedEnvelope(RedEnvelope & redenvelope)
{
    std::string strGetDetail;
    std::list<std::string> listGetDetail;
    redenvelope.GetRoleOpenIndex(listGetDetail);
    ::ConcatToString(listGetDetail, std::string("|"), strGetDetail);

    sprintf(sql, "update redenvelope set getdetail = '%s', expirationtime = %u where redenvelopeid = %u", strGetDetail.c_str(), redenvelope.m_nExpirationTime, redenvelope.m_nRedEnvelopeID);
    if (ExecSQL())
    {
        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

//加载所有的舞团信息
int CDBInterface::LoadAllDanceGroup(CDanceGroupManager & rDGMgr)
{
    int nRes = Query_Res_Fail;
    time_t tNow = time(NULL);
    {
        //加载舞团基本信息
        sprintf(sql, "SELECT d.group_id, d.group_name, d.leader_id, r.rolename, d.level, "
            "d.color, d.badge, d.effect, d.res_a, d.res_b, d.res_c, d.honor, d.profile, d.announcement, "
            "d.create_time, d.can_request,d.dismiss_time,d.cancel_dismiss_time,d.last_change_group_name_time, "
            "d.title_name_1,d.title_name_2,d.title_name_3,d.title_name_4,d.title_name_5,d.tuanhui_effect_lock, "
            "d.star_level,d.current_power_value,d.day_packet,d.process_time,d.ceremony_times,d.ceremony_stamp, d.challenge_active "
            "FROM dance_group d LEFT JOIN role r on r.roleid = d.leader_id ORDER BY d.group_id");

        //sprintf(sql, "SELECT d.group_id, d.group_name, d.leader_id, r.rolename, d.level, d.color, d.badge, d.res_a, d.res_b, d.res_c, d.honor, d.profile, d.announcement, d.create_time, d.can_request,d.dismiss_time,d.cancel_dismiss_time,d.title_name_1,d.title_name_2,d.title_name_3,d.title_name_4,d.title_name_5 FROM dance_group d LEFT JOIN role r on r.roleid = d.leader_id ORDER BY d.honor desc, d.rank_update_time");
        if (ExecSQL()){
            nRes = Query_Res_Empty;
            while ((row = FetchRow()) != NULL)
            {
                CDanceGroupInfo * pDGInfo = new CDanceGroupInfo();
                int nField = 0;
                pDGInfo->m_DGBaseInfo.m_nGroupID = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_strGroupName = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nLeaderRoleID = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_strLeaderRoleName = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nLevel = (unsigned short)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nColor = (unsigned short)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nBadge = (unsigned short)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nEffect = (unsigned short)atoi(row[nField++]); //
                pDGInfo->m_DGBaseInfo.m_nResA = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nResB = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nResC = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nResA = MIN(pDGInfo->m_DGBaseInfo.m_nResA, cMAX_GROUP_RES_A);
                pDGInfo->m_DGBaseInfo.m_nResB = MIN(pDGInfo->m_DGBaseInfo.m_nResB, cMAX_GROUP_RES_B);
                pDGInfo->m_DGBaseInfo.m_nResC = MIN(pDGInfo->m_DGBaseInfo.m_nResC, cMAX_GROUP_RES_C);
                pDGInfo->m_DGBaseInfo.m_nHonor = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nHonor = MIN(pDGInfo->m_DGBaseInfo.m_nHonor, cMAX_DANCE_GROUP_HONOR);
                pDGInfo->m_DGBaseInfo.m_strProfile = row[nField++];
                pDGInfo->m_DGBaseInfo.m_strAnnouncement = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nCreateTime = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_bCanRequestEnter = (atoi(row[nField++]) != 0);
                pDGInfo->m_DGBaseInfo.m_nLastDismissTime = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nLastCancelDismissTime = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nLastChangeGroupNameTime = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nTitleName1 = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nTitleName2 = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nTitleName3 = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nTitleName4 = row[nField++];
                pDGInfo->m_DGBaseInfo.m_nTitleName5 = row[nField++];
                pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock = row[nField++]; //
                pDGInfo->m_DGActivityInfo.m_nStarLevel = (unsigned short)atoi(row[nField++]);
                pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue = (unsigned int)atoi(row[nField++]);
                pDGInfo->m_DGActivityInfo.m_nDayPacket = (unsigned short)atoi(row[nField++]);
                pDGInfo->m_DGActivityInfo.m_nProcessTime = (unsigned int)atol(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nCeremonyTimes = atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nCeremonyStamp = atoi(row[nField++]);
                pDGInfo->m_DGBaseInfo.m_nChallengeActive = (unsigned int)strtoul(row[nField++], NULL, 10);
                if (pDGInfo->m_DGActivityInfo.m_nProcessTime == 0)
                {
                    pDGInfo->m_DGActivityInfo.m_nProcessTime = (unsigned int)time(NULL);
                }

                if (pDGInfo->IsValid(tNow))
                {
                    NameDeny::Instance().FilterInvalidText(pDGInfo->m_DGBaseInfo.m_strGroupName);

                    if (!rDGMgr.AddDanceGroupInfo(pDGInfo))
                    {
                        WriteLog(LOGLEVEL_ERROR, "Error: CDBInterface::LoadAllDanceGroup. AddDanceGroupInfo failed. GroupID=%u",
                            pDGInfo->m_DGBaseInfo.m_nGroupID);

                        delete pDGInfo, pDGInfo = NULL;
                    }
                }
                else
                {
                    delete pDGInfo, pDGInfo = NULL;
                }

                nRes = Query_Res_OK;
            }
        }
        SafeFreeResult();
    }

    {
        //加载舞团成员信息
        sprintf(sql, "SELECT dg.dismiss_time,dg.cancel_dismiss_time, d.group_id, d.role_id, "
            "r.rolename, d.title, d.property, r.sex, r.level, d.contribution, d.total_contribution,p.lastlogouttime, v.level, v.expired_time, "
            "d.normal_count,d.money_count,d.mcoin_count,d.packet_unlock_list,d.shop_exchange_count,d.in_time,d.process_time "
            "FROM dance_group_member d LEFT JOIN dance_group dg ON dg.group_id = d.group_id LEFT JOIN role r ON r.roleid = d.role_id LEFT JOIN vip v ON d.role_id = v.roleid LEFT JOIN nsytaccountdb.player p ON p.account = r.account");

        if (ExecSQL()){
            nRes = Query_Res_Empty;
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                unsigned int nLastDismissTime = (unsigned int)atoi(row[nField++]);
                unsigned int nLastCancelDismissTime = (unsigned int)atoi(row[nField++]);
                if (nLastDismissTime > nLastCancelDismissTime && nLastDismissTime + cDANCE_GROUP_DISMISS_RETENTION_TIME <= (unsigned int)tNow)
                {
                    continue;
                }
                unsigned int nGroupID = (unsigned int)atoi(row[nField++]);
                CDanceGroupInfo * pDGInfo = rDGMgr.GetDanceGroupInfo(nGroupID);
                if (pDGInfo == NULL)
                {
                    WriteLog(LOGLEVEL_ERROR, "Error. CDBInterface::LoadAllDanceGroup. pDGInfo == NULL. GroupID(%u)", nGroupID);
                    continue;
                }

                CDanceGroupMember member;
                member.m_nRoleID = (unsigned int)atoi(row[nField++]);
                member.m_strRoleName = row[nField++];
                member.m_nTitle = (unsigned char)atoi(row[nField++]);
                member.m_nProperty = (unsigned char)atoi(row[nField++]);
                member.m_nSex = (unsigned char)atoi(row[nField++]);
                member.m_nLevel = (unsigned short)atoi(row[nField++]);
                member.m_nContribution = (unsigned int)atoi(row[nField++]);
                member.m_nContribution = MIN(member.m_nContribution, cMAX_ROLE_CONTRIBUTION);
                member.m_nTotalContribution = (unsigned int)atoi(row[nField++]);
                if (member.m_nTotalContribution == 0)
                {
                    // 继承之前的值
                    member.m_nTotalContribution = member.m_nContribution;
                }
                member.m_nTotalContribution = MIN(member.m_nTotalContribution, cMAX_ROLE_CONTRIBUTION);
                member.m_nLastOffLineTime = (unsigned int)atoi(row[nField++]);
                member.m_nState = EPlayerState_Offline;
                if (row[nField] != NULL && (*row[nField]) != '\0') {
                    member.m_nVIPLevel = (unsigned short)atoi(row[nField]);
                }
                ++nField;
                if (row[nField] != NULL && (*row[nField]) != '\0') {
                    member.m_bIsVIP = (long long)atoi64(row[nField]) > (long long)tNow;
                }
                nField++;
                //
                member.m_nNormalCount = (unsigned short)atoi(row[nField++]);
                member.m_nMoneyCount = (unsigned short)atoi(row[nField++]);
                member.m_nPointCount = (unsigned short)atoi(row[nField++]);
                member.m_strPacketUnlockList = row[nField++];

                string strShopExchangeCount = row[nField++];
                member.m_mShopExchangeCount.clear();
                if (!strShopExchangeCount.empty())
                {
                    TransferKeyValueList(strShopExchangeCount.c_str(), member.m_mShopExchangeCount);
                }

                member.m_nInTime = (unsigned int)atoi(row[nField++]);
                member.m_nLastProcessTime = (unsigned int)atol(row[nField++]);
                if (member.m_nLastProcessTime == 0)
                {
                    member.m_nLastProcessTime = (unsigned int)time(NULL);
                }

                NameDeny::Instance().FilterInvalidText(member.m_strRoleName);

                pDGInfo->AddMemberInfo(member);
                nRes = Query_Res_OK;
            }
        }
        SafeFreeResult();
    }
    {
        //加载申请加入的列表信息
        sprintf(sql, "SELECT d.group_id, d.role_id, r.rolename, r.sex, r.level, d.expired_time, v.level, v.expired_time "
            "FROM dance_group_request d LEFT JOIN role r ON r.roleid = d.role_id LEFT JOIN vip v ON d.role_id = v.roleid");

        if (ExecSQL()){
            nRes = Query_Res_Empty;
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                unsigned int	nGroupID = (unsigned int)atoi(row[nField++]);
                unsigned int	nRoleID = (unsigned int)atoi(row[nField++]);
                string			strRoleName = row[nField++];
                unsigned char	nSex = (unsigned char)atoi(row[nField++]);
                unsigned short	nLevel = (unsigned short)atoi(row[nField++]);
                unsigned int	nExpiredTime = (unsigned int)atoi(row[nField++]);
                bool bIsVIP = false;
                unsigned short nVIPLevel = 0;
                if (row[nField] != NULL && (*row[nField]) != '\0') {
                    nVIPLevel = (unsigned short)atoi(row[nField]);
                }
                ++nField;
                if (row[nField] != NULL && (*row[nField]) != '\0') {
                    bIsVIP = (long long)atoi64(row[nField]) > (long long)tNow;
                }

                NameDeny::Instance().FilterInvalidText(strRoleName);

                CRequestDanceGroupInfo requestDGInfo(nRoleID, strRoleName, nSex, nLevel, nExpiredTime, bIsVIP, nVIPLevel);
                rDGMgr.AddRequestDanceGroupInfo(nGroupID, requestDGInfo);
                nRes = Query_Res_OK;
            }
        }
        SafeFreeResult();

    }

    {
        time_t tTtmp = tNow - 1 * 24 * 60 * 60;
        //加载舞团记录
        sprintf(sql, "select d.id, d.group_id,d.role_id,d.content,d.type,d.record_time "
            "from dance_group_records d where d.record_time>=%u", (unsigned int)tTtmp);

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                unsigned int nId = (unsigned int)(atoi(row[nField++]));
                unsigned int nGroupId = (unsigned int)(atoi(row[nField++]));
                unsigned int nRoleId = (unsigned int)(atoi(row[nField++]));
                string strContent = row[nField++];
                /*unsigned int nType = */(void)(row[nField++]);
                unsigned int nRecordTime = (unsigned int)atoi(row[nField++]);

                //筛选出最近三天的记录
                int nDiffDay = DiffDays(tNow, (time_t)nRecordTime);
                if (nDiffDay < 3)
                {
                    nDiffDay++;
                    CDanceGroupRecordsInfo recordInfo(nGroupId, nRoleId, strContent, nRecordTime, (unsigned short)nDiffDay);
                    recordInfo.m_nId = nId;
                    rDGMgr.AddDanceGroupRecord(nGroupId, recordInfo);
                }
            }

        }
        SafeFreeResult();

    }//end

    // 加载退团信息：
    const int day5 = 5 * 24 * 60 * 60; // 只加载五天之内的退团信息
    Formatter::Format(sql, sizeof(sql), "SELECT roleid, quitGroupID, stamp FROM dance_group_quit_info WHERE stamp > %u",
        (unsigned int)time(NULL) - day5);
    do
    {
        if (!ExecSQL())
        {
            nRes = Query_Res_Fail;
            break;
        }

        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            DanceGroupQuitInfo quitInfo;

            quitInfo.m_nRoleID = atoi(row[nField++]);
            quitInfo.m_nDanceGroupID = atoi(row[nField++]);
            quitInfo.m_nQuitStamp = atoi(row[nField++]);

            rDGMgr.InsertRoleQuitInfo(quitInfo); // 添加这个退出消息
        }

        SafeFreeResult();
    } while (0);

    return nRes;
}

//创建舞团
int CDBInterface::CreateDanceGroup(CDanceGroupInfo & rDGInfo, int & rnResult)
{
    rnResult = ECreateDanceGroupResult_Error;
    int nRes = Query_Res_OK;
    char szSQLGroupName[cMAX_DANCE_GROUP_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLGroupName, rDGInfo.m_DGBaseInfo.m_strGroupName.c_str(), rDGInfo.m_DGBaseInfo.m_strGroupName.length());
    char szSQLProfile[cMAX_DANCE_GROUP_PROFILE_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLProfile, rDGInfo.m_DGBaseInfo.m_strProfile.c_str(), rDGInfo.m_DGBaseInfo.m_strProfile.length());
    if (Query_Res_OK == nRes)
    {
        sprintf(sql, "CALL create_dance_group('%s', %u, %u, '%s', %u, @group_id, @res)",
            szSQLGroupName, rDGInfo.m_DGBaseInfo.m_nLeaderRoleID, rDGInfo.m_DGBaseInfo.m_nBadge, szSQLProfile, rDGInfo.m_DGBaseInfo.m_nCreateTime);
        if (!ExecSQL())
        {
            nRes = Query_Res_Fail;
        }
        SafeFreeResult();
    }

    int nCreateResult = -1;
    if (Query_Res_OK == nRes)
    {
        sprintf(sql, "SELECT @group_id, @res");
        if (ExecSQL())
        {
            if (mysql_num_rows(result) > 0)
            {
                row = FetchRow();
                rDGInfo.m_DGBaseInfo.m_nGroupID = atoi(row[0]);
                nCreateResult = atoi(row[1]);
            }
        }
        else
        {
            nRes = Query_Res_Fail;
        }
        SafeFreeResult();
    }

    if (nCreateResult == 0)
    {
        rnResult = ECreateDanceGroupResult_Success;
    }
    else if (nCreateResult == 1)
    {
        rnResult = ECreateDanceGroupResult_NameDuplicated;
    }
    else if (nCreateResult == 2)
    {
        rnResult = ECreateDanceGroupResult_HaveAGroup;
    }

    return nRes;
}
//移除舞团
int CDBInterface::RemoveDanceGroup(unsigned int nGroupID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "CALL reset_role_dance_group(%u)", nGroupID);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}
//更新舞团基本信息
int CDBInterface::UpdateDanceGroupInfo(CDanceGroupBaseInfo & rDGBaseInfo)
{
    int nRes = Query_Res_Fail;
    char szSQLGroupName[cMAX_DANCE_GROUP_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLGroupName, rDGBaseInfo.m_strGroupName.c_str(), rDGBaseInfo.m_strGroupName.length());
    char szSQLProfile[cMAX_DANCE_GROUP_PROFILE_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLProfile, rDGBaseInfo.m_strProfile.c_str(), rDGBaseInfo.m_strProfile.length());
    char szSQLAnnouncement[cMAX_DANCE_GROUP_ANNOUNCEMENT_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLAnnouncement, rDGBaseInfo.m_strAnnouncement.c_str(), rDGBaseInfo.m_strAnnouncement.length());
    char szSQLTitleName1[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLTitleName1, rDGBaseInfo.m_nTitleName1.c_str(), rDGBaseInfo.m_nTitleName1.length());
    char szSQLTitleName2[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLTitleName2, rDGBaseInfo.m_nTitleName2.c_str(), rDGBaseInfo.m_nTitleName2.length());
    char szSQLTitleName3[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLTitleName3, rDGBaseInfo.m_nTitleName3.c_str(), rDGBaseInfo.m_nTitleName3.length());
    char szSQLTitleName4[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLTitleName4, rDGBaseInfo.m_nTitleName4.c_str(), rDGBaseInfo.m_nTitleName4.length());
    char szSQLTitleName5[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLTitleName5, rDGBaseInfo.m_nTitleName5.c_str(), rDGBaseInfo.m_nTitleName5.length());

    //解锁的特效Id信息
    char szTuanhuiEffectLock[cMAX_DANCE_GROUP_TITILE_NAME_LEN * 3 + 1] = { 0 };
    CDBIMysql::EscapeString(szTuanhuiEffectLock, rDGBaseInfo.m_strBadgeEffectLock.c_str(), rDGBaseInfo.m_strBadgeEffectLock.length());

    sprintf(sql, "UPDATE dance_group SET group_name='%s',leader_id=%u,level=%u,color=%u,"
        "badge=%u,effect=%u,res_a=%u,res_b=%u,res_c=%u,honor=%u,"
        "profile='%s',announcement='%s',can_request=%u,dismiss_time=%u,"
        "cancel_dismiss_time=%u, last_change_group_name_time=%u,"
        "title_name_1='%s',title_name_2='%s',title_name_3='%s',title_name_4='%s',"
        "title_name_5='%s' ,tuanhui_effect_lock='%s', "
        "ceremony_times=%u,ceremony_stamp=%u "
        "WHERE group_id=%u",
        szSQLGroupName, rDGBaseInfo.m_nLeaderRoleID, rDGBaseInfo.m_nLevel, rDGBaseInfo.m_nColor, rDGBaseInfo.m_nBadge, rDGBaseInfo.m_nEffect, rDGBaseInfo.m_nResA, rDGBaseInfo.m_nResB, rDGBaseInfo.m_nResC,
        rDGBaseInfo.m_nHonor, szSQLProfile, szSQLAnnouncement, rDGBaseInfo.m_bCanRequestEnter ? 1 : 0, rDGBaseInfo.m_nLastDismissTime, rDGBaseInfo.m_nLastCancelDismissTime, rDGBaseInfo.m_nLastChangeGroupNameTime,
        szSQLTitleName1, szSQLTitleName2, szSQLTitleName3, szSQLTitleName4, szSQLTitleName5, szTuanhuiEffectLock,
        rDGBaseInfo.m_nCeremonyTimes, rDGBaseInfo.m_nCeremonyStamp, rDGBaseInfo.m_nGroupID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}
//添加舞团成员
int CDBInterface::AddDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID, unsigned char nTitle, unsigned char nProperty, unsigned int nContribution, unsigned int nInTime)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "UPDATE role SET dance_group_id=%u WHERE roleid=%u", nGroupID, nRoleID);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    if (nRes == Query_Res_OK)
    {
        sprintf(sql, "INSERT INTO dance_group_member(group_id, role_id, title, property, contribution,in_time) VALUES(%u,%u,%u,%u,%u,%u)",
            nGroupID, nRoleID, nTitle, nProperty, nContribution, nInTime);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

//更新舞团成员信息
int CDBInterface::UpdateDanceGroupMember(unsigned int nGoupId, unsigned int nRoleId, CDanceGroupMember *pMember)
{
    int nRes = Query_Res_Fail;
    string strExchangeCount;
    TransferKeyValueStr(strExchangeCount, pMember->m_mShopExchangeCount);

    char szSqlUnlockList[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlUnlockList, pMember->m_strPacketUnlockList.c_str(), pMember->m_strPacketUnlockList.length());
    char szSqlExchangeCount[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlExchangeCount, strExchangeCount.c_str(), strExchangeCount.length());

    sprintf(sql, "UPDATE dance_group_member SET title=%u, contribution=%u, total_contribution=%u, property=%u,"
        "normal_count=%u,money_count=%u,mcoin_count=%u, packet_unlock_list='%s',shop_exchange_count='%s' "
        " WHERE group_id=%u AND role_id=%u",
        pMember->m_nTitle, pMember->m_nContribution, pMember->m_nTotalContribution, pMember->m_nProperty,
        pMember->m_nNormalCount, pMember->m_nMoneyCount, pMember->m_nPointCount,
        szSqlUnlockList, szSqlExchangeCount,
        nGoupId, nRoleId);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateDanceGroupMemberWithTime(unsigned int nGoupId, unsigned int nRoleId, CDanceGroupMember *pMember)
{
    int nRes = Query_Res_Fail;
    string strExchangeCount;
    TransferKeyValueStr(strExchangeCount, pMember->m_mShopExchangeCount);

    char szSqlUnlockList[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlUnlockList, pMember->m_strPacketUnlockList.c_str(), pMember->m_strPacketUnlockList.length());
    char szSqlExchangeCount[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlExchangeCount, strExchangeCount.c_str(), strExchangeCount.length());


    sprintf(sql, "UPDATE dance_group_member SET title=%u, contribution=%u, total_contribution=%u, property=%u,"
        "normal_count=%u,money_count=%u,mcoin_count=%u, packet_unlock_list='%s',shop_exchange_count='%s',process_time=%u "
        " WHERE group_id=%u AND role_id=%u",
        pMember->m_nTitle, pMember->m_nContribution, pMember->m_nTotalContribution, pMember->m_nProperty,
        pMember->m_nNormalCount, pMember->m_nMoneyCount, pMember->m_nPointCount,
        szSqlUnlockList, szSqlExchangeCount, (unsigned int)time(NULL),
        nGoupId, nRoleId);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

//删除舞团成员
int CDBInterface::DelDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID)
{
    int nRes = Query_Res_Fail;
    {
        sprintf(sql, "DELETE FROM dance_group_member WHERE group_id=%u AND role_id=%u", nGroupID, nRoleID);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }
    if (nRes == Query_Res_OK)
    {
        sprintf(sql, "UPDATE role set dance_group_id=0 WHERE roleid=%u", nRoleID);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }
    return nRes;
}
//更新舞团申请信息
int CDBInterface::UpdateRequestDanceGroup(unsigned int nGroupID, unsigned int nRoleID, unsigned int nExpiredTime)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "REPLACE INTO dance_group_request(group_id, role_id, expired_time) VALUES (%u,%u,%u)", nGroupID, nRoleID, nExpiredTime);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

//删除舞团申请信息

int CDBInterface::RemoveRequestDanceGroup(unsigned int nGroupID, const std::list<unsigned int> *pListRoleID)
{
    int nRes = Query_Res_Fail;

    if (NULL != pListRoleID && 0 != nGroupID)
    {
        std::stringstream ss;

        for (std::list<unsigned int>::const_iterator bIt = pListRoleID->begin(), eIt = pListRoleID->end();
            bIt != eIt && 0 != *bIt; ++bIt)
        {
            ss << *bIt << ",";
        }

        std::string strRoleID(ss.str());

        if (0 == strRoleID.size())
            return nRes;

        strRoleID.erase(strRoleID.find_last_not_of(',') + 1);

        char *end = sql;
        sprintf(sql, "DELETE FROM dance_group_request WHERE group_id = %u AND role_id IN (", nGroupID);
        end += strlen(sql);
        end += CDBIMysql::EscapeString(end, strRoleID.c_str(), strRoleID.size());
        strcat(end, ")");

        if (ExecSQL())
            nRes = Query_Res_OK;

        SafeFreeResult();
    }

    return nRes;
}

//修改舞团名称
int CDBInterface::ChangeDanceGroupName(unsigned int nGroupID, std::string & rstrNewName, unsigned int nChangeDanceGroupNameTime, int & rnResult)
{
    int nRes = Query_Res_Fail;
    char szSQLGroupName[cMAX_DANCE_GROUP_NAME_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLGroupName, rstrNewName.c_str(), rstrNewName.length());
    sprintf(sql, "call change_dance_group_name(%u, '%s', %u, @result)", nGroupID, szSQLGroupName, nChangeDanceGroupNameTime);
    int nChangeNameResult = -1;
    if (ExecSQL()) {
        SafeFreeResult();
        sprintf(sql, "SELECT @result");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                nChangeNameResult = atoi(row[0]);
            }
            nRes = Query_Res_OK;
        }
    }
    if (nChangeNameResult == 0) {
        rnResult = EChangeDanceGroupNameResult_Success;
    }
    else if (nChangeNameResult == 1) {
        rnResult = EChangeDanceGroupNameResult_NameDuplicated;
    }
    else if (nChangeNameResult == 2) {
        rnResult = EChangeDanceGroupNameResult_GroupNotExist;
    }
    else {
        rnResult = EChangeDanceGroupNameResult_Unkown;
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::UpdateDanceGroupActivityInfo(unsigned int nGroupId, CDanceGroupActivityInfo& rDGActivityInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "update dance_group set star_level=%d,current_power_value=%u,day_packet=%d,process_time=%u "
        "where group_id=%u",
        (int)rDGActivityInfo.m_nStarLevel, rDGActivityInfo.m_nCurrentPowerValue, (int)rDGActivityInfo.m_nDayPacket, rDGActivityInfo.m_nProcessTime,
        nGroupId);
    if (ExecSQL())
        nRes = Query_Res_OK;

    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddDanceGroupRecord(unsigned int nGoupid, CDanceGroupRecordsInfo& rRecordInfo)
{
    int nRes = Query_Res_Fail;

    char szSqlContent[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlContent, rRecordInfo.m_strContent.c_str(), rRecordInfo.m_strContent.length());

    sprintf(sql, "insert into dance_group_records(group_id,role_id,content,type,record_time) "
        "values( %u, %u, '%s', 0, %d)",
        rRecordInfo.m_nGroupId, rRecordInfo.m_nRoleId, szSqlContent, rRecordInfo.m_nRecordTime);

    if (ExecSQL())
        nRes = Query_Res_OK;

    SafeFreeResult();
    return nRes;
}//end

int CDBInterface::UpdateDanceGroupQuitInfo(unsigned int nRoleId, unsigned int nDanceGroupID, unsigned int stamp)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO dance_group_quit_info(roleid, quitGroupID, stamp) VALUE(%u,%u,%u) "
        "ON DUPLICATE KEY UPDATE quitGroupID = %u, stamp = %u", nRoleId, nDanceGroupID, stamp, nDanceGroupID, stamp);

    if (!ExecSQL())
        return Query_Res_Fail;

    SafeFreeResult();

    return Query_Res_OK;
}


int CDBInterface::CeremonyLoadLog(DanceGroupCeremonyLogMap &logMap)
{
    const int sevenDay = 7 * 24 * 60 * 60;
    unsigned int nStartLoadTime = (unsigned int)time(NULL);

    // 读取七日内内的仪式log
    Formatter::Format(sql, sizeof(sql), "SELECT id, groupID, creatorID, creatorName, creatorVip, logtime FROM ceremony_room_log WHERE logtime > %u",
        nStartLoadTime - sevenDay);
    do
    {
        ENSURE_WITH_LOG_CMD(ExecSQL(), break, "load cremony_room_log failed.");

        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            DanceGroupCeremonyLog groupLog;
            groupLog.m_roomBaseLog.m_nLogID = atoi(row[nField++]);
            unsigned int groupID = atoi(row[nField++]);
            groupLog.m_roomBaseLog.m_nCreatorID = atoi(row[nField++]);
            groupLog.m_roomBaseLog.m_strCreatorName = row[nField++];
            groupLog.m_roomBaseLog.m_nCreatorVip = (unsigned char)atoi(row[nField++]);
            groupLog.m_roomBaseLog.m_logTime = atoi(row[nField++]);

            DanceGroupCeremonyLogsProcessor::InsertCeremonyLog(groupID, groupLog, logMap);
        }

        SafeFreeResult();
    } while (0);

    // 针对每个log 加载所有的 投票数据
    for (DanceGroupCeremonyLogMap::iterator iter = logMap.begin(); iter != logMap.end(); ++iter)
    {
        DanceGroupCeremonyLogs &groupLogs = iter->second;
        // 遍历vecLog:
        for (std::vector<DanceGroupCeremonyLog>::iterator iterVoter = groupLogs.m_vecLog.begin();
            iterVoter != groupLogs.m_vecLog.end(); ++iterVoter)
        {
            CeremonyLoadPraiseInfo(iterVoter->m_roomBaseLog.m_nLogID, *iterVoter);
        }
    }

    WriteLog(LOGLEVEL_DEBUG, "load cremony log ok, use time:%u", (unsigned int)(time(NULL) - nStartLoadTime));

    return Query_Res_OK;
}

int CDBInterface::CeremonyLoadPraiseInfo(unsigned int logID, DanceGroupCeremonyLog &roomLog)
{
    Formatter::Format(sql, sizeof(sql), "SELECT rookieID, praiserID, rookieName, rookieVip, praiserVip, praiserName, praiserCount, praiserTime "
        "FROM ceremony_room_praise_log WHERE id = %u", logID);
    do
    {
        ENSURE_WITH_LOG_CMD(ExecSQL(), break, "load cremony_room_log[%u] item failed.", logID);

        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CeremonyPraiseLogInfoDb info;
            info.m_nLogID = logID;
            info.m_newerInfo.m_nRoleID = atoi(row[nField++]);
            info.m_voterInfo.m_nPraiserID = atoi(row[nField++]);
            info.m_newerInfo.m_strRoleName = row[nField++];
            info.m_newerInfo.m_nVipLv = (unsigned char)atoi(row[nField++]);
            info.m_voterInfo.m_nPraiserVipLv = (unsigned char)atoi(row[nField++]);
            info.m_voterInfo.m_strPraiserName = row[nField++];
            info.m_voterInfo.m_nPraiserCount = (unsigned short)atoi(row[nField++]);
            info.m_voterInfo.m_logTime = atoi(row[nField++]);

            DanceGroupCeremonyLogsProcessor::InsertCeremonyPraiseItem(roomLog, info);
        }

        SafeFreeResult();

    } while (0);

    return Query_Res_OK;
}


// 入团仪式写入日志
int CDBInterface::CeremonyWriteLog(DanceGroupCeremonyLog &groupLog, unsigned int nGroupID)
{
    char escape[MAX_NAME_LEN] = { 0 }; // 名字最大长度

    // 插入房间日志和投票数据
    CeremonyLogRoomInfo &ri = groupLog.m_roomBaseLog;

    std::string strEscapeCreatorName;
    CDBIMysql::EscapeString(escape, ri.m_strCreatorName.c_str(), ri.m_strCreatorName.length());
    strEscapeCreatorName = escape;

    Formatter::Format(sql, sizeof(sql), "INSERT INTO ceremony_room_log(groupID, creatorID, creatorName, creatorVip, logtime) "
        "VALUE(%u,%u,'%s',%u,%u)", nGroupID, ri.m_nCreatorID, strEscapeCreatorName.c_str(),
        ri.m_nCreatorVip, ri.m_logTime);

    if (!ExecSQL())
        return Query_Res_Fail;
    SafeFreeResult();

    Formatter::Format(sql, sizeof(sql), "SELECT LAST_INSERT_ID()");
    if (!ExecSQL())
        return Query_Res_Fail;

    row = FetchRow();
    if (NULL != row)
    {
        groupLog.m_roomBaseLog.m_nLogID = atoi(row[0]); // 取得这个ID
    }
    else
    {
        return Query_Res_Fail;
    }
    SafeFreeResult();

    // 写入投票记录
    std::string strEscapeNewerName, strEscapePraiserName;
    for (std::vector<CeremonyPraiseLogInfo>::iterator iterP = groupLog.m_voteInfo.begin(); iterP != groupLog.m_voteInfo.end(); ++iterP)
    {
        CDBIMysql::EscapeString(escape, iterP->m_newerInfo.m_strRoleName.c_str(), iterP->m_newerInfo.m_strRoleName.length());
        strEscapeNewerName = escape;
        std::vector<CeremonyLogVoterInfo> &vecVoterInfo = iterP->m_vecVoterInfo;
        if (vecVoterInfo.empty())
        { // 没人给他投票：写入一条0的投票记录
            Formatter::Format(sql, sizeof(sql), "INSERT INTO ceremony_room_praise_log"
                "(id, rookieID, praiserID, rookieName, rookieVip, praiserVip, praiserName, praiserCount, praiserTime) "
                "VALUE(%u, %u, 0, '%s', %u, 0, '', 0, 0)",
                groupLog.m_roomBaseLog.m_nLogID, iterP->m_newerInfo.m_nRoleID,
                strEscapeNewerName.c_str(), iterP->m_newerInfo.m_nVipLv);

            ExecSQL();
            SafeFreeResult();
            continue;
        }
        for (std::vector<CeremonyLogVoterInfo>::iterator iter = vecVoterInfo.begin(); iter != vecVoterInfo.end(); ++iter)
        {
            CDBIMysql::EscapeString(escape, iter->m_strPraiserName.c_str(), iter->m_strPraiserName.length());
            strEscapePraiserName = escape;
            Formatter::Format(sql, sizeof(sql), "INSERT INTO ceremony_room_praise_log"
                "(id, rookieID, praiserID, rookieName, rookieVip, praiserVip, praiserName, praiserCount, praiserTime) "
                "VALUE(%u, %u, %u, '%s', %u, %u, '%s', %u, %u)",
                groupLog.m_roomBaseLog.m_nLogID,
                iterP->m_newerInfo.m_nRoleID, iter->m_nPraiserID,
                strEscapeNewerName.c_str(), iterP->m_newerInfo.m_nVipLv, iter->m_nPraiserVipLv, strEscapePraiserName.c_str(),
                iter->m_nPraiserCount, iter->m_logTime);

            ExecSQL();
            SafeFreeResult();
        }

    }

    return Query_Res_OK;
}


int CDBInterface::LoadBigMamaGroupInfo(DanceGroupBigMamaMap &bigMamaMap)
{
    Formatter::Format(sql, sizeof(sql), "SELECT dancegroupID, dance_round, stageID, bigmama_state, "
        "kill_type, killerRole, killerName, stamp FROM bigmama_dancegroup_info");

    if (!ExecSQL())
        return Query_Res_Fail;

    while (NULL != (row = FetchRow()))
    {
        int field = 0;
        DanceGroupBigMamaInfo dbmi;

        dbmi.m_nDanceGroupID = atoi(row[field++]);
        dbmi.m_nCurrentRound = (unsigned char)atoi(row[field++]);
        dbmi.m_nCurrentStageID = atoi(row[field++]);
        dbmi.m_nState = (EBigMamaActiveState)atoi(row[field++]);
        dbmi.m_nBeaterType = (EBigMamaLastBeatType)atoi(row[field++]);
        dbmi.m_nBeaterRoleID = atoi(row[field++]);
        dbmi.m_strBeaterName = row[field++];
        dbmi.m_nStamp = atoi(row[field++]);
        
        bigMamaMap.insert(std::make_pair(dbmi.m_nDanceGroupID, dbmi));
    }

    return Query_Res_OK;

}

int CDBInterface::UpdateOrInsertBigMamaGroupInfo(const DanceGroupBigMamaInfo &dbmi)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO bigmama_dancegroup_info(dancegroupID, dance_round, stageID, bigmama_state, "
        "kill_type, killerRole, killerName, stamp) "
        "VALUE(%u, %d, %u, %d, %d, %u, '%s', %u) "
        "ON DUPLICATE KEY UPDATE dance_round = %u, stageID = %d, bigmama_state = %d, "
        "kill_type = %d, killerRole = %u, killerName = '%s', stamp = %u", 
        dbmi.m_nDanceGroupID, (int)dbmi.m_nCurrentRound, dbmi.m_nCurrentStageID, (int)dbmi.m_nState, 
        (int)dbmi.m_nBeaterType, dbmi.m_nBeaterRoleID, dbmi.m_strBeaterName.c_str(), dbmi.m_nStamp,
        (int)dbmi.m_nCurrentRound, dbmi.m_nCurrentStageID, (int)dbmi.m_nState,
        (int)dbmi.m_nBeaterType, dbmi.m_nBeaterRoleID, dbmi.m_strBeaterName.c_str(), dbmi.m_nStamp);

    if (!ExecSQL())
        return Query_Res_Fail;

    return Query_Res_OK;
}

int CDBInterface::LoadBigMamaRoleInfo(unsigned int nRoleID, unsigned int &nPrizeStamp, unsigned int &nJoinStamp) // 
{
    Formatter::Format(sql, sizeof(sql), "SELECT prize_stamp, join_stamp FROM bigmama_role_info WHERE roleid = %u", nRoleID);

    if (!ExecSQL())
        return Query_Res_Fail;

    if (NULL != (row = FetchRow()))
    {
        nPrizeStamp = atoi(row[0]);
        nJoinStamp = atoi(row[1]);
    }

    return Query_Res_OK;
}

int CDBInterface::UpdateOrInsertBigMamaRoleInfo(unsigned int nRoleID, unsigned int nPrizeStamp, unsigned int nJoinStamp)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO bigmama_role_info(roleid, prize_stamp, join_stamp) "
        "VALUE(%u, %u, %u) "
        "ON DUPLICATE KEY UPDATE prize_stamp=%u,join_stamp=%u",
        nRoleID, nPrizeStamp, nJoinStamp, nPrizeStamp, nJoinStamp);

    if (!ExecSQL())
        return Query_Res_Fail;

    return Query_Res_OK;
}


int CDBInterface::DanceGroupChallengeLoadRanking(unsigned int nRoleID,
    unsigned int nStartTime, std::map<int, DanceGroupChallengeRanking> &rRanking)
{
    sprintf(sql, "select storey, ranking from group_challenge_ranking "
        "where starttime = %u and roleid = %u and state = %d", nStartTime,
        nRoleID, EDanceGroupChallengeRankingState_None);

    if (ExecSQL())
    {
        int nStorey = 0;
        short nRanking = 0;

        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            nStorey = atoi(row[nField++]);
            nRanking = (short)atoi(row[nField++]);

            DanceGroupChallengeRanking ranking(nStorey, nRanking);
            rRanking.insert(std::make_pair(ranking.GetStorey(), ranking));
        }

        return Query_Res_OK;
    }

    return Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeUpdateRankingState(unsigned int nRoleID,
    unsigned int nStartTime, std::map<int, DanceGroupChallengeRanking> &rRanking)
{
    if (rRanking.empty())
        return Query_Res_Fail;

    std::string strAppend;
    Formatter formator(strAppend);

    for (std::map<int, DanceGroupChallengeRanking>::const_iterator itr = rRanking.begin();
        itr != rRanking.end(); ++itr)
    {
        if (itr == rRanking.begin())
        {
            formator << itr->first;
        }
        else
        {
            formator << ", " << itr->first;
        }
    }

    sprintf(sql, "update group_challenge_ranking set state = %d "
        "where starttime = %u and roleid = %u and storey in (%s)",
        rRanking.begin()->second.GetState(), nStartTime, nRoleID, strAppend.c_str());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeRecordRanking(const DanceGroupChallengeRecordRankingInfo &rRanking)
{
    std::string strAppend;
    Formatter formator(strAppend);

    short nRanking = 0;
    unsigned int nStartTime = rRanking.GetStartTime();
    int nStorey = rRanking.GetStorey();

    const std::list<unsigned int> &rAllRoleID = rRanking.GetAllRoleID();
    for (std::list<unsigned int>::const_iterator itr = rAllRoleID.begin();
        itr != rAllRoleID.end(); ++itr)
    {
        if (itr == rAllRoleID.begin())
        {
            formator << "(";
        }
        else
        {
            formator << ", (";
        }

        formator << *itr << ", " << nStartTime << ", " << nStorey << ", " << ++nRanking << ")";
    }

    sprintf(sql, "insert into group_challenge_ranking(roleid, starttime, storey, ranking) values %s", strAppend.c_str());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeLoadAllInfo(unsigned int nStartTime,
    std::map<unsigned int, DanceGroupChallengeInfo *> &rmapInfo)
{
    sprintf(sql, "select groupid, storey, scene, music, "
        "musicmode, musiclevel, targetscore, currentscore "
        "from group_challenge_info where starttime = %u", nStartTime);

    if (!ExecSQL())
        return Query_Res_Fail;

    int nField = 0;
    unsigned int nGroupID = 0;
    DanceGroupChallengeInfo *pInfo = NULL;

    while ((row = FetchRow()) != NULL)
    {
        nField = 0;
        DanceGroupChallengeOpendStoreyInfo *pOpendInfo = new DanceGroupChallengeOpendStoreyInfo;

        nGroupID = (unsigned int)strtoul(row[nField++], NULL, 10);
        pOpendInfo->SetStorey(atoi(row[nField++]));
        pOpendInfo->SetScene(atoi(row[nField++]));
        pOpendInfo->SetMusic(atoi(row[nField++]));
        pOpendInfo->SetMusicMode(atoi(row[nField++]));
        pOpendInfo->SetMusicLevel(atoi(row[nField++]));
        pOpendInfo->SetTargetScore((unsigned int)strtoul(row[nField++], NULL, 10));
        pOpendInfo->SetCurrentScore((unsigned int)strtoul(row[nField++], NULL, 10));

        pInfo = rmapInfo[nGroupID];
        if (pInfo == NULL)
        {
            pInfo = new DanceGroupChallengeInfo(nGroupID, nStartTime);
            rmapInfo[nGroupID] = pInfo;
        }

        pInfo->AddOpendStoreyInfo(pOpendInfo);
    }

    sprintf(sql, "select groupid, storey, b.roleid, b.rolename, score, inspire "
        "from group_challenge_contribution a left join role b on a.roleid = b.roleid "
        "where starttime = %u", nStartTime);

    if (!ExecSQL())
        return Query_Res_Fail;

    int nStorey = 0;
    std::string strName;
    unsigned int nRoleID = 0, nScore = 0, nInspire = 0;
    DanceGroupChallengeContribution *pContribution = NULL;

    while ((row = FetchRow()) != NULL)
    {
        nField = 0;

        nGroupID = (unsigned int)strtoul(row[nField++], NULL, 10);

        pInfo = rmapInfo[nGroupID];
        if (pInfo == NULL)
        {
            WriteLog(LOGLEVEL_DEBUG, "Can not find dance group challenge opend storey info.");
            continue;
        }

        nStorey = atoi(row[nField++]);
        nRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
        strName = row[nField++];
        nScore = (unsigned int)strtoul(row[nField++], NULL, 10);
        nInspire = (unsigned int)strtoul(row[nField++], NULL, 10);

        pContribution = pInfo->GetStoreyContribution(nStorey);
        if (pContribution == NULL)
        {
            pContribution = new DanceGroupChallengeContribution(nStorey);
            pInfo->AddStoreyContribution(pContribution);
        }

        pContribution->AddRoleScore(nRoleID, strName, nScore);
        pContribution->AddRoleInspire(nRoleID, strName, nInspire);
    }

    return Query_Res_OK;
}

int CDBInterface::DanceGroupChallengeUpdateActive(unsigned int nGroupID, unsigned int nActive)
{
    sprintf(sql, "update dance_group set challenge_active = %u where group_id = %u", nActive, nGroupID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeUpdateContribution(const DanceGroupChallengeUpdateRoleContribution &rInfo)
{
    sprintf(sql, "select logid from group_challenge_contribution "
        "where starttime = %u and groupid = %u and storey = %d and roleid = %u",
        rInfo.GetStartTime(), rInfo.GetGroupID(), rInfo.GetStorey(), rInfo.GetRoleID());

    if (ExecSQL())
    {
        std::string strSql;
        Formatter formator(strSql);

        if ((row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
        {
            if (rInfo.GetType() == EDanceGroupChallengeUpdateRoleContributionType_Score)
            {
                formator << "score = " << rInfo.GetScore();
            }
            else if (rInfo.GetType() == EDanceGroupChallengeUpdateRoleContributionType_Inspire)
            {
                formator << "inspire = " << rInfo.GetInspire();
            }
            else
            {
                formator << "score = " << rInfo.GetScore() << ", inspire = " << rInfo.GetInspire();
            }

            sprintf(sql, "update group_challenge_contribution set %s "
                "where logid = %s", strSql.c_str(), row[0]);
        }
        else
        {
            if (rInfo.GetType() == EDanceGroupChallengeUpdateRoleContributionType_Score)
            {
                formator << rInfo.GetScore() << ", 0";
            }
            else if (rInfo.GetType() == EDanceGroupChallengeUpdateRoleContributionType_Inspire)
            {
                formator << "0, " << rInfo.GetInspire();
            }
            else
            {
                formator << rInfo.GetScore() << ", " << rInfo.GetInspire();
            }

            sprintf(sql, "insert into group_challenge_contribution(groupid, starttime, "
                "storey, roleid, score, inspire) values(%u, %u, %d, %u, %s)",
                rInfo.GetGroupID(), rInfo.GetStartTime(), rInfo.GetStorey(),
                rInfo.GetRoleID(), strSql.c_str());
        }

        if (ExecSQL())
        {
            return Query_Res_OK;
        }
    }

    return Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeOpenStorey(unsigned int nGroupID,
    unsigned int nStartTime, const DanceGroupChallengeOpendStoreyInfo &rOpendInfo)
{
    sprintf(sql, "insert into group_challenge_info(groupid, starttime, storey, scene, "
        "music, musicmode, musiclevel, targetscore) values(%u, %u, %d, %d, %d, %d, %d, %u)",
        nGroupID, nStartTime, rOpendInfo.GetStorey(), rOpendInfo.GetScene(), rOpendInfo.GetMusic(),
        rOpendInfo.GetMusicMode(), rOpendInfo.GetMusicLevel(), rOpendInfo.GetTargetScore());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupChallengeUpdateStoreyScore(unsigned int nGroupID,
    unsigned int nStartTime, const DanceGroupChallengeOpendStoreyInfo &rOpendInfo)
{
    sprintf(sql, "update group_challenge_info set currentscore = %u "
        "where starttime = %u and groupid = %u and storey = %d",
        rOpendInfo.GetCurrentScore(), nStartTime, nGroupID, rOpendInfo.GetStorey());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}
//舞团秘境
int CDBInterface::DanceGroupFairyLandLoadAllInfo(std::map<int, DanceGroupFairyLandGroupInfo>& rGinfo, 
    std::map<int, std::map<int, DanceGroupFairyLandPersonalInfo > >& rGPinfo,
    time_t &newDayClearStamp, time_t &passRewardStamp, time_t &storgeRewardStamp)
{
    sprintf(sql, "select group_id, cur_chapter, cur_score, today_rank, yestoday_rank, blass_rank, blessbuff_endtime, bless_lastName, bless_lastroleID "
        "from fairyland_groupinfo");

    if (!ExecSQL())
        return Query_Res_Fail;

    int nField = 0;
    while ((row = FetchRow()) != NULL)
    {
        nField = 0;
        DanceGroupFairyLandGroupInfo GInfo;

        GInfo.SetGroupId(atoi(row[nField++]));
        GInfo.SetCurChapterLvl(atoi(row[nField++]));
        GInfo.SetTotalScore(atoi(row[nField++]));
        GInfo.ParseScoreTodayRank(row[nField++]);
        GInfo.ParseScoreYesterdayRank(row[nField++]);
        GInfo.ParseBlessRank(row[nField++]);
        GInfo.SetBlessBuffEndTimeStamp(atoi(row[nField++]));
        std::string strName = row[nField++];
        unsigned int nRoleID = atoi(row[nField++]);
        GInfo.SetLastBlessRole(nRoleID, strName);

        rGinfo.insert(std::make_pair(GInfo.GetGroupId(), GInfo));
    }

    sprintf(sql, "select groupid, roleid, total_score, blass_times ,challenge_time, "
        "last_challenge_chapterlvl, passed_reward, storagebox, join_reward_flag "
        "from fairyland_personalinfo");

    if (!ExecSQL())
        return Query_Res_Fail;

    while ((row = FetchRow()) != NULL)
    {
        nField = 0;
        int nGroupId = atoi(row[nField++]);

        DanceGroupFairyLandPersonalInfo Pinfo;
        Pinfo.SetRoleId(atoi(row[nField++]));
        Pinfo.AddTotalScore(atoi(row[nField++]));
        Pinfo.AddBlassTimes(atoi(row[nField++]));
        Pinfo.SetCurChapterChallengeTimes(atoi(row[nField++]));
        Pinfo.SetLastChallengeChapterLvl(atoi(row[nField++]));

        Pinfo.ParsePassedReward(row[nField++]);
        Pinfo.ParseStorageBox(row[nField++]);
        Pinfo.ParseRewardFlag(row[nField++], Pinfo.m_nSex, Pinfo.m_setJoinRewardFalg);
//         Pinfo.ParseRewardFlag(row[nField++], Pinfo.m_nSex, Pinfo.m_setPassRewardFalg);

        std::map<int, std::map<int, DanceGroupFairyLandPersonalInfo > >::iterator it;
        it = rGPinfo.find(nGroupId);
        if (it == rGPinfo.end())
        {
            std::map<int, DanceGroupFairyLandPersonalInfo> Pdata;
            Pdata.insert(std::make_pair(Pinfo.GetRoleId(), Pinfo));
            rGPinfo.insert(std::make_pair(nGroupId, Pdata));
        }
        else
        {
            it->second[Pinfo.GetRoleId()] = Pinfo;
        }
    }

    newDayClearStamp = atoi(LoadSystemState(SYSTEM_TABLE_ID_FAIRYLAND_DAY_CLEAN).c_str());
    passRewardStamp = atoi(LoadSystemState(SYSTEM_TABLE_ID_FAIRYLAND_PASSREWARD).c_str());
    storgeRewardStamp = atoi(LoadSystemState(SYSTEM_TABLE_ID_FAIRYLAND_STORGEREWARD).c_str());

    return Query_Res_OK;
}


int CDBInterface::DanceGroupFairyLandNewDay(int nDefalutChapter)
{
    sprintf(sql, "call fairyland_new_day(%d)", nDefalutChapter); // 初始化为第一关
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateStampInfo(time_t nTimeStamp)
{ // 三个时间戳写入时间一致，但是清理时间不一致，分别代表不同的三个功能的完成情况
    UpdateSystemState(SYSTEM_TABLE_ID_FAIRYLAND_DAY_CLEAN, nTimeStamp);
    UpdateSystemState(SYSTEM_TABLE_ID_FAIRYLAND_PASSREWARD, nTimeStamp);
    UpdateSystemState(SYSTEM_TABLE_ID_FAIRYLAND_STORGEREWARD, nTimeStamp);

    return Query_Res_OK;
}

int CDBInterface::DanceGroupFairyLandUpdateStampInfo(int nType, time_t nTimeStamp)
{
    UpdateSystemState(nType, nTimeStamp);
    return Query_Res_OK;
}


int CDBInterface::DanceGroupFairyLandDelGroupInfo(int nGroupId)
{
    sprintf(sql, "DELETE FROM fairyland_groupinfo WHERE group_id = %d", nGroupId);

    if (!ExecSQL())
        return Query_Res_Fail;

    sprintf(sql, "DELETE FROM fairyland_personalinfo WHERE groupid = %d", nGroupId);

    if (!ExecSQL())
        return Query_Res_Fail;

    return Query_Res_OK;
}

int CDBInterface::DanceGroupFairyLandAddGroupInfo(int nGroupId, DanceGroupFairyLandGroupInfo& rGinfo)
{
    sprintf(sql, "INSERT INTO fairyland_groupinfo SET group_id = %d, cur_chapter = %d , "
        "cur_score = %d, today_rank = '%s' , yestoday_rank = '%s', blass_rank = '%s', blessbuff_endtime = %d",rGinfo.GetGroupId(),
        rGinfo.GetCurChapterLvl(), rGinfo.GetTotalScore(), rGinfo.ScoreTodayRankToJson().c_str(), \
        rGinfo.ScoreYesterdayToJson().c_str(), rGinfo.BlessRankToJson().c_str(), rGinfo.GetBlessBuffEndTimeStamp());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandDelPersonalInfo(int nGroupId, int nRoleId)
{
    sprintf(sql, "DELETE FROM fairyland_personalinfo WHERE groupid = %d AND roleid = %d", nGroupId, nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandAddPersonalInfo(int nGroupId, int nRoleId)
{
    sprintf(sql, "INSERT INTO fairyland_personalinfo SET roleid = %d, groupid = %d", nRoleId, nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateGroupChapter(int nGroupId, int nChapter)
{
    sprintf(sql, "UPDATE fairyland_groupinfo SET cur_chapter = %d WHERE group_id = %d", nChapter, nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateBlassRank(int nGroupId, std::string &rRankJsonFmt)
{
    CDBIMysql::EscapeString(sql, rRankJsonFmt.c_str(), rRankJsonFmt.length());
    std::string strEscapeRank = sql;

    sprintf(sql, "UPDATE fairyland_groupinfo SET blass_rank = '%s' WHERE group_id = %d", strEscapeRank.c_str(), nGroupId );
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateTodayRank(int nGroupId, std::string &rRankJsonFmt)
{
    CDBIMysql::EscapeString(sql, rRankJsonFmt.c_str(), rRankJsonFmt.length());
    std::string strEscapeRank = sql;

    sprintf(sql, "UPDATE fairyland_groupinfo SET today_rank = '%s' WHERE group_id = %d", strEscapeRank.c_str(), nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateBlassEndTime(int nGroupId, int nEndTime, unsigned int nLastRoleID, const std::string &strRoleName)
{
    CDBIMysql::EscapeString(sql, strRoleName.c_str(), strRoleName.length());
    std::string strEscapeLastName = sql;

    sprintf(sql, "UPDATE fairyland_groupinfo SET blessbuff_endtime = %d, bless_lastName = '%s', bless_lastroleID = %u WHERE group_id = %d", 
        nEndTime, strEscapeLastName.c_str(), nLastRoleID, nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdatePersonalTotalScore(int nRoleId, int nTotalScore)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET total_score = %d WHERE roleid = %d", nTotalScore, nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandClearYesterdayScore(int nRoleId)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET yesterday_total_score = 0 WHERE roleid = %d", nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdatePersonaBlassTimes(int nRoleId, int nTotalTimes)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET blass_times = %d WHERE roleid = %d", nTotalTimes, nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateChallengeTimes(int nRoleId, int nLastChapterLev, int nChallengeTimes)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET last_challenge_chapterlvl = %d, challenge_time = %d WHERE roleid = %d", nLastChapterLev, nChallengeTimes, nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateLastChallengeLvl(int nRoleid, int nchapterlvl)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET last_challenge_chapterlvl = %d WHERE roleid = %d", nchapterlvl, nRoleid);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateJoinRewardFlag(int nRoleId, std::string & str)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET join_reward_flag = '%s' WHERE roleid = %d", str.c_str(), nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdatePassedRewardFlag(int nRoleId, std::string & str)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET pass_reward_flag = '%s' WHERE roleid = %d", str.c_str(), nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdatePassedRewardRecord(int nRoleId, std::string & strRewardRecord)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET passed_reward = '%s' WHERE roleid = %u", strRewardRecord.c_str(), nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateStotageBox(int nRoleId, std::string & strStorageBox)
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET storagebox = '%s' WHERE roleid = %d", strStorageBox.c_str(), nRoleId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateGroupScore(int nGroupId, int nScore)
{
    sprintf(sql, "UPDATE fairyland_groupinfo SET cur_score = %d WHERE group_id = %d", nScore, nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandUpdateBlessEndTime(int nGroupId, int nEndTime)
{
    sprintf(sql, "UPDATE fairyland_groupinfo SET blessbuff_endtime = %d WHERE group_id = %d", nEndTime, nGroupId);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandClearAllPassedReward()
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET join_reward_flag = '', passed_reward=''");
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DanceGroupFairyLandClearAllStorageBoxReward()
{
    sprintf(sql, "UPDATE fairyland_personalinfo SET storagebox = ''");
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

std::string CDBInterface::LoadSystemState(int nID)
{
    std::string strRet = "";
    sprintf(sql, "SELECT json_value FROM system_state where id = %d", nID); // 

    if (!ExecSQL())
        return strRet;

    if ((row = FetchRow()) != NULL)
    {
        strRet = row[0];
    }

    return strRet;
}

int CDBInterface::UpdateSystemState(int nID, int nValue)
{
    sprintf(sql, "INSERT INTO system_state(id, json_value) VALUE(%d, '%d') ON DUPLICATE KEY UPDATE json_value = '%d'",
        nID, nValue, nValue);
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateSystemState(int nID, std::string strValue)
{
    sprintf(sql, "INSERT INTO system_state(id, json_value) VALUE(%d, '%s') ON DUPLICATE KEY UPDATE json_value = '%s'",
        nID, strValue.c_str(), strValue.c_str());
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}


//外部发送邮件
int CDBInterface::ExternalSendMail(ExternalSendMailInfo & rESMInfo)
{
    rESMInfo.m_nReceiveID = 0;

    char *pszTitle = new char[rESMInfo.m_strTitle.size() * 2];
    char *pszContent = new char[rESMInfo.m_strContent.size() * 2];

    if (NULL == pszTitle || NULL == pszContent)
        return Query_Res_Fail;

    char szSQLOrderID[65] = { 0 };
    CDBIMysql::EscapeString(szSQLOrderID, rESMInfo.m_strOrderID.c_str(), rESMInfo.m_strOrderID.length());
    CDBIMysql::EscapeString(pszTitle, rESMInfo.m_strTitle.c_str(), rESMInfo.m_strTitle.size());
    CDBIMysql::EscapeString(pszContent, rESMInfo.m_strContent.c_str(), rESMInfo.m_strContent.length());
    char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLPUID, rESMInfo.m_strPUID.c_str(), rESMInfo.m_strPUID.length());

    sprintf(sql, "SELECT receiveid FROM ex_sendmail_log WHERE pid = %u AND puid = '%s' AND order_id='%s'",
        rESMInfo.m_nPID, szSQLPUID, szSQLOrderID);

    if (ExecSQL()){
        if ((row = FetchRow()) != NULL){
            SafeFreeResult();
            return Query_Res_Fail;
        }
    }
    SafeFreeResult();

    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT roleid FROM role r, nsytaccountdb.player p WHERE p.pid=%u AND p.puid='%s' AND r.account=p.account;", rESMInfo.m_nPID, szSQLPUID);
    if (ExecSQL()) {
        row = FetchRow();
        if (row != NULL) {
            int nFiled = 0;
            rESMInfo.m_nReceiveID = (unsigned int)atoi(row[nFiled++]);
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    // log
    sprintf(sql, "INSERT INTO ex_sendmail_log(pid, puid, order_id, receiveid, title, content, money, itemtype, count, valid_time, time) VALUES(%u,'%s','%s',%u,'%s','%s',%u,%u,%u,%d,%u)",
        rESMInfo.m_nPID, szSQLPUID, szSQLOrderID, rESMInfo.m_nReceiveID, pszTitle, pszContent,
        rESMInfo.m_nMoney, rESMInfo.m_nItemType, rESMInfo.m_nCount, rESMInfo.m_nValidTime, (unsigned int)time(NULL));
    ExecSQL();
    SafeFreeResult();

    delete[] pszTitle;
    delete[] pszContent;

    return nRes;
}

//外部发送Gift邮件
int CDBInterface::ExternalSendMailGift(ExternalSendMailGiftInfo & rESMGInfo)
{
    rESMGInfo.m_nReceiveID = 0;

    char szSQLOrderID[65] = { 0 };
    CDBIMysql::EscapeString(szSQLOrderID, rESMGInfo.m_strOrderID.c_str(), rESMGInfo.m_strOrderID.length());
    char szSQLPUID[MAX_PUID_LEN * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(szSQLPUID, rESMGInfo.m_strPUID.c_str(), rESMGInfo.m_strPUID.length());

    sprintf(sql, "SELECT receiveid FROM ex_sendmailgift_log WHERE pid = %u AND puid = '%s' AND order_id='%s'",
        rESMGInfo.m_nPID, szSQLPUID, szSQLOrderID);

    if (ExecSQL()){
        if ((row = FetchRow()) != NULL){
            SafeFreeResult();
            return Query_Res_Fail;
        }
    }
    SafeFreeResult();

    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT roleid,sex FROM role r, nsytaccountdb.player p WHERE p.pid=%u AND p.puid='%s' AND r.account=p.account;", rESMGInfo.m_nPID, szSQLPUID);
    if (ExecSQL()) {
        row = FetchRow();
        if (row != NULL) {
            int nFiled = 0;
            rESMGInfo.m_nReceiveID = (unsigned int)atoi(row[nFiled++]);
            rESMGInfo.m_nbMale = atoi(row[nFiled++]) == ESexType_Male ? true : false;
            nRes = Query_Res_OK;
        }
    }
    SafeFreeResult();

    // log
    sprintf(sql, "INSERT INTO ex_sendmailgift_log(pid, puid, order_id, receiveid, mailgiftid, time) VALUES(%u,'%s','%s',%u,%u,%u)",
        rESMGInfo.m_nPID, szSQLPUID, szSQLOrderID, rESMGInfo.m_nReceiveID, rESMGInfo.m_nMailGiftID, (unsigned int)time(NULL));
    ExecSQL();
    SafeFreeResult();

    return nRes;

}

int CDBInterface::LoadPlayerSetting(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole != NULL)
    {
        pRole->m_playerSetting = CPlayerSetting();
        sprintf(sql, "select acceptRoomInvitation, openLBS, wifiDownload "
            " from playerSetting where roleid = %u", pRole->m_nRoleID);
        if (ExecSQL())
        {
            row = FetchRow();
            if (row == NULL)
            {
                sprintf(sql, "insert into playerSetting set roleid = %u", pRole->m_nRoleID);
                ExecSQL();
            }
            else
            {
                pRole->m_playerSetting.m_mapOption[EPlayerSetting_ReceiveRoomInvitation] = atoi(row[0]);
                pRole->m_playerSetting.m_mapOption[EPlayerSetting_LBS] = atoi(row[1]);
                pRole->m_playerSetting.m_mapOption[EPlayerSetting_WifiDownload] = atoi(row[2]);
            }
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::SavePlayerSetting(unsigned int nRoleID, CPlayerSetting & rPlayerSetting, int nOption)
{
    int nRes = Query_Res_Fail;
    string strField;
    int nState = rPlayerSetting.m_mapOption[(unsigned char)nOption];

    if (nOption == EPlayerSetting_ReceiveRoomInvitation)
    {
        strField = "acceptRoomInvitation";
    }
    else if (nOption == EPlayerSetting_LBS)
    {
        strField = "openLBS";
    }
    else if (nOption == EPlayerSetting_WifiDownload)
    {
        strField = "wifiDownload";
    }

    if (!strField.empty())
    {
        sprintf(sql, "UPDATE playerSetting SET %s=%d WHERE roleid=%u", strField.c_str(), nState, nRoleID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::AddCoupleInfo(CCoupleInfoDB & coupleInfo, int & nAddRes)
{
    int nRes = Query_Res_OK;
    sprintf(sql, "call addCouple1(%u, %u, %u, %u, @pairid, @res)", coupleInfo.m_nMaleID, coupleInfo.m_nFemaleID, coupleInfo.m_nCoupleDate, coupleInfo.m_nSmallRoleID);
    if (!ExecSQL()) {
        nRes = Query_Res_Fail;
        SafeFreeResult();
    }

    if (nRes == Query_Res_OK) {
        sprintf(sql, "select @pairid, @res");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                coupleInfo.m_nPairID = strtoul(row[0], NULL, 10);
                nAddRes = atoi(row[1]);
            }
        }
        else {
            nRes = Query_Res_Fail;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::AddDivorceAgreementInfo(CDivorceAgreementInfo & agreementinfo, int & nAddRes)
{
    int nRes = Query_Res_OK;
    //sprintf(sql, "call addDivorceAgreement(%u, %u, %u, @agreementid, @res)", agreementinfo.m_nFemaleRoleID, agreementinfo.m_nFemaleRoleID, agreementinfo.m_eState );
    sprintf(sql, "Insert into  divorceagreement SET maleroleid = %u, femaleroleid = %u, state = %d, maleisagree = %d, femaleisagree = %d, proposerid = %u, smallroleid = %u", agreementinfo.m_nMaleRoleID, agreementinfo.m_nFemaleRoleID,
        agreementinfo.m_eState, agreementinfo.m_bMaleIsAgree, agreementinfo.m_bFemaleIsAgree, agreementinfo.m_nProposerID, agreementinfo.m_nSmallRoleID);
    if (!ExecSQL()) {
        nRes = Query_Res_Fail;
        SafeFreeResult();
    }

    if (nRes == Query_Res_OK) {
        sprintf(sql, "select id from  divorceagreement ORDER BY id  DESC LIMIT 1");
        if (ExecSQL()) {
            if (mysql_num_rows(result) > 0) {
                row = FetchRow();
                agreementinfo.m_nAgreementID = strtoul(row[0], NULL, 10);
                //nAddRes = atoi(row[1]);
            }
        }
        else {
            nRes = Query_Res_Fail;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::AddDivorceAction(const CDivorceAction & action)
{
    char cItem[50] = { 0 }; // 无需mysql_escapse_string
    sprintf(cItem, "%d,%d,%d", action.m_SendItem.m_nItemType, action.m_SendItem.m_nItemCount, action.m_SendItem.m_nValidTime);

    char szSqlChatContent[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlChatContent, action.m_strChatContent.c_str(), action.m_strChatContent.length());

    sprintf(sql, "INSERT INTO divorceaction SET agreementid = %u, type = %d, proposerid = %u, item = '%s', chatcontent = '%s', time = %u", action.m_nAgreementID, (int)action.m_eType,
        action.m_nProposerID, cItem, szSqlChatContent, (unsigned int)time(NULL));
    bool bRes = ExecSQL();
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DelCoupleInfo(unsigned int nPairID)
{
    int nRes = Query_Res_OK;
    sprintf(sql, "delete from couple where id = %u", nPairID);
    if (!ExecSQL()) {
        nRes = Query_Res_Fail;
    }

    return nRes;
}

int CDBInterface::LoadCoupleInfo(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole->m_nSex == ESexType_Male)
    {
        sprintf(sql, "select a.id, a.femaleid, a.intimacy, a.coupledate, a.marriagedate,a.blessingvalue,a.marriagegiftdate,b.rolename,a.smallroleid from couple a,role b where a.maleid = %u and b.roleid = a.femaleid", pRole->m_nRoleID);
    }
    else
    {
        sprintf(sql, "select a.id, a.maleid, a.intimacy, a.coupledate, a.marriagedate,a.blessingvalue,a.marriagegiftdate,b.rolename,a.smallroleid from couple a,role b where a.femaleid = %u and b.roleid = a.maleid", pRole->m_nRoleID);
    }

    if (ExecSQL())
    {
        int nField = 0;
        row = FetchRow();
        if (row != NULL && mysql_num_rows(result) > 0)
        {
            pRole->m_coupleInfo.m_nPairID = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_nCoupleID = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_nIntimacy = atoi(row[nField++]);
            pRole->m_coupleInfo.m_nCoupleDate = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_nMarriageDate = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_nBlessingValue = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_nMarriageGiftDate = strtoul(row[nField++], NULL, 10);
            pRole->m_coupleInfo.m_strCoupleName = row[nField++];
            pRole->m_coupleInfo.m_nSmallRoleID = strtoul(row[nField++], NULL, 10);

            SafeFreeResult();
        }
        else
        {
            if (pRole->m_nSex == ESexType_Male)
            {
                sprintf(sql, "select a.id, a.maleid, a.intimacy, a.coupledate, a.marriagedate,a.blessingvalue,a.marriagegiftdate,b.rolename,a.smallroleid from couple a,role b where a.femaleid = %u and b.roleid = a.maleid", pRole->m_nRoleID);
            }
            else
            {
                sprintf(sql, "select a.id, a.femaleid, a.intimacy, a.coupledate, a.marriagedate,a.blessingvalue,a.marriagegiftdate,b.rolename,a.smallroleid from couple a,role b where a.maleid = %u and b.roleid = a.femaleid", pRole->m_nRoleID);
            }

            if (ExecSQL())
            {
                nField = 0;
                row = FetchRow();
                if (row != NULL && mysql_num_rows(result) > 0)
                {
                    pRole->m_coupleInfo.m_nPairID = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_nCoupleID = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_nIntimacy = atoi(row[nField++]);
                    pRole->m_coupleInfo.m_nCoupleDate = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_nMarriageDate = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_nBlessingValue = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_nMarriageGiftDate = strtoul(row[nField++], NULL, 10);
                    pRole->m_coupleInfo.m_strCoupleName = row[nField++];
                    pRole->m_coupleInfo.m_nSmallRoleID = strtoul(row[nField++], NULL, 10);

                    SafeFreeResult();
                }
            }
        }
    }

    return nRes;
}

int CDBInterface::LoadDivorceAgreementInfo(ROLEINFO_DB *pRole)
{
    if (pRole->m_nDivorceAgreementID <= 0)
    {
        return Query_Res_OK;
    }


    sprintf(sql, "select a.id, a.maleroleid, a.maleisagree, a.femaleroleid, a.femaleisagree, a.state, a.proposerid, b.rolename, a.smallroleid from divorceagreement a,role b where a.id = %u and b.roleid = a.femaleroleid", pRole->m_nDivorceAgreementID);
    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
            int nField = 0;
            row = FetchRow();

            pRole->m_DivorceAgreementInfo.m_nAgreementID = (unsigned int)strtoul(row[nField++], NULL, 10);
            unsigned int nMaleRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            if (pRole->m_nRoleID == nMaleRoleID)
            {
                pRole->m_DivorceAgreementInfo.m_nMaleRoleID = nMaleRoleID;
                pRole->m_DivorceAgreementInfo.m_bMaleIsAgree = strtoul(row[nField++], NULL, 10) > 0 ? true : false;;
                pRole->m_DivorceAgreementInfo.m_nFemaleRoleID = strtoul(row[nField++], NULL, 10);
                pRole->m_DivorceAgreementInfo.m_bFemaleIsAgree = strtoul(row[nField++], NULL, 10) > 0 ? true : false;
                pRole->m_DivorceAgreementInfo.m_eState = (eDivorceState)strtoul(row[nField++], NULL, 10);
                pRole->m_DivorceAgreementInfo.m_nProposerID = (unsigned int)strtoul(row[nField++], NULL, 10);
                pRole->m_DivorceAgreementInfo.m_strFemaleRoleName = row[nField++];
                pRole->m_DivorceAgreementInfo.m_strMaleRoleName = pRole->GetName();
                pRole->m_DivorceAgreementInfo.m_nSmallRoleID = (unsigned int)strtoul(row[nField++], NULL, 10);
            }
            else
            {
                SafeFreeResult();
                sprintf(sql, "select a.id, a.maleroleid, a.maleisagree, a.femaleroleid, a.femaleisagree, a.state, a.proposerid, b.rolename, a.smallroleid from divorceagreement a,role b where a.id = %u and b.roleid = a.maleroleid", pRole->m_nDivorceAgreementID);
                if (ExecSQL())
                {
                    if (mysql_num_rows(result) > 0)
                    {
                        nField = 0;
                        row = FetchRow();
                        pRole->m_DivorceAgreementInfo.m_nAgreementID = strtoul(row[nField++], NULL, 10);
                        nMaleRoleID = strtoul(row[nField++], NULL, 10);
                        bool bIsMaleAgree = strtoul(row[nField++], NULL, 10) > 0 ? true : false;
                        unsigned int nFemaleRoleID = strtoul(row[nField++], NULL, 10);
                        if (nFemaleRoleID == pRole->m_nRoleID)
                        {
                            pRole->m_DivorceAgreementInfo.m_nMaleRoleID = nMaleRoleID;
                            pRole->m_DivorceAgreementInfo.m_bMaleIsAgree = bIsMaleAgree;
                            pRole->m_DivorceAgreementInfo.m_nFemaleRoleID = nFemaleRoleID;
                            pRole->m_DivorceAgreementInfo.m_bFemaleIsAgree = strtoul(row[nField++], NULL, 10) > 0 ? true : false;
                            pRole->m_DivorceAgreementInfo.m_eState = (eDivorceState)strtoul(row[nField++], NULL, 10);
                            pRole->m_DivorceAgreementInfo.m_nProposerID = strtoul(row[nField++], NULL, 10);
                            pRole->m_DivorceAgreementInfo.m_strMaleRoleName = row[nField++];
                            pRole->m_DivorceAgreementInfo.m_strFemaleRoleName = pRole->GetName();
                            pRole->m_DivorceAgreementInfo.m_nSmallRoleID = strtoul(row[nField++], NULL, 10);
                        }

                        SafeFreeResult();

                        return Query_Res_OK;
                    }
                }
            }

            SafeFreeResult();
            return Query_Res_OK;
        }
    }

    return Query_Res_OK;
}

int CDBInterface::LoadDivorceAction(ROLEINFO_DB *pRole)
{
    if (pRole->m_nDivorceAgreementID <= 0)
    {
        return Query_Res_OK;
    }

    int nRes = Query_Res_Fail;
    sprintf(sql, "select type, proposerID, item, chatcontent, time from divorceaction where agreementid = %u ", pRole->m_nDivorceAgreementID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CDivorceAction action;
            action.m_nAgreementID = pRole->m_DivorceAgreementInfo.m_nAgreementID;
            action.m_eType = (eDivorceAction)strtoul(row[nField++], NULL, 10);
            action.m_nProposerID = strtoul(row[nField++], NULL, 10);
            std::string strItem = row[nField++];
            TransferItem(strItem.c_str(), action.m_SendItem);
            action.m_strChatContent = row[nField++];
            action.m_nTime = strtoul(row[nField++], NULL, 10);

            pRole->m_DivorceAction.push_back(action);
        }
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadMagicLamp(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_MagicLamp = MagicLamp();
    sprintf(sql, "select crystal, luckstar, magicdust, crystaltime, maxmainline, maxtheme, nextdayresettime, todaygetvitnum, vipbuyvitnum from magiclamp where roleid = %u ", pRole->m_nRoleID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            pRole->m_MagicLamp.m_nCrytsal = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nLuckStar = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nMagicDust = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nCrystalRestoreTime = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nMaxMainline = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nMaxTheme = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nNextDayResetTime = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nTodayHaveGetVitNum = strtoul(row[nField++], NULL, 10);
            pRole->m_MagicLamp.m_nFavorableBuyVitCount = strtoul(row[nField++], NULL, 10);
            nRes = Query_Res_OK;
        }
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadMainlineParcloseList(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_MainlineParcloseList.clear();
    sprintf(sql, "select dungeonid, isactived, score, quickfinishnum from dungeon where roleid = %u ", pRole->m_nRoleID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            MainlineParcloseRecord info;
            info.nParcloseID = strtoul(row[nField++], NULL, 10);
            info.bIsActived = strtoul(row[nField++], NULL, 10) != 0 ? true : false;
            info.nStarCount = strtoul(row[nField++], NULL, 10);
            info.nQuickFinishNum = strtoul(row[nField++], NULL, 10);

            pRole->m_MainlineParcloseList.push_back(info);
        }
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadThemeParcloseList(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_ThemeParcloseList.clear();
    sprintf(sql, "select themeid, parcloseid, isactived, starcount, quickfinishnum from dungeon_theme_parclose where roleid = %u ", pRole->m_nRoleID);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            ThemeParcloseRecord info;
            info.nThemeID = strtoul(row[nField++], NULL, 10);
            info.nParcloseID = strtoul(row[nField++], NULL, 10);
            info.bIsActived = strtoul(row[nField++], NULL, 10) != 0 ? true : false;
            info.nStarCount = (unsigned short)strtoul(row[nField++], NULL, 10);
            info.nQuickFinishNum = strtoul(row[nField++], NULL, 10);

            pRole->m_ThemeParcloseList.push_back(info);
        }
        nRes = Query_Res_OK;
    }
    SafeFreeResult();

    return nRes;
}

int CDBInterface::LoadDungeonStarReward(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_LuckStarReward.clear();
    sprintf(sql, "select rewardindex from dungeon_star_reward where roleid = %u ", pRole->m_nRoleID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nIndex = strtoul(row[nField++], NULL, 10);
            pRole->m_LuckStarReward.insert(nIndex);
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadPlayerDebris(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_PlayerDebrisInfo.clear();
    sprintf(sql, "select debrisid, debriscount from dungeon_debris where roleid = %u ", pRole->m_nRoleID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nDebrisID = strtoul(row[nField++], NULL, 10);
            unsigned int nDebrisCount = strtoul(row[nField++], NULL, 10);

            pRole->m_PlayerDebrisInfo.insert(std::make_pair(nDebrisID, nDebrisCount));
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadFriendContactInfo(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select friendroleid, contactnum, givevitnum, givevittime, begivevitnum, begivevittime from dungeon_friend_contact where roleid = %u ", pRole->m_nRoleID);

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            FriendContactInfo friendinfo;
            friendinfo.m_nFriendRoleID = strtoul(row[nField++], NULL, 10);
            friendinfo.m_nContactNum = strtoul(row[nField++], NULL, 10);
            friendinfo.m_nGiveVitNum = strtoul(row[nField++], NULL, 10);
            friendinfo.m_nGiveVitTime = strtoul(row[nField++], NULL, 10);
            friendinfo.m_nBeGiveVitNum = strtoul(row[nField++], NULL, 10);
            friendinfo.m_nBeGiveVitTime = strtoul(row[nField++], NULL, 10);

            pRole->m_mapFriendContactInfo[friendinfo.m_nFriendRoleID] = friendinfo;
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadLongactingRecharge(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select rewardindex from longacting_cumulative_recharge where roleid = %u ", pRole->m_nRoleID);

    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            pRole->m_nLongactingCumulativeRechargeIndex = strtoul(row[nField++], NULL, 10);
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateLongActingRechargeIndex(unsigned int nRoleID, unsigned int nRewardIndex)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "select rewardindex from longacting_cumulative_recharge where roleid = %u ", nRoleID);
    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            unsigned int nRecordIndex = strtoul(row[nField++], NULL, 10);

            if (nRecordIndex >= nRewardIndex)
            {
                return Query_Res_Fail;
            }

            sprintf(sql, "update longacting_cumulative_recharge set rewardindex = %u where roleid = %u", nRewardIndex, nRoleID);
            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
        }
        else
        {
            sprintf(sql, "INSERT INTO longacting_cumulative_recharge SET roleid = %u, rewardindex = %u", nRoleID, nRewardIndex);
            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
        }
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateCoupleMarriageDate(CCoupleInfo & coupleInfo)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "update couple set intimacy = %d, marriagedate = %u, blessingvalue = %d  where id = %u", coupleInfo.m_nIntimacy, coupleInfo.m_nMarriageDate, coupleInfo.m_nBlessingValue, coupleInfo.m_nPairID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}
int CDBInterface::UpdateCoupleBlessingValue(unsigned int nPairID, int nValue)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "update couple set blessingvalue = %d where id = %u", nValue, nPairID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateCoupleDivorceAgreementID(unsigned int nRoleID, unsigned int nDivorceAgreementID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update role set divorceagreementid = %u where roleid = %u", nDivorceAgreementID, nRoleID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateDivorceAgreementState(unsigned int nAgreementID, int nState)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update divorceagreement set state = %d where id = %u", nState, nAgreementID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateDivorceAgreement(unsigned int nAgreementID, CDivorceAgreementInfo & agreement)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update divorceagreement set maleisagree = %d, femaleisagree = %d, state = %d where id = %u", agreement.m_bMaleIsAgree, agreement.m_bFemaleIsAgree, agreement.m_eState, nAgreementID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddBlessingRankInfo(const CBlessingValueRankInfo & rankInfo)
{
    int nRes = Query_Res_Fail;
    unsigned int nTime = (unsigned int)time(NULL);
    sprintf(sql, "INSERT INTO blessingrank(pairid, blessingvalue, changetime ) VALUES( %u,%d,%u )",
        rankInfo.m_nPairID, rankInfo.m_nBlessingValue, nTime);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::DelBlessingRankInfo(unsigned int nPairID)
{
    int nRes = Query_Res_OK;
    sprintf(sql, "delete from blessingrank where pairid = %u", nPairID);
    if (!ExecSQL()) {
        nRes = Query_Res_Fail;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateBlessingRankValue(unsigned int nPairID, int nBlessingValue)
{
    int nRes = Query_Res_Fail;
    unsigned int nTime = (unsigned int)time(NULL);
    sprintf(sql, "update blessingrank set blessingvalue = %d, changetime = %u where pairid = %u", nBlessingValue, nTime, nPairID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateMarriageGiftDate(unsigned int nPairID, int nGiftDate)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update couple set marriagegiftdate = %d where id = %u", nGiftDate, nPairID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    SafeFreeResult();
    return nRes;
}



int CDBInterface::LoadAllBlessingValueRank(std::vector<CBlessingValueRankInfo> * prankvec)
{
    if (prankvec == NULL)
    {
        return Query_Res_Fail;
    }

    int nLoadCount = MAX_BLESSINGRANK_COUNT + 20;
    int nRes = Query_Res_Fail;
    sprintf(sql, "select a.id, a.maleid, b.rolename, a.femaleid, c.rolename, a.blessingvalue from couple a INNER JOIN role b on a.maleid = b.roleid INNER JOIN role c ON a.femaleid = c.roleid WHERE blessingvalue > 0 ORDER BY blessingvalue desc limit %d", nLoadCount);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CBlessingValueRankInfo rankinfo;
            rankinfo.m_nPairID = strtoul(row[nField++], NULL, 10);
            rankinfo.m_nMaleRoleID = strtoul(row[nField++], NULL, 10);
            rankinfo.m_strMaleRoleName = row[nField++];
            rankinfo.m_nFemaleRoleID = strtoul(row[nField++], NULL, 10);
            rankinfo.m_strFemaleRoleName = row[nField++];
            rankinfo.m_nBlessingValue = strtoul(row[nField++], NULL, 10);

            prankvec->push_back(rankinfo);
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::LoadofflineCoupleInfo(std::map<unsigned int, COfflineCoupleInfo> * pOfflineCoupleInfo)
{
    if (pOfflineCoupleInfo == NULL)
    {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;
    sprintf(sql, "select a.id, a.maleid, b.rolename, a.femaleid, c.rolename, a.blessingvalue, a.coupledate, a.marriagedate, a.intimacy, a.smallroleid from couple a  INNER  JOIN role b on a.maleid = b.roleid INNER  JOIN role c ON a.femaleid = c.roleid WHERE blessingvalue > 0 ORDER BY blessingvalue desc ");

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            COfflineCoupleInfo coupleinfo;
            coupleinfo.m_nPairID = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_nMaleRoleID = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_strMaleRoleName = row[nField++];
            coupleinfo.m_nFemaleRoleID = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_strFemaleRoleName = row[nField++];
            coupleinfo.m_nBlessingValue = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_nCoupleDate = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_nMarriageDate = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_nIntimacy = strtoul(row[nField++], NULL, 10);
            coupleinfo.m_nSmallRoleID = strtoul(row[nField++], NULL, 10);

            pOfflineCoupleInfo->insert(make_pair(coupleinfo.m_nPairID, coupleinfo));
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateCoupleIntimacy(unsigned int nPairID, int nValue)
{
    int nRes = Query_Res_Fail;

    if (nPairID > 0) {
        sprintf(sql, "call update_couple_intimacy(%u, %d)", nPairID, nValue);
        if (ExecSQL()) {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::LoadRolePuzzleOnLogin(ROLEINFO_DB *pRole)
{
    int nRes = Query_Res_Fail;

    if (pRole == NULL)
        return nRes;

    pRole->m_listPuzzleActivityDB.clear();
    sprintf(sql, "select puzzleid, updatetime, finishtime, puzzlestate from puzzle where roleid = %u and updatetime > %u",
        pRole->m_nRoleID, (unsigned int)time(NULL) - MONTH_PRE_SECONDS);

    if (ExecSQL())
    {
        std::list<CRolePuzzleActivityDB> &list = pRole->m_listPuzzleActivityDB;

        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CRolePuzzleActivityDB puzzleDB;
            puzzleDB.m_nPuzzleID = atoi(row[nField++]);
            puzzleDB.m_nUpdateTime = atoi(row[nField++]);
            puzzleDB.m_nFinishTime = atoi(row[nField++]);

            char *pPuzzleState = row[nField++];
            for (int i = 0; i < PUZZLE_FRAGMENTS_COUNT; ++i)
            {
                if (pPuzzleState[i] != '\0')
                    puzzleDB.m_arFragments[i] = pPuzzleState[i];
                else
                    break;
            }

            list.push_back(puzzleDB);
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadRolePuzzle(CRoleAllPuzzleDB& rRolePuzzle)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select puzzleid, updatetime, finishtime, puzzlestate from puzzle where roleid = %u and updatetime between %d and %d",
        rRolePuzzle.m_nRoleID, rRolePuzzle.m_nPuzzleBeginTime, rRolePuzzle.m_nPuzzleEndTime);
    if (ExecSQL()) {
        while ((row = FetchRow()) != NULL){
            int nField = 0;
            CRolePuzzleActivityDB puzzleDB;
            puzzleDB.m_nPuzzleID = atoi(row[nField++]);
            puzzleDB.m_nUpdateTime = atoi(row[nField++]);
            puzzleDB.m_nFinishTime = atoi(row[nField++]);

            char* pPuzzleState = row[nField++];
            for (int i = 0; i < PUZZLE_FRAGMENTS_COUNT; ++i)
            {
                if (pPuzzleState[i] != '\0') {
                    puzzleDB.m_arFragments[i] = pPuzzleState[i];
                }
                else {
                    break;
                }
            }

            if (rRolePuzzle.m_AllPuzzle.find(puzzleDB.m_nPuzzleID) == rRolePuzzle.m_AllPuzzle.end()) {
                rRolePuzzle.m_AllPuzzle.insert(std::make_pair(puzzleDB.m_nPuzzleID, puzzleDB));
            }
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::InsertRolePuzzle(unsigned int nRoleID, CRolePuzzleActivityDB& rPuzzleInfo)
{
    int nRes = Query_Res_Fail;
    char strSqlPuzzle[PUZZLE_FRAGMENTS_COUNT * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(strSqlPuzzle, rPuzzleInfo.m_arFragments, strlen(rPuzzleInfo.m_arFragments));

    sprintf(sql, "insert into puzzle(roleid, puzzleid, puzzlestate, updatetime) values(%u, %d, '%s', %d)",
        nRoleID, rPuzzleInfo.m_nPuzzleID, strSqlPuzzle, rPuzzleInfo.m_nUpdateTime);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateRolePuzzle(unsigned int nRoleID, int nOriTime, CRolePuzzleActivityDB& rPuzzleInfo)
{
    int nRes = Query_Res_Fail;
    char strSqlPuzzle[PUZZLE_FRAGMENTS_COUNT * 2 + 1] = { 0 };
    CDBIMysql::EscapeString(strSqlPuzzle, rPuzzleInfo.m_arFragments, strlen(rPuzzleInfo.m_arFragments));

    sprintf(sql, "update puzzle set puzzlestate = '%s', updatetime = %d, finishtime = %d where roleid = %u and puzzleid = %d and updatetime = %d",
        strSqlPuzzle, rPuzzleInfo.m_nUpdateTime, rPuzzleInfo.m_nFinishTime, nRoleID, rPuzzleInfo.m_nPuzzleID, nOriTime);

    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateRoleInvitationCode(unsigned int nRoleID, const char *szCode)
{
    int nRes = Query_Res_Fail;

    char *end = sql;
    sprintf(sql, "UPDATE role SET invitationcode = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, szCode, strlen(szCode));
    end += strlen(end);
    strcat(end, "'");
    end += strlen(end);
    sprintf(end, "where roleid = %u", nRoleID);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::CheckInvitationCode(const char* pszInvitationCode, int& isValid)
{
    int nRes = Query_Res_Fail;
    isValid = 0;

    strcpy(sql, "SELECT count(1) FROM role WHERE invitationcode = '");
    char *end = sql + strlen(sql);
    end += CDBIMysql::EscapeString(end, pszInvitationCode, strlen(pszInvitationCode));
    strcat(end, "'");
    end += strlen(end);

    if (ExecSQL())
    {
        int count = 0;
        row = FetchRow();
        if (row)
            count = atoi(row[0]);

        if (count > 0)
            isValid = 1;

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::GetInvitationBindCount(int nValidLevel, unsigned int nStartTime, unsigned int nEndTime, const char *pszInvitationCode, unsigned int& nBindCount)
{
    int nRes = Query_Res_Fail;

    char strSqlCode[64] = { 0 };
    CDBIMysql::EscapeString(strSqlCode, pszInvitationCode, strlen(pszInvitationCode));

    sprintf(sql,
        "SELECT r.level FROM role r, invitation i "
        " WHERE i.roleid = r.roleid AND i.code = '%s' "
        "   AND i.accepttime >= %u "
        "   AND i.accepttime <= %u ",
        strSqlCode, nStartTime, nEndTime);

    if (ExecSQL())
    {
        while (NULL != (row = FetchRow()))
        {
            int nLevel = atoi(row[0]);
            if (nLevel >= nValidLevel)
            {
                nBindCount++;
            }
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddInvitation(unsigned int nRoleID, unsigned int nTime, const char *szInvitationCode)
{
    int nRes = Query_Res_Fail;

    char strSqlCode[64] = { 0 };
    CDBIMysql::EscapeString(strSqlCode, szInvitationCode, strlen(szInvitationCode));

    sprintf(sql, "INSERT INTO invitation(code, roleid, accepttime) VALUES('%s', %u, %u)",
        strSqlCode, nRoleID, nTime);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::AddInviterReward(unsigned int nRoleID, int nLevel)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO invitation_reward(roleid, level, received) VALUES(%u, %d, 1)",
        nRoleID, nLevel);

    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::LoadInviterReward(CRoleAllInvitationInfoDB* pRewards, unsigned int nStartTime, unsigned int nEndTime)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT level, received, UNIX_TIMESTAMP(receivedtime) FROM invitation_reward WHERE roleid = %u AND receivedtime >= from_unixtime(%u) AND receivedtime <= from_unixtime(%u)", pRewards->m_nRoleID, nStartTime, nEndTime);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            CRoleInvitationRewardDB reward;
            reward.m_nLevel = (unsigned short)atoi(row[nField++]);
            reward.m_bReceived = atoi(row[nField++]) ? true : false;
            reward.m_nReceivedTime = (unsigned int)strtoul(row[nField++], NULL, 10);
            pRewards->m_InvitationRewards.insert(std::make_pair(reward.m_nLevel, reward));
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddAcceptorReward(unsigned int nRoleID)
{
    return AddEventEntry(nRoleID, eEvent_ReceivedInvitationReward, 1);
}

int CDBInterface::GetInvitationInfo(int nValidLevel, CRoleAllInvitationInfoDB* pInvitationInfo, unsigned int nStartTime, unsigned int nEndTime)
{
    int nRes = LoadInviterReward(pInvitationInfo, nStartTime, nEndTime);
    if (nRes == Query_Res_OK)
    {
        nRes = GetInvitationBindCount(nValidLevel, nStartTime, nEndTime, pInvitationInfo->m_strInvitationCode.c_str(), pInvitationInfo->m_nInvitationBindCount);
    }

    return nRes;
}

int CDBInterface::UpdateNoPushRatingToday(unsigned int nRoleID, unsigned int nTime)
{
    return UpdateEventEntry(nRoleID, eEvent_NoPushRatingToday, nTime);
}

int CDBInterface::UpdatePushRatedLevelUp(unsigned int nRoleID, int nLevel)
{
    return UpdateEventEntry(nRoleID, eEvent_PushRatedLevelUp, nLevel);
}

int CDBInterface::UpdatePushRatedOnlineTime(unsigned int nRoleID, unsigned int nTime)
{
    return UpdateEventEntry(nRoleID, eEvent_PushRatedOnlineTime, nTime);
}

int CDBInterface::UpdatePushRatedDailyQuest(unsigned int nRoleID, unsigned int nTime)
{
    return UpdateEventEntry(nRoleID, eEvent_PushRatedDailyQuest, nTime);
}

int CDBInterface::UpdatePushRatedDailyShoping(unsigned int nRoleID, unsigned int nTime)
{
    return UpdateEventEntry(nRoleID, eEvent_PushRatedDailyShoping, nTime);
}

int CDBInterface::UpdatePushRatedCount(unsigned int nRoleID, int nCount, unsigned int nTime)
{
    return UpdateEventEntry4(nRoleID, eEvent_PushRatedCount, nCount, nTime);
}

int CDBInterface::LoadAccountSecurityQuiz(unsigned int nRoleID, SecurityQuiz_DB *quiz)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT quiz1, quiz2, quiz3, answer1, answer2, answer3 "
        "FROM security_quiz WHERE roleid = %u", nRoleID);
    if (ExecSQL())
    {
        if ((row = mysql_fetch_row(result)) != NULL)
        {
            int i = 0;
            quiz->nQuizID[0] = (unsigned char)atoi(row[i++]);
            quiz->nQuizID[1] = (unsigned char)atoi(row[i++]);
            quiz->nQuizID[2] = (unsigned char)atoi(row[i++]);
            strcpy(quiz->szAnswer[0], row[i++]);
            strcpy(quiz->szAnswer[1], row[i++]);
            strcpy(quiz->szAnswer[2], row[i++]);
        }
        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::SetAccountSecurityQuiz(unsigned int nRoleID, const SecurityQuiz_DB *quiz)
{
    int nRes = Query_Res_Fail;

    char strSqlAnswer[MAX_SQUIZ_COUNT][MAX_SQUIZ_LEN];
    for (int i = 0; i < MAX_SQUIZ_COUNT; ++i)
    {
        memset(strSqlAnswer[i], 0, MAX_SQUIZ_LEN);
    }
    
    CDBIMysql::EscapeString(strSqlAnswer[0], quiz->szAnswer[0], strlen(quiz->szAnswer[0]));
    CDBIMysql::EscapeString(strSqlAnswer[1], quiz->szAnswer[1], strlen(quiz->szAnswer[1]));
    CDBIMysql::EscapeString(strSqlAnswer[2], quiz->szAnswer[2], strlen(quiz->szAnswer[2]));

    sprintf(sql, "call update_security_quiz(%u, %u, %u, %u, '%s', '%s', '%s')",
        nRoleID, (unsigned int)quiz->nQuizID[0], (unsigned int)quiz->nQuizID[1], (unsigned int)quiz->nQuizID[2],
        strSqlAnswer[0], strSqlAnswer[1], strSqlAnswer[2]);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::GetDancerName(CLiveRoomDancerDB *pDancer)
{
    int nRes = Query_Res_Fail;
    if (pDancer->m_bIsDanceGroup)
    {
        sprintf(sql, "SELECT group_id, group_name FROM dance_group "
            "WHERE group_id=%u OR group_id=%u OR group_id=%u OR group_id=%u",
            pDancer->m_nDancerID[0], pDancer->m_nDancerID[1], pDancer->m_nDancerID[2], pDancer->m_nDancerID[3]);
    }
    else
    {
        sprintf(sql, "SELECT roleid, rolename FROM role "
            "WHERE roleid=%u OR roleid=%u OR roleid=%u OR roleid=%u",
            pDancer->m_nDancerID[0], pDancer->m_nDancerID[1], pDancer->m_nDancerID[2], pDancer->m_nDancerID[3]);
    }

    pDancer->Clear();
    if (ExecSQL())
    {
        for (int i = 0; (row = FetchRow()) != NULL; i++)
        {
            pDancer->m_nDancerID[i] = (unsigned int)atoi(row[0]);
            pDancer->m_strDancerName[i] = row[1];
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}


int CDBInterface::UpdateDancerName(CLiveRoomDancerDB *pDancer)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT roleid, rolename FROM role "
        "WHERE roleid=%u OR roleid=%u OR roleid=%u OR roleid=%u",
        pDancer->m_nDancerID[0], pDancer->m_nDancerID[1], pDancer->m_nDancerID[2], pDancer->m_nDancerID[3]);

    pDancer->Clear();
    if (ExecSQL())
    {
        for (int i = 0; (row = FetchRow()) != NULL; i++)
        {
            pDancer->m_nDancerID[i] = (unsigned int)atoi(row[0]);
            pDancer->m_strDancerName[i] = row[1];
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}


int CDBInterface::LoadLiveRoomPointAndHot(unsigned int nStartTime, unsigned int nEndTime, CAllLiveRoomScoreDB *pScore)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT s.roleid, r.rolename, s.point, s.hot "
        " FROM live_room_score s JOIN role r ON r.roleid = s.roleid "
        "WHERE s.point_update_time > %u AND s.point_update_time < %u "
        "  AND s.hot_update_time > %u AND s.hot_update_time < %u "
        "ORDER BY point_update_time, hot_update_time",
        nStartTime, nEndTime, nStartTime, nEndTime);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            CLiveRoomScoreDB score;
            score.m_nRoleID = (unsigned int)atoi(row[nField++]);
            score.m_strRoleName = row[nField++];
            score.m_nPoint = atoi(row[nField++]);
            score.m_nHot = atoi(row[nField++]);

            pScore->m_listScore.push_back(score);
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int	CDBInterface::GetLiveRoomPointAndHot(unsigned int nStartTime, unsigned int nEndTime, CLiveRoomScoreDB *pScore)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT roleid, point, hot FROM live_room_score "
        " WHERE roleid = %u AND point_update_time > %u AND point_update_time < %u "
        " AND hot_update_time > %u AND hot_update_time < %u", pScore->m_nRoleID,
        nStartTime, nEndTime, nStartTime, nEndTime);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            pScore->m_nRoleID = (unsigned int)atoi(row[nField++]);
            pScore->m_nPoint = atoi(row[nField++]);
            pScore->m_nHot = atoi(row[nField++]);
        }

        nRes = Query_Res_OK;
    }

    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpdateAuditionPoint(unsigned int nRoleID, int nPoint, unsigned int nStartTime, unsigned int nEndTime)
{
    int nRes = Query_Res_Fail;
    unsigned int nNow = (unsigned int)time(NULL);

    sprintf(sql, "call update_audition_point(%u, %d, %u, %u, %u)", nRoleID, nPoint, nNow, nStartTime, nEndTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateDancerHot(unsigned int nRoleID, int nHot, unsigned int nStartTime, unsigned int nEndTime)
{
    int nRes = Query_Res_Fail;
    unsigned int nNow = (unsigned int)time(NULL);

    sprintf(sql, "call update_live_room_hot(%u, %d, %u, %u, %u)", nRoleID, nHot, nNow, nStartTime, nEndTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int	CDBInterface::ClearFirstPaymentInfo(unsigned int nRoleID)
{
    unsigned int nNow = (unsigned int)time(NULL);
    int nRes = Query_Res_Fail;

    PaymentInfoMgr & pim = PaymentInfoMgr::Instance();
    int nFirstChargeStartTime = 0;

    if (pim.FirstChargeRewardIsOpen(Version_Free, nNow))
    {
        nFirstChargeStartTime = pim.GetFirstChargeStartTime(Version_Free, nNow);
    }
    else if (pim.FirstChargeRewardIsOpen(Version_Charge, nNow))
    {
        nFirstChargeStartTime = pim.GetFirstChargeStartTime(Version_Charge, nNow);
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "ClearFirstPaymentInfo, First Recharge Rewards is not OPEN.");
    }

    sprintf(sql, "CALL clear_first_payment_info (%u, %u)", nRoleID, nFirstChargeStartTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::AddFresherReward(unsigned int nRoleID, int nDay)
{
    unsigned char eventId = 0;
    switch (nDay)
    {
    case 0: eventId = eEvent_FresherReward_AllDay; break;
    case 1: eventId = eEvent_FresherReward_Day1; break;
    case 2: eventId = eEvent_FresherReward_Day2; break;
    case 3: eventId = eEvent_FresherReward_Day3; break;
    case 4: eventId = eEvent_FresherReward_Day4; break;
    case 5: eventId = eEvent_FresherReward_Day5; break;
    case 6: eventId = eEvent_FresherReward_Day6; break;
    case 7: eventId = eEvent_FresherReward_Day7; break;
    default: return Query_Res_Fail;
    }
    return AddEventEntry(nRoleID, eventId, (unsigned int)time(NULL));
}

int CDBInterface::LoadCompensationInfo(CompensationInfoDB* pInfoDB)
{
    int nRes = Query_Res_Fail;
    int nNow = (int)time(NULL);
    sprintf(sql, "select id,endTime,money,title,content,maleitem,femaleitem from compensation where endTime > %d", nNow);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            CompensationInfo cInfo;
            cInfo.m_nID = atoi(row[nField++]);
            cInfo.m_nEndTime = atoi(row[nField++]);
            cInfo.m_nMoney = atoi(row[nField++]);
            cInfo.m_strTitle = row[nField++];
            cInfo.m_strContent = row[nField++];
            cInfo.m_strMaleItem = row[nField++];
            cInfo.m_strFemaleItem = row[nField++];
            TransferItemList(cInfo.m_strFemaleItem.c_str(), cInfo.m_FemaleItems);
            TransferItemList(cInfo.m_strMaleItem.c_str(), cInfo.m_MaleItems);

            pInfoDB->m_Compensations.insert(std::make_pair(cInfo.m_nID, cInfo));
        }
        nRes = Query_Res_OK;
    }
    return nRes;
}

int	CDBInterface::InsertCompensationInfo(CompensationInfo* pInfoDB)
{
    int nRes = Query_Res_Fail;
    int nID = -1;

    char szSqlTitle[1024] = { 0 };
    char szSqlContent[1024] = { 0 };
    char szSqlMaleItem[1024] = { 0 };
    char szSqlFemaleItem[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlTitle, pInfoDB->m_strTitle.c_str(), pInfoDB->m_strTitle.length());
    CDBIMysql::EscapeString(szSqlContent, pInfoDB->m_strContent.c_str(), pInfoDB->m_strContent.length());
    CDBIMysql::EscapeString(szSqlMaleItem, pInfoDB->m_strMaleItem.c_str(), pInfoDB->m_strMaleItem.length());
    CDBIMysql::EscapeString(szSqlFemaleItem, pInfoDB->m_strFemaleItem.c_str(), pInfoDB->m_strFemaleItem.length());

    sprintf(sql, "call insertcompensation (%d,%d,'%s','%s','%s','%s',@nID)", pInfoDB->m_nEndTime, pInfoDB->m_nMoney,
        szSqlTitle, szSqlContent, szSqlMaleItem, szSqlFemaleItem);
    if (ExecSQL())
    {
        SafeFreeResult();

        sprintf(sql, "select @nID");
        if (ExecSQL())
        {
            if (mysql_num_rows(result) > 0)
            {
                row = FetchRow();
                if (row != NULL && *row != NULL)
                {
                    nID = atoi(row[0]);
                    pInfoDB->m_nID = nID;
                }
                nRes = Query_Res_OK;
                SafeFreeResult();
            }
        }
    }
    return nRes;
}

int CDBInterface::ReworkFriendNotes(unsigned int nRoleID, unsigned int nFriendID, const std::string & notes)
{
    char szSqlNotes[1024] = { 0 };
    CDBIMysql::EscapeString(szSqlNotes, notes.c_str(), notes.length());

    sprintf(sql, "UPDATE friend SET notes = '%s' WHERE roleid =%u AND friendid = %u ", szSqlNotes, nRoleID, nFriendID);
    bool bRes = ExecSQL();
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int	CDBInterface::LoadRoleCompensation(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    pRole->m_CompensationReward.clear();
    sprintf(sql, "select compensationid from compensationreward where roleid = %d", pRole->m_nRoleID);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nCompensationID = atoi(row[0]);
            pRole->m_CompensationReward.push_back(nCompensationID);
        }
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertRoleCompensation(int nRoleID, int nCompensationID)
{
    int nRes = Query_Res_Fail;
    int nNow = (int)time(NULL);
    sprintf(sql, "insert into compensationreward (roleid,compensationid,updatetime) values (%d,%d,%d)", nRoleID, nCompensationID, nNow);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::LoadRoleQuestNew(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    pRole->listQuestNew.clear();
    sprintf(sql, "select roleid,questid,state,updatetime,music,useitem,functions,completetimes,createtime,refreshertimes,destroytype,jsonparam from questnew where roleid = %d", pRole->m_nRoleID);
    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            QUESTNEWINFO questInfo;
            questInfo.m_RoleID = atoi(row[0]);
            questInfo.m_QuestID = (unsigned short)atoi(row[1]);
            questInfo.m_State = (unsigned short)atoi(row[2]);
            questInfo.m_UpdateTime = atoi(row[3]);
            questInfo.m_Musics = row[4];
            questInfo.m_UseItems = row[5];
            questInfo.m_Functions = row[6];
            questInfo.m_Times = (unsigned short)atoi(row[7]);
            questInfo.m_CreateTime = atoi(row[8]);
            questInfo.m_DayTimes = (unsigned short)atoi(row[9]);
            questInfo.m_DestroyType = (unsigned short)atoi(row[10]);
            questInfo.m_strJsonQuestParam = row[11];
            pRole->listQuestNew.push_back(questInfo);
        }
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::LoadFriendOfflineTime(unsigned int nRoleID, std::map<unsigned int, unsigned int*> * pFriendInfo)
{
    std::map<unsigned int, unsigned int*>::iterator it = pFriendInfo->begin();
    for (; it != pFriendInfo->end(); ++it)
    {
        sprintf(sql, "SELECT lastlogintime, onlinetime FROM role WHERE roleid = %u", it->first);
        if (ExecSQL())
        {
            if ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                unsigned int nLoginTime = strtoul(row[nField++], &pStrStop, 10);
                unsigned int nOnlineTime = strtoul(row[nField++], &pStrStop, 10);

                *(it->second) = nLoginTime + nOnlineTime;
            }
        }
        SafeFreeResult();
    }

    return Query_Res_OK;
}

int CDBInterface::LoadRoleClothGroup(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if ( pRole == NULL )
        return nRes;

    pRole->m_listClothGroup.clear();
    sprintf( sql, "select groupid, groupname, hair, face, body, gloves, leg, cap, facial, shoulder, "
        "wing, lefthand, righthand, wrist, hip, socks, feet, skin, nothing, suit "
        "from clothgroup where roleid = %u", pRole->m_nRoleID );

    if ( ExecSQL() )
    {
        while ( (row = FetchRow()) != NULL )
        {
            int i = 0;

            RoleClothGroup roleClothGroup;
            roleClothGroup.SetGroupID( (unsigned char)atoi( row[i++] ) );
            roleClothGroup.SetGroupName( row[i++] );

            for ( unsigned char j = eItemClothType_Hair; j <= eItemClothType_Suit; ++j )
            {
                roleClothGroup.AddCloth( j, (itemtype_t)strtoul( row[i++], NULL, 10 ) );
            }

            pRole->m_listClothGroup.push_back( roleClothGroup );
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::AddFresherQuestReward(unsigned int nRoleID, unsigned char nEventID, unsigned short nQuestID)
{
    return AddEventEntry(nRoleID, nEventID, nQuestID);
}

int CDBInterface::AddFresherQuestBestMusicGrade(unsigned int nRoleID, unsigned char nGrade)
{
    return UpdateEventEntry(nRoleID, eEvent_FresherQuest_BestMusicGrade, nGrade);
}
int CDBInterface::AddMallBuyCount(unsigned int nRoleID, int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_MallBuyCount, nCount);
}

int CDBInterface::AddPlatformPrivilegeReward(unsigned int nRoleID, unsigned short nRewardLevel)
{
    unsigned char eventID = 0;

    switch (nRewardLevel)
    {
    case 1:
        eventID = eEvent_UCVIP_Pri1;
        break;
    case 2:
        eventID = eEvent_UCVIP_Pri2;
        break;
    case 3:
        eventID = eEvent_UCVIP_Pri3;
        break;
    case 4:
        eventID = eEvent_UCVIP_Pri4;
        break;
    case 5:
        eventID = eEvent_UCVIP_Pri5;
        break;
    default: return Query_Res_Fail;
    }

    return AddEventEntry(nRoleID, eventID, 1);
}



int CDBInterface::AddQuestNew(QUESTNEWINFO* pQuestInfo)
{
    int nRes = Query_Res_Fail;
    // 这里music，useitem，functions都是用数字合成的字符串，不存在mysql截断字符，所以不需要用CDBIMysql::EscapeString做转码处理
    sprintf(sql, "insert into questnew set roleid=%u,questid=%d,state=%d,createtime=%u,updatetime=%u,music='%s',useitem='%s',functions='%s',completetimes=%d,refreshertimes=%d,destroytype=0,jsonparam=''",
        pQuestInfo->m_RoleID, (int)pQuestInfo->m_QuestID, (int)pQuestInfo->m_State, pQuestInfo->m_CreateTime, pQuestInfo->m_UpdateTime, pQuestInfo->m_Musics.c_str(), pQuestInfo->m_UseItems.c_str(), pQuestInfo->m_Functions.c_str(),
        (int)pQuestInfo->m_Times, (int)pQuestInfo->m_DayTimes);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateQuestNew(QUESTNEWINFO* pQuestInfo)
{
    int nRes = Query_Res_Fail;
    // 这里music，useitem，functions都是用数字合成的字符串，不存在mysql截断字符，所以不需要用CDBIMysql::EscapeString做转码处理
    sprintf(sql, "update questnew set state=%d,createtime=%u,updatetime=%u,music='%s',useitem='%s',functions='%s',"
        "completetimes=%d,refreshertimes=%d,destroytype=%d,jsonparam='%s' where roleid=%u and questid=%u",
        (int)pQuestInfo->m_State, pQuestInfo->m_CreateTime, pQuestInfo->m_UpdateTime, pQuestInfo->m_Musics.c_str(), pQuestInfo->m_UseItems.c_str(), pQuestInfo->m_Functions.c_str(),
        (int)pQuestInfo->m_Times, (int)pQuestInfo->m_DayTimes, (int)pQuestInfo->m_DestroyType, pQuestInfo->m_strJsonQuestParam.c_str(), pQuestInfo->m_RoleID, (int)pQuestInfo->m_QuestID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::CreateTimeEgg(CreateTimeEggEntry* pInfo)
{
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }
    pInfo->nError = ECreateTimeEggError_OK;

    int nRes = Query_Res_Fail;
    unsigned int nNow = (unsigned int)time(NULL);
    pInfo->m_egg.m_nBeginTime = nNow;
    pInfo->m_egg.m_nEndTime += nNow;
    pInfo->m_egg.m_nDestroyTime += nNow;

    size_t size = pInfo->m_roleinfo.size();
    for (size_t i = 0; i < size; ++i)
    {
        sprintf(sql, "SELECT eggid FROM nsytroledb.plantegg WHERE roleid = %u AND sceneid = %u AND opentype = 0",
            pInfo->m_roleinfo[i].m_nRoleID, pInfo->m_roleinfo[i].m_nSceneID);
        if (ExecSQL()) {
            if (FetchRow() > 0) {
                nRes = Query_Res_OK;
                pInfo->nError = ECreateTimeEggError_ExistInScene;
                pInfo->strError = "DoAmuseAction_ExistInScene";
                SafeFreeResult();
                return nRes;
            }
            else{
                SafeFreeResult();
            }
        }
        else
        {
            nRes = Query_Res_Fail;
        }
    }

    sprintf(sql, "INSERT INTO nsytroledb.timeegg SET sceneid = %u, begintime = %u, endtime = %u, destroytime = %u, relation = %u",
        pInfo->m_egg.m_nSceneID, pInfo->m_egg.m_nBeginTime, pInfo->m_egg.m_nEndTime, pInfo->m_egg.m_nDestroyTime, pInfo->m_egg.m_nRelation);
    if (ExecSQL()) {
        nRes = Query_Res_OK;
    }
    else {
        nRes = Query_Res_Fail;
        pInfo->nError = ECreateTimeEggError_DunplicateEgg;
        pInfo->strError = "DoAmuseAction_DunplicateEgg";
    }
    SafeFreeResult();

    if (nRes == Query_Res_OK && pInfo->nError == ECreateTimeEggError_OK)
    {
        sprintf(sql, "SELECT LAST_INSERT_ID()");
        if (ExecSQL()) {
            if ((row = FetchRow()) > 0) {
                nRes = Query_Res_OK;
                pInfo->m_egg.m_nEggID = (unsigned int)atoi(row[0]);
                for (size_t i = 0; i < size; ++i)
                {
                    pInfo->m_roleinfo[i].m_nEggID = pInfo->m_egg.m_nEggID;
                }
            }
            else
            {
                nRes = Query_Res_Fail;
            }
        }
        SafeFreeResult();
    }

    if (nRes == Query_Res_OK && pInfo->nError == ECreateTimeEggError_OK && pInfo->m_egg.m_nEggID > 0)
    {
        for (size_t i = 0; i < size; ++i)
        {
            char* end = sql;
            sprintf(end, "INSERT INTO nsytroledb.plantegg SET roleid = %u, sceneid = %u, eggid = %u, itemid = %u, itemcount = %u, itemtime = %u, sex = %u, words = '",
                pInfo->m_roleinfo[i].m_nRoleID, pInfo->m_roleinfo[i].m_nSceneID, pInfo->m_roleinfo[i].m_nEggID, pInfo->m_roleinfo[i].m_item.m_nItemType, pInfo->m_roleinfo[i].m_item.m_nItemCount, pInfo->m_roleinfo[i].m_item.m_nValidTime, pInfo->m_roleinfo[i].m_nSex);
            end += strlen(end);
            end += CDBIMysql::EscapeString(end, pInfo->m_roleinfo[i].m_words.c_str(), pInfo->m_roleinfo[i].m_words.length());
            strcat(end, "'");
            end += strlen(end);

            if (ExecSQL(sql, (unsigned int)(end - sql))) {
                nRes = Query_Res_OK;
            }
            else {
                nRes = Query_Res_Fail;
                pInfo->nError = ECreateTimeEggError_ExistInScene;
                pInfo->strError = "DoAmuseAction_ExistInScene";
            }
            SafeFreeResult();
            if (pInfo->nError != ECreateTimeEggError_OK)
            {
                sprintf(sql, "UPDATE nsytroledb.timeegg set opentype = 100 WHERE eggid = %u", pInfo->m_egg.m_nEggID);
                if (ExecSQL())
                {
                    sprintf(sql, "UPDATE nsytroledb.plantegg set opentype = 100 WHERE eggid = %u", pInfo->m_egg.m_nEggID);

                    if (!ExecSQL())
                        LOG_ERROR << "Update plantegg set opentype = 100 failed. eggid = " << pInfo->m_egg.m_nEggID << std::endl;
                }

                break;
            }
        }
    }

    SafeFreeResult();

    return nRes;
}

int CDBInterface::GetTimeEggsByRoleID(QueryRoleTimeEggEntry* pInfo)
{
    if (pInfo == NULL || pInfo->m_nRoleID == 0)
    {
        return Query_Res_Fail;
    }
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT a.eggid, a.sceneid, a.begintime, a.endtime, a.destroytime, a.relation FROM nsytroledb.timeegg a JOIN nsytroledb.plantegg b ON a.eggid = b.eggid WHERE a.opentype = 0 AND b.roleid = %u ORDER BY a.begintime",
        pInfo->m_nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int i = 0;
            TimeEggEntry egg;
            egg.m_nEggID = (unsigned int)atoi(row[i++]);
            egg.m_nSceneID = (unsigned int)atoi(row[i++]);
            egg.m_nBeginTime = (unsigned int)atoi(row[i++]);
            egg.m_nEndTime = (unsigned int)atoi(row[i++]);
            egg.m_nDestroyTime = (unsigned int)atoi(row[i++]);
            egg.m_nRelation = (unsigned int)atoi(row[i++]);
            pInfo->m_eggs.push_back(egg);
        }
    }
    SafeFreeResult();
    for (vector<TimeEggEntry>::iterator it = pInfo->m_eggs.begin();
        it != pInfo->m_eggs.end(); ++it)
    {
        sprintf(sql, "SELECT a.roleid, b.rolename FROM nsytroledb.plantegg a JOIN nsytroledb.role b ON a.roleid=b.roleid WHERE a.eggid=%u AND a.roleid!=%u",
            it->m_nEggID, pInfo->m_nRoleID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            if ((row = FetchRow()) != NULL)
            {
                it->m_nRoleID = (unsigned int)atoi(row[0]);
                it->m_strRoleName = row[1];
            }
            else
            {
                WriteLog(LOGLEVEL_WARNING, "CDBInterface::GetTimeEggsByRoleID broken egg:%u", it->m_nEggID);
            }
        }
        else
        {
            nRes = Query_Res_Fail;
            WriteLog(LOGLEVEL_ERROR, "CDBInterface::GetTimeEggsByRoleID broken egg:%u", it->m_nEggID);
        }
        SafeFreeResult();
    }
    return nRes;
}

int CDBInterface::OpenTimeEgg(OpenTimeEggEntry* pInfo)
{
    if (pInfo == NULL || pInfo->m_nEggID == 0)
    {
        return Query_Res_Fail;
    }
    pInfo->nError = 1;//先置为失败
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT a.roleid, a.sceneid, a.eggid, a.words, a.itemid, a.itemcount, a.itemtime, a.opentype, a.sex, b.rolename FROM nsytroledb.plantegg a JOIN nsytroledb.role b ON a.roleid = b.roleid WHERE a.eggid = %u",
        pInfo->m_nEggID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) > 0)
        {
            RoleTimeEggEntry entry;
            int i = 0;
            entry.m_nRoleID = (unsigned int)atoi(row[i++]);
            entry.m_nSceneID = (unsigned int)atoi(row[i++]);
            entry.m_nEggID = (unsigned int)atoi(row[i++]);
            entry.m_words = row[i++];
            entry.m_item.m_nItemType = (unsigned int)atoi(row[i++]);
            entry.m_item.m_nItemCount = (itemcount_t)atoi(row[i++]);
            entry.m_item.m_nValidTime = (int)strtoul(row[i++], NULL, 10);
            entry.m_nOpenType = (OpenEggType)atoi(row[i++]);
            entry.m_nSex = (unsigned int)atoi(row[i++]);
            entry.m_strRoleName = row[i++];
            pInfo->m_roleinfo.push_back(entry);
        }
    }
    SafeFreeResult();

    if (nRes == Query_Res_OK)
    {
        nRes = Query_Res_Fail;
        //出于做排行榜的目的，暂时不删除
        /*sprintf(sql, "DELETE a.*, b.* FROM nsytroledb.timeegg a, nsytroledb.plantegg b WHERE a.eggid=%u AND b.eggid=%u",
        pInfo->m_nEggID, pInfo->m_nEggID);*/
        sprintf(sql, "UPDATE nsytroledb.timeegg SET opentype = %u WHERE eggid = %u", pInfo->m_nOpenType, pInfo->m_nEggID);
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
            if (GetAffectedRowCount() > 0)
            {
                pInfo->nError = 0;//成功，未在其他服务器开蛋（防止重复开蛋）
            }
            else
            {
                pInfo->nError = 1;
                pInfo->strError = "DoAmuseAction_AlreadyOpen";
            }
        }
        SafeFreeResult();
        if (pInfo->nError == 0)
        {
            for (std::vector<RoleTimeEggEntry>::iterator it = pInfo->m_roleinfo.begin();
                it != pInfo->m_roleinfo.end(); ++it)
            {
                OpenEggType nOpenType = pInfo->m_nOpenType;
                if (nOpenType == OpenEggType_OpenAlone)
                {
                    if (it->m_nRoleID != pInfo->m_nOpenRoleID)
                    {
                        nOpenType = OpenEggType_BeOpen;
                    }
                }
                sprintf(sql, "UPDATE nsytroledb.plantegg SET opentype = %u WHERE roleid = %u AND eggid = %u",
                    (unsigned int)nOpenType, it->m_nRoleID, pInfo->m_nEggID);
                ExecSQL();
                SafeFreeResult();
            }
        }
    }
    return nRes;
}

int CDBInterface::GetTimeEggs(std::map<unsigned int, TimeEggEntry*>* pInfo, unsigned int nEggID)
{
    if (pInfo == NULL)
    {
        return Query_Res_Fail;
    }
    int nRes = Query_Res_Fail;
    if (nEggID == 0)
    {
        sprintf(sql, "SELECT a.eggid, a.sceneid, a.begintime, a.endtime, a.destroytime, a.relation FROM nsytroledb.timeegg a WHERE a.opentype = 0");
    }
    else
    {
        sprintf(sql, "SELECT a.eggid, a.sceneid, a.begintime, a.endtime, a.destroytime, a.relation FROM nsytroledb.timeegg a WHERE a.opentype = 0 AND a.eggid = %u", nEggID);
    }
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int i = 0;
            TimeEggEntry* egg = new TimeEggEntry;
            egg->m_nEggID = (unsigned int)atoi(row[i++]);
            egg->m_nSceneID = (unsigned int)atoi(row[i++]);
            egg->m_nBeginTime = (unsigned int)atoi(row[i++]);
            egg->m_nEndTime = (unsigned int)atoi(row[i++]);
            egg->m_nDestroyTime = (unsigned int)atoi(row[i++]);
            egg->m_nRelation = (unsigned int)atoi(row[i++]);
            pInfo->insert(std::make_pair(egg->m_nEggID, egg));
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::AddEnterConstellationLobbyEvent(unsigned int nRoleID)
{
    return AddEventEntry(nRoleID, eEvent_ConstellationMatch_EnterLobby, 1);
}

int CDBInterface::UpdateConstellationCardInfo(unsigned int nRoleID, unsigned short nIndex, unsigned char nConstellation, unsigned int nCoolDownEndTime)
{
    int nEventID = 0;
    if (nIndex == 0)
    {
        nEventID = eEvent_ConstellationMatch_Card1;
    }
    else if (nIndex == 1)
    {
        nEventID = eEvent_ConstellationMatch_Card2;
    }
    else
    {
        return Query_Res_Fail;
    }

    return UpdateEventEntry4(nRoleID, (unsigned char)nEventID, nConstellation, nCoolDownEndTime);
}

int CDBInterface::UpdateConstellationMatchCount(unsigned int nRoleID, int nMatchCount)
{
    return UpdateEventEntry4(nRoleID, eEvent_ConstellationMatch_Count, nMatchCount, (unsigned int)time(NULL));
}

int CDBInterface::UpdateConstellationRoom(unsigned int nRoleID, unsigned int nRoomID, unsigned int nCreateTime, int nLine)
{
    int nRes = UpdateEventEntry4(nRoleID, eEvent_ConstellationMatch_RoomTime, nRoomID, nCreateTime);

    if (nRes != Query_Res_OK)
    {
        return nRes;
    }

    return UpdateEventEntry(nRoleID, eEvent_ConstellationMatch_RoomLine, nLine);
}

int CDBInterface::LoadFriendDungeonInfo(unsigned int nRoleID, std::vector<DungeonRankInfo> * pRankPlayers)
{
    if (pRankPlayers == NULL)
    {
        return Query_Res_Fail;
    }

    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT a.dungeonid,a.score,a.updatetime,a.roleid,c.rolename FROM nsytroledb.dungeon AS a,nsytroledb.friend AS b, nsytroledb.role AS c WHERE a.roleid = b.friendid AND c.roleid = b.friendid AND b.roleid = %d "
        "UNION ALL SELECT a.dungeonid,a.score,a.updatetime,a.roleid,c.rolename FROM  nsytroledb.dungeon AS a , nsytroledb.role AS c WHERE a.roleid = %d AND c.roleid = a.roleid", nRoleID, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        while ((row = FetchRow()) != NULL)
        {
            int i = 0;
            DungeonRankInfo record;
            record.nParcloseID = (unsigned int)atoi(row[i++]);
            record.nStarCount = (unsigned int)atoi(row[i++]);
            record.nUpdateTime = (unsigned int)atoi(row[i++]);
            record.nRoleID = (unsigned int)atoi(row[i++]);
            record.strRoleName = row[i++];


            std::vector<DungeonRankInfo>::iterator it = find(pRankPlayers->begin(), pRankPlayers->end(), record);
            if (it == pRankPlayers->end())
            {
                pRankPlayers->push_back(record);
            }
            else
            {
                unsigned int nLuckStarCount = record.nStarCount;
                if (it->nParcloseID <= record.nParcloseID)
                {
                    unsigned int nOldValue = it->nStarCount;
                    *it = record;
                    it->nStarCount += nOldValue;
                }
                else
                {
                    it->nStarCount += nLuckStarCount;
                }

            }
        }
    }
    SafeFreeResult();
    return nRes;
}

int CDBInterface::UpateMagicLampInfo(unsigned int nRoleID, const MagicLamp * pMagicLamp)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update magiclamp set crystal = %d, luckstar= %d, magicdust = %d, crystaltime = %d, maxmainline = %u, maxtheme = %u, nextdayresettime = %u,"
        " todaygetvitnum = %d, vipbuyvitnum = %d where roleid = %u",
        pMagicLamp->m_nCrytsal, pMagicLamp->m_nLuckStar, pMagicLamp->m_nMagicDust, pMagicLamp->m_nCrystalRestoreTime, pMagicLamp->m_nMaxMainline, pMagicLamp->m_nMaxTheme,
        pMagicLamp->m_nNextDayResetTime, pMagicLamp->m_nTodayHaveGetVitNum, pMagicLamp->m_nFavorableBuyVitCount, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertMagicLampInfo(unsigned int nRoleID, const MagicLamp * pMagicLamp)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO magiclamp set crystal = %d, luckstar= %d, magicdust = %d, crystaltime = %d, maxmainline = %u, maxtheme = %u, nextdayresettime = %u,"
        " todaygetvitnum = %d, vipbuyvitnum = %d, roleid = %u",
        pMagicLamp->m_nCrytsal, pMagicLamp->m_nLuckStar, pMagicLamp->m_nMagicDust, pMagicLamp->m_nCrystalRestoreTime, pMagicLamp->m_nMaxMainline,
        pMagicLamp->m_nMaxTheme, pMagicLamp->m_nNextDayResetTime, pMagicLamp->m_nTodayHaveGetVitNum, pMagicLamp->m_nFavorableBuyVitCount, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateParcloseInfo(unsigned int nRoleID, const MainlineParcloseRecord * pRecord)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon set score = %u, quickfinishnum = %u, updatetime = %u where roleid = %u and dungeonid = %u", pRecord->nStarCount, pRecord->nQuickFinishNum,
        pRecord->nUpdateTime, nRoleID, pRecord->nParcloseID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateThemeParcloseInfo(unsigned int nRoleID, const ThemeParcloseRecord * pRecord)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon_theme_parclose set starcount = %u, quickfinishnum = %u, updatetime=%u where roleid=%u and themeid = %u and parcloseid = %u", pRecord->nStarCount, pRecord->nQuickFinishNum,
        pRecord->nUpdateTime, nRoleID, pRecord->nThemeID, pRecord->nParcloseID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertParcloseInfo(unsigned int nRoleID, const MainlineParcloseRecord * pRecord)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO dungeon set roleid = %u, dungeonid = %u, isactived = %d, score = %u, updatetime=%u", nRoleID, pRecord->nParcloseID, 1, pRecord->nStarCount, pRecord->nUpdateTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertThemeParcloseInfo(unsigned int nRoleID, const ThemeParcloseRecord * pRecord)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO dungeon_theme_parclose set roleid = %u, themeid = %u, parcloseid = %u, isactived = %d, starcount = %u, updatetime=%u", nRoleID, pRecord->nThemeID, pRecord->nParcloseID, 1, pRecord->nStarCount, pRecord->nUpdateTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertStarRewardIndex(unsigned int nRoleID, unsigned int nIndex)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO dungeon_star_reward set roleid = %u, rewardindex = %u", nRoleID, nIndex);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateDebrisInfo(unsigned int nRoleID, unsigned int nDebrisID, unsigned int nCount)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "Select * from dungeon_debris where roleid = %u and debrisid = %u", nRoleID, nDebrisID);
    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
//            if (nCount >= 0)
            {
                sprintf(sql, "update dungeon_debris set debriscount = %u where roleid = %u and debrisid = %u", nCount, nRoleID, nDebrisID);
                if (ExecSQL())
                {
                    return Query_Res_OK;
                }
            }
        }
        else
        {
            if (nCount > 0)
            {
                sprintf(sql, "insert into  dungeon_debris set debrisid = %u, debriscount = %u , roleid = %u", nDebrisID, nCount, nRoleID);
                if (ExecSQL())
                {
                    return Query_Res_OK;
                }
            }
        }
    }

    return nRes;
}

int CDBInterface::UpdateOfflineFriendContactInfo(unsigned int nRoleID, unsigned int nFriendRoleID, unsigned int nVitNum)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "Select * from dungeon_friend_contact where roleid = %u and friendroleid = %u", nRoleID, nFriendRoleID);
    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
            sprintf(sql, "update dungeon_friend_contact set contactnum = contactnum + 1, begivevitnum = begivevitnum + %u, begivevittime = %u where roleid = %u and friendroleid = %u",
                nVitNum, (unsigned int)time(NULL), nRoleID, nFriendRoleID);
            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
        }
        else
        {
            sprintf(sql, "INSERT INTO dungeon_friend_contact set roleid = %u, friendroleid = %u, contactnum = %u, givevitnum = %u, givevittime = %u, begivevitnum = %u, begivevittime = %u",
                nRoleID, nFriendRoleID, 1, 0, 0, nVitNum, (unsigned int)time(NULL));
            if (ExecSQL())
            {
                nRes = Query_Res_OK;
            }
        }
    }

    return nRes;
}

int CDBInterface::BatchUpdateFriendContactInfo(unsigned int nRoleID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon_friend_contact set givevitnum = %u, givevittime = %u, begivevitnum = %u, begivevittime = %u where roleid = %u", 0, 0, 0, 0, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::ClearMainlineQuickFinishNum(unsigned int nRoleID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon set quickfinishnum = %u where roleid = %u", 0, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::ClearThemeQuickFinishNum(unsigned int nRoleID)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon_theme_parclose set quickfinishnum = %u where roleid = %u", 0, nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::InsertFriendContactInfo(unsigned int nRoleID, const FriendContactInfo * pFriendContactInfo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO dungeon_friend_contact set roleid = %u, friendroleid = %u, contactnum = %d,  givevitnum = %u, givevittime = %u, begivevitnum = %u, begivevittime = %u", nRoleID,
        pFriendContactInfo->m_nFriendRoleID, pFriendContactInfo->m_nContactNum, pFriendContactInfo->m_nGiveVitNum, pFriendContactInfo->m_nGiveVitTime,
        pFriendContactInfo->m_nBeGiveVitNum, pFriendContactInfo->m_nBeGiveVitTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateFriendContactInfo(unsigned int nRoleID, const FriendContactInfo * pFriendContactInfo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update dungeon_friend_contact set contactnum = %u, givevitnum = %u, givevittime = %u, begivevitnum = %u, begivevittime = %u where roleid = %u and friendroleid = %u",
        pFriendContactInfo->m_nContactNum, pFriendContactInfo->m_nGiveVitNum, pFriendContactInfo->m_nGiveVitTime, pFriendContactInfo->m_nBeGiveVitNum,
        pFriendContactInfo->m_nBeGiveVitTime, nRoleID, pFriendContactInfo->m_nFriendRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::AddDanceGroupReward(unsigned int nRoleId, unsigned int nTime)
{
    return UpdateEventEntry4(nRoleId, eEvent_DanceGroupFirstAddPowerReward, 1, nTime);
}//end

int CDBInterface::DelDanceGroupReward(unsigned int nRoleId)
{
    sprintf(sql, "delete from event where roleid=%u and eventid=%d", nRoleId, eEvent_DanceGroupFirstAddPowerReward);

    int nRes = Query_Res_Fail;
    if (nRoleId <= 0)
    {
        return nRes;
    }
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;

}//end

int CDBInterface::GetRoleIDByAccountID(unsigned int nAccount, unsigned int& nRoleID)
{
    sprintf(sql, "select roleid from role where account = %u", nAccount);
    int nRes = Query_Res_Fail;
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
        if ((row = FetchRow()) != NULL)
        {
            nRoleID = (unsigned int)strtoul(row[0], &pStrStop, 10);
        }
    }
    return nRes;
}

int CDBInterface::LoadSocialShare(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (pRole != NULL)
    {
        pRole->m_mapSocialShareFirstTime.clear();
        sprintf(sql, "SELECT uiid,firsttime FROM socialshare WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            nRes = Query_Res_Empty;
            while ((row = FetchRow()) != NULL)
            {
                int uiid = atoi(row[0]);
                unsigned int firsttime = strtoul(row[1], NULL, 10);
                pRole->m_mapSocialShareFirstTime[uiid] = firsttime;
            }
            nRes = Query_Res_OK;
        }
    }
    return nRes;
}

int CDBInterface::LoadMonthCard(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (pRole != NULL)
    {
        pRole->m_mapMonthCard.clear();
        sprintf(sql, "SELECT cardtype, opentime, rewardtime FROM monthcard WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            nRes = Query_Res_Empty;
            while ((row = FetchRow()) != NULL)
            {
                MonthCardDB info;
                info.nCardType = strtoul(row[0], NULL, 10);
                info.nOpenTime = strtoul(row[1], NULL, 10);
                info.nRewardTime = strtoul(row[2], NULL, 10);

                pRole->m_mapMonthCard.insert(std::make_pair(info.nCardType, info));
            }
            nRes = Query_Res_OK;
        }
    }
    return nRes;
}

int CDBInterface::UpdateMonthCardInfo(unsigned int nRoleID, const MonthCardDB * pMonthCardInfo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "update monthcard set opentime = %u, rewardtime =%u where roleid=%u and cardtype = %d", pMonthCardInfo->nOpenTime, pMonthCardInfo->nRewardTime, nRoleID, pMonthCardInfo->nCardType);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::InsertMonthCardInfo(unsigned int nRoleID, const MonthCardDB * pMonthCardInfo)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "INSERT INTO monthcard set roleid = %u, cardtype = %d, opentime = %u, rewardtime =%u", nRoleID, pMonthCardInfo->nCardType,
        pMonthCardInfo->nOpenTime, pMonthCardInfo->nRewardTime);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::SaveSocialShare(unsigned int nRoleId, int nUI, unsigned int nTime)
{
    int nRes = Query_Res_Fail;
    sprintf(sql, "SELECT * FROM socialshare WHERE roleid = %u AND uiid = %d", nRoleId, nUI);
    if (ExecSQL())
    {
        if (mysql_num_rows(result) > 0)
        {
            sprintf(sql, "UPDATE socialshare SET firsttime=%u WHERE roleid = %u AND uiid = %d", nTime, nRoleId, nUI);
        }
        else
        {
            sprintf(sql, "INSERT INTO socialshare SET roleid = %u, uiid = %d, firsttime = %u", nRoleId, nUI, nTime);
        }
        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    // log_SocialShare
    sprintf(sql, "INSERT INTO nsytsublogdb.log_SocialShare(role_id, uiid, time) VALUES(%u, %d, %u)", nRoleId, nUI, nTime);
    ExecSQL();

    return nRes;
}

int CDBInterface::LoadLimitedEdition(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (NULL != pRole)
    {
        sprintf(sql, "SELECT idx, endTime FROM SpecialMall WHERE eType = %d AND roleID = %u AND endTime >= %u",
            ESpecialMallType_LimitedEdition, pRole->m_nRoleID, (unsigned int)time(NULL));

        if (ExecSQL())
        {
            nRes = Query_Res_Empty;

            while (NULL != (row = FetchRow()))
            {
                unsigned short idx = (unsigned short)atoi(row[0]);
                unsigned int endTime = strtoul(row[1], NULL, 10);

                pRole->m_mapLimitedEditionBuy[idx] = endTime;
            }

            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::LoadAuctionNotGiveInfo(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;

    if (NULL != pRole)
    {
        sprintf(sql, "SELECT logID, idx, bidPrice, endTime FROM AuctionBid "
            "WHERE roleID = %u AND state = %d",
            pRole->m_nRoleID, (int)EAuctionBidState_NotReturnMoney);

        if (ExecSQL())
        {
            nRes = Query_Res_Empty;
            int i = 0;
            stringstream ss;

            // 查询并记录未返还M币信息
            while (NULL != (row = FetchRow()))
            {
                i = 0;
                ss.clear();
                ss.str("");

                CAuctionNotGivePointInfo info;
                ss << row[i++];
                ss >> info.m_lLogID;
                if (NULL != row[i])
                    info.m_uIdx = (unsigned short)atoi(row[i++]);
                if (NULL != row[i])
                    info.m_uBidPrice = (unsigned int)atoi(row[i++]);
                if (NULL != row[i])
                    info.m_tEndTime = (time_t)strtoul(row[i++], NULL, 10);

                pRole->m_mapAuctionNotGiveInfo.insert(std::make_pair(info.m_lLogID, info));
            }

            bool bAlreadyFoundTop = false;
            std::set<long long> setTopPrice; // key logID
            std::map<long long, CAuctionNotGivePointInfo>::const_iterator mapItr;

            for (std::map<long long, CAuctionNotGivePointInfo>::iterator itr = pRole->m_mapAuctionNotGiveInfo.begin();
                itr != pRole->m_mapAuctionNotGiveInfo.end(); ++itr)
            {
                bAlreadyFoundTop = false;

                // 判断同一活动同一物品自己是否已经是最高出价人
                if (setTopPrice.find(itr->first) != setTopPrice.end())
                {
                    mapItr = pRole->m_mapAuctionNotGiveInfo.find(itr->first);

                    if (mapItr != pRole->m_mapAuctionNotGiveInfo.end() &&
                        mapItr->second.m_tEndTime == itr->second.m_tEndTime &&
                        mapItr->second.m_uIdx == itr->second.m_uIdx)
                    {
                        bAlreadyFoundTop = true;
                    }
                }

                // 自己已经是最高出价人，并且还有未返还的M币，则直接返还M币
                // 不是，则查询数据库
                if (!bAlreadyFoundTop)
                {
                    // 1.非自己
                    // 2.同一活动
                    // 3.同一物品
                    // 4.非返还状态
                    // 5.价格比自己高
                    // 只要有满足上面5个条件的记录，则自己不是最高出价人
                    sprintf(sql, "SELECT logID FROM AuctionBid "
                        "WHERE roleID <> %u AND endTime = %u AND idx = %u "
                        "AND state <> %d AND bidPrice > %u limit 1",
                        pRole->m_nRoleID, (unsigned int)itr->second.m_tEndTime, (unsigned int)itr->second.m_uIdx,
                        (int)EAuctionBidState_ReturnedMoney, itr->second.m_uBidPrice);

                    // 不满足，则标记自己是最高出价人
                    if (ExecSQL() && mysql_num_rows(result) == 0)
                    {
                        itr->second.m_bTopPrice = true;
                        setTopPrice.insert(itr->first);
                    }
                }
            }

            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::SaveSpecialMall(CSpecialMallInfoDB *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        time_t tNow = time(NULL);

        sprintf(sql, "INSERT INTO SpecialMall(roleID, idx, itemType, itemCount, validTime, "
            "buyType, buyPrice, usedMoney, usedBindPt, usedPt, eType, endTime, time) "
            "VALUES(%u, %u, %u, %u, %d, %d, %u, %u, %u, %u, %d, %u, %u)",
            pInfo->m_uRoleID, (unsigned int)pInfo->m_uIdx, (unsigned int)pInfo->m_uItemType,
            (unsigned int)pInfo->m_uItemCount, pInfo->m_nValidTime, (int)pInfo->m_nBuyType,
            pInfo->m_uBuyPrice, pInfo->m_uUsedMoney, pInfo->m_uUsedBindPt, pInfo->m_uUsedPt,
            pInfo->m_eType, (unsigned int)pInfo->m_tEndTime, (unsigned int)tNow);

        if (ExecSQL())
            nRes = Query_Res_OK;

        if (ESpecialMallType_Auction == pInfo->m_eType && 0 == pInfo->m_uRoleID)
        {
            // 无人竞拍，则保存到AuctionBid中，服务器异常重启会读取该信息
            sprintf(sql, "INSERT INTO AuctionBid(roleID, roleName, idx, itemType, itemCount, validTime, bidPrice, state, endTime, time) "
                "VALUES(0, '', %u, %u, %u, %d, 0, %d, %u, %u)", (unsigned int)pInfo->m_uIdx, (unsigned int)pInfo->m_uItemType,
                (unsigned int)pInfo->m_uItemCount, pInfo->m_nValidTime, (int)EAuctionBidState_BidSuccess, (unsigned int)pInfo->m_tEndTime, (unsigned int)tNow);

            ExecSQL();
        }
    }

    return nRes;
}

int CDBInterface::SaveAuctionBid(CAuctionBidInfoDB *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        char *end = sql;

        sprintf(end, "INSERT INTO AuctionBid(roleID, roleName, idx, itemType, itemCount, validTime, bidPrice, state, endTime, time) "
            "VALUES(%u, '", pInfo->m_uRoleID);
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, pInfo->m_strRoleName.c_str(), pInfo->m_strRoleName.length());
        end += strlen(end);
        sprintf(end, "', %u, %u, %u, %d, %u, %d, %u, %u)", (unsigned int)pInfo->m_uIdx, (unsigned int)pInfo->m_uItemType,
            (unsigned int)pInfo->m_uItemCount, pInfo->m_nValidTime, pInfo->m_uBidPrice, EAuctionBidState_NotReturnMoney,
            (unsigned int)pInfo->m_tEndTime, (unsigned int)time(NULL));

        if (ExecSQL())
            nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::SaveGroupBuyingCount(CSaveGroupBuyingCount *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        sprintf(sql, "SELECT logID FROM GroupBuyingCount WHERE idx = %u AND endTime = %u", (unsigned int)pInfo->m_uIdx, (unsigned int)pInfo->m_tEndTime);

        if (ExecSQL())
        {
            if (NULL != (row = FetchRow()))
            {
                sprintf(sql, "UPDATE GroupBuyingCount SET totalCount = %u, time = %u WHERE logID = %s",
                    pInfo->m_uCount, (unsigned int)time(NULL), row[0]);
            }
            else
            {
                sprintf(sql, "INSERT INTO GroupBuyingCount(idx, totalCount, endTime, time) VALUES(%u, %u, %u, %u)",
                    (unsigned int)pInfo->m_uIdx, pInfo->m_uCount, (unsigned int)pInfo->m_tEndTime, (unsigned int)time(NULL));
            }

            if (ExecSQL())
                nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::ChangeAuctionState(long long nLogID, unsigned int nRoleID, unsigned short nIdx, unsigned int nEndTime, EAuctionBidState eState, unsigned int nPrice, bool &bSuccess)
{
    int nRes = Query_Res_Fail;
    bSuccess = false;
    unsigned int nNow = (unsigned int)time(NULL);

    sprintf(sql, "call change_auction_state(%lld, %u, %u, %u, %d, %u, %u, @out_is_changed)",
        nLogID, nRoleID, (unsigned int)nIdx, nEndTime, (int)eState, nPrice, nNow);

    if (ExecSQL())
    {
        sprintf(sql, "select @out_is_changed");

        if (ExecSQL() && NULL != (row = FetchRow()))
        {
            bSuccess = (atoi(row[0]) != 0) ? true : false;

            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::GetGroupBuyingCount(std::list<CGetLimitedSalesCount> *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        pInfo->clear();

        sprintf(sql, "SELECT idx, totalCount FROM GroupBuyingCount WHERE endTime >= %u", (unsigned int)time(NULL));

        if (ExecSQL())
        {
            nRes = Query_Res_OK;

            while (NULL != (row = FetchRow()))
            {
                CGetLimitedSalesCount groupCount;
                groupCount.m_uIdx = (unsigned short)atoi(row[0]);
                groupCount.m_uCount = atoi(row[1]);

                pInfo->push_back(groupCount);
            }
        }
    }

    return nRes;
}

int CDBInterface::GetLimitedEditionCount(std::list<CGetLimitedSalesCount> *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        pInfo->clear();

        sprintf(sql, "SELECT idx, count(*) FROM SpecialMall WHERE eType = %d AND endTime >= %u GROUP BY idx",
            ESpecialMallType_LimitedEdition, (unsigned int)time(NULL));

        if (ExecSQL())
        {
            nRes = Query_Res_OK;

            while (NULL != (row = FetchRow()))
            {
                CGetLimitedSalesCount groupCount;
                groupCount.m_uIdx = (unsigned short)atoi(row[0]);
                groupCount.m_uCount = atoi(row[1]);

                pInfo->push_back(groupCount);
            }
        }
    }

    return nRes;
}

int CDBInterface::GetAuctionBidInfo(std::map<unsigned short, CGetAuctionBidInfo> *pInfo)
{
    int nRes = Query_Res_Fail;

    if (NULL != pInfo)
    {
        pInfo->clear();

        sprintf(sql, "SELECT * FROM ( SELECT idx, state, bidPrice, roleID, roleName, time "
            "FROM AuctionBid WHERE state <> %d AND endTime >= %u ORDER BY time DESC) AS orderBySelf "
            "GROUP BY idx", (int)EAuctionBidState_ReturnedMoney, (unsigned int)time(NULL));

        if (ExecSQL())
        {
            nRes = Query_Res_OK;

            int i = 0;
            EAuctionBidState eState;

            while (NULL != (row = FetchRow()))
            {
                i = 0;
                CGetAuctionBidInfo bidInfo;
                bidInfo.m_uIdx = (unsigned short)atoi(row[i++]);
                eState = (EAuctionBidState)atoi(row[i++]);
                bidInfo.m_uCurPrice = (unsigned int)atoi(row[i++]);
                bidInfo.m_uRoleID = (unsigned int)atoi(row[i++]);
                bidInfo.m_strRoleName = row[i++];
                bidInfo.m_nTime = (unsigned int)strtoul(row[i++], NULL, 10);

                if (EAuctionBidState_NotReturnMoney != eState)
                    bidInfo.m_nTime = 0;

                pInfo->insert(std::map<unsigned short, CGetAuctionBidInfo>::value_type(bidInfo.m_uIdx, bidInfo));
            }
        }
    }

    return nRes;
}

int CDBInterface::GetRankRoleData(RankGetRoleDataDB *pRoleDataDB)
{
    int nRes = Query_Res_Fail;

    if ( pRoleDataDB == NULL || pRoleDataDB->m_nKey == 0 )
        return nRes;

    std::string strSQL;
    pRoleDataDB->GetSQL( strSQL );

    if ( !strSQL.empty() && ExecSQL( strSQL.c_str(), strSQL.length() ) )
    {
        nRes = Query_Res_OK;

        if ( ( row = FetchRow() ) != NULL )
        {
            pRoleDataDB->DecodeByRow( (const char **)row );
        }
    }

    return nRes;
}

int CDBInterface::GetRankAllDatas(RankGetAllDatasDB *pAllDatasDB)
{
    int nRes = Query_Res_Fail;

    if ( pAllDatasDB == NULL )
        return nRes;

    std::string strSQL;
    pAllDatasDB->GetSQL( strSQL );

    if ( !strSQL.empty() && ExecSQL( strSQL.c_str(), strSQL.length() ) )
    {
        nRes = Query_Res_OK;

        while ( ( row = FetchRow() ) != NULL )
        {
            pAllDatasDB->DecodeByRow( (const char **)row );
        }
    }

    return nRes;
}

int CDBInterface::SaveRankData(RankSaveDataDB *pSaveDataDB)
{
    int nRes = Query_Res_Fail;

    if ( pSaveDataDB == NULL )
        return nRes;

    std::string strSQL;
    pSaveDataDB->GetSQL( strSQL );

    if ( !strSQL.empty() && ExecSQL( strSQL.c_str(), strSQL.length() ) )
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::UpdateRankDataState(CRankUpdateDataStateDB *pUpdateDataStateDB)
{
    int nRes = Query_Res_Fail;

    if (NULL == pUpdateDataStateDB)
        return nRes;

    switch (pUpdateDataStateDB->m_nDataType)
    {
    case ERankDataType_Group_Honor:
    {
                                      sprintf(sql, "SELECT createtime FROM rank_grouphonor WHERE createtime = %u AND groupid = %u",
                                          pUpdateDataStateDB->m_nTime, pUpdateDataStateDB->m_nKey);

                                      if (ExecSQL())
                                      {
                                          if (mysql_num_rows(result) > 0)
                                          {
                                              sprintf(sql, "UPDATE rank_grouphonor SET dismissed = 1 WHERE createtime = %u AND groupid = %u",
                                                  pUpdateDataStateDB->m_nTime, pUpdateDataStateDB->m_nKey);
                                          }
                                          else
                                          {
                                              sprintf(sql, "INSERT INTO rank_grouphonor(groupid, createtime, total, updatetime, dismissed) "
                                                  "VALUES(%u, %u, 0, %u, 1)", pUpdateDataStateDB->m_nKey, pUpdateDataStateDB->m_nTime,
                                                  (unsigned int)time(NULL));
                                          }
                                      }
    }
        break;

    case ERankDataType_Intimacy:
    {
                                   sprintf(sql, "SELECT createtime FROM rank_intimacy WHERE createtime = %u AND pairid = %u",
                                       pUpdateDataStateDB->m_nTime, pUpdateDataStateDB->m_nKey);

                                   if (ExecSQL())
                                   {
                                       if (mysql_num_rows(result) > 0)
                                       {
                                           sprintf(sql, "UPDATE rank_intimacy SET divorced = 1 WHERE createtime = %u AND pairid = %u",
                                               pUpdateDataStateDB->m_nTime, pUpdateDataStateDB->m_nKey);
                                       }
                                       else
                                       {
                                           sprintf(sql, "INSERT INTO rank_intimacy(pairid, createtime, total, updatetime, divorced) "
                                               "VALUES(%u, %u, 0, %u, 1)", pUpdateDataStateDB->m_nKey, pUpdateDataStateDB->m_nTime,
                                               (unsigned int)time(NULL));
                                       }
                                   }
    }
        break;

    default:
        return nRes;
        break;
    }

    if (ExecSQL())
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::SaveRankHistory( RankSaveHistoryDB *pSaveHistoryDB )
{
    int nRes = Query_Res_Fail;

    if ( pSaveHistoryDB == NULL )
        return nRes;

    std::string strSQL;
    pSaveHistoryDB->GetSQL( strSQL );

    if ( !strSQL.empty() && ExecSQL( strSQL.c_str(), strSQL.length() ) )
        nRes = Query_Res_OK;

    return nRes;
}

int CDBInterface::GMPrivSendMCoin(std::vector<PlayerMoneyInfo>* pPlayerMoneyInfo, std::vector<GMRechargeResult>* pPlayerResultList)
{
    int nRes = Query_Res_Fail;

    std::vector<PlayerMoneyInfo>& playerMoneyInfo = *pPlayerMoneyInfo;
    for (size_t i = 0; i < playerMoneyInfo.size(); i++)
    {
        sprintf(sql, "update nsytaccountdb.bill set pt = pt + %d , bindpt = bindpt + %d , totalpt = totalpt + %d , totalbindpt = totalbindpt + %d where account = %u",
            playerMoneyInfo[i].m_nMoney, playerMoneyInfo[i].m_nBindMoney, playerMoneyInfo[i].m_nMoney, playerMoneyInfo[i].m_nBindMoney,
            (unsigned int)atoi(playerMoneyInfo[i].m_PlayerPUID.c_str()));
        if (ExecSQL())
        {
            (*pPlayerResultList)[i].m_Result = true;
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::LoadBlackDevice(std::list<std::string> &blackList)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT deviceid FROM device_blacklist");

    if (ExecSQL())
    {
        while (NULL != (row = FetchRow()))
        {
            if (row[0] != NULL)
            {
                blackList.push_back(std::string(row[0]));
            }
        }
    }

    return nRes;
}

int CDBInterface::InsertBlackDevice(std::vector<std::string>* pDeviceList, std::string* strGM)
{
    int nRes = Query_Res_Fail;
    memset(sql, 0, MAX_SQL_LEN);
    std::vector<std::string>::iterator it = pDeviceList->begin();
    for (; it != pDeviceList->end(); it++)
    {
        char *end = sql;
        sprintf(end, "INSERT INTO nsytaccountdb.device_blacklist set createtime = %d , deviceid = '", (int)time(NULL));
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, it->c_str(), (unsigned long)it->size());
        strcat(end, "', creator = '");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, strGM->c_str(), (unsigned long)strGM->size());
        strcat(end, "'");
        end += strlen(end);

        if (ExecSQL(sql, end - sql))
        {
            nRes = Query_Res_OK;
        }
    }
    return nRes;
}

int CDBInterface::ChangeUserPassword(std::string* pAccount, std::string* pPassword)
{
    int nRes = Query_Res_Fail;
    memset(sql, 0, MAX_SQL_LEN);
    char* end = sql;
    sprintf(end, "update nsytaccountdb.player set passwd = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pPassword->c_str(), (unsigned long)pPassword->size());
    strcat(end, "'  where username = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, pAccount->c_str(), (unsigned long)pAccount->size());
    strcat(end, "'");
    end += strlen(end);

    if (ExecSQL(sql, end - sql))
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::DeleteBlackDevice(std::vector<std::string>* pDeviceList)
{
    int nRes = Query_Res_Fail;
    memset(sql, 0, MAX_SQL_LEN);
    std::vector<std::string>::iterator it = pDeviceList->begin();
    for (; it != pDeviceList->end(); it++)
    {
        char *end = sql;
        sprintf(end, "delete from nsytaccountdb.device_blacklist where deviceid = '");
        end += strlen(end);
        end += CDBIMysql::EscapeString(end, it->c_str(), (unsigned long)it->size());
        strcat(end, "'");
        end += strlen(end);

        if (ExecSQL(sql, end - sql))
        {
            nRes = Query_Res_OK;
        }
    }
    return nRes;
}

int CDBInterface::UpdateRecommendCount(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry(nRoleID, eEvent_RecommendFriends, nCount);
}

int CDBInterface::UpdateRecommendResetTime(unsigned int nRoleID, unsigned int nCount)
{
    return UpdateEventEntry4(nRoleID, eEvent_RecommendFriends, nCount, (unsigned int)time(NULL));
}

int CDBInterface::UpdateRedEnvelopeAnoymityEvent(int nRoleID, int nAnonymity)
{
    return UpdateEventEntry4(nRoleID, eEvent_RedEnvelope_Anonymity, nAnonymity, (unsigned int)time(NULL));
}

int CDBInterface::LoadRoleActiveness(unsigned int nRoleID, RoleActivenessDB &activeness)
{
    int nRes = Query_Res_Fail;

    activeness = RoleActivenessDB();
    sprintf(sql, "SELECT update_time, score, reward_level_score, target_progress, dance_mode "
        "         FROM activeness WHERE roleid=%u", nRoleID);
    if (ExecSQL())
    {
        row = FetchRow();
        if (row != NULL && mysql_num_rows(result) > 0)
        {
            activeness.m_nRoleID = nRoleID;
            int nField = 0;
            activeness.m_nUpdateTime = (unsigned int)strtoul(row[nField++], NULL, 10);
            activeness.m_nScore = atoi(row[nField++]);
            const char *strScore = row[nField++];
            const char *strProgress = row[nField++];
            const char *strDanceMode = row[nField++];
            activeness.m_strRewardLevelScore = (strScore != NULL ? strScore : "");
            activeness.m_strTargetProgress = (strProgress != NULL ? strProgress : "");
            activeness.m_strDanceMode = (strDanceMode != NULL ? strDanceMode : "");
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::UpdateActivenessScore(unsigned int nRoleID, int nNewScore)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT score FROM activeness WHERE roleid=%u", nRoleID);
    if (ExecSQL())
    {
        row = FetchRow();
        if (row != NULL && mysql_num_rows(result) > 0)
        {
            sprintf(sql, "UPDATE activeness SET score = %d, update_time=%u WHERE roleid=%u",
                nNewScore, (unsigned int)time(NULL), nRoleID);
        }
        else
        {
            sprintf(sql, "INSERT INTO activeness SET score = %d, update_time=%u, roleid=%u",
                nNewScore, (unsigned int)time(NULL), nRoleID);
        }

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::UpdateActivenessTargetProgress(unsigned int nRoleID, const std::string &strTargetProgress, const std::string &strDanceMode)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "UPDATE activeness SET target_progress='%s', dance_mode='%s', update_time=%u WHERE roleid=%u",
        strTargetProgress.c_str(), strDanceMode.c_str(), (unsigned int)time(NULL), nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateActivenessReward(unsigned int nRoleID, const std::string &strScoreList)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "UPDATE activeness SET reward_level_score='%s' WHERE roleid=%u",
        strScoreList.c_str(), nRoleID);
    if (ExecSQL())
    {
        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateOrInsertActiveness(unsigned int nRoleID, RoleActivenessDB *pActiveness)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "SELECT score FROM activeness WHERE roleid=%u", nRoleID);
    if (ExecSQL())
    {
        row = FetchRow();
        if (row != NULL && mysql_num_rows(result) > 0)
        {
            sprintf(sql, "UPDATE activeness "
                "SET score = %d, update_time=%u, reward_level_score = \"%s\", target_progress = \"%s\", dance_mode = \"%s\" "
                "WHERE roleid=%u",
                pActiveness->m_nScore, pActiveness->m_nUpdateTime,
                pActiveness->m_strRewardLevelScore.c_str(), pActiveness->m_strTargetProgress.c_str(), pActiveness->m_strDanceMode.c_str(),
                nRoleID);
        }
        else
        {
            sprintf(sql, "INSERT INTO activeness "
                "SET roleid=%u, score = %d, update_time=%u, "
                "reward_level_score = \"%s\", target_progress = \"%s\", dance_mode = \"%s\"",
                nRoleID, pActiveness->m_nScore, pActiveness->m_nUpdateTime,
                pActiveness->m_strRewardLevelScore.c_str(), pActiveness->m_strTargetProgress.c_str(), pActiveness->m_strDanceMode.c_str());
        }

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}


int CDBInterface::LoadPetIncubatorInfo(unsigned int nRoleID, unsigned short &rLevel)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select level from pet_incubator where roleid = %u", nRoleID);

    if (ExecSQL() && (row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
    {
        rLevel = (unsigned short)atoi(row[0]);

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdatePetIncubatorLevel(unsigned int nRoleID, unsigned short nLevel)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select incubatorid from pet_incubator where roleid = %u", nRoleID);

    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
        {
            sprintf(sql, "update pet_incubator set level = %u where incubatorid = %s",
                (unsigned int)nLevel, row[0]);
        }
        else
        {
            sprintf(sql, "insert into pet_incubator(roleid, level) values(%u, %u)",
                nRoleID, (unsigned int)nLevel);
        }

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::LoadPetEggInfo(unsigned int nRoleID, PetEgg &rPetEgg)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select eggid, state, finishedtime, eggtypeid "
        "from pet_egg where roleid = %u and state <> %d", nRoleID, (int)ePetEggState_Opened);

    if (ExecSQL() && (row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
    {
        int nField = 0;

        unsigned int nEggID = (unsigned int)strtoul(row[nField++], NULL, 10);
        unsigned char nState = (unsigned char)atoi(row[nField++]);
        unsigned int nFinishedTime = (unsigned int)strtoul(row[nField++], NULL, 10);
        unsigned short nEggTypeID = (unsigned short)atoi(row[nField++]);

        rPetEgg.SetID(nEggID);
        rPetEgg.SetState(nState);
        rPetEgg.SetFinishedTime(nFinishedTime);
        rPetEgg.SetTypeID(nEggTypeID);

        nRes = Query_Res_OK;

        unsigned int nNow = (unsigned int)time(NULL);

        if (rPetEgg.GetState() == ePetEggState_Incubating && rPetEgg.GetFinishedTime() <= nNow)
        {
            rPetEgg.SetState(ePetEggState_Finished);

            sprintf(sql, "update pet_egg set state = %u where eggid = %u",
                (unsigned int)rPetEgg.GetState(), rPetEgg.GetID());

            ExecSQL();
        }
    }

    return nRes;
}

int CDBInterface::CreatePetEggInfo(unsigned int nRoleID, PetEgg &rPetEgg)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "insert into pet_egg(roleid, finishedtime, eggtypeid) values(%u, %u, %u)",
        nRoleID, (unsigned int)rPetEgg.GetFinishedTime(), (unsigned int)rPetEgg.GetTypeID());

    if (ExecSQL())
    {
        unsigned int nEggID = (unsigned int)mysql_insert_id(&m_mysqlConnection);
        rPetEgg.SetID(nEggID);

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdatePetEggState(unsigned int nEggID, unsigned char nState)
{
    sprintf(sql, "update pet_egg set state = %u where eggid = %u",
        (unsigned int)nState, nEggID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadPetExtendGridCount(unsigned int nRoleID, unsigned short &rCount)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select extendcount from pet_extend_grid where roleid = %u", nRoleID);

    if (ExecSQL() && (row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
    {
        rCount = (unsigned short)atoi(row[0]);

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdatePetExtendGridCount(unsigned int nRoleID, unsigned short nCount)
{
    int nRes = Query_Res_Fail;

    sprintf(sql, "select roleid from pet_extend_grid where roleid = %u", nRoleID);

    if (ExecSQL())
    {
        if ((row = FetchRow()) != NULL && mysql_num_rows(result) > 0)
        {
            sprintf(sql, "update pet_extend_grid set extendcount = %u where roleid = %u",
                (unsigned int)nCount, nRoleID);
        }
        else
        {
            sprintf(sql, "insert into pet_extend_grid(roleid, extendcount) values(%u, %u)",
                nRoleID, (unsigned int)nCount);
        }

        if (ExecSQL())
        {
            nRes = Query_Res_OK;
        }
    }

    return nRes;
}

int CDBInterface::LoadPetsInfo(unsigned int nRoleID, std::map<unsigned int, PetDB, std::greater<unsigned int> > &rPets)
{
    int nRes = Query_Res_Fail;

    rPets.clear();
    sprintf(sql,
        "select petid, state, name, potential, level, exp, endurance, endurance_recovery_time, attributetype, "
        "       qualitytype, evolutiontimes, pettypeid, skill_list, reset_skill_time, reset_skill_times "
        "  from pet where roleid = %u ", nRoleID);
    ///" where roleid = %u and state <> %u", nRoleID, (unsigned int)ePetState_Destroy);

    if (ExecSQL())
    {
        int nField = 0;
//         Pet *pPet = NULL;

//         unsigned int nPetID = 0;
//         unsigned char nState = 0;
//         std::string strName;
//         unsigned char nPotentialID = 0;
//         unsigned short nLevel = 0;
//         unsigned int nExp = 0;
//         unsigned short nEndurance = 0;
//         unsigned int nEnduranceRecoveryTime = 0;
//         unsigned char nAttributeType = 0;
//         unsigned char nQualityType = 0;
//         unsigned char nEvolutionTimes = 0;
//         unsigned short nPetTypeID = 0;
//         std::string strSkillList;
//         unsigned int nResetSkillTime = 0;
//         int nResetSkillTimes = 0;

        PetDB petDb;
        while ((row = FetchRow()) != NULL)
        {
            nField = 0;

            petDb.nID = (unsigned int)strtoul(row[nField++], NULL, 10);
            petDb.nState = (unsigned char)atoi(row[nField++]);
            petDb.strName = row[nField++];
            petDb.nPotentialID = (unsigned char)atoi(row[nField++]);
            petDb.nLevel = (unsigned short)atoi(row[nField++]);
            petDb.nExp = (unsigned int)strtoul(row[nField++], NULL, 10);
            petDb.nEndurance = (unsigned short)atoi(row[nField++]);
            petDb.nEnduranceRecoveryTime = (unsigned int)strtoul(row[nField++], NULL, 10);
            petDb.nAttributeType = (unsigned char)atoi(row[nField++]);
            petDb.nQualityType = (unsigned char)atoi(row[nField++]);
            petDb.nEvolutionTimes = (unsigned char)atoi(row[nField++]);
            petDb.nTypeID = (unsigned short)atoi(row[nField++]);
            petDb.strSkillList = row[nField++];
            petDb.nResetSkillTime = (unsigned int)strtoul(row[nField++], NULL, 10);
            petDb.nResetSkillTimes = atoi(row[nField++]);

//             pPet->Init(nPetID, nState, strName, nPotentialID, nLevel, nExp, nEndurance, nEnduranceRecoveryTime,
//                 nAttributeType, nQualityType, nEvolutionTimes, nPetTypeID,
//                 strSkillList, nResetSkillTime, nResetSkillTimes);

            rPets.insert(std::make_pair(petDb.nID, petDb));
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::CreatePetInfo(unsigned int nRoleID, Pet &rPet)
{
    int nRes = Query_Res_Fail;

    std::string strSkillList;
    rPet.SaveSkill(strSkillList);

    char *end = sql;
    sprintf(end, "insert into pet(roleid, name, potential, endurance, attributetype, pettypeid, skill_list) "
        "values(%u, '", nRoleID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, rPet.GetName().c_str(), rPet.GetName().size());
    sprintf(end, "', %d, %d, %d, %d, '%s')", (int)rPet.GetPotentialID(), (int)rPet.GetEndurance(),
        (int)rPet.GetAttributeType(), (int)rPet.GetTypeID(), strSkillList.c_str());

    if (ExecSQL())
    {
        unsigned int nPetID = (unsigned int)mysql_insert_id(&m_mysqlConnection);
        rPet.SetID(nPetID);

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::ChangePetName(unsigned int nPetID, const std::string &rNewName)
{
    char *end = sql;
    sprintf(end, "update pet set name = '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, rNewName.c_str(), rNewName.size());
    sprintf(end, "' where petid = %u", nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetState(unsigned int nPetID, unsigned char nState)
{
    sprintf(sql, "update pet set state = %u where petid = %u",
        (unsigned int)nState, nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetQuality(unsigned int nPetID, unsigned char nQualityType)
{
    sprintf(sql, "update pet set qualitytype = %u where petid = %u",
        (unsigned int)nQualityType, nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetEvolution(unsigned int nPetID, unsigned char nEvolutionTimes)
{
    sprintf(sql, "update pet set evolutiontimes = %u where petid = %u",
        (unsigned int)nEvolutionTimes, nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetEndurance(unsigned int nPetID, unsigned short nEndurance, unsigned int nRecoveryTime)
{
    sprintf(sql, "update pet set endurance = %u, endurance_recovery_time = %u where petid = %u",
        (unsigned int)nEndurance, nRecoveryTime, nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetLevelAndExp(unsigned int nPetID, unsigned short nLevel, unsigned int nExp)
{
    sprintf(sql, "update pet set level = %u, exp = %u where petid = %u",
        (unsigned int)nLevel, nExp, nPetID);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdatePetSkill(Pet &rPet)
{
    std::string strSkillList;
    rPet.SaveSkill(strSkillList);

    sprintf(sql, "update pet set skill_list = '%s' where petid = %u",
        strSkillList.c_str(), rPet.GetID());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::ResetPetSkill(Pet &rPet)
{
    std::string strSkillList;
    rPet.SaveSkill(strSkillList);

    sprintf(sql,
        "update pet "
        "   set skill_list = '%s', reset_skill_time=%u, reset_skill_times=%d "
        " where petid = %u",
        strSkillList.c_str(), rPet.GetResetSkillTime(), rPet.GetResetSkillTimes(),
        rPet.GetID());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}


int CDBInterface::CheckRebindInfo(RebindAccountInfo &rebindInfo)
{
    char trancode[MAX_TRANSCODE_LEN] = { 0 };
    CDBIMysql::EscapeString(trancode, rebindInfo.m_strActcode.c_str(), rebindInfo.m_strActcode.length());

    // test if this account is exist; // 新来的账号不能使已经存在的账号
    sprintf(sql, "SELECT account FROM player WHERE pid = %d and puid = \"%s\"", rebindInfo.m_nPid, rebindInfo.m_strPuid.c_str());
    if (!ExecSQL() || (row = FetchRow()) != NULL)
    {
        rebindInfo.m_nResult = Query_Res_AlreadyExist;
        return rebindInfo.m_nResult; // 
    }

    sprintf(sql, "SELECT accountid, pid, puid, roleid FROM tran_code WHERE act_code = \"%s\"", trancode);

    if (ExecSQL() && ((row = FetchRow()) != NULL && mysql_num_rows(result) > 0))
    {
        rebindInfo.m_nAccount = (unsigned int)atoi(row[0]);
        rebindInfo.m_nPidOld = atoi(row[1]);
        rebindInfo.m_strPuidOld = row[2];
        rebindInfo.m_nRoleid = (unsigned int)atoi(row[3]);

        rebindInfo.m_nResult = Query_Res_OK;
    }
    else
    {
        rebindInfo.m_nResult = (unsigned short)Query_Res_Fail;
    }

    return rebindInfo.m_nResult; // 
}

int CDBInterface::UpdateAccountByTrancode(RebindAccountInfo &rebindInfo)
{
    // 更新Player表account信息
    char szSQLPUID[MAX_PUID_LEN] = { 0 };
    char szSQLName[MAX_NAME_LEN] = { 0 };
    char szSQLPass[MAX_PASS_LEN] = { 0 };

    CDBIMysql::EscapeString(szSQLPUID, rebindInfo.m_strPuid.c_str(), rebindInfo.m_strPuid.length());
    CDBIMysql::EscapeString(szSQLName, rebindInfo.m_strName.c_str(), rebindInfo.m_strName.length());
    CDBIMysql::EscapeString(szSQLPass, rebindInfo.m_strPass.c_str(), rebindInfo.m_strPass.length());
    sprintf(sql, "UPDATE player SET pid = %u, puid='%s', username='%s', passwd='%s' WHERE account=%d",
        rebindInfo.m_nPid, szSQLPUID, szSQLName, szSQLPass, rebindInfo.m_nAccount);

    if (ExecSQL())
    {
        rebindInfo.m_nResult = Query_Res_OK;
    }
    else
    {
        rebindInfo.m_nResult = (unsigned short)Query_Res_Fail;
        WriteLog(LOGLEVEL_ERROR, "[%s] execute error...", sql);
        return rebindInfo.m_nResult; // 失败的话，直接返回
    }
    // 数据已经绑定OK，之后的结果不影响返回值

    // trancode 条目的信息删除
    sprintf(sql, "DELETE FROM tran_code WHERE accountid=%d", rebindInfo.m_nAccount);

    if (!ExecSQL())
    {
        WriteLog(LOGLEVEL_ERROR, "[%s] execute error...", sql);
    }

    // tran_code_used 绑定记录
    char szSQLPUIDOld[MAX_PUID_LEN] = { 0 };
    CDBIMysql::EscapeString(szSQLPUIDOld, rebindInfo.m_strPuidOld.c_str(), rebindInfo.m_strPuidOld.length());

    sprintf(sql, "INSERT INTO tran_code_used(accountid, pid, puid, roleid, act_code, new_pid, new_puid) "
        "VALUES(%u, %d, '%s', %u, '%s', %d, '%s')",
        rebindInfo.m_nAccount, rebindInfo.m_nPidOld, szSQLPUIDOld,
        rebindInfo.m_nRoleid, rebindInfo.m_strActcode.c_str(), rebindInfo.m_nPid, szSQLPUID);

    if (!ExecSQL())
    {
        WriteLog(LOGLEVEL_ERROR, "[%s] execute error...", sql);
    }

    return rebindInfo.m_nResult; // 
}

int CDBInterface::CreateMedal(unsigned int nRoleID, MedalUnitDB& unit)
{
    Formatter::Format(sql, sizeof(sql), "insert into medal set roleid = %u, medalid = %d, state = %d, gettime = %d ",
        nRoleID, unit.getID(), unit.getCurState(), unit.getGetTime());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadMedalMap(unsigned int nRoleID, std::map<int, MedalUnitDB> &mapCMedalUint)
{
    int nRes = Query_Res_Fail;

    mapCMedalUint.clear();
    Formatter::Format(sql, sizeof(sql), "select medalid, state, gettime from medal where roleid = %u ", nRoleID);
    if (ExecSQL())
    {
        int nField = 0;
        int medalid = 0;
        int state = 0;
        int gettime = 0;

        while ((row = FetchRow()) != NULL)
        {
            nField = 0;

            medalid = (unsigned int)strtoul(row[nField++], NULL, 10);
            state = atoi(row[nField++]);
            gettime = atoi(row[nField++]);

            MedalUnitDB unit;
            unit.setID( medalid );
            unit.setCurState( state );
            unit.setGetTime( gettime );

            mapCMedalUint.insert( std::make_pair( medalid, unit ) );
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateMedal(unsigned int nRoleID, MedalUnitDB& unit)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO medal(roleid, medalid, state, gettime) "
        "VALUE(%u, %d, %d, %d) ON DUPLICATE KEY UPDATE state = %d, gettime=%d",
        nRoleID, unit.getID(), unit.getCurState(), unit.getGetTime(),
        unit.getCurState(), unit.getGetTime());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::CreateMedalCondition(unsigned int nRoleID, MedalConditionDB& condit)
{
    Formatter::Format(sql, sizeof(sql), "insert into medalprogress "
        "set roleid = %u, progressid = %d, progress_value = %d, progress_aready_reward = %d",
        nRoleID, condit.GetID(), condit.GetCurProgress(), condit.GetAreadyReward());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadMedalCondition(unsigned int nRoleID, std::map<int, MedalConditionDB> &mapCondition)
{
    int nRes = Query_Res_Fail;

    mapCondition.clear();
    Formatter(sql, sizeof(sql)) << "select progressid, progress_value, progress_aready_reward " \
        "from medalprogress where roleid = " << nRoleID;

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;

            int nProgressId = atoi(row[nField++]);
            int nProgress = atoi(row[nField++]);
            int nProgressAreadyReward = atoi(row[nField++]);

            MedalConditionDB cond( nProgressId, nProgress, nProgressAreadyReward );

            mapCondition.insert( std::make_pair( nProgressId, cond ) );
        }

        nRes = Query_Res_OK;
    }

    return nRes;
}

int CDBInterface::UpdateMedalCondition(unsigned int nRoleID, MedalConditionDB& condition)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO medalprogress(roleid, progressid, progress_value, progress_aready_reward) "
        "VALUE(%u, %d, %d, %d) ON DUPLICATE KEY UPDATE progress_value = %d, progress_aready_reward=%d",
        nRoleID, condition.GetID(), condition.GetCurProgress(), condition.GetAreadyReward(),
        condition.GetCurProgress(), condition.GetAreadyReward());

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadMedalScore(unsigned int nRoleID, int & nMedalWearScore, int & nMedalCumulativeScore)
{
    int nRes = Query_Res_Fail;

    Formatter(sql, sizeof(sql)) << "select medal_wear_score, medal_cumulative_score " \
        "from medal_score where roleid = " << nRoleID;

    if (ExecSQL())
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            nMedalWearScore = atoi(row[nField++]);
            nMedalCumulativeScore = atoi(row[nField++]);
        }
        nRes = Query_Res_OK;
    }
    return nRes;
}

int CDBInterface::UpdateMedalScore(unsigned int nRoleID, MedalScoreDB& score)
{
    Formatter::Format(sql, sizeof(sql), "INSERT INTO medal_score(roleid, medal_wear_score, medal_cumulative_score) "
        "VALUE(%u, %u, %u) ON DUPLICATE KEY UPDATE medal_wear_score = %u, medal_cumulative_score=%u",
        nRoleID, score.m_nMedalWearScore, score.m_nMedalCumulativeScore, score.m_nMedalWearScore, score.m_nMedalCumulativeScore);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DeleteMedalCondition(unsigned int nRoleID, MedalConditionDB& condition)
{
    Formatter::Format(sql, sizeof(sql), "delete from medalprogress "
        "where roleid = %u and progressid = %d",
        nRoleID, condition.GetID());
    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DelVersionMedal(std::list<int >& listID)
{
    for (std::list<int >::iterator it = listID.begin(); it != listID.end(); ++it)
    {
        Formatter::Format(sql, sizeof(sql), "update medal "
            "set state = 2 where medalid = %d and state <> 0", *it);

        if (!ExecSQL())
        {
            return Query_Res_Fail;
        }
        SafeFreeResult();
    }
    return Query_Res_OK;
}

int CDBInterface::doCheckAddMedalByGm(unsigned int nRoleID, std::list<int>* pListneedCheckMedalID)
{
    for (std::list<int>::iterator it = pListneedCheckMedalID->begin();
        it != pListneedCheckMedalID->end(); ++it)
    {
        Formatter::Format(sql, sizeof(sql), "update medal "
            "set state = 2 where roleid = %u and (state = 1 or state = 3) and medalid = %d",
            nRoleID, *it);


        if (!ExecSQL())
        {
            return Query_Res_Fail;
        }
        SafeFreeResult();
    }
    return  Query_Res_OK;
}

int CDBInterface::AddMedalByGm(unsigned int nRoleID, int nMedalID, std::list<int>* pListneedCheckMedalID)
{
    unsigned int nNow = (unsigned int)time(NULL);
    if (doCheckAddMedalByGm(nRoleID, pListneedCheckMedalID) == Query_Res_Fail)
    {
        return Query_Res_Fail;
    }

    Formatter::Format(sql, sizeof(sql), "INSERT INTO medal(roleid, medalid, state, gettime) "
        "VALUE(%u, %d, %u, %u) ON DUPLICATE KEY UPDATE state = %d, gettime = %u",
        nRoleID, nMedalID, eMedalState_Activated, nNow, eMedalState_Activated, nNow);

    return ExecSQL() ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadHandbookPcInfo(unsigned int nRoleID, int nType, std::map<int, std::string > &PersonalInfo)
{
    int nRes = Query_Res_Fail;

    PersonalInfo.clear();
    Formatter(sql, sizeof(sql)) << "select id, info " \
        "from handbook_personal_data where roleid = " << nRoleID \
        << " and handbooktype = " << nType;
    nRes = ExecSQL();
    if (nRes)
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            int nDBKey = atoi(row[nField++]);

//             std::list<CHandbookPersonalInfo *> ListPcInfo;
//             HandbookDecode(ListPcInfo, row[nField++]);
            PersonalInfo.insert(std::make_pair(nDBKey, row[nField++]));
        }
    }

    return nRes;
}

int CDBInterface::LoadHandbookGlobalInfo(int nType, std::map<int, CHandbookUnit> &GlobalInfo)
{
    int nRes = Query_Res_Fail;

    Formatter(sql, sizeof(sql)) << "select id, praised_count, " \
        "envyed_count, collected_count from handbook_global_data "\
        "where handbooktype = " << nType;
    nRes = ExecSQL();
    if (nRes)
    {
        while ((row = FetchRow()) != NULL)
        {
            int nField = 0;
            int nDBKey = atoi(row[nField++]);
            int nPraisedCount = atoi(row[nField++]);
            int nEnvyedCount = atoi(row[nField++]);
            int nCollectedCount = atoi(row[nField++]);
            CHandbookUnit Unit(nDBKey, nPraisedCount, nEnvyedCount, nCollectedCount);
            GlobalInfo.insert(make_pair(nDBKey, Unit));
        }
    }

    return nRes;
}

int CDBInterface::UpdateHandbookGlobalInfo(int nType, std::map<int, CHandbookUnit > &unit)
{
    int bRes = Query_Res_Fail;

    for (std::map<int, CHandbookUnit>::iterator it = unit.begin();
        it != unit.end(); ++it)
    {
        Formatter::Format(sql, sizeof(sql), "call update_handbook_global_data( %d, %d , %u , %u , %u )",
            it->second.GetID(), nType, it->second.GetPraisedCount(), it->second.GetEnvyCount(), it->second.GetCollectCount());

        bRes = ExecSQL();
        SafeFreeResult();
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::UpdateHandbookPcInfo(unsigned roleid, const std::map<int, std::string> &PersonalInfo)
{
    int bRes = Query_Res_Fail;

    for (std::map<int, std::string>::const_iterator mapit = PersonalInfo.begin();
        mapit != PersonalInfo.end(); ++mapit)
    {
//         std::string strEncodeResult;
//         HandbookEncode(mapit->second, strEncodeResult);

        Formatter::Format(sql, sizeof(sql), "update handbook_personal_data "
            "set info = '%s' where id = %d and roleid = %d",
            mapit->second.c_str(), mapit->first, roleid); //strEncodeResult不包含中文

        bRes = ExecSQL();
        SafeFreeResult();
    }

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::InsertHandbookPcInfo(unsigned roleid, int nHandbookType, const std::map<int, std::string> &PersonalInfo)
{
    int bRes = Query_Res_Fail;

    for (std::map<int, std::string>::const_iterator mapit = PersonalInfo.begin();
        mapit != PersonalInfo.end(); ++mapit)
    {
//         std::string strEncodeResult;
//         HandbookEncode(mapit->second, strEncodeResult);

        Formatter::Format(sql, sizeof(sql), "INSERT INTO handbook_personal_data(roleid, handbooktype, info) "
            "VALUE(%u, %u, '%s') ", roleid, nHandbookType, mapit->second.c_str());   //strEncodeResult不包含中文

        bRes = ExecSQL();
        SafeFreeResult();
    }
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::LoadDesiredItem(LPROLEINFO_DB pRole)
{
    int nRes = Query_Res_Fail;
    if (pRole != NULL)
    {
        pRole->m_listDesiredItem.clear();
        sprintf(sql, "SELECT itemtype FROM desire WHERE roleid = %u", pRole->m_nRoleID);

        if (ExecSQL())
        {
            while ((row = FetchRow()) != NULL)
            {
                int nField = 0;
                itemtype_t nItemType = (itemtype_t)atoi(row[nField++]);;
                pRole->m_listDesiredItem.push_back(nItemType);
            }
            nRes = Query_Res_OK;
        }
        SafeFreeResult();
    }

    return nRes;
}

int CDBInterface::InsertDesiredItem(unsigned int nRoleID, itemtype_t nItemType)
{
    char* end = sql;
    strFmt = "INSERT INTO desire set roleid = %u, itemtype = %u";
    sprintf(end, strFmt.c_str(), nRoleID, nItemType);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();

    return bRes ? Query_Res_OK : Query_Res_Fail;
}

int CDBInterface::DeleteDesiredItem(unsigned int nRoleID, itemtype_t nItemType)
{
    char* end = sql;
    strFmt = "delete from desire WHERE roleid = %u and itemtype = %u";
    sprintf(end, strFmt.c_str(), nRoleID, nItemType);
    end = sql + strlen(sql);
    bool bRes = ExecSQL(sql, (unsigned int)(end - sql));
    SafeFreeResult();
    return bRes ? Query_Res_OK : Query_Res_Fail;
}

