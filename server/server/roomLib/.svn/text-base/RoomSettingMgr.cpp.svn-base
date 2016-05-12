#include "RoomSettingMgr.h"
#include "../../datastructure/Macro_Define.h"

RoomSettingMgr::RoomSettingMgr()
    : m_nDefaultScene(3)
    , m_nDefaultMode(MusicMode_Osu)
    , m_nDefaultLevel(MusicLevel_Easy)
    , m_nRoomStartWait(30)
    , m_nRoomEndWait(30)
    , m_nAutoMatchCount(2)
    , m_nAutoMatchInterval(15)
    , m_nMinLevelLiveRoomRequired(0)
    , m_nLiveRoomMaxAudienceCount(0)
    , m_nLiveRoomMaxGMCount(0)
    , m_nLiveRoomCloseTime(0)
    , m_nLiveRoomCountDownTime(0)
{

}

RoomSettingMgr::~RoomSettingMgr()
{

}

RoomSettingMgr& RoomSettingMgr::Instance()
{
    static RoomSettingMgr s;
    return s;
}

void RoomSettingMgr::RoomKickSetInfo(int nRoomStartWait, int nRoomEndWait)
{
    m_nRoomStartWait = nRoomStartWait;
    m_nRoomEndWait = nRoomEndWait;
}

int RoomSettingMgr::RoomStartWait() const
{
    return m_nRoomStartWait;
}

int RoomSettingMgr::RoomEndWait() const
{
    return m_nRoomEndWait;
}

void RoomSettingMgr::RoomMatchSetInfo(int nAutoMatchCount, int nAutoMatchInterval)
{
    m_nAutoMatchCount = nAutoMatchCount;
    m_nAutoMatchInterval = nAutoMatchInterval;
}

int RoomSettingMgr::AutoMatchCount() const
{
    return m_nAutoMatchCount;
}

int RoomSettingMgr::AutoMatchInterval() const
{
    return m_nAutoMatchInterval;
}

void RoomSettingMgr::RoomMusicSetInfo(int nDefaultScene, int nDefaultMode, int nDefaultLevel)
{
    m_nDefaultScene = nDefaultScene;
    m_nDefaultMode = nDefaultMode;
    m_nDefaultLevel = nDefaultLevel;
}

int RoomSettingMgr::RoomDefaultScene() const
{
    return m_nDefaultScene;
}

int RoomSettingMgr::RoomDefaultMode() const
{
    return m_nDefaultMode;
}

int RoomSettingMgr::RoomDefaultLevel() const
{
    return m_nDefaultLevel;
}


void RoomSettingMgr::LoadLiveRoomConfig(int nMinRoleLevel, int nMaxAudienceCount,
    int nMaxGMCount, int nCloseTime, int nCountDownTime)
{
    m_nMinLevelLiveRoomRequired = nMinRoleLevel;
    m_nLiveRoomMaxAudienceCount = nMaxAudienceCount;
    m_nLiveRoomMaxGMCount = nMaxGMCount;
    m_nLiveRoomCloseTime = nCloseTime;
    m_nLiveRoomCountDownTime = nCountDownTime;
}

int RoomSettingMgr::LiveRoomMinRoleLevel()
{
    return m_nMinLevelLiveRoomRequired;
}

void RoomSettingMgr::SetLiveRoomMinRoleLevel(int nValue)
{
    m_nMinLevelLiveRoomRequired = nValue;
}

int RoomSettingMgr::LiveRoomMaxAudienceCount()
{
    return m_nLiveRoomMaxAudienceCount;
}

void RoomSettingMgr::SetLiveRoomMaxAudienceCount(int nValue)
{
    m_nLiveRoomMaxAudienceCount = nValue;
}

int RoomSettingMgr::LiveRoomMaxGMCount()
{
    return m_nLiveRoomMaxGMCount;
}

int RoomSettingMgr::LiveRoomCloseTime()
{
    return m_nLiveRoomCloseTime;
}

void RoomSettingMgr::SetLiveRoomCloseTime(int nValue)
{
    m_nLiveRoomCloseTime = nValue;
}

int RoomSettingMgr::LiveRoomCountDownTime()
{
    return m_nLiveRoomCountDownTime;
}

void RoomSettingMgr::SetLiveRoomCountDownTime(int nValue)
{
    m_nLiveRoomCountDownTime = nValue;
}
