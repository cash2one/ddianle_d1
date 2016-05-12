#ifndef	__GROUPWORLDCHAT_H__
#define __GROUPWORLDCHAT_H__
#include "IGroupChat.h"

class CGroupWorldChat :
	public IGroupChat
{
public:
	CGroupWorldChat();
	~CGroupWorldChat();
public:
	void OnChat(GameMsg_Base *pMsg); //on net message
	void OnGroupChatMessage(GameMsg_Base *pMsg);
};

#endif

//file end

