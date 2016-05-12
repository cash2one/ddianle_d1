#ifndef __GROUPSERVER_CIRCULATOR_H__
#define __GROUPSERVER_CIRCULATOR_H__

#include <list>
using namespace std;

#include "../libServerFrame/Circulator.h"
#include "../share/ServerDataStruct.h"
#include "../share/LinkingInfos.h"
#include "../../datastructure/DataStruct_DB.h"
#include "WorldHost.h"

class CIDCreater;

class CGroupServerCirculator : public CCirculator
{
public:
    CGroupServerCirculator();
    ~CGroupServerCirculator();
protected:
    bool Initialize();
    void OnLoadCommands(QueryBase &pQuery);
    void ProcessHeartBeat(int& timeElapse);

    virtual void ReportStat();
    void RegisterMessage();
    void RegisterMessageProcessor();

    void ParseCommand(const CMDINFO& info);
    list< CMDINFO >m_listCmd;

public:
    unsigned short GetSlotByLine(int nLine) const;
    void Forward(GameMsg_Base &msg, unsigned int nRoleID);
    void Forward(GameMsg_Base &msg, const std::list<unsigned int> &roleIDList);

private:
    CIDCreater* m_pIDCreater;
public:
    int m_nGMSlot;
    CWorldHost* m_pWorldHost;
private:
    unsigned long m_nUpdateDelay;			//检查是否重新计算每日任务的心跳间隔
    int m_nLastCheckHour;
    unsigned int    m_uMallReloadInterval;      // 商城热更新检查间隔
    unsigned int    m_uMallReloadTime;          // 商城热更新时间
    bool            m_bMallNeedReload;          // 是否热更新
    LinkingInfos    m_linkingInfos;             // 连接信息
private:
    void OnClientConnect(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnClientDisconnect(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGameServerLinkInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnLinkCount(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void OnPlayerLogin(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnPlayerLogout(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnMallAskFor(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnMallDesire(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnWishComeTrue(GameMsg_Base & msg, CSlotPeer & slotPeer);

    void OnGMServerReportInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMSendMailRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMGetActivatedGiftNum(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMGetActivatedGiftNumRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMGetGiftCodesState(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMGetGiftCodesStateRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMSendAnnounce(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnBlessFriend(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnUpdateVIPInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMLockOrUnLockAccount(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMAddMCoin(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnDirectAllClient(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMUpdateCompensation(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnUpdateLBSPositon(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGetNearbyPlayers(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCloseLBS(GameMsg_Base & msg, CSlotPeer & slotPeer);

    void OnGMGetServerLine(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMGetRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnServerGetRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMSetMallUpdateTime(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMSendPrivMCoin(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMSendPrivMCoinRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMGetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGetServerPlayerLimitRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMSetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void OnForward(GameMsg_Base & msg, CSlotPeer & slotPeer);

    void OnGameServerClosed(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnGMDeleteVersionMedal(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMDeleteVersionMedalRet(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMAddMedal(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGMAddMedalRet(GameMsg_Base &msg, CSlotPeer &slotPeer);
private:
    //DB返回外部系统发送邮件请求
    void OnDBExternalSendMail(QueryBase &pQuery);
    //DB返回外部系统发送Gift邮件请求
    void OnDBExternalSendMailGift(QueryBase &pQuery);
    //外部系统请求发送邮件
    void OnExternalSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer);
    //外部系统请求发送Gift邮件
    void OnExternalSendMailGift(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnInsertCompensation(QueryBase& query);

    int GetAvailableGameServerLine();

    void SendMallReloadMsg();
};

extern CIDCreater* g_pIDCreater;
extern CGroupServerCirculator* g_pCirculator;

//给游戏服务器发消息,指定第几线(0-9)
void SendMsg2GameServer(LPGameMsg_Base pMsg, int nLine);
//给游戏服务器发消息,指定nSlot
void SendMsg2GameServerBySlot(LPGameMsg_Base pMsg, unsigned short nSlot);
//向游戏服务器广播消息
void BroadcastMsg2GameServer(LPGameMsg_Base pMsg);
//向游戏服务器广播消息，某一线除外（默认值LINES_IN_GROUP表示没有例外）
void BroadcastMsg2GameServer(LPGameMsg_Base pMsg, int nLineExcluded);
//向GM服务器发消息
void SendMsgToGMServer(LPGameMsg_Base pMsg);
void SendMsgToGMServer(LPGameMsg_Base pMsg, unsigned short nSolt);

void Forward(GameMsg_Base &msg, unsigned int nRoleID);
void Forward(GameMsg_Base &msg, const std::list<unsigned int> &roleIDList);

#endif//__GROUPSERVER_CIRCULATOR_H__
