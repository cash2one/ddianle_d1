/*
**	本服聊天频道
**/
#ifndef __GLOBALCHAT_H__
#define __GLOBALCHAT_H__

#include "IChat.h"

class GameMsg_Base;

class CGlobalChat 
	:public IChat
{
public:
	CGlobalChat(CEntityChatComponent *pRoleChat = NULL);
	~CGlobalChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg);
private:
};

#endif


//end file


