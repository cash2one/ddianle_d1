#ifndef	__GROUPADMIN_H__
#define __GROUPADMIN_H__
#include "IGroupChat.h"

class CGroupAdminChat :
	public IGroupChat
{
public:
	CGroupAdminChat();
	~CGroupAdminChat();
public:
	void OnChat(GameMsg_Base *pMsg); //on net message
	void OnGroupChatMessage(GameMsg_Base *pMsg);
};

#endif

//file end

