#ifndef	__GROUPPRIVATECHAT_H__
#define __GROUPPRIVATECHAT_H__
#include "IGroupChat.h"

class CGroupPrivateChat :
	public IGroupChat
{
public:
	CGroupPrivateChat();
	~CGroupPrivateChat();
public:
	void OnChat(GameMsg_Base *pMsg); //on net message
	void OnGroupChatMessage(GameMsg_Base *pMsg);
};

#endif


//file end

