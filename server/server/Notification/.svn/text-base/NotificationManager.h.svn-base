/** Counter.h
* Description: Notification manager.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao
* Create time: 2014-08-16 11:48:00
*/
#ifndef __NOTIFICATIONMANAGER_H_H__
#define __NOTIFICATIONMANAGER_H_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/QueryManagerBase.h"
#include "../../socket/GameMsg_Base.h"
#include "GameMsg_Notification.h"
#include "../GM/GameMsg_GM_share.h"
#include <vector>
#include <string>

class NotificationManager : public GameMsg_Processor
{
public:
	static NotificationManager & Instance();

public:
	NotificationManager();
	virtual ~NotificationManager();

public:
	void InitInstance();
	void RegisterMessage();

	void OnGMAddInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMUpdateInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMRemoveInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void LoadDbInfoAll(); // 启动时，加载数据库
	void OnLoadAllInfo(QueryBase &query);

	bool FillInfo(std::vector<unsigned int> &ids, std::vector<InfoCenterTranFull> &fullInfo, unsigned int pid);
	void FillAllInfo(std::vector<InfoCenterItem*> &fullInfo, unsigned int pid);

	bool IsForbidenPid(std::string &pids, unsigned int pid);
	bool IsNoForbidden(std::string &strPids);
	void ForbiddenString2Set(std::string &pids, std::set<int> &pidSet);

protected:
	InfoCenterPtrMap m_infos; // game server 也管理所有的数据

};

#endif //__NOTIFICATIONMANAGER_H_H__

