#include "GameMsg_S2A_LockOrUnLockAccountByGm.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2A_LockOrUnLockAccountByGm::GameMsg_S2A_LockOrUnLockAccountByGm(void)
:_baseClass(MSG_S2A_LockOrUnLockAccountByGm)
{
	nPid = 0;
	m_bLock = true;
	m_nLockTimeType = 0;
}

GameMsg_S2A_LockOrUnLockAccountByGm::~GameMsg_S2A_LockOrUnLockAccountByGm(void)
{
}

bool GameMsg_S2A_LockOrUnLockAccountByGm::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort(nPid);
	IOBuff.AddString(strPUID.c_str());
	IOBuff.AddString(strTransID.c_str());
	IOBuff.AddBool(m_bLock);
	IOBuff.AddInt(m_nLockTimeType);
	return true;
}

bool GameMsg_S2A_LockOrUnLockAccountByGm::doDecode( CParamPool& IoBuff )
{
	nPid = IoBuff.GetUShort();
	strPUID = IoBuff.GetString();
	strTransID = IoBuff.GetString();
	m_bLock = IoBuff.GetBool();
	m_nLockTimeType = IoBuff.GetInt();
	return true;
}

