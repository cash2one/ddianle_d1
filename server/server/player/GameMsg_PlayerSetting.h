#ifndef		__GAMEMSG_PLAYERSETTING_H__
#define		__GAMEMSG_PLAYERSETTING_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"


class CParamPool;

class GameMSG_S2C_SendPlayerSetting:
	public GameMsg_Base
{
public:
	GameMSG_S2C_SendPlayerSetting();
	~GameMSG_S2C_SendPlayerSetting();
public:
	virtual bool doEncode(CParamPool &IOBuff);	
public:
	CPlayerSetting m_playerSetting;
};

class GameMsg_C2S_OptionSetting : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_OptionSetting);
	GameMsg_C2S_OptionSetting();
	~GameMsg_C2S_OptionSetting();
public:
	virtual bool doDecode(CParamPool &IOBuff);	
public:
	unsigned char m_nOption;
	int m_nState;
};

class GameMsg_S2C_OptionSettingSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_OptionSettingSuc();
	~GameMsg_S2C_OptionSettingSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);	
public:
	unsigned char m_nOption;
	int m_nState;
};

class GameMsg_S2C_OptionSettingFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_OptionSettingFail();
	~GameMsg_S2C_OptionSettingFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nOption;
	std::string m_strError;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_PlayerMoveTo : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PlayerMoveTo);
	GameMsg_C2S_PlayerMoveTo();
	~GameMsg_C2S_PlayerMoveTo();
public:
	virtual bool doDecode(CParamPool &IOBuff);	
public:
	unsigned short m_nSceneID;
	float m_fOrient;
	float m_fDirectionX;
	float m_fDirectionZ;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_PlayerMoveTo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_PlayerMoveTo();
	~GameMsg_S2C_PlayerMoveTo();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nSceneID;
	unsigned int m_nRoleID;
	float m_fDirectionX;
	float m_fDirectionZ;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_PlayerMoveType : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PlayerMoveType);
	GameMsg_C2S_PlayerMoveType();
	~GameMsg_C2S_PlayerMoveType();
public:
	virtual bool doDecode(CParamPool &IOBuff);	
public:
	unsigned short m_nSceneID;
	unsigned char m_nMoveType;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_PlayerMoveType:
	public GameMsg_Base
{
public:
	GameMsg_S2C_PlayerMoveType();
	~GameMsg_S2C_PlayerMoveType();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nSceneID;
	unsigned int m_nRoleID;
	unsigned char m_nMoveType;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_PlayerMotion : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PlayerMotion);
	GameMsg_C2S_PlayerMotion();
	~GameMsg_C2S_PlayerMotion();

public:
	virtual bool doDecode(CParamPool &IOBuff);	

public:
	unsigned short m_nTransFormID;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_PlayerMotion:
	public GameMsg_Base
{
public:
	GameMsg_S2C_PlayerMotion();
	~GameMsg_S2C_PlayerMotion();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nRoleID;
	unsigned short m_nTransFormID;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_PlayerDownloading : 
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_PlayerDownloading);
    GameMsg_C2S_PlayerDownloading();
    ~GameMsg_C2S_PlayerDownloading();

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned char m_nProgress;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_PlayerDownloading:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_PlayerDownloading);
    GameMsg_S2C_PlayerDownloading();
    ~GameMsg_S2C_PlayerDownloading();

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    unsigned char m_nProgress;
};

#endif


