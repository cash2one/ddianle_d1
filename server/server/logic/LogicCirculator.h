#ifndef __LOGIC_CIRCULATOR_H__
#define __LOGIC_CIRCULATOR_H__

#include "../libServerFrame/Circulator.h"
#include "../../socket/ClientSocketMgr.h"
#include "../share/ServerDataStruct.h"
#include "../../datastructure/DataStruct_DB.h"
#include "PlayerLoginManager.h"

class MainManager;
class CIDCreater;
class CommandProcessor;
class CLog;
class GameMsg_S2G_GameServerLinkInfo;
class CRoleEntity;

//GameServer主动连接GroupServer、AccountServer，GateServer主动连接到GameServer
class CLogicCirculator : public CCirculator
{
public:
    CLogicCirculator();
    ~CLogicCirculator();
protected:
    bool Initialize();
    void Release();
    //循环体
    void PumpProc();
    //心跳，响应时间流逝
    void ProcessHeartBeat(int& timeElapse);

    void ProcessLoginSvrMsg(LPGameMsg_Base pGameMsg);
    //处理来自WorldHost的消息
    void ProcessHostMsg(LPGameMsg_Base pHostMsg);
    void OnGetRoleInfoQueryFinish(QueryBase& refQuery);
    void OnDBCreateRole(QueryBase & refQuery);
    void OnDBCheckRoleName(QueryBase & refQuery);
    void OnDBUpdateRole(QueryBase & refQuery);
    void OnDBGetRcvGMSysMailRoleInfo(QueryBase & refQuery);
    void OnDBQueryGiftByTimeRange(QueryBase & refQuery);
    void OnDBQueryMultiGift(QueryBase & refQuery);
    void onDBQueryDelVersionMedal(QueryBase& refQuery);
    void OnDBQueryAddMedalByGm(QueryBase& refQuery);

    void RegisterMessage();
    void RegisterMessageProcessor();

