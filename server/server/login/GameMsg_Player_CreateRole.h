#ifndef __GameMsg_Player_CREATEROLE_H__
#define __GameMsg_Player_CREATEROLE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include <string>
#include "../../datastructure/Macro_Define.h"

using namespace std;
class CParamPool;

class GameMsg_Player_CreateRole :
	public GameMsg_Base
{
public:	
	unsigned char nInstallerId;
	unsigned char m_nVersionType;
	char m_szRoleName[MAX_NAME_LEN];
	unsigned char m_nSex;
	char m_szSignature[MAX_SIGNATURE_LEN];
	//unsigned char	Constellation;
	char m_szYear[MAX_YEAR_LEN];				//1990
	char m_szMonth[MAX_MONTH_LEN];		//02
	char m_szDay[MAX_DAY_LEN];					//06
	unsigned char m_nColor;
	
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_CreateRole);
	GameMsg_Player_CreateRole();
	~GameMsg_Player_CreateRole();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:

};

#endif

//file end

