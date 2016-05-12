#include "GameMsg_LiveRoom.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetLiveRoomHotList::GameMsg_C2S_GetLiveRoomHotList()
: GameMsg_Base(MSG_C2S_GetLiveRoomHotList)
{
	m_nListType = eLiveRoomScoreList_Invalid;
	m_nPage = 0;
}

GameMsg_C2S_GetLiveRoomHotList::~GameMsg_C2S_GetLiveRoomHotList()
{
}

bool GameMsg_C2S_GetLiveRoomHotList::doDecode(CParamPool& IOBuff)
{
	m_nListType = IOBuff.GetUChar();
	m_nPage = IOBuff.GetUShort();
	return true;
}
bool GameMsg_C2S_GetLiveRoomHotList::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar(m_nListType);
	IOBuff.AddUShort(m_nPage);
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetLiveRoomHotListRes::GameMsg_S2C_GetLiveRoomHotListRes(void)
: GameMsg_Base(MSG_S2C_GetLiveRoomHotListRes)
{
	m_nCurPage = 0;
	m_nTotalPage = 0;
}
GameMsg_S2C_GetLiveRoomHotListRes::~GameMsg_S2C_GetLiveRoomHotListRes(void)
{
}

bool GameMsg_S2C_GetLiveRoomHotListRes::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort(m_nCurPage);
	IOBuff.AddUShort(m_nTotalPage);

	IOBuff.AddUShort((unsigned short)m_liveRoomHotList.size());
	for (std::list<LiveRoomHotInfo>::iterator i = m_liveRoomHotList.begin(); i != m_liveRoomHotList.end(); ++i)
	{
		i->doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_S2C_GetLiveRoomHotListRes::doDecode(CParamPool &IOBuff)
{
	m_nCurPage = IOBuff.GetUShort();
	m_nTotalPage = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nCount; ++i)
	{
		LiveRoomHotInfo info;
		info.doDecode(IOBuff);
		m_liveRoomHotList.push_back(info);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetLiveRoomAuditionScoreList::GameMsg_C2S_GetLiveRoomAuditionScoreList()
: GameMsg_Base(MSG_C2S_GetLiveRoomAuditionScoreList)
{
	m_nPage = 0;
	m_bIsFirstOpen = false;
}

GameMsg_C2S_GetLiveRoomAuditionScoreList::~GameMsg_C2S_GetLiveRoomAuditionScoreList()
{
}

bool GameMsg_C2S_GetLiveRoomAuditionScoreList::doDecode(CParamPool& IOBuff)
{
	m_nPage = IOBuff.GetUShort();
	m_bIsFirstOpen = IOBuff.GetBool();
	return true;
}


bool GameMsg_C2S_GetLiveRoomAuditionScoreList::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort(m_nPage);
	IOBuff.AddBool(m_bIsFirstOpen);
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetLiveRoomAuditionScoreListRes::GameMsg_S2C_GetLiveRoomAuditionScoreListRes(void)
: GameMsg_Base(MSG_S2C_GetLiveRoomAuditionScoreListRes)
{
	m_nCurPage = 0;
	m_nTotalPage = 0;
}
GameMsg_S2C_GetLiveRoomAuditionScoreListRes::~GameMsg_S2C_GetLiveRoomAuditionScoreListRes(void)
{
}

bool GameMsg_S2C_GetLiveRoomAuditionScoreListRes::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort(m_nCurPage);
	IOBuff.AddUShort(m_nTotalPage);

	IOBuff.AddUShort((unsigned short)m_liveRoomScoreList.size());
	for (std::list<LiveRoomAuditionScoreInfo>::iterator i = m_liveRoomScoreList.begin(); i != m_liveRoomScoreList.end(); ++i)
	{
		i->doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_S2C_GetLiveRoomAuditionScoreListRes::doDecode(CParamPool &IOBuff)
{
	m_nCurPage = IOBuff.GetUShort();
	m_nTotalPage = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nCount; ++i)
	{
		LiveRoomAuditionScoreInfo info;
		info.doDecode(IOBuff);
		m_liveRoomScoreList.push_back(info);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_UpdateDancerHot::GameMsg_S2G_UpdateDancerHot()
: GameMsg_Base(MSG_S2G_LiveRoom_UpdateDancerHot)
{
	m_nRoleID = 0;
	m_nAdded = 0;
}

GameMsg_S2G_UpdateDancerHot::~GameMsg_S2G_UpdateDancerHot()
{
}

bool GameMsg_S2G_UpdateDancerHot::doDecode(CParamPool& IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_strRoleName = IOBuff.GetString();
	m_nAdded = IOBuff.GetInt();
	return true;
}
bool GameMsg_S2G_UpdateDancerHot::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddString(m_strRoleName.c_str());
	IOBuff.AddInt(m_nAdded);
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_UpdateDancerPoint::GameMsg_S2G_UpdateDancerPoint()
: GameMsg_Base(MSG_S2G_LiveRoom_UpdateDancerPoint)
{
	m_nRoleID = 0;
	m_nAdded = 0;
}

GameMsg_S2G_UpdateDancerPoint::~GameMsg_S2G_UpdateDancerPoint()
{
}

bool GameMsg_S2G_UpdateDancerPoint::doDecode(CParamPool& IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_strRoleName = IOBuff.GetString();
	m_nAdded = IOBuff.GetInt();
	return true;
}
bool GameMsg_S2G_UpdateDancerPoint::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddString(m_strRoleName.c_str());
	IOBuff.AddInt(m_nAdded);
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_AddPunisherPlayer::GameMsg_S2G_AddPunisherPlayer()
    : GameMsg_Base(MSG_S2G_LiveRoom_AddPunishedPlayer)
    ,m_nRoleID(0)
    ,m_nPunishTime(0)
{
}

GameMsg_S2G_AddPunisherPlayer::~GameMsg_S2G_AddPunisherPlayer()
{
}

bool GameMsg_S2G_AddPunisherPlayer::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nPunishTime = IOBuff.GetUInt();
    return true;
}
bool GameMsg_S2G_AddPunisherPlayer::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nPunishTime);
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_IsPunisherPlayer::GameMsg_S2G_IsPunisherPlayer()
    : GameMsg_Base(MSG_S2G_LiveRoom_IsPunishedPlayer)
    ,m_nRoleID(0)
    ,m_cPhoneOS(0)
    ,m_nLine(0)
{
}

GameMsg_S2G_IsPunisherPlayer::~GameMsg_S2G_IsPunisherPlayer()
{
}

bool GameMsg_S2G_IsPunisherPlayer::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_cPhoneOS = (char)IOBuff.GetUInt();
    m_nLine = IOBuff.GetUInt();
    return true;
}
bool GameMsg_S2G_IsPunisherPlayer::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_cPhoneOS);
    IOBuff.AddUInt(m_nLine);
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_G2S_IsPunisherPlayer::GameMsg_G2S_IsPunisherPlayer()
    : GameMsg_Base(MSG_G2S_LiveRoom_IsPunishedPlayer)
    ,m_nRoleID(0)
    ,m_chPhoneOS(0)
    ,m_nLeftPunishTime(0)
{
}

