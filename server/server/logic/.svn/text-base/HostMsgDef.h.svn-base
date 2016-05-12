#ifndef __WORLDHOST_MSG_DEFINE_H__
#define __WORLDHOST_MSG_DEFINE_H__

#include "../../socket/inc/SocketLib.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"
#include "../../socket/GameMsg_Base.h"

#include <string>
using namespace std;

class HostMsg_Announce :public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(HostMsg_Announce);
	
	HostMsg_Announce(void):_baseClass(MSG_Host_Announce)
	{
		szContent="";
	}

	~HostMsg_Announce(void)
	{
	}

	bool doDecode( CParamPool &IoBuff )
	{
		szContent = IoBuff.GetString();
		return true;
	}

	bool doEncode( CParamPool &IoBuff )
	{
		IoBuff.AddString(szContent.c_str());
		return true;
	}

	string szContent;
};

typedef class HostMsg_KickRole: public GameMsg_Base
{
public:
	int nServerIndex;
	unsigned int nRoleID;
	HostMsg_KickRole()
        : GameMsg_Base(MSG_Host_KickRole)
	{
		nServerIndex = -1;
		nRoleID = 0;
		nSize = sizeof(HostMsg_KickRole);
		nMsg = MSG_Host_KickRole;
	}
}*LPHostMsg_KickRole;

typedef class HostMsg_Shutdown: public GameMsg_Base
{
public:
	HostMsg_Shutdown()
        : GameMsg_Base(MSG_Host_Shutdown)
	{
		nSize = sizeof(HostMsg_Shutdown);
		nMsg = MSG_Host_Shutdown;
	}
}*LPHostMsg_Shutdown;



#endif//__WORLDHOST_MSG_DEFINE_H__


//file end

