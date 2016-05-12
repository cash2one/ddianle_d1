#ifndef __GAMEMSG_S2A_LOCKACCT_H__
#define __GAMEMSG_S2A_LOCKACCT_H__

#include "../../socket/GameMsg_Base.h"
#include <string>
using namespace std;

class GameMsg_S2A_LockOrUnLockAccountByGm :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2A_LockOrUnLockAccountByGm);
	GameMsg_S2A_LockOrUnLockAccountByGm(void);
	~GameMsg_S2A_LockOrUnLockAccountByGm(void);

	unsigned short nPid;
	std::string strPUID;
	std::string strTransID;
	bool m_bLock;
	int m_nLockTimeType;
public:
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool& IoBuff);
};

#endif

//file end