GameMsg_G2S_IsPunisherPlayer::~GameMsg_G2S_IsPunisherPlayer()
{
}

bool GameMsg_G2S_IsPunisherPlayer::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_chPhoneOS = IOBuff.GetUChar();
    m_nLeftPunishTime = IOBuff.GetUInt();
    return true;
}
bool GameMsg_G2S_IsPunisherPlayer::doEncode(CParamPool& IOBuff)
{
    
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar(m_chPhoneOS);
    IOBuff.AddUInt(m_nLeftPunishTime);
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_SyncAuditionScore::GameMsg_S2G_SyncAuditionScore()
    : GameMsg_Base(MSG_S2G_LiveRoom_SyncAuditionScore)
    ,m_nRoleID(0)
    ,m_nScore(0)
{
}

GameMsg_S2G_SyncAuditionScore::~GameMsg_S2G_SyncAuditionScore()
{
}

bool GameMsg_S2G_SyncAuditionScore::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nScore = IOBuff.GetInt();
    return true;
}
bool GameMsg_S2G_SyncAuditionScore::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nScore);
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_G2S_SyncAuditionScore::GameMsg_G2S_SyncAuditionScore()
    : GameMsg_Base(MSG_G2S_LiveRoom_SyncAuditionScore)
    ,m_nRoleID(0)
    ,m_nScore(0)
{
}

GameMsg_G2S_SyncAuditionScore::~GameMsg_G2S_SyncAuditionScore()
{
}

bool GameMsg_G2S_SyncAuditionScore::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nScore = IOBuff.GetInt();
    return true;
}
bool GameMsg_G2S_SyncAuditionScore::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nScore);
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetMyAuditionScore::GameMsg_C2S_GetMyAuditionScore()
: GameMsg_Base(MSG_C2S_GetMyAuditionScore)
{
}

GameMsg_C2S_GetMyAuditionScore::~GameMsg_C2S_GetMyAuditionScore()
{
}

bool GameMsg_C2S_GetMyAuditionScore::doEncode(CParamPool& IOBuff)
{
	return true;
}

bool GameMsg_C2S_GetMyAuditionScore::doDecode(CParamPool& IOBuff)
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetMyAuditionScore::GameMsg_S2C_GetMyAuditionScore(void)
: GameMsg_Base(MSG_S2C_GetMyAuditionScore)
{
	m_nScore = 0;
	m_nPlace = 0;
	m_nRefreshTime = 0;
}
GameMsg_S2C_GetMyAuditionScore::~GameMsg_S2C_GetMyAuditionScore(void)
{
}

bool GameMsg_S2C_GetMyAuditionScore::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUInt(m_nScore);
	IOBuff.AddInt(m_nPlace);
	IOBuff.AddUInt(m_nRefreshTime);

	return true;
}

bool GameMsg_S2C_GetMyAuditionScore::doDecode(CParamPool& IOBuff)
{
	m_nScore = IOBuff.GetUInt();
	m_nPlace = IOBuff.GetInt();
	m_nRefreshTime = IOBuff.GetUInt();

	return true;
}

