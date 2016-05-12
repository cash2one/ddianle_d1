#ifndef __GAMEMSG_BASE_H__
#define __GAMEMSG_BASE_H__
#include "ParamPool.h"


typedef  class GameMsg_Base
{
public:
	GameMsg_Base( unsigned short nMsgType );
	GameMsg_Base( const GameMsg_Base &refMsg );
	virtual ~GameMsg_Base();
	//GAMEMSG_CREATEDECLRAE(GameMsg_Base);
public:
	__int64 SessionID;//由AccountServer生成
	unsigned int  nAccount;//账号数据库中存储的id
	unsigned short nSize;//数据总长度
	unsigned short nMsg;//游戏消息，0x0000到0x00FF保留
	unsigned short nIndex;//在服务器中的快捷指针,Gate Serve需要记录
public:

	//重载带参数的new，让变长的消息结构也能用new来产生
	//用法： GAMEMSG_D *pD = new(nRealSize) GAMEMSG_D;

	bool encodeMssage(CParamPool &IoBuff);
	bool decodeMssage(CParamPool &IoBuff, unsigned short nSize);

	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);

	unsigned int CheckSum(){ return 0; };
	//{
	//	unsigned int nCheckSum = 0;
	//	unsigned char* ptr = (unsigned char*)this;
	//	for(unsigned short i = 0; i < nSize; i++)
	//	{
	//		nCheckSum += *ptr++;
	//	}
	//	return nCheckSum;
	//};
}* LPGameMsg_Base;

//所有游戏消息的基类，所有的游戏消息都必须从此派生
#define GAMEMSG_CREATEDECLARE(x) public:\
	static GameMsg_Base* Creator_##x ( ){\
	return new x();\
}

// 消息类构造、析构函数申明
#define GAMEMSG_CLASSHEADER_DECLARE(__CLASS_NAME__) \
public:                                             \
    GAMEMSG_CREATEDECLARE(__CLASS_NAME__);          \
    __CLASS_NAME__();                               \
    ~__CLASS_NAME__();

#endif


//file end

