#include <time.h>
#include "InfoCenterMgr.h"
#include "GMMsgDef.h"
#include "GameMsg_GM.h"
#include "GameMsg_GM_share.h"
#include "../../socket/GameMsg_Map.h"
#include "GMEntity.h"
#include "GMEntityMgr.h"
#include "priv/PrivCmdMacroDef.h"
#include "TipMsgDef.h"
#include "priv/PrivMgr.h"
#include "GMCirculator.h"
#include "JsonLog.h"

InfoCenterMgr::InfoCenterMgr():
GameMsg_Processor(true, true)
{

}

InfoCenterMgr::~InfoCenterMgr()
{
}

InfoCenterMgr & InfoCenterMgr::Instance()
{
	static InfoCenterMgr s_infoCenterMgr;
	
	return s_infoCenterMgr;
}

void InfoCenterMgr::Initialize()
{
	_RegisterMessage();
	_RegisterMessageProcessor();
}


void InfoCenterMgr::_RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2M_GetInfoCenterResCM);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2M_AddInfoCenterResCM);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2M_UpdateInfoCenterResCM);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2M_RemoveInfoCenterResCM);

	GAMEMSG_REGISTERCREATOR(GameMsg_G2M_GetInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2M_AddInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2M_UpdateInfoCenterRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2M_RemoveInfoCenterRes);


	return ;
}

void InfoCenterMgr::_RegisterMessageProcessor()
{
	RegMsgProcessor(GM_C2M_GetInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvClientGet);
	RegMsgProcessor(GM_C2M_AddInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvClientAdd);
    RegMsgProcessor(GM_C2M_UpdateInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvClientUpdate);
    RegMsgProcessor(GM_C2M_RemoveInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvClientRemove);
	
	RegMsgProcessor(GM_G2M_GetInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvGroupGet);
	RegMsgProcessor(GM_G2M_AddInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvGroupAdd);
	RegMsgProcessor(GM_G2M_UpdateInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvGroupUpdate);
	RegMsgProcessor(GM_G2M_RemoveInfoCenterRes, &InfoCenterMgr::OnGMInfoCenterRecvGroupRemove);
	return ;
}

void Tran(InfoCenterItem &dst, const InfoCenterItemCM &src)
{
	dst.m_id = src.m_id;
	dst.m_updateTime = src.m_updateTime;
	dst.m_group = src.m_group;
	dst.m_icon = src.m_icon;
	dst.m_type = src.m_type; // 新闻，分享
	dst.m_strTitle = src.m_strTitle;
	dst.m_strContext = src.m_strContext;
	dst.m_strHref = src.m_strHref;
	dst.m_strHidPid = src.m_strHidPid;

	return ;
}

void Tran(InfoCenterItemCM &dst, const InfoCenterItem &src)
{
	dst.m_id = src.m_id;
	dst.m_updateTime = src.m_updateTime;
	dst.m_group = src.m_group;
	dst.m_icon = src.m_icon;
	dst.m_type = src.m_type; // 新闻，分享
	dst.m_strTitle = src.m_strTitle;
	dst.m_strContext = src.m_strContext;
	dst.m_strHref = src.m_strHref;
	dst.m_strHidPid = src.m_strHidPid;

	return ;
}


void InfoCenterMgr::OnGMInfoCenterRecvClientGet(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 仅进行权限验证和转发
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	if (!CPrivMgr::Instance().HasPriv(pRole->GetName(),EUserPrivType_InfoCenter))
	{
		GameMsg_S2C_PrivError retMsg;
		pRole->SendMsgToGM(&retMsg);
		return ;
	}

	GameMsg_M2G_GetInfoCenterRes tranMsg;
	// 转发：
	pRole->SendMsgToGroup(&tranMsg); // 
	return ; // 
}

void InfoCenterMgr::OnGMInfoCenterRecvClientAdd(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 仅进行权限验证和转发
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	if (!CPrivMgr::Instance().HasPriv(pRole->GetName(),EUserPrivType_InfoCenter))
	{
		GameMsg_S2C_PrivError retMsg;
		pRole->SendMsgToGM(&retMsg);
		return ;
	}

	GameMsg_C2M_AddInfoCenterResCM &cmmsg = (GameMsg_C2M_AddInfoCenterResCM &)msg;
	GameMsg_M2G_AddInfoCenterRes tranMsg;
	Tran(tranMsg.m_items, cmmsg.m_items);

    std::string jparam = GMJsonLog::AddInfoCenterToJson(cmmsg.m_items);
    CGMCirculator::LogGM(pRole->GetAccount(), EGMActionType_InfoCenter, jparam);

	// 转发：
	pRole->SendMsgToGroup(&tranMsg); // 
	return ; // 
}

