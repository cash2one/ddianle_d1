#include "GameMsg_G2S_LockOrUnLockAccountByGm.h"
#include "ServerMsgDef.h"

GameMsg_G2S_LockOrUnLockAccountByGm::GameMsg_G2S_LockOrUnLockAccountByGm():
GameMsg_Base(MSG_G2S_LockOrUnLockAccountByGm)
{
	m_nPid = 0;
	m_bLock = true;
	m_nLockTimeType = 0;
}

GameMsg_G2S_LockOrUnLockAccountByGm::~GameMsg_G2S_LockOrUnLockAccountByGm()
{
}

bool GameMsg_G2S_LockOrUnLockAccountByGm::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddString(m_strPUID.c_str());
	IOBuff.AddString(m_strTransID.c_str());
	IOBuff.AddBool(m_bLock);
	IOBuff.AddInt(m_nLockTimeType);
	return true;
}

bool GameMsg_G2S_LockOrUnLockAccountByGm::doDecode(CParamPool &IOBuff)
{
	m_nPid = IOBuff.GetUShort();
	m_strPUID = IOBuff.GetString();
	m_strTransID = IOBuff.GetString();
	m_bLock = IOBuff.GetBool();
	m_nLockTimeType = IOBuff.GetInt();
	return true;
}

