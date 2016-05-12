#pragma once
#include "../socket/gamemsg_base.h"
#include <string>

using namespace std;
class CParamPool;

typedef class GAMEMSG_CHAT :
	public GAMEMSG_BASE
{
	typedef  GAMEMSG_BASE _baseClass;
public:

	GAMEMSG_CREATEDECLRAE(GAMEMSG_CHAT);

	~GAMEMSG_CHAT(void);
	unsigned int	nEntityID;//发送者id
	unsigned int	nEntityIDReceiver;//私聊对象roleID ,，私聊时这个一定要填
	unsigned short	nChannel;//频道
	unsigned short dwColor[CHATCOLORTYPE];
	string      strSenderName;//发送者名称
	string		strReceiverName;//接收者名称，只有指定echatPrivate时有效
	string     strContext;//聊天的文字

	static GAMEMSG_CHAT* Create(const char* szSenderName,unsigned int nEntityID, eChatChannel eChannel
				,const char* szContext , const char* szReceiverName = NULL, unsigned int nEntityIDReceiver = 0
				, unsigned short dwColor1 =0xffff,unsigned short dwColor2 =0x0000 );

public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );

protected:
	GAMEMSG_CHAT(void);

}*LPGAMEMSG_CHAT;

//typedef	struct GAMEMSG_CHAT: public GAMEMSG_BASE
//{
//	//unsigned long	dwColor;//颜色
//	unsigned int	nEntityID;//发送者id
//	unsigned int	nEntityIDReceiver;//私聊对象roleID ,，私聊时这个一定要填
//	unsigned short	nChannel;//频道
//	unsigned short dwColor[CHATCOLORTYPE];
//	char            szSenderName[MAX_NAME_LEN];//发送者名称
//	char			szReceiverName[MAX_NAME_LEN];//接收者名称，只有指定echatPrivate时有效
//	char            szContext[1];//聊天的文字
//
//	static GAMEMSG_CHAT* Create(const char* szSenderName,unsigned int nEntityID, eChatChannel eChannel
//		,const char* szContext , const char* szReceiverName = NULL, unsigned int nEntityIDReceiver = 0
//		, unsigned short dwColor1 =0xffff,unsigned short dwColor2 =0x0000 )
//	{
//		unsigned short nLen = short(strlen(szContext));
//		unsigned short nSize = sizeof(GAMEMSG_CHAT)+nLen;
//		GAMEMSG_CHAT* pMsg = new(nSize)GAMEMSG_CHAT;
//		pMsg->nSize = nSize;
//		pMsg->nMsg = MSG_CHAT_CHAT;
//
//		int nStrLen;
//		if(szSenderName)
//		{
//			nStrLen = strlen(szSenderName);
//			if(nStrLen > MAX_NAME_LEN - 1)
//				nStrLen = MAX_NAME_LEN - 1;
//			if(szSenderName) memcpy(pMsg->szSenderName, szSenderName, nStrLen);
//			pMsg->szSenderName[nStrLen] = '\0';
//		}
//		else
//			pMsg->szSenderName[0] = '\0';
//
//		if(szReceiverName)
//		{
//			nStrLen = strlen(szReceiverName);
//			if(nStrLen > MAX_NAME_LEN - 1)
//				nStrLen = MAX_NAME_LEN - 1;
//			if(szReceiverName) memcpy(pMsg->szReceiverName, szReceiverName, nStrLen);
//			pMsg->szReceiverName[nStrLen] = '\0';
//		}
//		else
//			pMsg->szReceiverName[0] = '\0';
//
//		pMsg->nEntityID = nEntityID;
//		pMsg->nEntityIDReceiver = nEntityIDReceiver;
//		pMsg->nChannel = short(eChannel);
//		strcpy(pMsg->szContext, szContext);
//		pMsg->dwColor[0] = dwColor1;
//		pMsg->dwColor[1] = dwColor2;
//		return pMsg;
//	}
//}*LPGAMEMSG_CHAT;

