/*
**	世界聊天频道
**/
#ifndef __WORLDCHAT_H__
#define __WORLDCHAT_H__
#include "IChat.h"

class CWorldChat 
	:public IChat
{
public:
	CWorldChat(CEntityChatComponent *pRoleChat = NULL);
	~CWorldChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);//接收来自客户端的请求
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg);//接收来自租服务器的请求，无实体
};

#endif


//end file


