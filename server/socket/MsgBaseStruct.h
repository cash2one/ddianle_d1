//	File		:	MsgBaseStruct.h
//	Desc		:	消息机制的基本数据结构
//  Date		:	[2010-11-04]
//	Author		:	mei jiuhua

#ifndef __MSGBASESTRUCT_H__
#define __MSGBASESTRUCT_H__
#include "Windefine.h"

//网络数据包格式
#ifndef uint16
#define uint16 unsigned short
#endif 

typedef const uint16 CUSHORT;
#define MSG_TYPE_SYSTEM		 0x0000//保留的消息类型


//#ifndef MAX_PACKET_SIZE
////#define MAX_PACKET_SIZE			32768	//包的最大长度
//#define MAX_PACKET_SIZE			65500	//包的最大长度
//#endif

//const unsigned short MSG_TYPE_SYSTEM         = 0x0000;//保留的消息类型
const unsigned short MSG_SYSTEM_Nothing		 = MSG_TYPE_SYSTEM;
const unsigned short MSG_SYSTEM_Connect      = MSG_TYPE_SYSTEM+1;//连接成功
const unsigned short MSG_SYSTEM_Disconnect   = MSG_TYPE_SYSTEM+2;//断开连接
const unsigned short MSG_SYSTEM_Ping         = MSG_TYPE_SYSTEM+3;//ping值封包
const unsigned short MSG_SYSTEM_Packet       = MSG_TYPE_SYSTEM+4;//包含另一个消息的封包
const unsigned short MSG_SYSTEM_Slot		 = MSG_TYPE_SYSTEM+5;//双方的握手信息
const unsigned short MSG_SYSTEM_Invalid		 = MSG_TYPE_SYSTEM+6;//非法的数据
const unsigned short MSG_SYSTEM_Test		 = MSG_TYPE_SYSTEM+7;//测试用的消息


//所有游戏消息的基类，所有的游戏消息都必须从此派生
//typedef class GameMsg_Base
//{
//
//public:
//	__int64 SessionID;//SessionID 由GateServer生成.
//	__int64 uuid;//uuid是平台相关数据。
////	unsigned short nSize;//数据总长度
//	unsigned short m_nMsg;//游戏消息，0x0000到0x00FF保留
//	unsigned short nIndex;//角色在palyermanager中的数组下标
//public:
//	GameMsg_Base()
//	{
//		nMsg = 0;
//		uuid = 0;
//		nIndex = 0;
//		nSize = 0;
//		nSize = sizeof(GameMsg_Base);
//	}
//
//	//重载带参数的new，让变长的消息结构也能用new来产生
//	//用法： GAMEMSG_D *pD = new(nRealSize) GAMEMSG_D;
//
//	bool encodeMssage( CParamPool IoBuff );
//	bool decodeMssage( CParamPool IoBuff ,unsigned short nSize);
//
//	virtual bool doEncode( CParamPool IoBuff );
//	virtual bool doDecode( CParamPool IoBuff );
//
//	unsigned int CheckSum()
//	{
//		unsigned int nCheckSum = 0;
//		unsigned char* ptr = (unsigned char*)this;
//		for(unsigned short i = 0; i < nSize; i++)
//		{
//			nCheckSum += *ptr++;
//		}
//		return nCheckSum;
//	};
//}

//typedef struct GameMsg_System_Ping:public GameMsg_Base 
//{
//	unsigned int	nTickCount;
//	unsigned int	nDirection;//0:S->C->S, 1:C->S->C
//	GameMsg_System_Ping()
//	{
//		nSize = sizeof(GameMsg_System_Ping);
//		nMsg = MSG_SYSTEM_Ping;
//	}
//}*LPGameMsg_System_Ping;
//
//typedef struct GameMsg_System_Connect:public GameMsg_Base
//{
//	int nConnCode;//0 - 连接成功， -1 表示连接失败
//	GameMsg_System_Connect()
//	{
//		nMsg = MSG_SYSTEM_Connect;
//		nSize = sizeof(GameMsg_System_Connect);
//		nConnCode = 0;
//	}
//}*LPGameMsg_System_Connect;
//
//typedef struct GameMsg_System_Disconnect:public GameMsg_Base 
//{
//	GameMsg_System_Disconnect()
//	{
//		nMsg = MSG_SYSTEM_Disconnect;
//		nSize = sizeof(GameMsg_System_Connect);
//	}
//}*LPGameMsg_System_Disconnect;
//
//
////包含一个GAMEMSG的msg
//typedef struct GameMsg_System_Packet : public GameMsg_Base
//{
//	unsigned int nCheckSum;
//	GameMsg_Base msg;
//
//	GameMsg_System_Packet()
//	{
//		nMsg = MSG_SYSTEM_Packet;
//		nSize = sizeof(GameMsg_System_Packet);
//	}
//}*LPGameMsg_System_Packet;
//
//typedef struct GameMsg_System_Slot:public GameMsg_Base
//{
//	unsigned short nSlot;
//	unsigned short nRandom;
//	char	key[16];
//	GameMsg_System_Slot()
//	{
//		nMsg = MSG_SYSTEM_Slot;
//		nSize = sizeof(GameMsg_System_Slot);
//	};
//}*LPGameMsg_System_Slot;
//
//typedef struct GameMsg_System_Invalid:public GameMsg_Base
//{
//	unsigned int nParam;//
//	GameMsg_System_Invalid()
//	{
//		nSize = sizeof(GameMsg_System_Invalid);
//		nMsg = MSG_SYSTEM_Invalid;
//	}
//}*LPGameMsg_System_Invalid;
//
//typedef struct GameMsg_System_Test:public GameMsg_Base
//{
//	char szBuff[40];
//	GameMsg_System_Test()
//	{
//		nSize = sizeof(GameMsg_System_Test);
//		nMsg = MSG_SYSTEM_Test;
//	}
//}*LPGameMsg_System_Test;
//

#endif//__MSGBASESTRUCT_H__

//file end

