/** Counter.h
* Description: ceremony room manager
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao	
* Create time: 2015-08-10 11:48:00
*/

#ifndef __CEREMONYROOMMANAGER_H__
#define __CEREMONYROOMMANAGER_H__

#include <map>
#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Base.h"
#include "ceremonyRoomHead.h"

class CeremonyRoom;

class CeremonyRoomManager : public GameMsg_Processor
{
protected:
	CeremonyRoomManager();
	~CeremonyRoomManager();

public:

	static CeremonyRoomManager& Instance(); // 

	void Initialize();
	void RegMessageMap();

    void OnUpdate(const unsigned long &lTimeElapsed);
    // ����Groupserver�ɹ���ͬ���Լ�������
    void OnGroupServerConnected();
    // ��������˳��������Ϣ
    void OnLogout(unsigned int roleID);
    void OnQuitDanceGroup(unsigned int nRoleID, unsigned nDanceGroupID, int nReason);

	// ���Դ�
	// ��ʽ���䴴����Ϊ�����ߣ�
	// 1/�������������統ǰ�������ڴ������������з��俪��������ʧ��
	// 2/���ش������䣬����ȷ�ϳɹ�����
	// 3/��ʽ����֮�󣬹رո÷���
	void TryOpenCeremonyRoom(unsigned int roleID);

	void ApplyOpenRoom(unsigned int danceGroupID, unsigned int roleID); // ���봴�����ȼ��Ǯ��Ȼ�������룩
	
	CeremonyRoom* GetCeremonyRoom(unsigned int danceGroupID); // ȡ���Լ����ŵķ���
	void CloseRoom(unsigned int danceGroupID, unsigned char reason); // �ر�������ʽ����

    int CeremonyRoomNum() const;

protected:
	// ���䴴��������
	void OnCeremonyApplyCreateRet(GameMsg_Base & msg, CSlotPeer & slotPeer); // ���䴴��������
    void OnDirectToClient(GameMsg_Base & msg, CSlotPeer & slotPeer); // ͸����ͻ��˵���Ϣ
    void OnBroadcastMemberKicked(GameMsg_Base & msg, CSlotPeer & slotPeer); // �㲥���߳�����Ϣ
    void OnBroadcastMemberQuit(GameMsg_Base & msg, CSlotPeer & slotPeer); // �㲥�˳�����Ϣ

private:
// 	void SendRoleMsg(GameMsg_Base &msg);

    // �������ɹ��ĳ���
	void CancelApplyCreateCeremony(unsigned int nRoleID, unsigned int nDanceGroupID);

	// �������
	unsigned int CeremonyTimes(unsigned int CeremonyTimes, unsigned int stamp, time_t now);
	// ���ݾٰ�Ĵ���������Ҫ��Ǯ��
	unsigned int CalcApplyCeremonyMoney(unsigned int times);

	// ���������г�Ա�㲥
	void BroadcastDanceGroupMsg(unsigned int nDanceGroupID, GameMsg_Base &msg);

protected:
    typedef std::map<unsigned int, CeremonyRoom> CeremonyRoomMap; // 
    CeremonyRoomMap m_mapCeremonyRoom; // ���ߵķ���

//	std::map<unsigned int, unsigned int> m_applyRequest; // ����������Ϣ


};




#endif
