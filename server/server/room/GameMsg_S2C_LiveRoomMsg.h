#ifndef __GAMEMSG_S2C_LIVEROOM_MSG_H__
#define __GAMEMSG_S2C_LIVEROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
// #include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomDataStruct.h"
#include "GameMsg_LiveRoom.h"
#include <list>
#include <string>

class LiveRoomAudienceInfo
{
public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	char m_nRoleSex;
	bool m_bIsVIP;
	unsigned short m_nVIPLevel;

	LiveRoomAudienceInfo()
	{
		m_nRoleID = 0;
		m_strRoleName = "";
		m_nRoleSex = ESexType_No;
		m_bIsVIP = false;
		m_nVIPLevel = 0;
	}

	void doEncode(CParamPool& IOBuff)
	{
		IOBuff.AddUInt( m_nRoleID );
		IOBuff.AddStringW( m_strRoleName.c_str() );
		IOBuff.AddUChar( m_nRoleSex );
		IOBuff.AddBool( m_bIsVIP );
		IOBuff.AddUShort( m_nVIPLevel );
	}
};

class LiveRoomBriefInfo
{
public:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	char m_chState;
	char m_chType;
	unsigned short m_nAudienceCount;
	std::string m_strLiveRoomDesc;
	std::list<std::string> m_listPlayerName;

	LiveRoomBriefInfo()
	{
		m_nRoomID = 0;
		m_strRoomName = "";
		m_chState = 0;
		m_chType = 0;
		m_nAudienceCount = 0;
	}

	void doEncode(CParamPool& IOBuff)
	{
		IOBuff.AddUInt( m_nRoomID );
		IOBuff.AddStringW( m_strRoomName.c_str() );
		IOBuff.AddUChar( m_chState );
		IOBuff.AddUChar( m_chType );
		IOBuff.AddUShort(m_nAudienceCount);
		IOBuff.AddStringW(m_strLiveRoomDesc.c_str());

		IOBuff.AddUShort((unsigned short)m_listPlayerName.size());
		for (std::list<std::string>::iterator it = m_listPlayerName.begin(); 
			it != m_listPlayerName.end(); ++it)
		{
			IOBuff.AddStringW(it->c_str());
		}
	}
};


class LiveRoomWholeInfo
{
public:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	char m_chRoomType;
	char m_chState;
	char m_chScene;
	short m_nMusic;
	char m_chMode;
	char m_chLevel;
	bool m_bVote;
	bool m_bDefaultCloth;
	std::string m_strLiveRoomDesc;
	bool m_bSendAudience;
	char m_chRoomRoleType;

	std::list<RoomPlayerInfo> m_listDancer;
	std::list<RoomPlayerInfo> m_listGM;
	std::list<LiveRoomAudienceInfo> m_listAudience;

	LiveRoomWholeInfo()
	{
		m_nRoomID = 0;
		m_strRoomName = "";
		m_chRoomType = eRoomType_Unknown;
		m_chState = eRoomState_Idle;
		m_chScene = RAN_SCENE_ID;
		m_nMusic = RAN_MUSIC_ID;
		m_chMode = MusicMode_None;
		m_chLevel = MusicLevel_None;
		m_bVote = false;
		m_bDefaultCloth = false;
		m_bSendAudience = false;
		m_chRoomRoleType = 0;
	}

