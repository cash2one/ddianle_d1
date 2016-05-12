#ifndef		__LOCATIONMGR_H__
#define		__LOCATIONMGR_H__

#include <map>
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"

class CRoleEntity;
typedef 	std::map<unsigned int, CRoleEntity *> TMapZone;

class CLocationMgr:
	public GameMsg_Processor
{
private:
	CLocationMgr();
	~CLocationMgr();
public:
	static CLocationMgr & Instance();
	void AddEntity(int nLocation, CRoleEntity *pEntity);
	CRoleEntity * RemoveEntity(int nLocation, unsigned int nRoleID);
	void Initialize();
	void PlayerLogout(int nLocation, unsigned int nRoleID);

	void EnterZone(int nLocation, unsigned int nRoleID);
	void EnterZone(int nLocation, unsigned short nIndex);
	bool IsInZone(int nLocation, unsigned int nRoleID);
	void SendLocationChatMsg(int nLocation, unsigned int nSenderID, GameMsg_Base *pMsg);
private:
private:
	void RegisterMessage();
	void RegisterMessageProcessor();
private:
	void OnEnterZone(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
private:
	TMapZone m_mapZone[Zone_Max];
};









#endif

