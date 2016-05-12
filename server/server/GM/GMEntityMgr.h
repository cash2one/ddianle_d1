#ifndef __GMENITYMGR_H__
#define __GMENITYMGR_H__
#include <map>
#include "../../socket/Windefine.h"

using namespace std;

class GMEntity;

class GMEntityMgr
{
private:
	GMEntityMgr();
	~GMEntityMgr();
public:
	static GMEntityMgr & Instance();

	void AddGMEntity(GMEntity *pEntity);
	void RemoveGMEntity(GMEntity *pEntity);
	void AddSessionEntity(__int64 nSession, GMEntity *pEntity);
	GMEntity * RemoveSessionEntity(__int64 nSession);
    void AddSlotEntity(unsigned short nSession, GMEntity *pEntity);
    GMEntity * RemoveSlotEntity(unsigned short nSlot);
	void AddAccountEntity(unsigned int nAccount, GMEntity *pEntity);
	GMEntity * RemoveAccountEntity(unsigned int nAccount);
public:
	GMEntity *GetEntityBySession(__int64 nSession);
    GMEntity *GetEntityBySlot(unsigned short nSlot);
	GMEntity *GetEntityByAccount(unsigned int nAccount);
	__int64 GetCurSession();
	__int64 GetNewSession();

	void PlayerLogout(GMEntity *pEntity);
public:
	__int64 m_nEntitySession;

	map<__int64, GMEntity *> m_mapSessionEntity;
	map<unsigned short, GMEntity *> m_maptSlotEntity;
	map<unsigned int, GMEntity *> m_mapAccountEntity;
};






#endif

