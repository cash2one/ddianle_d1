#ifndef	__GAMEMSG_G2S_LOCKACCT_H__
#define __GAMEMSG_G2S_LOCKACCT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>

class CParamPool;

class GameMsg_G2S_LockOrUnLockAccountByGm:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_LockOrUnLockAccountByGm);
	GameMsg_G2S_LockOrUnLockAccountByGm();
	~GameMsg_G2S_LockOrUnLockAccountByGm();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nPid;
	std::string m_strPUID;
	std::string m_strTransID;
	bool m_bLock;
	int m_nLockTimeType;
};

#endif
// end of file

