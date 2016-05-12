#ifndef __ADMINCHAT_H__
#define __ADMINCHAT_H__
#include "IChat.h"

class CEntityChatComponent;

class CAdminChat
	:public IChat
{
public:
	CAdminChat(CEntityChatComponent *pRoleChat = NULL);
	~CAdminChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);
	void OnGroupChatMessage(GameMsg_Base *pMsg);

	static void AdminChat(const char* buffer);
};

#endif


//end file


