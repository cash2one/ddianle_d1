/********************************************************************
	created:	2010-11-09
	filename: 	server\chat\Chat.h
	author:		mei jiuhua
	
	purpose:	GameServer�ϵ�����ģ��
*********************************************************************/
#ifndef __chat_H__
#define __chat_H__

//#include "../group/grpchat/wchatmsgdefine.h"
class GameMsg_Base;
class CChatManager
{
public:
	CChatManager();
	~CChatManager();
public:
	//��ӦGroupServer��Ϣ
	//void OnGroupMessage(GameMsg_Base* pMsg);

	//����������Ϣ
//	void ProcessChat(GameMsg_CHAT* pMsg,unsigned short nIndexDestRole);
	//����������Ϣ����
//	void ProcessChatResult(int nResult, GameMsg_CHAT* pMsg,unsigned short nIndexDestRole);

};



extern CChatManager g_ChatManager;

#endif

