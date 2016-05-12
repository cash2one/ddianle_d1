#ifndef		__GM_TIMERMGR_H__
#define		__GM_TIMERMGR_H__

#include <map>
#include <vector>
#include "../../socket/GameMsg_Base.h"
#include "../../socket/GameMsg_Processor.h"
#include "GMDefine.h"

class ITimerGm;

typedef std::vector<ITimerGm *>	ContainerTimer_t;
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

	void Add(ITimerGm *pTimer);
	ITimerGm* Remove(ETimerType nType, unsigned int nID);

	void OnUpdate(const unsigned long &lTimeElapsed);
public:
	MapTimer_t	m_mapTimers;
};







#endif



