#ifndef __GameMsg_Server_CONNECTED_H__
#define __GameMsg_Server_CONNECTED_H__
//連接服务器成功//
#include "../../socket/GameMsg_Base.h"
#include <string>
using namespace std;
class GameMsg_Server_Connected :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Server_Connected);
	GameMsg_Server_Connected(void);
	~GameMsg_Server_Connected(void);

	unsigned short nCurPlayerCount;
	unsigned char nSvrType;
	int nLine;
	unsigned char nInstallerId;
	string strName;


public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};

#endif

//file end

