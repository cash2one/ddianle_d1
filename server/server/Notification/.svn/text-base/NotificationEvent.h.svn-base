#ifndef __NOTIFICATION_EVENT_H__
#define __NOTIFICATION_EVENT_H__

#include <queue>

enum eNotificationEvent
{
	NOTIFICATION_NONE,				//无事件
	NOTIFICATION_ONLINE_TIME,		//在线达到一定时长
	NOTIFICATION_LEVEL_UP,			//升级事件
	NOTIFICATION_FINISH_QUEST,		//完成任务事件
	NOTIFICATION_FINISH_SHOPING,	//完成商城购物
};

struct CNotificationEvent
{
	CNotificationEvent()
	{
		m_nEventType = NOTIFICATION_NONE;
	}
	int m_nEventType;
};

// 事件队列
typedef std::queue<CNotificationEvent> CNotificationEventQueue;

#endif //__NOTIFICATION_EVENT_H__

