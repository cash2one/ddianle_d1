
#include "GameMsg_LinkInfo.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"

GameMsg_LinkInfo::GameMsg_LinkInfo(void):_baseClass(MSG_SERVER_LinkInfo)
{
}

GameMsg_LinkInfo::~GameMsg_LinkInfo(void)
{
}

bool GameMsg_LinkInfo::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUInt(info.addr);
	IoBuff.AddUShort(info.nPort);
	IoBuff.AddUShort(info.nClient);
	IoBuff.AddUShort(info.nGroup);
	IoBuff.AddInt(info.nLine);
	IoBuff.AddString(info.strName.c_str());
	IoBuff.AddUChar(info.nInstallerID);
	IoBuff.AddBool(info.bEnableAudition);

	return true;
}
bool GameMsg_LinkInfo::doDecode( CParamPool& IoBuff )
{
	info.addr = IoBuff.GetUInt();
	info.nPort = IoBuff.GetUShort();
	info.nClient = IoBuff.GetUShort();
	info.nGroup = IoBuff.GetUShort();
	info.nLine = IoBuff.GetInt();
	info.strName = IoBuff.GetString();
	info.nInstallerID = IoBuff.GetUChar();
	info.bEnableAudition = IoBuff.GetBool();

	return true;
}

//end

