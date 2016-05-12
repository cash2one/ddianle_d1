#ifndef __DANCE_GROUP_CHAT_H__
#define __DANCE_GROUP_CHAT_H__
#include "IChat.h"

class CEntityChatComponent;

class CDanceGroupChat
	:public IChat
{
public:
	CDanceGroupChat(CEntityChatComponent *pRoleChat = NULL);
	~CDanceGroupChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);
	void OnGroupChatMessage(GameMsg_Base *pMsg);
};

#endif //__DANCE_GROUP_CHAT_H__


//end file