    virtual void EnterExitProcess();
public:
    void OnClientConnect(GameMsg_Base & Msg, CSlotPeer & SlotPeer);
    void OnClientDisconnect(GameMsg_Base & Msg, CSlotPeer & SlotPeer);
    void OnClientInvalidPacket(GameMsg_Base & Msg, CSlotPeer & SlotPeer);
    void OnClientLogin(GameMsg_Base & Msg, CSlotPeer & SlotPeer);
    void OnPlayerLogout(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnReportServerInfoRespond(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnCreateRole(GameMsg_Base &Msg, CSlotPeer & SlotPeer);
    void OnCheckRoleName(GameMsg_Base &Msg, CSlotPeer &Slotpeer);
    void OnDropClient(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnPlayerSwitchLine(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnSendMailFromGMServer(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnQueryMultiGift(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnGetGiftCodesState(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnChargeNotify(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnSyncChargeSetting(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnAnnounce(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnLockOrUnLockAccountByGm(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnBlessingNotify(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMNewCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSyncCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnNewCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnBugReport(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnGetAllBuff(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnUnApplyBuff(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnAddMCoinByGm(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnSendMailFromAccount(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnAgentAnnounce(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnAgentShutdownInTime(GameMsg_Base & refMsg, CSlotPeer & SlotPeer);
    void OnAgentClose(GameMsg_Base & refMsg, CSlotPeer & SlotPeer);
    void OnHostAnnounce(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);

    void OnUpdateLBSPositon(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnGetNearbyPlayers(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnSyncNearbyPlayers(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);

    void OnChargeReward(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnFixedChargeReward(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);

    void OnGetRoomListByGM(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnSendPrivMCoinByGM(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);

    void OnAccountGMSendPrivMCoinRet(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnAccountSendOnlineMCoin(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnGMGetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnAccountSendServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnGMSetServerPlayerLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void SendChargeNoticeMail(unsigned int nRoleID, int nPoints, unsigned int nChargeTime);

    void OnGmDeleteVersionMedal(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGmAddMedal(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnRequestPlayerCacheRes( GameMsg_Base& msg, CSlotPeer& slotPeer );
    void OnPlayerCacheTransferStart( GameMsg_Base& msg, CSlotPeer& slotPeer );
    void OnPlayerCacheTransfer( GameMsg_Base& msg, CSlotPeer& slotPeer );
    void OnPlayerCacheTransferEnd( GameMsg_Base& msg, CSlotPeer& slotPeer );

    void OnVerifyAccountRes( GameMsg_Base &msg, CSlotPeer &slotPeer );

    void OnTouristSignInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);

    CommandProcessor* GetCommandProcessor(){
        return m_pCommandProcessor;
    };
private:
    void _OnClientLogin(__int64 nSessionID, unsigned int nAccount, unsigned short nSlot, unsigned char nInstallerId, int nVersionType, bool bSwitchLine = false);
    //公告
    void OnWorldAnnounce(unsigned short nPID, const char * pContent);
    void OnAdminAnnouce(unsigned short nPID, std::string & strContent);

    void OnConnectedWithGroup();

    void DropPlayer( unsigned int nAccountID, bool bIsNoticeAccount, bool bNoticeGateServer, bool bIsSwitchServer, int nCause );

    void ReportPlayerCount() const;
public:
    void BroadCastMsg2AllClient(GameMsg_Base * pChatMsg2Client);
private:
    CClientSocketMgr* m_pSockHost;//到WorldHost的连接
    CSlotPeer *m_pHostSlotPeer;
    CClientSocketMgr* m_pSockLogin;
    CSlotPeer *m_pLogirSlotPeer;

    MainManager * m_pMainManager;

    unsigned int m_nFeedVersionPtReturn;//付费客户端金卷返还比例
private:
    CIDCreater* m_pIDCreater;
    int m_nReconnectLoginSvrInterval;
    int m_nReconnectGroupSvrInterval;
    std::string m_ServerName;
    unsigned char m_nInstallerID;		//是否是新手线
    bool m_bEnableAudition;				//是否是海选线
public:
    void SetServerNameInfo(const char * pstrName);
    std::string& GetServerName();
    void SetServerInstallerID(unsigned char nInstallerID);
    unsigned char GetServerInstallerID();
    void SetEnableAudition(bool bEnable);
    bool IsEnableAudition() const;

public:
    CommandProcessor* m_pCommandProcessor;

    bool m_bEnableClientCommand;//允许客户端控制命令
    bool m_bRecordLoad;//记录网络负载

    FLOWMETER flux;//网络流量记录
protected:
    virtual void ReportStat();
public:
    void BuildLinkInfo(GameMsg_S2G_GameServerLinkInfo &infoMsg);
    //完成充值奖励，pRole为空表示不在线的情况
    void FinishChargeReward(int nChargeRewardType, CRoleEntity *pRole, unsigned int nRoleID, const string& strRoleName,
        Version_Type nVersionType, MailInfo& mailInfo, int nBindMcoinReward, int nMoneyReward);

private:
    bool m_bConnectedGroupServer;   // 是否与GroupServer连接
    bool m_bConnectedAccountServer; // 是否与AccountServer连接

    PlayerLoginManager  m_loginManager;
};

extern CSocketMgr*		g_pSocketMgr;
extern CClientSocketMgr*g_pSockHost;
extern CQueryManager*	g_pQueryMgr;
extern CIDCreater* g_pIDCreater;

//发送消息给指定的客户端
int SendGameMsg(LPGameMsg_Base pMsg, const PLAYERTAG* pTag); // return 0 if success
//广播消息给所有的客户端
void BroadcastMsg(LPGameMsg_Base pMsg);
void BroadcastPlatformMsg(unsigned short nPID, LPGameMsg_Base pMsg);

//发送消息给GroupServer
void SendMsg2GroupServer(LPGameMsg_Base pMsg);
void SendMsg2AccountServer(LPGameMsg_Base pMsg);
void SendMsg2GateServer(LPGameMsg_Base pMsg, int nSlot);

#endif//__LOGIC_CIRCULATOR_H__

