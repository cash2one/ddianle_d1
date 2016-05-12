#ifndef __GAMEMSG_S2C_CHANGEROLESIGNATURERESULT_H__
#define __GAMEMSG_S2C_CHANGEROLESIGNATURERESULT_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

class CParamPool;

using namespace std;

enum ChangeRoleSignatureResult
{
	CHANGEROLESIGNATURE_SUCCESS = 0,
	CHANGEROLESIGNATURE_HasInvalidWords,	// º¬ÓÐ·Ç·¨×Ö·û
};

class GameMsg_S2C_ChangeRoleSignatureResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoleSignatureResult(unsigned char nResult = CHANGEROLESIGNATURE_SUCCESS);
	~GameMsg_S2C_ChangeRoleSignatureResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nResult;
	char m_szNewSignature[MAX_SIGNATURE_LEN];
};

#endif
// end of file


