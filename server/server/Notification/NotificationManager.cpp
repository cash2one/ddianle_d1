#include "NotificationManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Deletor.h"
#include "../../socket/Log.h"
#include "../GM/GameMsg_GM_share.h"
#include "../share/ServerMsgDef.h"
#include "../logic/LogicCirculator.h"
#include "../../socket/GameMsg_Processor.h"
#include "../libServerFrame/QueryManager.h"
#include "../util/CommonFunc.h"
#include "../logic/PlayerManager.h"
#include "../attribute/EntityAttributeComponent.h"

NotificationManager & NotificationManager::Instance()
{
	static NotificationManager s_instance;
	return s_instance;
}

NotificationManager::NotificationManager()
	: GameMsg_Processor(true, true)
{

}

NotificationManager::~NotificationManager()
{

}

void NotificationManager::InitInstance()
{
	RegisterMessage();
	LoadDbInfoAll();
}

void NotificationManager::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_AddInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_UpdateInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_RemoveInfoCenterRes);

	RegMsgProcessor(GM_M2G_AddInfoCenterRes, &NotificationManager::OnGMAddInfoCenterRes);
	RegMsgProcessor(GM_M2G_UpdateInfoCenterRes, &NotificationManager::OnGMUpdateInfoCenterRes);
	RegMsgProcessor(GM_M2G_RemoveInfoCenterRes, &NotificationManager::OnGMRemoveInfoCenterRes);

	RegQueryProcessor(QUERY_InfoCenter_LoadAllInfo, &NotificationManager::OnLoadAllInfo);
}


// 增、删、改的时候，需要做广播
void NotificationManager::OnGMAddInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_AddInfoCenterRes &rmsg = (GameMsg_M2G_AddInfoCenterRes &)msg;

	InfoCenterPtrMap::iterator iter = m_infos.find(rmsg.m_items.m_id);
	ENSURE_WITH_LOG_CMD(iter == m_infos.end(), return, "InfoCenter[%u] already in mem", rmsg.m_items.m_id);

	// 更新内存，并且发送广播
	InfoCenterItem *ptr = new InfoCenterItem();
	*ptr = rmsg.m_items;
	m_infos.insert(std::make_pair(ptr->m_id, ptr)); // 添加进来，
	
	
	// 向所有玩家发送广播。
	GameMsg_S2C_InfoCenterNotification sendMsg;
	sendMsg.Set(ptr->m_id, ptr->m_updateTime, ptr->m_type, ptr->m_group, 1); 

	std::set<int> forbiddenSet;
	bool noForbidden = IsNoForbidden(rmsg.m_items.m_strHidPid);
	ForbiddenString2Set(rmsg.m_items.m_strHidPid, forbiddenSet);
	std::list<CRoleEntity*> listAllRole;
	CPlayerManager::Instance().GetAllPlayers(listAllRole);
	for (std::list<CRoleEntity*>::iterator iter = listAllRole.begin(); iter != listAllRole.end(); ++iter)
	{
		CRoleEntity *pRole = *iter;
		ENSURE_WITH_LOG_CMD(NULL != pRole, continue, "error to get NULL CRoleEntity");
		CRoleAttribute *pAttr = pRole->GetComponent<CRoleAttribute>();
		ENSURE_WITH_LOG_CMD(NULL != pAttr, continue, "role[%u] find attr error", pRole->GetRoleID());
		
		if (noForbidden || forbiddenSet.find(pAttr->PID()) == forbiddenSet.end())
		{
			pRole->SendPlayerMsg(&sendMsg);
		}
	}

	return ;
}

void NotificationManager::OnGMUpdateInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_UpdateInfoCenterRes &rmsg = (GameMsg_M2G_UpdateInfoCenterRes &)msg;

	InfoCenterPtrMap::iterator iter = m_infos.find(rmsg.m_items.m_id);
	ENSURE_WITH_LOG_CMD(iter != m_infos.end(), return, "InfoCenter[%u] don't in mem on update", rmsg.m_items.m_id);
	ENSURE_WITH_LOG_CMD(NULL != iter->second, return, "InfoCenter[%u] should not be NULL on update", rmsg.m_items.m_id);

	std::set<int> forbiddenSetOrigin, forbiddenSetNew;
	bool noForbiddenOrigin = IsNoForbidden(iter->second->m_strHidPid);
	bool noForbiddenNew = IsNoForbidden(rmsg.m_items.m_strHidPid);
	ForbiddenString2Set(rmsg.m_items.m_strHidPid, forbiddenSetNew);
	ForbiddenString2Set(iter->second->m_strHidPid, forbiddenSetOrigin);

	*(iter->second) = rmsg.m_items;
	InfoCenterItem *ptr = &(rmsg.m_items);

	GameMsg_S2C_InfoCenterNotification sendMsgAdd, sendMsgUpdate, sendMsgRemove;
	sendMsgAdd.Set(ptr->m_id, ptr->m_updateTime, ptr->m_type, ptr->m_group, EInfoCenterOp_Add); 
	sendMsgUpdate.Set(ptr->m_id, ptr->m_updateTime, ptr->m_type, ptr->m_group, EInfoCenterOp_Update);
	sendMsgRemove.Set(ptr->m_id, ptr->m_updateTime, ptr->m_type, ptr->m_group, EInfoCenterOp_Remove); 

	bool bOriginSend = false;
	bool bNewSend = false;

	std::list<CRoleEntity*> listAllRole;
	CPlayerManager::Instance().GetAllPlayers(listAllRole);
	for (std::list<CRoleEntity*>::iterator iter = listAllRole.begin(); iter != listAllRole.end(); ++iter)
	{
		CRoleEntity *pRole = *iter;
		ENSURE_WITH_LOG_CMD(NULL != pRole, continue, "error to get NULL CRoleEntity");
		CRoleAttribute *pAttr = pRole->GetComponent<CRoleAttribute>();
		ENSURE_WITH_LOG_CMD(NULL != pAttr, continue, "role[%u] find attr error", pRole->GetRoleID());

		bOriginSend = (noForbiddenOrigin || (forbiddenSetOrigin.find(pAttr->PID()) == forbiddenSetOrigin.end()));
		bNewSend = (noForbiddenNew || (forbiddenSetNew.find(pAttr->PID()) == forbiddenSetNew.end()));

		if (bOriginSend && bNewSend)
		{
			pRole->SendPlayerMsg(&sendMsgUpdate);
		}
		else if (bOriginSend && !bNewSend)
		{
			pRole->SendPlayerMsg(&sendMsgRemove);
		}
		else if (!bOriginSend && bNewSend)
		{
			pRole->SendPlayerMsg(&sendMsgAdd); // 新添加的一条消息
		}
	}

	return ;
}

