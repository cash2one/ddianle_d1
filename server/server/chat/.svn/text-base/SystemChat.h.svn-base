/*
**	系统聊天频道,接收无具体的玩家对象
**/
#ifndef __SYSCHAT_H__
#define __SYSCHAT_H__
#include "IChat.h"

class CSystemChat 
	:public IChat
{
public:
	CSystemChat(CEntityChatComponent *pRoleChat = NULL);
	virtual ~CSystemChat();
public:
	//nSenderRoleID为0表示为接收系统系统信息的玩家roleID
	virtual bool OnChat(GameMsg_Base *pMsg);
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg);

	void SysChat(const char* buffer);
public:
};


#endif


//end file


