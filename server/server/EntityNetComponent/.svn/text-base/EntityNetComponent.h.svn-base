#ifndef __ENTITYNET_COMPONENT_H__
#define __ENTITYNET_COMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"


class CEntityNetComponent :
	public CEntityComponent
{
public:
	CEntityNetComponent(void);
	~CEntityNetComponent(void);

public:
	virtual void Start();
	void RegComponentNetMsgMap();
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void OnUpdate(const unsigned long &nTimeElapsed) {};
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

	//method
public:
	void SerializeComponent(CParamPool &IOBuff) {};
	virtual const char* SerializeName(){ return "Net"; };
	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB){return true;};
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const {return true;};

	bool SendPlayerMsg(GameMsg_Base * pMsg);
	void SetPlayerTag(PLAYERTAG * pTag);
	void Send2GroupServer(GameMsg_Base *pMsg);

	PLAYERTAG * GetPlayerTag();
	__int64 GetSessionID();
	unsigned short GetPlayerSlot();
	//unsigned short GetIndex(); //意思表述不清楚所以改成GetPlayerLogicIndex
	unsigned short GetPlayerLogicIndex();
	int GetVerType() const;
private:
	PLAYERTAG * m_pTag;
private:
	virtual void OnEvent(CComponentEvent& refEvent ){};

};
typedef CRoleComponentImpl<CEntityNetComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleNet;

#endif

//end file