void NotificationManager::OnGMRemoveInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_RemoveInfoCenterRes &rmsg = (GameMsg_M2G_RemoveInfoCenterRes &)msg;
	InfoCenterPtrMap::iterator iter = m_infos.find(rmsg.m_id);
	ENSURE_WITH_LOG_CMD(iter != m_infos.end(), return, "InfoCenter[%u] don't in mem on update", rmsg.m_id);
	ENSURE_WITH_LOG_CMD(NULL != iter->second, return, "InfoCenter[%u] should not be NULL on update", rmsg.m_id);

	std::set<int> forbiddenSet;
	bool noForbidden = IsNoForbidden(iter->second->m_strHidPid);
	ForbiddenString2Set(iter->second->m_strHidPid, forbiddenSet);

	// 向所有玩家发送广播。
	GameMsg_S2C_InfoCenterNotification sendMsg;
	sendMsg.Set(rmsg.m_id, 0, iter->second->m_type, 0, 3); 

	delete iter->second;
	iter->second = NULL;
	m_infos.erase(iter);

	std::list<CRoleEntity*> listAllRole;
	CPlayerManager::Instance().GetAllPlayers(listAllRole);
	for (std::list<CRoleEntity*>::iterator iter = listAllRole.begin(); iter != listAllRole.end(); ++iter)
	{
		CRoleEntity *pRole = *iter;
		ENSURE_WITH_LOG_CMD(NULL != pRole, continue, "error to get NULL CRoleEntity");
		CRoleAttribute *pAttr = pRole->GetComponent<CRoleAttribute>();
		ENSURE_WITH_LOG_CMD(NULL != pAttr, continue, "role[%u] find attr error", pRole->GetRoleID());

		if (noForbidden || forbiddenSet.find(pAttr->PID()) == forbiddenSet.end())
		{
			pRole->SendPlayerMsg(&sendMsg);
		}
	}
}


void NotificationManager::LoadDbInfoAll()
{
	InfoCenterPtrMap *ptrMap = new InfoCenterPtrMap;
	CDelHelper helper(ptrMap);

	g_pQueryMgr->AddQuery(QUERY_InfoCenter_LoadAllInfo, 0, ptrMap, 0, NULL, &helper); // 执行加载
}

void NotificationManager::OnLoadAllInfo(QueryBase &query)
{
	ENSURE_WITH_LOG_CMD(NULL != query.m_pAttachment, return, "attachment should not null on load infocenter all");
	InfoCenterPtrMap *tempMap = ((InfoCenterPtrMap*)(query.m_pAttachment));

	for (InfoCenterPtrMap::iterator iter = tempMap->begin(); iter != tempMap->end(); ++iter)
	{
		m_infos.insert(std::make_pair(iter->first, iter->second));
	}

	tempMap->clear();
    delete tempMap;
    tempMap = NULL;
    query.m_pAttachment = NULL;
    
    return ;
}

bool NotificationManager::FillInfo(std::vector<unsigned int> &ids, std::vector<InfoCenterTranFull> &fullInfo, unsigned int pid)
{
	for (size_t i = 0; i < ids.size(); ++i)
	{
		InfoCenterPtrMap::iterator iter = m_infos.find(ids[i]);

		if (m_infos.end() == iter)
			return false;

		InfoCenterTranFull info;
		info.m_id = iter->second->m_id;
		info.m_updateTime = iter->second->m_updateTime;
		info.m_group = iter->second->m_group;
		info.m_icon = iter->second->m_icon;
		info.m_type = iter->second->m_type; // 新闻，分享
		info.m_strTitle = iter->second->m_strTitle;
		info.m_strContext = iter->second->m_strContext;
		info.m_strHref = iter->second->m_strHref;
		fullInfo.push_back(info);
	}

	return true;
}

bool NotificationManager::IsForbidenPid(std::string &pids, unsigned int pid)
{
	if(IsNoForbidden(pids))
		return false;

	std::set<int> pidSet;
	ForbiddenString2Set(pids, pidSet);
	return (pidSet.find(pid) != pidSet.end());
}

bool NotificationManager::IsNoForbidden(std::string &strPids)
{
	return strPids.compare("0") == 0;
}

void NotificationManager::ForbiddenString2Set(std::string &pids, std::set<int> &pidSet)
{
	SplitString(pids, std::string("|"), pidSet);
}

void NotificationManager::FillAllInfo(std::vector<InfoCenterItem*> &fullInfo, unsigned int pid)
{
	for (InfoCenterPtrMap::iterator iter = m_infos.begin(); iter != m_infos.end(); ++iter)
	{
		if (!IsForbidenPid(iter->second->m_strHidPid, pid))
		{
			fullInfo.push_back(iter->second);
		}
	}

	return ;
}

