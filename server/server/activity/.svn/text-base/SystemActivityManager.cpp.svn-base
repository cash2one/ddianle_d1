#include "SystemActivityManager.h"
#include "InTimeOnlineActivity.h"
#include <time.h>
#include "../util/CommonFunc.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../socket/Log.h"
#include "../../datastructure/RoleEntity.h"
#include "../EntityNetComponent/EntityNetComponent.h"

CSystemActivityManager::CSystemActivityManager()
{
}

CSystemActivityManager::~CSystemActivityManager()
{
	std::list< ITimerActivity * >::iterator it = m_TimerActivityList.begin();
	for (; it != m_TimerActivityList.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	m_TimerActivityList.clear();
}

CSystemActivityManager & CSystemActivityManager::Instance()
{
	static CSystemActivityManager s_Instance;
	return s_Instance;
}

//添加时间活动
void CSystemActivityManager::AddTimerActivity(ITimerActivity * pActivity)
{
	m_TimerActivityList.push_back(pActivity);
}
void CSystemActivityManager::OnUpdate(const unsigned long &lTimeElapsed)
{
	time_t tNow = time(NULL);

	std::list< ITimerActivity * >::iterator it = m_TimerActivityList.begin();
	for (; it != m_TimerActivityList.end();)
	{
		ITimerActivity * pTimerActivity = *it;
		pTimerActivity->OnUpdate(lTimeElapsed, tNow);
		if (pTimerActivity->IsValid())
		{
			++it;
		}
		else
		{
			m_TimerActivityList.erase(it++);
		}
	}
}
//角色登陆时，需要将一些活动的状态同步到客户端
void CSystemActivityManager::OnRoleLogin(CRoleEntity & rRole)
{
	CEntityNetComponent * pNet = rRole.GetComponent< CRoleNet >();
	if (pNet == NULL)
	{
		return;
	}

	std::list< ITimerActivity * >::iterator it = m_TimerActivityList.begin();
	for (; it != m_TimerActivityList.end(); ++it)
	{
		ITimerActivity * pTimerActivity = *it;
		pTimerActivity->UpdateStateToClient(*pNet);
	}
}