	void doEncode(CParamPool& IOBuff)
	{
		IOBuff.AddUInt( m_nRoomID );
		IOBuff.AddStringW( m_strRoomName.c_str() );
		IOBuff.AddUChar( m_chRoomType );
		IOBuff.AddUChar( m_chState );
		IOBuff.AddUChar( m_chScene );
		IOBuff.AddUShort( m_nMusic );
		IOBuff.AddUChar( m_chMode );
		IOBuff.AddUChar( m_chLevel );
		IOBuff.AddBool( m_bVote );
		IOBuff.AddBool( m_bDefaultCloth );
		//IOBuff.AddStringW(m_strLiveRoomDesc.c_str());
		IOBuff.AddUChar(m_chRoomRoleType);

		IOBuff.AddUShort( (unsigned short)m_listDancer.size() );
		for ( std::list<RoomPlayerInfo>::iterator it = m_listDancer.begin(); 
			it != m_listDancer.end(); ++it )
		{
			(*it).doEncode( IOBuff );
		}

		IOBuff.AddUShort( (unsigned short)m_listGM.size() );
		for ( std::list<RoomPlayerInfo>::iterator it = m_listGM.begin(); 
			it != m_listGM.end(); ++it )
		{
			(*it).doEncode( IOBuff );
		}

		IOBuff.AddBool( m_bSendAudience );
		if (m_bSendAudience)
		{
			IOBuff.AddUShort( (unsigned short)m_listAudience.size() );
			for ( std::list<LiveRoomAudienceInfo>::iterator it = m_listAudience.begin(); 
				it != m_listAudience.end(); ++it )
			{
				(*it).doEncode( IOBuff );
			}
		}
	}
};



///////////////////////////////////////////////////////
class GameMsg_S2C_EnterLiveLobbyRes : public GameMsg_Base
{
public:
	GameMsg_S2C_EnterLiveLobbyRes(void);
	~GameMsg_S2C_EnterLiveLobbyRes(void);