void InfoCenterMgr::OnGMInfoCenterRecvClientUpdate(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 仅进行权限验证和转发
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	if (!CPrivMgr::Instance().HasPriv(pRole->GetName(),EUserPrivType_InfoCenter))
	{
		GameMsg_S2C_PrivError retMsg;
		pRole->SendMsgToGM(&retMsg);
		return ;
	}

	GameMsg_C2M_UpdateInfoCenterResCM &cmmsg = (GameMsg_C2M_UpdateInfoCenterResCM &)msg;
	GameMsg_M2G_UpdateInfoCenterRes tranMsg;
	Tran(tranMsg.m_items, cmmsg.m_items);

    std::string jparam = GMJsonLog::UpdateInfoCenterToJson(cmmsg.m_items);
    CGMCirculator::LogGM(pRole->GetAccount(), EGMActionType_InfoCenter, jparam);

    // 转发：
	pRole->SendMsgToGroup(&tranMsg); // 
	return ; // 
}

void InfoCenterMgr::OnGMInfoCenterRecvClientRemove(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 仅进行权限验证和转发
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	if (!CPrivMgr::Instance().HasPriv(pRole->GetName(),EUserPrivType_InfoCenter))
	{
		GameMsg_S2C_PrivError retMsg;
		pRole->SendMsgToGM(&retMsg);
		return ;
	}

	GameMsg_C2M_RemoveInfoCenterResCM &cmmsg = (GameMsg_C2M_RemoveInfoCenterResCM &)msg;
	GameMsg_M2G_RemoveInfoCenterRes tranMsg;
	tranMsg.m_id = cmmsg.m_id;

    std::string jparam = GMJsonLog::RemoveInfoCenterToJson(cmmsg.m_id);
    CGMCirculator::LogGM(pRole->GetAccount(), EGMActionType_InfoCenter, jparam);

    // 转发：
	pRole->SendMsgToGroup(&tranMsg); // 
	return ; // 
}


void InfoCenterMgr::OnGMInfoCenterRecvGroupGet(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 直接转发消息。
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	GameMsg_G2M_GetInfoCenterRes &thisMsg = (GameMsg_G2M_GetInfoCenterRes &)msg;
	GameMsg_M2C_GetInfoCenterResCM tranMsg;
	for (size_t i = 0; i < thisMsg.m_infos.size(); ++i)
	{
		tranMsg.m_infos.push_back(InfoCenterItemCM());
		Tran(tranMsg.m_infos[i], thisMsg.m_infos[i]);
	}

	pRole->SendMsgToGM(&tranMsg);

	return ;
}

void InfoCenterMgr::OnGMInfoCenterRecvGroupAdd(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 直接转发消息。
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	GameMsg_G2M_AddInfoCenterRes &thisMsg = (GameMsg_G2M_AddInfoCenterRes &)msg;
	GameMsg_M2C_AddInfoCenterResCM tranMsg;
	Tran(tranMsg.m_items, thisMsg.m_items);

	pRole->SendMsgToGM(&tranMsg);

	return ;
}

void InfoCenterMgr::OnGMInfoCenterRecvGroupUpdate(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 直接转发消息。
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	GameMsg_G2M_UpdateInfoCenterRes &thisMsg = (GameMsg_G2M_UpdateInfoCenterRes &)msg;
	GameMsg_M2C_UpdateInfoCenterResCM tranMsg;
	Tran(tranMsg.m_items, thisMsg.m_items);

	pRole->SendMsgToGM(&tranMsg);

	return ;
}

void InfoCenterMgr::OnGMInfoCenterRecvGroupRemove(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ // 直接转发消息。
	GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
	if (pRole == NULL)
		return ; //

	GameMsg_G2M_RemoveInfoCenterRes &thisMsg = (GameMsg_G2M_RemoveInfoCenterRes &)msg;
	GameMsg_M2C_RemoveInfoCenterResCM tranMsg;

	tranMsg.m_result = thisMsg.m_result;
	tranMsg.m_id = thisMsg.m_id;

	pRole->SendMsgToGM(&tranMsg);

	return ;
}

