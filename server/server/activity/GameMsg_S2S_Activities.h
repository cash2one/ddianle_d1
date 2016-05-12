#ifndef		__GAMEMSG_S2S_ACTIVITIES_H__
#define		__GAMEMSG_S2S_ACTIVITIES_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;


class GameMsg_S2G_BlessFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_BlessFriend);
	GameMsg_S2G_BlessFriend();
	~GameMsg_S2G_BlessFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSenderID;
	unsigned int m_nRcverID;
	std::string	 m_strSenderName;
	unsigned short m_nWordsIndex;
	unsigned int m_nBlessTime;
};

///////////////////////////////////////////////////////
class GameMsg_G2S_BlessFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_BlessFriend);
	GameMsg_G2S_BlessFriend();
	~GameMsg_G2S_BlessFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSenderID;
	unsigned int m_nRcverID;
	std::string	 m_strSenderName;
	unsigned short m_nWordsIndex;
	unsigned int m_nBlessTime;
};

class GameMsg_S2G_AddLongactingRechargeBoardCast:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddLongactingRechargeBoardCast);
    GameMsg_S2G_AddLongactingRechargeBoardCast();
    ~GameMsg_S2G_AddLongactingRechargeBoardCast();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    LongactingCumulativeRechargeBoardCast   m_BoardCast;
};

class GameMsg_S2G_GetLongactingRechargeBoardCast:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetLongactingRechargeBoardCast);
    GameMsg_S2G_GetLongactingRechargeBoardCast();
    ~GameMsg_S2G_GetLongactingRechargeBoardCast();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int                                        m_nRoleID;
};

class GameMsg_G2S_GetLongactingRechargeBoardCast:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetLongactingRechargeBoardCast);
    GameMsg_G2S_GetLongactingRechargeBoardCast();
    ~GameMsg_G2S_GetLongactingRechargeBoardCast();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    std::list<LongactingCumulativeRechargeBoardCast>    m_listLongactingBoardCast;
};

#endif








