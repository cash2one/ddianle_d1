#ifndef __GAMEMSG_ROLE_CREATEROLE_RES_H__
#define __GAMEMSG_ROLE_CREATEROLE_RES_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

#define		CreateRoleFail_InvalidBirthday		"CreateRoleFail_InvalidBirthday"
#define		CreateRoleFail_InvalidRoleName		"CreateRoleFail_InvalidRoleName"
#define		CreateRoleFail_DuplicatedName		"CreateRoleFail_DuplicatedName"
#define		CreateRoleFail_DuplicatedRole       "CreateRoleFail_DuplicatedRole"
#define		CreateRoleFail_InvalidInstaller     "CreateRoleFail_InvalidInstaller"

enum ECreateRoleError
{
	ECreateRoleError_OK = 0,
	ECreateRoleError_Duplicated_Role = 1,
	ECreateRoleError_Duplidated_Name = 2,
	ECreateRoleFail_InvalidInstaller = 3,
};

class CEntity;


class GameMsg_S2C_CreateRoleSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_CreateRoleSuc);

	GameMsg_S2C_CreateRoleSuc();
	~GameMsg_S2C_CreateRoleSuc();

public:
	void AttachEntity(CEntity* pEntity );

	virtual bool doEncode(CParamPool &IOBuff);

public:	
	CEntity* m_pEntity ;
};


class GameMsg_Player_CreateRoleFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Player_CreateRoleFail);

	GameMsg_Player_CreateRoleFail();
	~GameMsg_Player_CreateRoleFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:	
	std::string m_strResultMsg;
};


#endif

//end file


