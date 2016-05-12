#ifndef __COMPENSATION_COMPONENT_H__
#define __COMPENSATION_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include <vector>
#include <set>

class	CEntityItemComponent;
class	CEntityNetComponent;
class	CEntityAttributeComponent;
class	CEntityMailComponent;

class	CCompensationComponent : public CEntityComponent
{
public:
	CCompensationComponent();
	~CCompensationComponent();
public:
	virtual void OnEvent(CComponentEvent& refEvent) {}
	virtual void Start();
	virtual void OnLogin();
	virtual void OnLogout() {}
	virtual void OnUpdate(const unsigned long &nTimeElapsed) {}
	virtual void SerializeComponent(CParamPool &IOBuff) {}
	virtual const char* SerializeName(){ return "Compensation"; }
	virtual bool CreateFromDB( ROLEINFO_DB* pRoleInfoDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const { return true; }
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
	virtual void SendExtraStaticData() {}
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
	void RegComponentNetMsgMap();

private:
	void OnGetCompensation(GameMsg_Base& msg,CSlotPeer& slot);

	void NoticeNextCompensation();
	int	 NextCompensationID();	//取得一条补偿记录，返回-1表示没有获取到有效的补偿记录
	
	void OnInsertRoleCompensation(QueryBase& query);

private:
	CEntityItemComponent*			m_pItemCom;
	CEntityNetComponent*			m_pNetCom;
	CEntityAttributeComponent*		m_pRoleCom;
	CEntityMailComponent*			m_pMailCom;
	std::set<int>					m_setCompensation;
};

typedef CRoleComponentImpl<CCompensationComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleCompensation;

#endif

