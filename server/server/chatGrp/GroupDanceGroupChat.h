#ifndef	__GROUP_DANCE_GROUP_CHAT_H__
#define __GROUP_DANCE_GROUP_CHAT_H__
#include "IGroupChat.h"

class CGroupDanceGroupChat :
	public IGroupChat
{
public:
	CGroupDanceGroupChat();
	~CGroupDanceGroupChat();
public:
	void OnChat(GameMsg_Base *pMsg); //on net message
	void OnGroupChatMessage(GameMsg_Base *pMsg);
};

#endif


//file end

