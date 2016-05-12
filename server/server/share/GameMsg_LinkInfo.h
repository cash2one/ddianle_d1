#ifndef __GameMsg_LINKINFO_H__
#define __GameMsg_LINKINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

#define INVALID_GROUP_LINE	0x7F
typedef struct LINKSERVERINFO
{
	unsigned int   addr;   //linkserver的地址
	unsigned short nPort;  //linkserver的端口
	unsigned short nClient;//linkserver当前连接的客户数量
	unsigned short nGroup; //linkserver的组
	int nLine;  //Linkserver的线
	string strName;
	unsigned char nInstallerID;	//是否是新手线
	bool bEnableAudition;  //是否是直播房间比赛海选线

	LINKSERVERINFO()
	{
		addr = 0;
		nPort = 0;
		nClient = 0;
		nGroup = INVALID_GROUP_LINE;
		nLine = (int)INVALID_GROUP_LINE;
		nInstallerID = INSTALLER_INVALID;
		bEnableAudition = false;
	}
	void Clear()
	{
		addr = 0;
		nPort = 0;
		nClient = 0;
		strName = "";
		nGroup = INVALID_GROUP_LINE;
		nLine = (int)INVALID_GROUP_LINE;
		nInstallerID = INSTALLER_INVALID;
		bEnableAudition = false;
	}
}*LPLINKSERVERINFO;

class GameMsg_LinkInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_LinkInfo);
	GameMsg_LinkInfo(void);
	~GameMsg_LinkInfo(void);

	LINKSERVERINFO info;
public:
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};

#endif


//file end

