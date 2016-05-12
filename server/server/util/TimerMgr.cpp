#include <time.h>
#include "TimerMgr.h"

CTimerMgr::CTimerMgr()
{

}

CTimerMgr::~CTimerMgr()
{
	for (MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ++itrMap) {
		for (std::vector<ITimer *>::iterator itrVec = itrMap->second.begin(); itrVec != itrMap->second.end(); ++itrVec) {
			if (*itrVec != NULL) {
				delete *itrVec, *itrVec = NULL;
			}
		}
		itrMap->second.clear();
	}
	m_mapTimers.clear();
}

CTimerMgr & CTimerMgr::Instance()
{
	static CTimerMgr s_TimeMgr;
	
	return s_TimeMgr;
}

void CTimerMgr::Initialize()
{

}

void CTimerMgr::Add(ITimer *pTimer)
{
	if (pTimer != NULL/* && pTimer->TriggerTime() >= time(NULL)*/) {
		MapTimer_t::iterator itr = m_mapTimers.find(pTimer->TriggerTime());
		if (itr != m_mapTimers.end()) {
			ContainerTimer_t *pVecTimer = &(itr->second);	
			if (pVecTimer != NULL) {
				pVecTimer->push_back(pTimer);
			}
		} else {
			ContainerTimer_t vecTimer;
			vecTimer.push_back(pTimer);
			m_mapTimers.insert(make_pair(pTimer->TriggerTime(), vecTimer));
		}
	}
}

void CTimerMgr::RemoveOne(int nType, timerid_t nID, bool bDelete)
{
	ITimer *pTimer = NULL;

	for (MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ++itrMap) {
		for ( ContainerTimer_t::iterator itrContainer = itrMap->second.begin(); itrContainer != itrMap->second.end(); ++itrMap) {
			if (*itrContainer != NULL) {
				if ( (*itrContainer)->TimerType() == nType && nID == (*itrContainer)->TimerID()) {
					pTimer = *itrContainer;
					itrMap->second.erase(itrContainer++);
					if (bDelete)
					{
						delete pTimer;
						pTimer = NULL;
					}
					return;
				} 
			}
		}
	}
}

void CTimerMgr::RemoveAll(int nType, timerid_t nID, bool bDelete)
{
	ITimer *pTimer = NULL;

	for (MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ++itrMap) {
		for ( ContainerTimer_t::iterator itrContainer = itrMap->second.begin(); itrContainer != itrMap->second.end(); ++itrMap) {
			if (*itrContainer != NULL) {
				if ( (*itrContainer)->TimerType() == nType && nID == (*itrContainer)->TimerID()) {
					pTimer = *itrContainer;
					itrMap->second.erase(itrContainer++);
					if (bDelete)
					{
						delete pTimer;
						pTimer = NULL;
					}
				} 
			}
		}
	}
}

void CTimerMgr::OnUpdate(const unsigned long &lTimeElapsed)
{
	time_t nTimeNow = time(NULL);
	for ( MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ) {
		if ( itrMap->first <= nTimeNow ) {
			for ( std::vector<ITimer *>::iterator itrVec = itrMap->second.begin(); itrVec != itrMap->second.end(); ++itrVec) {
				if (*itrVec != NULL) {
					(*itrVec)->OnEvent();
					delete *itrVec, *itrVec = NULL;
				} 
			}
			itrMap->second.clear();
			m_mapTimers.erase(itrMap++);
		} else {
			break;
		}
	}
}

ITimer* CTimerMgr::FindOne( int nType, timerid_t nID )
{
	ITimer *pTimer = NULL;

	for (MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ++itrMap) {
		for ( ContainerTimer_t::iterator itrContainer = itrMap->second.begin(); itrContainer != itrMap->second.end(); ++itrMap) {
			if (*itrContainer != NULL) {
				if ( (*itrContainer)->TimerType() == nType && nID == (*itrContainer)->TimerID()) {
					pTimer = *itrContainer;
					return pTimer;
				} 
			}
		}
	}
	return pTimer;
}

bool CTimerMgr::FindAll( int nType, timerid_t nID, ContainerTimer_t& timers )
{
	ITimer *pTimer = NULL;

	for (MapTimer_t::iterator itrMap = m_mapTimers.begin(); itrMap != m_mapTimers.end(); ++itrMap) {
		for ( ContainerTimer_t::iterator itrContainer = itrMap->second.begin(); itrContainer != itrMap->second.end(); ++itrMap) {
			if (*itrContainer != NULL) {
				if ( (*itrContainer)->TimerType() == nType && nID == (*itrContainer)->TimerID()) {
					pTimer = *itrContainer;
					timers.push_back(pTimer);
				} 
			}
		}
	}
	return (pTimer != NULL);
}

