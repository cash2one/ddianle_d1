#ifndef __GAMEMSG_C2S_CHANGEROLENAME_H__
#define __GAMEMSG_C2S_CHANGEROLENAME_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

class CParamPool;

using namespace std;

class GameMsg_C2S_ChangeRoleName:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoleName);
	GameMsg_C2S_ChangeRoleName();
	~GameMsg_C2S_ChangeRoleName();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	char m_szNewName[MAX_NAME_LEN];
};

#endif
// end of file


