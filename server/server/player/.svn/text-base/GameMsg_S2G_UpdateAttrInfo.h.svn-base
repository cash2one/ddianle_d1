#ifndef	__GAMEMSG_S2G_UPDATEATTRINFO_H__		
#define	__GAMEMSG_S2G_UPDATEATTRINFO_H__	

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_UpdateAttrInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateAttrInfo);
	GameMsg_S2G_UpdateAttrInfo();
	GameMsg_S2G_UpdateAttrInfo(CWorldRoleUpdateAttrInfo &info);
	~GameMsg_S2G_UpdateAttrInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	CWorldRoleUpdateAttrInfo m_RoleUpdateAttrInfo;
};

class GameMsg_S2G_UpdateRoleName:
public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateRoleName);
	GameMsg_S2G_UpdateRoleName(unsigned int nRoleID = 0, const char* pRoleNema = NULL);
	~GameMsg_S2G_UpdateRoleName();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	char m_szName[MAX_NAME_LEN];
};

class GameMsg_S2G_UpdateLevel:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateLevel);
	GameMsg_S2G_UpdateLevel(unsigned int nRoleID = 0, unsigned short nLevel = 0);
	~GameMsg_S2G_UpdateLevel();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned short m_nLevel;
};

class GameMsg_S2G_UpdateMoney:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateMoney);
	GameMsg_S2G_UpdateMoney(unsigned int nRoleID = 0, unsigned int nMoney = 0);
	~GameMsg_S2G_UpdateMoney();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	int m_nMoney;
};

class GameMsg_S2G_UpdateRoleLocation:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateRoleLocation);
	GameMsg_S2G_UpdateRoleLocation();
	~GameMsg_S2G_UpdateRoleLocation();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned short m_nRoleLocation;
};

class GameMsg_S2G_UpdateRoleAge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateRoleAge);
    GameMsg_S2G_UpdateRoleAge(unsigned int nRoleID = 0, short nAge = 0, short nConstellation = 0);
    ~GameMsg_S2G_UpdateRoleAge();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    short m_nAge;
    short m_nConstellation;
};

#endif

