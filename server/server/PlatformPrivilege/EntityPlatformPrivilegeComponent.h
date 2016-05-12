#ifndef _ENTITYPLATFORMPRIVILEGE_H__
#define _ENTITYPLATFORMPRIVILEGE_H__
#pragma once

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "UCVIP/UCVIP.h"

class CEntityNetComponent;
class CEntityAttributeComponent;

/************************************************************************/
/* 平台特权奖励处理
在lua文件中配置平台对应的特权奖励，代码中添加对应平台的特权奖励
处理（每个平台对应不同的平台协议）
*/
/************************************************************************/
class CEntityPlatformPrivilege :
	public CEntityComponent
{
public:
	CEntityPlatformPrivilege();
	~CEntityPlatformPrivilege();

public:
	virtual void Start();
	void RegComponentNetMsgMap();

public:
	CEntityNetComponent * GetRoleNet(){ return this->m_pRoleNet; }

public:
	virtual const char* SerializeName(){ return "PlatformPrivilege"; };

	bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	bool PacketToDB(ROLEINFO_DB*pRoleInfoDB) const;
	virtual void SerializeComponent(CParamPool &IOBuff) ;

	virtual void OnLogin();
	virtual void OnLogout();
	virtual void OnUpdate(const unsigned long & nTimeElapsed);
	virtual void OnEvent(CComponentEvent& refEvent );
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

private:
	//UCVIP
	void _OnGetUCVIPRewardStatue( GameMsg_Base & rMsg, CSlotPeer & rSlotPeer );
	void _OnGetUCVIPRewardDetial( GameMsg_Base &rMsg, CSlotPeer & rSlotPeer );
	void _OnGainUCVIPReward( GameMsg_Base & rMsg, CSlotPeer & rSlotPeer );


private:
	void _SendPlayerMsg( GameMsg_Base * pMsg );

private:
	CEntityNetComponent						*m_pRoleNet;
	CEntityAttributeComponent				*m_pRoleAttribute;

private:
	std::set<int> m_setUCVIPReward; //UCVIP特权状态

	CUCVIPInfo  m_ucvipInfo; //存放平台信息

};

typedef CRoleComponentImpl< CEntityPlatformPrivilege, CGetRoleProcessorObj, CGetRoleProcessorObj >CRolePlatformPrivilege;

#endif //_ENTITYPLATFORMPRIVILEGE_H__
