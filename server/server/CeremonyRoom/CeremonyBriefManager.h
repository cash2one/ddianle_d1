/** Counter.h
* Description: manage brief info for ceremony room, this is used in group manager.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao
* Create time: 2015-08-11 11:48:00
*/


#ifndef __CEREMONYBRIEFMANAGER_H__
#define __CEREMONYBRIEFMANAGER_H__

#include <map>
#include <set>
#include <vector>
#include <string>

#include "../../socket/GameMsg_Base.h"
#include "../../socket/GameMsg_Processor.h"
#include "ceremonyRoomHead.h"
#include "GameMsg_CeremonyRoom.h"

class CGroupRoleEntity;

// 入团仪式房间简要信息
class CeremonyBrief
{
public:
	CeremonyBrief();
	~CeremonyBrief();

public:
	unsigned int m_nDanceGroupID;
	unsigned char m_state; //
	unsigned int m_line;				// 所在线的编号
	unsigned int m_createTime;			// 仪式举办时间
    std::string m_strLineName;          
};

// 入团仪式申请锁定信息（同一时刻只能有一个人在申请）
class CeremonyApplyInfo
{
public:
	CeremonyApplyInfo();
	~CeremonyApplyInfo();

	unsigned int m_nDanceGroupID;
	unsigned int m_nCreaterID;
	unsigned int m_applyTime; // 需要做倒计时
};

typedef std::vector<CeremonyApplyInfo> CeremonyApplyInfoVec;

typedef std::map<unsigned int, CeremonyBrief> CeremonyBriefMap;

class BufferCeremonyLog
{
public:
    int m_nLine;
    unsigned int m_nLogSeq;

    unsigned int m_nGroupID;
    DanceGroupCeremonyLog m_log;
};


class CeremonyBriefManager : public GameMsg_Processor
{
protected:
	CeremonyBriefManager();
	~CeremonyBriefManager();

public:
	static CeremonyBriefManager& Instance();

    void Initialize(); // 初始化
    void InitializeAfterLoadDanceGroup(); // 初始化

    void OnPlayerLogin(CGroupRoleEntity & role);
    void OnPlayerEnterDanceGroup(CGroupRoleEntity & role);
	// 更新
	void OnUpdate(time_t tNow);
private:
	// 注册消息
    void RegisterMessageBeforeLoad();
	void RegisterMessageAfterLoad();
    void LoadDbInfo();

    // 房间信息同步
    void OnCeremonySyncRoomInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// 申请创建
	void OnCeremonyApplyCreate(GameMsg_Base & msg, CSlotPeer & slotPeer); // 舞团创建申请
	// 创建通知
	void OnCeremonyCreateRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer); // 舞团创建申请
	// 取消创建申请（创建失败）
	void OnCeremonyApplyCreateCancel(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// 房间关闭通知
	void OnCeremonyCloseRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// 房间状态变更通知
	void OnCeremonyRoomStateChange(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomNewMember(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// 插入日志
    void OnCeremonyRoomLogsStart(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomLogsItem(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomLogsEnd(GameMsg_Base & msg, CSlotPeer & slotPeer);
    // 客户端请求日志信息
    void OnRequestCeremonyRecord(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnRequestCeremonyPraise(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomStatics(GameMsg_Base & msg, CSlotPeer & slotPeer);

private:
    void OnCeremonyWriteLog(QueryBase &rQuery);
    void OnCeremonyLoadLog(QueryBase &rQuery);

private:
	// 检测是否存在某个舞团的仪式申请
	bool HasGroupCeremonyApply(unsigned int danceGroupID);
	// 检测是否存在某个舞团的仪式正在举行
	bool HasGroupCeremony(unsigned int danceGroupID);
	// 
	bool RemoveFromApply(unsigned int danceGroupID);

	// 广播舞团状态改变
    void BroadcastCeremonyRoomStateChange(GameMsg_Base &bmsg);
    void BroadcastSyncCeremonyState(unsigned int groupID, unsigned char newState, int line, std::string strLineName);

    BufferCeremonyLog* GetBuffCeremonyLog(int nLine, unsigned int logSeq);
    void InsertBuffCeremonyLog(BufferCeremonyLog &log);
    void RemoveBuffCeremonyLog(int nLine, unsigned int logSeq);

    // 服务器存储的日志结构转换成客户端的日志结构
    void TransSrvLog2ClientLog(DanceGroupCeremonyLog &srvLog, CeremonyRecordInfo &clientLog);
    void TransSrvVoteLog2ClientLog(CeremonyPraiseLogInfo &srvLog, unsigned int startIndex, unsigned int endIndex, std::vector<CeremonyPraiseSendInfo> &vecClientLog);
    void SendCeremonyState(CGroupRoleEntity & role);

protected:
	CeremonyBriefMap m_ceremonyBriefMap;
	CeremonyApplyInfoVec m_ceremonyApplyInfo;

    DanceGroupCeremonyLogMap m_logMap;
    bool m_bLoadLogComplete; // 

    // 缓冲的日志log,写入的时候，需要写完所有，然后才进入数据库
    typedef std::vector<BufferCeremonyLog> BufferCeremonyLogVec;
    BufferCeremonyLogVec m_bufferLogVec; 
};



#endif // __CEREMONYBRIEFMANAGER_H__
