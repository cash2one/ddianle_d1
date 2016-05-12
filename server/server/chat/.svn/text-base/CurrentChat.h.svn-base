/*
** 当前频道，包括在房间大厅、房间内,在该区域内的玩家都可见
**
**/
#ifndef __CURRENT_CHAT_H__
#define __CURRENT_CHAT_H__
#include "IChat.h"

class CRoomComponent;

class CCurrentChat:
	public IChat
{
public:
	CCurrentChat(CEntityChatComponent *pRoleChat = NULL);	
	~CCurrentChat();
public:
	virtual bool OnChat(GameMsg_Base *pMsg);
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg);
	virtual bool CanTalk();
protected:
	void SendRoomPlayerMessage(CRoomComponent* pPlayer, GameMsg_Base *msg);
};


#endif



//end file


