#ifndef __GMDBINTERFACE_H__
#define __GMDBINTERFACE_H__

#include <list>
#include "../../socket/DBIMysql.h"
#include "GMDataStruct.h"
#include "../../datastructure/DataStruct_Base.h"

using namespace std;

const int Query_Res_OK = 0;//查询成功
const int Query_Res_Fail = -1;//失败

class GMDBInterface : public CDBIMysql
{
public:
    GMDBInterface();
    ~GMDBInterface();
public:
    int GetGMAccInfo(GMAccountInfo &accountInfo);
    int GetGMRoleInfo(CGMRoleInfoDB & roleInfo);

    int InsertGrantedCmd(const char *pAccName, list<string> & listCmd, int & nErrno);
    int RemoveGrantedCmd(const char *pAccName, list<string> & listCmd, int & nErrno);
    int LoadGrantedCmd(unsigned int nAccount, list<string> & listCmd);

    int InsertRolePriv(unsigned int nAcccount, list< unsigned int> & listPriv);
    int LoadRolePriv(unsigned int nAccount, list<unsigned int> & listPriv);

    int InsertRoleDisabledCmds(unsigned int nAccount, list<string> & listCmd);
    int LoadRoleDisableCmds(unsigned int nAccount, list<string> & listCmd);

    int InsertSysMailLog(CGMSysMailLog & sysMailLog);

    int AddAnnouce(CAnnounceInfo & record);
    int UpdateAnnouce(CAnnounceInfo & record);
    int RemoveAnnounce(CAnnounceInfo & record);
    int LoadAnnounce(std::map< unsigned int, CAnnounceInfo*> & mapAnnounce);

    int AddUser(const char* szName, const char* szPassword);
    int LoadUsers(std::vector<CGMRoleInfoDB>* pRoleList);
    int SetUserPriv(CGMRoleInfoDB* pRole);
    int DelUser(CGMRoleInfoDB* pRole);

    // 玩家封号记录
    int WriteUserLockRecord(PlayerLockRequestDb &request);
    int LoadUserLockRecord(PlayerLockQueryDb &request);

    // IOS多账号禁止充值
    int LoadRechargeRecord(IosForbidRechargeDB & request);

    // 骚扰信息查询：
    int LoadHarassInfo(HarassReqDb &harassDb, RoleHarassInfoAMap &harassMap, std::set<unsigned int> &setRoleID, unsigned int roleIdMin, unsigned int roleIdMax);
    int LoadHarassRoleInfo(HarassReqDb &harassDb, RoleHarassInfoAMap &harassMap, std::set<unsigned int> &setRoleID);
    // 加载某人某时间段的非法聊天记录
    int LoadHarassChatInfo(HarassChatReqDb &harassChatReqDb);
    int LoadPlayerInfoForHarassChatInfo(HarassChatReqDb &harassChatReqDb);

    int WriteGMLog(unsigned int nAccount, int nAction, const char* param);
    int LoadGMLog(QueryLogDb *pQueryDb);
    int LoadGMLogItem(QueryLogItemDb *pQueryDb);
    int ChangePassword(unsigned int nAccount, const char* szNewPwd);
};

#endif

