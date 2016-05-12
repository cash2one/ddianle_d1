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

// ������ʽ�����Ҫ��Ϣ
class CeremonyBrief
{
public:
	CeremonyBrief();
	~CeremonyBrief();

public:
	unsigned int m_nDanceGroupID;
	unsigned char m_state; //
	unsigned int m_line;				// �����ߵı��
	unsigned int m_createTime;			// ��ʽ�ٰ�ʱ��
    std::string m_strLineName;          
};

// ������ʽ����������Ϣ��ͬһʱ��ֻ����һ���������룩
class CeremonyApplyInfo
{
public:
	CeremonyApplyInfo();
	~CeremonyApplyInfo();

	unsigned int m_nDanceGroupID;
	unsigned int m_nCreaterID;
	unsigned int m_applyTime; // ��Ҫ������ʱ
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

    void Initialize(); // ��ʼ��
    void InitializeAfterLoadDanceGroup(); // ��ʼ��

    void OnPlayerLogin(CGroupRoleEntity & role);
    void OnPlayerEnterDanceGroup(CGroupRoleEntity & role);
	// ����
	void OnUpdate(time_t tNow);
private:
	// ע����Ϣ
    void RegisterMessageBeforeLoad();
	void RegisterMessageAfterLoad();
    void LoadDbInfo();

    // ������Ϣͬ��
    void OnCeremonySyncRoomInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// ���봴��
	void OnCeremonyApplyCreate(GameMsg_Base & msg, CSlotPeer & slotPeer); // ���Ŵ�������
	// ����֪ͨ
	void OnCeremonyCreateRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer); // ���Ŵ�������
	// ȡ���������루����ʧ�ܣ�
	void OnCeremonyApplyCreateCancel(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// ����ر�֪ͨ
	void OnCeremonyCloseRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// ����״̬���֪ͨ
	void OnCeremonyRoomStateChange(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomNewMember(GameMsg_Base & msg, CSlotPeer & slotPeer);
	// ������־
    void OnCeremonyRoomLogsStart(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomLogsItem(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomLogsEnd(GameMsg_Base & msg, CSlotPeer & slotPeer);
    // �ͻ���������־��Ϣ
    void OnRequestCeremonyRecord(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnRequestCeremonyPraise(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnCeremonyRoomStatics(GameMsg_Base & msg, CSlotPeer & slotPeer);

private:
    void OnCeremonyWriteLog(QueryBase &rQuery);
    void OnCeremonyLoadLog(QueryBase &rQuery);

private:
	// ����Ƿ����ĳ�����ŵ���ʽ����
	bool HasGroupCeremonyApply(unsigned int danceGroupID);
	// ����Ƿ����ĳ�����ŵ���ʽ���ھ���
	bool HasGroupCeremony(unsigned int danceGroupID);
	// 
	bool RemoveFromApply(unsigned int danceGroupID);

	// �㲥����״̬�ı�
    void BroadcastCeremonyRoomStateChange(GameMsg_Base &bmsg);
    void BroadcastSyncCeremonyState(unsigned int groupID, unsigned char newState, int line, std::string strLineName);

    BufferCeremonyLog* GetBuffCeremonyLog(int nLine, unsigned int logSeq);
    void InsertBuffCeremonyLog(BufferCeremonyLog &log);
    void RemoveBuffCeremonyLog(int nLine, unsigned int logSeq);

    // �������洢����־�ṹת���ɿͻ��˵���־�ṹ
    void TransSrvLog2ClientLog(DanceGroupCeremonyLog &srvLog, CeremonyRecordInfo &clientLog);
    void TransSrvVoteLog2ClientLog(CeremonyPraiseLogInfo &srvLog, unsigned int startIndex, unsigned int endIndex, std::vector<CeremonyPraiseSendInfo> &vecClientLog);
    void SendCeremonyState(CGroupRoleEntity & role);

protected:
	CeremonyBriefMap m_ceremonyBriefMap;
	CeremonyApplyInfoVec m_ceremonyApplyInfo;

    DanceGroupCeremonyLogMap m_logMap;
    bool m_bLoadLogComplete; // 

    // �������־log,д���ʱ����Ҫд�����У�Ȼ��Ž������ݿ�
    typedef std::vector<BufferCeremonyLog> BufferCeremonyLogVec;
    BufferCeremonyLogVec m_bufferLogVec; 
};



#endif // __CEREMONYBRIEFMANAGER_H__
