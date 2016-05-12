#ifndef __LIBSERVERFRAME_ROOMSETTINGMGR_H__
#define __LIBSERVERFRAME_ROOMSETTINGMGR_H__

class RoomSettingMgr
{
protected:
    RoomSettingMgr();

public:
    ~RoomSettingMgr();

    static RoomSettingMgr& Instance();

    void RoomKickSetInfo(int nRoomStartWait, int nRoomEndWait);
    int RoomStartWait() const;
    int RoomEndWait() const;

    void RoomMatchSetInfo(int nAutoMatchCount, int nAutoMatchInterval);
    int AutoMatchCount() const;
    int AutoMatchInterval() const;

    void RoomMusicSetInfo(int nDefaultScene, int nDefaultMode, int nDefaultLevel);
    int RoomDefaultScene() const;
    int RoomDefaultMode() const;
    int RoomDefaultLevel() const;


    // 直播房间相关配置
    void LoadLiveRoomConfig(int nMinRoleLevel, int nMaxAudienceCount, int nMaxGMCount,
        int nCloseTime, int nCountDownTime);

    int LiveRoomMinRoleLevel();
    void SetLiveRoomMinRoleLevel(int nValue);
    int LiveRoomMaxAudienceCount();
    void SetLiveRoomMaxAudienceCount(int nValue);
    int LiveRoomMaxGMCount();
    int LiveRoomCloseTime();
    void SetLiveRoomCloseTime(int nValue);
    int LiveRoomCountDownTime();
    void SetLiveRoomCountDownTime(int nValue);

public:
    // room
    int     m_nDefaultScene;
    int     m_nDefaultMode;
    int     m_nDefaultLevel;
    int     m_nRoomStartWait;
    int     m_nRoomEndWait;
    int     m_nAutoMatchCount;
    int     m_nAutoMatchInterval;

    // live room
    int     m_nMinLevelLiveRoomRequired;
    int     m_nLiveRoomMaxAudienceCount;
    int     m_nLiveRoomMaxGMCount;
    int     m_nLiveRoomCloseTime;
    int     m_nLiveRoomCountDownTime;
};



#endif // __LIBSERVERFRAME_ROOMSETTINGMGR_H__