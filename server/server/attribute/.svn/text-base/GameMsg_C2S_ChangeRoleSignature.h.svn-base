#ifndef __GAMEMSG_C2S_CHANGEROLESIGNATURE_H__
#define __GAMEMSG_C2S_CHANGEROLESIGNATURE_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

class CParamPool;

using namespace std;

class GameMsg_C2S_ChangeRoleSignature:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoleSignature);
	GameMsg_C2S_ChangeRoleSignature();
	~GameMsg_C2S_ChangeRoleSignature();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	char m_szNewSignature[MAX_SIGNATURE_LEN];
};

#endif
// end of file


