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
	__int64 SessionID;//��AccountServer����
	unsigned int  nAccount;//�˺����ݿ��д洢��id
	unsigned short nSize;//�����ܳ���
	unsigned short nMsg;//��Ϸ��Ϣ��0x0000��0x00FF����
	unsigned short nIndex;//�ڷ������еĿ��ָ��,Gate Serve��Ҫ��¼
public:

	//���ش�������new���ñ䳤����Ϣ�ṹҲ����new������
	//�÷��� GAMEMSG_D *pD = new(nRealSize) GAMEMSG_D;

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

//������Ϸ��Ϣ�Ļ��࣬���е���Ϸ��Ϣ������Ӵ�����
#define GAMEMSG_CREATEDECLARE(x) public:\
	static GameMsg_Base* Creator_##x ( ){\
	return new x();\
}

// ��Ϣ�๹�졢������������
#define GAMEMSG_CLASSHEADER_DECLARE(__CLASS_NAME__) \
public:                                             \
    GAMEMSG_CREATEDECLARE(__CLASS_NAME__);          \
    __CLASS_NAME__();                               \
    ~__CLASS_NAME__();

#endif


//file end

