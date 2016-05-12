#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_G2S_GetPlayerInfo_Forward:
	public GameMsg_Base
{
public:	
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetPlayerInfo_Forward);
	GameMsg_G2S_GetPlayerInfo_Forward(unsigned int nSrcID = 0, unsigned int nQueryID = 0);
	~GameMsg_G2S_GetPlayerInfo_Forward();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcID;
	unsigned int m_nQueryID;
};

class GameMsg_G2S_GetPlayerInfo_Backward :
public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetPlayerInfo_Backward);
	GameMsg_G2S_GetPlayerInfo_Backward();
	GameMsg_G2S_GetPlayerInfo_Backward(PlayerInfo &playerInfo);
	~GameMsg_G2S_GetPlayerInfo_Backward();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	PlayerInfo m_playerInfo;
};

class GameMsg_G2S_GetOfflinePlayerInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetOfflinePlayerInfo);
    GameMsg_G2S_GetOfflinePlayerInfo();
    ~GameMsg_G2S_GetOfflinePlayerInfo();

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    EOfflinePlayerDataFrom m_eFrom;
    unsigned int      m_nSelfRoleID;
};








