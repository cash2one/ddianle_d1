/*
**	ÀΩ»À¡ƒÃÏ∆µµ¿
**/
#ifndef __PRIVATECHAT_H__
#define __PRIVATECHAT_H__
#include "GameMsg_G2S_Chat.h"
#include "IChat.h"

class CPrivateChat
	:public IChat
{
public:
	CPrivateChat(CEntityChatComponent *pRoleChat = NULL);
	~CPrivateChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);
	void OnGroupChatMessage(GameMsg_Base *pMsg);
private:
	void ProcessForwardMsg(GameMsg_G2S_Chat *pChatMsg);
	void ProcessBackWardMsg(GameMsg_G2S_Chat *pChatMsg);
};

#endif


//end file


