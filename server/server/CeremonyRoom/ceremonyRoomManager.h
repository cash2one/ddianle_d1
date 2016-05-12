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
    // 连接Groupserver成功，同步自己的数据
    void OnGroupServerConnected();
    // 处理玩家退出房间的信息
    void OnLogout(unsigned int roleID);
    void OnQuitDanceGroup(unsigned int nRoleID, unsigned nDanceGroupID, int nReason);

	// 尝试打开
	// 仪式房间创建分为三步走：
	// 1/申请锁定，假如当前有人正在创建，或者已有房间开启，则开启失败
	// 2/本地创建房间，并且确认成功创建
	// 3/仪式结束之后，关闭该房间
	void TryOpenCeremonyRoom(unsigned int roleID);

	void ApplyOpenRoom(unsigned int danceGroupID, unsigned int roleID); // 申请创建（先检测钱，然后再申请）
	
	CeremonyRoom* GetCeremonyRoom(unsigned int danceGroupID); // 取得自己舞团的房间
	void CloseRoom(unsigned int danceGroupID, unsigned char reason); // 关闭入团仪式房间

    int CeremonyRoomNum() const;

protected:
	// 房间创建请求结果
	void OnCeremonyApplyCreateRet(GameMsg_Base & msg, CSlotPeer & slotPeer); // 房间创建请求结果
    void OnDirectToClient(GameMsg_Base & msg, CSlotPeer & slotPeer); // 透传入客户端的消息
    void OnBroadcastMemberKicked(GameMsg_Base & msg, CSlotPeer & slotPeer); // 广播被踢出的消息
    void OnBroadcastMemberQuit(GameMsg_Base & msg, CSlotPeer & slotPeer); // 广播退出的消息

private:
// 	void SendRoleMsg(GameMsg_Base &msg);

    // 创建不成功的撤销
	void CancelApplyCreateCeremony(unsigned int nRoleID, unsigned int nDanceGroupID);

	// 计算次数
	unsigned int CeremonyTimes(unsigned int CeremonyTimes, unsigned int stamp, time_t now);
	// 根据举办的次数计算需要的钱数
	unsigned int CalcApplyCeremonyMoney(unsigned int times);

	// 舞团内所有成员广播
	void BroadcastDanceGroupMsg(unsigned int nDanceGroupID, GameMsg_Base &msg);

protected:
    typedef std::map<unsigned int, CeremonyRoom> CeremonyRoomMap; // 
    CeremonyRoomMap m_mapCeremonyRoom; // 本线的房间

//	std::map<unsigned int, unsigned int> m_applyRequest; // 舞团申请信息


};




#endif
