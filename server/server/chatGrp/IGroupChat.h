#ifndef __GROUPCHAT_H__
#define __GROUPCHAT_H__

#include "../../socket/GameMsg_Base.h"


class GAMEMSG_BASE;
class IGroupChat
{
public:
	IGroupChat()
	{
	}
	virtual ~IGroupChat(){};
	virtual void OnChat(GameMsg_Base *pMsg) = 0; //on net message
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg) = 0; // on group message
};

#endif


//end file


