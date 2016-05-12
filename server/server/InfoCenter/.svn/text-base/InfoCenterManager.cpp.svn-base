
#include "../group/Global.h"
#include "../../socket/SocketDef.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Log.h"
#include "../libServerFrame/QueryManager.h"
#include "../share/ServerMsgDef.h"
#include "../util/CommonFunc.h"
#include "InfoCenterManager.h"
#include <time.h>
#include "../GM/GameMsg_GM_share.h"
#include "../group/GroupServerCirculator.h"


extern CQueryManager* g_pQueryMgr;

InfoCenterManager::InfoCenterManager()
    : GameMsg_Processor(true, true)
{
}

InfoCenterManager::~InfoCenterManager()
{
}

InfoCenterManager & InfoCenterManager::Instance()
{
    static InfoCenterManager s_instance;
    return s_instance;
}

void InfoCenterManager::Initialize()
{
    RegMessageMap();
	LoadDbInfoAll();
}

void InfoCenterManager::RegMessageMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GetInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_AddInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_UpdateInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_RemoveInfoCenterRes);

	RegMsgProcessor(GM_M2G_GetInfoCenterRes, &InfoCenterManager::OnGMGetInfoCenterRes);
	RegMsgProcessor(GM_M2G_AddInfoCenterRes, &InfoCenterManager::OnGMAddInfoCenterRes);
	RegMsgProcessor(GM_M2G_UpdateInfoCenterRes, &InfoCenterManager::OnGMUpdateInfoCenterRes);
	RegMsgProcessor(GM_M2G_RemoveInfoCenterRes, &InfoCenterManager::OnGMRemoveInfoCenterRes);

	// 数据库回调注册
	RegQueryProcessor(QUERY_InfoCenter_LoadAllInfo, &InfoCenterManager::OnLoadAllInfo);
	RegQueryProcessor(QUERY_InfoCenter_AddInfo, &InfoCenterManager::OnAddInfo);
}

// 查询消息，
void InfoCenterManager::OnGMGetInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_G2M_GetInfoCenterRes sendMsg;
	for (InfoCenterPtrMap::iterator iter = m_infos.begin(); iter != m_infos.end(); ++iter)
	{
		InfoCenterItem* ptr = iter->second;
		if (NULL == ptr)
			continue;

		sendMsg.m_infos.push_back(*ptr); // 
	}

	SendMsgToGMServer(&sendMsg, slotPeer.GetSlot());

	return ;
}

// 插入一条消息：
void InfoCenterManager::OnGMAddInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_AddInfoCenterRes &rmsg = (GameMsg_M2G_AddInfoCenterRes &)msg;
	rmsg.m_items.m_updateTime = (unsigned int)time(NULL);

	InfoCenterItem *pItem = new InfoCenterItem();
	*pItem = rmsg.m_items;

	// 添加数据库，需要异步结果，取得ID，然后通知GameServer, 和 返回消息。
	CDelHelper helper(pItem);
	g_pQueryMgr->AddQuery(QUERY_InfoCenter_AddInfo, slotPeer.GetSlot(), pItem, 0, NULL, &helper);
	return ;
}


void InfoCenterManager::OnAddInfo(QueryBase &query)
{
	ENSURE_WITH_LOG_CMD(0 == query.m_nTag, return, "error to write infocenter info");
	ENSURE_WITH_LOG_CMD(NULL != query.m_pAttachment, return, "error to get NULL attachment on add infocenter cb");

	InfoCenterItem* ptr = (InfoCenterItem*)(query.m_pAttachment);
	
    do 
    {
        // 内存更新：
        ENSURE_WITH_LOG_CMD(m_infos.find(ptr->m_id) == m_infos.end(), break, "error to insert new info[%u]", ptr->m_id);

        // 内存更新
        InfoCenterItem* newItem = new InfoCenterItem();
        *newItem = *ptr;
        m_infos[newItem->m_id] = newItem; //

        // 回复GM 通知成功，
        GameMsg_G2M_AddInfoCenterRes msg2gm;
        msg2gm.m_items = *newItem;
        SendMsgToGMServer(&msg2gm, query.m_nTag); // 

        // 通知GameServer更新数据
        GameMsg_M2G_AddInfoCenterRes msg2game;
        msg2game.m_items = *newItem;
        BroadcastMsg2GameServer(&msg2game); // 游戏服广播
    } while (0);

    delete ptr;
    ptr = NULL;
    query.m_pAttachment = NULL;

	return ;
}

void InfoCenterManager::OnGMUpdateInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_UpdateInfoCenterRes &rmsg = (GameMsg_M2G_UpdateInfoCenterRes &)msg;
	 
	InfoCenterPtrMap::iterator iter = m_infos.find(rmsg.m_items.m_id);
	// 内存更新：
	ENSURE_WITH_LOG_CMD(iter != m_infos.end(), return, "erro to get info[%u] in mem", rmsg.m_items.m_id);
	ENSURE_WITH_LOG_CMD(NULL != iter->second, return, "InfoCenterItem[%d] should not null in map.", rmsg.m_items.m_id);

	rmsg.m_items.m_updateTime = (unsigned int)time(NULL);
	*(iter->second) = rmsg.m_items; // 直接更新掉

	// 更新数据库
	g_pQueryMgr->AddQuery(QUERY_InfoCenter_UpdateInfo, 0, iter->second, 0, NULL, NULL);

	// gameserver
	BroadcastMsg2GameServer(&rmsg);

// 	// 回复GM
// 	GameMsg_G2M_UpdateInfoCenterRes msg2Gm;
// 	msg2Gm.m_items = rmsg.m_items;
// 	SendMsgToGMServer(&msg2Gm, slotPeer.GetSlot());

	return ;
}

void InfoCenterManager::OnGMRemoveInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_M2G_RemoveInfoCenterRes &rmsg = (GameMsg_M2G_RemoveInfoCenterRes &)msg;

	InfoCenterPtrMap::iterator iter = m_infos.find(rmsg.m_id);
	ENSURE_WITH_LOG_CMD(iter != m_infos.end(), return, "can't find InfoCenterItem[%u] in map.", rmsg.m_id);
	ENSURE_WITH_LOG_CMD(NULL != iter->second, return, "infoCenterItem[%u] should not NULL", rmsg.m_id);

	// 内存更新
	delete iter->second;
	iter->second = NULL;
	m_infos.erase(iter);

	// 然后更新数据库
	g_pQueryMgr->AddQuery(QUERY_InfoCenter_RemoveInfo, rmsg.m_id);
	
	// gameserver
	BroadcastMsg2GameServer(&rmsg); // 

	return ;
}

void InfoCenterManager::OnLoadAllInfo(QueryBase &query)
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

void InfoCenterManager::LoadDbInfoAll()
{
	InfoCenterPtrMap *ptrMap = new InfoCenterPtrMap;
	CDelHelper helper(ptrMap);

	g_pQueryMgr->AddQuery(QUERY_InfoCenter_LoadAllInfo, 0, ptrMap, 0, NULL, &helper); // 执行加载
}



