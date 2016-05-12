#ifndef __LOGIN_CIRCULATOR_H__
#define __LOGIN_CIRCULATOR_H__

#include "../libServerFrame/Circulator.h"
#include "../share/ServerDataStruct.h"
#include "../share/LinkingInfos.h"
#include "../../datastructure/Macro_Define.h"
#include "PlayerCacheManager.h"
#include <list>
#include <map>


class CAutoAccount;
class AccountInfo;
class ChongZhiInfo;
struct RechargeRewardInfo;


// Acccount Server开放两个端口,7752供Game Server连接,7753供Gate Server连接。
// 账号服务器无法直接transport(gate server没有该消息的定义、解包等函数)消息到客户端
class CLoginCirculator : public CCirculator
{
public:
    CLoginCirculator();
    ~CLoginCirculator();

    void InitPlayerCacheManager( unsigned int nMaxCount, unsigned int nValidTime );

protected:
    bool Initialize();
    void Release();
    void PumpProc();
    void ProcessQueryFinished(QueryBase* pQuery);
    void ProcessHeartBeat(int& timeElapse);

    void RegisterMessage();
    void RegisterMessageProcessor();

    void SendMsgToPlayerLine(LPGameMsg_Base pMsg, unsigned int nAccount);
    bool SendMsgToAnyLine(LPGameMsg_Base pMsg);

    void SendMessageToGate(GameMsg_Base *pMsg, unsigned short nSlot);
    void SendMessageToPlayer(GameMsg_Base *pMsg, const AccountInfo *pAccountInfo);
    void BroadcastMsg2GameServer(GameMsg_Base *pMsg, int nExcludedLine = -1);

protected:
    void OnClientConnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnClientDisconnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnClientInvalidPacket(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnServerState(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);

    void OnAccountLogin(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnPlayerCharge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnPlayerChargeByPUID(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnServerAccountLogin(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnServerAccountLogout(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnCreateAccount(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnQuickCreateAccount(GameMsg_Base &pMsg, CSlotPeer & SlotPeer);
    void OnCheckActivateCode(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnPlayerSwitchLine(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnLinkCount(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnGameServerLinkInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetServersInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnCmdAddMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMAccountLock(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMAddMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMAddPrivMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMGetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnGMSetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnClientDeviceInfo(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);

    void OnSaveChargeAction(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);

    void OnPlayerCacheTransferStart( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void OnPlayerCacheTransfer( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void OnPlayerCacheTransferEnd( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void OnRequestPlayerCache( GameMsg_Base &msg, CSlotPeer &slotPeer );

    void OnVerifyAccount( GameMsg_Base &msg, CSlotPeer &slotPeer );

    void OnTransformPlatform(GameMsg_Base& pMsg, CSlotPeer& SlotPeer);
    void OnTransformPlatform_DBCheckActcode(QueryBase& pQuery);
    void OnTransformPlatform_DBUpdateAccount(QueryBase& pQuery);

    void OnDBGetGetRegistedRoleCount(QueryBase& pQuery);
    void OnDBCreateAccount(QueryBase & pQuery);
    void OnDBAccountLogin(QueryBase & pQuery);
    void OnDBQuickCreateAccount(QueryBase & pQuery);
    void OnDBCreateNewAutoAccount(QueryBase &query);
    void OnChargeCallBack(QueryBase & query);
    void OnDBActivateCode(QueryBase & query);
    void OnDBLockAccountCallBack(QueryBase & query);
    void OnGetFirstPaymentRewardByAccountIdCallBack(QueryBase & refQuery);
    void OnGMPrivRechargeCallBack(QueryBase& query);
    void OnSendBlackDeviceCallBack(QueryBase& query);
    void OnDeleteBlackDeviceCallBack(QueryBase& query);

protected:
    bool _ProcessLogin(AccountInfo *pAccountInfo, int nLine = -1);
    void _OnChargeOK(ChongZhiInfo * pInfo);

    void KickPlayOff(AccountInfo *pAccountInfo, unsigned short reason = DropPlayerCause_ReLogin);

    void _OnServerLink(LPGameMsg_Base pGameMsg, unsigned short &nSlot);
    const GameServerLinkInfo* GetGameServerLinkInfo(unsigned short nSlot) const;

    __int64 GenerateSession();
    __int64 GetSesssionID();

    void GetRecTag(char *szRecTag);

    bool IsSpecialUser(const char* szUsername);

    void FillFixedChargeData(RechargeRewardInfo* pRechargeRewardInfo, unsigned amount);

    bool IsInLoginProgress(AccountInfo *pAccount);
    AccountInfo *GetLoginAccount(int nSlot, __int64 nSessionID);
    bool AddLoginAccount(__int64 SessionID, AccountInfo *pAccount);
    bool RemoveLoginAccount(int nSlot, __int64 nSessionID); //在调用这个方法之前，需要先自己释放 second数据指针的内存

    AccountInfo *GetOnlineAccount(unsigned int nAccount);

    int GetOnlineAccountCount();

    virtual void ReportStat();

private:
    typedef std::map<int, std::map<__int64, AccountInfo *> >    LoginSessionMap;    //登录玩家列表: key: nSlot, value: session id -> account info
    typedef std::map<unsigned int, AccountInfo *>               OnlineAccountMap;   //在线玩家列表: key: accountId, value: account info

    LoginSessionMap                                             m_mapLoginSession;  //登陆中和登陆成功的账号
    OnlineAccountMap                                            m_mapAccountOnline; //在线的账号
    __int64                                                     m_nAccountSessionIdCounter; //Account Server Session ID 计数器

    CSocketMgr                                                  *m_pGameSrvSocketMgr;   //管理GameServer的socket
    CSlotPeer                                                   *m_pLinkerSlotPeer;
    LinkingInfos                                                m_linkingInfos;         //连接信息

    int                                                         m_nSoftLimitClientCount;//允许连接游戏服务器的客户端数量
    bool                                                        m_bEnableRecharge;      //充值和充值奖励开关
    bool                                                        m_bEnableFirstRechargeReward;

    std::list<CAutoAccount *>               m_listAutoAccount;
    std::map<unsigned int, unsigned char>   m_mapActivateKey;       // 激活码: key: code, value: status(0-used, 1-unused)
    std::list<__int64>                      m_listActivateQueue;    // 激活队列

    static PlayerCacheManager               m_playerCacheManager;

    int                                     m_nServerUpTime;        // 服务器启动时长
};

extern CLoginCirculator *g_pCirculator;

#endif //__LOGIN_CIRCULATOR_H__

