#ifndef		__LIBSERVERFRAME_TIMERMGR_H__
#define		__LIBSERVERFRAME_TIMERMGR_H__

#include <map>
#include <vector>
#include "ITimer.h"

typedef std::vector<ITimer *>	ContainerTimer_t;
typedef std::map< time_t, ContainerTimer_t > MapTimer_t;

class CTimerMgr
{
public:
	CTimerMgr();	
	~CTimerMgr();
public:
	static CTimerMgr & Instance();
public:
	void Initialize();

	void Add(ITimer *pTimer);
	void RemoveOne(int nType, timerid_t nID, bool bDelete);
	void RemoveAll(int nType, timerid_t nID, bool bDelete);
	ITimer* FindOne(int nType, timerid_t nID);
	bool FindAll(int nType, timerid_t nID, ContainerTimer_t& timers);

	void OnUpdate(const unsigned long &lTimeElapsed);
public:
	MapTimer_t	m_mapTimers;
};







#endif



