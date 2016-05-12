#ifndef	__ENTITYTIMERCOMPONENT_H__
#define __ENTITYTIMERCOMPONENT_H__
#include "RoleComponentImpl.h"
#include "EntityComponent.h"
#include <functional>
#include <queue>
using namespace std;

class CTimerEvent
{
public:
	int nID;
	unsigned short Data;

	CTimerEvent(int _nID,unsigned short _Data){
		nID = _nID;
		Data = _Data;
	}
};

typedef std::map<unsigned int,std::vector<CTimerEvent> > TimerMap;

class CEntityTimerComponent :
	public CEntityComponent
{

public:
	CEntityTimerComponent ();
	~CEntityTimerComponent ();

public:
	virtual void  Start();
	void          RegComponentNetMsgMap();
	virtual void  SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "Timer"; };
	void          OnEvent(CComponentEvent& refEvent );
	bool          CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const {return true;};
	virtual void  OnUpdate(const unsigned long &nTimeElapsed);
	virtual void  OnLogin();
	virtual void  OnLogout();
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
	bool AddTimer(unsigned int nEndTime,int nType,unsigned short nData);

private:
	void SendEvent(unsigned int nEndTime);

private:
	std::priority_queue< unsigned int,std::vector<unsigned int>,std::greater<unsigned int> > m_qTimer;
	TimerMap m_aMapTimer;
	unsigned long m_nElapsedTime;
};

typedef CRoleComponentImpl<CEntityTimerComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleTimer;

#endif

//end file

