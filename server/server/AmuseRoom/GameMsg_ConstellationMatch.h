#ifndef __GAMEMSG_CONSTELLATION_H__
#define __GAMEMSG_CONSTELLATION_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/datainfor/ConstellationConfigManager.h"

#include <list>

class CParamPool;


///////////////////////////////////////////////////////
class GameMsg_S2C_GetConstellationMatchInfoRes : public GameMsg_Base
{
public:
	GameMsg_S2C_GetConstellationMatchInfoRes(void);
	~GameMsg_S2C_GetConstellationMatchInfoRes(void);

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned short m_nMinRoleLevel;
	bool m_bIsOpen;
	int m_nLeftOpenTime;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_EnterConstellationLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterConstellationLobby);

	GameMsg_C2S_EnterConstellationLobby(void);
	~GameMsg_C2S_EnterConstellationLobby(void);

	virtual bool doDecode(CParamPool &IOBuff);

public:
};

class GameMsg_S2C_EnterConstellationLobbySuc : public GameMsg_Base
{
public:
	GameMsg_S2C_EnterConstellationLobbySuc(void);
	~GameMsg_S2C_EnterConstellationLobbySuc(void);

	virtual bool doEncode(CParamPool &IOBuff);

public:
	bool m_bFirstEnter;
	std::string m_strFirstEnterTip;
	unsigned short m_nMaxUseCount;
	unsigned short m_nUseCount;
	std::list<SConstellationCardInfo> m_listCard;
	std::string m_strRule;
};

class GameMsg_S2C_EnterConstellationLobbyFail : public GameMsg_Base
{
public:
	GameMsg_S2C_EnterConstellationLobbyFail(void);
	~GameMsg_S2C_EnterConstellationLobbyFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
	unsigned short m_nUseCount;
	int m_nWillOpenTime;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_QuitConstellationLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitConstellationLobby);

	GameMsg_C2S_QuitConstellationLobby(void);
	~GameMsg_C2S_QuitConstellationLobby(void);

	virtual bool doDecode(CParamPool &IOBuff);

public:
};

///////////////////////////////////////////////////////
class GameMsg_C2S_ConstellationMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ConstellationMatch);

	GameMsg_C2S_ConstellationMatch(void);
	~GameMsg_C2S_ConstellationMatch(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned short m_nCardIndex;
};

class GameMsg_S2C_ConstellationMatchSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_ConstellationMatchSuc(void);
	~GameMsg_S2C_ConstellationMatchSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	unsigned short m_nLevel;
	unsigned char m_nConstellation;
	unsigned char m_nMatchRate;
	std::string m_strWishWords;
	unsigned short m_nSceneID;

	std::vector<ConstellationConfig> m_vecQuestion;
};

class GameMsg_S2C_ConstellationMatchFail : public GameMsg_Base
{
public:
	GameMsg_S2C_ConstellationMatchFail(void);
	~GameMsg_S2C_ConstellationMatchFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_EnterConstellationRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterConstellationRoom);

	GameMsg_C2S_EnterConstellationRoom(void);
	~GameMsg_C2S_EnterConstellationRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
};

///////////////////////////////////////////////////////
class GameMsg_C2S_SendQuestionAnswer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendQuestionAnswer);

	GameMsg_C2S_SendQuestionAnswer(void);
	~GameMsg_C2S_SendQuestionAnswer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::list<unsigned char> m_listAnswer;
};

#endif

