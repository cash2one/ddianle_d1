#ifndef __LIBSERVERFRAME_ROOMDANCEINFO_H__
#define __LIBSERVERFRAME_ROOMDANCEINFO_H__

#include <vector>
#include "../Macros/Macros.h"
#include "RoomMarkInfo.h"
#include "RoomDataStruct.h"


class LWDanceRoleInfo
{
public:
    LWDanceRoleInfo(unsigned int nRoleID, ERoleRoomState eRoleRoomState);
    ~LWDanceRoleInfo();

public:
    bool RecvRoundMark(int nMode, int nRound, unsigned int nMark,
        char nKeyRank, const std::vector<int>& rRoundRank);
    void ClearMark();

    void SumTotalMark(int nRank, int nRankMark);
    void CalcGrade();

    unsigned int GetTotalScore() const;

    CLASS_PROPERTY_READONLY(unsigned int, m_nRoleID, RoleID)
//     CLASS_PROPERTY(int, m_nPosition, Position)
    CLASS_PROPERTY(ERoleRoomState, m_eState, State)
//    CLASS_BOOL_PROPERTY(m_bHost, Host)
//    CLASS_BOOL_PROPERTY(m_bDancer, Dancer)
    CLASS_PROPERTY_READONLY_BY_REF(CRoomMarkInfo, m_markInfo, MarkInfo)
        
private:
    int m_nTotalMark;
};

class LWRoomDanceInfo
{
public:
    LWRoomDanceInfo();
    ~LWRoomDanceInfo();

    void Init();
    void Clear();

    // request start dance in room.
    EStartRoomRes StartRoomDance(unsigned int nRoleID);

    // recieve client loading progress.
    void OnLoadingStartGameProgress(unsigned int nRoleID, unsigned char nRate); 

    // client load all res complete.
    void OnLoadComplete(unsigned int nRoleID);

    // client report round mark.
    void OnReportRoundMark(unsigned int nRoleID, const std::string &rMD5Code,
        unsigned int nMark, char nKeyRank, unsigned char nRound, const std::vector<char> &rRoundRank);

    // 跳舞结束
    enum EEndRoomResult{
        EEndRoomResult_Sucess = 0,              
        EEndRoomResult_Fail,                    // any reason fail
        EEndRoomResult_TooFast,                 // end room to fast, kick him off.
    };
    EEndRoomResult EndRoom(unsigned int nRoleID);

    void OnDanceStartCallBack();
    void OnDanceEndCallBack();

    void FillMusicInfo(int nPlayScene, int nPlayMusic, unsigned char nPlayMusicMode, unsigned char nPlayMusicLevel);

public:
    // 取得跳舞信息
    bool RoleMarkInfo(unsigned int nRoleID, CRoomMarkInfo& markInfo);
    bool LoadStage(std::vector<char> &vecAndroidStage, std::vector<char> &vecIosStage, std::vector<char> &vecWinPhoneStage);
    void CalcDanceResult();
    bool HasStart() const;
    bool HasDancer() const; 
    bool HasDancer(unsigned int nRoleID) const;
    LWDanceRoleInfo* GetDanceRole(unsigned int nRoleID);
    void AddRole(unsigned int nRoleID, ERoleRoomState eRoleRoomState);
    void RemoveRole(unsigned int nRoleID);
    
    void RemoveNotToStartRole(std::vector<unsigned int> &vecRole);
    void RemoveNotToEndRole(std::vector<unsigned int> &vecRole);

public:
    // Is every body ready.
    bool CheckAllRoleState(ERoleRoomState eState);
    bool IsPlaying();
    void SetAllDancerState(ERoleRoomState eState);
    void SetDancerState(unsigned int nRoleID, ERoleRoomState eState);
    ERoleRoomState GetDancerState(unsigned int nRoleID);
    bool HasLegalEndTime() const;

    void PrepareMusicAndScene();
    void SumRankMark(LWDanceRoleInfo *pDancer) const;

private:
    CLASS_PROPERTY_READONLY(ERoomState, m_eState, State)

//     CLASS_PROPERTY(int, m_nChoosedMusic, ChoosedMusic)
//     CLASS_PROPERTY(unsigned char, m_nChoosedMusicMode, ChoosedMusicMode)
//     CLASS_PROPERTY(unsigned char, m_nChoosedMusicLevel, ChoosedMusicLevel)

    CLASS_PROPERTY_READONLY(int, m_nPlayScene, PlayScene)
    CLASS_PROPERTY_READONLY(int, m_nPlayMusic, PlayMusic)
    CLASS_PROPERTY_READONLY(unsigned char, m_nPlayMusicMode, PlayMusicMode)
    CLASS_PROPERTY_READONLY(unsigned char, m_nPlayMusicLevel, PlayMusicLevel)

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strCheckCode, CheckCode)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMusicSource, MusicSource)

    CLASS_PROPERTY(unsigned int, m_nCheckTime, CheckTime)

private:
    unsigned int m_nStartTime;
    typedef std::map<unsigned int, LWDanceRoleInfo *> DancerMap;
    DancerMap m_mapDancers; // 执行跳舞的玩家
};



#endif // __LIBSERVERFRAME_ROOMDANCEINFO_H__