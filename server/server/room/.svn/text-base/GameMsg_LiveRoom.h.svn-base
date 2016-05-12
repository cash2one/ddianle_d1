#ifndef	__GAMEMSG_GROUP_LIVE_ROOM_H__
#define	__GAMEMSG_GROUP_LIVE_ROOM_H__

#include "../../socket/GameMsg_Base.h"
#include <map>
#include <list>
#include <string>

enum eLiveRoomScoreList
{ 
	eLiveRoomScoreList_Invalid, 
	eLiveRoomScoreList_WholeServer,
	eLiveRoomScoreList_LocalRoom,
	eLiveRoomScoreList_LocalLine,
	eLiveRoomScoreList_Max,
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetLiveRoomHotList : public GameMsg_Base
{
public:
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLiveRoomHotList);

	GameMsg_C2S_GetLiveRoomHotList(void);
	~GameMsg_C2S_GetLiveRoomHotList(void);

	virtual bool doDecode(CParamPool& IOBuff);
	virtual bool doEncode(CParamPool& IOBuff);


public:
	unsigned char m_nListType;
	unsigned short m_nPage;
};

//////////////////////////////////////////////////////////////////////////
class LiveRoomHotInfo
{
public:
	LiveRoomHotInfo()
	{
		m_nRoleID = 0;
		m_nPlace = 0;
		m_nHot = 0;
	}

	void doEncode(CParamPool &ioBuff)
	{
		ioBuff.AddUInt(m_nRoleID);
		ioBuff.AddStringW(m_strRoleName.c_str());
		ioBuff.AddUShort(m_nPlace);
		ioBuff.AddUInt(m_nHot);
	}

	void doDecode(CParamPool &ioBuff)
	{
		m_nRoleID = ioBuff.GetUInt();
		ioBuff.GetStringW(m_strRoleName);
		m_nPlace = ioBuff.GetUShort();
		m_nHot = ioBuff.GetUInt();
	}


public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	unsigned short m_nPlace;
	unsigned int m_nHot;
};

class GameMsg_S2C_GetLiveRoomHotListRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLiveRoomHotListRes);

	GameMsg_S2C_GetLiveRoomHotListRes();
	~GameMsg_S2C_GetLiveRoomHotListRes();

public:
	bool doEncode(CParamPool& IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nCurPage;
	unsigned short m_nTotalPage;
	std::list<LiveRoomHotInfo> m_liveRoomHotList;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetLiveRoomAuditionScoreList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLiveRoomAuditionScoreList);

	GameMsg_C2S_GetLiveRoomAuditionScoreList(void);
	~GameMsg_C2S_GetLiveRoomAuditionScoreList(void);

	virtual bool doDecode(CParamPool& IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nPage;
	bool m_bIsFirstOpen;
};


//////////////////////////////////////////////////////////////////////////
class LiveRoomAuditionScoreInfo
{
public:
	enum LiveRoomAuditionScoreTide
	{
		NoChange,
		Up,
		Down,
	};

public:
	LiveRoomAuditionScoreInfo()
	{
		m_nRoleID = 0;
		m_nPlace = 0;
		m_nScore = 0;
		m_nTide = NoChange;
	}

	void doEncode(CParamPool &ioBuff)
	{
		ioBuff.AddUInt(m_nRoleID);
		ioBuff.AddStringW(m_strRoleName.c_str());
		ioBuff.AddUShort(m_nPlace);
		ioBuff.AddUInt(m_nScore);
		ioBuff.AddUChar(m_nTide);
	}
	void doDecode(CParamPool &ioBuff)
	{
		m_nRoleID = ioBuff.GetUInt();
		ioBuff.GetStringW(m_strRoleName);
		m_nPlace = ioBuff.GetUShort();
		m_nScore = ioBuff.GetUInt();
		m_nTide = ioBuff.GetUChar();
	}

public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	unsigned short m_nPlace;
	unsigned int m_nScore;
	char m_nTide;
};

class GameMsg_S2C_GetLiveRoomAuditionScoreListRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLiveRoomAuditionScoreListRes);

	GameMsg_S2C_GetLiveRoomAuditionScoreListRes();
	~GameMsg_S2C_GetLiveRoomAuditionScoreListRes();

public:
	bool doEncode(CParamPool& IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nCurPage;
	unsigned short m_nTotalPage;
	std::list<LiveRoomAuditionScoreInfo> m_liveRoomScoreList;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_UpdateDancerHot : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateDancerHot);

	GameMsg_S2G_UpdateDancerHot(void);
	~GameMsg_S2G_UpdateDancerHot(void);

	virtual bool doDecode(CParamPool& IOBuff);
	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	int m_nAdded;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_UpdateDancerPoint : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateDancerPoint);

	GameMsg_S2G_UpdateDancerPoint(void);
	~GameMsg_S2G_UpdateDancerPoint(void);

	virtual bool doDecode(CParamPool& IOBuff);
	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	int m_nAdded;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_AddPunisherPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddPunisherPlayer);

    GameMsg_S2G_AddPunisherPlayer(void);
    ~GameMsg_S2G_AddPunisherPlayer(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    unsigned int m_nPunishTime;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_IsPunisherPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_IsPunisherPlayer);

    GameMsg_S2G_IsPunisherPlayer(void);
    ~GameMsg_S2G_IsPunisherPlayer(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    char         m_cPhoneOS;
    int          m_nLine;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_G2S_IsPunisherPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_IsPunisherPlayer);

    GameMsg_G2S_IsPunisherPlayer(void);
    ~GameMsg_G2S_IsPunisherPlayer(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    char         m_chPhoneOS;
    unsigned int m_nLeftPunishTime;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_SyncAuditionScore : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_SyncAuditionScore);

    GameMsg_S2G_SyncAuditionScore(void);
    ~GameMsg_S2G_SyncAuditionScore(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    int          m_nScore;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_G2S_SyncAuditionScore : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_SyncAuditionScore);

    GameMsg_G2S_SyncAuditionScore(void);
    ~GameMsg_G2S_SyncAuditionScore(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    int          m_nScore;
};


//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetMyAuditionScore : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMyAuditionScore);

	GameMsg_C2S_GetMyAuditionScore(void);
	~GameMsg_C2S_GetMyAuditionScore(void);

	virtual bool doDecode(CParamPool& IOBuff);
	virtual bool doEncode(CParamPool& IOBuff);

public:
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetMyAuditionScore : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMyAuditionScore);

	GameMsg_S2C_GetMyAuditionScore();
	~GameMsg_S2C_GetMyAuditionScore();

public:
	bool doEncode(CParamPool& IOBuff);
	bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nScore;
	int m_nPlace;
	unsigned int m_nRefreshTime;
};

#endif //__GAMEMSG_GROUP_LIVE_ROOM_H__

