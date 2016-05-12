#ifndef __GameMsg_Player_CHECKROLENAMERESULT_H__
#define __GameMsg_Player_CHECKROLENAMERESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include <string>
#include "../../datastructure/Macro_Define.h"

using namespace std;
class CParamPool;

#define CheckRoleName_Successfully							0
#define CheckRoleName_Fail									1

#define StrCheckRoleName_OK									"CreateRole_Successfully"	
#define StrCheckRoleName_MySQLError							"CreateRole_MysqlError"
#define StrCheckRoleName_Duplicated							"CreateRole_Duplicated"
#define	StrCheckRoleName_NotValid							"CreateRole_NotValid"

class GameMsg_Player_CheckRoleNameResult :
	public GameMsg_Base
{
public:	
	unsigned char	m_nResult;			// 0表示成功,1表示失败
	string			m_strResult;		// 
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_CheckRoleNameResult);
	GameMsg_Player_CheckRoleNameResult();
	~GameMsg_Player_CheckRoleNameResult();
public:
	virtual bool doEncode(CParamPool &IOBuff);
};

#endif
//file end

