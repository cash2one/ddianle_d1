#include "GMEntityMgr.h"
#include "GMEntity.h"
#include "../../socket/Log.h"


GMEntityMgr::GMEntityMgr()
:m_nEntitySession(0)
{
}

GMEntityMgr::~GMEntityMgr()
{
}

GMEntityMgr &GMEntityMgr::Instance()
{
	static GMEntityMgr  s_GMEntityMgr;

	return s_GMEntityMgr;
}

void GMEntityMgr::AddGMEntity(GMEntity *pEntity)
{
	if (pEntity != NULL) {
		AddSessionEntity(pEntity->GetSession(), pEntity);
		AddSlotEntity(pEntity->GetSlot(), pEntity);
		AddAccountEntity(pEntity->GetAccount(), pEntity);
	}
}

void GMEntityMgr::RemoveGMEntity(GMEntity *pEntity)
{
	if (pEntity != NULL) {
		RemoveSessionEntity(pEntity->GetSession());
		RemoveSlotEntity(pEntity->GetSlot());
		RemoveAccountEntity(pEntity->GetAccount());
	}
}

void GMEntityMgr::AddSessionEntity(__int64 nSession, GMEntity *pEntity)
{
	if (pEntity != NULL) {
		map<__int64, GMEntity *>::iterator itr = m_mapSessionEntity.find(nSession);
		if (itr == m_mapSessionEntity.end()) {
			m_mapSessionEntity.insert(make_pair(nSession, pEntity));
		}
	}
}

GMEntity* GMEntityMgr::RemoveSessionEntity(__int64 nSession)
{
	GMEntity *pEntity = NULL;
	map<__int64, GMEntity *>::iterator itr = m_mapSessionEntity.find(nSession);

	if (itr != m_mapSessionEntity.end()) {
		pEntity = itr->second;
		m_mapSessionEntity.erase(itr);
		return pEntity;
	}

	return NULL;
}

GMEntity * GMEntityMgr::GetEntityBySession( __int64 nSession )
{
	map<__int64, GMEntity *>::iterator itr = m_mapSessionEntity.find(nSession);
	if (itr != m_mapSessionEntity.end()) {
		return itr->second;
	}

	return NULL;
}

void GMEntityMgr::AddSlotEntity(unsigned short nSlot, GMEntity *pEntity)
{
	if (pEntity != NULL) {
        map<unsigned short, GMEntity *>::iterator itr = m_maptSlotEntity.find(nSlot);
		if (itr == m_maptSlotEntity.end()) {
			m_maptSlotEntity.insert(make_pair(nSlot, pEntity));
		}
	}
}

GMEntity* GMEntityMgr::GetEntityBySlot(unsigned short nSlot)
{
    map<unsigned short, GMEntity *>::iterator itr = m_maptSlotEntity.find(nSlot);
	if (itr != m_maptSlotEntity.end()) {
		return itr->second;
	}

	return NULL;
}

GMEntity *GMEntityMgr::GetEntityByAccount(unsigned int nAccount)
{
	map<unsigned int, GMEntity *>::iterator itr = m_mapAccountEntity.find(nAccount);
	if (itr != m_mapAccountEntity.end()) {
		return itr->second;
	}

	return NULL;
}

GMEntity * GMEntityMgr::RemoveSlotEntity(unsigned short nSlot)
{
	GMEntity *pEntity = NULL;
	map<unsigned short, GMEntity *>::iterator itr = m_maptSlotEntity.find(nSlot);

	if (itr != m_maptSlotEntity.end()) {
		pEntity = itr->second;
		m_maptSlotEntity.erase(itr);
		return pEntity;
	}

	return NULL;
}

void GMEntityMgr::AddAccountEntity(unsigned int nAccount, GMEntity *pEntity)
{
	if (pEntity != NULL) {
		map<unsigned int, GMEntity *>::iterator itr = m_mapAccountEntity.find(nAccount);
		if (itr == m_mapAccountEntity.end()) {
			m_mapAccountEntity.insert(make_pair(nAccount, pEntity));
		}
	}
}

GMEntity * GMEntityMgr::RemoveAccountEntity(unsigned int nAccount)
{
	GMEntity *pEntity = NULL;
	map<unsigned int, GMEntity *>::iterator itr = m_mapAccountEntity.find(nAccount);
	if (itr != m_mapAccountEntity.end()) {
		pEntity = itr->second;	
		m_mapAccountEntity.erase(itr);
		return pEntity;
	}

	return NULL;
}

__int64 GMEntityMgr::GetCurSession()
{
	return m_nEntitySession;
}

__int64 GMEntityMgr::GetNewSession()
{
	return ++m_nEntitySession;
}

void GMEntityMgr::PlayerLogout( GMEntity *pEntity )
{
	if (pEntity != NULL) {
		RemoveGMEntity(pEntity);	
		WriteLog(LOGLEVEL_DEBUG, "player logout:slot = %u, session = %lld, account = %u\n", pEntity->GetSlot(), pEntity->GetSession(), pEntity->GetAccount());
	}
}