	bool virtual doEncode(CParamPool& IOBuff);

public:
	bool m_bCanCreateLiveRoom;
	std::string m_strTitle;
	std::string m_strRule;
	std::list<itemtype_t> m_listMaleDefaultCloth;
	std::list<itemtype_t> m_listFemaleDefaultCloth;
	std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_GetLiveRoomListResult : public GameMsg_Base
{
public:
	GameMsg_S2C_GetLiveRoomListResult(void);
	~GameMsg_S2C_GetLiveRoomListResult(void);

	bool virtual doEncode(CParamPool& IOBuff);

public:
	unsigned short m_nCurPage;
	unsigned short m_nRoomTotalNum;

	std::vector<LiveRoomBriefInfo> m_vecRoomInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateLiveRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_CreateLiveRoomSuc(void);
	~GameMsg_S2C_CreateLiveRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	LiveRoomWholeInfo m_RoomInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateLiveRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_CreateLiveRoomFail(void);
	~GameMsg_S2C_CreateLiveRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_EnterLiveRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_EnterLiveRoomFail(void);
	~GameMsg_S2C_EnterLiveRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_LiveRoomEnterPlayer : public GameMsg_Base
{
public:
	GameMsg_S2C_LiveRoomEnterPlayer(void);
	~GameMsg_S2C_LiveRoomEnterPlayer(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	RoomPlayerInfo m_PlayerInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_LiveRoomEnterAudience : public GameMsg_Base
{
public:
	GameMsg_S2C_LiveRoomEnterAudience(void);
	~GameMsg_S2C_LiveRoomEnterAudience(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	LiveRoomAudienceInfo m_AudienceInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitLiveRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_QuitLiveRoomSuc(void);
	~GameMsg_S2C_QuitLiveRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_chRoomRoleType;
	char m_chRoomRolePos;
	unsigned int m_nRoleID;
	char m_chQuitType;
	char m_chQuitTo;
	unsigned int m_nRoleIDKicked;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_QuitLiveRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_QuitLiveRoomFail(void);
	~GameMsg_S2C_QuitLiveRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};



///////////////////////////////////////////////////////
class GameMsg_S2C_CloseLiveRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_CloseLiveRoomSuc(void);
	~GameMsg_S2C_CloseLiveRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	int m_nCloseCountDownTime;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_CloseLiveRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_CloseLiveRoomFail(void);
	~GameMsg_S2C_CloseLiveRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_SetLiveRoomDefaultClothRes : public GameMsg_Base
{
public:
	GameMsg_S2C_SetLiveRoomDefaultClothRes(void);
	~GameMsg_S2C_SetLiveRoomDefaultClothRes(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	bool m_bSet;
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_GetLiveRoomScoreRes : public GameMsg_Base
{
public:
	GameMsg_S2C_GetLiveRoomScoreRes(void);
	~GameMsg_S2C_GetLiveRoomScoreRes(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned short m_nCurrPage;
	unsigned short m_nTotalPage;
	CLiveRoomRoundDancerScore m_score;
};

class LiveRoomPlayerInfo
{
public:
	LiveRoomPlayerInfo()
	{
		m_chRoomRoleType = 0;
		m_bSendAudience = false;
	}

	void doEncode(CParamPool& IOBuff)
	{
		IOBuff.AddUChar(m_chRoomRoleType);

		IOBuff.AddUShort( (unsigned short)m_listDancer.size() );
		for ( std::list<RoomPlayerInfo>::iterator it = m_listDancer.begin(); 
			it != m_listDancer.end(); ++it )
		{
			(*it).doEncode( IOBuff );
		}

		IOBuff.AddBool( m_bSendAudience );

		if (m_bSendAudience)
		{
			IOBuff.AddUShort( (unsigned short)m_listAudience.size() );
			for ( std::list<LiveRoomAudienceInfo>::iterator it = m_listAudience.begin(); 
				it != m_listAudience.end(); ++it )
			{
				(*it).doEncode( IOBuff );
			}
		}
	}

public:
	char m_chRoomRoleType;
	bool m_bSendAudience;
	std::list<RoomPlayerInfo> m_listDancer;
	std::list<LiveRoomAudienceInfo> m_listAudience;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_UpdateLiveRoomDancerSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateLiveRoomDancerSuc(void);
	~GameMsg_S2C_UpdateLiveRoomDancerSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	LiveRoomPlayerInfo m_info;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_UpdateLiveRoomDancerFail : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateLiveRoomDancerFail(void);
	~GameMsg_S2C_UpdateLiveRoomDancerFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_SetLiveRoomDancerToAudienceFail : public GameMsg_Base
{
public:
	GameMsg_S2C_SetLiveRoomDancerToAudienceFail(void);
	~GameMsg_S2C_SetLiveRoomDancerToAudienceFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_OpenVoteSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_OpenVoteSuc(void);
	~GameMsg_S2C_OpenVoteSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	bool m_bOpen;
};

class GameMsg_S2C_OpenVoteFail : public GameMsg_Base
{
public:
	GameMsg_S2C_OpenVoteFail(void);
	~GameMsg_S2C_OpenVoteFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_VoteSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_VoteSuc(void);
	~GameMsg_S2C_VoteSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
};

class GameMsg_S2C_VoteFail : public GameMsg_Base
{
public:
	GameMsg_S2C_VoteFail(void);
	~GameMsg_S2C_VoteFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////
class LiveRoomDancerVoteInfo
{
public:
	LiveRoomDancerVoteInfo()
	{
		m_nRoleID = 0;
		m_nHot = 0;
	}

	void doEncode(CParamPool &ioBuff)
	{
		ioBuff.AddUInt(m_nRoleID);
		ioBuff.AddInt(m_nHot);
	}

public:
	unsigned int m_nRoleID;
	int m_nHot;
};

class GameMsg_S2C_DancerVoteInfo : public GameMsg_Base
{
public:
	GameMsg_S2C_DancerVoteInfo(void);
	~GameMsg_S2C_DancerVoteInfo(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::list<LiveRoomDancerVoteInfo> m_hotList;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_LiveRoomJoinAuditionFail : public GameMsg_Base
{
public:
	GameMsg_S2C_LiveRoomJoinAuditionFail(void);
	~GameMsg_S2C_LiveRoomJoinAuditionFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
    unsigned int m_nLeftPunishedTime;

};


//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_LiveRoomPrepareAudition : public GameMsg_Base
{
public:
	GameMsg_S2C_LiveRoomPrepareAudition(void);
	~GameMsg_S2C_LiveRoomPrepareAudition(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	RoomWholeInfo m_roomInfo;
	char m_chScenceID;
	short m_nMusicID;
	char m_chMode;
	char m_chLevel;
	std::string m_strCheckKey;
	std::vector<char> m_vecStage;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_SyncMyAuditionScore : public GameMsg_Base
{
public:
	GameMsg_S2C_SyncMyAuditionScore(void);
	~GameMsg_S2C_SyncMyAuditionScore(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	int m_nTotalScore;
	int m_nScore;
};

#endif


// end of file


