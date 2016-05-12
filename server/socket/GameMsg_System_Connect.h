#ifndef __GameMsg_System_Connect_H__
#define __GameMsg_System_Connect_H__
#include "GameMsg_Base.h"

typedef class GameMsg_System_Connect :
	public GameMsg_Base
{
private:
	typedef GameMsg_Base _baseClass;
public:
	GameMsg_System_Connect(void);
	~GameMsg_System_Connect(void);
	GAMEMSG_CREATEDECLARE(GameMsg_System_Connect);

public:
	int nConnCode;//0 连接成功,-1 表示连接失败
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
}* LPGameMsg_System_Connect;

#endif

//file end

