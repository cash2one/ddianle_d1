#ifndef		__GMCIRCULAOTOR_H__
#define		__GMCIRCULAOTOR_H__

#include "../libServerFrame/Circulator.h"
#include "GMDefine.h"

class GMEntity;
class CGMQueryMgr;

using namespace std;

class CGMCirculator : public CCirculator
{
public:
    CGMCirculator();
    virtual ~CGMCirculator();
public:
    void SendMsgToGroup(GameMsg_Base *pMsg);
    void SendMsgToGM(GameMsg_Base *pMsg, unsigned short nSlot);
    static void LogGM(unsigned int account, EGMActionType action, std::string& param);
protected:
    bool Initialize();
    void Release();

    void PumpProc();
    void ProcessQueryFinished(QueryBase* pQuery);
    void ProcessHeartBeat(int& timeElapse);
    //网络消息和数据库处理
    void RegisterMessage();
    void RegisterMessageProcessor();
private:
    void OnClientConnect(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnClientDisconnect(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMServerReportWorldInfoRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMSendMail(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMSendMailRes(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMLogin(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMLogout(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMQueryGiftByTimeRange(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMQueryGiftByTimeRangeRes(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMQueryMultiGift(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGMQueryMultiGiftRes(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void OnGrantGMCmd(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnRemoveGrantedCmd(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnLockAccount(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnAddMCoin(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnSendCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnUpdateCompensationRes(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetUsers(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSetPriv(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnAddUser(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnDelUser(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnGetServerLine(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetServerRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGroupGetServerLineRes(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGroupGetRoomListRes(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnSetMallUpdateTime(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGroupBuyControl(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnChangeSelfPassword(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnChangePassword(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnPrivAddMCoin(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnPrivAddMCoinRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetServerPlayerCountInfoRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetServerPlayerLimitRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnDeletVersionMedal(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnAddMedal(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnDeletVersionMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnAddMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnRequestLockList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryPlayerLockCB(QueryBase& query);
    void OnRequestHarassList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryHarassListCB(QueryBase& query);
    void OnRequestRoleHarassChatInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryRoleHarassChatInfoCB(QueryBase& query);
    void OnRequestRechargeRecordList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryRechargeRecordList(QueryBase& query); 

    void OnRequestQueryLog(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryLogLoad(QueryBase& query);
    
    void OnRequestQueryLogItem(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnQueryLogItemLoad(QueryBase& query);
private:
    void GetActivateGiftNum(GameMsg_Base &msg, GMEntity &entity);
    void GetGiftCodesState(GameMsg_Base &msg, GMEntity &entity);
    bool HasPriv(int nEvent, int nPriv);
    void KickGM(GMEntity *pEntity);

private:
    void ProcessWorldMessage(GameMsg_Base *pGameMsg);

private:
    void CreateGMAccountDBCB(QueryBase & query);
    void GMLoginDBCB(QueryBase & query);
    void GMGetRoleInfoDBCB(QueryBase & query);
    void GrantGMCmdDBCB(QueryBase & query);
    void RemoveGMCmdDBCB(QueryBase & query);
    void InsertSysMailLogDBCB(QueryBase & query);
    void AddUserDBCB(QueryBase& query);
    void SetPrivDBCB(QueryBase& query);
    void DelUserDBCB(QueryBase& query);
    void ChangePasswdDBCB(QueryBase& query);

private:
    void AddLoginingMap(unsigned short nSlot, GMEntity *pEntity);
    GMEntity *RemoveLoginingMap(unsigned short nSlot);
    GMEntity *GetLoginingPlayer(unsigned short nSlot);

private:
    void InitManager(void);

private:
    bool m_bGMConnectedToWorld;
    int m_nGMRecGroupSvrInterval;
    map<unsigned short, GMEntity *> m_mapLoginingPlayer;

    CGMQueryMgr *m_pGMQueryMgr;
    CClientSocketMgr *m_pSockWorldMgr;	// connect to Group Server;
    CSlotPeer *m_pWorldSlotPeer;
    CSocketMgr* m_pSockClientMgr;		//管理client的socket管理器
};

extern CGMCirculator* g_pGMCirculator;

#endif

