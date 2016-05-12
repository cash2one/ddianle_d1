#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2G_GetPlayerInfo_Forward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetPlayerInfo_Forward);
	GameMsg_S2G_GetPlayerInfo_Forward(unsigned int nSrcID = 0, unsigned int nQueryID = 0);
	~GameMsg_S2G_GetPlayerInfo_Forward();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcID;
	unsigned int m_nQueryID;
};
 
class GameMsg_S2G_GetPlayerInfo_Backward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetPlayerInfo_Backward);
	GameMsg_S2G_GetPlayerInfo_Backward(unsigned int nSrcID = 0);
	GameMsg_S2G_GetPlayerInfo_Backward(unsigned int nSrcID, PlayerInfo &playInfo);
	~GameMsg_S2G_GetPlayerInfo_Backward();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcID;
	PlayerInfo m_playerInfo;
};

class GameMsg_S2G_GetOfflinePlayerInfo:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetOfflinePlayerInfo);
    GameMsg_S2G_GetOfflinePlayerInfo();
    ~GameMsg_S2G_GetOfflinePlayerInfo();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nRoleID;
    EOfflinePlayerDataFrom  m_eFrom;
    RoleImageOfflineData    m_OffLineData;
};


