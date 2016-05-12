#include "GameMsg_S2C_RequireCreateRole.h"
#include "../../socket/ParamPool.h"
#include "../SystemSetting/SystemSettingMgr.h"


GameMsg_S2C_RequireCreateRole::GameMsg_S2C_RequireCreateRole():GameMsg_Base(MSG_S2C_RequireCreateRole)
{
}
GameMsg_S2C_RequireCreateRole::~GameMsg_S2C_RequireCreateRole()
{
}

bool GameMsg_S2C_RequireCreateRole::doEncode( CParamPool &IOBuff )
{
	CSystemSettingMgr::Instance().SerializeLoadingADList(IOBuff);
	return true;
}


//file end

