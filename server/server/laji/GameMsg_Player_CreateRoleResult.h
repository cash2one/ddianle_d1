#ifndef __GameMsg_Player_CREATEROLERESULT_H__
#define __GameMsg_Player_CREATEROLERESULT_H__

#include "../socket/GameMsg_Base.h"
#include "../datastructure/datainfor/ItemType.h"
#include <string>
#include "../datastructure/Macro_Define.h"

using namespace std;
class CParamPool;

enum
{
	CREATE_ROLE_RESULT_SUCCESS,
	CREATE_ROLE_RESULT_DUPLICATED_NAME,		//名字重复
	CREATE_ROLE_RESULT_INVALID_NAME,		//包含不合法字符
	CREATE_ROLE_RESULT_INVALID_BIRTHDAY,	//生日不对
	CREATE_ROLE_RESULT_INVALID_NAME_LENGTH	//角色名称长度过长
};

class GameMsg_Player_CreateRoleResult :
	public GameMsg_Base
{
public:	
	unsigned char m_nResult;
	string m_strResultMsg;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_CreateRoleResult);
	GameMsg_Player_CreateRoleResult();
	~GameMsg_Player_CreateRoleResult();
public:
	virtual bool doEncode(CParamPool &IOBuff);
};

#endif
//file end

