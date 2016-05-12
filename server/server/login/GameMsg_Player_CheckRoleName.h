#ifndef __GameMsg_Player_CHECK_ROLE_NAME_H__
#define __GameMsg_Player_CHECK_ROLE_NAME_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include <string>
#include "../../datastructure/Macro_Define.h"

using namespace std;
class CParamPool;

class GameMsg_Player_CheckRoleName :
	public GameMsg_Base
{
public:	
	char m_szRoleName[MAX_NAME_LEN];
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_CheckRoleName);
	GameMsg_Player_CheckRoleName();
	GameMsg_Player_CheckRoleName(const  GameMsg_Player_CheckRoleName &rhs);
	~GameMsg_Player_CheckRoleName();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};

#endif
//file end

